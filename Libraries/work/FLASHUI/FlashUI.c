#include "includes.h"
#include "FlashUI.h"
#include "flash.h"
//#include "ctrl.h"

VIM_PID  stree_vim_pid;
PID streepid;       //转向pid
PID speedpid;       //速度pid
PID linepid;        //直线pid
int speed;          //速度

#define FTF    FTFL
#define PGM4      0x06  /* program phase program 4 byte */
#define ERSSCR    0x09  /* erase flash sector */
/*!
 *  @brief      Flash命令
 *  @return     命令执行结果(1成功，0失败)
 *  @since      v5.0
 */
static inline u8 flash_cmd(){

	FTF->FSTAT = (0
                    |  FTFL_FSTAT_CCIF_MASK        // 指令完成标志(写1清0)
                    |  FTFL_FSTAT_RDCOLERR_MASK    // 读冲突错误标志(写1清0)
                    |  FTFL_FSTAT_ACCERR_MASK      // 访问错误标志位(写1清0)
                    |  FTFL_FSTAT_FPVIOL_MASK      // 非法访问保护标志位(写1清0)
                  );
	while(!(FTF->FSTAT & FTFL_FSTAT_CCIF_MASK));  
	
	// 检查错误标志
    if( FTF->FSTAT & (FTFL_FSTAT_ACCERR_MASK | FTFL_FSTAT_RDCOLERR_MASK | FTFL_FSTAT_FPVIOL_MASK | FTFL_FSTAT_MGSTAT0_MASK))
    {
        return 0 ;                                  //执行命令出错
    }
    else
    {
        return 1;                                   //执行命令成功
    }


}


/*!
 *  @brief      写入长字节数据到 flash指定地址
 *  @param      sector_num      扇区号（0 ~ FLASH_SECTOR_NUM）
 *  @param      offset          写入扇区内部偏移地址（0~2043 中 4的倍数）
 *  @param      data            需要写入的数据
 *  @return     执行结果(1成功，0失败)
 *  @since      
 *  Sample usage:       flash_write(SECTOR_NUM,0,12);        //扇区255,偏移地址为0,写入数据:12
 */
u8 flash_write(u16 sector_num,u16 offset,FLASH_WRITE_TYPE data)
{

	u16 ret;
	u32 addr = sector_num * FLASH_SECTOR_SIZE + offset;
	u32 tmpdata;
	typedef union
	{
		uint32_t  word;
		uint8_t   byte[4];
	} dest;
//	ASSERT(offset % FLASH_ALIGN_ADDR == 0);             //偏移量必须为4的倍数
//    //此处提示警告，但是安全的……
//    ASSERT(offset <= FLASH_SECTOR_SIZE);                //扇区大小为2K，即偏移量必须不大于 0x800
//    //此处提示警告，但是安全的……
	//设置目标地址
	FTF->FCCOB1 = ((dest *)&addr)->byte[2];
	FTF->FCCOB2 = ((dest *)&addr)->byte[1];
	FTF->FCCOB3 = ((dest *)&addr)->byte[0];
	
	//设置低32为数据
	tmpdata = (u32)data;
	FTF->FCCOB4 = ((dest *)&tmpdata)->byte[3];
	FTF->FCCOB5 = ((dest *)&tmpdata)->byte[2];
	FTF->FCCOB6 = ((dest *)&tmpdata)->byte[1];
	FTF->FCCOB7 = ((dest *)&tmpdata)->byte[0];
	
	 // 设置擦除命令
    FTF->FCCOB0 = PGM4;
	
	 __disable_irq();   //禁止中断
    ret =  flash_cmd();    
    __enable_irq();     //开启中断
	
	if(ret ==0)
	{
		return 0;
	}
	return 1;
}


/*!
 *  @brief      擦除指定flash扇区
 *  @param      sector_num    扇区号（K60N512实际使用0~255）
 *  @return     执行结果(1成功，0失败)
 *  @since      v5.0
 *  Sample usage:       flash_erase_sector(127);        //擦除扇区127
 */
u8 flash_erase_sector(u16 sector_num)
{
	typedef union
	{
		uint32_t  word;
		uint8_t   byte[4];
	} dest;
	u32 addr = sector_num * FLASH_SECTOR_SIZE;
	/* set cmd */
	FTF->FCCOB0 = ERSSCR; 
	FTF->FCCOB1 = ((dest *)&addr)->byte[2];
	FTF->FCCOB2 = ((dest *)&addr)->byte[1];
	FTF->FCCOB3 = ((dest *)&addr)->byte[0];
	
	if(flash_cmd() == 0)
    {
        return 0;
    }

    if(sector_num == 0)
    {
        return flash_write(sector_num,0x00040C,0xFFFFFFFE);
    }

    return 1;
}



void Falsh_Reserve() 
{
  flash_erase_sector(SECTOR_NUM); 
  flash_write(SECTOR_NUM, 0,(u32)(speedpid.p*10));
	flash_write(SECTOR_NUM, 4,(u32)(speedpid.i*10));
  flash_write(SECTOR_NUM, 8,(u32)(speedpid.d*10));
	
// flash_write(SECTOR_NUM, 12,(u32)(linepid.p*10));     //知道pid
//  flash_write(SECTOR_NUM, 16,(u32)(linepid.i*10));   
//	flash_write(SECTOR_NUM, 20,(u32)(linepid.d*10));
//	
//  flash_write(SECTOR_NUM, 24,(u32)(speedpid.p*10));  
//  flash_write(SECTOR_NUM, 28,(u32)(speedpid.i*10));  
//  flash_write(SECTOR_NUM, 32,(u32)(speedpid.d*10));  
//  flash_write(SECTOR_NUM, 36,(u32)speed);  
	
	flash_write(SECTOR_NUM, 12,(u32)(streepid.p*10));  
  flash_write(SECTOR_NUM, 16,(u32)(streepid.i*10));  
  flash_write(SECTOR_NUM, 20,(u32)(streepid.d*10));  
  flash_write(SECTOR_NUM, 24,(u32)speed);  

}

void Flash_Read()  
{
  speedpid.p=((float)(flash_read(SECTOR_NUM, 0, u32)))/10;
  speedpid.i=((float)(flash_read(SECTOR_NUM, 4, u32)))/10;
  speedpid.d=((float)(flash_read(SECTOR_NUM, 8, u32)))/10;
	
  streepid.p=((float)(flash_read(SECTOR_NUM, 12, u32)))/10;
  streepid.i=((float)(flash_read(SECTOR_NUM, 16, u32)))/10;
  streepid.d=((float)(flash_read(SECTOR_NUM, 20, u32)))/10;
//	
//	linepid.p=((float)(flash_read(SECTOR_NUM, 24, u32)))/10;
//  linepid.i=((float)(flash_read(SECTOR_NUM, 28, u32)))/10;
//  linepid.d=((float)(flash_read(SECTOR_NUM, 32, u32)))/10;
//  speed=flash_read(SECTOR_NUM, 36, u32);
	
	speed=flash_read(SECTOR_NUM, 24, u32);

//  stree_vim_pid.Kp_3=((float)(flash_read(SECTOR_NUM, 48, u32)))/10;
//  stree_vim_pid.Kp_4=((float)(flash_read(SECTOR_NUM, 52, u32)))/10;
//  stree_vim_pid.Kp_5=((float)(flash_read(SECTOR_NUM, 56, u32)))/10;
//  missLine= ((float)(flash_read(SECTOR_NUM, 48, u32)));
//  missLine_M= ((float)(flash_read(SECTOR_NUM, 52, u32)));
}





