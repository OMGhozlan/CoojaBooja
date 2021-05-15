/**
 * @file sensornode.c
 * @author Ghozlan
 * @brief 
 * @version 0.1
 * @date 2021-05-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "sensornode.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#define PI 3.1415926535

/**
 * @brief Generate a random value
 * 
 * @param min: Lower bound of random range
 * @param max: Upper bound of random range
 * @return float (random number)
 */


float getRandValue(float min, float max) {
    // srand(33);
    return (min + ((float) rand() / RAND_MAX) * (max - min));
}

/**
 * @brief Gets a sample(number) from a Gaussian distribution
 * :: Used to mimic added noise when reading data from sensor
 * 
 * @param iv Intial value (random number) to draw from the distribution
 * @return float (number drawn from a Gaussian distribution)
 */
float gausGen() {  
    float u, v, r2;
    do {
        u = 2.0 * getRandValue(-1, 1) - 1.0;
        v = 2.0 * getRandValue(-1, 1) - 1.0;
        r2 = u * u + v * v;
    } while (r2 >= 1.0 || r2 == 0);
    return (float)(u * sqrt(-2.0 * log(r2) / r2));
}

/**
 * @brief Gets all three data readings from sensor
 * 
 * @param sensor 
 */
void getSensorData(sensor* sensor, float noise) {
    sensor->temperature = ((TEMPR + (*awgnGen_ptr)()) * AMP_FACTOR * FADE_FACTOR) + noise;
    sensor->humidity = ((HUMIDITY + (*awgnGen_ptr)()) * AMP_FACTOR * FADE_FACTOR) + noise;
    sensor->pressure = ((PRESSURE + (*awgnGen_ptr)()) * AMP_FACTOR * FADE_FACTOR) + noise;
    printf("%.3f,%.3f,%.3f\n",
     sensor->temperature, sensor->humidity, sensor->pressure);
}

/**
 * @brief Generate addative (white) gaussian noise
 * 
 * @return float (noise)
 */
float awgnGen() {
    return gausGen();
}

float (*awgnGen_ptr)() = &awgnGen;