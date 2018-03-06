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


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "delay.h"
#include "ibutton.h"
#include "ir.h"
#include "uart.h"

#include "stdio.h"

/* Private defines -----------------------------------------------------------*/
#define countof(a)   ((sizeof(a) / sizeof(*(a))) - 1)

/* Private function prototypes -----------------------------------------------*/
void processCommands(void);
/* Private functions ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint8_t TIM2_timeOut = 0;


/**
* @brief  Configure TIM2 to generate an update interrupt each 1s 
* @param  None
* @retval None
*/
static void TIM2_Config(void)
{
  TIM2_DeInit();
  
  /* Time base configuration */
  TIM2_TimeBaseInit(TIM2_PRESCALER_16384, 975);
  
  /* Prescaler configuration */
  TIM2_PrescalerConfig(TIM2_PRESCALER_16384, TIM2_PSCRELOADMODE_IMMEDIATE);
  
  /* Clear TIM4 update flag */
  TIM2_ClearFlag(TIM2_FLAG_UPDATE);
  
  /* TIM2 IT enable */
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
  
  /* enable interrupts */
  enableInterrupts();
}

void main(void)
{
  /* Clock configuration -----------------------------------------*/
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  
  TIM1_DeInit();
  TIM4_DeInit();
  ADC1_DeInit();
  I2C_DeInit();
  SPI_DeInit();
  
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
  
  /* Timer2 configuration ----------------------------------------*/
  TIM2_Config();
  
  /* GPIO Config unused pins -------------------------------------*/
  GPIO_Init(GPIOA, (GPIO_Pin_TypeDef)GPIO_PIN_ALL, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOB, (GPIO_Pin_TypeDef)GPIO_PIN_ALL, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)GPIO_PIN_ALL, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_ALL, GPIO_MODE_IN_PU_NO_IT);
  
  IR_InitPins();
  
  /* UART1 configuration -----------------------------------------*/
  UART1_Config();
  
  /* UART1 clean received data register --------------------------*/
  UART1_ReceiveData8();
  
  /* Infinite loop */
  while (1)
  {
    wfi();
    processCommands();
  }
}

uint8_t pulseCount      = 0x00;
uint8_t receivedCommand = 0x00;

/**
* @brief  Process Commands
* @param  None
* @retval None
*/
void processCommands(void)
{
  receivedCommand = UART1_ReceiveByte();
  
  switch(receivedCommand)
  {
  case 'i':
    {
      uint8_t buffer[] = "HVAC";
      UART1_SendArray(buffer, countof(buffer));
    }
    break;
    
  case 'r':
    pulseCount = IR_CommandRecord();
    UART1_SendByte(pulseCount);
    UART1_SendArray(IR_PulseBufferPointer(), pulseCount*4);
    break;
    
  case 'w':
    TIM2_SetCounter(0);
    TIM2_Cmd(ENABLE);
    
    pulseCount = UART1_ReceiveByte();
    UART1_ReceiveArray(IR_PulseBufferPointer(), pulseCount*4);
    
    TIM2_Cmd(DISABLE);
    TIM2_timeOut = 0;
    
    IR_CommandReplay(pulseCount);
    break;
    
  case 't':
    {
      uint8_t buffer [2];
      Dallas_GetTemp(buffer);
      UART1_SendArray(buffer, 2);
    }
    break;
    
  case 'p':
    IR_CommandReplay(pulseCount);
    break;
    
  default:
    break;
  }
  
  UART1_SendByte(receivedCommand);
}


#ifdef USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*   where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval : None
*/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
