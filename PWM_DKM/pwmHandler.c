#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <taskLib.h>
#include <string.h>


// Direccion fisica del PWM0 en BCM2711 (0x7E20C000 en bus periferico -> 0xFE20C000 en ARM)
#define PWM_BASE_PHYS      0xFE20C000
#define PWM_REG_SIZE       0x28        

// Offsets de registros PWM (consultado en la documentacion del SoC)
#define PWM_CTL_OFFSET     0x00
#define PWM_STA_OFFSET     0x04
#define PWM_DMAC_OFFSET    0x08
#define PWM_RNG1_OFFSET    0x10
#define PWM_DAT1_OFFSET    0x14
#define PWM_FIF1_OFFSET    0x18
#define PWM_RNG2_OFFSET    0x20
#define PWM_DAT2_OFFSET    0x24

// Bits de control
#define PWM_CTL_PWEN1      (1 << 0)    // Habilita PWM canal 1
#define PWM_CTL_PWEN2      (1 << 8)    // Habilita PWM canal 2


// FUNCIONES DE INICIALIZACION


// Cambio de duty dinamicamente
void pwmSetDuty(){
	
};

// Activar el PWM
void pwmDkmInit(){

};

// Desactivar el PWM
void pwmDkmShutdown(){
   
};
