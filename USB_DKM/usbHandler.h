#ifndef UVC_CAPTURE_H
#define UVC_CAPTURE_H

#include <vxWorks.h>
#include <time.h>

/**********************************************/
/*        Estructura de Control UVC          */
/**********************************************/

/* 
 * Estructura de control para manejar una sesión de captura UVC.
 * Debe inicializarse con uvcOpenDevice() y liberarse con uvcCloseDevice().
 * Contiene todos los recursos necesarios para una sesión de captura de video.
 */
typedef struct {
    int fdFile;       /* Descriptor del archivo donde se almacenan los frames */
    int fdVideo;      /* Descriptor del dispositivo de video USB */
    char *buffer;     /* Buffer temporal para almacenar datos del frame */
    int bufferSize;   /* Tamaño del buffer en bytes */
} UVC_HANDLE;

/**********************************************/
/*        Enumeraciones de Operaciones       */
/**********************************************/

// Tipos de operación de cámara para comunicación por colas de mensajes
typedef enum camera_operation
{
    CAM_OPEN,         // Abrir dispositivo de cámara
    CAM_CAPTURE,      // Capturar un frame individual
    CAM_CLOSE,        // Cerrar dispositivo de cámara
    CAM_RECORD,       // Grabar múltiples frames
    CAM_STATUS        // Consultar estado del dispositivo
} cam_operation;

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************/
/*        Funciones Básicas de UVC          */
/**********************************************/

/**
 * Inicializa la captura desde un dispositivo UVC.
 * Abre el dispositivo de video y el archivo de salida, obtiene el tamaño de buffer
 * y reserva memoria necesaria para la captura.
 *
 * @param handle     Puntero a la estructura UVC_HANDLE a inicializar
 * @param pDevName   Nombre del dispositivo (ejemplo: "/usb2/uvc0")
 * @param fileName   Ruta del archivo de salida donde se guardarán los datos
 * @return OK en caso de éxito, ERROR en caso de fallo
 */
STATUS uvcOpenDevice(UVC_HANDLE *handle, const char *pDevName, const char *fileName);

/**
 * Captura un solo frame desde el dispositivo UVC.
 * Si tsOut no es NULL, se almacena la marca de tiempo del frame capturado.
 *
 * @param handle     Puntero a estructura UVC_HANDLE válida
 * @param tsOut      Puntero a estructura timespec para obtener la marca de tiempo (opcional)
 * @return OK si la captura fue exitosa, ERROR en caso de fallo
 */
STATUS uvcCaptureFrame(UVC_HANDLE *handle, struct timespec *tsOut);

/**
 * Libera todos los recursos utilizados por una sesión UVC: 
 * descriptores y buffer de captura.
 *
 * @param handle     Puntero a estructura UVC_HANDLE que se va a limpiar
 */
void uvcCloseDevice(UVC_HANDLE *handle);

/**
 * Función de alto nivel para capturar múltiples frames desde un dispositivo UVC.
 * Internamente abre y configura el dispositivo, captura nframes y luego limpia los recursos.
 *
 * @param pDevName   Nombre del dispositivo de video (ej: "/usb2/uvc0")
 * @param nframes    Número de frames a capturar
 * @param fileName   Ruta del archivo de salida donde se guardarán los datos
 * @return OK en caso de éxito, ERROR en caso de fallo
 */
STATUS uvcRecord(const char *pDevName, int nframes, const char *fileName);

#ifdef __cplusplus
}
#endif

#endif /* UVC_CAPTURE_H */
