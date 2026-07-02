#ifndef __MPU9250_DRIVER_H__   // Evita inclusión múltiple del archivo header
#define __MPU9250_DRIVER_H__

/**********************************************/
/*           Inclusiones del Sistema         */
/**********************************************/

// Inclusiones necesarias del sistema y librerías para soporte de VxWorks e I2C
#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <taskLib.h>
#include <hwif/vxBus.h>
#include <string.h>
#include <hwif/buslib/vxbI2cLib.h>
#include <vxbFdtBcm2711I2c.h>
#include <sysLib.h>

/**********************************************/
/*           Definiciones del MPU9250        */
/**********************************************/

// Dirección I2C del MPU9250 en el bus (configuración por defecto)
#define MPU9250_I2C_ADDR     0x68

// Registro de identificación del dispositivo (debe devolver 0x71 o 0x73 para MPU9250)
#define MPU9250_WHO_AM_I_REG 0x75

// Registro de gestión de energía (se usa para despertar el sensor desde modo sleep)
#define MPU9250_PWR_MGMT_1   0x6B

// Registro alto de datos del eje X del acelerómetro (los siguientes registros son contiguos)
#define MPU9250_ACCEL_XOUT_H 0x3B

// Registro alto de datos del eje X del giroscopio (los siguientes registros son contiguos)
#define MPU9250_GYRO_XOUT_H  0x43

/**********************************************/
/*           Definiciones del AK8963         */
/**********************************************/

// Dirección I2C del magnetómetro AK8963 (componente interno del MPU9250)
#define AK8963_I2C_ADDR 0x0C

// Registro de control 1 del AK8963 (configura modo de operación y resolución)
#define AK8963_CNTL1    0x0A

// Registro de estado 1 del AK8963 (indica si hay datos nuevos disponibles para lectura)
#define AK8963_ST1      0x02

// Registro bajo de datos del eje X del magnetómetro (los siguientes son contiguos)
#define AK8963_XOUT_L   0x03

// Registro de control 2 del AK8963 (usado para reset del dispositivo)
#define AK8963_CNTL2    0x0B  

// Registro de estado 2 del AK8963 (verificación de overflow en mediciones)
#define AK8963_ST2      0x09  

/**********************************************/
/*           Estructuras de Datos            */
/**********************************************/

/**
 * Estructura que representa al dispositivo MPU9250 en el sistema VxBus.
 * Contiene los identificadores necesarios para comunicación con el dispositivo
 * y su controlador I2C padre.
 */
typedef struct mpu9250_dev {
    VXB_DEV_ID pDev;         // ID del dispositivo MPU9250 en VxBus
    VXB_DEV_ID i2cBusCtrl;   // ID del controlador I2C padre
} MPU9250_DEV;

/**********************************************/
/*           Variables Externas              */
/**********************************************/

// Declaración externa del driver VxBus del MPU9250 (definido en el archivo .c)
extern VXB_DRV mpu9250Drv;

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************/
/*           Funciones de Control            */
/**********************************************/

/**
 * Despierta al MPU9250 desde modo de bajo consumo.
 * Escribe en el registro de gestión de energía para activar el dispositivo.
 * 
 * @param pDev ID del dispositivo VxBus
 * @return OK si la operación fue exitosa, ERROR en caso contrario
 */
STATUS mpu9250WakeUp(VXB_DEV_ID pDev);

/**********************************************/
/*           Funciones de Lectura            */
/**********************************************/

/**
 * Lee datos del acelerómetro y los almacena en las variables proporcionadas.
 * Obtiene mediciones de aceleración en los tres ejes espaciales.
 * 
 * @param pDev ID del dispositivo VxBus
 * @param ax   Puntero para almacenar aceleración en eje X
 * @param ay   Puntero para almacenar aceleración en eje Y
 * @param az   Puntero para almacenar aceleración en eje Z
 * @return OK si la lectura fue exitosa, ERROR en caso contrario
 */
STATUS mpu9250ReadAccel(VXB_DEV_ID pDev, int16_t *ax, int16_t *ay, int16_t *az);

/**
 * Lee datos del giroscopio y los almacena en las variables proporcionadas.
 * Obtiene mediciones de velocidad angular en los tres ejes espaciales.
 * 
 * @param pDev ID del dispositivo VxBus
 * @param gx   Puntero para almacenar velocidad angular en eje X
 * @param gy   Puntero para almacenar velocidad angular en eje Y
 * @param gz   Puntero para almacenar velocidad angular en eje Z
 * @return OK si la lectura fue exitosa, ERROR en caso contrario
 */
STATUS mpu9250ReadGyro(VXB_DEV_ID pDev, int16_t *gx, int16_t *gy, int16_t *gz);

/**********************************************/
/*           Funciones del Magnetómetro      */
/**********************************************/

/**
 * Inicializa el magnetómetro AK8963 interno del MPU9250.
 * Configura el bypass I2C y establece el modo de operación del magnetómetro.
 * 
 * @param pDev ID del dispositivo VxBus
 * @return OK si la inicialización fue exitosa, ERROR en caso contrario
 */
STATUS ak8963Init(VXB_DEV_ID pDev);

/**
 * Lee datos del magnetómetro y los almacena en las variables proporcionadas.
 * Obtiene mediciones de campo magnético en los tres ejes espaciales.
 * 
 * @param pDev ID del dispositivo VxBus
 * @param mx   Puntero para almacenar campo magnético en eje X
 * @param my   Puntero para almacenar campo magnético en eje Y
 * @param mz   Puntero para almacenar campo magnético en eje Z
 * @return OK si la lectura fue exitosa, ERROR en caso contrario
 */
STATUS ak8963ReadMag(VXB_DEV_ID pDev, int16_t *mx, int16_t *my, int16_t *mz);

/**********************************************/
/*           Funciones del Driver VxBus      */
/**********************************************/

/**
 * Función de adjuntado del driver (llamada automáticamente por VxBus).
 * Inicializa la estructura de control del dispositivo y realiza configuración inicial.
 * 
 * @param pDev ID del dispositivo VxBus a inicializar
 * @return OK si la inicialización fue exitosa, ERROR en caso contrario
 */
STATUS mpu9250Attach(VXB_DEV_ID pDev);

/**
 * Función de detección del dispositivo durante el escaneo del bus.
 * Verifica si el dispositivo es compatible con este driver mediante Device Tree.
 * 
 * @param pDev ID del dispositivo VxBus a verificar
 * @return OK si el dispositivo es compatible, ERROR en caso contrario
 */
STATUS mpu9250Probe(VXB_DEV_ID pDev);

/**
 * Función que lee un registro del sensor MPU-9250.
 * 
 * @param pDev ID del dispositivo VxBus a verificar
 * @return OK si el dispositivo es compatible, ERROR en caso contrario
 */
STATUS mpu9250ReadReg(VXB_DEV_ID pDev, UINT8 addr, UINT8 *val);

#ifdef __cplusplus
}
#endif

#endif /* __MPU9250_DRIVER_H__ */
