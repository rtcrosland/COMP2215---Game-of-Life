#include "graphics.h"

void set_foreground_col(uint16_t col) {
    display.foreground = col;
}

void set_background_col(uint16_t col) {
    display.background = col;
}

void invert_pixel_p(struct Point point) {
    invert_pixel(point.x, point.y);
}

void invert_pixel(uint16_t x, uint16_t y) {
    uint16_t current_col = read_pixel(x, y);
    uint16_t new_col = ~current_col;
    uint16_t old_foreground = display.foreground;
    
    display_pixel_col(x, y, new_col);
}

void invert_pixel_2_p(struct Point point) {
    invert_pixel_2(point.x, point.y);
}

void invert_pixel_3_p(struct Point point) {
    invert_pixel_3(point.x, point.y);
}

void invert_pixel_2(uint16_t x, uint16_t y) {
    x = x << 1;
    y = y << 1;
    
    invert_pixel(x, y);
    invert_pixel(x, y + 1);
    invert_pixel(x + 1, y);
    invert_pixel(x + 1, y + 1);
}

void invert_pixel_3(uint16_t x, uint16_t y) {
    x = x * 3;
    y = y * 3;
    
    invert_pixel(x, y);
    invert_pixel(x, y + 1);
    invert_pixel(x, y + 2);
    invert_pixel(x + 1, y);
    invert_pixel(x + 1, y + 1);
    invert_pixel(x + 1, y + 2);
    invert_pixel(x + 2, y);
    invert_pixel(x + 2, y + 1);
    invert_pixel(x + 2, y + 2);
}

void display_pixel_p(struct Point point) {
    display_pixel(point.x, point.y);
}

void display_pixel_p_col(struct Point point, uint16_t col) {
    uint16_t old_col;
    old_col = display.foreground;
    display.foreground = col;
    
    display_pixel(point.x, point.y);
    
    display.foreground = old_col;
}

void display_pixel_2(uint16_t x, uint16_t y) {
    x = x << 1;
    y = y << 1;
    
    rectangle pixel;
    pixel.left = x;
    pixel.right = x + 1;
    pixel.top = y;
    pixel.bottom = y + 1;
    
    fill_rectangle(pixel, display.foreground);
}

void display_pixel_2_col(uint16_t x, uint16_t y, uint16_t col) {
    uint16_t old_col;
    old_col = display.foreground;
    display.foreground = col;
    
    display_pixel_2(x, y);
    
    display.foreground = old_col;
}

void display_pixel_3_col(uint16_t x, uint16_t y, uint16_t col) {
    uint16_t old_col;
    old_col = display.foreground;
    display.foreground = col;
    
    display_pixel_3(x, y);
    
    display.foreground = old_col;
}

void display_pixel_2_p_col(struct Point point, uint16_t col) {
    uint16_t old_col;
    old_col = display.foreground;
    display.foreground = col;
    
    display_pixel_2(point.x, point.y);
    
    display.foreground = old_col;
}

void display_pixel_3_p_col(struct Point point, uint16_t col) {
    uint16_t old_col;
    old_col = display.foreground;
    display.foreground = col;
    
    display_pixel_3(point.x, point.y);
    
    display.foreground = old_col;
}

void display_pixel_3(uint16_t x, uint16_t y) {
    x = x * 3;
    y = y * 3;
    
    rectangle pixel;
    pixel.left = x;
    pixel.right = x + 2;
    pixel.top = y;
    pixel.bottom = y + 2;
    
    fill_rectangle(pixel, display.foreground);
}

void display_pixel(uint16_t x, uint16_t y) {
    write_cmd(PAGE_ADDRESS_SET);
    write_data16(y);
    write_data16(y);
    
    write_cmd(COLUMN_ADDRESS_SET);
    write_data16(x);
    write_data16(x);
    
    write_cmd(MEMORY_WRITE);
    write_data16(display.foreground);
}

void display_pixel_col(uint16_t x, uint16_t y, uint16_t col) {
    uint16_t old_col;
    old_col = display.foreground;
    display.foreground = col;
    
    display_pixel(x, y);
    
    display.foreground = old_col;
}

void display_line_2_l(struct Line line) {
    display_line_2(line.point_a, line.point_b);
}

void display_line_2(struct Point point_a, struct Point point_b) {
    uint16_t a_x, a_y, b_x, b_y;
    a_x = point_a.x << 1;
    a_y = point_a.y << 1;
    b_x = point_b.x << 1;
    b_y = point_b.y << 1;
    
    struct Point line1_a;
    struct Point line1_b;
    line1_a.x = a_x;
    line1_a.y = a_y;
    line1_b.x = b_x;
    line1_b.y = b_y;
    
    struct Point line2_a;
    struct Point line2_b;
    line2_a.x = a_x;
    line2_a.y = a_y + 1;
    line2_b.x = b_x;
    line2_b.y = b_y + 1;
    
    struct Point line3_a;
    struct Point line3_b;
    line3_a.x = a_x + 1;
    line3_a.y = a_y;
    line3_b.x = b_x + 1;
    line3_b.y = b_y;
    
    struct Point line4_a;
    struct Point line4_b;
    line4_a.x = a_x + 1;
    line4_a.y = a_y + 1;
    line4_b.x = b_x + 1;
    line4_b.y = b_y + 1;
}

void display_line_l(struct Line line) {
    display_line(line.point_a, line.point_b);
}

void display_line(struct Point point_a, struct Point point_b) {
    uint16_t x0, y0, x1, y1, x, y, dx, dy;
    
    x0 = point_a.x;
    y0 = point_a.y;
    x1 = point_b.x;
    y1 = point_b.y;
    
    bool steep = FALSE;
    
    // check if the line is straight
    if(x0 == x1) {
        display_line_vert(x0, y0, y1);
        return;
    }
    if(y0 == y1) {
        display_line_horz(x0, x1, y0);
        return;
    }
    
    if(abs(x0 - x1) < abs(y0 - y1))
    {
        swap(&x0, &y0);
        swap(&x1, &y1);
        steep = TRUE;
    }
    if(x0 > x1) {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }
    
    x = x0;
    for(x; x <= x1; x++) {
        float t = (x-x0)/(float)(x1-x0);
        y = y0*(1.-t) + y1*t;
        if(steep) {
            display_pixel(y, x);
        }
        else {
            display_pixel(x, y);
        }
    }
}

display_line_vert(uint16_t x, uint16_t y0, uint16_t y1) {
    if(y0 > y1) {
        swap(&y0, &y1);
    }
    
    for(y0; y0 <= y1; y0++) {
        display_pixel(x, y0);
    }
}

display_line_horz(uint16_t x0, uint16_t x1, uint16_t y) {
    if(x0 > x1) {
        swap(&x0, &x1);
    }
    
    for(x0; x0 <= x1; x0++) {
        display_pixel(x0, y);
    }
}

void swap(uint16_t* a, uint16_t* b) {
    uint16_t temp = *a;
    *a = *b;
    *b = temp;
}