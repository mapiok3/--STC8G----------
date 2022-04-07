
#include "reg52.h"

sfr P_SW1=0xA2;	//�����л���ַ

typedef unsigned int u16;	//��ϵͳĬ���������ͽ����ض���
typedef unsigned char u8;
static int time=5000;	//�����ʱ�����
static char rec_data[64];	//�洢��ת����
static int i=0,j=0;	//����Ԫ��λ	
static int count=0,countCompare=0;	//��¼�ͱȽ�ֵ
static int bool=1;	//�жϽ��շ���	

void uart_init(u8 baud)
{
	TMOD|=0X20;	//���ü�����������ʽ2
	SCON=0X50;	//����Ϊ������ʽ1
	PCON=0X80;	//�����ʼӱ�
	TH1=baud;	//��������ʼֵ����
	TL1=baud;
	ES=1;		//�򿪽����ж�
	EA=1;		//�����ж�
	TR1=1;		//�򿪼�����		
}


void main()
{	
	uart_init(0XFA);//������Ϊ9600
	P_SW1=0x00;
	while(1)
	{	
		countCompare=count;		
		while(time--);
		if(countCompare==count&&count!=0&&bool==1)
		{
			if(P_SW1==0x00){
			  //P_SW1=0x00;
			   P_SW1=0x40; //�л�����
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

void uart() interrupt 4 //����ͨ���жϺ���
{
	if(bool==1)
	{
	RI = 0;			//��������жϱ�־λ
	rec_data[i]=SBUF;	//�洢���յ�������
	i++;		//�л�����Ԫ��
	count++;	  //���Ӽ�¼��
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