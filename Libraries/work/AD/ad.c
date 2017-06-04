#include "ad.h"
#include "common.h"

int max1,min1;
int max2,min2;
int sum_ad1,sum_ad2;   //归一化后的值
int ad1;
int ad2;
int ad3;
int ad4;

int ad2_one=0,ad4_one=0;
float ad2_lv[10]={0,0,0,0,0,0,0,0,0,0};
float ad4_lv[10]={0,0,0,0,0,0,0,0,0,0};
int average1;
int average2;  //滤波后的值
//int sum1=0;
//int sum0=0;
int lv=0;   

char i=0;

int value_buf1[12]={0,0,0,0,0,0,0,0,0,0,0,0};
int value_buf2[12]={0,0,0,0,0,0,0,0,0,0,0,0};
int ad1_max;
int ad3_max;
int ad1_min=900;
int ad3_min=900;

float ad2_max;
float ad4_max;
float ad2_min=900;
float ad4_min=900;

float guiyi_ad1 = 0;
float guiyi_ad3 = 0;

//int ad3;
//int ad4;

void Init_AD(uint32_t instance)
{
	/* 初始化ADC模块 ADC0_SE19_BM0 */
    ADC_InitTypeDef ADC_InitStruct1;
    ADC_InitStruct1.instance = instance;
    ADC_InitStruct1.clockDiv = kADC_ClockDiv2; /* ADC采样时钟2分频 */
    ADC_InitStruct1.resolutionMode = kADC_SingleDiff12or13;
    ADC_InitStruct1.triggerMode = kADC_TriggerSoftware; /* 软件触发转换 */
    ADC_InitStruct1.singleOrDiffMode = kADC_Single; /*单端模式 */
    ADC_InitStruct1.continueMode = kADC_ContinueConversionEnable; /* 启动连续转换 转换一次后 自动开始下一次转换*/
    ADC_InitStruct1.hardwareAveMode = kADC_HardwareAverageDisable; /*禁止 硬件平均 功能 */
    ADC_InitStruct1.vref = kADC_VoltageVREF;                       /* 使用外部VERFH VREFL 作为模拟电压参考 */
    ADC_Init(&ADC_InitStruct1);
    
    /* 初始化对应引脚 */
    /* DM0引脚为专门的模拟引脚 ADC时 无需设置复用  DM0也无法当做普通的数字引脚 */
    
    /* 启动一次ADC转换 */
//    ADC_StartConversion(HW_ADC0, 13, kADC_MuxA);     //PB3
//	  ADC_StartConversion(HW_ADC1, 10, kADC_MuxA);     //PB4
//	  ADC_StartConversion(HW_ADC1, 11, kADC_MuxA);     //PB5
//	  ADC_StartConversion(HW_ADC1, 12, kADC_MuxA);     //PB6
}

//启动一次ad转换
int adc_once(uint32_t instance, uint32_t chl)
{
	ADC_StartConversion(instance, chl, kADC_MuxA);
  return ADC_ReadValue(instance, kADC_MuxA);
}

//int max_main(void)
//{
//	max
//}	


#define N 3
void filter(void)       //滤波
{  
	  ad();
}
//*********************************************************************************//
//void ad(void)  ad采集函数
//ad 采集出来的值   现在只用到了ad2  和 ad4的值    2017-5-31
//将采集出来的值做平均处理，使其不发生跳变，但是过多次数平均会降低反应时速度，思路整合

//入口参数：无
//返回参数：无

 //说明：

//我将数据做5次平均 进行测试    结果没有影响可用

//*********************************************************************************//
int avg_ad1[5],avg_ad2[5],avg_ad3[5],avg_ad4[5];
void ad(void)
{
	
	int d=0;
  float ad2_buf=0,ad4_buf=0;	
	float ad2_yuan=0,ad4_yuan=0;
	
		ADC_StartConversion(HW_ADC1, 4, kADC_MuxA);
	avg_ad1[0]=ADC_QuickReadValue(ADC1_SE4_PE0);
////	ad1=2000;
	ADC_StartConversion(HW_ADC1, 5, kADC_MuxA);
	avg_ad2[0]=ADC_QuickReadValue(ADC1_SE5_PE1);
	
	ADC_StartConversion(HW_ADC1, 6, kADC_MuxA);
	avg_ad3[0]=ADC_QuickReadValue(ADC1_SE6_PE2);
	
	ADC_StartConversion(HW_ADC1, 7, kADC_MuxA);
	 //原来减50  不知道为什么
	//ad4=ADC_QuickReadValue(ADC1_SE7_PE3)-50;
	avg_ad4[0]=ADC_QuickReadValue(ADC1_SE7_PE3);
	
	
	avg_ad1[4]=avg_ad1[3];
	avg_ad1[3]=avg_ad1[2];
	avg_ad1[2]=avg_ad1[1];
	avg_ad1[1]=avg_ad1[0];
	ad1=(avg_ad1[4]+avg_ad1[3]+avg_ad1[2]+avg_ad1[1]+avg_ad1[0])/5;
	      
	avg_ad2[4]=avg_ad2[3];
	avg_ad2[3]=avg_ad2[2];
	avg_ad2[2]=avg_ad2[1];
	avg_ad2[1]=avg_ad2[0];
	ad2=(avg_ad2[4]+avg_ad2[3]+avg_ad2[2]+avg_ad2[1]+avg_ad2[0])/5;
	
	avg_ad3[4]=avg_ad3[3];
	avg_ad3[3]=avg_ad3[2];
	avg_ad3[2]=avg_ad3[1];
	avg_ad3[1]=avg_ad3[0];
	ad3=(avg_ad3[4]+avg_ad3[3]+avg_ad3[2]+avg_ad3[1]+avg_ad3[0])/5;
	
	avg_ad4[4]=avg_ad4[3];
	avg_ad4[3]=avg_ad4[2];
	avg_ad4[2]=avg_ad4[1];
	avg_ad4[1]=avg_ad4[0];
	ad4=(avg_ad4[4]+avg_ad4[3]+avg_ad4[2]+avg_ad4[1]+avg_ad4[0])/5;
	
/******************滤波1**********************************/	
//	if((ad2-ad2_one>=500)||(ad2-ad2_one<=-500))
//		ad2=ad2_one;
//	else
//		ad2_one=ad2;
//	if((ad4-ad4_one>=500)||(ad4-ad4_one<=-500))
//		ad4=ad4_one;
//	else
//		ad4_one=ad4;

/*****************滤波2***********************************/

//    ad2_lv[lv]=ad2_yuan;
//	  ad4_lv[lv]=ad4_yuan;
//    lv++;
//  if(lv==4)
//	{
//		ad2_one=ad2_lv[0];
//		ad4_one=ad4_lv[0];
//		for(d=0;d<=lv;d++)
//		{
//			if(ad2_one<=ad2_lv[d])
//				ad2_one=ad2_lv[d];
//			
//			if(ad4_one<=ad4_lv[d])
//				ad4_one=ad4_lv[d];
//		}
//		
//		for(d=0;d<=(lv-1);d++)
//		{
//			ad2_buf=ad2_lv[d];
//			ad4_buf=ad4_lv[d];
//		}
//	  ad2=(int)((ad2_buf-ad2_one)/(lv-1));
//		ad4=(int)((ad4_buf-ad4_one)/(lv-1));
//		lv=0;
//	}
		

//	ad1=adc_once(HW_ADC1, 4);
//	ad2=adc_once(HW_ADC1, 13);
}

void guiyi(void)               //归一化
{
//	u8 count;
	float value_buf1,value_buf3;
	int i=0;
	int sum;
  float sum1,sum3,sum4;
	
//	value_buf1[i]=ad1;
//	value_buf2[i]=ad2;
	  //2017-6-1   归一化思想  
	  //让占有比重 来决定打脚 或者辅助 判断  
	  // 
	//   sum=ad3+ad4;
	//   sum3=ad3/sum;
	//   sum4=ad4/sum;
	//  // 
	
  guiyi_ad3 = ((float)ad4/((float)ad4_max - (float)ad4_min))*800;
	guiyi_ad1 = ((float)ad3/((float)ad3_max - (float)ad3_min))*800;
	
//	if(i<6)
//	{
//		i++;
//		value_buf1+=sum1;
//		value_buf3+=sum3;
//	}
//  if(i==6)
//	{
//  guiyi_ad1=(float)(value_buf1/6);
//	guiyi_ad3=(float)(value_buf3/6);
//		value_buf1=0;
//		value_buf3=0;
//		i=0;
//	}
}

void max_min(void)
{
	if(ad3_max<=ad3)
	   ad3_max=ad3;
	
	if(ad4_max<=ad4)
	   ad4_max=ad4;
	
	if(ad3_min>=ad3)
	   ad3_min=ad3;
	
	if(ad4_min>=ad4)
	   ad4_min=ad4;
	
	
}