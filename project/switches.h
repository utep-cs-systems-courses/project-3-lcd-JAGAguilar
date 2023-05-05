#ifndef switches_included
#define switches_included


#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8
#define SWITCHES 15


static char switch_sense();
void switches_init();
void switch_interrupt_handler();
void __interrupt_vec(PORT2_VECTOR) Port_2();
void __interrupt_vec(WDT_VECTOR) WDT();
#endif
