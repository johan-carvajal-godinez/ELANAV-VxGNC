// Inclusiones necesarias del sistema y subsistemas de VxWorks
#include <vxWorks.h>
#include <stdio.h>
#include <string.h>
#include <hwif/vxBus.h>
#include <hwif/vxbus/vxbLib.h>
#include <hwif/buslib/vxbFdtI2cLib.h>
#include <semLib.h>
#include <taskLib.h>
#include <sysLib.h>
#include <stdlib.h>

// Inclusiones específicas del sistema de buses y métodos I2C
#include <hwif/buslib/vxbI2cLib.h>
#include <hwif/buslib/vxbFdtLib.h>
#include <hwif/methods/vxbI2cMethod.h>
#include <iosLib.h>
#include <subsys/int/vxbIntLib.h>

// Driver específico de BCM2711 para I2C (Raspberry Pi 4)
#include <vxbFdtBcm2711I2c.h>
#include <subsys/timer/vxbTimerLib.h>

// Header del handler y driver personalizado del MPU9250
#include "i2cHandler.h"

// Variable global que almacenará el valor del registro WHO_AM_I
UINT8 whoAmI;

// Función principal para inicializar el sensor IMU
void startIMU(void)
{
    // Verifica si el driver ya está registrado o está huérfano (no asociado aún)
    if (vxbDrvOrphanCheck("mpu9250"))
    {
        printf("[DKM] Registrando driver MPU9250...\n");

        // Agrega el driver a VxBus
        if (vxbDrvAdd(&mpu9250Drv) == OK)
        {
            printf("[DKM] Registro correcto del driver MPU9250.\n");
        }
        else
        {
            printf("[DKM] Fallo al registrar el driver MPU9250.\n");
            return;
        }
    }
    else
    {
        printf("[DKM] Driver MPU9250 ya registrado o en uso.\n");
    }

    // Espera un poco para asegurar que el sistema procese el registro
    taskDelay(sysClkRateGet() / 2);  // Espera de medio segundo

    // Adquiere el dispositivo MPU9250 por nombre
    VXB_DEV_ID pDev = vxbDevAcquireByName("mpu9250", 0);
    if (pDev == NULL)
    {
        printf("[DKM] El dispositivo no se asocio con el driver.\n");
        return;
    }

    printf("[DKM] Dispositivo MPU9250 encontrado.\n\n");

    // Llama manualmente a la función de attach si no fue llamada automáticamente
    if (mpu9250Attach(pDev) != OK)
    {
        printf("[DKM] Error al ejecutar vxbDevAttach().\n");
    }

    // Imprime el valor leído del registro WHO_AM_I (esperado: 0x71 o 0x73)
    printf("WHO_AM_I = 0x%02X\n", whoAmI);

    // Variables para almacenar los datos de los sensores
    int16_t ax, ay, az, gx, gy, gz, mx, my, mz;

    // Lectura de datos del acelerómetro
    if (mpu9250ReadAccel(pDev, &ax, &ay, &az) == OK)
        printf("Acelerometro: X=%d Y=%d Z=%d\n", ax, ay, az);

    // Lectura de datos del giroscopio
    if (mpu9250ReadGyro(pDev, &gx, &gy, &gz) == OK)
        printf("Giroscopio: X=%d Y=%d Z=%d\n", gx, gy, gz);

    // Inicialización y lectura del magnetómetro AK8963
    if (ak8963Init(pDev) == OK && ak8963ReadMag(pDev, &mx, &my, &mz) == OK)
        printf("Magnetometro: X=%d Y=%d Z=%d\n", mx, my, mz);
}
