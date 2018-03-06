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

#ifndef _IBUTTON_H
#define _IBUTTON_H

#include "stm8s.h"

/* Operation Mode */
typedef enum
{
  THERM_MODE_9BIT  = 0x1F,
  THERM_MODE_10BIT = 0x3F,
  THERM_MODE_11BIT = 0x5F,
  THERM_MODE_12BIT = 0x7F
} THERM_MODE;

/* Init device */
void Dallas_InitMode(THERM_MODE mode);

/* Get 2 bytes temp and place it to buffer */
void Dallas_GetTemp(uint8_t* buffer);

#endif /* _IBUTTON_H */