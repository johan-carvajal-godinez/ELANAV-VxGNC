#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <taskLib.h>
#include <msgQLib.h>
#include <semLib.h>
#include <stdbool.h>
#include "usbHandler.h"
#include "usbMessageHandler.h"

/**********************************************/
/*           Variables Globales              */
/**********************************************/

MSG_Q_ID cmdQueue = NULL;           // Cola de comandos de cámara
MSG_Q_ID respQueue = NULL;          // Cola de respuestas de cámara
TASK_ID handlerTaskId = TASK_ID_NULL;       // ID de tarea principal
UVC_HANDLE cameraHandle = {-1, -1, NULL, 0}; // Estructura de control del dispositivo UVC
bool cameraOpen = false;            // Flag: dispositivo de cámara abierto
SEM_ID cameraSem = NULL;            // Semáforo para sincronización de acceso a cámara

/**********************************************/
/*                 Funciones                  */
/**********************************************/

// Procesa comandos de cámara recibidos por cola de mensajes
STATUS handleCommand(Msg* cmd, Msg* resp) {
    STATUS result = ERROR;
    
    // Preparar estructura de respuesta con datos del comando
    resp->cmd = cmd->cmd;
    resp->id = cmd->id;
    strcpy(resp->device, cmd->device);
    strcpy(resp->file, cmd->file);
    resp->frames = cmd->frames;
    
    // Sección crítica para operaciones sobre la cámara
    semTake(cameraSem, WAIT_FOREVER);
    
    switch (cmd->cmd) {
        case CMD_OPEN:
            // Abrir dispositivo de cámara
            if (cameraOpen) {
                // Si ya está abierto, cerrar primero para reinicializar
                uvcCloseDevice(&cameraHandle);
                cameraOpen = false;
            }
            // Intentar abrir el dispositivo con los parámetros especificados
            result = uvcOpenDevice(&cameraHandle, cmd->device, cmd->file);
            if (result == OK) {
                cameraOpen = true;
                printf("DKM: Camara abierta exitosamente\n");
            } else {
                printf("DKM: Error al abrir camara\n");
            }
            break;
            
        case CMD_CAPTURE:
            // Capturar un frame individual
            if (cameraOpen) {
                struct timespec ts; // Estructura para marca de tiempo
                result = uvcCaptureFrame(&cameraHandle, &ts);
                if (result == OK) {
                    printf("DKM: Frame capturado exitosamente\n");
                } else {
                    printf("DKM: Error al capturar frame\n");
                }
            } else {
                printf("DKM: Camara no esta abierta\n");
                result = ERROR;
            }
            break;
            
        case CMD_CLOSE:
            // Cerrar dispositivo de cámara
            if (cameraOpen) {
                uvcCloseDevice(&cameraHandle);
                cameraOpen = false;
                printf("DKM: Camara cerrada\n");
            }
            result = OK; // Siempre exitoso, incluso si ya estaba cerrada
            break;
            
        case CMD_RECORD:
            // Grabar múltiples frames usando función de alto nivel
            result = uvcRecord(cmd->device, cmd->frames, cmd->file);
            printf("DKM: Grabacion %s\n", (result == OK) ? "EXITOSA" : "FALLIDA");
            break;
            
        case CMD_STATUS:
            // Consultar estado actual del dispositivo
            result = cameraOpen ? OK : ERROR;
            printf("DKM: Estado de camara %s\n", cameraOpen ? "ABIERTA" : "CERRADA");
            break;
            
        default:
            printf("DKM: Comando desconocido\n");
            result = ERROR;
            break;
    }
    
    semGive(cameraSem);
    
    // Establecer resultado en la respuesta (0=éxito, 1=error)
    resp->result = (result == OK) ? 0 : 1;
    return result;
}

// Tarea principal del gestor de mensajes de cámara
void MessageHandler() {
    Msg command, response;
    
    
    
    // Bucle infinito para procesar mensajes de la cola
    while (1) {
        // Esperar comando desde la cola de comandos
        if (msgQReceive(cmdQueue, (char*)&command, sizeof(command), WAIT_FOREVER) == ERROR) {
            continue; // Si hay error, continuar esperando el siguiente mensaje
        }
        
        // Limpiar estructura de respuesta y procesar comando
        memset(&response, 0, sizeof(response));
        handleCommand(&command, &response);
        
        // Enviar respuesta por la cola de respuestas (sin bloqueo)
        msgQSend(respQueue, (char*)&response, sizeof(response), NO_WAIT, MSG_PRI_NORMAL);
    }
}

/**********************************************/
/*           Interfaz Pública               */
/**********************************************/

// Inicializar el sistema gestor de mensajes de cámara
STATUS startUsbHandler() {
    
    // Crear semáforo binario para sincronización (inicialmente disponible)
    cameraSem = semBCreate(SEM_Q_FIFO, SEM_FULL);
    if (!cameraSem) return ERROR;
    
    // Crear cola de comandos de cámara
    cmdQueue = msgQOpen("/CameraCmd", 10, sizeof(Msg), MSG_Q_FIFO, OM_CREATE, NULL);
    if (!cmdQueue) {
        semDelete(cameraSem);
        return ERROR;
    }
    
    // Crear cola de respuestas de cámara
    respQueue = msgQOpen("/CameraResp", 10, sizeof(Msg), MSG_Q_FIFO, OM_CREATE, NULL);
    if (!respQueue) {
        msgQClose(cmdQueue);
        semDelete(cameraSem);
        return ERROR;
    }
    
    // Crear tarea principal con prioridad 90
    handlerTaskId = taskSpawn("tHandler", 90, 0, 8192, 
                              (FUNCPTR)MessageHandler,
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    
    if (handlerTaskId == TASK_ID_ERROR) {
        // Si falla la creación de la tarea, limpiar recursos
        msgQClose(cmdQueue);
        msgQClose(respQueue);
        semDelete(cameraSem);
        return ERROR;
    }
    
    printf("[USB] Message handler ready\n");
    return OK;
}

// Detener el sistema gestor de mensajes de cámara
STATUS stopHandler() {
    
    
    // Cerrar cámara si está abierta
    if (cameraOpen) {
        semTake(cameraSem, WAIT_FOREVER);
        uvcCloseDevice(&cameraHandle);
        cameraOpen = false;
        semGive(cameraSem);
    }
    
    // Terminar tarea principal
    if (handlerTaskId != TASK_ID_NULL) {
        taskDelete(handlerTaskId);
        handlerTaskId = TASK_ID_NULL;
    }
    
    // Limpiar colas de mensajes
    if (cmdQueue) {
        msgQClose(cmdQueue);
        cmdQueue = NULL;
    }
    if (respQueue) {
        msgQClose(respQueue);
        respQueue = NULL;
    }
    
    // Eliminar semáforo
    if (cameraSem) {
        semDelete(cameraSem);
        cameraSem = NULL;
    }
   
    return OK;
}

// Mostrar estado actual del sistema de cámara
void showStatus() {
    printf("\n=== Estado del Sistema de Camara ===\n");
    printf("Gestor: %p\n", handlerTaskId);
    printf("Camara: %s\n", cameraOpen ? "ABIERTA" : "CERRADA");
    printf("====================================\n");
}
