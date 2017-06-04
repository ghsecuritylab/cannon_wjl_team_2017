#ifndef __FLASHUI_H__
#define __FLASHUI_H__


#include <stdint.h>
#include <includes.h>
//#include "ctrl.h"

#define     FLASH_SECTOR_SIZE       (2*1024)                //扇区大小 为 2k 字节
#define     FLASH_SECTOR_NUM        (256)                   //扇区数
#define     SECTOR_NUM  (FLASH_SECTOR_NUM-1)         //尽量用最后面的扇区，确保安全
#define     FLASH_ALIGN_ADDR        4                       //地址对齐整数倍
typedef     uint32_t                  FLASH_WRITE_TYPE;       //flash_write 函数写入 的数据类型
#define flash_read(sectorNo,offset,type)        (*(type *)((u32)(((sectorNo)*FLASH_SECTOR_SIZE) + (offset)))) 
      
#define SECTOR_NUM (FLASH_SECTOR_NUM-1) 

typedef struct pid
{
  float p;
	float i;
	float d;
}PID;

typedef struct stree_vim_pid_
{
   float  Kp_1;
   float  Kp_2;
   float  Kp_3;
   float  Kp_4;
   float  Kp_5;

   float  Kd_1;
   float  Kd_2;
   float  Kd_3;
   float  Kd_4;
   float  Kd_5;

}VIM_PID;

void Flash_Read(void);
void Falsh_Reserve(void);
void Falsh_Reserve_Head(void); 
void Flash_Read_Head(void); 
extern u8 flash_write(u16 sector_num,u16 offset,FLASH_WRITE_TYPE data);
extern void flash_init(void);
extern u8 flash_erase_sector(u16 sector_num);



#endif




































