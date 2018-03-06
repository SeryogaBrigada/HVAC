/****************************************************************************
**
** Copyright (C) 2018 Sergey Kovalenko <seryoga.engineering@gmail.com>
**
** This file is part of IR-Transmitter.
** IR-Transmitter is free software: you can redistribute
** it and/or modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation, either version 3 of
** the License, or (at your option) any later version.
**
** IR-Transmitter is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU General Public License for more details.
**
** You should have received a copy of the
** GNU General Public License along with IR-Transmitter.
** If not, see http://www.gnu.org/licenses/.
**
****************************************************************************/

#include "ir.h"
#include "delay.h"

/* Private define ------------------------------------------------------------*/
#define MAX_PULSE_COUNT         120
#define MAX_PULSE_DURATION      65000 /*us*/

#define IR_PORT                 GPIOC
#define IR_IN_Pin               GPIO_PIN_3
#define IR_OUT_Pin              GPIO_PIN_4

/* Private variables ---------------------------------------------------------*/
uint8_t pulseBuffer[4*MAX_PULSE_COUNT]; /* 120 pairs of uint16_t pulses */
uint8_t currentPulseCount = 0x00;

/**
* @brief Get PulseBuffer Pointer
* @param None
* @retval uint8_t*: Pointer
* @par Required preconditions:
* None
*/
uint8_t* IR_PulseBufferPointer(void)
{
  return pulseBuffer;
}

/**
* @brief Returns high pulse duration.
* @param uint8_t: index
* @retval high pulse duration
* - uint16_t: duration in us
* @par Required preconditions:
* None
*/
uint16_t getHighPulse(uint8_t idx)
{
  uint16_t index = 4*idx;
  return (uint16_t)((pulseBuffer[index] << 8) | pulseBuffer[index + 1]);
}

/**
* @brief Returns low pulse duration.
* @param uint8_t: index
* @retval low pulse duration
* - uint16_t: duration in us
* @par Required preconditions:
* None
*/
uint16_t getLowPulse(uint8_t idx)
{
  uint16_t index = 4*idx + 2;
  return (uint16_t)((pulseBuffer[index] << 8) | pulseBuffer[index + 1]);
}

/**
* @brief Put HighPulseDuration in pulseBuffer
* @param uint8_t: index
*        uint16_t: pulseDuration
* @retval None
* @par Required preconditions:
* None
*/
void putHighPulse(uint8_t idx, uint16_t pulseDuration)
{
  uint16_t index         = 4*idx;
  pulseBuffer[index]     = pulseDuration >> 8;
  pulseBuffer[index + 1] = pulseDuration;
}

/**
* @brief Put LowPulseDuration in pulseBuffer
* @param uint8_t: index
*        uint16_t: pulseDuration
* @retval None
* @par Required preconditions:
* None
*/
void putLowPulse(uint8_t idx, uint16_t pulseDuration)
{
  uint16_t index         = 4*idx + 2;
  pulseBuffer[index]     = pulseDuration >> 8;
  pulseBuffer[index + 1] = pulseDuration;
}


/**
* @brief Init IR Pins
* @param None
* @retval None
* @par Required preconditions:
* None
*/
void IR_InitPins(void)
{
  GPIO_Init(IR_PORT, IR_IN_Pin, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(IR_PORT, IR_OUT_Pin, GPIO_MODE_OUT_PP_LOW_FAST);
}

/**
* @brief Clean PulseBuffer
* @param None
* @retval None
* @par Required preconditions:
* None
*/
void cleanPulseBuffer(void)
{
  for (uint16_t i = 0; i < (4*MAX_PULSE_COUNT); i++)
  {
    pulseBuffer[i] = 0x00;
  }
}

/**
* @brief Record IR pulses to PulseBuffer
* @param None
* @retval uint8_t: PulseCount
* @par Required preconditions:
* None
*/
uint8_t IR_CommandRecord(void)
{
  uint8_t pulseCount = 0x00;
  uint16_t highPulseDuration = 0x00;
  uint16_t lowPulseDuration = 0x00;
  uint8_t status = 1;
  
  cleanPulseBuffer();
  
  while(status)
  {
    highPulseDuration = 0x00;
    lowPulseDuration = 0x00;
    
    /* Read highPulseDuration */
    while ( RESET != GPIO_ReadInputPin(IR_PORT, IR_IN_Pin) )
    {
      highPulseDuration += 20;
      
      delay_us(20);
      
      if ( (highPulseDuration > MAX_PULSE_DURATION) && pulseCount )
      {
        status = 0;
        break;
      }
    }
    
    putHighPulse(pulseCount, highPulseDuration);
    
    /* Read lowPulseDuration */
    while ( RESET == GPIO_ReadInputPin(IR_PORT, IR_IN_Pin) )
    {
      lowPulseDuration += 20;
      delay_us(20);
      
      if ( (lowPulseDuration > MAX_PULSE_DURATION) && pulseCount )
      {
        status = 0;
        break;
      }
    }
    
    putLowPulse(pulseCount, lowPulseDuration);
    
    ++pulseCount;
    
    if (pulseCount > MAX_PULSE_COUNT)
    {
      pulseCount = 0;
      status = 0;
      break;
    }
  }
  
  return pulseCount;
}

/**
* @brief Send 38kHz pulse
* @param uint16_t: Duration in us
* @retval None
* @par Required preconditions:
* None
*/
void pulseIR(uint16_t duration)
{
  while(duration > 19)
  {
    GPIO_WriteHigh(IR_PORT, IR_OUT_Pin);
    delay_us(10);
    GPIO_WriteLow(IR_PORT, IR_OUT_Pin);
    delay_us(10);
    duration -= 20;
  }
}

/**
* @brief Replay IR pulses from PulseBuffer
* @param uint8_t: PulseCount
* @retval None
* @par Required preconditions:
* None
*/
void IR_CommandReplay(uint8_t pulseCount)
{
  for( uint8_t i = 0; i < pulseCount; i++)
  {
    delay_us( getHighPulse(i) );
    pulseIR( getLowPulse(i) );
  }
}