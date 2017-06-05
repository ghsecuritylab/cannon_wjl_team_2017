#include "app.h"
#include "ctrl.h"
#include "ui.h"
#include "ad.h"

OS_STK  APP_START_STK[TASK_STK_SIZE];
OS_STK  APP_UI_STK[UI_STK_SIZE];
OS_STK  APP_KEY_STK[KEY_STK_SIZE];
OS_STK  APP_AD_STK[AD_STK_SIZE];
OS_STK  APP_SPEED_STK[SPEED_STK_SIZE];
OS_STK  APP_TURN_STK[TURN_STK_SIZE];
//OS_STK  APP_BLUETOOTH_STK[BLUETOOTH_STK_SIZE];    //蓝牙
//信号量 邮箱 
OS_EVENT *msg_key;				  //按键邮箱块指针

#define LongPressTime   20 //按键触发长按的时间
#define LongPressSpeed   2 //按键触发长按后执行速度

u8 key;
//int ad1;
//int ENCODE;
//int ad1;
//int ad2;
//int ad3;
//int ad4;

int speedswitch;
//声明一个存放左右差值变量  
int sum;



extern int ad1; 
extern int ad2; 
extern int ad3; 
extern int ad4; 

extern float guiyi_ad1;
extern float guiyi_ad3;
float sum_diff  = 0;

void UiStartTask(void *pdata)    //UI任务
{
	
	pdata=pdata;
	u8 ID=0;
	while(1)
	{
		ID = FunctionPointer[ID]();
		OSTimeDlyHMSM(0, 0, 0, 10);
	}
}

void KeyStartTask(void *pdata)
{
	
	msg_key = OSMboxCreate((void*)0);	//创建空消息邮箱
    pdata = pdata;
    u16 T =0;
    u16 Ti=0;
    u8 keysave;//上次按键状态保存
	while(1)
	{
		if(key_check(13) == 1)
		{ key =1;}   //上键
		else if(key_check(10) == 1)
		{ key = 2; }  //下键
		else if(key_check(11) == 1)
		{ key = 4;}  //右键
		else if(key_check(14) == 1)
		{ key = 3; }  //左键
		else if(key_check(12) == 1)
		{ key = 5; }  //确认键
		else key = 0;
		
		switch(keysave)
		{
			case 1:  //上
				if(key != keysave)
				{
					OSMboxPost(msg_key,(void *)keysave);     //发送邮箱
					T = 0;
					Ti = 0;
				}
				else
				{
					if(T > LongPressTime)                   // 触发长按
					{
						T= LongPressTime;
						if(Ti> LongPressSpeed)                //长按执行速度
						{
							Ti = 0;
							OSMboxPost(msg_key,(void *)keysave);
						}
						Ti++;
					}
					T++;
				}
				break;
				
			case 2: //下
				if(key != keysave)
				{
					OSMboxPost(msg_key,(void *)keysave);
					T = 0;
					Ti = 0;
				}
				else
				{
					if(T > LongPressTime) // 触发长按
					{
						T= LongPressTime;
						if(Ti> LongPressSpeed) //长按执行速度
						{
							Ti = 0;
							OSMboxPost(msg_key,(void *)keysave);
						}
						Ti++;
					}
					T++;
				}
			
				break;
				
			case 3: //左
				if(key != keysave)
				{
					OSMboxPost(msg_key,(void *)keysave);
					T = 0;
					Ti = 0;
				}
				else
				{
					if(T > LongPressTime) // 触发长按
					{
						T= LongPressTime;
						if(Ti> LongPressSpeed) //长按执行速度
						{
							Ti = 0;
							OSMboxPost(msg_key,(void *)keysave);
						}
						Ti++;
					}
					T++;
				}
				break;
				
			case 4: //右
				if(key != keysave)
				{
					OSMboxPost(msg_key,(void *)keysave);
					T = 0;
					Ti = 0;
				}
				else
				{
					if(T > LongPressTime) // 触发长按
					{
						T= LongPressTime;
						if(Ti> LongPressSpeed) //长按执行速度
						{
							Ti = 0;
							OSMboxPost(msg_key,(void *)keysave);
						}
						Ti++;
					}
					T++;
				}
				break;
				
				case 5: //确认
				if(key != keysave)
				{
					OSMboxPost(msg_key,(void *)keysave);
					T = 0;
					Ti = 0;
				}
				else
				{
					if(T > LongPressTime) // 触发长按
					{
						T= LongPressTime;
						if(Ti> LongPressSpeed) //长按执行速度
						{
							Ti = 0;
							OSMboxPost(msg_key,(void *)keysave);
						}
						Ti++;
					}
					T++;
				}
				break;
				
		}
		keysave = key; 
		OSTimeDlyHMSM(0, 0, 0, 10);
	}	

}

//AD转换任务
void AdStartTask(void *pdata)
{
	float sum_diff1=0;
//	float sum_diff  = 0;
	pdata = pdata;
  
	while(1)
    {
//				ad1=adc_once(HW_ADC0, 13);
//				ad2=adc_once(HW_ADC1, 13);
//				ad3=adc_once(HW_ADC1, 11);
//				ad4=adc_once(HW_ADC1, 12);
//			  filter();                //ad采值
			  ad();
// 归一前处理
		  //    guiyi_qian();
		//	    guiyi_ad3=ad4;
		//	    guiyi_ad1=ad3;
			guiyi();
//			  max_min();
			  sum_diff = (float)(((float)guiyi_ad1 - (float)guiyi_ad3)/((float)guiyi_ad1 + (float)guiyi_ad3))*1000;
//        if(((sum_diff-sum_diff1)>60)||((sum_diff-sum_diff1)<-60))
//					sum_diff=sum_diff1;
//				if(adc_count > 10)
//						adc_count = 1;
//				ADC1_buf[adc_count-1] = ad1;
//			  ADC2_buf[adc_count-1] = ad3;
//				for(i = 0;i < 10;i++)
//				{
//					ADC1_sum += ADC1_buf[i];
//					ADC2_sum += ADC2_buf[i];
//				}
//				adc1 = ADC1_sum / adc_count;
//				adc2 = ADC2_sum / adc_count;
//				
//			  adc_count++;
				
        OSTimeDlyHMSM(0, 0, 0, 5);
    }
}

//编码器采值
void speedStartTask(void *pdata)
{
	pdata = pdata;
	while(1)
	{
		ENCODE = DMA_CITER_ELINKNO_CITER_MASK - DMA_GetMajorLoopCount(HW_DMA_CH1);
		
		if(speedswitch)
		{
			speedCtrl();
		}
		DMA_CancelTransfer();
		DMA_SetMajorLoopCounter(HW_DMA_CH1, DMA_CITER_ELINKNO_CITER_MASK);
		DMA_EnableRequest(HW_DMA_CH1);
		
	  OSTimeDlyHMSM(0, 0, 0, 5);
	}

}
//舵机转角函任务
void TurnStartTask(void *pdata)
{
	
	pdata=pdata;
	while(1)
	{
     ctrl();
		OSTimeDlyHMSM(0, 0, 0, 5);
  }
}

void AppStartTast(void *pdata)            //主任务
{
#if OS_CRITICAL_METHOD == 3
	OS_CPU_SR cpu_sr;
#endif
	pdata = pdata;
	

	
	  OSTaskCreate(UiStartTask,(void *)0,
                &APP_UI_STK[UI_STK_SIZE - 1],
                APP_UI_PRIO); 
    OSTaskCreate(KeyStartTask,(void *)0,
                &APP_KEY_STK[KEY_STK_SIZE - 1],
                APP_KEY_PRIO); 
		OSTaskCreate(AdStartTask,(void *)0,
                &APP_AD_STK[AD_STK_SIZE - 1],
                APP_AD_PRIO); 
		OSTaskCreate(speedStartTask,(void *)0,
                &APP_SPEED_STK[SPEED_STK_SIZE - 1],
                APP_SPEED_PRIO); 
		OSTaskCreate(TurnStartTask,(void *)0,
                &APP_TURN_STK[TURN_STK_SIZE - 1],
                APP_TURN_PRIO); 
			
	OS_ENTER_CRITICAL();   //进入临界区
		//开启时钟节拍中断
	SYSTICK_Init(1000*1000/OS_TICKS_PER_SEC);
    SYSTICK_ITConfig(true);
    SYSTICK_Cmd(true);
	
	OS_EXIT_CRITICAL();   //退出临界区
	
	
	for(;;)
		{
				OSTaskSuspend(APP_START_TASK_PRIO);	//挂起起始任务.
		}
}

void allInit(void)
{
	 OLED_Init();   
   KEY_Init();  
   DelayInit();	
	 flash_init();   
   Flash_Read();   
	 Init_AD(HW_ADC0);
	 Init_AD(HW_ADC1);
	 speedInit();
	 DMA_PulseCountInit(HW_DMA_CH1, HW_GPIOA, 6);
}