/*
 * Sys_Manager.c
 *
 *  Created on: Jun 8, 2020
 *      Author: Ahmed_Saad
 */
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "HAL/Flow_Sensor/Flow_Sensor.h"
#include "HAL/Pressure_Sensor/Pressure_Sensor.h"
#include "HAL/ON_OFF_Valves/ON_OFF_Valves.h"

#include "MCAL/I2C/I2C_Manage.h"

#include "Sys_Manager.h"



typedef enum {
    INIT,
    VALVES_SETTING,
    TANK_FILLING,
    ERROR_STATE,
    TANK_MONITORING,
}System_States_T;


int Array[100];
int i =0;
#ifdef Testings
int16_t ReadFlowValue = 0;

int16_t ReadTempValue = 0;
int16_t ReadFlowValue = 0;

int16_t ReadTempValue = 0;

int Array[100];
int i =0;
      //vidSensorReset();
      ReadFlowValue = u32ReadPressure();

      ReadTempValue =  u32ReadTemp();
      Array[i] =  ReadFlowValue;
      if(i<100)
      {
          i++;
      }
#endif

static System_States_T enuSysCurrentState = INIT;
/******************************Stubs***************************************/
uint32_t Stub_EnteredOxygenRatio(void)
{
    uint32_t OxygenRatio = 0;

    return OxygenRatio;
}

uint32_t Stub_ReadOxygenSensor(void)
{
    uint32_t OxygenSensor = 0;

    return OxygenSensor;
}

bool Stub_ReadTrigger(void)
{
    bool ReadTrigger = false;

    return ReadTrigger;
}

void Stub_MixReadyPin(bool bMixReady)
{

}
/*************************************************************************/
#define BOX_VOLUME 16u
#define FINAL_BOX_PRESSURE 250u
#define OXYGEN_GAS_CONSTANT 259u
#define NITROGEN_GAS_CONSTANT 297u
#define AIR_GAS_CONSTANT 287u
#define MUTI_100000 100000u
#define MUTI_1000 1000u
#define KELVIN_CONVERT 27315u
#define MUTI_100 100u
#define GAUGE_TO_ABS 100u
#define SOURCE_PRESSURE 500
#define VALVE_FLOW 266u   /*160 /1000 then / 60 = .0026666 then multiple 100000*/

#define PRESSURE_UP 250u
#define PRESSURE_MID 240u
#define PRESSURE_DOWN 150u
#define PRESSURE_DOWN_LIMIT 120u

#define OXYGEN_REFILL_MARGIN 5u
#define OXYGEN_STOP_MARGIN 10u
#if 0
void SCH_vidSysManager(void)
{
   uint32_t u32Pressure;
   uint32_t u32ReadOxygenRatio;
   uint32_t u32WantedOxygenRatio;
   uint32_t u32Tempreture;
   uint32_t TargetOxygenMass;
   uint32_t TargetNitrogenMass;
   uint32_t MassFlowOxygenValve;
   uint32_t MassFlowAirValve;
   uint32_t FlowOxygenInAirValve;
   uint32_t FlowNitrogenInAirValve;
   uint32_t CurrentOxygenMass;
   uint32_t CurrentNitrogenMass;
   uint32_t NeededOxygenMass;
   uint32_t NeededNitrogenMass;
   uint32_t OxygenMassFromAirValve;
   uint32_t TimeAirValve;
   uint32_t TimeOxygenValve;
   static uint32_t u32TimeCounter = 0;
   static bool bOxygenValveStatus = false;
   static bool bAirValveStatus = false;
   static uint8_t u8ErrorCount = 0;
   bool bTrigger = false;
  switch (enuSysCurrentState)
  {
  case INIT:
      /*Read Oxygen percentage, Output enter value and monitor trigger pin
        * calculate the needed time for each value to be opened
        * To create a service layer with average and de-bouncing for all sensors*/
      /*Want oxygen from 0 to 100*/
      u32WantedOxygenRatio = Stub_EnteredOxygenRatio();
      bTrigger = Stub_ReadTrigger();
      if(bTrigger == true)
      {
          enuSysCurrentState = VALVES_SETTING;
      }
      break;
  case VALVES_SETTING:
      /*Read sensors*/
      u32ReadOxygenRatio = Stub_ReadOxygenSensor();
      u32Pressure = u32ReadPressure();
      u32Tempreture =  u32ReadTemp();

      /*Mass of oxygen with needed ratio in pressure 2.5bar*/
      TargetOxygenMass = (BOX_VOLUME*FINAL_BOX_PRESSURE*u32WantedOxygenRatio*MUTI_1000)
                                    /(OXYGEN_GAS_CONSTANT * ((u32Tempreture*MUTI_100)+KELVIN_CONVERT));
      /*Mass of nitrogen when oxygen be in its needed ratio in 2.5bar*/
      TargetNitrogenMass = (BOX_VOLUME*FINAL_BOX_PRESSURE*(100-u32WantedOxygenRatio)*MUTI_1000)
                                     /(NITROGEN_GAS_CONSTANT * ((u32Tempreture*MUTI_100)+KELVIN_CONVERT));
      /*Oxygen mass flow from oxygen valve with gram/s*/
      MassFlowOxygenValve = (SOURCE_PRESSURE * VALVE_FLOW * MUTI_1000)
                                         / (OXYGEN_GAS_CONSTANT * ((u32Tempreture*MUTI_100)+KELVIN_CONVERT));
      /*Air mass flow from air valve with gram/s*/
      MassFlowAirValve =   (SOURCE_PRESSURE * VALVE_FLOW * MUTI_1000)
                                          / (AIR_GAS_CONSTANT * ((u32Tempreture*MUTI_100)+KELVIN_CONVERT));
      /*Oxygen mass percentage in air 23.14%*/
      FlowOxygenInAirValve = (MassFlowAirValve * 2314) / 10000;
      /*Rest is Nitrogen*/
      FlowNitrogenInAirValve = MassFlowAirValve - FlowOxygenInAirValve;

      /*Mass of current oxygen in the box*/
      CurrentOxygenMass = (BOX_VOLUME* (u32Pressure+GAUGE_TO_ABS) *u32ReadOxygenRatio*MUTI_1000)
                                    /(OXYGEN_GAS_CONSTANT * ((u32Tempreture*MUTI_100)+KELVIN_CONVERT));
      /*Mass of current nitrogen on the box*/
      CurrentNitrogenMass = (BOX_VOLUME*(u32Pressure+GAUGE_TO_ABS)*(100-u32ReadOxygenRatio)*MUTI_1000)
                                     /(NITROGEN_GAS_CONSTANT * ((u32Tempreture*MUTI_100)+KELVIN_CONVERT));
      /*Need nitrogen mass*/
      NeededNitrogenMass = TargetNitrogenMass - CurrentNitrogenMass;
      /*Needed time for air valve opening to reach this ratio*/
      TimeAirValve = NeededNitrogenMass / FlowNitrogenInAirValve;
      /*Entered Oxygen mass form air valve*/
      OxygenMassFromAirValve = TimeAirValve * FlowOxygenInAirValve;
      /*Needed Oxygen mass*/
      NeededOxygenMass = TargetOxygenMass - CurrentOxygenMass - OxygenMassFromAirValve;
      /*Needed time for oxygen valve to reach this oxygen ratio in the box*/
      TimeOxygenValve = NeededOxygenMass / MassFlowOxygenValve;
      /*Open Valves*/
      if(TimeOxygenValve != 0)
      {
          OxygenValve(VALVE_OPEN);
      }
      if(TimeAirValve != 0)
      {
          AirValve(VALVE_OPEN);
      }
      u32TimeCounter++;

      enuSysCurrentState = TANK_FILLING;
      break;
  case TANK_FILLING :
      /*Read sensors*/
      u32TimeCounter++;
      /*Task count with 10ms, while valve time calculation with second*/
      if(u32TimeCounter == (TimeOxygenValve*100))
      {
          OxygenValve(VALVE_CLOSE);
          bOxygenValveStatus = true;
      }
      if(u32TimeCounter == (TimeAirValve*100))
      {
          AirValve(VALVE_CLOSE);
          bAirValveStatus = true;
      }

      if((bAirValveStatus == true)&&(bOxygenValveStatus == true))
      {
          /*Check Oxygen and pressure values*/
          u32ReadOxygenRatio = Stub_ReadOxygenSensor();
          u32Pressure = u32ReadPressure();
          if((u32ReadOxygenRatio < (u32WantedOxygenRatio+OXYGEN_REFILL_MARGIN))
            &&(u32ReadOxygenRatio > (u32WantedOxygenRatio-OXYGEN_REFILL_MARGIN))
            &&(u32Pressure <= PRESSURE_UP)
            &&(u32Pressure > PRESSURE_MID))
          {
              u32TimeCounter = 0;
              bAirValveStatus = false;
              bOxygenValveStatus = false;
              /*Mix Ready inform flow circuit and go to monitoring status*/
              Stub_MixReadyPin(true);
              enuSysCurrentState = TANK_MONITORING;
          }

      }
      break;
  case TANK_MONITORING :
      /*1- Read oxygen sensor
       *2- Read pressure sensor and temperature
       *3- check that their value on accepted range
       *4- Depend on what fill below the threshold, calculate needed time for each value
       *5- Open valves and move to TANK_FILLING*/
      /*Check Oxygen and pressure values*/
      u32ReadOxygenRatio = Stub_ReadOxygenSensor();
      u32Pressure = u32ReadPressure();
      if((u32Pressure < PRESSURE_DOWN_LIMIT)
        ||(u32ReadOxygenRatio < (u32WantedOxygenRatio-OXYGEN_STOP_MARGIN))
        ||(u32ReadOxygenRatio > (u32WantedOxygenRatio+OXYGEN_STOP_MARGIN)))
      {
          /*Mix isn't Ready inform flow circuit and go to Error state*/
          Stub_MixReadyPin(false);
          enuSysCurrentState = ERROR_STATE;
      }

      if((u32Pressure < PRESSURE_DOWN)
        ||(u32ReadOxygenRatio < (u32WantedOxygenRatio-OXYGEN_REFILL_MARGIN))
        ||(u32ReadOxygenRatio > (u32WantedOxygenRatio+OXYGEN_REFILL_MARGIN)))
      {
          enuSysCurrentState = VALVES_SETTING;
      }
      break;
  case ERROR_STATE :
      u8ErrorCount++;
       if(u8ErrorCount<=3)
       {
           /*Retry*/
           enuSysCurrentState = VALVES_SETTING;
       }
       else
       {
           /*Go wait for another trigger*/
           u8ErrorCount = 0;
           enuSysCurrentState = INIT;
       }
      break;
  default:
      break;

  }

}

#endif

