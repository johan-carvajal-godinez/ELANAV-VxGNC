#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <taskLib.h>
#include <msgQLib.h>
#include <semLib.h>
#include <stdbool.h>
#include "gpsHandler.h"

/**********************************************/
/*             Message Structure              */
/**********************************************/

typedef enum {
    GPS_CMD_START = 1,
    GPS_CMD_READ = 2,
    GPS_CMD_STOP = 3,
    GPS_CMD_STATUS = 4
} GpsCommand;

typedef struct {
    GpsCommand cmd;
    int readingsRequested;
    int readingsCompleted;
    int result;
    int id;
    struct {
        char time[16];
        float latitude;
        float longitude;
        float altitude;
        float speed;
        int satellites;
        int timeReady;
        int positionReady;
        int altitudeReady;
        int speedReady;
        int satReady;
    } lastFix;
} GpsMsg;

/**********************************************/
/*           Global Variables                 */
/**********************************************/

MSG_Q_ID gpsCmdQueue = NULL;
MSG_Q_ID gpsRespQueue = NULL;
TASK_ID gpsHandlerTaskId = TASK_ID_NULL;
TASK_ID gpsReaderTaskId = TASK_ID_NULL;
bool gpsActive = false;
int gpsReadingsTarget = 0;
int gpsReadingsCount = 0;
SEM_ID gpsSem = NULL;
FILE* gpsLogFile = NULL;

/**********************************************/
/*           GPS Reader Task                  */
/**********************************************/

void GpsReaderTask() {
    char nmea[128];
    
    printf("[GPS] Reader task started\n");
    
    while (gpsActive && gpsReadingsCount < gpsReadingsTarget) {
        if (gpsReadLine(nmea, sizeof(nmea)) == OK) {
            // Parse NMEA data first
            gpsParseGGA(nmea);
            gpsParseRMC(nmea);
            
            // Only count and log if we have a VALID fix with real coordinates
            if (gpsHasFix()) {
                float lat, lon;
                if (gpsGetLatLon(&lat, &lon) == OK) {
                    // Check if coordinates are valid (not zero or near zero)
                    if (lat != 0.0 && lon != 0.0 && lat > -90.0 && lat < 90.0 && 
                        lon > -180.0 && lon < 180.0) {
                        
                        semTake(gpsSem, WAIT_FOREVER);
                        gpsReadingsCount++;
                        
                        // Log the valid NMEA data
                        if (gpsLogFile) {
                            fprintf(gpsLogFile, "%s\n", nmea);
                            fflush(gpsLogFile);
                        }
                        
                        printf("[GPS] Valid fix %d/%d - Lat: %.5f, Lon: %.5f\n", 
                               gpsReadingsCount, gpsReadingsTarget, lat, lon);
                        
                        // Check if target reached
                        if (gpsReadingsCount >= gpsReadingsTarget) {
                            printf("[GPS] Target readings completed (%d/%d)\n", 
                                   gpsReadingsCount, gpsReadingsTarget);
                            gpsActive = false;
                        }
                        semGive(gpsSem);
                    }
                }
            }
        }
        taskDelay(10); // Small delay
    }
    
    printf("[GPS] Reader task finished\n");
}

/**********************************************/
/*           Handler Functions                */
/**********************************************/

STATUS handleGpsCommand(GpsMsg* cmd, GpsMsg* resp) {
    STATUS result = ERROR;
    
    resp->cmd = cmd->cmd;
    resp->id = cmd->id;
    resp->readingsRequested = cmd->readingsRequested;
    
    semTake(gpsSem, WAIT_FOREVER);
    
    switch (cmd->cmd) {
        case GPS_CMD_START:
            if (!gpsActive) {
                if (gpsInit() == OK) {
                    // Open log file
                    gpsLogFile = fopen("/sd0a/gpslog.txt", "w");
                    if (gpsLogFile) {
                        fprintf(gpsLogFile, "GPS NMEA Data Log\n");
                        fflush(gpsLogFile);
                    }
                    
                    gpsReadingsTarget = cmd->readingsRequested;
                    gpsReadingsCount = 0;
                    gpsActive = true;
                    
                    // Start GPS reader task
                    gpsReaderTaskId = taskSpawn("tGpsReader", 100, 0, 8192,
                                                (FUNCPTR)GpsReaderTask,
                                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
                    
                    if (gpsReaderTaskId != TASK_ID_ERROR) {
                        printf("[GPS] Started with target %d readings\n", gpsReadingsTarget);
                        result = OK;
                    } else {
                        gpsActive = false;
                        gpsShutdown();
                        printf("[GPS] Failed to start reader task\n");
                    }
                } else {
                    printf("[GPS] Initialization failed\n");
                }
            } else {
                printf("[GPS] Already active\n");
                result = OK;
            }
            break;
            
        case GPS_CMD_READ:
            // Return current fix data
            {
                GpsFix* fix = gpsGetFix();
                if (fix) {
                    // Copy fix data to response structure
                    strncpy(resp->lastFix.time, fix->time, sizeof(resp->lastFix.time)-1);
                    resp->lastFix.time[sizeof(resp->lastFix.time)-1] = '\0';
                    resp->lastFix.latitude = fix->latitude;
                    resp->lastFix.longitude = fix->longitude;
                    resp->lastFix.altitude = fix->altitude;
                    resp->lastFix.speed = fix->speed;
                    resp->lastFix.satellites = fix->satellites;
                    resp->lastFix.timeReady = fix->timeReady;
                    resp->lastFix.positionReady = fix->positionReady;
                    resp->lastFix.altitudeReady = fix->altitudeReady;
                    resp->lastFix.speedReady = fix->speedReady;
                    resp->lastFix.satReady = fix->satReady;
                    result = OK;
                } else {
                    printf("[GPS] No fix data available\n");
                    result = ERROR;
                }
            }
            break;
            
        case GPS_CMD_STOP:
            if (gpsActive) {
                gpsActive = false;
                if (gpsReaderTaskId != TASK_ID_NULL) {
                    taskDelete(gpsReaderTaskId);
                    gpsReaderTaskId = TASK_ID_NULL;
                }
                if (gpsLogFile) {
                    fclose(gpsLogFile);
                    gpsLogFile = NULL;
                }
                gpsShutdown();
                printf("[GPS] Stopped\n");
            }
            result = OK;
            break;
            
        case GPS_CMD_STATUS:
            resp->readingsCompleted = gpsReadingsCount;
            result = (gpsReadingsCount >= gpsReadingsTarget && gpsReadingsTarget > 0) ? OK : ERROR;
            printf("[GPS] Status: %d/%d readings, %s\n", 
                   gpsReadingsCount, gpsReadingsTarget,
                   result == OK ? "COMPLETE" : "IN_PROGRESS");
            break;
            
        default:
            printf("[GPS] Unknown command\n");
            result = ERROR;
            break;
    }
    
    semGive(gpsSem);
    
    resp->result = (result == OK) ? 0 : 1;
    resp->readingsCompleted = gpsReadingsCount;
    return result;
}

void GpsMessageHandler() {
    GpsMsg command, response;
    
    printf("[GPS] Message handler started\n");
    
    while (1) {
        if (msgQReceive(gpsCmdQueue, (char*)&command, sizeof(command), WAIT_FOREVER) == ERROR) {
            continue;
        }
        
        memset(&response, 0, sizeof(response));
        handleGpsCommand(&command, &response);
        
        msgQSend(gpsRespQueue, (char*)&response, sizeof(response), NO_WAIT, MSG_PRI_NORMAL);
    }
}

/**********************************************/
/*           Public Interface                 */
/**********************************************/

STATUS startGpsHandler() {
    
    gpsSem = semBCreate(SEM_Q_FIFO, SEM_FULL);
    if (!gpsSem) return ERROR;
    
    gpsCmdQueue = msgQOpen("/GpsCmd", 10, sizeof(GpsMsg), MSG_Q_FIFO, OM_CREATE, NULL);
    if (!gpsCmdQueue) {
        semDelete(gpsSem);
        return ERROR;
    }
    
    gpsRespQueue = msgQOpen("/GpsResp", 10, sizeof(GpsMsg), MSG_Q_FIFO, OM_CREATE, NULL);
    if (!gpsRespQueue) {
        msgQClose(gpsCmdQueue);
        semDelete(gpsSem);
        return ERROR;
    }
    
    gpsHandlerTaskId = taskSpawn("tGpsHandler", 90, 0, 8192,
                                 (FUNCPTR)GpsMessageHandler,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    
    if (gpsHandlerTaskId == TASK_ID_ERROR) {
        msgQClose(gpsCmdQueue);
        msgQClose(gpsRespQueue);
        semDelete(gpsSem);
        return ERROR;
    }
    
    printf("[GPS] Message handler ready\n");
    return OK;
}

STATUS stopGpsHandler() {
    printf("[GPS] Stopping message handler\n");
    
    if (gpsActive) {
        gpsActive = false;
        if (gpsReaderTaskId != TASK_ID_NULL) {
            taskDelete(gpsReaderTaskId);
            gpsReaderTaskId = TASK_ID_NULL;
        }
        gpsShutdown();
    }
    
    if (gpsHandlerTaskId != TASK_ID_NULL) {
        taskDelete(gpsHandlerTaskId);
        gpsHandlerTaskId = TASK_ID_NULL;
    }
    
    if (gpsCmdQueue) {
        msgQClose(gpsCmdQueue);
        gpsCmdQueue = NULL;
    }
    if (gpsRespQueue) {
        msgQClose(gpsRespQueue);
        gpsRespQueue = NULL;
    }
    if (gpsSem) {
        semDelete(gpsSem);
        gpsSem = NULL;
    }
    
    printf("[GPS] Message handler stopped\n");
    return OK;
}

void showGpsStatus() {
    printf("\n=== GPS Status ===\n");
    printf("Handler: %p\n", gpsHandlerTaskId);
    printf("Active: %s\n", gpsActive ? "YES" : "NO");
    printf("Readings: %d/%d\n", gpsReadingsCount, gpsReadingsTarget);
    printf("==================\n");
}
