//=================================================================
// The speech class file.
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

#include "speech.h"
#define FONT FONT_5X7
#include "globals.h"
#include "hardware.h"
#define WHITE 0xFFFFFF
#define BLACK 0x000000
#define YELLOW 0xFFFF00

///////////////////////////////
//Static function declarations
///////////////////////////////
/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
static void draw_speech_line(const char* line, int which);


///////////////////////////////
//Drawing function declarations
///////////////////////////////


//***
// HINT: for this function and below: Check out the ULCD demo in the docs to see what
// uLCD methods you can use to draw graphics 

//Link: https://os.mbed.com/users/4180_1/notebook/ulcd-144-g2-128-by-128-color-lcd/
//***


void draw_speech_bubble()
{
    //****************
    // TODO: Implement
    //****************
    uLCD.filled_rectangle(1, 94, 126, 114, YELLOW);
    
}

void erase_speech_bubble()
{

    uLCD.filled_rectangle(0, 93, 127, 115, BLACK);
}

void draw_speech_line(const char* line, int which)
{
    uLCD.text_string((char*) line, 1, which + 12, FONT_5X7, YELLOW);
}

void speech_bubble_wait()
{
 GameInputs in;
    bool buttonPressed = false;
    
    do {
        in = read_inputs();
        if (!in.b1 && !buttonPressed) {
            buttonPressed = true; // mark the button as pressed
            wait(.5);
        } else if (!in.b1 && buttonPressed) {
            buttonPressed = false; // mark the button as released
        }
    } while (in.b1 || buttonPressed); 

}

void speech(const char* line1, const char* line2)
{
    //****************
    // TODO: Implement
    //****************
    // 1. Place the two lines into an array of lines
    // 2. Pass in to long_speech with the corresponding number of lines
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
    speech_bubble_wait();
    erase_speech_bubble();

    //const char* lines[2] = {line1, line2};
    //long_speech(lines, 2);
}

void long_speech(const char* lines[], int n)
{

    //****************
    // TODO: Implement
    //****************

    //1. Create a speech bubble

    //2. For each lines, display only two lines at a time
    //   If two lines are displayed, make sure to wait (call the wait function)

    //3. Erase the speech bubble when you are done
    draw_speech_bubble();
    for (int i = 0; i < n; i++) {
        draw_speech_line(lines[i], 0);
        if (i + 1 < n) {
            draw_speech_line(lines[i + 1], 1);
            speech_bubble_wait();
        }
        speech_bubble_wait();
        erase_speech_bubble();
    }
}

