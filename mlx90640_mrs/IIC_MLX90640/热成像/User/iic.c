/*
 * iic.c
 *
 *  Created on: Mar 27, 2024
 *      Author: 小弟
 */
#include "iic.h"

int I2CSendByte(int8_t);
void I2CReadBytes(int, char *);
void I2CStart(void);
void I2CStop(void);
void I2CRepeatedStart(void);
void I2CSendACK(void);
void I2CSendNack(void);
int I2CReceiveAck(void);
void Wait(int);


static int freqCnt = 2;


void MLX90640_I2CInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;//使用PA1和PA2作为模拟IIC引脚，PC10对应SDA，PC11对应SCL
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //开漏输出模式
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

    SCL_HIGH();
    SDA_HIGH();
    I2CStop();
}





int MLX90640_I2CRead(uint8_t slaveAddr, uint16_t startAddress,uint16_t nMemAddressRead, uint16_t *data)
{
    uint8_t sa;
    int ack = 0;
    int cnt = 0;
    int i = 0;
    uint8_t cmd[2] = {0,0};
    uint8_t i2cData[1664] = {0};
    uint16_t *p;

    p = data;
    sa = (slaveAddr << 1);
    cmd[0] = startAddress >> 8;
    cmd[1] = startAddress & 0x00FF;

    I2CStop();
    Wait(freqCnt);
    I2CStart();
    Wait(freqCnt);

    ack = I2CSendByte(sa)!=0;
    if(ack != 0)
    {
        return -1;
    }

    ack = I2CSendByte(cmd[0])!=0;
    if(ack != 0)
    {
        return -1;
    }

    ack = I2CSendByte(cmd[1])!=0;
    if(ack != 0)
    {
        return -1;
    }

    I2CRepeatedStart();

    sa = sa | 0x01;

    ack = I2CSendByte(sa);
    if(ack != 0)
    {
        return -1;
    }

    I2CReadBytes((nMemAddressRead << 1), i2cData);

    I2CStop();

//    printf("data0:%02x %02x\r\n",i2cData[0],i2cData[1]);
    for(cnt=0; cnt < nMemAddressRead; cnt++)
    {
        i = cnt << 1;
        *p++ = (i2cData[i]<<8)|i2cData[i+1];
    }
    return 0;
}

void MLX90640_I2CFreqSet(int freq)
{
    freqCnt = freq>>1;
}

int MLX90640_I2CWrite(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data)
{
    uint8_t sa;
    int ack = 0;
    char cmd[4] = {0,0,0,0};
    static uint16_t dataCheck;

    sa = (slaveAddr << 1);
    cmd[0] = writeAddress >> 8;
    cmd[1] = writeAddress & 0x00FF;
    cmd[2] = data >> 8;
    cmd[3] = data & 0x00FF;

    I2CStop();
    Wait(freqCnt);
    I2CStart();
    ack = I2CSendByte(sa);
    if (ack != 0x00)
    {
        return 1;
    }

    for(int i = 0; i<4; i++)
    {
        ack = I2CSendByte(cmd[i]);

        if (ack != 0x00)
        {
            return -1;
        }
    }
    I2CStop();

    MLX90640_I2CRead(slaveAddr,writeAddress,1, &dataCheck);

    if ( dataCheck != data)
    {
        return -2;
    }

    return 0;
}

int I2CSendByte(int8_t data)
{
   int ack = 1;
   int8_t byte = data;

   for(int i=0;i<8;i++)
   {
       Wait(freqCnt);

       if(byte & 0x80)
       {
           SDA_HIGH();
       }
       else
       {
           SDA_LOW();
       }
       Wait(freqCnt);
       SCL_HIGH();
       Wait(freqCnt);
       Wait(freqCnt);
       SCL_LOW();
       byte = byte<<1;
   }

   Wait(freqCnt);
   ack = I2CReceiveAck();

   return ack;
}

void I2CReadBytes(int nBytes, char *dataP)
{
    uint8_t data;
    for(int j=0;j<nBytes;j++)
    {

        Wait(freqCnt);
        SDA_HIGH();

        data = 0;
        for(int i=0;i<8;i++){
            Wait(freqCnt);
            SCL_HIGH();
            Wait(freqCnt);
//            if(j==0)
//            {printf("da1:%d\r\n",SDA_GET());}
//            else {
//                printf("da2:%d\r\n",SDA_GET());
//            }
            data |= SDA_GET();
            if(i<7)
            {
                data = data<<1;
            }

//

//            if(j==0){printf("datbit:%x\r\n",data);}
//            else {
//                printf("datbit2:%x\r\n",data);
//            }
//            if(SDA_GET())
//                        {
//                data = data+1;
//            }
            Wait(freqCnt);
            SCL_LOW();
            Wait(freqCnt);
        }

        *(dataP+j) = data;
//        printf("%02x\r\n",data);
        if(j == (nBytes-1))
        {

            I2CSendNack();
        }
        else
        {

            I2CSendACK();
        }

    }

}

void Wait(int freqCnt)
{
//    Delay_Us(1);
    volatile uint16_t cnt=0;
    for(uint16_t i = 0;i<freqCnt;i++)
    {
        cnt = cnt++;
    }
}

void I2CStart(void)
{
    SDA_HIGH();
    SCL_HIGH();
    Wait(freqCnt);
    Wait(freqCnt);
    SDA_LOW();
    Wait(freqCnt);
    SCL_LOW();
    Wait(freqCnt);

}

void I2CStop(void)
{
    SCL_LOW();
    SDA_LOW();
    Wait(freqCnt);
    SCL_HIGH();
    Wait(freqCnt);
    SDA_HIGH();
    Wait(freqCnt);
}

void I2CRepeatedStart(void)
{
    SCL_LOW();
    Wait(freqCnt);
    SDA_HIGH();
    Wait(freqCnt);
    SCL_HIGH();
    Wait(freqCnt);
    SDA_LOW();
    Wait(freqCnt);
    SCL_LOW();

}

void I2CSendACK(void)
{
    SDA_LOW();
    Wait(freqCnt);
    SCL_HIGH();
    Wait(freqCnt);
    Wait(freqCnt);
    SCL_LOW();
    Wait(freqCnt);
    SDA_HIGH();

}

void I2CSendNack(void)
{
    SDA_HIGH();
    Wait(freqCnt);
    SCL_HIGH();
    Wait(freqCnt);
    Wait(freqCnt);
    SCL_LOW();
    Wait(freqCnt);
    SDA_HIGH();

}

int I2CReceiveAck(void)
{
    int ack;

    SDA_HIGH();
    Wait(freqCnt);
    SCL_HIGH();
    Wait(freqCnt);
    if(SDA_GET() == 0)
    {
        ack = 0;
    }
    else
    {
        ack = 1;
    }
    Wait(freqCnt);
    SCL_LOW();
    SDA_LOW();

    return ack;
}


