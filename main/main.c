#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"


const int BTN_PIN_R= 15;
const int BTN_PIN_G =14;
const int LED_R= 22;
const int LED_G =21;
const int LED_Y =19;
const int BUZZER =17;

volatile int btn_pressed = 0;
volatile int verifica = 0;
volatile int n = 1;
volatile int i = 0;
volatile int comecou=0;
volatile int timer_value = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) { // fall edge
        btn_pressed = gpio;
        verifica = 1;
        comecou=1;

      
    }
    if (events == 0x8) { // rise edge
        busy_wait_ms(150);
        btn_pressed = 0;
        verifica = 0;
    }
}
bool timer_0_callback(repeating_timer_t *rt) {
    timer_value += 1; 
}
void play(int freq, int tempo, int pino) {
    int periodo = 1000000 / freq;
    for (int i = 0; i < (tempo * freq / 1000); i++) {
        gpio_put(pino, 1);
        sleep_us(periodo / 2);
        gpio_put(pino, 0);
        sleep_us(periodo / 2);
    }
}

void sorteador(int arr[], int leds[]) {
    int random_number = rand() % 2;
    if (random_number == 0) {
        arr[i] = BTN_PIN_R;
        leds[i] = LED_R;
        
    } else if (random_number == 1) {
        arr[i] = BTN_PIN_G;
        leds[i] = LED_G;
        
    }
}



int main() {
    stdio_init_all();
    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_init(LED_Y);
    gpio_set_dir(LED_Y, GPIO_OUT);
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);
    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);
    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);
    repeating_timer_t timer_0;
    add_repeating_timer_ms(1,timer_0_callback, NULL, &timer_0);
    
    int arr[100];
    int leds[100];
    while (true) {
        if(comecou){
            cancel_repeating_timer(&timer_0);
            srand(timer_value);
            int errado = 0;
            sorteador(arr, leds);

            for (int c = 0; c < n; c++) {
                if(leds[c]==LED_G){
                    play(440, 900, BUZZER); 
                }
                if(leds[c]==LED_R){
                    play(330, 300, BUZZER); 
                }
                gpio_put(leds[c], 1);
                sleep_ms(500);
                gpio_put(leds[c], 0);
                sleep_ms(500);
            }

            int contador = 0;
            btn_pressed=0;
            verifica=0;
            while (contador < n) {
                if (verifica && btn_pressed == arr[contador]) {
                    
                    contador++;
                    btn_pressed = 0;
                    verifica = 0;
                    
                } else if (verifica && btn_pressed != arr[contador]) {
                
                    errado = 1;
                    contador++;
                    btn_pressed = 0;
                    verifica = 0;
                    
            
                }
            }

            if (!errado) {
                gpio_put(LED_G, 1);
                sleep_ms(500);
                gpio_put(LED_G, 0);
                sleep_ms(500);
                n++; // Incrementa o número de LEDs na próxima rodada
                i++; 
                timer_value+=10;
            } else {
                gpio_put(LED_Y, 1);
                sleep_ms(500);
                gpio_put(LED_Y, 0);
                sleep_ms(500);
                i=0;
                n=1;
                timer_value+=10;
            }
        }

        

       
    }

    
}
