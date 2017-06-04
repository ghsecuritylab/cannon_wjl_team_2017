#include "gpio.h"
#include "common.h"
#include "systick.h"
#include "uart.h"
//uCOS
#include "includes.h"
#include "app.h"
#include "key.h"
#include "oled.h"


int main(void)
{
	 allInit();
// flash_write(SECTOR_NUM, 0,(u32)(0));
// flash_write(SECTOR_NUM, 4,(u32)(0));  
// flash_write(SECTOR_NUM, 8,(u32)(0));  
// flash_write(SECTOR_NUM, 12,(u32)(0));  
// flash_write(SECTOR_NUM, 16,(u32)(0));  
// flash_write(SECTOR_NUM, 20,(u32)(0));  
// flash_write(SECTOR_NUM, 24,(u32)(0));
// flash_write(SECTOR_NUM, 28,(u32)(0));
// flash_write(SECTOR_NUM, 32,(u32)(0));
// flash_write(SECTOR_NUM, 36,(u32)(0));
// flash_write(SECTOR_NUM, 40,(u32)(0));
// flash_write(SECTOR_NUM, 44,(u32)(0));
	

    OSInit();  //OS初始化
	
		OSTaskCreate(AppStartTast,(void *)0,
                &APP_START_STK[TASK_STK_SIZE - 1],
                APP_START_TASK_PRIO); //create the first task

        
    /* 控制权交给操作系统,在执行OSStart()之后操作系统开始进行任务调度 */
    OSStart();
		/* 程序永远不会运行到这 */
}
