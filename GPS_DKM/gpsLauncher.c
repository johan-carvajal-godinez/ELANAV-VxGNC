#include <vxWorks.h>
#include <stdio.h>
#include "gpsHandler.h"
#include "taskLib.h"
/**********************************************/
/*           Funcion Principal de GPS        */
/**********************************************/
// Funcion principal para iniciar el sistema GPS y procesar datos NMEA
void startGPS(void) {
    char nmea[128]; // Buffer para almacenar lineas NMEA recibidas
    
    // Inicializar dispositivo GPS (abrir puerto UART y configurar baudrate)
    if (gpsInit() != OK) {
        return;
    }
    
    // Bucle principal de lectura y procesamiento de datos GPS
    while (1) {
        // Leer una linea NMEA completa desde el dispositivo GPS
        if (gpsReadLine(nmea, sizeof(nmea)) == OK) {
            // Realizar decodificacion de las tramas NMEA segun su tipo
            // Procesar mensajes GGA (posicion, altitud, satelites)
            gpsParseGGA(nmea);
            // Procesar mensajes RMC (velocidad, rumbo)
            gpsParseRMC(nmea);
            
            // Registrar datos en archivo de log solo cuando hay un fix valido
            // Esta funcion internamente verifica si el fix es confiable antes de escribir
            gpsLogFix(nmea);
        }
        // Pausa ajustable entre lecturas para no saturar el CPU
        // 10 ticks del sistema (aproximadamente 100ms en la mayoria de configuraciones)
        taskDelay(10);
    }
}
