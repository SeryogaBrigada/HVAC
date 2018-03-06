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

#ifndef _UART_H
#define _UART_H

#include "stm8s.h"

/* Includes ------------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void UART1_SendByte(uint8_t byte);
uint8_t UART1_ReceiveByte(void);
void UART1_SendArray(uint8_t* buffer, uint16_t length);
void UART1_ReceiveArray(uint8_t* buffer, uint16_t length);
void UART1_Config(void);

#endif /* _UART_H */