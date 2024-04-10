/*
 * iic.h
 *
 *  Created on: Mar 27, 2024
 *      Author: Ð¡µÜ
 */

#ifndef USER_IIC_H_
#define USER_IIC_H_

#include "ch32v30x_conf.h"
#include "debug.h"

#define MLX90640_SDA_PORT       GPIOB
#define MLX90640_SDA_PIN        GPIO_Pin_0
#define MLX90640_SCL_PORT       GPIOB
#define MLX90640_SCL_PIN        GPIO_Pin_1

#define IIC_SDA_IN()  {GPIOD->CFGHR&=0XFFFFFF0F;GPIOD->CFGHR|=(u32)8<<4;}
#define IIC_SDA_OUT() {GPIOD->CFGHR&=0XFFFFFF0F;GPIOD->CFGHR|=(u32)3<<4;}
//#define IIC_SCL(n)    (n?GPIO_WriteBit(GPIOD, GPIO_Pin_10, 1):GPIO_WriteBit(GPIOD, GPIO_Pin_10, 0))//SDA      //SCL
//#define IIC_SDA(n)    (n?GPIO_WriteBit(GPIOD, GPIO_Pin_9, 1):GPIO_WriteBit(GPIOD, GPIO_Pin_9, 0))//SDA
//#define READ_SDA       GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9)  //SDAÊäÈë
#define IIC_SCL(n)    (n?GPIO_WriteBit(GPIOB, GPIO_Pin_1, 1):GPIO_WriteBit(GPIOB, GPIO_Pin_1, 0))//SDA      //SCL
#define IIC_SDA(n)    (n?GPIO_WriteBit(GPIOB, GPIO_Pin_0, 1):GPIO_WriteBit(GPIOB, GPIO_Pin_0, 0))//SDA

#define READ_SDA       GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)  //SDAÊäÈë

#define SDA_LOW()                 GPIO_WriteBit(MLX90640_SDA_PORT, MLX90640_SDA_PIN, 0)
#define SDA_HIGH()                GPIO_WriteBit(MLX90640_SDA_PORT, MLX90640_SDA_PIN, 1)
#define SDA_GET()                 GPIO_ReadInputDataBit(MLX90640_SDA_PORT,MLX90640_SDA_PIN)
#define SCL_LOW()                 GPIO_WriteBit(MLX90640_SCL_PORT, MLX90640_SCL_PIN, 0)
#define SCL_HIGH()                GPIO_WriteBit(MLX90640_SCL_PORT, MLX90640_SCL_PIN, 1)
void MLX90640_I2CInit(void);
int MLX90640_I2CRead(uint8_t slaveAddr,uint16_t startAddress, uint16_t nMemAddressRead, uint16_t *data);
int MLX90640_I2CWrite(uint8_t slaveAddr,uint16_t writeAddress, uint16_t data);
void MLX90640_I2CFreqSet(int freq);

#endif /* USER_IIC_H_ */
