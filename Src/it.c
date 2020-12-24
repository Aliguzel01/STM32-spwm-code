/*
 * it.c
 *
 *  Created on: Dec 11, 2020
 *      Author: HP
 */

 #include "main.h"

extern TIM_HandleTypeDef tim3;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}



void TIM3_IRQHandler()
{

	HAL_TIM_IRQHandler(&tim3);

}
