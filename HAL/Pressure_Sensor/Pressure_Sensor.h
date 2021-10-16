/*
 * Pressure_Sensor.h
 *
 *  Created on: Aug 22, 2020
 *      Author: Ahmed_Saad
 */

#ifndef HAL_PRESSURE_SENSOR_PRESSURE_SENSOR_H_
#define HAL_PRESSURE_SENSOR_PRESSURE_SENSOR_H_


#define PRESSURE_SENSOR_AIR         0
#define PRESSURE_SENSOR_OXYGEN      1
#define PRESSURE_SENSOR_TANK        2

uint32_t u32ReadPressure(uint8_t SensorID);


#endif /* HAL_PRESSURE_SENSOR_PRESSURE_SENSOR_H_ */
