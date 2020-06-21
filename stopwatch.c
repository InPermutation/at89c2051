#include <8051.h>

// I have a soldering kit Amazon calls the
// WHDTS 4-Digit Digital Clock Kits with PCB
// for Soldering Practice Learning Electronics with English Instructions
// ( https://www.amazon.com/gp/product/B0711MHKDZ/ )
// It has "E EQKIT" silkscreened on the front of the circuit board.
// The back has "DIY YSZ-4 860925" in copper.
//
// The clock comes with a preprogrammed STC11E02E.
// This chip is programmable with stcgal, but you can't read the program
// out of it. I don't want to lose the program.
// Fortunately, this chip is 8051-compatible, and there are
// many many many other chips that support the same pinout and
// instruction set. I'm using an AT89C2051, because they're
// cheap and abundant.
//
// There is a 12MHz crystal and capacitors on the board.
// I'm using a 7.3728MHz oscillator can, since that's what I have handy.
// This will need to be taken into account.
//
// The LED display is multiplexed. Each digit has a common anode.
// The segments of the LEDs are set via Port 1:
//
//
//       1.7
//    1       1
//    .       .
//    6       5
//       1.1
//    1       1
//    .       .
//    4       2
//       1.3
//
// The decimals do not appear to be wired.
// 1.0 controls the flashing colon that separates hours from minutes.
//
// Port 3 is used to select the anode to ground:
//
// 3.0  3.1  :  3.2  3.7
//
// Set one of these pins to 0 at a time, and wiggle the segment bits
// to show numbers.
//
// The two switches will ground 3.4 and 3.5.
// The buzzer is enabled when 3.3 is low.


// handy P3 constants:
// 84218421 (bin->hex lut)
// 76543210 (P3 index)
//
// 11111110
#define D0 0xFE
// 11111101
#define D1 0xFD
// 11111011
#define D2 0xFB
// 01111111
#define D3 0x7F
// 11111111
#define DNONE 0xFF

// handy P1 constants:
// 84218421 (bin->hex lut)
// 76543210 (P1 index)
// AFBEDCG: (segment lut)
// 11111100
#define ZERO 0xFC
// 00100100
#define ONE 0x24
// 10111010
#define TWO 0xBA
// 10101110
#define THREE 0xAE

void digit(unsigned char p1, unsigned char p3)
{
    P3 = p3;
    P1 = p1;
    for(int i=0; i<0x00ff; i++){}
    P3 = DNONE;
}

int main() {
    while(1)
    {
        digit(ZERO, D0);
        digit(ONE, D1);
        digit(TWO, D2);
        digit(THREE, D3);
    }
}
