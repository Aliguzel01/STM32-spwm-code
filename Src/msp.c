/*
 * msp.c
 *
 *  Created on: Dec 11, 2020
 *      Author: HP
 */

 #include "main.h"

void  HAL_MspInit(void)
{
	//arm cortex mx işlemcinin grup yapılandırması
   HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

   //arm cortex mx işlemcide gerekli sistem istisnalarını etkinleştir
   SCB->SHCSR |=(0x7 << 16);

   //sistem istisnalarını için yapılandırma yapalım
   HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
   HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
   HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);


}


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{

	 GPIO_InitTypeDef gpioinit={0};

	 __HAL_RCC_TIM1_CLK_ENABLE();
	 __HAL_RCC_GPIOE_CLK_ENABLE();

	 //PE9-> TIM1_CH1
	 //PE14->TIM1_CH4
	 gpioinit.Pin=GPIO_PIN_9 | GPIO_PIN_14;
	 gpioinit.Mode=GPIO_MODE_AF_PP;
	 gpioinit.Pull=GPIO_NOPULL;
	 gpioinit.Speed=GPIO_SPEED_FREQ_MEDIUM;
	 gpioinit.Alternate=GPIO_AF1_TIM1;
	 HAL_GPIO_Init(GPIOE,&gpioinit);

}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
   __HAL_RCC_TIM3_CLK_ENABLE();

  // İnterupt configuration
   HAL_NVIC_SetPriority(TIM3_IRQn, 15, 0);
   HAL_NVIC_EnableIRQ(TIM3_IRQn);

}
