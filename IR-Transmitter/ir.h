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

#ifndef _IR_H
#define _IR_H

#include "stm8s.h"

/**
* @brief Init IR Pins
* @param None
* @retval None
* @par Required preconditions:
* None
*/
void IR_InitPins(void);

/**
* @brief Record IR pulses to PulseBuffer
* @param None
* @retval uint8_t: PulseCount
* @par Required preconditions:
* None
*/
uint8_t IR_CommandRecord(void);

/**
* @brief Replay IR pulses from PulseBuffer
* @param uint8_t: PulseCount
* @retval None
* @par Required preconditions:
* None
*/
void IR_CommandReplay(uint8_t pulseCount);

/**
* @brief Get PulseBuffer Pointer
* @param None
* @retval uint8_t*: Pointer
* @par Required preconditions:
* None
*/
uint8_t* IR_PulseBufferPointer(void);

/**
* @brief Returns high pulse duration.
* @param uint8_t: index
* @retval high pulse duration
* - uint16_t: duration in us
* @par Required preconditions:
* None
*/
uint16_t getHighPulse(uint8_t idx);

/**
* @brief Returns low pulse duration.
* @param uint8_t: index
* @retval low pulse duration
* - uint16_t: duration in us
* @par Required preconditions:
* None
*/
uint16_t getLowPulse(uint8_t idx);

#endif /* _IR_H */