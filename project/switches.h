#ifndef switches_included
#define switches_included



static char switch_sense();
void switches_init();
void switch_interrupt_handler();
void __interrupt_vec(PORT2_VECTOR) Port_2();
void __interrupt_vec(WDT_VECTOR) WDT();
#endif
