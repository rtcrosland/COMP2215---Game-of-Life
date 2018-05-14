#include <stdint.h>
#include "bool.h"

#define ROWS 80 // 15
#define COLUMNS 12 // 20

//extern uint8_t screen[15][20];

void flush_array(uint8_t array[ROWS][COLUMNS]);
void set_pixel(uint8_t array[ROWS][COLUMNS], uint8_t x, uint8_t y);
void clear_pixel(uint8_t array[ROWS][COLUMNS], uint8_t x, uint8_t y);
bool get_pixel_state(uint8_t array[ROWS][COLUMNS], uint8_t x, uint8_t y);
void toggle_pixel(uint8_t array[ROWS][COLUMNS], uint8_t x, uint8_t y);