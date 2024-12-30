//#include<math.h>
#include<reg51.h>
#include<intrins.h>

//#include "stdio.h"
#define N 8
#define TIME 0xfc18
#define MIN 10 // ??
#define POS 1  // ?????
#define IO_LCD P2 // LCD??????
#define CYCLE 100 // ???????


unsigned int now_time;
unsigned int temperature;
unsigned char pos[4] = {0x80, 0x90, 0x88, 0x98};
//unsigned char tempArr[8]={10,20,30,40,50,60,70,80};
unsigned char cnt = 0; 
unsigned char LIGHT = 80; // ???
unsigned char target=0; // ????
unsigned int num = 0; // ????????
unsigned int speed = 0; // ??????
unsigned int temperSet[4]={80,60,40,20};
unsigned int tmp;
unsigned int curTemp;


unsigned char mode=0;
//int graph[8]={0,0,0,0,0,0,0,0};
//int graph[8]={10,75,25,88,15,72,33,42};
unsigned char graph[8]={0,0,0,0,0,0,0,0};
//int graph[8]={80,80,80,80,80,80,80,80};
unsigned char showTpos=7;
int flag=1;
int motorOn=0;
void LCD_busy_check();              // LCD?????
void LCD_write_cmd(unsigned char cmd);      // LCD???
void LCD_write_data(unsigned char dat);     // LCD???
void LCD_set_pos(unsigned char x, unsigned char y); // LCD??DDRAM??
void LCD_dis_str(unsigned char *s);         // LCD?????
//void LCD_dis_pic(unsigned char *s);         // LCD????
void LCD_clr_pic();                 // LCD????RAM
void init_LCD();                    // ???LCD
void init_timer0();                 // ?????
void timer0();                      // ???0??,??
void int0();                        // ????,??
void display();                     // ?????
void LCD_clr_pic();
float absN(float x){
	
	return x>=0?x:-x;
}


sbit RS = P1^ 1;
sbit RW = P1^ 2;
sbit E = P1^ 3;
sbit DRV = P1^ 0;
sbit RST = P1 ^ 4;

//sbit D6 = P0 ^ 6;
//sbit D7 = P0 ^ 7;
//sbit Bus=P0^3;//?????
sbit 		DQ = P0^3;			
#define 	DQ_H	DQ = 1;
#define 	DQ_L	DQ = 0;

//函数原型声明，可以提出为单个头文件
void reset_ds18b20(void);
void write_byte(unsigned char dat);
unsigned char read_byte(void);
unsigned char getTemper();
//void Delay10us(void);//10us????
//void Delay600us(void);//600us?????  
//void Delay(unsigned int n);//LCD1602??????
//void Delay1ms(unsigned int t);//t???????
 
//void Init_Ds(void);//DS18B20???
//void Write_Ds(unsigned char com);//?DS18B20?????
//unsigned char Read_Ds(void);//?DS18B20?????
//unsigned int Get_Tem(void);//?????
 
void LCD_busy_check(); // LCD?????
void LCD_write_cmd(unsigned char cmd); // LCD???
void LCD_write_data(unsigned char dat);  // LCD???
void LCD_set_pos(unsigned char x, unsigned char y); // LCD??DDRAM??
void LCD_dis_str(unsigned char *s);
void LCD_dis_pic();
void delay_10us(unsigned char n);           // ??,10us?????
void delay(unsigned int t); 
void delayNus(unsigned int t){
	int i;
	for(i=t;i>0;i--){
		
		_nop_();_nop_();
		
	}
	
	
}
void delayNms(unsigned int t){
	int i;
	for(i=t*1000;i>0;i--){
		
		_nop_();
		
	}
	
}


code const unsigned char up_arrow[32][16]={
	
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7B,0xDE,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF3,0xCF,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xE3,0xC7,0x80,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC3,0xC3,0xC0,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	
};

void main()
 
{
		unsigned char i;
		
		//getTemper();;
		
		reset_ds18b20();
		init_LCD();//关反白，游标右移动,8bit接口,文字清屏
    LCD_clr_pic();
    init_timer0();
    getTemper();
		//printf("\n------开始-------------\n");
		
		while(1){
			
			
			
			temperature=temperSet[P0&0x03];
			
		
				
				//if(curTemp<=0)curTemp=0;
				//else if(curTemp>99)curTemp=99;
				//curTemp=getTemper();
				if(flag)
				{
					
					curTemp=getTemper();
					graph[7]=curTemp;
						
					if(temperature>=curTemp)motorOn=0;
					else motorOn=1;
					
								//LCD_clr_pic();
								//LCD_write_cmd(0x01);
					if((P1>>7)==1){
									
							//if(!isDraw){
								LCD_write_cmd(0x01);
							//	isDraw=1;
								delay(50);
						
							//	LCD_set_pos(2, 0); // ????
		
							
							//LCD_clr_pic();
							LCD_clr_pic();					
							LCD_dis_pic();
							
							//delayNms(10);
							
						}
					else{
								//LCD_write_cmd(0x01);
							//isDraw=0;
							//delay(200);
								LCD_clr_pic();
							//delay(50);
							display();
								//delayNms(10);
						
						}
				//printf("curTemp=%d,temperature=%d\n",curTemp,temperature);
				
					flag=0;
				
				//LCD_write_cmd(0x01);
				//EA=0;
				//delay_10us(10);
				
					for(i=0;i<7;i++){
						
							graph[i]=graph[i+1];
					}
				
				//LCD_write_cmd(0x01);
				//EA=1;
			}
				
		}
	
}



unsigned char getTemper(){
	unsigned int ret;
	float tp;
	unsigned char tempL, tempH;
	EA=0;
    reset_ds18b20();     // 复位
    write_byte(0xcc);    // 跳过ROM
    write_byte(0x44);    // 温度转换
		//delay(5);
    reset_ds18b20();     // 复位
    write_byte(0xcc);    // 跳过ROM
    write_byte(0xbe);    // 读温度命令
    tempL = read_byte(); // 读取温度
    tempH = read_byte();
    ret = (tempH << 8) | tempL;
		tp=ret*0.0625;
		ret=tp;
		if(tp-ret>=0.5)ret+=1;
	EA=1;
		return ret;
	
	
}


void display()
{
    unsigned char high, low;
    EA=0;

	
		
	
		LCD_set_pos(0, 0); // ????
		//LCD_dis_str(" T4 T5 T6 T7");
		if(motorOn){
				LCD_dis_str("警告超过阈值 T");
			
			LCD_set_pos(1, 0); // ????
			LCD_dis_str("占空比:");
			high =  (100-LIGHT)/ 10;
			low =(100-LIGHT) % 10;
			LCD_write_data('0'+high);
			LCD_write_data('0'+low);
		}else{
				LCD_dis_str("当前低于阈值T");
				LCD_set_pos(1, 0);
				LCD_dis_str("风扇状态: 停止");
		}
		
		
		high =  curTemp/ 10;
    low = curTemp % 10;
		
		LCD_set_pos(3, 0); // ????
		LCD_dis_str("T now:");
    LCD_set_pos(3, 3); // ????
    LCD_write_data(high + '0');
    LCD_write_data(low + '0');
		LCD_write_data(' ');
		LCD_write_data(' ');
    high = speed / 10;
    low = speed % 10;
		LCD_set_pos(3, 5); // ????
		LCD_dis_str("V:");
    LCD_set_pos(3, 6); // ??????
    LCD_write_data(high + '0');
    LCD_write_data(low + '0');
		LCD_set_pos(2, 0); // ????
		LCD_dis_str("设定温度T:");
    LCD_set_pos(2, 5);
    high =  temperature/ 10;
    low = temperature % 10;
    LCD_write_data(high + '0');
    LCD_write_data(low + '0');
		EA=1;
}

void init_timer0()
{
    EA = 1;           // ????
    TR0 = 1;          // ????
    ET0 = 1;          // ??????
    EX0 = 1;
		//EX1=1;// ??????
    IT0 = 1;          // ????????
    TMOD = 0X01;      // ???0??1
    TH0 = TIME / 256; // ?????
    TL0 = TIME % 256;
}

void int0() interrupt 0
{
    num++; // ????????????
}
/*void int1() interrupt 2
{
		 unsigned char i;
		 i=tempArr[((P1&0xE0)>>5)];
		 reset_ds18b20();     // 复位
		 write_byte(0xCC);    // 跳过ROM
		 write_byte(0x4E);		//写命令
		 write_byte((i>>4)&0x03);	 
		 write_byte((i<<4)&0xF0);
		 
		
		 write_byte(0x7F);
}*/
void timer0() interrupt 1
{
  	int i;
		TH0 = (TIME) / 256;
    TL0 = (TIME) % 256;
	
		if(motorOn){
			
				if (cnt < CYCLE) {
        DRV = 0;
			} else {
        DRV = 1; // ?????????,??1
        cnt = cnt % (CYCLE );
			}
			cnt = cnt + LIGHT;
		}else{
			LIGHT = 80;
			cnt=0;
			DRV=0;
		}
    
    
    if (now_time == 1000) { // 1s?????,?????
				//curTemp=getTemper();
        
				
				now_time = 0;
        //pre_speed = speed;
        speed = num*60/360;
        num = 0;
				target=1+(40*(curTemp-temperature)/curTemp)%40;
        if (speed > target) { // ????,?????(???????,?????)
            if (LIGHT&&motorOn) {
                LIGHT--;
                cnt = 0;
            }
        } else if (speed < target) { // ?????,?????(???????,?????)
            if (LIGHT<100&&motorOn) {
                LIGHT++;
                cnt = 0;
            }
        }
				flag=1;
				
    } else {
        now_time++;
    }
}

void LCD_dis_pic()
{
	
		
    unsigned char i, j, k, m, y, byte1 = 0, byte2 = 0, bitPos, n;
    float add;
    int row, beforeRow, lowBound;

    // 清除屏幕或者清除绘图区域
    
	//	EA=0;

						
				
	
	
	
    // 启用图形模式
    LCD_write_cmd(0x34);
		//delay_10us(10);
    for (i = 7; i >= 1 && graph[i] != 0; i--) {
        y = 31 - graph[i] / 4;  // 计算 y 坐标
        beforeRow = y;

        if (i != 7) {
            // 计算增量 add，用于平滑折线
            add = (float)(j - y) / 16.0;

            k = 0;
            while (k < 16) {
                lowBound = (int)(add * k);
                bitPos = k;

                if (absN(add * k - lowBound) > 0.5)
                    row = (int)absN((int)(add * k + y) - beforeRow) + 1;
                else
                    row = (int)absN((int)(add * k + y) - beforeRow);

                if (row == 0) {
                    bitPos++;
                    row = (int)absN((int)(add * bitPos + y) - beforeRow);

                    while (row == 0 && bitPos < 16) {
                        bitPos++;
                        row = (int)absN((int)(add * bitPos + y) - beforeRow);
                    }

                    // 设置显示位置，避免越界
                    LCD_write_cmd(0x80 + beforeRow);
										//delay_10us(10);
                    LCD_write_cmd(0x80 + i);
										//delay_10us(10);
                    if (bitPos <= 8) {
                        for (n = k; n < bitPos; n++) {
                            byte1 |= (1 << (7 - n));
                        }
                        LCD_write_data(byte1);
												//delay_10us(10);
                    } else if (k >= 8) {
                        for (n = k; n < bitPos; n++) {
                            byte2 |= (1 << (7 - n % 8));
                        }
                        LCD_write_data(0x00);  // 清除前半部分
                        //delay_10us(100);
												LCD_write_data(byte2);
												//delay_10us(100);
                    } else {
                        for (n = k; n < 8; n++) {
                            byte1 |= (1 << (7 - n));
                        }
                        for (n = 8; n < bitPos; n++) {
                            byte2 |= (1 << (7 - n % 8));
                        }
                        LCD_write_data(byte1);
												//delay_10us(100);
                        LCD_write_data(byte2);
												//delay_10us(100);
                    }

                    byte1 = 0;
                    byte2 = 0;
                    k = bitPos;
										
                }else {
                    for (m = 1; m <= row; m++) {
                        if (add > 0 &&beforeRow + m<=j) {
													LCD_write_cmd(0x80 + (beforeRow + m));
														//delay_10us(10);
                            LCD_write_cmd(0x80 + i);
														//delay_10us(10);
                            if (k >= 8) LCD_write_data(0x00);
                            LCD_write_data(1 << (7 - k % 8));
														//delay_10us(10);
                        } else if (add < 0&&beforeRow -m>=j ) {
                            LCD_write_cmd(0x80 + (beforeRow -m));
														//delay_10us(10);
                            LCD_write_cmd(0x80 + i);
														//delay_10us(10);
                            if (k >= 8) LCD_write_data(0x00);
                            LCD_write_data(1 << (7 - k % 8));
														//delay_10us(10);
												}
												
                    }
                    beforeRow = (add > 0) ? beforeRow + row : beforeRow - row;
                    k++;
                }
								 
            }
        }
        j = y;
    }

    // 更新 graph 数组
		if(graph[6]!=0&&graph[7]>graph[6]){
			
				
				
				
				
				
				for (i = 0; i < 32; i++) {
        LCD_write_cmd(0x80 + i);
        LCD_write_cmd(0x88);
        for (j = 0; j < 16; j++) {
            LCD_write_data(up_arrow[i][j]);
        }
			}
			
		}
		else if(graph[6]!=0&&graph[7]<graph[6]){
			

			
				for (i = 0; i < 32; i++) {
        LCD_write_cmd(0x80 + i);
        LCD_write_cmd(0x88);
        for (j = 0; j < 16; j++) {
            LCD_write_data(up_arrow[31-i][j]);
        }
			}
				
		
			
			
		}else{
				
			for (i = 8; i < 24; i++) {
        LCD_write_cmd(0x80 + i);
        LCD_write_cmd(0x88);
        for (j = 0; j < 16; j++) {
            LCD_write_data(0xAA);
        }
			}
		}

    // 结束图形模式
    LCD_write_cmd(0x36);
		//delay_10us(100);
    LCD_write_cmd(0x30);  // 恢复默认模式
		//delay_10us(100);
		//EA=1;
}


void LCD_clr_pic()
{
		
    unsigned char i, j;
    EA=0;
		LCD_write_cmd(0x34); // ????,?????????,????????
    // LCD_write_cmd(0x36);
	
		
	
    for (i = 0; i < 32; i++) {
        LCD_write_cmd(0x80 + i);
        LCD_write_cmd(0x80);
        for (j = 0; j < 16; j++) {
            LCD_write_data(0x00);
        }
    }
		for (i = 0; i < 32; i++) {
        LCD_write_cmd(0x80 + i);
        LCD_write_cmd(0x88);
        for (j = 0; j < 16; j++) {
            LCD_write_data(0x00);
        }
    }

    LCD_write_cmd(0x36);
    LCD_write_cmd(0x30);
		EA=1;
}

void LCD_busy_check()
{
    unsigned char tmp;
    IO_LCD = 0xff;
    RS = 0; // ???????
    RW = 1;
    while (1) {
        E = 1;
        tmp = IO_LCD;
        E = 0;
        if ((tmp & 0x80) == 0) { // ??????
            break;
        }
    }
}

void LCD_write_cmd(unsigned char cmd)
{
    LCD_busy_check(); // ???
    RS = 0;           // ???????
    RW = 0;
    IO_LCD = cmd;
		//delayNus(5);
    E = 1;
		//delayNus(5);

     E = 0;
		//_nop_(); _nop_();_nop_(); _nop_();	_nop_(); _nop_();_nop_(); _nop_();

    //delayNus(100);
}

void LCD_write_data(unsigned char dat)
{
    LCD_busy_check();
    RS = 1; // ???????
    RW = 0;
    IO_LCD = dat;
		//delayNus(5);


    E = 1;
		//delayNus(5);
    E = 0;
		//delayNus(100);
}

void LCD_set_pos(unsigned char x, unsigned char y)
{
    x = pos[x];
    LCD_write_cmd(x + y); // ????????
}

void LCD_dis_str(unsigned char *s)
{
    while (*s) {
        LCD_write_data(*s++); // ??????????
    }
}

void init_LCD()
{
    // LCD_write_cmd(0x01);
    RST = 0; // LCD????
    delay(5);
    RST = 1;
    LCD_write_cmd(0x30);
    LCD_write_cmd(0x0c);
    LCD_write_cmd(0x06);
    LCD_write_cmd(0x01);
}



void delay_10us(unsigned char n)
{
    unsigned char i;
    for (i = n; i > 0; --i) {
        _nop_();
        _nop_(); //12MHz
    }
}
// delay t ms
void delay(unsigned int t)
{
    while (t--) {
        delay_10us(100);
    }
}









/**********************************************************************************************************
函数名称：DS18B20初始化子程序
输入参数：无
输出参数：无
函数返回：无
**********************************************************************************************************/

void reset_ds18b20(void){
  	DQ_H;          		//  DQ先置高
	delay_10us(10);     //  延时
  	DQ_L;             	//  发送复位脉冲
	delay_10us(60);     //  延时（480us - 960us)
  	DQ_H;          		//  拉高数据线
	delay_10us(24);     //  DS18B20会产生应答脉冲，持续60-240us
}


/**********************************************************************************************************
函数名称：写一字节数据到DS18B20
输入参数：数据
输出参数：无
函数返回：无
**********************************************************************************************************/
void write_byte(unsigned char dat){ 
	unsigned char i=0; 		
	for (i = 8; i > 0; i--){
		//为避免判断的时间加入到时序中，将判断放到外面
		if(dat & 0x01)
        {
			DQ_L;
			_nop_(); _nop_(); //延时 2us
            DQ_H;   //释放总线
			delay_10us(6);
        }
        else
        {
			DQ_L;
			delay_10us(6);
            DQ_H;
			_nop_(); _nop_(); //延时 2us
        }
		dat>>=1;
	}
	delay_10us(3);
}
/**********************************************************************************************************
函数名称：从ds18b20读一字节数据函数
输入参数：无
输出参数：无
函数返回：读出的字节
**********************************************************************************************************/

unsigned char read_byte(void)  			
{
	unsigned char i = 0; 		
	unsigned char dat = 0;
	bit b;
    //先读低位再读高位
    for (i = 8; i > 0; i--)  		
	{
		DQ_L;
		_nop_(); _nop_(); //延时 2us
		DQ_H;  //释放总线
		dat >>= 1;  //必须在15us内读数据, 用这个表达式的时间占位
		b= DQ;

    	if (DQ){
            dat|=0x80;
        }

		delay_10us(6);
	}
    return(dat);
}

