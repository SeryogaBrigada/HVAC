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

#include "uart.h"

/**
* @addtogroup UART1
* @{
*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint8_t UART1_RX_Interrupt = 0x00;
extern __IO uint8_t TIM2_timeOut;

/** @brief  Send data by the UART1 peripheral.
* @param  The send data
* @retval None.
*/
void UART1_SendByte(uint8_t byte)
{
  /* Wait until end of transmit */
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
  /* Write one byte in the UART1 Transmit Data Register */
  UART1_SendData8(byte);
}

/** @brief  Returns the received data by the UART1 peripheral.
* @param  None
* @retval The received data.
*/
uint8_t UART1_ReceiveByte(void)
{
  /* Wait until end of receive */
//  while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET);
  
  while(!UART1_RX_Interrupt);
  UART1_RX_Interrupt = 0x00;
  /* Write one byte in the UART1 Transmit Data Register */
  return UART1_ReceiveData8();
}

/**
* @brief  Configure UART1
* @param  None
* @retval None
*/
void UART1_Config(void)
{
  /* EVAL COM (UART) configuration -----------------------------------------*/
  /* USART configured as follow:
  - BaudRate = 115200 baud  
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Receive and transmit enabled
  - UART Clock disabled
  */
  UART1_DeInit();
  UART1_Init((uint32_t)115200, UART1_WORDLENGTH_8D,UART1_STOPBITS_1, UART1_PARITY_NO,
             UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
  
  /* Enable the UART Receive interrupt: this interrupt is generated when the UART
  receive data register is not empty */
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
  
  /* Enable the UART Transmit complete interrupt: this interrupt is generated 
  when the UART transmit Shift Register is empty */
  //UART1_ITConfig(UART1_IT_TXE, ENABLE);
  
  /* Enable UART */
  UART1_Cmd(ENABLE);
  
  /* Enable general interrupts */
  enableInterrupts();
}

/** @brief  Send array of data by the UART1 peripheral.
* @param  uint8_t*: Pointer to array
*         uint16_t: length
* @retval None.
*/
void UART1_SendArray(uint8_t* buffer, uint16_t length)
{
  do {
    UART1_SendByte(*buffer);
    ++buffer;
  } while (--length);
}

/** @brief  Receive array of data from the UART1 peripheral.
* @param  uint8_t*: Pointer to array
*         uint16_t: length
* @retval None.
*/
void UART1_ReceiveArray(uint8_t* buffer, uint16_t length)
{
  do {
    *buffer = UART1_ReceiveByte();
    ++buffer;
  } while ( (--length) && (!TIM2_timeOut) );
}