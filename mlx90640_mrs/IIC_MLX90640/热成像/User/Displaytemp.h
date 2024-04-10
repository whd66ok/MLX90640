/*
 * Displaytemp.h
 *
 *  Created on: 2024年4月3日
 *      Author: 小弟
 */

#ifndef USER_DISPLAYTEMP_H_
#define USER_DISPLAYTEMP_H_

#include "lcd.h"
#include "MLX90640_API.h"
#include "math.h"

//typedef union {
//    uint16_t mlx90640_frame[834];
//    uint8_t Disbuf[10*160];
//}UnionData;
//
//typedef union{
//     float mlx90640_To[768];
//}UnionData2;


void mlx_90640_init(void);
void mlx90640_Getvalue(void);
void Disp_TempPic(void);

void setImagePixel(uint8_t x_coord, uint8_t y_coord, float temp);
uint8_t scaleTemp(float temp, const float scaledMax);
void temp_data_sort(float *frameData,float *result);//温度数据镜像
void interpolateBilinear(int newWidth, int newHeight);


void showx(void);
#endif /* USER_DISPLAYTEMP_H_ */
