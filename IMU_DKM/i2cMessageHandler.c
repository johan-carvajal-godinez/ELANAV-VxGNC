#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <taskLib.h>
#include <msgQLib.h>
#include <semLib.h>
#include <stdbool.h>
#include <hwif/vxBus.h>
#include <hwif/vxbus/vxbLib.h>
#include "i2cHandler.h"

/**********************************************/
/*           Estructura de Mensajes          */
/**********************************************/

// Enumeracion de comandos IMU disponibles para comunicacion por colas de mensajes
typedef enum {
    IMU_CMD_START = 1,    // Iniciar coleccion de datos IMU
    IMU_CMD_READ = 2,     // Leer datos actuales del IMU
    IMU_CMD_STOP = 3,     // Detener coleccion de datos IMU
    IMU_CMD_STATUS = 4    // Consultar estado del sistema IMU
} ImuCommand;

// Estructura de mensaje IMU para comunicacion entre RTP y DKM
typedef struct {
    ImuCommand cmd;               // Comando a ejecutar
    int readingsRequested;        // Numero de lecturas solicitadas
    int readingsCompleted;        // Numero de lecturas completadas
    int result;                   // Resultado de la operacion (0=exito, 1=error)
    int id;                       // Identificador unico del mensaje
    struct {
        int16_t ax, ay, az;       // Datos del acelerometro (ejes X, Y, Z)
        int16_t gx, gy, gz;       // Datos del giroscopio (ejes X, Y, Z)
        int16_t mx, my, mz;       // Datos del magnetometro (ejes X, Y, Z)
    } lastReading;
} ImuMsg;

/**********************************************/
/*           Variables Globales              */
/**********************************************/

MSG_Q_ID imuCmdQueue = NULL;        // Cola de comandos IMU
MSG_Q_ID imuRespQueue = NULL;       // Cola de respuestas IMU
TASK_ID imuHandlerTaskId = TASK_ID_NULL;    // ID de tarea principal
TASK_ID imuReaderTaskId = TASK_ID_NULL;     // ID de tarea lectora de IMU
VXB_DEV_ID imuDevice = NULL;        // Identificador del dispositivo IMU
bool imuActive = false;             // Flag: sistema IMU activo
int imuReadingsTarget = 0;          // Objetivo de lecturas a completar
int imuReadingsCount = 0;           // Contador de lecturas completadas
SEM_ID imuSem = NULL;               // Semaforo para sincronizacion
FILE* imuLogFile = NULL;            // Archivo de log IMU

/**********************************************/
/*         Tarea Lectora de IMU              */
/**********************************************/

// Tarea dedicada a la lectura continua de datos IMU y logging en formato TXT
void ImuReaderTask() {
    int16_t ax, ay, az, gx, gy, gz, mx, my, mz;  // Variables para datos de sensores
    
    // Bucle principal de lectura mientras el IMU este activo
    while (imuActive && imuReadingsCount < imuReadingsTarget) {
        bool readingSuccess = false;
        
        // Leer datos del acelerometro
        if (mpu9250ReadAccel(imuDevice, &ax, &ay, &az) == OK) {
            // Leer datos del giroscopio
            if (mpu9250ReadGyro(imuDevice, &gx, &gy, &gz) == OK) {
                // Intentar leer magnetometro (puede fallar ocasionalmente)
                if (ak8963ReadMag(imuDevice, &mx, &my, &mz) != OK) {
                    mx = my = mz = 0; // Establecer en cero si falla el magnetometro
                }
                readingSuccess = true;
            }
        }
        
        // Si la lectura fue exitosa, procesar y registrar datos
        if (readingSuccess) {
            // Seccion critica para actualizar contadores
            semTake(imuSem, WAIT_FOREVER);
            imuReadingsCount++;
            
            // Escribir datos al archivo de log en formato TXT legible
            if (imuLogFile) {
                fprintf(imuLogFile, "%d", imuReadingsCount);
                fprintf(imuLogFile, "A,");
                fprintf(imuLogFile, "%d,", ax);
                fprintf(imuLogFile, "%d,", ay);
                fprintf(imuLogFile, "%d,", az);
                fprintf(imuLogFile, "G,");
                fprintf(imuLogFile, "%d,", gx);
                fprintf(imuLogFile, "%d,", gy);
                fprintf(imuLogFile, "%d,", gz);
                fprintf(imuLogFile, "M,");
                fprintf(imuLogFile, "%d,", mx);
                fprintf(imuLogFile, "%d,", my);
                fprintf(imuLogFile, "%d\n", mz);
                fflush(imuLogFile);
            }
            
            // Verificar si se alcanzo el objetivo
            if (imuReadingsCount >= imuReadingsTarget) {
                imuActive = false;
            }
            
            semGive(imuSem);
        }
        
        taskDelay(sysClkRateGet() / 10); // Pausa de 100ms entre lecturas
    }
    
    // Cerrar archivo de log con resumen final
    if (imuLogFile) {
        fprintf(imuLogFile, "=== RESUMEN FINAL ===\n");
        fprintf(imuLogFile, "Total de lecturas completadas: %d\n", imuReadingsCount);
        fprintf(imuLogFile, "Archivo generado por el sistema IMU MPU9250\n");
        fprintf(imuLogFile, "====================\n");
        fclose(imuLogFile);
        imuLogFile = NULL;
    }
}

/**********************************************/
/*               Funciones                    */
/**********************************************/

// Procesa comandos IMU recibidos por cola de mensajes
STATUS handleImuCommand(ImuMsg* cmd, ImuMsg* resp) {
    STATUS result = ERROR;
    
    // Preparar estructura de respuesta con datos del comando
    resp->cmd = cmd->cmd;
    resp->id = cmd->id;
    resp->readingsRequested = cmd->readingsRequested;
    
    // Seccion critica para operaciones sobre el IMU
    semTake(imuSem, WAIT_FOREVER);
    
    switch (cmd->cmd) {
        case IMU_CMD_START:
            // Iniciar coleccion de datos IMU
            if (!imuActive) {
                // Obtener dispositivo IMU por nombre
                imuDevice = vxbDevAcquireByName("mpu9250", 0);
                if (imuDevice == NULL) {
                    break;
                }
                
                // Inicializar dispositivo IMU
                if (mpu9250Attach(imuDevice) != OK) {
                    break;
                }
                
                // Inicializar magnetometro AK8963
                ak8963Init(imuDevice);
                
                // Abrir archivo de log con encabezado informativo en formato TXT
                imuLogFile = fopen("/sd0a/imulog.txt", "w");
                if (imuLogFile) {
                    fprintf(imuLogFile, "=== LOG DE DATOS IMU MPU9250 ===\n");
                    fprintf(imuLogFile, "Dispositivo: MPU9250 (Acelerometro + Giroscopio + Magnetometro)\n");
                    fprintf(imuLogFile, "Objetivo de lecturas: %d\n", cmd->readingsRequested);
                    fprintf(imuLogFile, "Formato: Datos organizados por sensor y eje\n");
                    fprintf(imuLogFile, "Unidades: Acelerometro (mg), Giroscopio (grados/seg), Magnetometro (uT)\n");
                    fprintf(imuLogFile, "================================\n\n");
                    fflush(imuLogFile);
                }
                
                // Configurar parametros de coleccion
                imuReadingsTarget = cmd->readingsRequested;
                imuReadingsCount = 0;
                imuActive = true;
                
                // Crear tarea lectora de IMU
                imuReaderTaskId = taskSpawn("tImuReader", 100, 0, 8192,
                                            (FUNCPTR)ImuReaderTask,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
                
                if (imuReaderTaskId != TASK_ID_ERROR) {
                    result = OK;
                } else {
                    imuActive = false;
                    if (imuLogFile) {
                        fclose(imuLogFile);
                        imuLogFile = NULL;
                    }
                }
            } else {
                result = OK;
            }
            break;
            
        case IMU_CMD_READ:
            // Devolver datos actuales del sensor
            if (imuDevice && mpu9250ReadAccel(imuDevice, &resp->lastReading.ax, 
                                             &resp->lastReading.ay, &resp->lastReading.az) == OK) {
                // Leer giroscopio y magnetometro
                mpu9250ReadGyro(imuDevice, &resp->lastReading.gx, 
                               &resp->lastReading.gy, &resp->lastReading.gz);
                ak8963ReadMag(imuDevice, &resp->lastReading.mx, 
                             &resp->lastReading.my, &resp->lastReading.mz);
                result = OK;
            }
            break;
            
        case IMU_CMD_STOP:
            // Detener coleccion de datos IMU
            if (imuActive) {
                imuActive = false;
                // Terminar tarea lectora
                if (imuReaderTaskId != TASK_ID_NULL) {
                    taskDelete(imuReaderTaskId);
                    imuReaderTaskId = TASK_ID_NULL;
                }
                // El archivo se cierra automaticamente en ImuReaderTask
            }
            result = OK;
            break;
            
        case IMU_CMD_STATUS:
            // Consultar estado actual del sistema
            resp->readingsCompleted = imuReadingsCount;
            result = (imuReadingsCount >= imuReadingsTarget && imuReadingsTarget > 0) ? OK : ERROR;
            break;
            
        default:
            result = ERROR;
            break;
    }
    
    semGive(imuSem);
    
    // Establecer resultado en la respuesta
    resp->result = (result == OK) ? 0 : 1;
    resp->readingsCompleted = imuReadingsCount;
    return result;
}

// Tarea principal del gestor de mensajes IMU
void ImuMessageHandler() {
    ImuMsg command, response;
    
    // Bucle infinito para procesar mensajes de la cola
    while (1) {
        // Esperar comando desde la cola de comandos
        if (msgQReceive(imuCmdQueue, (char*)&command, sizeof(command), WAIT_FOREVER) == ERROR) {
            continue;
        }
        
        // Limpiar estructura de respuesta y procesar comando
        memset(&response, 0, sizeof(response));
        handleImuCommand(&command, &response);
        
        // Enviar respuesta por la cola de respuestas
        msgQSend(imuRespQueue, (char*)&response, sizeof(response), NO_WAIT, MSG_PRI_NORMAL);
    }
}

/**********************************************/
/*           Interfaz Publica               */
/**********************************************/

// Inicializar el sistema gestor de mensajes IMU
STATUS startImuHandler() {
    // Crear semaforo para sincronizacion
    imuSem = semBCreate(SEM_Q_FIFO, SEM_FULL);
    if (!imuSem) return ERROR;
    
    // Crear cola de comandos IMU
    imuCmdQueue = msgQOpen("/ImuCmd", 10, sizeof(ImuMsg), MSG_Q_FIFO, OM_CREATE, NULL);
    if (!imuCmdQueue) {
        semDelete(imuSem);
        return ERROR;
    }
    
    // Crear cola de respuestas IMU
    imuRespQueue = msgQOpen("/ImuResp", 10, sizeof(ImuMsg), MSG_Q_FIFO, OM_CREATE, NULL);
    if (!imuRespQueue) {
        msgQClose(imuCmdQueue);
        semDelete(imuSem);
        return ERROR;
    }
    
    // Crear tarea principal
    imuHandlerTaskId = taskSpawn("tImuHandler", 90, 0, 8192,
                                 (FUNCPTR)ImuMessageHandler,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    
    if (imuHandlerTaskId == TASK_ID_ERROR) {
        msgQClose(imuCmdQueue);
        msgQClose(imuRespQueue);
        semDelete(imuSem);
        return ERROR;
    }
    printf("[IMU] Message handler ready\n");
    return OK;
}

// Detener el sistema gestor de mensajes IMU
STATUS stopImuHandler() {
    // Detener sistema IMU si esta activo
    if (imuActive) {
        imuActive = false;
        if (imuReaderTaskId != TASK_ID_NULL) {
            taskDelete(imuReaderTaskId);
            imuReaderTaskId = TASK_ID_NULL;
        }
        if (imuLogFile) {
            fclose(imuLogFile);
            imuLogFile = NULL;
        }
    }
    
    // Terminar tarea principal
    if (imuHandlerTaskId != TASK_ID_NULL) {
        taskDelete(imuHandlerTaskId);
        imuHandlerTaskId = TASK_ID_NULL;
    }
    
    // Limpiar colas de mensajes
    if (imuCmdQueue) {
        msgQClose(imuCmdQueue);
        imuCmdQueue = NULL;
    }
    if (imuRespQueue) {
        msgQClose(imuRespQueue);
        imuRespQueue = NULL;
    }
    
    // Eliminar semaforo
    if (imuSem) {
        semDelete(imuSem);
        imuSem = NULL;
    }
    
    return OK;
}

// Mostrar estado actual del sistema IMU
void showImuStatus() {
    printf("\n=== Estado del Sistema IMU ===\n");
    printf("Manejador: %p\n", imuHandlerTaskId);
    printf("Activo: %s\n", imuActive ? "SI" : "NO");
    printf("Dispositivo: %p\n", imuDevice);
    printf("Lecturas: %d/%d\n", imuReadingsCount, imuReadingsTarget);
    printf("==============================\n");
}
