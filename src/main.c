#include "jxd393.h"
#include "nRF24.h"
#include <stdint.h>

int main() {
    uint32_t i;
	// Configure pins
    leds_enable();
    nRF24_init();
    nRF24_TXMode();
	// Loop
	do {
        /*nRF24_TXPacket("AAAA", 4);*/
        /*if(nRF24_TXPacket("AAAA", 4) == nRF24_MASK_MAX_RT){*/
        if(nRF24_TXPacket("AAAA", 4) == nRF24_MASK_TX_DS){
            leds_toggle();
        }
        for(i = 0; i <  160000; i++){
        }
	} while(1);
}
