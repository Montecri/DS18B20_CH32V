// By cristianomonteiro@gmail.com - Nov 26, 2013
// DS18B20 1-Wire temperature reader for WCH CH32V003
// https://github.com/Montecri/DS18B20_CH32V
// MIT Licensed
//
// Ported from https://github.com/lamik/DS18B20_STM32_HAL


#include "main.h"
#include "debug.h"
#include "onewire.h"
#include "ds18b20.h"
#include "string.h"
#include <stdio.h>

float temperature;
char message[64];
uint8_t j = 0;

void GPIO_Toggle_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LED_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GPIO_Port, &GPIO_InitStructure);
}


int main(void)
{
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Delay_Init();
  USART_Printf_Init(9600);

  GPIO_Toggle_INIT();

  DS18B20_Init(DS18B20_Resolution_12bits);
  printf("Started!\n");
  
  while (1)
  {
	  DS18B20_ReadAll();
	  GPIO_WriteBit(TEST_GPIO_Port, TEST_Pin, 1);
    DS18B20_StartAll();
    GPIO_WriteBit(TEST_GPIO_Port, TEST_Pin, 0);
		uint8_t ROM_tmp[8];
		uint8_t i;

    //for(i = 0; i < 8; i++)
    for(i = 0; i < 1; i++)
      {
        if(DS18B20_GetTemperature(i, &temperature))
          {
            DS18B20_GetROM(i, ROM_tmp);
        
            // printf enabling will bloat code, so I use the trick below
            printf("%d. ROM: %X%X%X%X%X%X%X%X Temp: %d.%d oC\n\r",i, ROM_tmp[0], ROM_tmp[1], ROM_tmp[2], ROM_tmp[3], ROM_tmp[4], ROM_tmp[5], ROM_tmp[6], ROM_tmp[7], (int)temperature, (int)(temperature*100) % 100);
          }
      }
		Delay_Ms(1000);
    GPIO_WriteBit(LED_GPIO_Port, LED_Pin, (j == 0) ? (j = Bit_SET) : (j = Bit_RESET));
  }
}
