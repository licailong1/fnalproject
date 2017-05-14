#include"LPC11xx.h"
#define COM1_ON LPC_GPIO2->DATA &=~(1<<10)
#define COM2_ON LPC_GPIO2->DATA &=~(1<<11)
#define ALL_OFF LPC_GPIO2->DATA |=(1<<10)+(1<<11)
uint8_t table[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uint8_t DispBuffer[2];
uint8_t data[2];


void DisplayScan()
{
	static uint16_t com;
	com++;
	if(com>=2)
		com=0; 
	ALL_OFF ;
	switch(com)
	{
		
		case 0:
			  LPC_GPIO2->DATA &=~0x0FF;
		    LPC_GPIO2->DATA |=DispBuffer [0];
			  COM2_ON ;
		    break ;
		case 1:
			  LPC_GPIO2->DATA &=~0x0FF;
		    LPC_GPIO2->DATA |=DispBuffer [1];
			  COM1_ON ;
		    break ;
	}
}

void ToDisplayBuffer(void)
{
	uint8_t i,j=0;
	for(i=0;i<1;i++)
	{
	DispBuffer[j++]=table[data[i]%10];
	DispBuffer[j++]=table[data[i]/10];
	}
}
void PIOINT3_IRQHandler()
{
	
	if((LPC_GPIO3->MIS &(1<<0))==(1<<0))
	{
		data[0]++;
		ToDisplayBuffer();
			if(data[0]>=30)
		{
			data[0]=30;
		  ToDisplayBuffer ();
		}
		LPC_GPIO3->IC=(1<<0);
	}
	if((LPC_GPIO3->MIS&(1<<1))==(1<<1))
	{
		data[0]--;
		ToDisplayBuffer ();
		ToDisplayBuffer ();
		if(data[0]<=15)
		{
			data[0]=15;
			ToDisplayBuffer ();
		}	
		LPC_GPIO3->IC =(1<<1);
	}

}


void SEG_IOInit()
{
	LPC_GPIO2->DIR |=0xFFF;
	LPC_GPIO3->DIR &=~(1<<0);
	LPC_GPIO3->DIR &=~(1<<1);
}

int main()
{
	SEG_IOInit ();
	LPC_GPIO3->IE |=(1<<0);
	LPC_GPIO3->IE |=(1<<1);
	LPC_GPIO3->IS &=~(1<<0);
	LPC_GPIO3->IS &=~(1<<1);
	LPC_GPIO3->IEV &=~(1<<0);
	LPC_GPIO3->IEV %=~(1<<1);
	NVIC_EnableIRQ(EINT3_IRQn);
	data[0]=15;
	ToDisplayBuffer();
  while(1)
	{
	DisplayScan();
	}
	
}
