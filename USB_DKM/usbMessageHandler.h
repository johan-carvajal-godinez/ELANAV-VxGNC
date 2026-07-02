#ifndef CAMERA_MESSAGE_HANDLER_H
#define CAMERA_MESSAGE_HANDLER_H

#include <vxWorks.h>
#include <stdbool.h>

/**********************************************/
/*           Estructuras de Mensajes         */
/**********************************************/

// Enumeración de comandos de cámara disponibles para comunicación por colas de mensajes
typedef enum {
    CMD_OPEN = 1,     // Abrir dispositivo de cámara
    CMD_CAPTURE = 2,  // Capturar un frame individual
    CMD_CLOSE = 3,    // Cerrar dispositivo de cámara
    CMD_RECORD = 4,   // Grabar múltiples frames consecutivos
    CMD_STATUS = 5    // Consultar estado actual del dispositivo
} CameraCommand;

// Estructura de mensaje de cámara para comunicación entre RTP y DKM
typedef struct {
    CameraCommand cmd;    // Comando a ejecutar
    char device[32];      // Nombre del dispositivo de video (ej: "/uvc/0")
    char file[64];        // Ruta del archivo de salida
    int frames;           // Número de frames a capturar (para CMD_RECORD)
    int result;           // Resultado de la operación (0=éxito, 1=error)
    int id;               // Identificador único del mensaje
} Msg;

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************/
/*           Interfaz Pública               */
/**********************************************/

/**
 * Inicializa el sistema manejador de mensajes de cámara.
 * Crea las colas de mensajes necesarias y lanza la tarea manejadora principal.
 * 
 * @return OK en caso de éxito, ERROR en caso de fallo
 */
STATUS startUsbHandler(void);

/**
 * Detiene el sistema manejador de mensajes de cámara.
 * Limpia todos los recursos y termina la tarea manejadora.
 * 
 * @return OK en caso de éxito, ERROR en caso de fallo
 */
STATUS stopHandler(void);

/**
 * Muestra el estado actual del sistema manejador de mensajes de cámara.
 * Imprime información sobre la tarea manejadora y el estado del dispositivo.
 */
void showStatus(void);

#ifdef __cplusplus
}
#endif

#endif /* CAMERA_MESSAGE_HANDLER_H */
