#include "screen_cache.h"

// 15 x 20 = 300 entries = 2400 positions
// uint8_t screen[ROWS][COLUMNS];

void flush_array(uint8_t array[ROWS][COLUMNS]) {
    uint8_t i, j;
    
    for(i = 0; i < ROWS; i++) {
        for(j = 0; j < COLUMNS; j++) {
            array[i][j] = 0x00;
        }
    }
}

void set_pixel(uint8_t array[ROWS][COLUMNS], uint8_t x, uint8_t y) {
    uint8_t x_index;
    
    x_index = x / 8;
    
    array[y][x_index] |= 0x1 << (x % 8);
}

void clear_pixel(uint8_t array[ROWS][COLUMNS], uint8_t x, uint8_t y) {
    uint8_t x_index;
    
    x_index = x / 8;
    
    array[y][x_index] &= ~(0x1 << (x % 8));
}

bool get_pixel_state(uint8_t array[ROWS][COLUMNS], uint8_t x, uint8_t y)
{
    uint8_t x_index;
    
    x_index = x / 8;
    
    uint8_t location;
    location = array[y][x_index];
    
    bool state;
    state = (location & (0x1 << (x % 0x8))) != 0x00;
    
    return state;
}

void toggle_pixel(uint8_t array[ROWS][COLUMNS], uint8_t x, uint8_t y) {
    if(get_pixel_state(array, x, y)) {
        clear_pixel(array, x, y);
    }
    else {
        set_pixel(array, x, y);
    }
}