/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

const int BTN_PIN_B = 16;
const int BTN_PIN_G = 21;
const int LED_B = 18;
const int LED_G = 19; 
const int BUZZER = 15;
volatile int blue_flag = 0;
volatile int green_flag = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    if (gpio == BTN_PIN_B) {
      blue_flag = 1;
    }
    else if (gpio == BTN_PIN_G) {
      green_flag = 1;
    }
  }
}

void play(int freq, int tempo, int pino) {
    int periodo = 1000000/freq;
    for (int i = 0; i < (tempo*freq/1000); i++) {
        gpio_put(pino, 1);
        sleep_us(periodo/2);
        gpio_put(pino, 0);
        sleep_us(periodo/2);
    }
}

int main() {
  int azul = 0;
  int play_once = 0;
  stdio_init_all();

  gpio_init(BUZZER);
  gpio_set_dir(BUZZER, GPIO_OUT);

  gpio_init(LED_B);
  gpio_set_dir(LED_B, GPIO_OUT);

  gpio_init(LED_G);
  gpio_set_dir(LED_G, GPIO_OUT);

  gpio_init(BTN_PIN_B);
  gpio_set_dir(BTN_PIN_B, GPIO_IN);
  gpio_pull_up(BTN_PIN_B);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);

  // callback led r (first)
  gpio_set_irq_enabled_with_callback(BTN_PIN_B, GPIO_IRQ_EDGE_FALL, true,
                                      &btn_callback);

  // callback led g (nao usar _with_callback)
  gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);

  while (true) {
    if (play_once == 0)
    {
      gpio_put(LED_B, 1);
      sleep_ms(500);
      gpio_put(LED_B, 0);
      sleep_ms(500);
      azul = 1;
      play_once = 1;
    }
    
    if (blue_flag)
    {
        sleep_ms(1);
        play(440, 900, BUZZER);       
    }

    if (azul && blue_flag) 
    {
      gpio_put(LED_G, 1);
      sleep_ms(500);
      gpio_put(LED_G, 0);
      blue_flag = 0;
      play_once = 0;
    }
    
    if (green_flag)
    {
        green_flag = 0;
        sleep_ms(1);
        play(330, 300, BUZZER);          
    }
  }
}
