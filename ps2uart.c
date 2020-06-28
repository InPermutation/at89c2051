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


volatile unsigned char circular_ps2[0x20];
volatile int ps2_write = 0;
int ps2_read = 0;

int decode_status = 0;
char decoded = 0;

void ps2_advance_reader() {
    while(ps2_read != ps2_write) {
        unsigned char n = circular_ps2[ps2_read];
        ps2_read = (ps2_read + 1) & 0x1F;

        switch(decode_status) {
            case 0:
                // start bit
                decoded = 0;
                decode_status++;
                break;
            case 9:
                // parity bit
                decode_status++;
                break;
            case 10:
                // stop bit
                decode_status = 0;
                puthex(decoded);
                break;
            default:
                decoded = decoded >> 1;
                if (n) {
                    decoded = decoded | 0x80;
                }
                decode_status++;
                break;
        }
    }
}

void ps2_advance_writer() __interrupt (IE0_VECTOR) {
    circular_ps2[ps2_write] = P3_3;
    ps2_write = (ps2_write + 1) & 0x1F;
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
        ps2_advance_reader();
    }
}

