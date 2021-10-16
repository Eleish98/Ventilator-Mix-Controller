/*
 * Oxygen_Sensor.c
 *
 *  Created on: Jul 3, 2021
 *      Author: Ahmed_Saad
 */
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "MCAL/ADC/ADC_Manager.h"

#include "Oxygen_Sensor.h"

#define ADC_MAX_DIFF                        5
#define NEW_RANGE_CONFIRMATION_READING      3
#define NUM_AVERAGE_VALUES                  (uint32_t)50


    static uint32_t arr[NUM_AVERAGE_VALUES]={0};


static bool OutOFRange(uint32_t Reading1,uint32_t Reading2,uint32_t Range);
static uint32_t debouncingADCReading();
static uint32_t AverageValue();


static uint32_t debouncingADCReading()
{
    bool isOutOfRange = false;
    uint32_t CurrentReading=0;
    static uint32_t LastConfirmedReading=0;
    static uint32_t lastRawReading=0;
    static uint8_t NewRangeCounter = 0;
    uint32_t ReadADCROWValue = 0;

    ReadADCROWValue = ADC_SampleChannel(ADC0_BASE,3);

    isOutOfRange = OutOFRange(ReadADCROWValue,LastConfirmedReading,ADC_MAX_DIFF);

    if(isOutOfRange == false)
    {
        /*Value in range take it*/
        CurrentReading = ReadADCROWValue;
        NewRangeCounter = 0;

    }
    else
    {
        isOutOfRange = OutOFRange(lastRawReading,ReadADCROWValue,ADC_MAX_DIFF);

        if(isOutOfRange == false)
        {
            if(NewRangeCounter >= NEW_RANGE_CONFIRMATION_READING - 2)
            {
                NewRangeCounter = 0;
                /*Go to new range*/
                CurrentReading = ReadADCROWValue;
            }
            else
            {
                NewRangeCounter++;
                /*Value is out of confirmed range*/
                 CurrentReading = LastConfirmedReading;
            }
        }
        else
        {
            /*Value is out of confirmed range*/
            CurrentReading = LastConfirmedReading;
            NewRangeCounter = 0;
        }
    }

    LastConfirmedReading = CurrentReading;
    lastRawReading = ReadADCROWValue;


    return CurrentReading;
}

static bool OutOFRange(uint32_t Reading1,uint32_t Reading2,uint32_t Range)
{
    uint32_t diff = 0;
    bool retValue;

    if(Reading1 > Reading2)
    {
       diff = Reading1 - Reading2;
    }
    else
    {
        diff = Reading2 - Reading1;
    }

    if(diff >= Range)
    {
        retValue = true;
    }
    else
    {
        retValue = false;
    }

    return retValue;
}




/*First NUM_AVERAGE_VALUES-1 reading from average filter are invalid*/
static uint32_t AverageValue()
{
    static uint32_t Sum = 0;
    static bool firstCall = true;
    static uint8_t lastIndex = 0;
    uint32_t Average=0;
    uint8_t index;
    uint32_t NewValue=0;

    if(firstCall == true)
    {
        firstCall = false;
        /*If first call read value for NUM_AVERAGE_VALUES times to validate average filter*/
        for(index=0; index<NUM_AVERAGE_VALUES ;index++)
        {
            arr[index] = debouncingADCReading();
            Sum += arr[index];
        }

        Average = Sum / NUM_AVERAGE_VALUES;
    }
    else
    {
          NewValue = debouncingADCReading();
          /*Remove one of the old values and add the new value to the average calculation*/
          Sum = (uint32_t)((uint32_t)Sum  + NewValue - arr[lastIndex]);
          Average = (Sum + (NUM_AVERAGE_VALUES/2)) / NUM_AVERAGE_VALUES;
          arr[lastIndex] = NewValue;

          /*Increase index and set it with zero when reach NUM_AVERAGE_VALUES*/
          lastIndex++;
          if(lastIndex >= NUM_AVERAGE_VALUES)
          {
              lastIndex = 0;
          }
    }

    return Average;
}





uint8_t ReadOxygenSensor()
{
    uint32_t ADCfilteredValue;
    uint32_t OxygenVoltageValue;
    uint32_t OxygenPercentage = 0;

    ADCfilteredValue = AverageValue() * 12 / 78;

    OxygenVoltageValue = (5000 * ADCfilteredValue) / 4095;

    OxygenPercentage = (( (79*OxygenVoltageValue) - 41 )  / 50);

  //  OxygenPercentage = (OxygenPercentage * 100) / 614;

    return (uint8_t)OxygenPercentage;
}



