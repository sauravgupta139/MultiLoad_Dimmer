//------------------------------------------
// TivaWare Header Files
//------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "inc/hw_ints.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include <time.h>
#include <math.h>
//----------------------------------------
// Prototypes
//----------------------------------------
void hardware_init(void);						// init hardware via Xware
void delay(void);


//Global

int count=0,count1=0,count0=0;
uint32_t ui32Period;
uint32_t ui32Period1;
//---------------------------------------------------------------------------
// main()
//---------------------------------------------------------------------------
void main(void)
{
   hardware_init();							// init hardware via Xware
   int flag=0,flag1=1;
   int speed=0;
   int speed1=0;
   int k=1;
   int device_on=0,device1_on=0;
   int timer_activate=0,timer_activate1=0;
   int timerset=0,timerset1=0;
   int finalset=0,finalset1=0;
   int count3=0;
   int flag_timer=1,flag_timer1=1;
   int final_value,final_value1;
   uint32_t ADC1Value[4];
   int time_period=106;
   double del,del1,del2,del3,del4,del5,del6,del7;
   int timer,timer1;
   volatile uint32_t VoltAvg;
   volatile float Volt;
   char a;
   while(1)									// forever loop
   {
	   if (UARTCharsAvail(UART2_BASE)) {
		   a = UARTCharGet(UART2_BASE);

		   switch (a)
		   	{
		   //for first Load
		   	   case '0': speed=0;break;
		   	   case '1': speed=1;break;
		   	   case '2': speed=2;break;
		   	   case '3': speed=3;break;
		   	   case '4': speed=4;break;
		   	   case '5': speed=5;break;
		   	   case '6': speed=6;break;
		   	   case '7': speed=7;break;
		   	   case '8': speed=8;break;
		   //for second load
		   	   case '9': speed1=0;break;
		   	   case 'A': speed1=1;break;
		   	   case 'B': speed1=2;break;
		   	   case 'C': speed1=3;break;
		   	   case 'D': speed1=4;break;
		   	   case 'E': speed1=5;break;
		   	   case 'F': speed1=6;break;
		   	   case 'G': speed1=7;break;
		   	   case 'H': speed1=8;break;

		  // to switch device on and off
		   	   case 'I': device_on =1; break;
		   	   case 'J': device_on = 0; break;
		   	   case 'K': device1_on =1; break;
		   	   case 'L': device1_on =0; break;

		   	//Settable profile
		   	   case 'W': timer_activate=1; break;
		   	   case 'X': timer_activate=0; break;
		   	   case 'Y': timer_activate1=1; break;
		   	   case 'Z': timer_activate1=0; break;

		   	   case 'P': timer=1000; timerset =1; break;
		   	   case 'Q': timer=2000; timerset =1; break;
		   	   case 'R': timer=3000; timerset =1; break;
		   	   case 'S': timer1=1000; timerset1 =1; break;
		   	   case 'T': timer1=2000; timerset1 =1; break;
		   	   case 'U': timer1=3000; timerset1 =1; break;

		   	   case 'a': final_value = 0; finalset=1; break;
		   	   case 'b': final_value = 1; finalset=1; break;
		   	   case 'c': final_value = 2; finalset=1; break;
		   	   case 'd': final_value = 3; finalset=1; break;
		   	   case 'e': final_value = 4; finalset=1; break;
		   	   case 'f': final_value = 5; finalset=1; break;
		   	   case 'g': final_value = 6; finalset=1; break;
		   	   case 'h': final_value = 7; finalset=1; break;
		   	   case 'i': final_value = 8; finalset=1; break;

		   	   case 'j': final_value1 = 0; finalset1=1; break;
		   	   case 'k': final_value1 = 1; finalset1=1;break;
		   	   case 'l': final_value1 = 2; finalset1=1;break;
		   	   case 'm': final_value1 = 3; finalset1=1;break;
		   	   case 'n': final_value1 = 4; finalset1=1;break;
		   	   case 'o': final_value1 = 5; finalset1=1;break;
		   	   case 'p': final_value1 = 6; finalset1=1;break;
		   	   case 'q': final_value1 = 7; finalset1=1;break;
		   	   case 'r': final_value1 = 8;
		   	}
	   }

	   //frequency measurement
	   if(count3>10000){
		   if(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1) && flag1==1){
			   flag1=0;
			   TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period1 -1);
			   IntEnable(INT_TIMER0A);
			   TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
			   TimerEnable(TIMER0_BASE, TIMER_A);
		   	   }
		   else if(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1)==0x00 && flag1==0){
			   flag1=1;
			   TimerDisable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
			   time_period = count0;
			   count0=0;count3=0;
		   }
		   count3=count3+1;

		   ADCIntClear(ADC1_BASE, 2); //clear the interrupt status flag
		   ADCProcessorTrigger(ADC1_BASE, 2); //trigger ADC conversion
		   while(!ADCIntStatus(ADC1_BASE, 2, false)) //wait until conversion is complete
		   			   {}
		   ADCSequenceDataGet(ADC1_BASE, 2, ADC1Value); //copy the data from sampler sequence to buffer in memory
		   VoltAvg = (ADC1Value[0] + ADC1Value[1] + ADC1Value[2] + ADC1Value[3] + 2)/4; //calculate the average
		   Volt = VoltAvg*3.3/4096.0;
		   k=Volt/2.8;
	   }

	   //for frequency testing
	   if(time_period>105)
		   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);//green
	   if(time_period>97)
		   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);//blue
	   else if(time_period>92)
		   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);//red
	   else if(time_period>90)
	   	   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 10);//green+red
	   else if(time_period>80)
	   	   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 6);//red and blue
	   else
		   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 14);//white
	   //--remove this when done

	   //For triggering triac
	   if(speed!= 0 && speed1!=0){
		   if(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1) && flag==0 ){
			   flag =1;
			   if(speed>speed1){
				   del = (time_period/100.0)*(8-speed+0.005)/4.6;
				   del4 = acos((1+cos(del))/k-1);
				   del2 = (time_period/100.0)*(speed-speed1-0.4)/4.6;
				   del6 = acos((1+cos(del2))/k-1);
				   SysCtlDelay(67000*del4); //for variable firing angle
				   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 16);
				   delay(); // fixed triggering width
				   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
				   SysCtlDelay(67000*del6);
				   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 8);
				   delay(); // fixed triggering width
				   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
			   }
			   else if(speed1>speed){
				   del1 = (time_period/100.0)*(8-speed1+0.005)/4.6;
				   del5 = acos((1+cos(del1))/k-1);
				   del3 = (time_period/100.0)*(speed1-speed-0.4)/4.6;
				   del7 = acos((1+cos(del3))/k-1);
				   SysCtlDelay(67000*del5); //for variable firing angle
				   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 8);
				   delay(); // fixed triggering width
				   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
				   SysCtlDelay(67000*del7);
				   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 16);
				   delay(); // fixed triggering width
				   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
			   }
			   else{
				   del = (time_period/100.0)*(8-speed+0.005)/4.6;
				   del4 = acos((1+cos(del))/k-1);
				   SysCtlDelay(67000*del4);
				   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 16);
				   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 8);
				   delay(); // fixed triggering width
				   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
				   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
			   }

		   }
		   else if(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1)==0x00 && flag==1){
			   flag =0;
			   if(speed>speed1){
				    del = (time_period/100.0)*(8-speed+0.005)/4.6;
				    del4 = acos((1+cos(del))/k-1);
				    del2 = (time_period/100.0)*(speed-speed1-0.4)/4.6;
				    del6 = acos((1+cos(del2))/k-1);
				    SysCtlDelay(67000*del4); //for variable firing angle
		   	   	   	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 16);
		   	   	   	delay(); // fixed triggering width
		   	   	   	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
		   	   	   	SysCtlDelay(67000*del6);
		   	   	   	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 8);
		   	   	   	delay(); // fixed triggering width
		   	   	   	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
			   }
			   else if(speed1>speed){
				    del1 = (time_period/100.0)*(8-speed1+0.005)/4.6;
				    del5 = acos((1+cos(del1))/k-1);
				   	del3 = (time_period/100.0)*(speed1-speed-0.4)/4.6;
				   	del7 = acos((1+cos(del3))/k-1);
				    SysCtlDelay(67000*del1); //for variable firing angle
		   	   	   	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 8);
		   	   	   	delay(); // fixed triggering width
		   	   	   	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
		   	   	   	SysCtlDelay(67000*del7);
		   	   	   	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 16);
		   	   	   	delay(); // fixed triggering width
		   	   	   	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
			   }
			   else{
				    del = (time_period/100.0)*(8-speed+0.005)/4.6;
				    del4 = acos((1+cos(del))/k-1);
				   	SysCtlDelay(67000*del4);
				   	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 16);
			   		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 8);
			   		delay(); // fixed triggering width
			   		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
			   		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
			   }

		   }
	   }

	   else if(speed!=0 && speed1==0){
		   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
		   if(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1) && flag==0){
			   flag=1;
			   del = (time_period/100.0)*(8-speed+0.005)/4.4;
			   del4 = acos((1+cos(del))/k-1);
			   SysCtlDelay(67000*del4);
			   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 16);
			   delay(); // fixed triggering width
			   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
		   }

		   else if(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1)==0x00 && flag==1){
			   flag=0;
			   del = (time_period/100.0)*(8-speed+0.005)/4.4;
			   del4 = acos((1+cos(del))/k-1);
			   SysCtlDelay(67000*del4);
			   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 16);
			   delay(); // fixed triggering width
			   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
		   }

	   }

	   else if(speed==0 && speed1!=0){
	   		   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
	   		   if(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1) && flag==0){
	   			   flag=1;
	   			   del1 = (time_period/100.0)*(8-speed1+0.005)/4.4;
	   			   del5 = acos((1+cos(del1))/k-1);
	   			   SysCtlDelay(67000*del5);
	   			   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 8);
	   			   delay(); // fixed triggering width
	   			   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
	   		   }

	   		   else if(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1)==0x00 && flag==1){
	   			   flag=0;
	   			   del1 = (time_period/100.0)*(8-speed1+0.005)/4.4;
	   			   del5 = acos((1+cos(del1))/k-1);
	   			   SysCtlDelay(67000*del5);
	   			   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 8);
	   			   delay(); // fixed triggering width
	   			   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
	   		   }

	   }
	   else{
		   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
		   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
	   }


	   //for settable profile
	   if(timer_activate==1 && flag_timer==1 && finalset==1 && timerset==1 && device_on==1){
		   flag_timer=0;
		   TimerLoadSet(TIMER2_BASE, TIMER_A, ui32Period -1);
		   IntEnable(INT_TIMER2A);
		   TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
		   TimerEnable(TIMER2_BASE, TIMER_A);
	   }
	   else if(timer_activate==0){
		   TimerDisable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
		   flag_timer=1;
	   }

	   if(count > timer && timer_activate ==1 && finalset==1 && timerset==1 && device_on==1){
		   if(speed > final_value)
			   speed = speed-1;
		   else if(speed < final_value)
				speed = speed+1;
		   count = 0;
	   }

	   if(timer_activate1==1 && flag_timer1==1){
		   flag_timer1=0;
		   TimerLoadSet(TIMER1_BASE, TIMER_A, ui32Period -1);
		   IntEnable(INT_TIMER1A);
		   TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
		   TimerEnable(TIMER1_BASE, TIMER_A);
	   }
	   else if(timer_activate1==0){
		   TimerDisable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
		   flag_timer1=1;
	   }

	   if(count1 > timer1 && timer_activate1==1 && finalset1==1 && timerset1==1 && device1_on==1){
		   if(speed1 > final_value1)
		   		speed1 = speed1-1;
		   else if(speed1 < final_value1)
		   		speed1 = speed1+1;
		   count1 = 0;
	   }

   }
}
//---------------------------------------------------------------------------
// hardware_init()
//

//---------------------------------------------------------------------------
void hardware_init(void)
{
	//Set CPU Clock to 40MHz. 400MHz PLL/2 = 200 DIV 5 = 40MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5| SYSCTL_USE_PLL| SYSCTL_OSC_MAIN| SYSCTL_XTAL_16MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

	ADCSequenceConfigure(ADC1_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);//second highest priority
	ADCSequenceStepConfigure(ADC1_BASE, 2, 0, ADC_CTL_CH3);
	ADCSequenceStepConfigure(ADC1_BASE, 2, 1, ADC_CTL_CH3);
	ADCSequenceStepConfigure(ADC1_BASE, 2, 2, ADC_CTL_CH3);
	ADCSequenceStepConfigure(ADC1_BASE,2,3,ADC_CTL_IE|ADC_CTL_END|ADC_CTL_CH3);
	ADCSequenceEnable(ADC1_BASE, 2);

	ui32Period = (SysCtlClockGet() /1000); //1000 Hz or 1ms
	ui32Period1 = (SysCtlClockGet() /10000); //10000 Hz or .1ms
	TimerConfigure(TIMER2_BASE|TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	IntMasterEnable();

	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period1 -1);
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER0_BASE, TIMER_A);

	UARTEnable(UART2_BASE);

	GPIOPinConfigure(GPIO_PD6_U2RX);
	GPIOPinConfigure(GPIO_PD7_U2TX);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);

	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_3| GPIO_PIN_4);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1);

	UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 9600,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// delay()
//
void delay(void)
{
	 SysCtlDelay(67000*0.0055);		// creates delay - TivaWare fxn
}

void Timer2IntHandler(void)
{
	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	count=count+1;
	TimerLoadSet(TIMER2_BASE, TIMER_A, ui32Period -1);
	IntEnable(INT_TIMER2A);
	TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER2_BASE, TIMER_A);
}

void Timer1IntHandler(void)
{
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	count1=count1+1;
	TimerLoadSet(TIMER1_BASE, TIMER_A, ui32Period -1);
	IntEnable(INT_TIMER1A);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER1_BASE, TIMER_A);
}

void Timer0IntHandler(void)
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	count0=count0+1;
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period1 -1);
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER0_BASE, TIMER_A);
}
