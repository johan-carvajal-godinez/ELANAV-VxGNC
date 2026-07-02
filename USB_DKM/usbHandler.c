#include <vxWorks.h>
#include <ioLib.h>
#include <usb2Video.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "usbHandler.h"

/**********************************************/
/*        Implementación de Funciones UVC    */
/**********************************************/

/* 
 * Inicializa el dispositivo UVC para captura de video.
 * Esta función abre el archivo de salida y el dispositivo de video, 
 * obtiene el tamaño de buffer requerido y reserva memoria para almacenar los frames.
 * 
 * Pasos de inicialización:
 * 1. Validar parámetros de entrada
 * 2. Abrir archivo de destino para almacenamiento
 * 3. Abrir dispositivo de video USB
 * 4. Obtener tamaño de buffer necesario mediante ioctl
 * 5. Reservar memoria para buffer de captura
 */
STATUS uvcOpenDevice(UVC_HANDLE *handle, const char *pDevName, const char *fileName) {
    // Validación de parámetros de entrada
    if (!handle || !pDevName || !fileName)
        return ERROR;

    /* Abrir archivo donde se almacenarán los frames capturados */
    handle->fdFile = open(fileName, (O_CREAT | O_RDWR), 0);
    if (handle->fdFile < 0)
        return ERROR;

    /* Abrir dispositivo de video USB para acceso de solo lectura */
    handle->fdVideo = open(pDevName, O_RDONLY, 0);
    if (handle->fdVideo < 0) {
        close(handle->fdFile); // Limpiar archivo si falla apertura de video
        return ERROR;
    }

    /* Obtener el tamaño del buffer requerido para capturar un frame completo */
    if (ioctl(handle->fdVideo, USB2_VIDEO_IOCTL_GET_BUFFER_SIZE, (void *)&handle->bufferSize) == ERROR) {
        close(handle->fdFile);
        close(handle->fdVideo);
        return ERROR;
    }

    /* Reservar memoria dinámica para el buffer de captura */
    handle->buffer = malloc(handle->bufferSize);
    if (!handle->buffer) {
        close(handle->fdFile);
        close(handle->fdVideo);
        return ERROR;
    }

    return OK;
}

/* 
 * Captura un solo frame del dispositivo de video UVC.
 * Lee datos de video desde el dispositivo y los almacena en el archivo de salida.
 * Si se proporciona tsOut, también obtiene la marca de tiempo del frame capturado.
 * 
 * Proceso de captura:
 * 1. Validar estructura de control
 * 2. Leer frame desde dispositivo de video
 * 3. Obtener marca de tiempo (opcional)
 * 4. Escribir datos del frame al archivo
 */
STATUS uvcCaptureFrame(UVC_HANDLE *handle, struct timespec *tsOut) {
    // Validar estructura de control y buffer
    if (!handle || !handle->buffer)
        return ERROR;

    /* Leer frame completo desde el dispositivo de video */
    ssize_t size = read(handle->fdVideo, handle->buffer, handle->bufferSize);
    if (size < 1)
        return ERROR;

    /* Obtener marca de tiempo del frame si se solicita */
    if (tsOut && ioctl(handle->fdVideo, USB2_VIDEO_IOCTL_GET_TS, tsOut) == ERROR)
        return ERROR;

    /* Escribir el frame capturado en el archivo de salida */
    if (write(handle->fdFile, handle->buffer, size) != size)
        return ERROR;

    return OK;
}

/* 
 * Libera todos los recursos asociados con una sesión UVC.
 * Cierra descriptores de archivo y libera memoria del buffer.
 * Esta función debe llamarse siempre al finalizar una sesión de captura
 * para evitar fugas de memoria y descriptores.
 */
void uvcCloseDevice(UVC_HANDLE *handle) {
    if (!handle) return;

    /* Cerrar descriptor del dispositivo de video */
    if (handle->fdVideo >= 0)
        close(handle->fdVideo);
        
    /* Cerrar descriptor del archivo de salida */
    if (handle->fdFile >= 0)
        close(handle->fdFile);
        
    /* Liberar memoria del buffer de captura */
    if (handle->buffer)
        free(handle->buffer);

    /* Limpiar estructura de control para evitar uso accidental */
    handle->fdVideo = -1;
    handle->fdFile = -1;
    handle->buffer = NULL;
    handle->bufferSize = 0;
}

/* 
 * Función de alto nivel para grabar múltiples frames consecutivos.
 * Esta función encapsula todo el proceso: inicialización, captura múltiple
 * y limpieza de recursos. Es ideal para grabaciones simples sin necesidad
 * de control detallado sobre cada frame individual.
 * 
 * Flujo de trabajo:
 * 1. Inicializar dispositivo UVC
 * 2. Capturar número especificado de frames
 * 3. Liberar todos los recursos automáticamente
 */
STATUS uvcRecord(const char *pDevName, int nframes, const char *fileName) {
    // Validación de parámetros de entrada
    if (!pDevName || !fileName || nframes <= 0)
        return ERROR;

    // Inicializar estructura de control con valores por defecto
    UVC_HANDLE handle = { .fdFile = -1, .fdVideo = -1, .buffer = NULL, .bufferSize = 0 };

    /* Inicializar dispositivo UVC */
    if (uvcOpenDevice(&handle, pDevName, fileName) != OK)
        return ERROR;

    STATUS result = OK;
    struct timespec ts; // Estructura para captura de marca de tiempo

    /* Bucle de captura para el número especificado de frames */
    for (int i = 0; i < nframes; ++i) {
        if (uvcCaptureFrame(&handle, &ts) != OK) {
            result = ERROR;
            break; // Salir del bucle si hay error en captura
        }
    }

    /* Liberar recursos automáticamente al finalizar */
    uvcCloseDevice(&handle);
    return result;
}


