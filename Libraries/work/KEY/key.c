#include "KEY.h"

/******按键初始化*******/
void KEY_Init()
{
  GPIO_QuickInit(HW_GPIOA,12, kGPIO_Mode_IPU );          //初始化 PTE0 管脚，复用功能为GPIO ，下降沿触发中断，上拉电阻
  GPIO_QuickInit(HW_GPIOA,11, kGPIO_Mode_IPU );          //初始化 PTE2 管脚，复用功能为GPIO ，下降沿触发中断，上拉电阻
  GPIO_QuickInit(HW_GPIOA,13, kGPIO_Mode_IPU );          //初始化 PTE4 管脚，复用功能为GPIO ，下降沿触发中断，上拉电阻
  GPIO_QuickInit(HW_GPIOA,14, kGPIO_Mode_IPU );          //初始化 PTE6 管脚，复用功能为GPIO ，下降沿触发中断，上拉电阻
	GPIO_QuickInit(HW_GPIOA,10, kGPIO_Mode_IPU );          //初始化 PTE6 管脚，复用功能为GPIO ，下降沿触发中断，上拉电阻
	//GPIO_QuickInit(HW_GPIOA,29, kGPIO_Mode_IPU );          //干簧管
}


u8 key_check(u8 key)
{
    
    if(PAin(key))
    {
        OSTimeDly(0);
        if(PAin(key))
        {
            return 0;
        }
    }
    return 1;
}







