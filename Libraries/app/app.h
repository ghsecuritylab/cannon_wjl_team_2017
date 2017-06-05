#ifndef _APP_H_
#define _APP_H_

#include "includes.h"
#include "ui.h"
#include "key.h"
#include "ad.h"
#include "adc.h"
#include "dma.h"
#include "flash.h"
#include "FlashUI.h"
#include "pwm.h"
#include "conm.h"

#define TASK_STK_SIZE              (128)
#define UI_STK_SIZE                (2048)
#define KEY_STK_SIZE               (128)
#define AD_STK_SIZE                (128)
#define SPEED_STK_SIZE             (128)
#define TURN_STK_SIZE             (128)
//#define BLUETOOTH_STK_SIZE         (128)   //蓝牙

#define APP_START_TASK_PRIO        (4)
#define APP_UI_PRIO                (9)
#define APP_KEY_PRIO               (8)            //在ui之前
#define APP_AD_PRIO                (5) 
#define APP_SPEED_PRIO             (6) 
#define APP_TURN_PRIO              (7) 
//#define APP_BLUETOOTH_PRIO         (11)           //蓝牙


extern OS_STK  APP_START_STK[TASK_STK_SIZE];
extern OS_STK  APP_UI_STK[UI_STK_SIZE];
extern OS_STK  APP_KEY_STK[KEY_STK_SIZE];
extern OS_STK  APP_AD_STK[AD_STK_SIZE];
extern OS_STK  APP_SPEED_STK[SPEED_STK_SIZE];
extern OS_STK  APP_TURN_STK[TURN_STK_SIZE];
//extern OS_STK  APP_BLUETOOTH_STK[BLUETOOTH_STK_SIZE];    //蓝牙

extern OS_EVENT *msg_key;				  //按键邮箱块指针

extern int ENCODE;  //编码器的值

void AppStartTast(void *pdata);   //创建任务的任务
void UiStartTask(void *pdata);   //创建任务的任务
void KeyStartTask(void *pdata);   //创建任务的任务
void AdStartTask(void *pdata);
void speedStartTask(void *pdata);
void TurnStartTask(void *pdata);    //转向任务
#endif