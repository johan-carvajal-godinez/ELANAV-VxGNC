// gpsHandler.h
#ifndef GPS_HANDLER_H
#define GPS_HANDLER_H

#include <vxWorks.h>
#include <stdbool.h>

/**********************************************/
/*           Estructuras de Datos GPS        */
/**********************************************/

/**
 * Estructura que representa un fix (posición) GPS completo.
 * Cada campo tiene una bandera booleana asociada que indica si el valor 
 * fue obtenido exitosamente del último mensaje NMEA procesado.
 * 
 * Esta estructura centraliza toda la información GPS disponible y permite
 * verificar la validez de cada componente individualmente.
 */
typedef struct {
    char time[16];      /* Hora UTC del fix en formato hhmmss.sss */
    float latitude;     /* Latitud en grados decimales (positiva al norte del ecuador) */
    float longitude;    /* Longitud en grados decimales (positiva al este de Greenwich) */
    float altitude;     /* Altitud sobre el nivel del mar en metros */
    float speed;        /* Velocidad sobre el terreno en nudos */
    int satellites;     /* Número de satélites utilizados para calcular el fix */
    int fixQuality;     /* Calidad del fix GPS (0=sin fix, 1=GPS, 2=DGPS, etc.) */

    /* Banderas de validez para cada campo de datos */
    BOOL timeReady;         /* TRUE si el campo 'time' contiene datos válidos */
    BOOL positionReady;     /* TRUE si 'latitude' y 'longitude' son válidos */
    BOOL altitudeReady;     /* TRUE si el campo 'altitude' es válido */
    BOOL speedReady;        /* TRUE si el campo 'speed' es válido */
    BOOL satReady;          /* TRUE si el campo 'satellites' es válido */
} GpsFix;

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************/
/*           Funciones de Inicialización     */
/**********************************************/

/**
 * Inicializa el dispositivo GPS y configura el puerto UART.
 * Abre el dispositivo serie, establece la velocidad de transmisión
 * y prepara el sistema para recibir datos NMEA.
 *
 * @return OK si se inicializó correctamente, ERROR si falló.
 */
STATUS gpsInit(void);

/**********************************************/
/*           Funciones de Lectura            */
/**********************************************/

/**
 * Lee una línea NMEA completa del dispositivo GPS.
 * Procesa caracteres hasta encontrar un terminador de línea (\n)
 * y filtra caracteres de control no deseados.
 *
 * @param buf      Buffer donde se almacenará la línea leída.
 * @param maxLen   Longitud máxima del buffer incluyendo terminador null.
 * @return OK si se leyó exitosamente una línea, ERROR si no.
 */
STATUS gpsReadLine(char *buf, int maxLen);

/**********************************************/
/*           Funciones de Parsing NMEA       */
/**********************************************/

/**
 * Parsea un mensaje NMEA tipo GGA para obtener información de posición y altitud.
 * Extrae hora UTC, coordenadas, calidad del fix, número de satélites y altitud.
 * Realiza validación exhaustiva de todos los campos antes de aceptar los datos.
 *
 * @param nmea     Cadena NMEA que comienza con $GPGGA o $GNGGA.
 * @return OK si el parsing fue exitoso, ERROR si no.
 */
STATUS gpsParseGGA(const char *nmea);

/**
 * Parsea un mensaje NMEA tipo RMC para obtener información de velocidad.
 * Extrae la velocidad sobre el terreno y otros datos de navegación.
 *
 * @param nmea     Cadena NMEA que comienza con $GPRMC o $GNRMC.
 * @return OK si el parsing fue exitoso, ERROR si no.
 */
STATUS gpsParseRMC(const char *nmea);

/**********************************************/
/*           Funciones de Acceso a Datos     */
/**********************************************/

/**
 * Devuelve un puntero a la estructura que contiene el último fix GPS.
 * Permite acceso directo a todos los campos de datos y sus banderas de validez.
 *
 * @return Puntero a estructura GpsFix con los últimos datos disponibles.
 */
GpsFix* gpsGetFix(void);

/**
 * Obtiene la hora UTC del último fix GPS.
 * Retorna una representación en cadena de texto de la hora.
 *
 * @return Cadena de texto con la hora o "N/A" si no está disponible.
 */
const char* gpsGetTime(void);

/**
 * Obtiene la latitud y longitud del último fix válido.
 * Solo retorna coordenadas si el fix de posición es confiable.
 *
 * @param lat    Puntero donde se almacenará la latitud en grados decimales.
 * @param lon    Puntero donde se almacenará la longitud en grados decimales.
 * @return OK si se obtuvieron ambos valores, ERROR si no hay fix válido.
 */
STATUS gpsGetLatLon(float *lat, float *lon);

/**
 * Obtiene la altitud del último fix en metros sobre el nivel del mar.
 *
 * @return Altitud en metros o -1.0f si no está disponible.
 */
float gpsGetAltitude(void);

/**
 * Obtiene la velocidad convertida a kilómetros por hora.
 * Convierte automáticamente desde nudos (unidad NMEA estándar) a km/h.
 *
 * @return Velocidad en kilómetros por hora o -1.0f si no está disponible.
 */
float gpsGetSpeedKmh(void);

/**
 * Devuelve el número de satélites usados en el último fix.
 * Mayor número de satélites generalmente indica mejor precisión.
 *
 * @return Número de satélites o -1 si no está disponible.
 */
int gpsGetSatellites(void);

/**********************************************/
/*           Funciones de Validación         */
/**********************************************/

/**
 * Verifica si el GPS tiene un fix válido de posición básico.
 * Realiza validación mínima para determinar disponibilidad de coordenadas.
 *
 * @return true si la posición está disponible, false si no.
 */
bool gpsHasFix(void);

/**
 * Verifica si el fix GPS es válido para almacenar datos (validación exhaustiva).
 * Diseñada específicamente para UBLOX NEO M8N, realiza validación completa:
 * coordenadas válidas, suficientes satélites, rangos geográficos correctos.
 *
 * @return true si el fix es confiable para almacenar, false si no.
 */
bool gpsIsFixValid(void);

/**
 * Obtiene la calidad del fix GPS actual según estándar NMEA.
 * Permite determinar el tipo y confiabilidad del fix recibido.
 *
 * @return Calidad del fix (0=sin fix, 1=GPS, 2=DGPS, 3=PPS, etc.)
 */
int gpsGetFixQuality(void);

/**
 * Verifica si tenemos suficientes satélites para un fix confiable.
 * Un fix 3D confiable requiere al menos 4 satélites.
 *
 * @return true si hay 4+ satélites, false si no.
 */
bool gpsHasGoodSatelliteCount(void);

/**********************************************/
/*           Funciones de Logging            */
/**********************************************/

/**
 * Guarda una línea NMEA en un archivo de log si hay fix válido.
 * Realiza validación interna antes de escribir para asegurar calidad de datos.
 *
 * @param nmea   Cadena NMEA original a registrar en el archivo.
 * @return OK si se guardó exitosamente, ERROR si falló.
 */
STATUS gpsLogFix(const char *nmea);

/**********************************************/
/*           Funciones de Limpieza           */
/**********************************************/

/**
 * Cierra el dispositivo GPS y libera todos los recursos asociados.
 * Debe llamarse al finalizar el uso del GPS para evitar fugas de recursos.
 */
void gpsShutdown(void);

#ifdef __cplusplus
}
#endif

#endif // GPS_HANDLER_H
