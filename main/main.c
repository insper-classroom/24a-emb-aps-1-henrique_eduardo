/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "funcoes.h"

void btn_callback(uint gpio, uint32_t events) {
    // Handle the button press based on the GPIO and events
    if (events == 0x4) { // Falling edge
        if (gpio == BTN_PIN_B) {
          if (inicio == 1)
          {
            blue_flag = 1;
            btn_pressed_player=0;
          }
        }
        else if (gpio == BTN_PIN_G) {
          if (inicio == 1)
          {
            green_flag = 1;
            btn_pressed_player=1;
          }          
        }
        else if (gpio == BTN_PIN_R) {
          if (inicio == 1)
          {
            red_flag = 1;
            btn_pressed_player=2;
          }
        }
        else if (gpio == BTN_PIN_Y) {
          if (inicio == 1)
          {
            yellow_flag = 1;
            btn_pressed_player=3;
          }
        }
        else if (gpio == BTN_PIN_START) {
          if (flag_start == 0)
          {
            inicio = 1;
            flag_start = 1;
          }
          else if (flag_start == 2)
          {
            inicio = 2;
          }
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

void playWin(int pin) {
    int melody[] = {
        0, 2, 294, 4,       // D4
        392, -4, 466, 8, 440, 4, // G4, AS4, A4
        392, 2, 587, 4,        // G4, D5
        523, -2,               // C5
        440, -2,               // A4
        392, -4, 466, 8, 440, 4, // G4, AS4, A4
        349, 2, 415, 4,        // F4, GS4
        294, -1,               // D4
        294, 4,                // D4
        392, -4, 466, 8, 440, 4, // G4, AS4, A4
        392, 2, 587, 4,        // G4, D5
        698, 2, 659, 4,        // F5, E5
        622, 2, 494, 4,        // DS5, B4
        622, -4, 587, 8, 554, 4, // DS5, D5, CS5
        554, 2, 494, 4,        // CS4, B4
        392, -1,               // G4
        466, 4,                // AS4
        587, 2, 466, 4,        // D5, AS4
        587, 2, 466, 4,        // D5, AS4
        622, 2, 587, 4,        // DS5, D5
        554, 2, 440, 4,        // CS5, A4
        466, -4, 587, 8, 554, 4, // AS4, D5, CS5
        554, 2, 587, 4,        // CS4, D4
        587, -1,               // D5
        0, 4, 466, 4,       // A4
        587, 2, 466, 4,        // D5, AS4
        587, 2, 466, 4,        // D5, AS4
        698, 2, 659, 4,        // F5, E5
        622, 2, 494, 4,        // DS5, B4
        622, -4, 587, 8, 554, 4, // DS5, D5, CS5
        554, 2, 466, 4,        // CS4, AS4
        392, -1                // G4
    };
    int melodyLength = sizeof(melody) / sizeof(melody[0]) / 2;
    int tempo = 144;

    float wholenote = (60000 * 4) / (float)tempo;

    for (int thisNote = 0; thisNote < melodyLength * 2; thisNote = thisNote + 2) {
      float divider = melody[thisNote + 1];
      float noteDuration = 0; // Initialize noteDuration
      if (divider > 0) {
          noteDuration = (wholenote) / divider;
      } else if (divider < 0) {
          noteDuration = (wholenote) / (float)abs(divider);
          noteDuration *= 1.5;
      }
      play(melody[thisNote], noteDuration, pin);
      sleep_ms(1); // Optional pause between notes
    }

}


int main() {
  int play_once = 0;
  int answer[16];
  int round = 1;
  int count = 0;
  int errou=0;
  int blue = 0;
  int green = 0;
  int red = 0;
  int yellow = 0;
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

  gpio_init(LED_R_RGB);
  gpio_set_dir(LED_R_RGB, GPIO_OUT);

  gpio_init(LED_B_RGB);
  gpio_set_dir(LED_B_RGB, GPIO_OUT);

  gpio_init(LED_G_RGB);
  gpio_set_dir(LED_G_RGB, GPIO_OUT);

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

  gpio_init(BTN_PIN_START);
  gpio_set_dir(BTN_PIN_START, GPIO_IN);
  gpio_pull_up(BTN_PIN_START);


  // callback led r (first)
  gpio_set_irq_enabled_with_callback(BTN_PIN_B, GPIO_IRQ_EDGE_FALL, true,
                                      &btn_callback);

  // callback led g (nao usar _with_callback)
  gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BTN_PIN_Y, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BTN_PIN_START, GPIO_IRQ_EDGE_FALL, true);
  
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
          displayColor(answer[i], &blue, &green, &red, &yellow);
        }

        play_once = 1;
      }
      
      action_button(answer,&count,&errou);
      if (errou == 2 && round <= count)
      {
        for (int i = 0; i < 3; i++) 
        {
          gpio_put(LED_G_RGB, 1);
          sleep_ms(500);
          gpio_put(LED_G_RGB, 0);
          sleep_ms(500);
        }
        play_victory_theme(BUZZER);
        sleep_ms(100);

        round += 1;
        if (round >= 16)
        {
          sleep_ms(250);
          playWin(BUZZER);
          flag_start = 0;
        }      
        count = 0;
        blue = 0;
        green = 0;
        red = 0;
        yellow = 0;
        play_once = 0;
        errou = 0;
      }

      else if (round < count || errou == 1)    
      {
        for (int i = 0; i < 3; i++) 
        {
          gpio_put(LED_R_RGB, 1);
          sleep_ms(500);
          gpio_put(LED_R_RGB, 0);
          sleep_ms(500);
        }
        play_loss_theme(BUZZER);
        sleep_ms(100);

        round = 1;
        count = 0;
        blue = 0;
        green = 0;
        red = 0;
        yellow = 0;
        play_once = 0;
        errou=0;
        generateSequence(answer, 16);
      }
    }

    if (inicio == 2)
    {
      play_turnoff_theme(BUZZER);
      inicio = 0;
      flag_start = 0;
    }    
  }
}