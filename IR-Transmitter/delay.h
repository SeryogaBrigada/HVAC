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

#ifndef _DELAY_H
#define _DELAY_H

#include "stm8s.h"

/**
  * @brief  Inserts a delay time.
  * @param  uint16_t: delay time in microseconds.
  * @retval None
  */
void delay_us(uint16_t us);


#endif /* _DELAY_H */