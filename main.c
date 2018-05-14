#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"
#include "ruota.h"

#include "screen_cache.h"

#define HEIGHT 80 // 240
#define WIDTH  96 // 320

enum states {MENUING, SETTINGS, PLAYING, SIMULATING};
enum states current_state = MENUING;

struct Point user;
struct Point line_start;
struct Point centre;

volatile bool draw;
uint16_t value_under;
bool centre_clicked;
bool north, south, east, west;

// stuff for settings
bool loop;

bool running;

uint16_t colours[5] = {WHITE, RED, GREEN, BLUE, YELLOW};
uint16_t current_colour = 0;

void pixel_drawer(void);
void toggle_pixels(void);
bool check_neighbours(uint16_t x, uint16_t y);
bool check_neighbour(int x_mod, int y_mod, uint16_t x, uint16_t y);
char* get_colour_string(uint16_t i);

uint8_t screen[ROWS][COLUMNS];
uint8_t changes[ROWS][COLUMNS];

int main(void) {
    CLKPR = (1 << CLKPCE);
	CLKPR = 0;
    
	init_lcd();
    flush_array(screen);
    flush_array(changes);
    display.foreground = WHITE;
	os_init_ruota();
    
    value_under = BLACK;
    running = FALSE;
    loop = FALSE;
    
    //init(&head);
    
    user.x = 0;
    user.y = 0;
    centre.x = display.width >> 2;
    centre.y = display.height >> 2;
	set_frame_rate_hz(61);
	
	DDRB |= _BV(PB7);
	PORTB &= ~_BV(PB7);

    /* Enable tearing interrupt to get flicker free display */
	EIMSK |= _BV(INT6);
    
    /* enable button press inturrupt 
	TCCR1A = 0;
	TCCR1B = _BV(WGM12);
	TCCR1B |= _BV(CS11);
	TIMSK1 |= _BV(OCIE1A);

	OCR1A = 2000;
    
    TCCR3A = 0;
	TCCR3B = _BV(WGM32);
	TCCR3B |= _BV(CS31); // clk/8
	TIMSK3 |= _BV(OCIE3A);
    
	OCR3A = 2000;
    */
    
/*
 _____                                        _      
/  __ \                                      ( )     
| /  \/  ___   _ __  __      __  __ _  _   _ |/  ___ 
| |     / _ \ | '_ \ \ \ /\ / / / _` || | | |   / __|
| \__/\| (_) || | | | \ V  V / | (_| || |_| |   \__ \
 \____/ \___/ |_| |_|  \_/\_/   \__,_| \__, |   |___/
 _____                                  __/ |        
|  __ \                                |___/         
| |  \/  __ _  _ __ ___    ___                       
| | __  / _` || '_ ` _ \  / _ \                      
| |_\ \| (_| || | | | | ||  __/                      
 \____/ \__,_||_| |_| |_| \___|                      
         __   _      _   __                          
        / _| | |    (_) / _|                         
  ___  | |_  | |     _ | |_   ___                    
 / _ \ |  _| | |    | ||  _| / _ \                   
| (_) || |   | |____| || |  |  __/                   
 \___/ |_|   \_____/|_||_|   \___| */

    sei();
    
    for(;;) {
        if(current_state == MENUING) {
            menu_loop();
        }
        else if(current_state == SETTINGS) {
            settings_loop();
        }
        else {
            play_loop();
        }
    }
}

void menu_loop() {
    wipe_screen();
    
    display.x = 0;
    display.y = 0;
    
    display_string(" _____                                        _\n");
    display_string("/  __ \\                                      ( )\n");
    display_string("| /  \\/  ___   _ __  __      __  __ _  _   _ |/ ___\n");
    display_string("| |     / _ \\ | '_ \\ \\ \\ /\\ / / / _` || | | |  / __|\n");
    display_string("| \\__/\\| (_) || | | | \\ V  V / | (_| || |_| |  \\__ \\\n");
    display_string(" \\____/ \\___/ |_| |_|  \\_/\\_/   \\__,_| \\__, |  |___/\n");
    display_string(" _____                                  __/ |\n");
    display_string("|  __ \\                                |___/\n");
    display_string("| |  \\/  __ _  _ __ ___    ___\n");
    display_string("| | __  / _` || '_ ` _ \\  / _ \\\n");
    display_string("| |_\\ \\| (_| || | | | | ||  __/\n");
    display_string(" \\____/ \\__,_||_| |_| |_| \\___|\n");
    display_string("         __   _      _   __\n");
    display_string("        / _| | |    (_) / _|\n");
    display_string("  ___  | |_  | |     _ | |_   ___\n");
    display_string(" / _ \\ |  _| | |    | ||  _| / _ \\\n");
    display_string("| (_) || |   | |____| || |  |  __/\n");
    display_string(" \\___/ |_|   \\_____/|_||_|   \\___|\n");
    
    enum buttons {PLAY, SETTINGS};
    enum buttons current_button = PLAY;
    
    while(current_state == MENUING) {
        scan_switches();
        
        if(get_switch_press(_BV(SWE)) || get_switch_press(_BV(SWW))) {
            if(current_button == PLAY) {
                current_button = SETTINGS;
            }
            else {
                current_button = PLAY;
            }
        }
        if(get_switch_press(_BV(SWC))) {
            if(current_button == PLAY) {
                current_state = PLAYING;
            }
            else {
                current_state = SETTINGS;
            }
        }
        
        cli();
        if(current_button == PLAY) {
            display.foreground = RED;
            display_string_xy("PLAY", 20, 200);
            display.foreground = WHITE;
            
            display_string_xy("SETTINGS", 180, 200);
        }
        else {
            display_string_xy("PLAY", 20, 200);
            
            display.foreground = RED;
            display_string_xy("SETTINGS", 180, 200);
            display.foreground = WHITE;
        }
        sei();
    }
}

void settings_loop() {
    wipe_screen();
    
    enum settings_buttons {LOOP, COLOUR, BACK};
    enum settings_buttons current_button = LOOP;
    
    while(current_state == SETTINGS) {
        scan_switches();
        
        if(get_switch_press(_BV(SWN)) || get_switch_press(_BV(SWS))) {
            if(current_button == LOOP) {
                current_button = COLOUR;
            }
            else if(current_button == COLOUR) {
                current_button = BACK;
            }
            else {
                current_button = LOOP;
            }
        }
        if(get_switch_press(_BV(SWC))) {
            if(current_button == LOOP) {
                if(loop) {
                    loop = FALSE;
                }
                else {
                    loop = TRUE;
                }
            }
            else if(current_button == COLOUR) {
                next_colour();
            }
            else {
                current_state = MENUING;
            }
        }
        
        cli();
        if(current_button == LOOP) {
            display.foreground = RED;
            display_string_xy("Make the screen loop?", 10, 10);
            if(loop) {
                display_string_xy("YES", 150, 10);
            }
            else {
                display_string_xy("NO ", 150, 10);
            }
            display.foreground = WHITE;
            
            display_string_xy("Cell colour: ", 10, 20);
            display_string_xy(get_colour_string(current_colour), 150, 20);
            
            display_string_xy("Back.", 10, 200);
        }
        else if(current_button == COLOUR) {
            display_string_xy("Make the screen loop?", 10, 10);
            if(loop) {
                display_string_xy("YES", 150, 10);
            }
            else {
                display_string_xy("NO ", 150, 10);
            }
            
            display.foreground = RED;
            display_string_xy("Cell colour: ", 10, 20);
            display_string_xy(get_colour_string(current_colour), 150, 20);
            display.foreground = WHITE;
            
            display_string_xy("Back.", 10, 200);
        }
        else {
            display_string_xy("Make the screen loop?", 10, 10);
            if(loop) {
                display_string_xy("YES", 150, 10);
            }
            else {
                display_string_xy("NO ", 150, 10);
            }
            
            display_string_xy("Cell colour: ", 10, 20);
            display_string_xy(get_colour_string(current_colour), 150, 20);
            
            display.foreground = RED;
            display_string_xy("Back.", 10, 200);
            display.foreground = WHITE;
        }
        sei();
    }
}

void play_loop() {
    wipe_screen();
    
    // main loop
    while(!running) {
        scan_switches();
        
        if(get_switch_state(_BV(SWC))) {
            draw = TRUE;
        }
        else {
            draw = FALSE;
        }
        
        if(get_switch_short(_BV(SWC))) {
            centre_clicked = TRUE;
        }
        else {
            centre_clicked = FALSE;
        }
        if(get_switch_press(_BV(SWN))) {
            north = TRUE;
        }
        else {
            north = FALSE;
        }
        if(get_switch_press(_BV(SWS))) {
            south = TRUE;
        }
        else {
            south = FALSE;
        }
        if(get_switch_press(_BV(SWE))) {
            east = TRUE;
        }
        else {
            east = FALSE;
        }
        if(get_switch_press(_BV(SWW))) {
            west = TRUE;
        }
        else {
            west = FALSE;
        }
        
        if(!running) {
            // move cursor and draw if necessary
            
            cli();
            
            uint16_t new_value;
            if(north && (user.y > 0))
            {
                pixel_drawer();
                user.y--;
            }
            else if(south && (user.y < HEIGHT)) {
                pixel_drawer();
                user.y++;
            }
            else if(east && (user.x < WIDTH)) {
                pixel_drawer();
                user.x++;
            }
            else if(west && (user.x > 0)) {
                pixel_drawer();
                user.x--;
            }
            if(centre_clicked) {
                if(get_pixel_state(screen, user.x, user.y)) {
                    display_pixel_3_col(user.x, user.y, BLACK);
                }
                else {
                    display_pixel_3_col(user.x, user.y, WHITE);
                }
                toggle_pixel(screen, user.x, user.y);
                
                if((user.x == (uint16_t) 0) && (user.y == (uint16_t) 0)) {
                    running = TRUE;
                    current_state = SIMULATING;
                }
            }
            
            sei();
        }
    }
    
    cli();
        
    // start simluation
    uint16_t x, y;
    while(current_state == SIMULATING) {
        for(y = 0; y < HEIGHT; y++) {
            for(x = 0; x < WIDTH; x++) {
                if(check_neighbours(x, y)) {
                    /*
                    DATA data;
                    data.x = x;
                    data.y = y;
                            
                    head = add(head, data);
                    */
                    set_pixel(changes, x, y);
                }
            }
        }
     
        toggle_pixels();
        //head = free_list(head);
        flush_array(changes);
        
        scan_switches();
        if(get_switch_press(_BV(SWC))) {
            current_state = MENUING;
        }
    }
}


void wipe_screen() {
    uint8_t i;
    
    cli();
    display.foreground = BLACK;
    for(i = 0; i < display.height; i++) {
        struct Point p1, p2;
        
        p1.x = 0;
        p1.y = i;
        p2.x = display.width-1;
        p2.y = i;
        
        display_line(p1, p2);
    }
    display.foreground = WHITE;
    sei();
}

char* get_colour_string(uint16_t i) {
    switch(i) {
        case 0:
            return "white ";
        case 1:
            return "red   ";
        case 2:
            return "green ";
        case 3:
            return "blue  ";
        case 4:
            return "yellow";
    }
}

void next_colour() {
    current_colour++;
    
    if(current_colour > 4) {
        current_colour = 0;
    }
}


void pixel_drawer() {
    if(draw) { // toggle the current value
        if(get_pixel_state(screen, user.x, user.y)) {
            display_pixel_3_col(user.x, user.y, BLACK);
        }
        else {
            display_pixel_3_col(user.x, user.y, WHITE);
        }
        toggle_pixel(screen, user.x, user.y);
    }
    else {
        if(get_pixel_state(screen, user.x, user.y)) {
            display_pixel_3_col(user.x, user.y, WHITE);
        }
        else {
            display_pixel_3_col(user.x, user.y, BLACK);
        }
    }
}

void toggle_pixels() {
    uint8_t i, j;
    
    for(i = 0; i < HEIGHT; i++) {
        for(j = 0; j < WIDTH; j++) {
            if (get_pixel_state(changes, j, i)) {
                toggle_pixel(screen, j, i);
                invert_pixel_3(j, i);
            }
        }
    }
}

bool check_neighbours(uint16_t x, uint16_t y) {
    int live_neighbours;
    live_neighbours = 0;
    
    bool alive;
    if(get_pixel_state(screen, x, y)) {
        alive = TRUE;
    }
    else {
        alive = FALSE;
    }
    
    // NW
    if(check_neighbour(-1, -1, x, y)) {
       live_neighbours++; 
    }
    // N
    if(check_neighbour(0, -1, x, y)) {
       live_neighbours++; 
    }
    // NE
    if(check_neighbour(+1, -1, x, y)) {
       live_neighbours++; 
    }
    // E
    if(check_neighbour(+1, 0, x, y)) {
       live_neighbours++; 
    }
    // SE
    if(check_neighbour(+1, +1, x, y)) {
       live_neighbours++; 
    }
    // S
    if(check_neighbour(0, +1, x, y)) {
       live_neighbours++; 
    }
    // SW
    if(check_neighbour(-1, +1, x, y)) {
       live_neighbours++; 
    }
    // W
    if(check_neighbour(-1, 0, x, y)) {
       live_neighbours++; 
    }
    
    if(alive) {
        if((live_neighbours < 2) || (live_neighbours > 3)) {
            return TRUE;
        }
        else {
            return FALSE;
        }
    }
    else {
        if(live_neighbours == 3) {
            return TRUE;
        }
        else {
            return FALSE;
        }
    }
}

bool check_neighbour(int x_mod, int y_mod, uint16_t x, uint16_t y) {
    int new_x, new_y;
    
    new_x = (int) x + x_mod;
    if(new_x < 0 || new_x > WIDTH) {
        return FALSE; // anything outside the boundaries are counted as dead
    }
    new_y = (int) y + y_mod;
    if(new_y < 0 || new_y > HEIGHT) {
        return FALSE;
    }
    
    bool pixel_state = get_pixel_state(screen, (uint16_t) new_x, (uint16_t) new_y);
    
    if(pixel_state) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

ISR(INT6_vect) {
    // DRAW SCREEN:
    // clear screen
    
    // draw user pixel
    if(current_state == PLAYING) {
        display_pixel_3_p_col(user, MAGENTA);
    }
    
    // draw menu buttons
    if(current_state == MENUING) {
        struct Point P1, P2, P3, P4;
        P1.x = 10; // top left
        P1.y = 180;
        P2.x = 10; // bottom left
        P2.y = 230;
        P3.x = 150; // top right
        P3.y = 180;
        P4.x = 150; // bottom right
        P4.y = 230;
        display_line(P1, P3);
        display_line(P3, P4);
        display_line(P4, P2);
        display_line(P2, P1);
        
        struct Point S1, S2, S3, S4;
        S1.x = 310; // top right
        S1.y = 180;
        S2.x = 310; // bottom right
        S2.y = 230;
        S3.x = 170; // top left
        S3.y = 180;
        S4.x = 170; // bottom left
        S4.y = 230;
        display_line(S3, S1);
        display_line(S1, S2);
        display_line(S2, S4);
        display_line(S4, S3);
    }
}

/*
struct Line transform_line(struct Line line) {
    
    // width/height = 320/240
    
    struct Point newa, newb;
    newa = transform_point(line.point_a);
    newb = transform_point(line.point_b);
    
    struct Line newLine;
    newLine.point_a = newa;
    newLine.point_b = newb;
    
    return newLine;
}

float to_radians(float degrees) {
    return degrees * (M_PI / 180.0f);
}

float to_degrees(float radians) {
    return radians * (180.0f / M_PI);
}
*/