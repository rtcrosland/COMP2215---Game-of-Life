#include <avr/io.h>
#include <stdint.h>
#include "ili934x.h"
#include "lcd.h"
#include "bool.h"

typedef struct Point {
    uint16_t x, y;
};

typedef struct Line {
    struct Point point_a;
    struct Point point_b;
};

void set_foreground_col(uint16_t col);
void set_background_col(uint16_t col);
void invert_pixel_p(struct Point point);
void invert_pixel(uint16_t x, uint16_t y);
void invert_pixel_2_p(struct Point point);
void invert_pixel_3_p(struct Point point);
void invert_pixel_2(uint16_t x, uint16_t y);
void invert_pixel_3(uint16_t x, uint16_t y);
void display_pixel_2(uint16_t x, uint16_t y);
void display_pixel_2_col(uint16_t x, uint16_t y, uint16_t col);
void display_pixel_3_col(uint16_t x, uint16_t y, uint16_t col);
void display_pixel_2_p_col(struct Point point, uint16_t col);
void display_pixel_3(uint16_t x, uint16_t y);
void display_pixel_p(struct Point point);
void display_pixel_p_col(struct Point point, uint16_t col);
void display_pixel(uint16_t x, uint16_t y);
void display_pixel_col(uint16_t x, uint16_t y, uint16_t col);
void display_line_2_l(struct Line line);
void display_line_2(struct Point point_a, struct Point point_b);
void display_line_l(struct Line line);
void display_line(struct Point point0, struct Point point1);
void display_line_vert(uint16_t x, uint16_t y0, uint16_t y1);
void display_line_horz(uint16_t x0, uint16_t x1, uint16_t y);
void swap(uint16_t* a, uint16_t* b);