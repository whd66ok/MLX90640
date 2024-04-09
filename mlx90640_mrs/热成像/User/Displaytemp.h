/*
 * Displaytemp.h
 *
 *  Created on: 2024年4月3日
 *      Author: 小弟
 */

#ifndef USER_DISPLAYTEMP_H_
#define USER_DISPLAYTEMP_H_

#include "lcd.h"

#include "math.h"

void Disp_TempPic(void);
void interpolateBilinear(int newWidth, int newHeight);
#endif /* USER_DISPLAYTEMP_H_ */
