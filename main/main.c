/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "funcoes.c"

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
      
      verifica_button(answer,&count,&errou);
      if (errou == 2 && round <= count)
      {
        for (int i = 0; i < 3; i++) 
        {
          gpio_put(LED_G_RGB, 1);
          sleep_ms(250);
          gpio_put(LED_G_RGB, 0);
          sleep_ms(100);
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

        show_points(round);

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
      play_once = 0;
      round = 1;
      count = 0;
      blue = 0;
      green = 0;
      red = 0;
      yellow = 0;
      play_once = 0;
      errou=0;
    }    
  }
}