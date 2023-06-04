#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>
#include "freertos/Freertos.h"
#include "freertos/task.h"
#include "sdkconfig.h"


#define pdSECOND pdMS_TO_TICKS(1000)    //generates 1 sec delay
#define NOT_RECEIVED "-1"               // if parameter is not received in packet

typedef struct
{
    char *Header, *Time, *Longitude, *Long_Ind, *Latitude, *Lat_Ind, *Altitude_Unit, *Geoidal_Unit;
    int Satellite_Num, Quality_Indicator, StationID;
    uint8_t Checksum;
    float HDOP, Altitude, Geoidal_Separation, Differential_Corrections;
} GPS_data;

char **split_string(const char *packet);

int validate(const char *packet);

uint8_t GPGGA_checksum_calc(const char *packet);

GPS_data *Parse_gps_data(const char *packet);