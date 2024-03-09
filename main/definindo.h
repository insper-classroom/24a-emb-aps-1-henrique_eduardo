#ifndef DEFININDO_H
#define DEFININDO_H
extern const int BTN_PIN_B;
extern const int BTN_PIN_G;
extern const int BTN_PIN_R;
extern const int BTN_PIN_Y;
extern const int BTN_PIN_START;

extern const int LED_B;
extern const int LED_G;
extern const int LED_R;
extern const int LED_Y;

extern const int LED_B_RGB;
extern const int LED_G_RGB;
extern const int LED_R_RGB;

extern const int BUZZER;

extern volatile int blue_flag;
extern volatile int green_flag;
extern volatile int red_flag;
extern volatile int yellow_flag;
extern volatile int flag_start;
extern volatile int inicio;

extern volatile int btn_pressed_player;

void generateSequence(int sequence[], int length);
void play(int freq, int tempo, int pino);
void displayColor(int color, int* blue, int* green, int* red, int* yellow);
void play_victory_theme(int pino);
void play_turnoff_theme(int pino);
void play_loss_theme(int pino);
int igualNoIntervalo(int v[], int v2[], int l, int r);
void start_game(int pino, int sequence[], int length);
void playWin(int pin);


#endif