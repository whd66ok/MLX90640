/*
 * Displaytemp.h
 *
 *  Created on: 2024��4��3��
 *      Author: С��
 */

#ifndef USER_DISPLAYTEMP_H_
#define USER_DISPLAYTEMP_H_

#include "lcd.h"

#include "math.h"

void Disp_TempPic(void);
void interpolateBilinear(int newWidth, int newHeight);
#endif /* USER_DISPLAYTEMP_H_ */
