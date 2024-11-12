#include <stdbool.h>
#include <stm8s.h>
#include "main.h"
#include "milis.h"

void init(void) {
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // Taktování MCU na 16MHz

    GPIO_Init(BTN_PORT, BTN_PIN, GPIO_MODE_IN_FL_NO_IT);

    GPIO_Init(GPIOB, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(GPIOB, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_SLOW);

    init_milis();
}

int main(void) {
    uint8_t akt_stav_tlac = 1;
    uint8_t pred_stav_tlac = 1;
    uint8_t vystup_led = 1;

    init();

    while (1) {
        akt_stav_tlac = GPIO_ReadInputPin(BTN_PORT, BTN_PIN);

    // predchozi stav se nemuze rovnat aktualnimu. Tudiz aby nebyli stejne pricteme +1
        if (pred_stav_tlac == 0 && akt_stav_tlac == 1) {
            vystup_led += 1;
        }

    // zadání hranice stavů (3). Po překročení 3 stavu se bude rovnat(vrátí se) k 1 stavu 
        if (vystup_led > 3) {
            vystup_led = 1;
        }

    // nastavování výstupů LEDek s piny 
        if (vystup_led == 1) {
            GPIO_WriteHigh(GPIOB, GPIO_PIN_2);
            GPIO_WriteLow(GPIOB, GPIO_PIN_4);
        } else if (vystup_led == 2) {
            GPIO_WriteHigh(GPIOB, GPIO_PIN_3);
            GPIO_WriteLow(GPIOB, GPIO_PIN_2);
        } else {
            GPIO_WriteHigh(GPIOB, GPIO_PIN_4);
            GPIO_WriteLow(GPIOB, GPIO_PIN_3);
        }
        pred_stav_tlac = akt_stav_tlac;
    }
}
