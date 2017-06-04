#ifndef CTRL_H_
#define CRTL_H_

#include "includes.h"

extern int result; 

#define  KPPLUSMAX      (190)
#define  KPNEGATIVEMAX  (-190)
#define  KIPLUSMAX      (190)
#define  KINEGATIVEMAX  (-190)
#define  KDPLUSMAX      (190)
#define  KDNEGATIVEMAX  (-190)
#define  KWPLUSMAX      (190)
#define  KWNEGATIVEMAX  (-190)
#define  KOUPLUSMAX     (5000)

//extern PID streepid;       //转向pid
//extern PID speedpid;       //速度pid
//extern int speed;          //速度
//extern int ad1_value;              //ad1滤波后的值
//extern int ad2_value;              //ad2滤波后的值
//int ad1_value;             //ad1滤波后的值
//int ad2_value;              //ad2滤波后的值

void linePID(int nowValue);
void speedCtrl(void);
void speedPID(u32 nowValue);
void streePID(int nowValue);
void ctrl(void);
void guiyi_qian(void);
//void filter(void);
int Turn_speed(int sum_ad);
	
#endif
