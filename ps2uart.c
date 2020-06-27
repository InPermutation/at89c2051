#include <8051.h>

void uart_init()
{
    TMOD = 0x20;
    // TH1 = 256 - ((f/384)/Baud)
    //     = 256 - ((7372800/384)/19200)
    //     = 256 - (19200/19200)
    //     = 256 - 1
    //     = 255
    //     = 0xFF
    TH1 = 0xFF;
    SCON = 0x50;
    TR1 = 1;
}

void putchar(char ch)
{
    SBUF = ch;
    while (TI == 0) { }
    TI = 0;
}

void puts(const char *s)
{
    while(*s) {
        putchar(*s);
        ++s;
    }
}

const char* HEXLUT = "0123456789ABCDEF";

void puthex(char b) {
    putchar(HEXLUT[ (b & 0xf0) >> 4]);
    putchar(HEXLUT[ (b & 0x0f)]);
}

void isr_ie0() __interrupt (IE0_VECTOR) {
    P1=0x70;
}
void main()
{
    P1 = 0x00;

    uart_init();
    puts("PS2 to UART\n");

    EA = 1; // enable interrupts
    EX0 = 1; // enable external interrupt 0
    IT0 = 1; // set external interrupt 0 to falling-edge trigger

    while(1) {
    }
}

