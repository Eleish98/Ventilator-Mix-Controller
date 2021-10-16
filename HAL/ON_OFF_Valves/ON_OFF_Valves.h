/*
 * ON_OFF_Valves.h
 *
 *  Created on: Dec 26, 2020
 *      Author: Ahmed_Saad
 */

#ifndef HAL_ON_OFF_VALVES_ON_OFF_VALVES_H_
#define HAL_ON_OFF_VALVES_ON_OFF_VALVES_H_


typedef enum
{
    VALVE_CLOSE=0,
    VALVE_OPEN=1
}ValveState_T;



void OxygenValve(ValveState_T enuValveState);
void AirValve(ValveState_T enuValveState);
void ExpValve(ValveState_T enuValveState);
void SafetyValve(ValveState_T enuValveState);

#endif /* HAL_ON_OFF_VALVES_ON_OFF_VALVES_H_ */
