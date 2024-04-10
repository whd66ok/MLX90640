/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 USART Print debugging routine:
 USART1_Tx(PA9).
 This example demonstrates using USART1(PA9) as a print debug port output.

*/

#include "debug.h"
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"
#include "lcd.h"

#include "Displaytemp.h"
#include "string.h"



unsigned short EE2[832];
unsigned short Frame[834];
paramsMLX90640 MLXPars;
    uint16_t data800f=0xffff;

    int status2;
    float Temp[768];
    float Ta;
/* Global typedef */

/* Global define */
    uint16_t state;
    uint16_t m[1];
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();
	USART_Printf_Init(115200);	
    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("This is printf example\r\n");

//    printf("6666666");
//    fflush(stdout);
	lcd_init();

//    status2= MLX90640_DumpEE(MLX90640_ADDR, EE2);//读取校正参数
//    printf("\r\n");
//     for(int i=0;i<832;i++)
//    {
//        printf("%04x,",EE2[i]);
//        fflush(stdout);
//    }
//    if (status2!= 0)printf("\r\nload system parameters error with code:%d\r\n",status2);
//    status2= MLX90640_ExtractParameters(EE2, &MLXPars);   //解析校正参数为计算参数
//    if (status2 != 0)printf("\r\nParameter extraction failed with error code:%d\r\n",status2);


    mlx_90640_init();


	while(1)
    {
	    mlx90640_Getvalue();
	    Disp_TempPic();
	 }
}
