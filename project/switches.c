#include <msp430.h>
#include "switches.h"
#include "buzzer.h"
#include "lcdutils.h"
#include "lcddraw.h"

void
switches_init(){
  P2REN |= SWITCHES;
  P2IE |= SWITCHES;
  P2OUT |= SWITCHES;
  P2DIR &= ~SWITCHES;
  switch_sense();
}

void main(){

  configureClocks();
  lcd_init();
  switches_init();
  enableWDTInterrupts();
}

static char
switch_sense()
{
  char p2val = P2IN;
  P2IES |= (p2val & SWITCHES);
  P2IES &= (p2val | ~SWITCHES);
  return p2val;
}

static short siren1 =0;
static short siren2 =0;
static short siren3 =0;
static short pause =0;
void switch_interrupt_handler()
{
  char p2val = switch_sense();
  if(!(SW1 & p2val)){
    pause=0;
    siren2=0;
    siren3=0;
    siren1=1;
  }
  if(!(SW2 & p2val)){
    pause=0;
    siren1=0;
    siren3=0;
    siren2=1;
  }
  if((SW3 & p2val)){
    pause=0;
    siren1=0;
    siren2=0;
    siren3=1;
  }
  if(!(SW4& p2val)){
    siren1=0;
    siren2=0;
    siren3=0;
    pause=1;
  }
}


void
__interrupt_vec(PORT2_VECTOR) Port_2()
{
  if(P2IFG & SWITCHES){
    P2IFG &= ~SWITCHES;
    switch_interrupt_handler();
  }
}
static int lcdState =0;
void
__interrupt_vec(WDT_VECTOR) WDT()
{
  static int sirenState =0;
  if(siren1){
    if (sirenState < 5) {
      buzSet(800);
      sirenState++;
    } else if (sirenState < 10) {
      buzSet(400);
      sirenState++;
    } else {
      sirenState = 0;
    }

    if (lcdState == 0) {
      clearScreen(COLOR_RED);
      lcdState = 1;
    } else {
      clearScreen(COLOR_BLUE);
      lcdState = 0;
    }
  }
}
