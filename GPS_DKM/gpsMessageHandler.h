#ifndef GPS_MESSAGE_HANDLER_H
#define GPS_MESSAGE_HANDLER_H

#include <vxWorks.h>
#include <stdbool.h>

/**********************************************/
/*           Estructuras de Mensajes         */
/**********************************************/

// Enumeración de comandos GPS disponibles para comunicación por colas de mensajes
typedef enum {
    GPS_CMD_START = 1,    // Iniciar colección de datos GPS
    GPS_CMD_READ = 2,     // Leer datos actuales del GPS
    GPS_CMD_STOP = 3,     // Detener colección de datos GPS
    GPS_CMD_STATUS = 4    // Consultar estado del sistema GPS
} GpsCommand;

// Estructura de mensaje GPS para comunicación entre RTP y DKM
// Contiene toda la información necesaria para comandos y respuestas
typedef struct {
    GpsCommand cmd;               // Comando a ejecutar
    int readingsRequested;        // Número de ciclos NMEA solicitados
    int readingsCompleted;        // Número de ciclos NMEA completados
    int result;                   // Resultado de la operación (0=éxito, 1=error)
    int id;                       // Identificador único del mensaje
} GpsMsg;

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************/
/*           Interfaz Pública               */
/**********************************************/

/**
 * Inicializa el sistema manejador de mensajes GPS.
 * Crea las colas de mensajes necesarias, semáforos de sincronización
 * y lanza la tarea manejadora principal.
 * 
 * @return OK en caso de éxito, ERROR en caso de fallo
 */
STATUS startGpsHandler(void);

/**
 * Detiene el sistema manejador de mensajes GPS.
 * Termina todas las tareas asociadas, cierra el dispositivo GPS,
 * libera las colas de mensajes y limpia todos los recursos.
 * 
 * @return OK en caso de éxito, ERROR en caso de fallo
 */
STATUS stopGpsHandler(void);

/**
 * Muestra el estado actual del sistema manejador de mensajes GPS.
 * Imprime información sobre las tareas activas, estado del dispositivo,
 * progreso de colección de datos y estadísticas generales.
 */
void showGpsStatus(void);

#ifdef __cplusplus
}
#endif

#endif /* GPS_MESSAGE_HANDLER_H */
