#include <vxWorks.h>
#include "stdio.h"
#include <symLib.h>
#include <stdlib.h>
#include <sysSymTbl.h>

#include "usbHandler.h"



// Definicion de la funcion que inicia el DKM
void startUSB(void) {
    // Parametros para uvcRecord
    char *devName = "/uvc/0";       // Nombre del dispositivo USB 
    int numFrames = 3;            // Numero de frames por capturar 
    char *outputFile = "/sd0a/video.dat"; // Ruta para el archivo de salida

    // Mensaje que indica que el DKM esta siendo inicializado
    printf("\nIniciando DKM\n");

    // Llamada de la funcion uvcRecord con los parametros necesarios
    STATUS status = uvcRecord(devName, numFrames, outputFile);

    // Verificar el status retornado por uvcRecord
    if (status == OK) {
    	printf("[USB] Captura de frames completada exitosamente\n");
    } else {
    	printf("[USB] Error: La captura de frames fallo.\n");
    }
   
}






