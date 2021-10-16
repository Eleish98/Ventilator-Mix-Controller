#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sysctl.h>
#include"Timer/Timer_Manage.h"
#include"APP/schedule/Schedule.h"
#include "FreeRTOS.h"
#include "OS/FreeRTOS/Source/include/task.h"
#include "MCAL/UART/UART_Manager.h"

#include "HAL/Flow_Sensor/Flow_Sensor.h"
#include "HAL/ON_OFF_Valves/ON_OFF_Valves.h"
#include "HAL/Oxygen_Sensor/Oxygen_Sensor.h"
#include "HAL/Pressure_Sensor/Pressure_Sensor.h"


volatile int16_t Oxygen_Flow;
volatile int16_t Air_Flow;

volatile uint8_t Oxygen_Percentage;

volatile uint32_t Oxygen_Pressure;
volatile uint32_t Tank_Pressure;
volatile uint32_t Air_Pressure;


/**
 * main.c
 */

static void StringUARTSend(char* String){
    while(*String)
        UART_vidSend((unsigned char)(*String++));
}
char S[100];
void main(void)
{

    (void)bDriverInit();

	while(1){
	    Oxygen_Flow = s16ReadFlow(FLOW_SENSOR_OXYGEN);
	    Air_Flow = s16ReadFlow(FLOW_SENSOR_AIR);

	    Oxygen_Percentage = ReadOxygenSensor();

	    Oxygen_Pressure = u32ReadPressure(PRESSURE_SENSOR_OXYGEN);
	    Air_Pressure = u32ReadPressure(PRESSURE_SENSOR_AIR);
	    Tank_Pressure = u32ReadPressure(PRESSURE_SENSOR_TANK);

	    sprintf(S,"Flow Readings:\n\tOxygen:\t%d\n\tAir:\t%d\n",Oxygen_Flow,Air_Flow);
	    StringUARTSend(S);
	    sprintf(S,"Pressure Readings:\n\tOxygen:\t%lu\n\tAir:\t%lu\n\tTank:%lu\n",Oxygen_Pressure,Air_Pressure,Tank_Pressure);
	    StringUARTSend(S);
	    sprintf(S,"Oxygen Percentage:\t%u",Oxygen_Percentage);
	    SysCtlDelay(50000000);
	}
}
