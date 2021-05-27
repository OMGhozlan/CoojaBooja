/**
 * @file sensornode.h
 * @author Ghozlan
 * @brief Contains structs, macros and function prototypes needed for a sensor node
 * @version 0.1
 * @date 2021-05-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef SENSORNODE_H
#define SENSORNODE_H

#ifndef AMP_FACTOR
#define AMP_FACTOR 2.0f
#endif

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_INFO
#endif

#ifndef FADE_FACTOR
#define FADE_FACTOR 0.5f
#endif

#ifndef TEMPR
#define TEMPR 25.0f
#endif

#ifndef HUMIDITY
#define HUMIDITY 40.0f
#endif

#ifndef PRESSURE
#define PRESSURE (101.0f * 1000)
#endif

/**
 * @brief Data type representing the data that
 *  the sensors in this scenario measure
 */
typedef struct sensorData {
    float temperature;
    float humidity;
    float pressure;
    unsigned int msgCount;
} sensor;
#endif