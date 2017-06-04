#include "ui.h"
#include "FlashUI.h"
#include "pwm.h"
#include "app.h"
#include "ctrl.h"

static u8 Arrow = 3;//¼ýÍ·ËùÔÚÎ»ÖÃ

extern u8 key;

//int speedswitch;
extern VIM_PID stree_zhi_pid;
extern PID streepid;       //×ªÏòpid
extern PID speedpid;       //ËÙ¶Èpid
extern PID linepid;        //Ö±Ïßpid
extern int speed;          //ËÙ¶È
extern int ad1;
extern int ad2;
extern int ad3;
extern int ad4;
//extern int adc1;
//extern int adc2;
extern float guiyi_ad3;
extern float guiyi_ad1;

extern int ad_all;
extern float sum_diff;
extern int ENCODE;


extern int sum_ad1,sum_ad2;
extern int average1;
extern int average2;   //ÂË²¨ºóµÄÖµ
//extern int ad1_value;              //ad1ÂË²¨ºóµÄÖµ
//extern int ad2_value;              //ad2ÂË²¨ºóµÄÖµ
extern int sum1,sum2;                //ÂË²¨ºóµÄÖµ

int result;      //ËÙ¶È
// º¯ÊýÖ¸ÕëÉè¼Æ//
u8 (*FunctionPointer[11])(void)={ 
								  Menu_UI,//Ö÷½çÃæ(µØÖ·Îª0)
								  Run_UI,//ÆðÅÜ½çÃæ(µØÖ·Îª1)
								  State_UI,//³µÌå×´Ì¬¹Û²ì½ç1(µØÖ·Îª2)
								  Set_UI,//²ÎÊýÉèÖÃ½çÃæ(µØÖ·Îª3)
								  Record_UI,//·½°¸Ñ¡Ôñ½çÃæ(µØÖ·Îª4)
								  Read_UI,//·½°¸Ñ¡Ôñ½çÃæ(µØÖ·Îª5)
								};


//²Ëµ¥Ê×Ò³

u8 Menu_UI()
{

  
  u8 S0[]="  Menu neusoft Juju";
	u8 S1[]="---------------------";
	u8 S3[]="      Run";
	u8 S4[]="      Car State";
	u8 S5[]="      Set Variable";
	u8 S6[]="      Plan Record";
	u8 S7[]="      Read";
	u8 AR[]="->";//ÏÔÊ¾¼ýÍ·
	u8 AC[]="  ";//Çå³ý¼ýÍ·
	
	u8 key = 0;
    u8 err;

  OLED_CLS();//ÇåÆÁ
	OLED_P6x8Str(0,0,S0);
	OLED_P6x8Str(0,1,S1);
	OLED_P6x8Str(0,3,S3);
	OLED_P6x8Str(0,4,S4);
	OLED_P6x8Str(0,5,S5);
	OLED_P6x8Str(0,6,S6);
	OLED_P6x8Str(0,7,S7);
  OLED_P6x8Str(23,Arrow,AR);

//  LCD_Write_Number(95,0,0);
	u32 Dev;
    while(1)
    {
			key=(u8)OSMboxPend(msg_key,200,&err);      //µÈ´ý°´¼üÓÊÏä
			switch(key)
			{
				case 1:
					OLED_P6x8Str(23,Arrow,AC);
								Arrow--;
								if(Arrow < 3) Arrow=7;
								OLED_P6x8Str(23,Arrow,AR);
					
					break;
						case 2:
					OLED_P6x8Str(23,Arrow,AC);
								Arrow++;
								if(Arrow > 7) Arrow=3;
								OLED_P6x8Str(23,Arrow,AR);
					 
					break;
				case 3:
					switch(Arrow)
						{
							case 3: return Run_UI_ID;          // ½øÈëÆðÅÜÒ³Ãæ
							case 4: return State_UI_ID;        // ½øÈë³µÌå×´Ì¬¹Û²ìÒ³Ãæ
							case 5: return Set_UI_ID;          // ½øÈë²ÎÊýÉèÖÃÒ³Ãæ
							case 6: return Record_UI_ID;       // ½øÈë·½°¸Ñ¡ÔñÒ³Ãæ
							case 7: return Read_UI_ID;         // ½øÈë·½°¸Ñ¡ÔñÒ³Ãæ
						}
						
						break;
				case 4:
					 
						return Menu_UI_ID;
						break;
							default:
						break;
			}
    }
}


u8 Run_UI()
{
	
  u8 S0[]="Launching  Bett:";
	u8 S1[]="---------------------";
	u8 SR[]="RUNING";
	u8 SS[]=" STOP ";
	
  char  ch;
	u8 key = 0;
  u8 err;
	
  OLED_CLS();//ÇåÆÁ
	OLED_P6x8Str(0,0,S0);
	OLED_P6x8Str(0,1,S1);
  OLED_P6x8Str(0,2,SS);
  LCD_Write_Number(95,0, 0);
	
	
	 while(1)
    {
//			speedPID(ENCODE);
//			FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 0);
//			
//	    FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2,result);
		 if(key ==3)
		 {
			speedPID(ENCODE);
			speedswitch=1;
			OLED_P6x8Str(0,2,SR);
//			FLAG_RUN = 1;      // Í¨¹ý±êÖ¾Î»À´¿ØÖÆÖÐ¶Ïº¯ÊýµÄÖ´ÐÐÓë·ñ
		 }

		 if(key == 4)
		 {
			 speedswitch=0;
			 speedPID(ENCODE);
			 FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3,0);
	     FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2,0);
				return Menu_UI_ID;//·µ»ØÖ÷½çÃæ
			 
		 }
		 key=(u8)OSMboxPend(msg_key,200,&err);//°´¼üÓÊÏä
	}

}


#define SetOptionsNum 8 //ÉèÖÃÏî¹öÂÖ´óÐ
u8 State_UI()
{
  static u8 SetArrow = 2;
	static u8 Page = 0;
	static u8 Item = 1;
	u8 key = 0;
  u8 err;
  u8 i,n;
	
  u8 S0[]  =  "Reading:";
	u8 S1[]  =  "---------------------";

	float dnum[9];
	
  u8 AR[]="->";//ÏÔÊ¾¼ýÍ·
	u8 AC[]="  ";//Çå³ý¼ýÍ·
	
//  SS[0]=(char)ad;
	
	OLED_CLS();//ÇåÆÁ
  OLED_P6x8Str(0,0,S0);
  OLED_P6x8Str(0,1,S1);
  
	u8 SaveSetArrow = 2;
	u16 ArrowState = 0;//¼ýÍ·×´Ì¬±ê¼Ç0:Õý³£Ñ¡ÔñÏî 1:ÊýÖµÐÞ¸Ä
  u8 text[5];
	while(1)
	{
		dnum[0]=ENCODE;
	  if(key == 4)
		 {
			 FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3,0);
	     FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2,0);
		  	return Menu_UI_ID;//·µ»ØÖ÷½çÃæ
		 }
		 key=(u8)OSMboxPend(msg_key,5,&err);//°´¼üÓÊÏä
		 
		 max_min();
//		 FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 2000);
//	   FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2,0);
//		 FTM_PWM_ChangeDuty(HW_FTM1, HW_FTM_CH1, 1700);
//		LCD_Write_Number(95,2, ad1);
//		LCD_Write_Number(95,3, dnum[0]);
//		 key=(u8)OSMboxPend(msg_key,200,&err);//°´¼üÓÊÏä
}
}

u8 Record_UI()
{
    u8 S0[]  =  "         Plan:";
    u8 S1[]  =  "---------------------";
		u8 S2[]  =  "    Plan_1 = ";
    u8 S3[]  =  "    Plan_2 = ";
    u8 S4[]  =  "    Plan_3 = ";
    u8 S5[]  =  "    Plan_4 = ";
    u8 S6[]  =  "    Plan_5 = ";
    u8 S7[]  =  "    Plan_6 = ";

    OLED_CLS();
    OLED_P6x8Str(0,0,S0);
    OLED_P6x8Str(0,1,S1);
    OLED_P6x8Str(0,2,S2);
    OLED_P6x8Str(0,3,S3);
    OLED_P6x8Str(0,4,S4);
    OLED_P6x8Str(0,5,S5);
    OLED_P6x8Str(0,6,S6);
    OLED_P6x8Str(0,7,S7);


	return 0;
		
}

#define SetOptionsNum 14 //ÉèÖÃÏî¹öÂÖ´óÐ¡
u8	Read_UI()              
{
	static u8 SetArrow = 2;
	static u8 Page = 0;
	static u8 Item = 1;
	u8 key = 0;
  u8 err;
  u8 i,n;
	
  u8 S0[]  =  "Reading:";
	u8 S1[]  =  "---------------------";
	u8 SS[SetOptionsNum][19]={
		"      Left = ",
    "      Right = ",
	  "      Enco = ",     //Encoder
	  "      l_cha = ",    //left_cha
	  "      D_RIGHT = ",
	  "      D_LEFT = ",
		"      LMAX = ",
		"      LMIN = ",
		"      RMAX = ",
		"      RMIN = ",
		"      DLMAX = ",
		"      DLMIN = ",
		"      DRMAX = ",
		"      DRMIN = ",
	};

	
	
	int dnum[14];
	
  u8 AR[]="->";//ÏÔÊ¾¼ýÍ·
	u8 AC[]="  ";//Çå³ý¼ýÍ·
	
	OLED_CLS();//ÇåÆÁ
  OLED_P6x8Str(0,0,S0);
  OLED_P6x8Str(0,1,S1);
	
//  LCD_Write_Number(95,0, 0);
    
	u8 SaveSetArrow = 2;
	u16 ArrowState = 0;//¼ýÍ·×´Ì¬±ê¼Ç0:Õý³£Ñ¡ÔñÏî 1:ÊýÖµÐÞ¸Ä
  u8 text[5];
	while(1)
	{
		if(key == 4)
		 {
		  	return Menu_UI_ID;//·µ»ØÖ÷½çÃæ
		 }
		 key=(u8)OSMboxPend(msg_key,5,&err);//°´¼üÓÊÏä
		
		dnum[0]=ad1;
		dnum[1]=ad3;
    dnum[2]=ad2;            //guiyi_ad1
		dnum[3]=ad4;            //guiyi_ad3
    dnum[4]=ad_all;
		dnum[5]= sum_diff;
		for(i=2;i<8;i++)
		 {
			LCD_Write_Number(70,i, dnum[Page+i-2]);
			 
		 }
		 OLED_P6x8Str(10,2,"ad1");
		 OLED_P6x8Str(10,3,"ad3");
		 OLED_P6x8Str(10,4,"ad2");
		 OLED_P6x8Str(10,5,"ad4");
	}
		
}
		
		
#define SetOptionsNum 7 //ÉèÖÃÏî¹öÂÖ´óÐ¡
u8 Set_UI()
{
   
  int a,b;
  u8 S0[]  =  "Set Variable Bett:";
	u8 S1[]  =  "---------------------";
	u8 SS[SetOptionsNum][19]={
		"      Speed_P:    ",
		"      Spedd_I:    ",
		"      Spedd_D:    ",
		"      Turn__P:    ",
    "      Turn__I:    ",
		"      Turn__D:    ",
		
//		"      zhi_p       ",                       //Ö±µ½pid
//		"      zhi_i       ",
//		"      zhi_d       ",
		
		"      Speed:    ",
//    "      LGOAL:    ",
//		"      RGOAL:    ",
//		"      vim_p3_:    ",
//		"      vim_p4_:    ",
//		"      vim_p5_:    ",
//		"      missline    ",
//		"      missline_m  ",
                                    };

    static u8 SetArrow = 2;//¼ýÍ·±ê¼ÇÎ»(2~6)
	static u8 Page = 0;//Ò³Ãæ±ê¼ÇÎ»(0~6)
	static u8 Item = 1;//ÏîÄ¿±ê¼ÇÎ»(1~SetOptionsNum)

   u8 AR[]="->";//ÏÔÊ¾¼ýÍ·
	u8 AM[]="- ";//Ñ¡ÖÐÄ£Ê½
	u8 AC[]="  ";//Çå³ý¼ýÍ·
    u8 SaveSetArrow = 2;
	u16 ArrowState = 0;//¼ýÍ·×´Ì¬±ê¼Ç0:Õý³£Ñ¡ÔñÏî 1:ÊýÖµÐÞ¸Ä
    u8 text[5];
    u8 i;

    u8 key = 0;
    u8 err;
									
    OLED_CLS();
	OLED_P6x8Str(0,0,S0);
	OLED_P6x8Str(0,1,S1);
    LCD_Write_Number(95,0, 0);
    while(1)
	{

		
		switch(key)
		{
			case 0:break;

			case 1://ÉÏ¼ü

                
				if(ArrowState== 0)
				{
					SetArrow--;
					Item--;
					if(Item < 1) Item = SetOptionsNum;
					if(SetArrow<2)
					{
						if(Page == 0)
						{
//							Page = 11;
//							SetArrow = 10;
							Page = 1;
							SetArrow = 7;
						}
						else
						{
							SetArrow = 2;
                        	Page--;
						}
					}
				}
				break;

			case 2://ÏÂ¼ü

              
				if(ArrowState== 0)
				{
					SetArrow++;
					Item++;
					if(Item > SetOptionsNum) Item = 1;
					if(SetArrow>7)
//						if(SetArrow>10)
					{
						if(Page == 1)
//						if(Page == 11)
						{
							Page = 0;
							SetArrow = 2;
						}
						else
						{
							SetArrow = 7;
//							SetArrow = 10;
                       	 	Page++;
						}
					}
				}
				break;

              case 3://È·ÈÏ
               
				if(ArrowState == 0) ArrowState = 1;
				else ArrowState = 0;
				break;

              case 4://ÍË³ö

				//ÌîÐ´±£´æµ½flashµÄ´úÂë
                Falsh_Reserve();
				//ÌîÐ´±£´æµ½flashµÄ´úÂë

				if(ArrowState == 1) ArrowState = 0;//¼ýÍ·×´Ì¬ÐÞ¸Ä
				else
				{
					return Menu_UI_ID;
				}
				break;
        }
         //*****ÊýÖµÐÞ¸Ä
		if(ArrowState)
		{

          
			switch(Item)
			{
				case 1:
					if(key == 1) speedpid.p += 0.1;
					if(key == 2) speedpid.p -= 0.1;
					if(speedpid.p > 99.9) speedpid.p = 0.0;
                    
					break;

				case 2:
					if(key == 1) speedpid.i += 0.1;
					if(key == 2) speedpid.i -= 0.1;
					if(speedpid.i > 99.9) speedpid.i = 0.0;
                 
					break;

        case 3:
					if(key == 1) speedpid.d += 0.1;
					if(key == 2) speedpid.d -= 0.1;
					if(speedpid.d > 99.9) speedpid.d = 0.0;
                  
					break;

                case 4:
					if(key == 1) streepid.p += 0.1;
					if(key == 2) streepid.p -= 0.1;
//					if(streepid.p > 99.9) streepid.p = 0.0;
                  
					break;

                case 5:
					if(key == 1) streepid.i += 0.1;
					if(key == 2) streepid.i -= 0.1;
//					if(streepid.i > 99.9) streepid.i = 0.0;
                 
					break;

                case 6:
					if(key == 1) streepid.d += 0.1;
					if(key == 2) streepid.d -= 0.1;
//					if(streepid.d > 99.9) streepid.d = 0.0;
                  
					break;
								
								case 7:
					if(key == 1) speed +=1;
					if(key == 2) speed -= 1;
					if(speed> 999) speed = 0;
                  
					break;

//					 case 7:
//					if(key == 1) linepid.p += 0.1;
//					if(key == 2) linepid.p -= 0.1;
//					if(linepid.p > 99.9) linepid.p = 0.0;
//                  
//					break;

//                case 8:
//					if(key == 1) linepid.i += 0.1;
//					if(key == 2) linepid.i -= 0.1;
//					if(linepid.i > 99.9) linepid.i = 0.0;
//                 
//					break;

//           case 9:
//					if(key == 1) linepid.d += 0.1;
//					if(key == 2) linepid.d -= 0.1;
//					if(linepid.d > 99.9) linepid.d = 0.0;
//                  
//					break;
//								
//                case 10:
//					if(key == 1) speed +=1;
//					if(key == 2) speed -= 1;
//					if(speed> 999) speed = 0;
//                  
//					break;

//                 case 11:
//					if(key == 1)
//                      a +=1;
//					if(key == 2)
//                      a -= 1;
//					if(a> 99.9) a = 0.0;
//                  
//					break;

//                 case 12:
//					if(key == 1)
//                      b +=1;
//					if(key == 2)
//                      b -= 1;
//					if(b> 99.9) b = 0.0;
//                   
//					break;

//                 case 13:
//					if(key == 1)
//                      stree_vim_pid.Kp_3 +=0.1;
//					if(key == 2)
//                      stree_vim_pid.Kp_3 -= 0.1;
//					if(stree_vim_pid.Kp_3> 99.9) stree_vim_pid.Kp_3 = 0.0;
//                 
//					break;

//                 case 14:
//					if(key == 1)
//                      stree_vim_pid.Kp_4 +=0.1;
//					if(key == 2)
//                      stree_vim_pid.Kp_4 -= 0.1;
//					if(stree_vim_pid.Kp_4> 99.9) stree_vim_pid.Kp_4 = 0.0;
//                  
//					break;

//                 case 15:
//					if(key == 1)
//                      stree_vim_pid.Kp_5 +=0.1;
//					if(key == 2)
//                      stree_vim_pid.Kp_5 -= 0.1;
//					if(stree_vim_pid.Kp_5> 99.9) stree_vim_pid.Kp_5 = 0.0;
//                  
//					break;

                default:
					ArrowState = 0;//Î´Éè¶¨µÄ±äÁ¿²»¸øÓè½øÈë×´Ì¬
					break;
            }
        }
        //ÊýÖµÐÞ¸Ä

        //ÊýÖµÇ¶Èë
    sprintf((char *)text,"%4d",(int)(speedpid.p*10));
		AddNumString(SS[0],text);
		sprintf((char *)text,"%4d",(int)(speedpid.i*10));
		AddNumString(SS[1],text);
		sprintf((char *)text,"%4d",(int)(speedpid.d*10));
		AddNumString(SS[2],text);
		sprintf((char *)text,"%4d",(int)(streepid.p*10));
		AddNumString(SS[3],text);
		sprintf((char *)text,"%4d",(int)(streepid.i*10));
		AddNumString(SS[4],text);
		sprintf((char *)text,"%4d",(int)(streepid.d*10));
		AddNumString(SS[5],text);
				
//		sprintf((char *)text,"%4d",(int)(linepid.p*10));
//		AddNumString(SS[6],text);
//		sprintf((char *)text,"%4d",(int)(linepid.i*10));
//		AddNumString(SS[7],text);
//		sprintf((char *)text,"%4d",(int)(linepid.d*10));				
//		AddNumString(SS[8],text);
    		
//		sprintf((char *)text,"%4d",(int)(speed));
//		AddNumString(SS[9],text);

      sprintf((char *)text,"%4d",(int)(speed));
  		AddNumString(SS[6],text);

//    sprintf((char *)text,"%4d",(int)(stree_vim_pid.Kp_1));
//		AddNumString(SS[10],text);
//    sprintf((char *)text,"%4d",(int)(stree_vim_pid.Kp_2));
//	 	AddNumString(SS[11],text);
//    sprintf((char *)text,"%4d",(int)(stree_vim_pid.Kp_3*10));
//		AddNumString(SS[12],text);
//    sprintf((char *)text,"%4d",(int)(stree_vim_pid.Kp_4*10));
//		AddNumString(SS[13],text);
//    sprintf((char *)text,"%4d",(int)(stree_vim_pid.Kp_5*10));
//		AddNumString(SS[14],text);
        //*****ÊýÖµÇ¶Èë

		for(i=2;i<8;i++)
		{
			OLED_P6x8Str(0,i,SS[Page+i-2]);//´òÓ¡ÏÔÊ¾
		}

		if(ArrowState == 0)//¼ýÍ·ÎªÐÞ¸ÄÊýÖµ×´Ì¬
		{
			OLED_P6x8Str(24,SaveSetArrow,AC);
			OLED_P6x8Str(24,SetArrow,AR);
			SaveSetArrow = SetArrow;
		}
		else//ÐÞ¸Ä¼ýÍ·ÎªÊýÖµÑ¡Ôñ×´Ì¬
		{
			OLED_P6x8Str(24,SaveSetArrow,AM);
		}
		
		key=(u8)OSMboxPend(msg_key,0,&err);//°´¼üÓÊÏä
    }
}

void AddNumString(u8 ss[],u8 n[])//Êý×Ö¼ÓÈëÏÔÊ¾×Ö·û´®
{
	u8 i;
	for(i=0;i<4;i++)
	{
		ss[14+i] = n [i];
	}
	ss[18] = '\0';
}




