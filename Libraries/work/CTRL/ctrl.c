#include "ctrl.h"
#include "FlashUI.h"
#include "pwm.h"

extern PID streepid;       //转向pid
extern PID speedpid;       //速度pid
extern int speed;          //速度
extern PID linepid;        //直线pid


extern int sum;
extern int ad1;
extern int ad2;
extern int ad3;
extern int ad4;
extern int average1,average2;
int ad1_value;              //ad1滤波后的值
int ad2_value;              //ad2滤波后的值
extern float sum_diff;
u8 Lose=0;

int spe;   //丢线标志位 1左，2右
int MEDIAN=4600;     //舵机中值
int ad_all;
//int max1,min1;                //滤波最大最小值
//int max2,min2;                //滤波最大最小值
//int sum1,sum2;                //虑完波的值

int ENCODE;
int resultturn;
int old_resulturn;
//归一前整合*************************  2017-6-1  wjl  //
//声明8个变量存放最大最小值
float   ad1max=600,ad1min=200,ad2max=600,ad2min=200;
float   ad3max=600,ad3min=200,ad4max=600,ad4min=200;
//声明4个存放范围
float  ad1_sum,ad2_sum,ad3_sum,ad4_sum;
//声明4个存放分份
float ad1_mei,ad2_mei,ad3_mei,ad4_mei;
//定义个要分的份数
float use_fenshu=800;
//给最大最小加限制
float ad_shang_max=1000,ad_xia_min=0;
//*****************************************************//

//int result;                //
float error[5];

u8 inclu;   //丢线标志位


void speedCtrl(void)
{
	u32 duty = 0;
	speedPID(ENCODE);
	duty +=result;
	
	if(duty >5000)
	duty = 5000;
	FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3,duty );
	FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2,0);
}

//归一前整合
void guiyi_qian(void)	
{ 
	//ad1最大最小判断
   if(ad1<ad1min)
	 {
	    ad1min=ad1;
		   if(ad1min<ad_xia_min)
				  ad1min=ad_xia_min;
	 }
	 if(ad1>ad1max)
	 {
	    ad1max=ad1;
		   if(ad1max>ad_shang_max)
				  ad1max=ad_shang_max;
	 }
   //ad2最大最小判断 	 
   if(ad2<ad2min)
	 {
	    ad2min=ad2;
		   if(ad2min<ad_xia_min)
				  ad2min=ad_xia_min;
	 }
	 if(ad2>ad2max)
	 {
	    ad2max=ad2;
		   if(ad2max>ad_shang_max)
				  ad2max=ad_shang_max;
	 }
	 //ad3最大最小判断 	 
   if(ad3<ad3min)
	 {
	    ad3min=ad3;
		  // if(ad3min<ad_xia_min)
			//	  ad3min=ad_xia_min;
	 }
	 if(ad3>ad3max)
	 {
	    ad3max=ad3;
		 //  if(ad3max>ad_shang_max)
			//	  ad3max=ad_shang_max;
	 }
	 //ad4最大最小判断 	 
   if(ad4<ad4min)
	 {
	    ad4min=ad4;
		   if(ad4min<ad_xia_min)
				  ad4min=ad_xia_min;
	 }
	 if(ad4>ad4max)
	 {
	    ad4max=ad4;
		   if(ad4max>ad_shang_max)
				  ad4max=ad_shang_max;
	 }
	 //求范围
	 ad1_sum=ad1max-ad1min;
	 ad2_sum=ad2max-ad2min;
	 ad3_sum=ad3max-ad3min;
	 ad4_sum=ad4max-ad4min;
   //分份数      //use_fenshu=800
	 ad1_mei=ad1_sum/use_fenshu;
	 ad2_mei=ad2_sum/use_fenshu;
	 ad3_mei=ad3_sum/use_fenshu;
	 ad4_mei=ad4_sum/use_fenshu;
	 //化完值
	 ad1=ad1*ad1_mei;
	 ad2=ad2*ad2_mei;
	 ad3=ad3*ad3_mei;
	 ad4=ad4*ad4_mei;
	 //变为int型输出
	 ad1=(int)ad1;
	 ad2=(int)ad2;
	 ad3=(int)ad3;
	 ad4=(int)ad4;
	 
}


//速度控制
void speedPID(u32 nowValue)                    
{
	 float  SpeedKP = speedpid.p ;
   float  SpeedKI = speedpid.i ;
   float  SpeedKD = speedpid.d ;

   static int16_t LastSpeedCut0,LastSpeedCut1,LastSpeedCut2 ,SpeedLastPWMK ;
   int16_t  SpeedPWMKP ,SpeedPWMKI ,SpeedPWMKD,SpeedPWMK ;
   int16_t  SpeedPWMOUT;
   int16_t  SpeedDifference0=0;
   int16_t  speedDEARE1,speedDEARE2,DSpeed ;

   LastSpeedCut0 =  nowValue ;                //编码器的值
   DSpeed =(int16_t) speed;                   //速度赋值

   SpeedDifference0 = DSpeed - LastSpeedCut0  ; 
   speedDEARE1 = LastSpeedCut0 - LastSpeedCut1;  
   speedDEARE2 = LastSpeedCut2+LastSpeedCut0-2*LastSpeedCut1;  
   LastSpeedCut2  = LastSpeedCut1;
   LastSpeedCut1  = LastSpeedCut0;

          SpeedPWMKP = SpeedKP*SpeedDifference0;   
          if(SpeedPWMKP>KPPLUSMAX){
           SpeedPWMKP = KPPLUSMAX;
          }else if (SpeedPWMKP <KPNEGATIVEMAX){
          SpeedPWMKP = KPNEGATIVEMAX;
          }
          SpeedPWMKI = SpeedKI* speedDEARE1;
          if(SpeedPWMKI > KIPLUSMAX){
          SpeedPWMKI = KIPLUSMAX;
          } else if(SpeedPWMKI < KINEGATIVEMAX){
          SpeedPWMKI = KINEGATIVEMAX;
          }
          SpeedPWMKD = SpeedKD* speedDEARE2;
          if(SpeedPWMKD > KDPLUSMAX){
             SpeedPWMKD = KDPLUSMAX;
          } else if(SpeedPWMKD < KDNEGATIVEMAX){
            SpeedPWMKD = KDNEGATIVEMAX;
          }
          SpeedPWMK = SpeedPWMKD-SpeedPWMKI+SpeedPWMKP ;
          if(SpeedPWMK > KWPLUSMAX){
          SpeedPWMK = KWPLUSMAX;
          }else if(SpeedPWMK < KWNEGATIVEMAX){
          SpeedPWMK = KWNEGATIVEMAX;
          }
         SpeedPWMOUT = SpeedLastPWMK + SpeedPWMK ;
         if(SpeedPWMOUT < 0 ){
            SpeedPWMOUT = 0 ;
         } else if(SpeedPWMOUT > KOUPLUSMAX){
           SpeedPWMOUT = KOUPLUSMAX ;
         }
         SpeedLastPWMK = SpeedPWMOUT ;
         result = SpeedPWMOUT;	
}




//转向控制

//2017-6-5    转向PID    测数据 拟合后  三次方 

// 
void streePID(int nowValue)
{
//	FTM_PWM_ChangeDuty(HW_FTM1, HW_FTM_CH1, 1660);            //1940  1660  1370
    
	
    float stree_p,stree_i,stree_d;
	  float stree_zhi_p,stree_zhi_i,stree_zhi_d;
    int pid_count = 0,pid_errsum = 0;
		float HIGH_RUN;
		int Kpvalue;
	  int Kdvalue;
    float p1,p2,p3,p4,p5,p6;
	  int x;
	
    stree_p = streepid.p*0.01;
    stree_i = streepid.i*0.01;
    stree_d = streepid.d*0.01;
	
	    p1 =     -0.2492 ;
       p2 =      0.9905 ;
       p3 =       124.8  ;
       p4 =      -40.71 ;

//	     p1 =    0.007119 ;
//       p2 =  -0.2827 ;
//       p3 =    3.415 ;
//       p4 =  -10.65  ;
//       p5 =   40.22 ;
//       p6 =  2.301 ;
	  x =  p1*nowValue*nowValue*nowValue*nowValue +p2*nowValue*nowValue*nowValue+p3*nowValue*nowValue + p4;
	 //coefficient (with 95% confidence bounds)
	     


//	 x =  p1*nowValue*nowValue*nowValue + p2*nowValue*nowValue + p3*nowValue + p4;
//	   nowValue=x;
		error[0] = x;
		Kpvalue =(int) (stree_p * error[0]);
		Kdvalue = stree_d * (error[0] - error[1]);
		resultturn =MEDIAN + Kpvalue + Kdvalue;
		error[1] = error[0];		
  }

	
void linePID(int nowValue)
{
//	FTM_PWM_ChangeDuty(HW_FTM1, HW_FTM_CH1, 1660);            //1940  1660  1370

	
    float stree_p,stree_i,stree_d;
	  float stree_zhi_p,stree_zhi_i,stree_zhi_d;
    int pid_count = 0,pid_errsum = 0;
		float HIGH_RUN;
   
	  
	
    stree_p = linepid.p;
    stree_i = linepid.i;
    stree_d = linepid.d;

//		x = p1*nowValue+p2;
	
	  
	  
	
		error[0] = nowValue;
		pid_errsum +=  error[1] * stree_i;
		resultturn =(int) (stree_p * error[1]);
		resultturn += pid_errsum;
		resultturn += stree_d * (error[1] - error[0]);
//		pid_count ++;
		error[1] = error[0];
		
  }
	
void ctrl(void)
{
	//ad1是右边的
//	int spe;   //丢线标志位 1左，2右
//	int ad;
	int overflow1=0,overflow2=0;     //检测旁边跑到变量
	int sum_ad=0;
	u8 side_flag=0;                    //检测旁边跑道标志位
//	u8 Lose=0;
//		sum=ad4-ad3;
//	resultturn=Turn_speed(sum);
	
if((ad3>=50)&&(ad4>=50))
{
//	  spe=0;
	  streePID(sum_diff);
}

if((ad3>=200)&&(ad4>=200))
{
	spe=0;
}
//	else
//		resultturn=old_resulturn;
//		sum_diff=0;
///******************************丢线*************************/

//  if((ad2>900)&&(ad4>900)&&(ad2>ad4))
//		Lose=1;
//  if((ad2>900)&&(ad4>900)&&(ad2<ad4))
//		Lose=2;
////  if((ad2>900)&&(ad4>900)&&(ad2>ad4))
////		Lose=1;
//	
////	if(Lose==1)
////	{
////		if(ad2<300||ad4<300)
////		{
////			spe=2;          //左边丢线
////		}
////  }
//	
//  if(Lose==2)
//	{
//		if(ad2<300||ad4<300)
//		{
//		//	if(spe==0)
//				 spe=1;          //右边丢线
//		}
//  }
//	
//	if(Lose==1)
//	{
//		if(ad2<300||ad4<300)
//		{
//			spe=2;          //左边丢线
//		}
//  }

//  if((ad2>1800)&&(1800>ad4>1500)&&(ad2>ad4))
//	{
//		Lose=1;
//	}
//  if((ad4>1800)&&(1800>ad2>1500)&&(ad4>ad2))
//	{
//		Lose=2;
//	}

	if(ad3<60)
	{
		if(spe==0)
		spe=1;
	}
		
	if(ad4<60)
	{
		if(spe==0)
		spe=2;
	}
//	if((ad3<50 ) && (ad4<50))
//	{ 
//		 if(spe==0)
//	   spe=3;
//	}
	switch(spe)
	{
		case 1:
			resultturn=MEDIAN-900;
		  break;
		case 2:
			resultturn=MEDIAN+1000;
		  break;
//		case 3:
//			resultturn=old_resulturn;
//		  break;
		default:
			//resultturn=old_resulturn;
			break;
	}
/*************************丢线********************************/
	
	
	//舵机最后PWM输出 
		FTM_PWM_ChangeDuty(HW_FTM1, HW_FTM_CH1, resultturn);    //  -900  4600  +1000  resultturn
	old_resulturn=resultturn;
}

//void filter(void)       //滤波
//{
//	u8 count;
//	int value_buf1[6],value_buf2[6];
//	int i=0;

//	value_buf1[i]=ad1;
//	value_buf2[i]=ad2;
//	
//	if(i==0)
//	{
//		sum1=ad1;
//		sum2=ad2;
//	}
//	i++;
//	
//	if(value_buf1[i]>max1)
//		max1=value_buf1[i];
//	if(value_buf1[i]<min1)
//		min1=value_buf1[i];
//	
//	if(value_buf2[i]>max2)
//		max2=value_buf2[i];
//	if(value_buf2[i]<min2)
//		min2=value_buf2[i];
//	
//	if(i==6)
//	{
//		i=0;
//		sum1=(sum1-min1)/(max1-min1);    //新数据=（原数据-极小值）/（极大值-极小值）   现在是最大最小值
//		sum2=(sum2-min2)/(max2-min2);
//		max1=0;
//		min1=0;
//		max2=0;
//		min2=0;
//	}
//  
//}







///**********************防检测旁边赛道*********************************************/
////	overflow1=ad;            //当前值赋给overflow1
//  
//	
//	if(average2<20)            //左边为0 判断右边
//	{
//		overflow1=average1;
//		if((average1<1200)||((overflow1-overflow2)>10))  //这次减上次大于0
//		{
//			side_flag=1;
//		}
//		else
//		{
//			side_flag=0;
//		}
//		
//		overflow2=overflow1;
//	}
//	
//	if(average1<20)            //右边边为0 判断左边
//	{
//		overflow1=average2;
//		if((average2<1200)&&((overflow2-overflow1)>10))  //这次减上次大于0  向内移动
//		{
//			side_flag=1;
//		}
//		else
//		{
//			side_flag=0;
//		}
//		overflow2=overflow2;
//	}
////	overflow2=overflow1;     //上次值赋给overflow2
///**********************************************************************/
	
	//
	int Turn_speed(int sum_ad)
{
  int pwm_turnD;
  int turn_pwm;
  int turn_error_max=0;//(??)
  int turnErrSave=0;
	int error[4],GoStritOn[5];
  float turn_Kd=streepid.p*0.1;
  float turn_Kp=streepid.d*0.1;
//  turn.Error[0]=turn_e;
  
   turn_error_max=(1.986*(sum_ad/1000)*(sum_ad/1000)*(sum_ad/1000) + 0.2274*(sum_ad/1000)*(sum_ad/1000) + 0.006591*sum_ad + 15.44-15); //16??? 
            error[0]=turn_error_max;
            if(error[0]>650) error[0] = 650;//????
			   if(error[0]<-650) error[0] = -650;//????
            
            turnErrSave=error[0];
            
            GoStritOn[4]= GoStritOn[3];
            GoStritOn[3]= GoStritOn[2];
            GoStritOn[2]= GoStritOn[1];
            GoStritOn[1]= GoStritOn[0];
            GoStritOn[0]= error[0]-GoStritOn[1]>8?GoStritOn[1]+8:error[0]  ;//????  GoStritOn[1]-Turn.Error>5? GoStritOn[1]-5: 
            //8??
            
            error[0]= GoStritOn[0];//(int )(GoStritOn[0] * 0.7 +GoStritOn[1]*0.3);
            turnErrSave=GoStritOn[0];
  
  
  pwm_turnD = (error[0]-error[1])*turn_Kd;  //????d?
  if(pwm_turnD>turn_Kd*8) pwm_turnD = turn_Kd*8;
  else if(pwm_turnD<-turn_Kd*8) pwm_turnD = -turn_Kd*8;
//  TurnP_LINSHI=Normal.Turn_Pnum;
  turn_pwm = (int)((GoStritOn[0]*turn_Kp*0.8+pwm_turnD));//*0.1
  error[2] = error[1];
  error[1] = GoStritOn[0];
  
  return turn_pwm;
}