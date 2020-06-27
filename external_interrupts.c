#include <8051.h>

void isr_ie0() __interrupt (IE0_VECTOR) {
    P1_6=!P1_6;
}
void isr_ie1() __interrupt (IE1_VECTOR) {
    P1_7=!P1_7;
}

void main()
{
    P1 = 0x00;

    EX0 = 1;
    EA = 1;
    EX1 = 1;
    IT0 = 1; // set external interrupt 0 to falling-edge trigger
    IT1 = 0; // set external interrupt 1 to level trigger


    while(1) {}
}

