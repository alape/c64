#include <stdio.h>
#include <stdlib.h>
#include <peekpoke.h>
#include <time.h>
#include <c64.h>

int v = 0xD000;	// START OF DISPLAY CHIP

// Need wait function to slow down loop
void rasterWait(void) {
	unsigned char raster;
	do {
		raster = PEEK(v + 18);
	} while (raster < 250 || raster > 252);
}

void toggleUppercase() {
//  POKE 53272,23 :REM LOWER/UPPER CASE MODE
//  POKE 53272,21 :REM UPPER CASE/PETSCII MODE
    if (PEEK(53272) == 23) {
        POKE(53272, 21);
    } else {
        POKE(53272, 23);
    }
}

void setChrAt(unsigned char col, unsigned char row, unsigned char chr) {
    if ((col > 39) || (row > 24)) {
        return;
    }
    POKE(1024 + col + 40*row, chr);
}

void setColourAt(unsigned char col, unsigned char row, unsigned char colour) {
    if ((col > 39) || (row > 24)) {
        return;
    }
    POKE(55296 + col + 40*row, colour);
}

void beep(unsigned int frequency) {
    //unsigned int t;

    SID.amp      = 0x1F; 	// Volume
	SID.v1.ad    = 0x09; 	// Attack/decay voice 1
	
    SID.v1.freq  = frequency;	// Frequency 
    SID.v1.ctrl  = 0x11;	// Control voice 1
    // for (t = 0 ; t < 2500; t++) {}
    // SID.v1.ctrl  = 0x10;
}

int main (void) {
    int x, y;
    int dx = 3, dy = 0;

    unsigned char ball_colour = 0;
    unsigned char is_playing = 0;
    unsigned char play_duration = 0;

	srand(time(NULL));

    x = rand() % 100;
    y = rand() % 100;

	printf("%c", 147); // Clear screen
	POKE(v + 24, 21); // Set upper case
	POKE(v + 32, COLOR_GRAY2); // Set border color
	POKE(v + 33, COLOR_BLACK); // Set background color
    printf("bouncing ball demo, o rly?");

	do {
		rasterWait();

        setChrAt(x / 10, y / 10, 32); // 32 is the PETSCII symbol for whitespace

        dy += 1; // gravity

        x += dx;
        if (x < 0 || x > 399) { 
            dx = -(dx - 1); // loss of impulse on collision
            x += dx;
            ball_colour = (unsigned char) (rand() % 15) + 1; // choose any colour other than black
            beep(0x133F); is_playing = 1;
        }	

        y += dy;
        if (y < 0 || y > 249) {
            dy = dy > 0? -(dy - 1) : -(dy + 1); // loss of impulse on collision
            if (dy == 0) {
                beep(0x16E3);
                x = rand() % 100;
                y = rand() % 100;
                dx = (rand() % 5) + 1;
            }
            y += dy;
            ball_colour = (unsigned char) (rand() % 15) + 1;
            beep(0x133F); is_playing = 1;
        }

        if (is_playing) {
            if (play_duration > 3) {
                play_duration = 0;
                is_playing = 0;
                SID.v1.ctrl  = 0x10;
            } else {
                ++play_duration;
            }
        }
        
		setChrAt(x / 10, y / 10, 87); // 87 is the PETSCII symbol for ball
        setColourAt(x / 10, y / 10, ball_colour);
	} while (1);
    return EXIT_SUCCESS;	
}