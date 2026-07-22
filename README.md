# ELANAV-VxGNC

Componentes embebidos de guía, navegación y control (GNC) para VxWorks 21.07 sobre Raspberry Pi 4 Model B, construidos sobre el paradigma multiagente **MAES** mediante la biblioteca **VxMAES**.

Desarrollado en el Laboratorio de Sistemas Espaciales (SETEC Lab), Escuela de Ingeniería Electrónica, Instituto Tecnológico de Costa Rica.

---

## Contenido

1. [Descripción](#1-descripción)
2. [Requisitos](#2-requisitos)
3. [Estructura del repositorio](#3-estructura-del-repositorio)
4. [Arquitectura](#4-arquitectura)
5. [Conexión física](#5-conexión-física)
6. [Configuración del árbol de dispositivos](#6-configuración-del-árbol-de-dispositivos)
7. [Construcción](#7-construcción)
8. [Ejecución](#8-ejecución)
9. [Formato de los datos generados](#9-formato-de-los-datos-generados)
10. [Referencia de API](#10-referencia-de-api)
11. [Estado de implementación](#11-estado-de-implementación)
12. [Limitaciones conocidas](#12-limitaciones-conocidas)
13. [Archivos requeridos no versionados](#13-archivos-requeridos-no-versionados)
14. [Licencia y créditos](#14-licencia-y-créditos)

---

## 1. Descripción

Este repositorio contiene las **bibliotecas de acceso a hardware** (device drivers) que habilitan los sensores mínimos de un subsistema GNC en la computadora de a bordo del SETEC Lab, junto con una aplicación de validación multiagente.

**Alcance implementado:**

- Adquisición de imágenes desde cámara USB compatible con la clase UVC.
- Determinación de posición geográfica mediante receptor GNSS u-blox NEO-M8N (tramas NMEA por UART).
- Lectura de aceleración, velocidad angular y campo magnético con una IMU MPU-9250 de 9 grados de libertad (I²C).
- Nodo de árbol de dispositivos preparado para el módulo PWM interno del BCM2711 (futuros actuadores).
- Aplicación RTP multiagente que orquesta la adquisición concurrente de los tres sensores.

**Fuera de alcance:** este repositorio **no implementa algoritmos GNC**. No hay estimador de actitud, filtro de fusión sensorial, odometría visual ni lazo de control. La capa entregada es la interfaz entre el hardware y VxMAES, sobre la cual esos algoritmos deben construirse.

---

## 2. Requisitos

### 2.1 Hardware

| Componente | Modelo validado | Interfaz |
|---|---|---|
| Computadora de a bordo | Raspberry Pi 4 Model B (SoC BCM2711, Cortex-A72) | — |
| Cámara | Logitech C270 (cualquier dispositivo UVC) | USB |
| Receptor GNSS | u-blox NEO-M8N con antena externa | UART3 @ 9600 bd |
| IMU | InvenSense MPU-9250 (con AK8963 integrado) | I²C1 @ 100 kHz, dir. 0x68 |
| Consola serie | Convertidor FT232 | UART0 @ 115200 bd |
| Almacenamiento | Tarjeta microSD formateada en FAT32 | — |

VxWorks no ofrece salida HDMI en esta plataforma: **la consola serie es obligatoria**.

### 2.2 Software

- Wind River **VxWorks 21.07** con licencia válida y BSP `rpi_4_0_1_2_0_UNSUPPORTED`.
- Wind River **Workbench 4**.
- Biblioteca **VxMAES** (dependencia externa, no incluida en este repositorio).
- Firmware de arranque de Raspberry Pi, commit `d9c382e0f3a546e9da153673dce5dd4ba1200994` del repositorio `raspberrypi/firmware`.
- Python 3 con `numpy`, `matplotlib` y `opencv-python` para el análisis *offline* de los datos capturados.

---

## 3. Estructura del repositorio

```
ELANAV-VxGNC/
├── GNC_TEST/          Aplicación RTP multiagente de validación
│   └── gnctest.c              4 agentes MAES + protocolo de colas       (555 LOC)
├── USB_DKM/           Driver de cámara UVC
│   ├── usbHandler.c/.h        Apertura, captura de frame, cierre        (250 LOC)
│   ├── usbLauncher.c          Punto de entrada startUSB()
│   └── usbMessageHandler.c/.h Tarea de colas de mensajes                (282 LOC)
├── GPS_DKM/           Driver GNSS NMEA
│   ├── gpsHandler.c/.h        UART, parsing GGA/RMC, logging            (523 LOC)
│   ├── gpsLauncher.c          Punto de entrada startGPS()
│   └── gpsMessageHandler.c/.h Tarea de colas + tarea lectora            (383 LOC)
├── IMU_DKM/           Driver MPU-9250 tipo vxBus
│   ├── i2cHandler.c/.h        Probe/attach, registros, AK8963           (404 LOC)
│   ├── i2cLauncher.c          Punto de entrada startIMU()
│   └── i2cMessageHandler.c/.h Tarea de colas + tarea lectora            (419 LOC)
└── PWM_DKM/           Plantilla para el módulo PWM
    └── pwmHandler.c           Offsets de registro; funciones vacías      (43 LOC)
```

Cada directorio incluye además el subárbol `VSBraspi_CORTEX_A72llvm_LP64_LARGE/` con los artefactos de compilación generados por Workbench. **No son necesarios para reconstruir el proyecto** y pueden ignorarse.

Cada DKM sigue una estructura de tres capas consistente:

| Archivo | Responsabilidad |
|---|---|
| `*Handler.c/.h` | Acceso al hardware: registros, protocolo de bus, conversión de datos |
| `*Launcher.c` | Punto de entrada invocable desde la consola para prueba directa del driver |
| `*MessageHandler.c/.h` | Tarea de servicio con colas de mensajes para diálogo con el RTP |

---

## 4. Arquitectura

### 4.1 Modelo de ejecución en dos espacios

Los drivers se ejecutan en **espacio de kernel** (acceso directo al hardware, sin sobrecarga de conmutación) y la lógica de misión en **espacio de usuario** (memoria protegida, depuración aislada). Ambos dominios se comunican exclusivamente por colas de mensajes de VxWorks.

```
┌─────────────────────── ESPACIO DE USUARIO (RTP) ───────────────────────┐
│  GNC_TEST.vxe                                                          │
│    CameraAgent (150)   GpsAgent (150)   ImuAgent (150)                 │
│                     Coordinator (140)                                  │
└────────────┬─────────────────┬─────────────────┬───────────────────────┘
   /CameraCmd│/CameraResp /GpsCmd│/GpsResp   /ImuCmd│/ImuResp
┌────────────┴─────────────────┴─────────────────┴───────────────────────┐
│                        ESPACIO DE KERNEL (DKM)                         │
│    tHandler (90)      tGpsHandler (90)     tImuHandler (90)            │
│                       tGpsReader (100)     tImuReader (100)            │
│         │                    │                    │                    │
│    USB / uvc/0          UART3 /ttyS1          I²C1 0x68 + 0x0C         │
└────────────────────────────────────────────────────────────────────────┘
```

Los números entre paréntesis son prioridades de tarea de VxWorks (**menor número = mayor prioridad**). El esquema garantiza que la adquisición de datos preempta a la lógica de agentes, y que el coordinador preempta a los agentes trabajadores.

### 4.2 Protocolo de mensajes

Seis colas FIFO de profundidad 10, creadas por los DKM con `OM_CREATE` y abiertas por el RTP con `OM_EXCL`:

| Cola | Tamaño de mensaje | Comandos |
|---|---|---|
| `/CameraCmd` · `/CameraResp` | `sizeof(Msg)` | `CMD_OPEN`, `CMD_CAPTURE`, `CMD_CLOSE`, `CMD_RECORD`, `CMD_STATUS` |
| `/GpsCmd` · `/GpsResp` | `sizeof(GpsMsg)` | `GPS_CMD_START`, `GPS_CMD_READ`, `GPS_CMD_STOP`, `GPS_CMD_STATUS` |
| `/ImuCmd` · `/ImuResp` | `sizeof(ImuMsg)` | `IMU_CMD_START`, `IMU_CMD_READ`, `IMU_CMD_STOP`, `IMU_CMD_STATUS` |

El diálogo es **síncrono**: el RTP envía con `msgQSend(..., WAIT_FOREVER)` y bloquea en `msgQReceive` con un *timeout* de 5 s (1 s para consultas de estado). Los *timeouts* se derivan de `sysClkRateGet()` para ser independientes de la frecuencia de tick.

> **Orden de arranque obligatorio.** El RTP no crea las colas: las abre. Los tres manejadores DKM deben iniciarse **antes** de lanzar `GNC_TEST.vxe`, o `msgQOpen` devuelve `NULL` y la aplicación aborta.

### 4.3 Ciclo de vida de la misión

1. El RTP arranca la plataforma VxMAES y abre las seis colas.
2. Registra los cuatro agentes con `platform.agent_init()` / `platform.register_agent()`.
3. Cada agente trabajador envía `*_CMD_START` a su DKM y sondea el progreso cada 3 s.
4. El coordinador consulta el estado global cada 10 s y termina cuando los tres objetivos se cumplen (5 *frames*, 500 lecturas IMU, 500 ciclos GPS), o tras un *timeout* de seguridad de 5 min.

---

## 5. Conexión física

El mapeo de pines de VxWorks **difiere del de Linux**. Estas conexiones solo son válidas con el `rpi-4.dtsi` de este proyecto.

| Señal | Pin físico | GPIO / función |
|---|---|---|
| MPU-9250 VCC | 1 | 3V3 |
| MPU-9250 SDA | 3 | GPIO2 — I2C1_SDA |
| MPU-9250 SCL | 5 | GPIO3 — I2C1_SCL |
| MPU-9250 GND | 9 | Ground |
| MPU-9250 AD0 | 34 | Ground → fija la dirección I²C en `0x68` |
| FT232 GND | 6 | Ground |
| FT232 RX | 8 | GPIO14 — UART0_TXD |
| FT232 TX | 10 | GPIO15 — UART0_RXD |
| NEO-M8N VCC | 17 | 3V3 |
| NEO-M8N GND | 25 | Ground |
| NEO-M8N TX | 29 | GPIO5 — UART3_RXD |

La cámara se conecta a cualquier puerto USB. El pin TX de la Raspberry hacia el GPS (GPIO4) **no se cablea**: el driver abre el puerto en modo `O_RDONLY` y no envía configuración al receptor, que opera con sus valores de fábrica (9600 bd, NMEA).

La antena del NEO-M8N debe orientarse hacia cielo abierto. La precisión se degrada notablemente en presencia de edificaciones cercanas.

---

## 6. Configuración del árbol de dispositivos

Editar `rpi-4.dtsi` en el BSP antes de compilar el VIP.

**UART3 para el GPS** — se expone como `/ttyS1`:

```dts
uart3: serial@fe201600 {
    compatible = "brcm,bcm2711-pl011", "arm,pl011", "arm,primecell";
    reg = <0x0 0xfe201600 0x0 0x0200>;
    interrupts = <153 0 4>;
    interrupt-parent = <&intc>;
    clock-frequency = <48000000>;
    pinmux-0 = <&uart3_pins>;
    fifo-disabled = <1>;
    status = "okay";           /* habilitar */
};
```

**I²C1 con el nodo hijo del MPU-9250** — la cadena `compatible` es la que enlaza el driver:

```dts
i2c1: i2c@3f804000 {
    compatible = "brcm,bcm2711-i2c";
    reg = <0x0 0xfe804000 0x0 0x1000>;
    interrupts = <149 0 4>;
    interrupt-parent = <&intc>;
    def-bus-frequency = <100000>;
    clock-frequency = <500000000>;
    pinmux-0 = <&i2c1_pins>;
    #address-cells = <1>;
    #size-cells = <0>;
    polled-mode = <0>;
    status = "okay";

    mpu9250@68 {
        compatible = "invensense,mpu9250";
        reg = <0x68>;
        status = "okay";
    };
};
```

**PWM (preparado, deshabilitado)** — habilitar solo cuando exista un driver:

```dts
pwm: pwm@7e20c000 {
    compatible = "brcm,bcm2835-pwm";
    reg = <0x0 0xfe20c000 0x0 0x28>;
    assigned-clock-rates = <1000000>;
    #pwm-cells = <2>;
    status = "disabled";
};

pwm_pins: pwmPins {
    pin-set = <18 2 0        /* GPIO18 = PWM0, ALT5, sin pull */
               19 2 0>;      /* GPIO19 = PWM1, ALT5, sin pull */
};
```

---

## 7. Construcción

### 7.1 VxWorks Source Build (VSB)

`File → New → Wind River Workbench Project → VxWorks Source Build`

| Parámetro | Valor |
|---|---|
| BSP | `rpi_4_0_1_2_0_UNSUPPORTED` |
| Active CPU | `CORTEX A72` |
| Debug mode | On, compiler optimizations disabled |
| IP version | IPv6 and IPv4 enabled libraries |
| VSB profile | `DEVELOPMENT` |

En *Source Build Configuration* verificar que estén habilitados `FS` y `FS_ROMFS`. Compilar.

### 7.2 VxWorks Image Project (VIP)

`File → New → ... → VxWorks Image Project`, sobre el VSB anterior, BSP `rpi_4_0_1_2_0`, perfil `PROFILE_RPI4`.

**Bundles** a agregar: `BUNDLE_EDR`, `BUNDLE_RTP_DEVELOP`, `BUNDLE_RUNTIME_ANALYSIS_DEVELOP`, `BUNDLE_STANDALONE_SHELL`.

**Componentes** a incluir:

```
DRV_SIO_FDT_NS16550            DRV_SIO_FDT_ARM_AMBA_PL011
DRV_INTCTLR_FDT_ARM_GIC        DRV_ARM_GEN_TIMER_NG
DRV_FDT_BRCM_2711_PCIE         DRV_GPIO_FDT_BCM2711
DRV_I2C_FDT_BCM2711            DRV_PINMUX_FDT_BCM2711
DRV_SPI_FDT_BCM2711            INCLUDE_USB_XHCI_HCD_INIT
DRV_END_FDT_BCM_GENETv5        DRV_FDT_BRCM_2711_EMMC2
INCLUDE_USB_GEN2_STORAGE_INIT  INCLUDE_DOSFS
INCLUDE_XBD_PART_LIB           DRV_SDSTORAGE_CARD
INCLUDE_ROMFS                  DRV_FSL_I2C
DRV_I2C_RTC                    FOLDER_BUSLIB_SPI
FOLDER_DRIVERS_SPI             INCLUDE_SPI_BUS
DRV_FSL_SPI
```

`DRV_I2C_FDT_BCM2711` es imprescindible: es el driver de controlador sobre el que se apoya el driver de dispositivo del MPU-9250.

### 7.3 Biblioteca compartida VxMAES

Crear un proyecto `Shared User Library` sobre el VSB e importar la carpeta `VxMAESLib` de la distribución de VxMAES. Compilar; si aparece el diálogo de compilación, pulsar *Generate Includes* y aceptar la actualización del índice C/C++.

### 7.4 Módulos de kernel (DKM)

Para cada uno de `USB_DKM`, `GPS_DKM`, `IMU_DKM` (y `PWM_DKM` si se desea):

1. `File → New → ... → Downloadable Kernel Module`, sobre el VSB del proyecto.
2. Clic derecho → `Import → General → File System` → seleccionar los `.c` y `.h` del directorio correspondiente de este repositorio.
3. Compilar.

**Recomendado:** arrastrar cada carpeta de DKM dentro de la carpeta del VIP en el *Project Explorer*. Así se enlazan estáticamente en la imagen y se cargan en cada arranque, evitando el `ld <` manual.

### 7.5 Aplicación RTP

Crear un proyecto `Real Time Process`, importar `GNC_TEST/gnctest.c` y enlazarlo contra la biblioteca compartida VxMAES siguiendo el capítulo 8 del *VxWorks Kernel Application and RTP Application Projects Guide 21.07*.

### 7.6 ROMFS

Crear un proyecto `ROMFS File System` y agregar:

| Archivo | Origen |
|---|---|
| `libVxMAESLib.so` | `workspace/VxMAESLib/VSB_.../libVxMAESLib/Debug` |
| `GNC_TEST.vxe` | `workspace/GNC_TEST/VSB_.../GNC_TEST/Debug` |
| `libc.so.1` | `workspace/VSB/usr/root/<compilador>/bin` |
| `libllvm.so.1` | `workspace/VSB/usr/root/<compilador>/bin` |

Guardar y **recompilar el VIP** para embeber el sistema de archivos en la imagen.

### 7.7 Preparación de la tarjeta SD

```bash
# 1. Formatear la SD en FAT32
# 2. Obtener el firmware en el commit validado
git clone https://github.com/raspberrypi/firmware.git
cd firmware && git checkout d9c382e0f3a546e9da153673dce5dd4ba1200994
# 3. Copiar el contenido de firmware/boot/ a la raíz de la SD
```

Luego copiar el contenido de `C:\WindRiver\VxWorks\21.07\os\unsupported\rpi_4\rpi_4\_sd_card_files` a la SD. **La carpeta `overlays` no debe sobrescribirse**: fusionar su contenido con la ya existente. Finalmente añadir `rpi-4.dtb` y `uVxWorks` desde `workspace/VIP/default`.

---

## 8. Ejecución

### 8.1 Consola serie

Conectar el FT232 según §5 y abrir PuTTY sobre el puerto COM asignado a **115200 bd**. El sistema arranca automáticamente al alimentar la Raspberry Pi con 5 V.

### 8.2 Prueba directa de cada driver

Comprobación rápida de cableado y comunicación, sin agentes ni colas de mensajes:

```
-> startUSB      # captura 3 frames a /sd0a/video.dat
-> startGPS      # bucle de lectura NMEA; imprime fix, coordenadas, altitud, satélites
-> startIMU      # registra el driver vxBus, adquiere el dispositivo, lee los 3 sensores
```

Si los DKM no se enlazaron en el VIP, cargarlos primero:

```
-> cd "/romfs"
-> ld < USB_DKM.out
```

### 8.3 Aplicación multiagente completa

```
-> startUsbHandler        # arranca tHandler y crea /CameraCmd, /CameraResp
-> startImuHandler        # arranca tImuHandler y crea /ImuCmd, /ImuResp
-> startGpsHandler        # arranca tGpsHandler y crea /GpsCmd, /GpsResp
-> cd "/romfs"
-> rtpSp "GNC_TEST.vxe start"
```

Los tres `start*Handler` **deben** ejecutarse antes del `rtpSp`. Salida esperada al finalizar:

```
[COORDINATOR] Estado: Camara COMPLETO | GPS COMPLETO | IMU COMPLETO

*** MISION COMPLETADA EXITOSAMENTE ***
Archivos de datos guardados:
  Camara: /sd0a/video.dat
  GPS: /sd0a/gpslog.txt
  IMU: /sd0a/imulog.txt
```

Objetivos por defecto, ajustables en `gnctest.c`:

```c
#define TARGET_FRAMES        5
#define TARGET_GPS_READINGS  500
#define TARGET_IMU_READINGS  500
```

---

## 9. Formato de los datos generados

### `/sd0a/video.dat` — cámara

*Frames* YUYV (YUV 4:2:2) de 640×480 concatenados sin cabecera ni delimitador; 614 400 bytes por *frame*. Decodificación *offline*:

```python
import numpy as np, cv2
FRAME = 640 * 480 * 2
data = open("video.dat", "rb").read()
for i in range(len(data) // FRAME):
    yuyv = np.frombuffer(data[i*FRAME:(i+1)*FRAME], np.uint8).reshape((480, 640, 2))
    cv2.imwrite(f"frame_{i:03d}.png", cv2.cvtColor(yuyv, cv2.COLOR_YUV2BGR_YUY2))
```

Las marcas de tiempo se consultan vía `USB2_VIDEO_IOCTL_GET_TS` pero **no se persisten**.

### `/sd0a/gpslog.txt` — GNSS

Tramas NMEA crudas, una por línea, escritas **solo cuando el fix es válido** (`gpsIsFixValid()`: `fixQuality > 0`, coordenadas no nulas y dentro de rango, ≥4 satélites).

```
$GNGGA,073848.00,1552.60444,N,10819.81548,E,1,10,1.48,12.7,M,-7.9,M,,*69
$GNRMC,073849.00,A,1552.60440,N,10819.81550,E,0.107,,030625,,,A*66
```

Las coordenadas quedan en formato NMEA **DDMM.MMMM / DDDMM.MMMM**; la conversión a grados decimales se hace en el análisis *offline*:

```python
lat = int(raw[:2])  + float(raw[2:])  / 60.0    # DDMM.MMMM
lon = int(raw[:3])  + float(raw[3:])  / 60.0    # DDDMM.MMMM
```

### `/sd0a/imulog.txt` — IMU

Encabezado informativo seguido de una línea por muestra:

```
<n>A,<ax>,<ay>,<az>,G,<gx>,<gy>,<gz>,M,<mx>,<my>,<mz>
```

> ⚠️ **Los valores son cuentas ADC crudas de 16 bits con signo, no unidades físicas**, pese a lo que declara el encabezado del archivo. El driver no escribe `ACCEL_CONFIG` ni `GYRO_CONFIG`, por lo que el sensor opera en sus rangos por defecto. Factores de conversión aplicables en el análisis:

| Sensor | Rango por defecto | Escala | Ejemplo |
|---|---|---|---|
| Acelerómetro | ±2 g | 16 384 LSB/g | `-14544` → `-0.888 g` |
| Giroscopio | ±250 °/s | 131 LSB/(°/s) | `-302` → `-2.31 °/s` |
| Magnetómetro | ±4912 µT, 16 bits | 0.15 µT/LSB | `-367` → `-55.0 µT` |

El contador de muestra y el marcador `A` se escriben sin separador (`1A,...`), lo que obliga al parser a tolerar el token combinado.

---

## 10. Referencia de API

### `usbHandler.h`

```c
STATUS uvcOpenDevice (UVC_HANDLE *h, const char *pDevName, const char *fileName);
STATUS uvcCaptureFrame(UVC_HANDLE *h, struct timespec *tsOut);   /* tsOut opcional */
void   uvcCloseDevice(UVC_HANDLE *h);
STATUS uvcRecord     (const char *pDevName, int nframes, const char *fileName);
```

`UVC_HANDLE` encapsula los descriptores de video y archivo, el buffer de captura y su tamaño. `uvcRecord()` ejecuta un ciclo completo abrir → capturar *n* → cerrar.

### `gpsHandler.h`

```c
STATUS  gpsInit(void);                          /* abre /ttyS1 a 9600 bd  */
STATUS  gpsReadLine(char *buf, int maxLen);     /* una sentencia NMEA     */
STATUS  gpsParseGGA(const char *nmea);          /* posición, altitud, sats */
STATUS  gpsParseRMC(const char *nmea);          /* velocidad               */
GpsFix* gpsGetFix(void);
STATUS  gpsGetLatLon(float *lat, float *lon);   /* ver §12, defecto abierto */
float   gpsGetAltitude(void);                   /* m ; -1.0f si no válido  */
float   gpsGetSpeedKmh(void);                   /* km/h ; -1.0f si no válido */
int     gpsGetSatellites(void);
bool    gpsHasFix(void);
bool    gpsIsFixValid(void);                    /* validación exhaustiva   */
int     gpsGetFixQuality(void);                 /* 0 sin fix, 1 GPS, 2 DGPS */
bool    gpsHasGoodSatelliteCount(void);         /* >= 4 satélites          */
STATUS  gpsLogFix(const char *nmea);
void    gpsShutdown(void);
```

`GpsFix` incluye una bandera de validez por campo (`timeReady`, `positionReady`, `altitudeReady`, `speedReady`, `satReady`).

### `i2cHandler.h`

```c
STATUS mpu9250Probe    (VXB_DEV_ID pDev);       /* enlace con el device tree */
STATUS mpu9250Attach   (VXB_DEV_ID pDev);       /* softc + wake-up           */
STATUS mpu9250WriteReg (VXB_DEV_ID pDev, UINT8 addr, UINT8 val);
STATUS mpu9250ReadReg  (VXB_DEV_ID pDev, UINT8 addr, UINT8 *val);
STATUS mpu9250WakeUp   (VXB_DEV_ID pDev);
STATUS mpu9250ReadAccel(VXB_DEV_ID pDev, int16_t *ax, int16_t *ay, int16_t *az);
STATUS mpu9250ReadGyro (VXB_DEV_ID pDev, int16_t *gx, int16_t *gy, int16_t *gz);
STATUS ak8963Init      (VXB_DEV_ID pDev);       /* bypass I²C + CNTL1        */
STATUS ak8963ReadMag   (VXB_DEV_ID pDev, int16_t *mx, int16_t *my, int16_t *mz);
```

El driver se registra en VxBus como `mpu9250Drv` y se enlaza por la cadena `"invensense,mpu9250"`. El magnetómetro AK8963 se alcanza habilitando el modo *bypass* del MPU-9250 (registro `INT_PIN_CFG` = `0x02`), tras lo cual responde en la dirección I²C `0x0C`.

### Manejadores de mensajes

```c
STATUS startUsbHandler(void);   /* USB_DKM */
STATUS startGpsHandler(void);   STATUS stopGpsHandler(void);   void showGpsStatus(void);
STATUS startImuHandler(void);   STATUS stopImuHandler(void);   void showImuStatus(void);
```

---

## 11. Estado de implementación

| Requisito del SETEC Lab | Estado | Observación |
|---|---|---|
| Determinación de posición | ✅ Implementado | Fix NMEA validado; ver defecto en §12 |
| Determinación de orientación relativa | ⚠️ Parcial | Se captura imagen; **no hay algoritmo de visión** |
| Estimación de aceleración | ✅ Implementado | Cuentas crudas, sin escalado ni calibración |
| Periférico para actuadores | ❌ Plantilla | Nodo DT presente y deshabilitado; funciones vacías |

Complementos no requeridos pero presentes: giroscopio y magnetómetro del MPU-9250, ambos operativos a nivel de lectura.

---

## 12. Limitaciones conocidas

Documentadas para evitar que se reutilicen como si estuvieran resueltas.

**Defectos abiertos**

1. **`convertNMEAToDecimal()` es incorrecta.** `float degrees = (float)(value / 100)` no trunca la parte entera, por lo que la función devuelve `value/100`. `gpsGetLatLon()` reporta 15.526° donde corresponde 15.877° (≈39 km de error). El rango `±90°/±180°` no lo detecta. **El análisis *offline* no está afectado**, porque parte de las tramas NMEA crudas del log.
2. **Sin validación del *checksum* NMEA.** El campo `*hh` nunca se verifica; una trama corrupta con formato válido se acepta.
3. **`strtok()` colapsa comas consecutivas.** NMEA usa campos vacíos, lo que puede desplazar los índices de campo cuando falta un valor intermedio.
4. **`WHO_AM_I` no se verifica.** `i2cLauncher.c` imprime una variable global que `mpu9250Attach()` nunca asigna (lee en una local homónima); el valor mostrado es siempre `0x00`. Falta la comparación contra `0x71`.
5. **`ST2` del AK8963 no se lee** tras los datos de medición, como exige el *datasheet* para cerrar el ciclo y detectar saturación (`HOFL`). La constante está definida pero sin usar.
6. **`video.dat` se abre sin `O_TRUNC`.** Una captura corta tras una larga deja *frames* obsoletos al final del archivo.
7. **Carrera en las colas de estado.** Agente trabajador y coordinador consultan `*_STATUS` sobre el mismo par de colas sin exclusión mutua; el campo `id`, previsto para correlacionar, se fija siempre a `1` y nunca se verifica.
8. **Estructuras de mensaje duplicadas.** `gnctest.c` redefine `CameraMsg`/`GpsMsg`/`ImuMsg` en vez de incluir los headers de los DKM; el `ImuMsg` del RTP omite el campo `lastReading`. Funciona por coincidencia del prefijo de la estructura.

**Limitaciones de diseño**

9. **Cadencia IMU ≈ 8.5 Hz** (`taskDelay` de 100 ms más ~17–20 ms de espera del magnetómetro). Insuficiente para determinación de actitud, que requiere 50–200 Hz. Además `taskDelay(sysClkRateGet()/50)` sufre truncamiento entero y produce cero espera si `sysClkRate < 50`.
10. **Sin base de tiempo común.** Ni imágenes, ni muestras IMU, ni fixes GPS llevan marca de tiempo persistida: **no es posible fusión sensorial** sobre estos datos tal como se registran.
11. **Sin calibración.** No se remueven sesgos ni factores de escala; el giroscopio presenta ~2.3 °/s de sesgo en reposo.
12. **E/S bloqueante en sección crítica.** `tImuReader` hace `fprintf` + `fflush` a la SD dentro del semáforo `imuSem`; `gpsLogFix()` abre y cierra el archivo en cada trama.
13. **Sin redundancia ni detección de fallos.** No hay *watchdog*, reintento de bus, ni degradación controlada ante pérdida de un sensor.

---

## 13. Archivos requeridos no versionados

Necesarios para reproducir el sistema y **ausentes de este repositorio**:

- **`rpi-4.dtsi`** — árbol de dispositivos con el mapeo de pines de §5 y §6. Sin él, las conexiones documentadas no corresponden y el sistema no arranca correctamente.
- **`mpu9250I2cDrv.c`**, **`dkm.c`**, **`startup.c`** — referenciados por los artefactos de compilación versionados, sin código fuente.
- **Scripts de análisis en Python** — decodificador YUYV y procesamiento estadístico de IMU y GPS (apéndices B, C y D del TFG). Contienen la conversión NMEA correcta señalada en §12.1.
- **Biblioteca VxMAES** — dependencia externa, se distribuye por separado.

---

## 14. Licencia y créditos

**Autoría del trabajo original:** Priscilla María Víquez Castro — Trabajo Final de Graduación, *Desarrollo e integración de componentes embebidos para la computadora de guía, navegación y control del SETEC Lab*, Instituto Tecnológico de Costa Rica, Cartago, 17 de junio de 2025.

**Profesor asesor:** Johan Carvajal Godínez. **Profesores lectores:** Juan José Montero Rodríguez, Juan Scott Chaves Noguera.

El documento del TFG se distribuye bajo Creative Commons **BY-SA 4.0**. **Este repositorio no declara licencia**; conviene añadir un archivo `LICENSE` coherente con la del trabajo original antes de cualquier redistribución.

VxWorks, VxBus y Wind River Workbench son productos propietarios de Wind River Systems. El SETEC Lab opera bajo un acuerdo académico de licenciamiento; este repositorio no incluye ni redistribuye componentes de VxWorks.

**Base teórica:**

- C. Chan-Zheng, *MAES: A Multi-Agent Systems Framework for Embedded Systems*, TU Delft.
- B. V. L. Mora, *VxMAES: Plataforma de Ejecución del Paradigma Multi-Agentes para Sistemas Embebidos de Misión Crítica*, TEC, 2024.
- NASA, *State-of-the-Art of Small Spacecraft Technology*, 2024.
- ESA, *Space Avionics Open Interface Architecture (SAVOIR)*.
