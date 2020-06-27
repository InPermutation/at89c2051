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

void assert(__bit must_be_true, const char* error_msg) {
    if(!must_be_true) {
        P1 = 0xFF;
        puts(error_msg);
        P1 = 0x00;
        while(1) { };
    }
}

__bit ps2_next_bit() {
    __bit rval = 0;
    // wait for falling edge
    while(P3_2) { }

    // sample
    rval = P3_3;

    // wait for rising edge
    while(!P3_2) { }

    return rval;
}

int ps2_next_byte()
{
    __bit b;
    unsigned char i, p;
    int ps2_buf = 0;

    // 1 start bit, must be 0
    do { } while(ps2_next_bit());
    // 8 data bits, lsb first
    for(i = 0, p = 1 ; i < 8 ; i++, p = p << 1) {
        if (ps2_next_bit()) {
            ps2_buf = ps2_buf | p;
        }
    }
    // parity bit (currently ignored)
    b = ps2_next_bit();
    // stop bit
    assert(!ps2_next_bit(), "stop bit should be set\n");

    return ps2_buf;
}

void ps2_debug(int tmp)
{
    puthex((char) ((tmp >> 8) & 0xff));
    puthex((char) (tmp & 0xff));
    putchar('\n');
}

void main()
{
    int v;
    P1 = 0xff;
    uart_init();
    puts("PS2 to UART\n");

    while(1) {
        P1 = 0x80;
        v = ps2_next_byte();
        P1 = 0x40;
        ps2_debug(v);
    }
}

