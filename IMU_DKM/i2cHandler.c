#include <vxWorks.h>
#include <stdio.h>
#include "taskLib.h"
#include <hwif/vxBus.h>
#include <string.h>
#include <hwif/buslib/vxbI2cLib.h>
#include <subsys/timer/vxbTimerLib.h>
#include <inttypes.h>
#include <vxbFdtBcm2711I2c.h>
#include <vsbConfig.h>
#include <semLib.h>
#include <intLib.h>

#include <hwif/buslib/vxbFdtI2cLib.h>
#include <subsys/int/vxbIntLib.h>
#include <subsys/timer/vxbTimerLib.h>
#include <subsys/clk/vxbClkLib.h>
#include <subsys/pinmux/vxbPinMuxLib.h>
#include <hwif/vxBus/vxbMethod.h> 
#include <sysLib.h>
#include <subsys/gpio/vxbGpioLib.h>
#include <hwif/methods/vxbI2cMethod.h>

// Inclusiones de headers del hardware específico
#include "vxbFdtBcm2711I2c.h"
#include "vxbFdtBcm2711Gpio.h"

// Header con definiciones y estructuras específicas del MPU9250
#include "i2cHandler.h"

// Tabla de coincidencias para v	incular el driver con el nodo del Device Tree
const VXB_FDT_DEV_MATCH_ENTRY mpu9250Match[] = {
    { "invensense,mpu9250", 0 }, // Nombre compatible del device tree
    { NULL, 0 } // Fin de la tabla
};

/**********************************************/
/*        Implementación de Funciones I2C    */
/**********************************************/

// Escribe un valor en un registro del MPU9250
STATUS mpu9250WriteReg(VXB_DEV_ID pDev, UINT8 addr, UINT8 val)
{
    UINT8 buf[2] = { addr, val }; // Dirección del registro seguido del valor
    I2C_MSG msg = { .addr = MPU9250_I2C_ADDR, .buf = buf, .len = 2, .flags = I2C_M_WR };
    return vxbI2cDevXfer(pDev, &msg, 1); // Transferencia I2C
}

// Verifica si el dispositivo es compatible con este driver (coincidencia DT)
STATUS mpu9250Probe(VXB_DEV_ID pDev)
{
    return vxbFdtDevMatch(pDev, mpu9250Match, NULL);
}

// Lee un valor de un registro del MPU9250
STATUS mpu9250ReadReg(VXB_DEV_ID pDev, UINT8 addr, UINT8 *val)
{
    I2C_MSG msgs[2] = {
        { .addr = MPU9250_I2C_ADDR, .buf = &addr, .len = 1, .flags = I2C_M_WR }, // Dirección del registro
        { .addr = MPU9250_I2C_ADDR, .buf = val,  .len = 1, .flags = I2C_M_RD } // Lectura del valor
    };
    return vxbI2cDevXfer(pDev, msgs, 2);
}

// Despierta al MPU9250 desde modo de bajo consumo
STATUS mpu9250WakeUp(VXB_DEV_ID pDev)
{
    return mpu9250WriteReg(pDev, MPU9250_PWR_MGMT_1, 0x00); // Escritura en registro de gestión de energía
}

// Lee valores del acelerómetro (3 ejes)
STATUS mpu9250ReadAccel(VXB_DEV_ID pDev, int16_t *ax, int16_t *ay, int16_t *az)
{
    UINT8 reg = MPU9250_ACCEL_XOUT_H; // Registro de inicio de datos de acelerómetro
    UINT8 data[6]; // Datos en bruto: 2 bytes por eje
    I2C_MSG msgs[2] = {
        { .addr = MPU9250_I2C_ADDR, .buf = &reg, .len = 1, .flags = I2C_M_WR },
        { .addr = MPU9250_I2C_ADDR, .buf = data, .len = 6, .flags = I2C_M_RD }
    };

    if (vxbI2cDevXfer(pDev, msgs, 2) != OK) return ERROR;
    
    // Combina los bytes alto y bajo para cada eje
    *ax = (int16_t)((data[0] << 8) | data[1]);
    *ay = (int16_t)((data[2] << 8) | data[3]);
    *az = (int16_t)((data[4] << 8) | data[5]);
    return OK;
}

// Lee valores del giroscopio (3 ejes)
STATUS mpu9250ReadGyro(VXB_DEV_ID pDev, int16_t *gx, int16_t *gy, int16_t *gz)
{
    UINT8 reg = MPU9250_GYRO_XOUT_H; // Registro de inicio de datos de giroscopio
    UINT8 data[6];
    I2C_MSG msgs[2] = {
        { .addr = MPU9250_I2C_ADDR, .buf = &reg, .len = 1, .flags = I2C_M_WR },
        { .addr = MPU9250_I2C_ADDR, .buf = data, .len = 6, .flags = I2C_M_RD }
    };

    if (vxbI2cDevXfer(pDev, msgs, 2) != OK) return ERROR;

    *gx = (int16_t)((data[0] << 8) | data[1]);
    *gy = (int16_t)((data[2] << 8) | data[3]);
    *gz = (int16_t)((data[4] << 8) | data[5]);
    return OK;
}

// Inicializa el magnetómetro AK8963 (interno al MPU9250)
STATUS ak8963Init(VXB_DEV_ID pDev) {
    // Enable I2C bypass
    if (mpu9250WriteReg(pDev, 0x37, 0x02) != OK) return ERROR;
    taskDelay(sysClkRateGet() / 2); // 500ms instead of 100ms
    
    // Reset magnetometer first
    UINT8 reset[2] = { AK8963_CNTL2, 0x01 };
    I2C_MSG resetMsg = { .addr = AK8963_I2C_ADDR, .buf = reset, .len = 2, .flags = I2C_M_WR };
    vxbI2cDevXfer(pDev, &resetMsg, 1);
    taskDelay(sysClkRateGet() / 2); // Wait for reset
    
    // Configure single measurement mode
    UINT8 cfg[2] = { AK8963_CNTL1, 0x11 }; // Single measurement, 16-bit
    I2C_MSG msg = { .addr = AK8963_I2C_ADDR, .buf = cfg, .len = 2, .flags = I2C_M_WR };
    if (vxbI2cDevXfer(pDev, &msg, 1) != OK) return ERROR;
    
    taskDelay(sysClkRateGet() / 2); // Longer stabilization
    return OK;
}

// Lee valores del magnetómetro (3 ejes)
STATUS ak8963ReadMag(VXB_DEV_ID pDev, int16_t *mx, int16_t *my, int16_t *mz) {
    // Step 1: Trigger single measurement
    UINT8 trigger[2] = { AK8963_CNTL1, 0x11 };
    I2C_MSG triggerMsg = { .addr = AK8963_I2C_ADDR, .buf = trigger, .len = 2, .flags = I2C_M_WR };
    if (vxbI2cDevXfer(pDev, &triggerMsg, 1) != OK) {
        *mx = *my = *mz = 0;
        return ERROR;
    }

    // Step 2: Wait for measurement completion (AK8963 needs ~9ms)
    taskDelay(sysClkRateGet() / 50); // 20ms delay

    // Step 3: Check if data is ready
    UINT8 st1 = AK8963_ST1, val = 0;
    I2C_MSG st1Msgs[2] = {
        { .addr = AK8963_I2C_ADDR, .buf = &st1, .len = 1, .flags = I2C_M_WR },
        { .addr = AK8963_I2C_ADDR, .buf = &val, .len = 1, .flags = I2C_M_RD }
    };
    if (vxbI2cDevXfer(pDev, st1Msgs, 2) != OK || (val & 0x01) == 0) {
        *mx = *my = *mz = 0;
        return ERROR;
    }

    // Step 4: Read the magnetic data (keep your existing logic)
    UINT8 reg = AK8963_XOUT_L;
    UINT8 data[6];
    I2C_MSG msgs[2] = {
        { .addr = AK8963_I2C_ADDR, .buf = &reg, .len = 1, .flags = I2C_M_WR },
        { .addr = AK8963_I2C_ADDR, .buf = data, .len = 6, .flags = I2C_M_RD }
    };

    if (vxbI2cDevXfer(pDev, msgs, 2) != OK) {
        *mx = *my = *mz = 0;
        return ERROR;
    }

    // Step 5: Convert data (your existing conversion is correct)
    *mx = (int16_t)((data[1] << 8) | data[0]);
    *my = (int16_t)((data[3] << 8) | data[2]);
    *mz = (int16_t)((data[5] << 8) | data[4]);
    return OK;
}

// Función de attach del driver: inicializa estructura de control y despierta el sensor
STATUS mpu9250Attach(VXB_DEV_ID pDev)
{
    MPU9250_DEV *pDrvCtrl = calloc(1, sizeof(MPU9250_DEV));
    if (!pDrvCtrl) return ERROR;

    vxbDevSoftcSet(pDev, pDrvCtrl);
    pDrvCtrl->pDev = pDev;
    pDrvCtrl->i2cBusCtrl = vxbDevParent(pDev);

    if (mpu9250WakeUp(pDev) != OK) goto error;
    taskDelay(sysClkRateGet() / 10);

    UINT8 whoAmI;
    if (mpu9250ReadReg(pDev, MPU9250_WHO_AM_I_REG, &whoAmI) != OK) goto error;
    

    return OK;

error:
    free(pDrvCtrl);
    return ERROR;
}


// Tabla de métodos del driver para el sistema VxBus
VXB_DRV_METHOD mpu9250Methods[] = {
    { VXB_DEVMETHOD_CALL(vxbDevProbe),  mpu9250Probe },
    { VXB_DEVMETHOD_CALL(vxbDevAttach), mpu9250Attach },
    { 0, NULL }
};

// Registro final del driver MPU9250
VXB_DRV mpu9250Drv =
{
    { NULL },
    "mpu9250",
    "MPU9250 I2C Driver",
    VXB_BUSID_FDT,
    0,
    0,
    mpu9250Methods,
    (void *)mpu9250Match

};

