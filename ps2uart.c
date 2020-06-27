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

void main()
{
    P1 = 0xff;
    uart_init();
    puts("Hello, world!\n");
    P1 = 0x00;
    while(1) { }
}

