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

#include "ibutton.h"
#include "delay.h"

#define THERM_PORT GPIOA
#define THERM_PIN  GPIO_PIN_3

#define THERM_INPUT_MODE()  (THERM_PORT->DDR &= ~THERM_PIN)
#define THERM_OUTPUT_MODE() (THERM_PORT->DDR |=  THERM_PIN)
#define THERM_LOW()         (THERM_PORT->ODR &= (u8)(~THERM_PIN))
#define THERM_HIGH()        (THERM_PORT->ODR |= (u8)THERM_PIN)
#define THERM_READ()        (THERM_PORT->IDR & (u8)THERM_PIN)

#define THERM_CMD_CONVERTTEMP 0x44
#define THERM_CMD_RSCRATCHPAD 0xBE
#define THERM_CMD_WSCRATCHPAD 0x4E
#define THERM_CMD_CPYSCRATCHPAD 0x48
#define THERM_CMD_RECEEPROM 0xB8
#define THERM_CMD_RPWRSUPPLY 0xB4
#define THERM_CMD_SEARCHROM 0xF0
#define THERM_CMD_READROM 0x33
#define THERM_CMD_MATCHROM 0x55
#define THERM_CMD_SKIPROM 0xCC
#define THERM_CMD_ALARMSEARCH 0xEC

/* Reset operation
* 0 - OK
* 1 - ERROR
*/
uint8_t therm_reset(void)
{
  uint8_t i = 0xFF;
  
  THERM_OUTPUT_MODE();
  THERM_LOW();
  
  delay_us(480);
  
  THERM_INPUT_MODE();
  
  delay_us(60);
  
  i = THERM_READ();
  
  delay_us(480);
  return i;
}

/* Write bit */
void therm_write_bit(uint8_t bBit)
{
  THERM_OUTPUT_MODE();
  THERM_LOW();
  
  delay_us(1);
  
  if (bBit) {
    THERM_INPUT_MODE();
  }
  
  delay_us(60);
  THERM_INPUT_MODE();
}

/* Read bit */
bool therm_read_bit(void)
{
  bool bBit = FALSE;
  
  THERM_OUTPUT_MODE();
  THERM_LOW(); 
  
  delay_us(1);
  
  THERM_INPUT_MODE();
  
  delay_us(15);
  
  if (THERM_READ()) {
    bBit = TRUE;
  }
  
  delay_us(45);
  return bBit;
}

/* Read operation */
uint8_t therm_read_byte(void)
{
  uint8_t n = 0;
  
  for (uint8_t i = 0; i < 8; ++i) {
    n >>= 1;
    n |= (therm_read_bit() << 7);
  }
  
  return n;
}

/* Write operation */
void therm_write_byte(uint8_t byte)
{
  for (uint8_t i = 0; i < 8; ++i) {
    therm_write_bit(byte & 1);
    byte >>= 1;
  }
}

/* Init device */
void Dallas_InitMode(THERM_MODE mode)
{
  therm_reset();
  therm_write_byte(THERM_CMD_SKIPROM);
  therm_write_byte(THERM_CMD_WSCRATCHPAD);
  therm_write_byte(0);
  therm_write_byte(0);
  therm_write_byte(mode);
}

/* Get 2 bytes temp and place it to buffer */
void Dallas_GetTemp(uint8_t* buffer)
{
  /* Disable interrupts before reading temp sensor */
  disableInterrupts();
  
  therm_reset();
  therm_write_byte(THERM_CMD_SKIPROM);
  therm_write_byte(THERM_CMD_CONVERTTEMP);
  
  /* Wait 1s until the operation is complete */
  do {
    for (int i = 0; i < 20; ++i)
      delay_us(50000);
  } while ( !therm_read_bit() );
  
  /* Read temp */
  therm_reset();
  therm_write_byte(THERM_CMD_SKIPROM);
  therm_write_byte(THERM_CMD_RSCRATCHPAD);  
  *buffer++ = therm_read_byte();
  *buffer   = therm_read_byte();
  
  enableInterrupts();
}