#include "jxd393.h"
#include "nRF24.h"
#include "stm8s_conf.h"

int main() {
    uint32_t i;
    uint8_t j = 0;
	// Configure pins
    leds_enable();
    /*leds_on();*/

    nRF24_init();
    nRF24_TXMode();
    // Loop
    do {
        /*nRF24_TXPacket("AAAA", 4);*/
        /*if(nRF24_TXPacket("AAAA", 4) == nRF24_MASK_MAX_RT){*/
        if(nRF24_TXPacket("AAAA", 4) == nRF24_MASK_TX_DS){
            /*debug[j++] = 'A';*/
            leds_toggle();
            /*__asm__("nop");*/
            for(i = 0; i < 78000; i++){
            }
        }
        /*leds_off();*/
        /*for(i = 0; i <  160000; i++){*/
        /*}*/
    } while(1);
}
