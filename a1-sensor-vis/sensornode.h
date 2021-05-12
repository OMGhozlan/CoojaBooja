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


/**
 * @brief Data type that contains all parameters
 * that are taken into consideration to get a sensor
 * reading in the proposed environment
 */
typedef struct ctrlblkData{
    float theta;
    float n_i;
    float a_i;
    float h_i;    
}ctrlblk;

float getRandValue(float min, float max);
float gausGen(float num);
float awgnGen();
void getSensorData(sensor* sensor);
float (*awgnGen_ptr)();
// extern "C" float (*awgnGen_ptr)() = &awgnGen;

// extern int node_count;

#endif