#include <8052.h>

#define SLEEP_1 1000000

void busy(int cycles) {
    for(int i = 0 ; i < cycles; i++) { }
}

int main() {
    while(1) {
        P1 = 0xFF;
        busy(SLEEP_1);
        P1 = 0x00;
        busy(SLEEP_1);
    }
}
