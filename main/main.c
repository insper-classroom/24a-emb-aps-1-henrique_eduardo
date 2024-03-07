/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <time.h>
#include <stdlib.h>

const int BTN_PIN_B = 16;
const int BTN_PIN_G = 17;
const int BTN_PIN_R = 13;
const int BTN_PIN_Y = 12;
const int LED_B = 18;
const int LED_G = 19;
const int LED_R = 20;
const int LED_Y = 22; 
const int BUZZER = 15;
volatile int blue_flag = 0;
volatile int green_flag = 0;
volatile int red_flag = 0;
volatile int yellow_flag = 0;
volatile int flag_start = 1;
volatile int blue = 0;
volatile int green = 0;
volatile int red = 0;
volatile int yellow = 0;

void btn_callback(uint gpio, uint32_t events) {
    // Handle the button press based on the GPIO and events
    if (events == 0x4) { // Falling edge
        if (gpio == BTN_PIN_B) {
            blue_flag = 1;
        }
        else if (gpio == BTN_PIN_G) {
            green_flag = 1;
        }
        else if (gpio == BTN_PIN_R) {
            red_flag = 1;
        }
        else if (gpio == BTN_PIN_Y) {
            yellow_flag = 1;
        }
    }
}


void generateSequence(int sequence[], int length) {
    absolute_time_t start_time = get_absolute_time();
    uint64_t start_time_us = to_ms_since_boot(start_time);
    srand(start_time_us);

    for (int i = 0; i < length; i++) {
        sequence[i] = rand() % 4;
    }
}

void displayColor(int color) {
    if (color == 0) 
    {
      gpio_put(LED_B, 1);
      sleep_ms(500);
      gpio_put(LED_B, 0);
      sleep_ms(500);
      blue += 1;
    }
    else if (color == 1) 
    {
      gpio_put(LED_G, 1);
      sleep_ms(500);
      gpio_put(LED_G, 0);
      sleep_ms(500);
      green += 1;
    }
    else if (color == 2) 
    {
      gpio_put(LED_R, 1);
      sleep_ms(500);
      gpio_put(LED_R, 0);
      sleep_ms(500);
      red += 1;
    }
    else if (color == 3) 
    {
      gpio_put(LED_Y, 1);
      sleep_ms(500);
      gpio_put(LED_Y, 0);
      sleep_ms(500);
      yellow += 1;
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

void play_victory_theme(int pino) {
  int level_complete_notes[] = {659, 659, 659, 523, 659, 523, 659, 440, 523, 494, 523, 440};
  int level_complete_durations[] = {125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125};

  for (int i = 0; i < sizeof(level_complete_notes) / sizeof(level_complete_notes[0]); i++) {
    play(level_complete_notes[i], level_complete_durations[i], pino);
    sleep_ms(50); // Pause between notes
  }
}

void play_loss_theme(int pino) {
  int level_complete_notes[] = {659, 659, 659, 659, 523, 523, 587, 659, 523, 440, 370, 440};
  int level_complete_durations[] = {125, 125, 125, 125, 125, 125, 250, 125, 62, 125, 125, 125};

  for (int i = 0; i < sizeof(level_complete_notes) / sizeof(level_complete_notes[0]); i++) {
    play(level_complete_notes[i], level_complete_durations[i], pino);
    sleep_ms(50); // Pause between notes
  }
}

int igualNoIntervalo(int v[], int v2[], int l, int r) {
    if (l == r) {
        return 1;
    }

    if (v[l] != v2[l]) {
        return 0;
    }

    return igualNoIntervalo(v, v2, l + 1, r);
}

void start_game(int pino, int sequence[], int length) {
  play_victory_theme(BUZZER);
  generateSequence(sequence, length);
  flag_start = 2;
}

int main() {
  int play_once = 0;
  int answer[16];
  int player_answer[16];
  int round = 1;
  int count = 0;

  stdio_init_all();

  gpio_init(BUZZER);
  gpio_set_dir(BUZZER, GPIO_OUT);

  gpio_init(LED_B);
  gpio_set_dir(LED_B, GPIO_OUT);

  gpio_init(LED_G);
  gpio_set_dir(LED_G, GPIO_OUT);

  gpio_init(LED_R);
  gpio_set_dir(LED_R, GPIO_OUT);

  gpio_init(LED_Y);
  gpio_set_dir(LED_Y, GPIO_OUT);

  gpio_init(BTN_PIN_B);
  gpio_set_dir(BTN_PIN_B, GPIO_IN);
  gpio_pull_up(BTN_PIN_B);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_init(BTN_PIN_Y);
  gpio_set_dir(BTN_PIN_Y, GPIO_IN);
  gpio_pull_up(BTN_PIN_Y);

  // callback led r (first)
  gpio_set_irq_enabled_with_callback(BTN_PIN_B, GPIO_IRQ_EDGE_FALL, true,
                                      &btn_callback);

  // callback led g (nao usar _with_callback)
  gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BTN_PIN_Y, GPIO_IRQ_EDGE_FALL, true);

  while (true) {
    if(flag_start == 1)
    {
      start_game(BUZZER, answer, 16);
    }
    else if (flag_start == 2)
    {
      if (play_once == 0)
      {
        for (int i = 0; i < round; i++)
        {
          displayColor(answer[i]);
        }

        play_once = 1;
      }
      
      if (blue_flag) {
        sleep_ms(200);
        play(440, 100, BUZZER);
        blue_flag = 0;
        player_answer[count] = 0;
        count += 1;
      }
      if (green_flag) {
        sleep_ms(200);
        play(392, 100, BUZZER);
        green_flag = 0;
        player_answer[count] = 1;
        count += 1;
      }
      if (red_flag) {
        sleep_ms(200);
        play(262, 100, BUZZER);
        red_flag = 0;
        player_answer[count] = 2;
        count += 1;
      }
      if (yellow_flag) {
        sleep_ms(200);
        play(294, 100, BUZZER);
        yellow_flag = 0;
        player_answer[count] = 3;
        count += 1;
      }

      if (igualNoIntervalo(answer, player_answer, 0, count) == 1 && round == count)
      {
        for (int i = 0; i < 3; i++) 
        {
          gpio_put(LED_G, 1);
          sleep_ms(500);
          gpio_put(LED_G, 0);
          sleep_ms(500);
        }
        play_victory_theme(BUZZER);

        round += 1;      
        count = 0;
        blue = 0;
        green = 0;
        red = 0;
        yellow = 0;
        play_once = 0;
        for (int i = 0; i < 16; i++) {
          player_answer[i] = 5;
        }
      }

      else if (round < count || player_answer[round-1] < 4)    
      {
        for (int i = 0; i < 3; i++) 
        {
          gpio_put(LED_R, 1);
          sleep_ms(500);
          gpio_put(LED_R, 0);
          sleep_ms(500);
        }
        play_loss_theme(BUZZER);

        round = 1;
        count = 0;
        blue = 0;
        green = 0;
        red = 0;
        yellow = 0;
        play_once = 0;
        for (int i = 0; i < 16; i++) {
          player_answer[i] = 5;
        }
        generateSequence(answer, 16);
      }
    }
    
  }
}