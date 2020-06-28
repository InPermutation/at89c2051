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

char decode_scancode(char ch) {
    switch(ch) {
        case 0x1c: return 'A';
        case 0x32: return 'B';
        case 0x21: return 'C';
        case 0x23: return 'D';
        case 0x24: return 'E';
        case 0x2B: return 'F';
        case 0x34: return 'G';
        case 0x33: return 'H';
        case 0x43: return 'I';
        case 0x3B: return 'J';
        case 0x42: return 'K';
        case 0x4B: return 'L';
        case 0x3a: return 'M';
        case 0x31: return 'N';
        case 0x44: return 'O';
        case 0x4d: return 'P';
        case 0x15: return 'Q';
        case 0x2d: return 'R';
        case 0x1b: return 'S';
        case 0x2c: return 'T';
        case 0x3c: return 'U';
        case 0x2a: return 'V';
        case 0x1d: return 'W';
        case 0x22: return 'X';
        case 0x35: return 'Y';
        case 0x1a: return 'Z';
        case 0x45: return '0';
        case 0x16: return '1';
        case 0x1e: return '2';
        case 0x26: return '3';
        case 0x25: return '4';
        case 0x2e: return '5';
        case 0x36: return '6';
        case 0x3d: return '7';
        case 0x3e: return '8';
        case 0x46: return '9';
        case 0x0e: return '`';
        case 0x4e: return '-';
        case 0x55: return '=';
        case 0x5d: return '\\';
        case 0x29: return ' ';
        case 0x0d: return '\t';
        case 0x5a: return '\n';
        case 0x54: return '[';
        case 0x5b: return ']';
        case 0x4c: return ';';
        case 0x52: return '\'';
        case 0x41: return ',';
        case 0x49: return '.';
        case 0x4a: return '/';
    }
    return '!';
}

unsigned char ignore_next_byte = 0;
void ps2_advance_byte(char ch) {
    if (ch == 0xF0) {
        ignore_next_byte = 1;
        return;
    }
    if (ignore_next_byte) {
        ignore_next_byte = 0;
        return;
    }
    putchar(decode_scancode(ch));
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
                ps2_advance_byte(decoded);
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

