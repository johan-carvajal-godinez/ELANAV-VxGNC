#ifndef IMU_MESSAGE_HANDLER_H
#define IMU_MESSAGE_HANDLER_H

#include <vxWorks.h>
#include <stdbool.h>

/**********************************************/
/*           Estructuras de Mensajes         */
/**********************************************/

// Enumeración de comandos IMU disponibles para comunicación por colas de mensajes
typedef enum {
    IMU_CMD_START = 1,    // Iniciar colección de datos IMU
    IMU_CMD_READ = 2,     // Leer datos actuales del IMU
    IMU_CMD_STOP = 3,     // Detener colección de datos IMU
    IMU_CMD_STATUS = 4    // Consultar estado del sistema IMU
} ImuCommand;

// Estructura de mensaje IMU para comunicación entre RTP y DKM
// Contiene toda la información necesaria para comandos y respuestas del sistema inercial
typedef struct {
    ImuCommand cmd;               // Comando a ejecutar
    int readingsRequested;        // Número de lecturas solicitadas
    int readingsCompleted;        // Número de lecturas completadas
    int result;                   // Resultado de la operación (0=éxito, 1=error)
    int id;                       // Identificador único del mensaje
    struct {
        int16_t ax, ay, az;       // Datos del acelerómetro (ejes X, Y, Z)
        int16_t gx, gy, gz;       // Datos del giroscopio (ejes X, Y, Z)
        int16_t mx, my, mz;       // Datos del magnetómetro (ejes X, Y, Z)
    } lastReading;
} ImuMsg;

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************/
/*           Interfaz Pública               */
/**********************************************/

/**
 * Inicializa el sistema manejador de mensajes IMU.
 * Crea las colas de mensajes necesarias, semáforos de sincronización
 * y lanza la tarea manejadora principal para el sistema inercial.
 * 
 * @return OK en caso de éxito, ERROR en caso de fallo
 */
STATUS startImuHandler(void);

/**
 * Detiene el sistema manejador de mensajes IMU.
 * Termina todas las tareas asociadas, libera el dispositivo IMU,
 * cierra archivos de log y limpia todos los recursos del sistema.
 * 
 * @return OK en caso de éxito, ERROR en caso de fallo
 */
STATUS stopImuHandler(void);

/**
 * Muestra el estado actual del sistema manejador de mensajes IMU.
 * Imprime información sobre las tareas activas, estado del dispositivo MPU9250,
 * progreso de colección de datos y estadísticas de funcionamiento.
 */
void showImuStatus(void);

#ifdef __cplusplus
}
#endif

#endif /* IMU_MESSAGE_HANDLER_H */
