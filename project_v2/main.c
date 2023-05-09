#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include <stdlib.h>
#include <stdio.h>

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15

void buzSet(short cycles)
{
  CCR0 = cycles;
  CCR1 = cycles >>1;
}


void buzzer_init(){
  timerAUpmode();
  P2SEL2 &= ~(BIT6 | BIT7);
  P2SEL &= ~BIT7;
  P2SEL |= BIT6;
  P2DIR = BIT6;
}

static char 
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  P2IES |= (p2val & SWITCHES);	
  P2IES &= (p2val | ~SWITCHES);	
  return p2val;
}

void 
switch_init()			
{  
  P2REN |= SWITCHES;		
  P2IE |= SWITCHES;		
  P2OUT |= SWITCHES;		
  P2DIR &= ~SWITCHES;		
  switch_update_interrupt_sense();
}

int switches = 0;
short siren1 =0;
short siren2 =0;
short siren3 =0;
void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
  if (switches & SW3){
    siren1=1;
    siren2=0;
    siren3=0;
  }
  if (switches & SW2){
    siren2=1;
    siren1=0;
    siren3=0;
  }
  if (switches & SW1){
    siren3=1;
    siren1=0;
    siren2=0;
  }
}
short sirenPlaying =1;
void wdt_c_handler()
{
  static int secCount = 0;
  if(siren1){
    secCount ++;
    if (secCount <= 5) {
      buzSet(900);
      clearScreen(COLOR_BLUE);
    }
    else if(secCount<=10){
      buzSet(600);
      clearScreen(COLOR_RED);
    }   
    else
      secCount = 0;
    if (switches & SW4) return;
    sirenPlaying =1;
  }
  if(siren2){
    secCount ++;
    if (secCount <= 5) {
      buzSet(1700);
      clearScreen(COLOR_GREEN);
    }
    else if(secCount<=10){
      buzSet(3800);
      clearScreen(COLOR_ORANGE);
    }   
    else
      secCount = 0;
    if (switches & SW4) return;
    sirenPlaying =1;
  }
  if(siren3){
    secCount ++;
    if (secCount <= 5) {
      buzSet(400);
      clearScreen(COLOR_MEDIUM_PURPLE);
    }
    else if(secCount<=10){
      buzSet(1200);
      clearScreen(COLOR_VIOLET);
    }   
    else
      secCount = 0;
    if (switches & SW4) return;
    sirenPlaying =1;
  }
}

  
void main()
{
  buzzer_init();
  configureClocks();
  lcd_init();
  switch_init();
  
  enableWDTInterrupts();
  or_sr(0x8);	        
  
  while (1) {		
    if (sirenPlaying) {
      sirenPlaying = 0;
    }
    or_sr(0x10);	
  }
}


void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {
    P2IFG &= ~SWITCHES;	
    switch_interrupt_handler();	
  }
}

void
__interrupt_vec(WDT_VECTOR) WDT(){
  wdt_c_handler();
}
