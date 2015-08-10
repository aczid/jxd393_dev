#include "jxd393.h"
#include "nRF24.h"
#include "stm8s_conf.h"
#include "debug.h"
#include "delay.h"

int main() {
    uint32_t i;
    uint8_t j = 0;
	// Configure pins
    leds_enable();

    nRF24_init();
    nRF24_TXMode();
    // Loop
    do {
        /*log.buffer[j++] = 'A';*/
        /*if(nRF24_TXPacket("AAAA", 4) == nRF24_MASK_MAX_RT){*/
        /**//*if(nRF24_TXPacket("AAAA", 4) == nRF24_MASK_TX_DS){*/
        leds_toggle();
        _delay_ms(1000);
        /*for(i = 0; i < 78000; i++){*/
        /*}*/
    } while(1);
}
