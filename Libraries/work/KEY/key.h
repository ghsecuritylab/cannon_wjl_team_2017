#ifndef _KEY_H
#define _KEY_H
#include "includes.h"
 
//#define KEY_U  PAin(13)
//#define KEY_D  PAin(12)
//#define KEY_A  PAin(19)
//#define KEY_B  PAin(11)
//#define KEY_S  PAin(29)

static void KEY_Scan(void);
void KEY_Init(void);    //按键初始化
void KEY_Service(void); //中断管脚配置

u8 key_check(u8 key);  //扫描按键

#endif












