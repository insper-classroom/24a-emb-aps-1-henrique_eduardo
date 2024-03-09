#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "definindo.h"

const int BTN_PIN_B = 12;
const int BTN_PIN_G = 13;
const int BTN_PIN_R = 15;
const int BTN_PIN_Y = 14;
const int BTN_PIN_START = 17;

const int LED_B = 16;
const int LED_G = 18;
const int LED_R = 20;
const int LED_Y = 6; 

const int LED_B_RGB = 2;
const int LED_G_RGB = 3;
const int LED_R_RGB = 4;

const int BUZZER = 28;

volatile int blue_flag = 0;
volatile int green_flag = 0;
volatile int red_flag = 0;
volatile int yellow_flag = 0;
volatile int flag_start = 0;
volatile int inicio = 0;

volatile int btn_pressed_player=10;

void generateSequence(int sequence[], int length) {
    absolute_time_t start_time = get_absolute_time();
    uint64_t start_time_us = to_ms_since_boot(start_time);
    srand(start_time_us);

    for (int i = 0; i < length; i++) {
        sequence[i] = rand() % 4;
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

void displayColor(int color, int* blue, int* green, int* red, int* yellow) {
    if (color == 0) 
    {
      sleep_ms(250);
      play(440, 100, BUZZER);
      gpio_put(LED_B_RGB, 1);
      sleep_ms(500);
      gpio_put(LED_B_RGB, 0);
      sleep_ms(500);
      *blue += 1;
    }
    else if (color == 1) 
    {
      sleep_ms(250);
      play(392, 100, BUZZER);
      gpio_put(LED_G, 1);
      sleep_ms(500);
      gpio_put(LED_G, 0);
      sleep_ms(500);
      *green += 1;
    }
    else if (color == 2) 
    {
      sleep_ms(250);
      play(262, 100, BUZZER);
      gpio_put(LED_R, 1);
      sleep_ms(500);
      gpio_put(LED_R, 0);
      sleep_ms(500);
      *red += 1;
    }
    else if (color == 3) 
    {
      sleep_ms(250);
      play(294, 100, BUZZER);
      gpio_put(LED_Y, 1);
      sleep_ms(500);
      gpio_put(LED_Y, 0);
      sleep_ms(500);
      *yellow += 1;
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

void play_turnoff_theme(int pino) {
  int melody[] = {
    440, 392, 349, 330, 294, 262
  };

  // Duration of each note (500ms)
  const int noteDuration = 500;

  for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
    play(melody[i], noteDuration, pino);
    sleep_ms(50); // Pause between notes
  }
}


void play_loss_theme(int pino) {
  int loss_notes[] = {131, 117, 104, 98, 78};
  int loss_durations[] = {200, 200, 200, 200, 400};

  for (int i = 0; i < sizeof(loss_notes) / sizeof(loss_notes[0]); i++) {
    play(loss_notes[i], loss_durations[i], pino);
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

void action_button(int answer[],int *count,int *errou){
  if (blue_flag) {
        sleep_ms(200);
        play(440, 100, BUZZER);
        blue_flag = 0;
        if(answer[*count] != btn_pressed_player){
            *errou=1;
        }
        else
        {
          *errou = 2;
        }
        gpio_put(LED_B_RGB, 1);
        sleep_ms(500);
        gpio_put(LED_B_RGB, 0);
        sleep_ms(500);
        *count += 1;
  }
  if (green_flag) {
        sleep_ms(200);
        play(392, 100, BUZZER);
        green_flag = 0;        
        if(answer[*count] != btn_pressed_player){
            *errou=1;
        }
        else
        {
          *errou = 2;
        }
        gpio_put(LED_G, 1);
        sleep_ms(500);
        gpio_put(LED_G, 0);
        sleep_ms(500);
        *count += 1;
  }
  if (red_flag) {
        sleep_ms(200);
        play(262, 100, BUZZER);
        red_flag = 0;
        if(answer[*count] != btn_pressed_player){
            *errou=1;
        }
        else
        {
          *errou = 2;
        }
        gpio_put(LED_R, 1);
        sleep_ms(500);
        gpio_put(LED_R, 0);
        sleep_ms(500);
        *count += 1;
  }
  if (yellow_flag) {
        sleep_ms(200);
        play(294, 100, BUZZER);
        yellow_flag = 0;
        if(answer[*count] != btn_pressed_player){
            *errou=1;
        }
        else
        {
          *errou = 2;
        }
        gpio_put(LED_Y, 1);
        sleep_ms(500);
        gpio_put(LED_Y, 0);
        sleep_ms(500);
        *count += 1;
  }

}