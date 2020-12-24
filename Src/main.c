/*
 * main.c
 *
 *  Created on: Dec 11, 2020
 *      Author: HP
 */

#include "main.h"
#include <stdint.h>
#include<math.h>
#include<string.h>



void Error_handler(void);
void SystemClock_Config_HSE(void);
void TIM1_Init(void);
void TIM3_Init(void);
void changing_SPWM(void);

TIM_HandleTypeDef tim1;
TIM_HandleTypeDef  tim3;

float A, pi, f,t, omega, vs1, vs2, vs, fs, offset;

uint32_t period=8000;



int main(void)
{

  HAL_Init();

  SystemClock_Config_HSE();

  TIM1_Init();

  TIM3_Init();

  if (HAL_TIM_Base_Start_IT(&tim3) != HAL_OK )
  {
	  Error_handler();
  }

  if (HAL_TIM_PWM_Start(&tim1, TIM_CHANNEL_1) != HAL_OK )
  {
	  Error_handler();
  }

  if (HAL_TIM_PWM_Start(&tim1, TIM_CHANNEL_4) != HAL_OK )
   {
 	  Error_handler();
   }



  while (1);



}

void SystemClock_Config_HSE(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState= RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
     Error_handler();
  }



  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
	  Error_handler();
  }

    /**Configure the Systick interrupt time
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void TIM1_Init(void)
{
   TIM_MasterConfigTypeDef  masterConfig;
   TIM_OC_InitTypeDef       ocinit;
   TIM_BreakDeadTimeConfigTypeDef  breakdeadTimeConfig;

   tim1.Instance=TIM1;
   tim1.Init.Prescaler=0;
   tim1.Init.CounterMode=TIM_COUNTERMODE_UP;
   tim1.Init.Period=period;
   tim1.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
   tim1.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_DISABLE;
   tim1.Init.RepetitionCounter=0;

   if( HAL_TIM_PWM_Init(&tim1) != HAL_OK )
   {
	   Error_handler();
   }

   memset(&masterConfig,0,sizeof(masterConfig));
   masterConfig.MasterOutputTrigger=TIM_TRGO_RESET;
   masterConfig.MasterSlaveMode=TIM_MASTERSLAVEMODE_DISABLE;
   if( HAL_TIMEx_MasterConfigSynchronization(&tim1,&masterConfig) != HAL_OK )
   {
	   Error_handler();
   }

   memset(&ocinit,0,sizeof(ocinit));

   ocinit.OCMode=TIM_OCMODE_PWM1;
   ocinit.Pulse=0;
   ocinit.OCFastMode=TIM_OCFAST_DISABLE;
   ocinit.OCIdleState=TIM_OCIDLESTATE_RESET;
   ocinit.OCNIdleState=TIM_OCNIDLESTATE_RESET;
   ocinit.OCNPolarity=TIM_OCNPOLARITY_HIGH;
   ocinit.OCPolarity=TIM_OCPOLARITY_HIGH;

   if( HAL_TIM_PWM_ConfigChannel(&tim1,&ocinit,TIM_CHANNEL_1 ) != HAL_OK )
   {
	   Error_handler();
   }

   ocinit.OCMode=TIM_OCMODE_TIMING;
   if( HAL_TIM_PWM_ConfigChannel(&tim1,&ocinit,TIM_CHANNEL_4 ) != HAL_OK )
   {
	   Error_handler();
   }

   memset(&breakdeadTimeConfig,0,sizeof(breakdeadTimeConfig));

   breakdeadTimeConfig.AutomaticOutput=TIM_AUTOMATICOUTPUT_DISABLE;
   breakdeadTimeConfig.BreakFilter=0x0;
   breakdeadTimeConfig.BreakPolarity=TIM_BREAKPOLARITY_HIGH;
   breakdeadTimeConfig.BreakState=TIM_BREAK_DISABLE;
   breakdeadTimeConfig.DeadTime=0x00;
   breakdeadTimeConfig.LockLevel=TIM_LOCKLEVEL_OFF;
   breakdeadTimeConfig.OffStateIDLEMode=TIM_OSSI_DISABLE;
   breakdeadTimeConfig.OffStateRunMode=TIM_OSSR_DISABLE;

   if( HAL_TIMEx_ConfigBreakDeadTime(&tim1,&breakdeadTimeConfig) != HAL_OK )
   {
	   Error_handler();
   }


}

void TIM3_Init(void)
{
	TIM_ClockConfigTypeDef clockConfig;
	TIM_MasterConfigTypeDef  masterConfig;

   tim3.Instance=TIM3;
   tim3.Init.Prescaler=0;
   tim3.Init.CounterMode=TIM_COUNTERMODE_UP;
   tim3.Init.Period=period;
   tim3.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
   tim3.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_DISABLE;
   tim3.Init.RepetitionCounter=0;
   if( HAL_TIM_Base_Init(&tim3) != HAL_OK  )
   {
	   Error_handler();
   }

   memset(&clockConfig,0,sizeof(clockConfig));

   clockConfig.ClockSource=TIM_CLOCKSOURCE_INTERNAL;
   if( HAL_TIM_ConfigClockSource(&tim3,&clockConfig) != HAL_OK )
   {
	   Error_handler();
   }

   memset(&masterConfig,0,sizeof(masterConfig));

   masterConfig.MasterOutputTrigger=TIM_TRGO_RESET;
   masterConfig.MasterSlaveMode=TIM_MASTERSLAVEMODE_DISABLE;
   if( HAL_TIMEx_MasterConfigSynchronization(&tim3,&masterConfig) != HAL_OK )
   {
	   Error_handler();
   }

}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	changing_SPWM();

}


void changing_SPWM(void)
{
	float A=2047, pi=3.14285714, f=50.0,  omega=2*pi*f , fs=10000.0;
	float offset=A;


		vs=A * sinf(omega*t);
		vs1=vs+offset;
		vs2=offset-vs;

		t=t+(1/fs);

		if (t>=1.0/f)
		{
			t=0;
		}

		TIM1->CCR1=vs1;
		TIM1->CCR2=vs2;

		if (t>=0 && t<=1.0/f/2)
		{

			HAL_TIM_PWM_Stop(&tim1, TIM_CHANNEL_4);

			HAL_TIM_PWM_Start(&tim1, TIM_CHANNEL_1);

		}
		else if (t>1.0/f/2 && t<=1.0/f)
		{
			HAL_TIM_PWM_Stop(&tim1, TIM_CHANNEL_1);

			HAL_TIM_PWM_Start(&tim1, TIM_CHANNEL_4);
		}

}



 void Error_handler(void)
 {
	 while(1);
 }


