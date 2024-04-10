/*
 * Displaytemp.c
 *
 *  Created on: 2024年4月3日
 *      Author: 小弟
 */
#include "Displaytemp.h"
#define SRC_MAX_X 32
#define SRC_MAX_Y 24
#define DST_MAX_X 160
#define DST_MAX_Y 120
#define scale_x   198  //0.19375*1024
#define scale_y   196  //0.19166*1024
 uint16_t camColors[] = {
0x400F,0x400F,0x400F,0x4010,0x3810,0x3810,0x3810,
0x3810,0x3010,0x3010,0x3010,0x2810,0x2810,0x2810,0x2810,
0x2010,0x2010,0x2010,0x1810,0x1810,0x1811,0x1811,0x1011,
0x1011,0x1011,0x0811,0x0811,0x0811,0x0011,0x0011,0x0011,
0x0011,0x0011,0x0031,0x0031,0x0051,0x0072,0x0072,0x0092,
0x00B2,0x00B2,0x00D2,0x00F2,0x00F2,0x0112,0x0132,0x0152,
0x0152,0x0172,0x0192,0x0192,0x01B2,0x01D2,0x01F3,0x01F3,
0x0213,0x0233,0x0253,0x0253,0x0273,0x0293,0x02B3,0x02D3,
0x02D3,0x02F3,0x0313,0x0333,0x0333,0x0353,0x0373,0x0394,
0x03B4,0x03D4,0x03D4,0x03F4,0x0414,0x0434,0x0454,0x0474,
0x0474,0x0494,0x04B4,0x04D4,0x04F4,0x0514,0x0534,0x0534,
0x0554,0x0554,0x0574,0x0574,0x0573,0x0573,0x0573,0x0572,
0x0572,0x0572,0x0571,0x0591,0x0591,0x0590,0x0590,0x058F,
0x058F,0x058F,0x058E,0x05AE,0x05AE,0x05AD,0x05AD,0x05AD,
0x05AC,0x05AC,0x05AB,0x05CB,0x05CB,0x05CA,0x05CA,0x05CA,
0x05C9,0x05C9,0x05C8,0x05E8,0x05E8,0x05E7,0x05E7,0x05E6,
0x05E6,0x05E6,0x05E5,0x05E5,0x0604,0x0604,0x0604,0x0603,
0x0603,0x0602,0x0602,0x0601,0x0621,0x0621,0x0620,0x0620,
0x0620,0x0620,0x0E20,0x0E20,0x0E40,0x1640,0x1640,0x1E40,
0x1E40,0x2640,0x2640,0x2E40,0x2E60,0x3660,0x3660,0x3E60,
0x3E60,0x3E60,0x4660,0x4660,0x4E60,0x4E80,0x5680,0x5680,
0x5E80,0x5E80,0x6680,0x6680,0x6E80,0x6EA0,0x76A0,0x76A0,
0x7EA0,0x7EA0,0x86A0,0x86A0,0x8EA0,0x8EC0,0x96C0,0x96C0,
0x9EC0,0x9EC0,0xA6C0,0xAEC0,0xAEC0,0xB6E0,0xB6E0,0xBEE0,
0xBEE0,0xC6E0,0xC6E0,0xCEE0,0xCEE0,0xD6E0,0xD700,0xDF00,
0xDEE0,0xDEC0,0xDEA0,0xDE80,0xDE80,0xE660,0xE640,0xE620,
0xE600,0xE5E0,0xE5C0,0xE5A0,0xE580,0xE560,0xE540,0xE520,
0xE500,0xE4E0,0xE4C0,0xE4A0,0xE480,0xE460,0xEC40,0xEC20,
0xEC00,0xEBE0,0xEBC0,0xEBA0,0xEB80,0xEB60,0xEB40,0xEB20,
0xEB00,0xEAE0,0xEAC0,0xEAA0,0xEA80,0xEA60,0xEA40,0xF220,
0xF200,0xF1E0,0xF1C0,0xF1A0,0xF180,0xF160,0xF140,0xF100,
0xF0E0,0xF0C0,0xF0A0,0xF080,0xF060,0xF040,0xF020,
0x0000,0xffff
};
uint16_t EE[832];
uint16_t mlx90640_frame[834];
float mlx90640_To[768];
uint8_t imagebuf[120*160];
uint16_t To16_mlx90640[768];

float T_a;
float emissivity=0.95;
paramsMLX90640 mlx_90640;





void mlx_90640_init(void)
{
    MLX90640_I2CInit();
    delay_ms(50);
    int status;
    uint16_t test;
//     MLX90640_I2CWrite(0x33, 0x800D, 0x1901);

    MLX90640_SetRefreshRate(0x33, 0x04);//设置帧率

     MLX90640_I2CRead(0x33,0x800D,1,&test);
     status = MLX90640_DumpEE(0x33, EE);  //读取像素校正参数
     printf("hhh:%x\r\n",EE[0]);
     printf("test:%x\r\n",test);
     if (status != 0)printf("\r\nload system parameters error with code:%d\r\n",status);
     status = MLX90640_ExtractParameters(EE, &mlx_90640);  //解析校正参数
     if (status != 0)printf("\r\nParameter extraction failed with error code:%d\r\n",status);


}

void mlx90640_Getvalue(void)
{

int status;

    for(int i=0;i<2;i++)
    {
//MLX90640_I2CRead(0x33, 0x8000, 1, &statusReg);
//if(statusReg&0x0008)
//{
    status= MLX90640_GetFrameData(0x33, mlx90640_frame);

    float vdd = MLX90640_GetVdd(mlx90640_frame, &mlx_90640);
        T_a = MLX90640_GetTa(mlx90640_frame, &mlx_90640);
        printf("vdd:  %.2f Tr: %.2f  Status:%d\r\n",vdd,T_a-8,status);


        MLX90640_CalculateTo(mlx90640_frame, &mlx_90640, emissivity, T_a-8.0, mlx90640_To);
            MLX90640_BadPixelsCorrection(mlx_90640.brokenPixels, mlx90640_To, 0, &mlx_90640);  //坏点处理
//            MLX90640_BadPixelsCorrection(mlx_90640.outlierPixels, mlx90640_To, 0, &mlx_90640);  //坏点处理

//                   printf("\r\n==========================IAMLIUBO MLX90640 WITH STM32 SWI2C EXAMPLE Github:github.com/imliubo==========================\r\n");
//                   for(int i = 0; i < 768; i++)
//                   {
//
//                       if(i%32 == 0 && i != 0)
//                       {
//                           printf("\r\n");
//                       }
//                       printf("%.2f,",mlx90640_To[i]);
//                   }
//         printf("\r\n");

//    }

    }

}

void Disp_TempPic(void)
{
    uint16_t x,y,color,adr,dst;
    uint16_t max=0,min=3400,scale;
    uint16_t fx,fy,cbufx[2],cbufy[2];
    uint8_t  sx,sy;

    for (int i = 0; i < 768; i++)
    {
        To16_mlx90640[i] = (uint16_t)(mlx90640_To[i]*10);

            if(To16_mlx90640[i]>max){max = To16_mlx90640[i];}
            if(To16_mlx90640[i]<min){min = To16_mlx90640[i];}

    }

    scale = 2530/(max-min);
    fy=0;adr=0;
    for(y = 0;y<120;y++)
    {
        sy = fy>>10;                                       //SrcX中的整数

        cbufy[1] = fy&0x3FF;         //v
        cbufy[0] = 1024-cbufy[1];                      //1-v

        fy+=scale_y;

        /******************************************* 0-31列(0-15) *****************************/
        fx=0;
        for (x = 0; x < 160; x++)
        {
            sx = fx>>10;                                   //SrcX中的整数

            cbufx[1] =fx&0x3FF;     //u
            cbufx[0] = 1024-cbufx[1];                  //1-u

            fx+=scale_x;

            color=sy*32+31-sx;
            dst =    (  To16_mlx90640[ color    ] * cbufx[0] * cbufy[0] +
                        To16_mlx90640[ color+32 ] * cbufx[0] * cbufy[1] +
                        To16_mlx90640[ color-1  ] * cbufx[1] * cbufy[0] +
                        To16_mlx90640[ color+31 ] * cbufx[1] * cbufy[1] )>>20;

            dst=( (dst-min)*scale )/10;  //

            imagebuf[adr++]=dst;
            lcd_draw_point_color( x, y, camColors[imagebuf[x+(y)*160]]);
        }
    }
}







