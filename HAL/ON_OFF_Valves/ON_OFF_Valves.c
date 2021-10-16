/*
 * ON_OFF_Valves.c
 *
 *  Created on: Dec 26, 2020
 *      Author: Ahmed_Saad
 */
#include <stdbool.h>
#include <stdint.h>
#include "sysctl.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "MCAL/Pins/Pins.h"
#include "ON_OFF_Valves.h"



void OxygenValve(ValveState_T enuValveState)
{
    if(enuValveState == VALVE_OPEN)
    {
        //PIN_vidSetPin(GPIO_PORTA_BASE,GPIO_PIN_3,0);
        PIN_vidSetPin(GPIO_PORTA_BASE, GPIO_PIN_3, 0);
        PIN_vidSetPin(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4);
    }
    else
    {
        PIN_vidSetPin(GPIO_PORTA_BASE, GPIO_PIN_3, 0);
        PIN_vidSetPin(GPIO_PORTA_BASE, GPIO_PIN_4, 0);

        //PIN_vidSetPin(GPIO_PORTA_BASE,GPIO_PIN_4,0);
    }

}



void AirValve(ValveState_T enuValveState)
{
    if(enuValveState == VALVE_OPEN)
    {
        PIN_vidSetPin(GPIO_PORTB_BASE, GPIO_PIN_6, 0);
        PIN_vidSetPin(GPIO_PORTB_BASE, GPIO_PIN_7, GPIO_PIN_7);

        //PIN_vidSetPin(GPIO_PORTB_BASE,GPIO_PIN_7,GPIO_PIN_7);
    }
    else
    {
        PIN_vidSetPin(GPIO_PORTB_BASE, GPIO_PIN_6, 0);
        PIN_vidSetPin(GPIO_PORTB_BASE, GPIO_PIN_7, 0);
        //PIN_vidSetPin(GPIO_PORTB_BASE,GPIO_PIN_7,0);
    }
}




void SafetyValve(ValveState_T enuValveState)
{
    if(enuValveState == VALVE_OPEN)
    {
        PIN_vidSetPin(GPIO_PORTD_BASE, GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_2);
        //PIN_vidSetPin(GPIO_PORTB_BASE,GPIO_PIN_7,GPIO_PIN_7);
    }
    else
    {
        PIN_vidSetPin(GPIO_PORTD_BASE, GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_3);
        //PIN_vidSetPin(GPIO_PORTB_BASE,GPIO_PIN_7,0);
    }
}


void ExpValve(ValveState_T enuValveState)
{
    if(enuValveState == VALVE_OPEN)
    {
        PIN_vidSetPin(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_1);
        //PIN_vidSetPin(GPIO_PORTB_BASE,GPIO_PIN_7,GPIO_PIN_7);
    }
    else
    {
        PIN_vidSetPin(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_2);
        //PIN_vidSetPin(GPIO_PORTB_BASE,GPIO_PIN_7,0);
    }
}
