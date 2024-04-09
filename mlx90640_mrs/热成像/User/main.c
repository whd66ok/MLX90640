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

#include "lcd.h"

#include "Displaytemp.h"
#include "string.h"


//UART
uint8_t mlxrxbuffer[1544];
uint8_t mlx_flag = 0;
int Idex = 0;
uint16_t checksum = 0;

float mlx90640_uart[768];

#define  RXBUF_SIZE 1544
uint8_t RxBuffer[RXBUF_SIZE]={0};
uint8_t RxBuffer2[RXBUF_SIZE]={0};
uint16_t RecvFlag=0;
uint16_t Crc;//求校验和

/* Global typedef */

/* Global define */

void USART3_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    //开启时钟

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
    /* USART3 TX-->Pd8  RX-->Pd9 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;           //RX，输入上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART3, ENABLE);

}
void DMA_INIT(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

    // RX DMA 初始化，
    DMA_DeInit(DMA2_Channel11);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&UART8->DATAR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)RxBuffer;                   // 接收缓冲区
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      // 方向 : 外设 作为 源，即 内存 <- 外设
    DMA_InitStructure.DMA_BufferSize = RXBUF_SIZE;                          // 缓冲区长度为 RXBUF_SIZE
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                         // 循环模式，构成环形缓冲区
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA2_Channel11, &DMA_InitStructure);
//    USART_DMACmd(UART8, USART_DMAReq_Rx, ENABLE);

}
void USART8_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE);

    //开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_USART8, ENABLE);

    /* UART8   RX-->PE15 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode =USART_Mode_Rx;
    USART_Init(UART8, &USART_InitStructure);
    USART_Cmd(UART8, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(UART8, USART_IT_IDLE, ENABLE);

    DMA_Cmd(DMA2_Channel11, ENABLE);
    USART_DMACmd(UART8, USART_DMAReq_Rx, ENABLE);
}
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();
	USART_Printf_Init(115200);	
	lcd_init();
	Delay_Ms(50);
	DMA_INIT();
	USART3_CFG();
	USART8_CFG();
	while(1)
    {

	    // mxl90640串口接收处理
	    if(RecvFlag==1)
	    {
	        Crc=0;
	        RecvFlag=0;
	        for (uint16_t i =0;i<1542;i++)
	        {
                Crc +=RxBuffer2[i];
            }
            if(Crc == ((RxBuffer2[1542]<<8)|(RxBuffer2[1543])))
            {
                for (int i=0;i<768;i++)
                {
                    mlx90640_uart[i]=((RxBuffer2[4+i*2]<<8)|RxBuffer2[5+i*2])*0.01;
                }
                printf("ok\r\n");
            }
            Disp_TempPic();
//	        if((RxBuffer2[0]+RxBuffer2[1]+RxBuffer2[2]+RxBuffer2[3])==0xBC)
//	        {
//
//                printf("buf0:%02x buf1:%02x,%02x,%02x\r\n",RxBuffer2[0],RxBuffer2[1],RxBuffer2[2],RxBuffer2[3]);
//
//                printf("CRC:%04x H:%02x L:%02x \r\n",Crc,RxBuffer2[1542],RxBuffer2[1543]);
//
//	        }
//	        else
//	        {
//                printf("error_data\r\n");
//          }
	    }
    }
}
void UART8_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void UART8_IRQHandler(void)
{
    uint16_t clear;

     if(USART_GetITStatus(UART8, USART_IT_IDLE) == SET)   // 空闲中断
     {
         USART_ClearITPendingBit(UART8, USART_IT_IDLE);
         clear = UART8->STATR;
         clear = UART8->DATAR;
         DMA_Cmd(DMA2_Channel11, DISABLE);
         clear = RXBUF_SIZE -DMA_GetCurrDataCounter(DMA2_Channel11);
         DMA2_Channel11->CNTR = RXBUF_SIZE;
         printf("recv:%d\r\n",clear);

         if(clear==1544)
         {
             memcpy(RxBuffer2,RxBuffer,RXBUF_SIZE);
             RecvFlag =1;
         }
         else
         {
            memset(RxBuffer2,0,(unsigned int)RxBuffer);
         }
         DMA_Cmd(DMA2_Channel11, ENABLE);
     }
}
void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART3_IRQHandler(void)
{
    uint8_t ucTemp;
    if((USART_GetITStatus(USART3, USART_IT_RXNE) != RESET))
    {
        ucTemp = USART_ReceiveData(USART3);               //接收数据
        if(Idex==0 && ucTemp == 0x5A)
        {
            mlxrxbuffer[Idex] = ucTemp;
            checksum = mlxrxbuffer[Idex];
            Idex++;
        }else if(Idex ==1 && ucTemp == 0x5A){
            mlxrxbuffer[Idex++] = ucTemp;
            checksum += mlxrxbuffer[Idex-1];
        }else if (Idex>1 && Idex<1544) {
            mlxrxbuffer[Idex++]=ucTemp;
            if(Idex<1543) checksum += mlxrxbuffer[Idex-1];
            if (Idex==1544) {
                printf("MLX_flagok:%04x H:%02x L:%02x\r\n",checksum,mlxrxbuffer[1542],mlxrxbuffer[1543]);
                if (checksum == ((mlxrxbuffer[1542]<<8)|mlxrxbuffer[1543])) {
                    mlx_flag = 1;
                }else {
                    mlx_flag = 0;
                }
                checksum=0;
                Idex = 0;
            }
        }else {
            checksum=0;
            Idex = 0;
        }
        USART_ClearITPendingBit(USART3,USART_IT_RXNE);
    }
}
