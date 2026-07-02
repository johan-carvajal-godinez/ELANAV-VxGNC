#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ioLib.h>
#include "taskLib.h"
#include <fcntl.h>
#include <unistd.h>
#include <sioLib.h>
#include <stdbool.h>
#include "gpsHandler.h"

// Definición del dispositivo UART donde está conectado el GPS
#define GPS_DEVICE "/ttyS1"

// Longitud máxima esperada de una sentencia NMEA
#define MAX_NMEA_LENGTH 128

// Baud rate esperado para la comunicación serial con el GPS
#define GPS_BAUDRATE 9600

// Descriptor de archivo del dispositivo GPS (UART)
static int gpsFd = -1;

// Estructura que almacena el último fix recibido del GPS
static GpsFix latestFix = {
    .timeReady = FALSE,
    .positionReady = FALSE,
    .altitudeReady = FALSE,
    .speedReady = FALSE,
    .satReady = FALSE,
    .fixQuality = 0
};

/**********************************************/
/*        Implementación de Funciones         */
/**********************************************/

// Convierte coordenadas NMEA (formato DDMM.MMMM) a grados decimales
static float convertNMEAToDecimal(float value) {
    if (value == 0.0) return 0.0;
    
    float degrees = (float)(value / 100);
    float minutes = value - (degrees * 100);
    return degrees + minutes / 60.0f;
}

// Inicializa el GPS: abre el puerto UART y configura la velocidad de transmisión
STATUS gpsInit(void) {
    gpsFd = open(GPS_DEVICE, O_RDONLY, 0);
    if (gpsFd < 0) {
        perror("[GPS] Error opening GPS UART");
        return ERROR;
    }

    if (ioctl(gpsFd, SIO_BAUD_SET, GPS_BAUDRATE) == ERROR) {
        perror("[GPS] Error setting baud rate");
        close(gpsFd);
        gpsFd = -1;
        return ERROR;
    }

    printf("[GPS] GPS initialized on %s at %d baud\n", GPS_DEVICE, GPS_BAUDRATE);
    return OK;
}

// Lee una línea NMEA completa desde el GPS (terminada en '\n')
STATUS gpsReadLine(char *buf, int maxLen) {
    int i = 0;
    char c;

    if (gpsFd < 0) return ERROR;

    while (i < maxLen - 1) {
        if (read(gpsFd, &c, 1) == 1) {
            if (c == '\n') break;
            if (c != '\r') buf[i++] = c;
        } else {
            taskDelay(1);
        }
    }
    buf[i] = '\0';
    return (i > 0) ? OK : ERROR;
}

// Parsea una sentencia NMEA tipo GGA para extraer hora, latitud, longitud, altitud y número de satélites
STATUS gpsParseGGA(const char *nmea) {
    if (strncmp(nmea, "$GPGGA", 6) != 0 && strncmp(nmea, "$GNGGA", 6) != 0) return ERROR;

    char copy[MAX_NMEA_LENGTH];
    strncpy(copy, nmea, MAX_NMEA_LENGTH);
    copy[MAX_NMEA_LENGTH - 1] = '\0';

    // Split by commas and process each field
    char *fields[15];  // GGA has up to 15 fields
    int fieldCount = 0;
    
    char *token = strtok(copy, ",");
    while (token != NULL && fieldCount < 15) {
        fields[fieldCount] = token;
        fieldCount++;
        token = strtok(NULL, ",");
    }
    
    // Reset position status for this parse
    latestFix.positionReady = FALSE;
    
    // Only proceed if we have enough fields
    if (fieldCount < 7) return ERROR;
    
    float tempLat = 0.0, tempLon = 0.0;
    int quality = 0;
    bool hasLatDir = false, hasLonDir = false;
    bool hasValidCoords = false;
    
    // Field 1: Time (fields[1])
    if (fieldCount > 1 && strlen(fields[1]) > 0) {
        strncpy(latestFix.time, fields[1], sizeof(latestFix.time)-1);
        latestFix.time[sizeof(latestFix.time)-1] = '\0';
        latestFix.timeReady = TRUE;
    }
    
    // Field 2: Latitude (fields[2])
    if (fieldCount > 2 && strlen(fields[2]) > 0) {
        tempLat = convertNMEAToDecimal((float) atof(fields[2]));
        hasValidCoords = true;
    }
    
    // Field 3: Latitude Direction (fields[3])
    if (fieldCount > 3 && strlen(fields[3]) > 0) {
        hasLatDir = true;
        if (fields[3][0] == 'S') tempLat *= -1.0f;
    }
    
    // Field 4: Longitude (fields[4])
    if (fieldCount > 4 && strlen(fields[4]) > 0) {
        tempLon = convertNMEAToDecimal((float) atof(fields[4]));
    } else {
        tempLon = 0.0;  // Set explicit invalid value
        hasValidCoords = false;
    }
    
    // Field 5: Longitude Direction (fields[5])
    if (fieldCount > 5 && strlen(fields[5]) > 0) {
        hasLonDir = true;
        if (fields[5][0] == 'W') tempLon *= -1.0f;
    }
    
    // Field 6: GPS Quality (fields[6])
    if (fieldCount > 6 && strlen(fields[6]) > 0) {
        quality = atoi(fields[6]);
        // Validate quality range (0-9 for standard GPS)
        if (quality >= 0 && quality <= 9) {
            latestFix.fixQuality = quality; // Store for later use
            if (quality > 0) {
                printf("[GPS] Fix exitoso: calidad %d\n", quality);
            }
        } else {
            quality = 0; // Invalid quality, treat as no fix
            latestFix.fixQuality = 0;
        }
    }
    
    // Field 7: Number of satellites (fields[7])
    if (fieldCount > 7 && strlen(fields[7]) > 0) {
        latestFix.satellites = atoi(fields[7]);
        latestFix.satReady = TRUE;
    }
    
    // Field 9: Altitude (fields[9])
    if (fieldCount > 9 && strlen(fields[9]) > 0) {
        latestFix.altitude = (float) atof(fields[9]);
        latestFix.altitudeReady = TRUE;
    }
    
    // Only set position as ready if ALL validation passes
    if (quality > 0 && hasLatDir && hasLonDir && hasValidCoords &&
        tempLat != 0.0 && tempLon != 0.0 &&
        tempLat >= -90.0 && tempLat <= 90.0 &&
        tempLon >= -180.0 && tempLon <= 180.0) {
        
        latestFix.latitude = tempLat;
        latestFix.longitude = tempLon;
        latestFix.positionReady = TRUE;
    }

    return OK;
}

STATUS gpsParseRMC(const char *nmea) { // Parsea una sentencia NMEA tipo RMC para extraer la velocidad
    if (strncmp(nmea, "$GPRMC", 6) != 0 && strncmp(nmea, "$GNRMC", 6) != 0) return ERROR;

    char copy[MAX_NMEA_LENGTH];
    strncpy(copy, nmea, MAX_NMEA_LENGTH);
    copy[MAX_NMEA_LENGTH - 1] = '\0';

    char *token = strtok(copy, ",");
    int field = 0;

    while (token != NULL) {
        field++;
        if (field == 8 && strlen(token) > 0) {
            latestFix.speed = (float) atof(token);
            latestFix.speedReady = TRUE;
        }
        token = strtok(NULL, ",");
    }

    return OK;
}

// Devuelve un puntero a la estructura con el último fix recibido
GpsFix* gpsGetFix(void) {
    return &latestFix;
}

// Devuelve la hora del fix si está disponible
const char* gpsGetTime(void) {
    return latestFix.timeReady ? latestFix.time : "N/A";
}

// Devuelve latitud y longitud si hay un fix disponible
STATUS gpsGetLatLon(float *lat, float *lon) {
    if (latestFix.positionReady) {
        *lat = latestFix.latitude;
        *lon = latestFix.longitude;
        return OK;
    }
    return ERROR;
}

// Devuelve la altitud en metros si está disponible, -1 si no
float gpsGetAltitude(void) {
    return latestFix.altitudeReady ? latestFix.altitude : -1.0f;
}

// Devuelve la velocidad en km/h si está disponible, -1 si no
float gpsGetSpeedKmh(void) {
    return latestFix.speedReady ? latestFix.speed * 1.852f : -1.0f;
}

// Devuelve el número de satélites si está disponible, -1 si no
int gpsGetSatellites(void) {
    return latestFix.satReady ? latestFix.satellites : -1;
}

// Indica si el GPS tiene un fix válido de posición
bool gpsHasFix(void) {
    return latestFix.positionReady;
}

// Indica si el fix es válido para almacenar datos (UBLOX NEO M8N)
bool gpsIsFixValid(void) {
    // Para UBLOX NEO M8N, un fix válido requiere:
    // 1. Posición lista
    // 2. Coordenadas no cero
    // 3. Coordenadas dentro de rangos válidos
    // 4. Al menos 4 satélites para 3D fix
    
    if (!latestFix.positionReady) {
        return false;
    }
    
    // Verificar coordenadas válidas
    if (latestFix.latitude == 0.0 || latestFix.longitude == 0.0) {
        return false;
    }
    
    // Verificar rangos de coordenadas
    if (latestFix.latitude < -90.0 || latestFix.latitude > 90.0 ||
        latestFix.longitude < -180.0 || latestFix.longitude > 180.0) {
        return false;
    }
    
    // Para un fix 3D confiable, necesitamos al menos 4 satélites
    if (latestFix.satReady && latestFix.satellites < 4) {
        return false;
    }
    
    return true;
}

// Nueva función: Obtiene la calidad del fix actual
int gpsGetFixQuality(void) {
    // Devuelve la calidad del último fix parseado
    // 0 = sin fix, 1 = GPS fix, 2 = DGPS fix, etc.
    return latestFix.fixQuality;
}

// Nueva función: Verifica si tenemos suficientes satélites
bool gpsHasGoodSatelliteCount(void) {
    return (latestFix.satReady && latestFix.satellites >= 4);
}

// Guarda en archivo la línea NMEA completa si hay fix disponible
STATUS gpsLogFix(const char *nmea) {
    if (!gpsIsFixValid()) return OK;  

    FILE *f = fopen("/sd0a/gpslog.txt", "a");
    if (!f) return ERROR;

    fprintf(f, "%s\n", nmea);  // Guarda la línea cruda completa
    fclose(f);

    return OK;
}

// Cierra el descriptor de archivo del GPS
void gpsShutdown(void) {
    if (gpsFd >= 0) {
        close(gpsFd);
        gpsFd = -1;
    }
}
