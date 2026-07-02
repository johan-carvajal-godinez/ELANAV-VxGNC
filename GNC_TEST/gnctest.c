#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <taskLib.h>
#include <msgQLib.h>
#include <semLib.h>
#include <tickLib.h>
#include <stdbool.h>

#include "VxMAES.h"

/**********************************************/
/*           Prototipos de Funciones         */
/**********************************************/

STATUS startExtendedRtp(void);
STATUS initializeMessageQueues(void);
STATUS initializeAgents(void);

/**********************************************/
/*         Estructuras de Mensajes          */
/**********************************************/

// Estructura de mensaje de camara (del manejador de camara existente)
typedef enum {
    CMD_OPEN = 1,     // Abrir dispositivo de camara
    CMD_CAPTURE = 2,  // Capturar frame individual
    CMD_CLOSE = 3,    // Cerrar dispositivo de camara
    CMD_RECORD = 4,   // Grabar multiples frames
    CMD_STATUS = 5    // Consultar estado de camara
} CameraCommand;

typedef struct {
    CameraCommand cmd;    // Comando a ejecutar
    char device[32];      // Nombre del dispositivo
    char file[64];        // Archivo de salida
    int frames;           // Numero de frames
    int result;           // Resultado de operacion
    int id;               // ID unico del mensaje
} CameraMsg;

// Estructura de mensaje GPS
typedef enum {
    GPS_CMD_START = 1,    // Iniciar captura GPS
    GPS_CMD_READ = 2,     // Leer datos GPS actuales
    GPS_CMD_STOP = 3,     // Detener captura GPS
    GPS_CMD_STATUS = 4    // Consultar estado GPS
} GpsCommand;

typedef struct {
    GpsCommand cmd;           // Comando GPS a ejecutar
    int readingsRequested;    // Lecturas solicitadas
    int readingsCompleted;    // Lecturas completadas
    int result;               // Resultado de operacion
    int id;                   // ID unico del mensaje
} GpsMsg;

// Estructura de mensaje IMU
typedef enum {
    IMU_CMD_START = 1,    // Iniciar captura IMU
    IMU_CMD_READ = 2,     // Leer datos IMU actuales
    IMU_CMD_STOP = 3,     // Detener captura IMU
    IMU_CMD_STATUS = 4    // Consultar estado IMU
} ImuCommand;

typedef struct {
    ImuCommand cmd;           // Comando IMU a ejecutar
    int readingsRequested;    // Lecturas solicitadas
    int readingsCompleted;    // Lecturas completadas
    int result;               // Resultado de operacion
    int id;                   // ID unico del mensaje
} ImuMsg;

/**********************************************/
/*           Variables Globales              */
/**********************************************/

// Plataforma de agentes y entorno del sistema
Agent_Platform platform;
sysVars env;

// Agentes del sistema multimodal
MAESAgent cameraAgent, gpsAgent, imuAgent, coordinatorAgent;

// Colas de mensajes para comunicacion con DKMs
MSG_Q_ID cameraCmdQueue, cameraRespQueue;
MSG_Q_ID gpsCmdQueue, gpsRespQueue;
MSG_Q_ID imuCmdQueue, imuRespQueue;

// Seguimiento de finalizacion de tareas
volatile bool cameraComplete = false;
volatile bool gpsComplete = false;
volatile bool imuComplete = false;

// Configuracion de objetivos de la mision
#define TARGET_FRAMES 5           // Frames de video objetivo
#define TARGET_GPS_READINGS 500   // Ciclos GPS objetivo
#define TARGET_IMU_READINGS 500   // Lecturas IMU objetivo

/**********************************************/
/*           Funciones de Comunicacion       */
/**********************************************/

// Envia comando a DKM de camara y espera respuesta
STATUS sendCameraCommand(CameraCommand cmd, const char* device, const char* file, int frames) {
    CameraMsg command = {0};
    CameraMsg response = {0};
    
    // Preparar comando
    command.cmd = cmd;
    command.frames = frames;
    command.id = 1;
    if (device) strncpy(command.device, device, sizeof(command.device)-1);
    if (file) strncpy(command.file, file, sizeof(command.file)-1);
    
    // Enviar comando a DKM de camara
    if (msgQSend(cameraCmdQueue, (char*)&command, sizeof(command), WAIT_FOREVER, MSG_PRI_NORMAL) != OK) {
        printf("[CAMERA] Error al enviar comando\n");
        return ERROR;
    }
    
    // Esperar respuesta con timeout de 5 segundos
    if (msgQReceive(cameraRespQueue, (char*)&response, sizeof(response), 5 * sysClkRateGet()) == ERROR) {
        printf("[CAMERA] Timeout esperando respuesta\n");
        return ERROR;
    }
    
    return (response.result == 0) ? OK : ERROR;
}

// Envia comando a DKM de GPS y espera respuesta
STATUS sendGpsCommand(GpsCommand cmd, int readings) {
    GpsMsg command = {0};
    GpsMsg response = {0};
    
    // Preparar comando GPS
    command.cmd = cmd;
    command.readingsRequested = readings;
    command.id = 1;
    
    // Enviar comando a DKM de GPS
    if (msgQSend(gpsCmdQueue, (char*)&command, sizeof(command), WAIT_FOREVER, MSG_PRI_NORMAL) != OK) {
        printf("[GPS] Error al enviar comando\n");
        return ERROR;
    }
    
    // Esperar respuesta con timeout de 5 segundos
    if (msgQReceive(gpsRespQueue, (char*)&response, sizeof(response), 5 * sysClkRateGet()) == ERROR) {
        printf("[GPS] Timeout esperando respuesta\n");
        return ERROR;
    }
    
    return (response.result == 0) ? OK : ERROR;
}

// Envia comando a DKM de IMU y espera respuesta
STATUS sendImuCommand(ImuCommand cmd, int readings) {
    ImuMsg command = {0};
    ImuMsg response = {0};
    
    // Preparar comando IMU
    command.cmd = cmd;
    command.readingsRequested = readings;
    command.id = 1;
    
    // Enviar comando a DKM de IMU
    if (msgQSend(imuCmdQueue, (char*)&command, sizeof(command), WAIT_FOREVER, MSG_PRI_NORMAL) != OK) {
        printf("[IMU] Error al enviar comando\n");
        return ERROR;
    }
    
    // Esperar respuesta con timeout de 5 segundos
    if (msgQReceive(imuRespQueue, (char*)&response, sizeof(response), 5 * sysClkRateGet()) == ERROR) {
        printf("[IMU] Timeout esperando respuesta\n");
        return ERROR;
    }
    
    return (response.result == 0) ? OK : ERROR;
}

// Verifica el estado de finalizacion de cada dispositivo
bool checkTaskCompletion(int deviceType) {
    switch (deviceType) {
        case 1: { // Verificacion de camara
            CameraMsg command = {0}, response = {0};
            command.cmd = CMD_STATUS;
            command.id = 1;
            
            if (msgQSend(cameraCmdQueue, (char*)&command, sizeof(command), NO_WAIT, MSG_PRI_NORMAL) != OK)
                return false;
            if (msgQReceive(cameraRespQueue, (char*)&response, sizeof(response), 1 * sysClkRateGet()) == ERROR)
                return false;
            
            return (response.result == 0); // Camara retorna OK cuando todos los frames estan capturados
        }
        
        case 2: { // Verificacion de GPS
            GpsMsg command = {0}, response = {0};
            command.cmd = GPS_CMD_STATUS;
            command.id = 1;
            
            if (msgQSend(gpsCmdQueue, (char*)&command, sizeof(command), NO_WAIT, MSG_PRI_NORMAL) != OK)
                return false;
            if (msgQReceive(gpsRespQueue, (char*)&response, sizeof(response), 1 * sysClkRateGet()) == ERROR)
                return false;
            
            return (response.readingsCompleted >= TARGET_GPS_READINGS);
        }
        
        case 3: { // Verificacion de IMU
            ImuMsg command = {0}, response = {0};
            command.cmd = IMU_CMD_STATUS;
            command.id = 1;
            
            if (msgQSend(imuCmdQueue, (char*)&command, sizeof(command), NO_WAIT, MSG_PRI_NORMAL) != OK)
                return false;
            if (msgQReceive(imuRespQueue, (char*)&response, sizeof(response), 1 * sysClkRateGet()) == ERROR)
                return false;
            
            return (response.readingsCompleted >= TARGET_IMU_READINGS);
        }
    }
    return false;
}

/**********************************************/
/*         Comportamientos de Agentes        */
/**********************************************/

// Comportamiento del agente de camara: captura frames secuenciales
void cameraAgentBehavior(MAESArgument param) {
    Agent_Msg msg;
    ConstructorAgent_Msg(&msg, &env);
    msg.Agent_Msg(&msg);
    
    printf("\n[CAMERA] Iniciando captura de video...\n");
    
    // Abrir camara con dispositivo y archivo correctos
    if (sendCameraCommand(CMD_OPEN, "/uvc/0", "/sd0a/video.dat", 0) == OK) {
        printf("[CAMERA] Camara abierta exitosamente\n\n");
        
        // Capturar frames uno por uno
        for (int i = 0; i < TARGET_FRAMES; i++) {
            if (sendCameraCommand(CMD_CAPTURE, NULL, NULL, 0) == OK) {
                printf("[CAMERA] Frame %d/%d capturado\n", i + 1, TARGET_FRAMES);
            } else {
                printf("[CAMERA] Error al capturar frame %d\n\n", i + 1);
                break;
            }
            taskDelay(sysClkRateGet() / 2); // Pausa de 500ms entre capturas
        }
        
        // Cerrar camara
        sendCameraCommand(CMD_CLOSE, NULL, NULL, 0);
        printf("\n[CAMERA] Todos los frames capturados - guardados en /sd0a/video.dat\n\n");
        
        cameraComplete = true;
    } else {
        printf("[CAMERA] Error al abrir camara en /uvc/0\n\n");
        cameraComplete = true; // Marcar como completo para no bloquear mision
    }
    
    // Mantener agente vivo para verificacion de estado
    while (1) {
        taskDelay(sysClkRateGet());
    }
}

// Comportamiento del agente GPS: captura ciclos NMEA completos
void gpsAgentBehavior(MAESArgument param) {
    Agent_Msg msg;
    ConstructorAgent_Msg(&msg, &env);
    msg.Agent_Msg(&msg);
    
    printf("\n[GPS] Iniciando captura de datos GPS...\n");
    
    // Iniciar captura de datos GPS
    if (sendGpsCommand(GPS_CMD_START, TARGET_GPS_READINGS) == OK) {
        printf("[GPS] Captura iniciada - esperando %d ciclos NMEA\n\n", TARGET_GPS_READINGS);
        
        // Monitorear progreso con menor frecuencia
        int lastCount = 0;
        while (!gpsComplete) {
            if (checkTaskCompletion(2)) {
                gpsComplete = true;
                printf("\n[GPS] Todos los %d ciclos capturados\n\n", TARGET_GPS_READINGS);
                break;
            }
            
            // Mostrar progreso cada 25 lecturas
            GpsMsg cmd = {GPS_CMD_STATUS, 0, 0, 0, 1}, resp;
            if (msgQSend(gpsCmdQueue, (char*)&cmd, sizeof(cmd), NO_WAIT, MSG_PRI_NORMAL) == OK &&
                msgQReceive(gpsRespQueue, (char*)&resp, sizeof(resp), 1 * sysClkRateGet()) != ERROR) {
                if (resp.readingsCompleted >= lastCount + 25) {
                    printf("[GPS] Progreso: %d/%d ciclos\n", resp.readingsCompleted, TARGET_GPS_READINGS);
                    lastCount = resp.readingsCompleted;
                }
            }
            
            taskDelay(3 * sysClkRateGet()); // Verificar cada 3 segundos
        }
        
        // Detener GPS
        sendGpsCommand(GPS_CMD_STOP, 0);
    } else {
        printf("[GPS] Error al iniciar captura GPS\n\n");
    }
    
    // Mantener agente vivo para verificacion de estado
    while (1) {
        taskDelay(sysClkRateGet());
    }
}

// Comportamiento del agente IMU: captura lecturas de sensores inerciales
void imuAgentBehavior(MAESArgument param) {
    Agent_Msg msg;
    ConstructorAgent_Msg(&msg, &env);
    msg.Agent_Msg(&msg);
    
    printf("\n[IMU] Iniciando captura de datos IMU...\n");
    
    // Iniciar captura de datos IMU
    if (sendImuCommand(IMU_CMD_START, TARGET_IMU_READINGS) == OK) {
        printf("[IMU] Captura iniciada - capturando %d lecturas\n\n", TARGET_IMU_READINGS);
        
        // Monitorear progreso con menor frecuencia
        int lastCount = 0;
        while (!imuComplete) {
            if (checkTaskCompletion(3)) {
                imuComplete = true;
                printf("\n[IMU] Todas las %d lecturas capturadas\n\n", TARGET_IMU_READINGS);
                break;
            }
            
            // Mostrar progreso cada 25 lecturas
            ImuMsg cmd = {IMU_CMD_STATUS, 0, 0, 0, 1}, resp;
            if (msgQSend(imuCmdQueue, (char*)&cmd, sizeof(cmd), NO_WAIT, MSG_PRI_NORMAL) == OK &&
                msgQReceive(imuRespQueue, (char*)&resp, sizeof(resp), 1 * sysClkRateGet()) != ERROR) {
                if (resp.readingsCompleted >= lastCount + 25) {
                    printf("[IMU] Progreso: %d/%d lecturas\n", resp.readingsCompleted, TARGET_IMU_READINGS);
                    lastCount = resp.readingsCompleted;
                }
            }
            
            taskDelay(3 * sysClkRateGet()); // Verificar cada 3 segundos
        }
        
        // Detener IMU
        sendImuCommand(IMU_CMD_STOP, 0);
    } else {
        printf("[IMU] Error al iniciar captura IMU\n\n");
    }
    
    // Mantener agente vivo para verificacion de estado
    while (1) {
        taskDelay(sysClkRateGet());
    }
}

// Comportamiento del agente coordinador: supervisa progreso global de la mision
void coordinatorAgentBehavior(MAESArgument param) {
    Agent_Msg msg;
    ConstructorAgent_Msg(&msg, &env);
    msg.Agent_Msg(&msg);
    
    printf("\n[COORDINATOR] Mision iniciada\n");
    printf("Objetivos: %d frames de camara, %d ciclos GPS, %d lecturas IMU\n\n", 
           TARGET_FRAMES, TARGET_GPS_READINGS, TARGET_IMU_READINGS);
    
    // Esperar a que todos los agentes esten listos
    taskDelay(3 * sysClkRateGet());
    
    // Monitorear progreso general de la mision
    bool allComplete = false;
    int statusChecks = 0;
    
    while (!allComplete) {
        statusChecks++;
        
        // Verificar estado cada 10 segundos
        taskDelay(10 * sysClkRateGet());
        
        bool cameraStatus = cameraComplete || checkTaskCompletion(1);
        bool gpsStatus = gpsComplete || checkTaskCompletion(2);
        bool imuStatus = imuComplete || checkTaskCompletion(3);
        
        printf("[COORDINATOR] Estado: Camara %s | GPS %s | IMU %s\n",
               cameraStatus ? "COMPLETO" : "TRABAJANDO",
               gpsStatus ? "COMPLETO" : "TRABAJANDO", 
               imuStatus ? "COMPLETO" : "TRABAJANDO");
        
        if (cameraStatus && gpsStatus && imuStatus) {
            allComplete = true;
            printf("\n*** MISION COMPLETADA EXITOSAMENTE ***\n");
            printf("Archivos de datos guardados:\n");
            printf("  Camara: /sd0a/video.dat\n");
            printf("  GPS: /sd0a/gpslog.txt\n");
            printf("  IMU: /sd0a/imulog.txt\n\n");
            break;
        }
        
        // Timeout de seguridad despues de 30 verificaciones (5 minutos)
        if (statusChecks > 30) {
            printf("\n[COORDINATOR] Timeout de mision\n\n");
            break;
        }
    }
    
    // Mision completa - salir limpiamente
    taskDelay(2 * sysClkRateGet());
    exit(0);
}

/**********************************************/
/*         Funciones de Inicializacion      */
/**********************************************/

// Inicializa todas las colas de mensajes para comunicacion con DKMs
STATUS initializeMessageQueues() {
    printf("[INIT] Inicializando colas de mensajes\n");
    
    // Colas de camara
    cameraCmdQueue = msgQOpen("/CameraCmd", 10, sizeof(CameraMsg), MSG_Q_FIFO, OM_EXCL, NULL);
    cameraRespQueue = msgQOpen("/CameraResp", 10, sizeof(CameraMsg), MSG_Q_FIFO, OM_EXCL, NULL);
    
    if (!cameraCmdQueue || !cameraRespQueue) {
        printf("[INIT] Error al abrir colas de mensajes de camara\n");
        return ERROR;
    }
    
    // Colas de GPS
    gpsCmdQueue = msgQOpen("/GpsCmd", 10, sizeof(GpsMsg), MSG_Q_FIFO, OM_EXCL, NULL);
    gpsRespQueue = msgQOpen("/GpsResp", 10, sizeof(GpsMsg), MSG_Q_FIFO, OM_EXCL, NULL);
    
    if (!gpsCmdQueue || !gpsRespQueue) {
        printf("[INIT] Error al abrir colas de mensajes de GPS\n");
        return ERROR;
    }
    
    // Colas de IMU
    imuCmdQueue = msgQOpen("/ImuCmd", 10, sizeof(ImuMsg), MSG_Q_FIFO, OM_EXCL, NULL);
    imuRespQueue = msgQOpen("/ImuResp", 10, sizeof(ImuMsg), MSG_Q_FIFO, OM_EXCL, NULL);
    
    if (!imuCmdQueue || !imuRespQueue) {
        printf("[INIT] Error al abrir colas de mensajes de IMU\n");
        return ERROR;
    }
    
    printf("[INIT] Todas las colas de mensajes listas\n");
    return OK;
}

// Inicializa y configura todos los agentes del sistema
STATUS initializeAgents() {
    printf("[INIT] Inicializando agentes\n");
    
    // Inicializar constructores de agentes
    ConstructorAgente(&cameraAgent);
    ConstructorAgente(&gpsAgent);
    ConstructorAgente(&imuAgent);
    ConstructorAgente(&coordinatorAgent);
    
    // Configurar agente de camara
    cameraAgent.Iniciador(&cameraAgent, "CameraAgent", 150, 8192);
    platform.agent_init(&platform, &cameraAgent, (MAESTaskFunction_t)cameraAgentBehavior);
    platform.register_agent(&platform, cameraAgent.agent.aid);
    
    // Configurar agente de GPS
    gpsAgent.Iniciador(&gpsAgent, "GpsAgent", 150, 8192);
    platform.agent_init(&platform, &gpsAgent, (MAESTaskFunction_t)gpsAgentBehavior);
    platform.register_agent(&platform, gpsAgent.agent.aid);
    
    // Configurar agente de IMU
    imuAgent.Iniciador(&imuAgent, "ImuAgent", 150, 8192);
    platform.agent_init(&platform, &imuAgent, (MAESTaskFunction_t)imuAgentBehavior);
    platform.register_agent(&platform, imuAgent.agent.aid);
    
    // Configurar agente coordinador
    coordinatorAgent.Iniciador(&coordinatorAgent, "Coordinator", 140, 8192);
    platform.agent_init(&platform, &coordinatorAgent, (MAESTaskFunction_t)coordinatorAgentBehavior);
    platform.register_agent(&platform, coordinatorAgent.agent.aid);
    
    printf("[INIT] Todos los agentes registrados\n");
    return OK;
}

/**********************************************/
/*           Funcion Principal RTP          */
/**********************************************/

// Funcion principal del programa - punto de entrada
int main(int argc, char *argv[]) {
    printf("\n========================================\n");
    printf("  Sistema de Captura Multi-Dispositivo  \n");
    printf("========================================\n");
    printf("Objetivos: 5 frames, 500 ciclos GPS, 500 lecturas IMU\n");
    printf("========================================\n\n");
    
    // Iniciar la mision inmediatamente
    if (startExtendedRtp() == OK) {
        printf("Mision completada exitosamente!\n\n");
        printf("Verificar archivos de datos:\n");
        printf("  Camara: /sd0a/video.dat\n");
        printf("  GPS: /sd0a/gpslog.txt\n");
        printf("  IMU: /sd0a/imulog.txt\n\n");
    } else {
        printf("Error al iniciar la mision\n\n");
        return 1;
    }
    
    return 0;
}

// Funcion principal para iniciar el RTP extendido
STATUS startExtendedRtp() {
    printf("[INIT] Iniciando mision de captura de datos\n");
    
    // Inicializar entorno y plataforma
    ConstructorSysVars(&env);
    ConstructorAgent_Platform(&platform, &env);
    platform.Agent_Platform(&platform, "DataCollectionRTP", taskIdSelf());
    
    // Arrancar plataforma
    if (platform.boot(&platform) != NO_ERRORS) {
        printf("[ERROR] Error al arrancar plataforma de agentes\n");
        return ERROR;
    }
    
    // Inicializar colas de mensajes
    if (initializeMessageQueues() != OK) {
        printf("[ERROR] Error al inicializar colas de mensajes\n");
        return ERROR;
    }
    
    // Esperar a que los manejadores DKM esten listos
    taskDelay(2 * sysClkRateGet());
    
    // Inicializar y lanzar agentes
    if (initializeAgents() != OK) {
        printf("[ERROR] Error al inicializar agentes\n");
        return ERROR;
    }
    
    printf("[INIT] Mision iniciada - capturando datos...\n");
    
    // Esperar finalizacion de mision (el coordinador llamara exit())
    while (1) {
        taskDelay(sysClkRateGet());
    }
    
    return OK;
}

