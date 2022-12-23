#include <stdio.h>

#include "TUM_Draw.h" 
#include "TUM_Font.h"

#include "draw.h"

/* Filenames */
#define BACKGROUND_FILENAME "background.png"
#define LOGO_FILENAME "FlappyBird.png"

/* Definitions for drawButton */
#define BOX_COLOUR Teal
#define BOX_FRAME_COLOUR Silver
#define BOX_OUTLINE_COLOUR Black
#define BUTTON_TEXT_COLOUR White


image_handle_t backroundImage = NULL;
image_handle_t logoImage = NULL;

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

int drawLogo(coord_t pos) {
    static int image_height;
    static int image_width;

    if (logoImage == NULL) {
        logoImage = tumDrawLoadImage(LOGO_FILENAME);
    }

    if ((image_height = tumDrawGetLoadedImageHeight(logoImage)) != -1) {
        image_width = tumDrawGetLoadedImageWidth(logoImage);
        tumDrawLoadedImage(logoImage, pos.x - image_width/2, pos.y - image_height/2);
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
    tumDrawFilledBox(pos.x , pos.y, BOX_WIDTH, BOX_HEIGHT + 3, BOX_OUTLINE_COLOUR);
    
    // Draw Box with Frame
    tumDrawFilledBox(pos.x + 2, pos.y + 2, BOX_WIDTH - 4, BOX_HEIGHT - 4, BOX_FRAME_COLOUR);
    tumDrawFilledBox(pos.x + 4, pos.y + 4, BOX_WIDTH - 8, BOX_HEIGHT - 8, BOX_COLOUR);

    // Select Font
    tumFontSelectFontFromName(BUTTON_FONT);

    // Draw Text
    if (!tumGetTextSize((char *)str, &text_width, NULL)) {
        tumDrawText(str, pos.x + BOX_WIDTH/2 - text_width/2, 
                         pos.y + BOX_HEIGHT/2 - BUTTON_FONT_SIZE/6*3 , BUTTON_TEXT_COLOUR);
        
    }           

    // Reset Font
    tumFontSelectFontFromHandle(cur_font);
    tumFontPutFontHandle(cur_font);

    return 1;
}