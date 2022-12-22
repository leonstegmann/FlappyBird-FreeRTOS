#include <stdio.h>

#include "TUM_Draw.h" 
#include "TUM_Font.h"

#include "draw.h"

/* Filenames */
#define BACKGROUND_FILENAME "flappybird.png"

/* Definitions for drawButton */
#define BOX_WIDTH 100
#define BOX_HEIGHT 40
#define BOX_COLOUR Teal
#define BOX_FRAME_COLOUR Silver
#define BOX_OUTLINE_COLOUR Teal
#define BOX_FONT_SIZE 18

image_handle_t backroundImage = NULL;

int drawBackround() {
    static int image_height;

    if (backroundImage == NULL) {
        backroundImage = tumDrawLoadImage(BACKGROUND_FILENAME);
    }

    if ((image_height = tumDrawGetLoadedImageHeight(backroundImage)) != -1) {
        tumDrawLoadedImage(backroundImage, 0, 0);
        return 1;

    } else { 
        printf("Failed to get size of image '%s', does it exist?\n",
                BACKGROUND_FILENAME);
        return 0;
    }
}

int drawButton(coord_t pos, char *str) {

    static int text_width;
    font_handle_t cur_font = tumFontGetCurFontHandle();

    // Draw outlines
    tumDrawFilledBox(pos.x , pos.y + 1, BOX_WIDTH, BOX_HEIGHT, BOX_OUTLINE_COLOUR);
    tumDrawFilledBox(pos.x + 1, pos.y, BOX_WIDTH, BOX_HEIGHT, BOX_OUTLINE_COLOUR);
    tumDrawFilledBox(pos.x + 1, pos.y + 1, BOX_WIDTH, BOX_HEIGHT, BOX_OUTLINE_COLOUR);
    
    // Draw Box with Frame
    tumDrawFilledBox(pos.x, pos.y, BOX_WIDTH, BOX_HEIGHT, BOX_FRAME_COLOUR);
    tumDrawFilledBox(pos.x + 3, pos.y + 3, BOX_WIDTH - 6, BOX_HEIGHT - 6, BOX_COLOUR);

    // Select Font
    tumFontSelectFontFromName("SF Automaton.ttf");

    // Draw Text
    if (!tumGetTextSize((char *)str, &text_width, NULL)) {
        tumDrawText(str, pos.x + BOX_WIDTH/2 - text_width/2, 
                         pos.y + BOX_HEIGHT/2 - BOX_FONT_SIZE/3 , BOX_FRAME_COLOUR);
        
    }           

    // Reset Font
    tumFontSelectFontFromHandle(cur_font);
    tumFontPutFontHandle(cur_font);

    return 1;
}