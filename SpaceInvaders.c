#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "time.h"
#include "stdlib.h"

void PortF_Init(void);
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Timer2_Init(unsigned long period);
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
unsigned long TimerCounter;
unsigned long Semaphore;


// *************************** Images ***************************
// enemy ship that starts at the top of the screen (arms/mouth closed)
// width=16 x height=10

const unsigned char car[] ={
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xF0,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,

};

const unsigned char enemy[] ={
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,

};



// *************************** Capture image dimensions out of BMP**********

#define carW        ((unsigned char)car[18])
#define carH        ((unsigned char)car[22])
#define enemyW      ((unsigned char)enemy[18])
#define enemyH      ((unsigned char)enemy[22])


// ***************************** Variables ************************ //

int xcoordinate = 0, ycoordinate , zcoordinate = 10;			  //player position(x, y) anh height
int monstery, monsterW ,monsterPOS, monsterPOS1;		//enemies position and movement
int screenWIDTH = 84, screenHEIGHT = 47;						//screen diminssions
int tracks[3] = {28, 11, 45};										//tracks x positions
//int randomYenemy[3] = {0, 21, 30};		        //random y for the first enemy for appearing new enemy
int f1 = 0, f2 = 0, start = 0;									//flags
int score;

int main(void){
	
	// ******************** initializations ***************************** //
	PortF_Init();
	ycoordinate = 28;		//mid screean y = 28
	monsterPOS = 84-enemyW;
	monsterPOS1 = 84-enemyW;
	monstery = rand()%3;
	score = 0;
	
  TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
  Random_Init(1);
  Nokia5110_Init();
  Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();      // draw buffer
	
	// ***************************** Welcome screen *************************** //
	Nokia5110_Clear();
	Nokia5110_SetCursor(3, 0);
  Nokia5110_OutString("Start");
  Nokia5110_SetCursor(1, 2);
  Nokia5110_OutString("Race Car");
  Nokia5110_SetCursor(0, 4);
  Nokia5110_OutString("To Play");
  Nokia5110_SetCursor(1, 5);
  Nokia5110_OutString("Press SW1");
  
	while(1){
		if(((GPIO_PORTF_DATA_R&(1<<4)) == 0))
			break;
  }
	
	// *****************************  delay befor start ************************ //
	Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(xcoordinate, ycoordinate, car, 0);
	Nokia5110_DisplayBuffer();
	Delay100ms(250);
	
	// ***************************** Game loop ************************ //
	while(1){
		Nokia5110_ClearBuffer();		
		
								// ************** car Movement ************** //
		if(!(((GPIO_PORTF_DATA_R&(1<<4)) == 0) && ((GPIO_PORTF_DATA_R&0x1) == 0))){
			if(((GPIO_PORTF_DATA_R&(1<<4)) == 0)){
				if((ycoordinate > 11) && (f1 == 0)){
					ycoordinate -= 17;
					f1 = 1;
				}
			}
			else{
				f1 = 0;
			}

			if(((GPIO_PORTF_DATA_R&0x1) == 0)){
				if((ycoordinate < 45) && (f2 == 0)){
					ycoordinate += 17;
					f2 = 1;
				}
			}
			else{
				f2 = 0;
			}
		}
		
		Nokia5110_PrintBMP(xcoordinate, ycoordinate, car, 0);
		
									//***************** enemy movement ********************//
		if(monsterPOS != 0){
			Nokia5110_PrintBMP(monsterPOS, tracks[monstery], enemy, 0);
			monsterPOS--;
		}
		
		if(monsterPOS == 0){
			
			if(monsterPOS1 == 0){
				score++;					//number of passed enemys
				monstery = rand()%3;
				monsterPOS1 = 84-enemyW;
			}
			Nokia5110_PrintBMP(monsterPOS1, tracks[monstery], enemy, 0);
		}
		
		if(monsterPOS1 <= xcoordinate+carW-2 && tracks[monstery] == ycoordinate){
			break;
		}
		monsterPOS1--;
				
								//***************** Screen displaying ********************//
		Nokia5110_DisplayBuffer();
		Delay100ms(1);
	}
	
	Delay100ms(50);				//Delay befor gameover screen

						 //***************** Gameover screen ********************//
  Nokia5110_Clear();
  Nokia5110_SetCursor(1, 0);
  Nokia5110_OutString("GAME OVER");
  Nokia5110_SetCursor(1, 2);
  Nokia5110_OutString("Nice try");
	Nokia5110_SetCursor(1, 4);
  Nokia5110_OutString("your score is");
	Nokia5110_SetCursor(3, 5);
  Nokia5110_OutUDec(score);
  while(1){
  }

}
//end of main

void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}


// You can use this timer only if you learn how it works
void Timer2_Init(unsigned long period){ 
  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  TimerCounter = 0;
  Semaphore = 0;
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}
void Timer2A_Handler(void){ 
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
  TimerCounter++;
  Semaphore = 1; // trigger
}
void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 727240/80;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
