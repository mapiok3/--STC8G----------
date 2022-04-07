
#include "reg52.h"

sfr P_SW1=0xA2;	//串口切换地址

typedef unsigned int u16;	//对系统默认数据类型进行重定义
typedef unsigned char u8;
static int time=5000;	//检测间隔时间参数
static char rec_data[64];	//存储中转数组
static int i=0,j=0;	//数组元素位	
static int count=0,countCompare=0;	//记录和比较值
static int bool=1;	//判断接收发送	

void uart_init(u8 baud)
{
	TMOD|=0X20;	//设置计数器工作方式2
	SCON=0X50;	//设置为工作方式1
	PCON=0X80;	//波特率加倍
	TH1=baud;	//计数器初始值设置
	TL1=baud;
	ES=1;		//打开接收中断
	EA=1;		//打开总中断
	TR1=1;		//打开计数器		
}


void main()
{	
	uart_init(0XFA);//波特率为9600
	P_SW1=0x00;
	while(1)
	{	
		countCompare=count;		
		while(time--);
		if(countCompare==count&&count!=0&&bool==1)
		{
			if(P_SW1==0x00){
			  //P_SW1=0x00;
			   P_SW1=0x40; //切换串口
			  //P_SW1=0x80;
			  bool=0;
			  count=0;
			  j=i;
			  i=0;
			  SBUF=rec_data[i];
			  i++; 
			}
			else{
			  P_SW1=0x00;
			  bool=0;
			  count=0;
			  j=i;
			  i=0;
			  SBUF=rec_data[i];
			  i++;
			}
		}
		if(countCompare==count&&count!=0&&bool==0)
		{
			bool=1;
			i=0;
			count=0;
		}							
	}		
}

void uart() interrupt 4 //串口通信中断函数
{
	if(bool==1)
	{
	RI = 0;			//清除接收中断标志位
	rec_data[i]=SBUF;	//存储接收到的数据
	i++;		//切换数组元素
	count++;	  //增加记录数
	}
	else
	{
		if(i<=j)
		{
		TI=0;
		SBUF=rec_data[i];
		i++;
		count++;
		}
	}				
}