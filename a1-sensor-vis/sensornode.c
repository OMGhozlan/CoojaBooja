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
    srand(33);
    return (min + ((float) rand() / RAND_MAX) * (max - min));
}

/**
 * @brief Gets a sample(number) from a Gaussian distribution
 * :: Used to mimic added noise when reading data from sensor
 * 
 * @param iv Intial value (random number) to draw from the distribution
 * @return float (number drawn from a Gaussian distribution)
 */
float gausGen(float iv) {    
    return (float)(exp(-0.5f * iv * iv) * sqrt(2 * PI));
}

/**
 * @brief Gets all three data readings from sensor
 * 
 * @param sensor 
 */
void getSensorData(sensor* sensor) {
    sensor->temperature = (TEMPR + (*awgnGen_ptr)()) * AMP_FACTOR * FADE_FACTOR;
    sensor->humidity = (HUMIDITY + (*awgnGen_ptr)()) * AMP_FACTOR * FADE_FACTOR;
    sensor->pressure = (PRESSURE + (*awgnGen_ptr)()) * AMP_FACTOR * FADE_FACTOR;
    printf("Temp = %.3f || Humid = %.3f || Pres = %.3f\n",
     sensor->temperature, sensor->humidity, sensor->pressure);
}

/**
 * @brief Generate addative (white) gaussian noise
 * 
 * @return float (noise)
 */
float awgnGen() {
    return gausGen(getRandValue(-3.5, 3.5));
}

float (*awgnGen_ptr)() = &awgnGen;