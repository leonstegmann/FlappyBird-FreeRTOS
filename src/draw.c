#include "TUM_Draw.h"
#include <stdio.h>

#include "draw.h"

#define LOGO_FILENAME "flappybird.png"
#define BOX_WIDTH 100
#define BOX_HEIGHT 50
#define BOX_COLOUR Red
#define BOX_FRAME_COLOUR Black

image_handle_t logo_image = NULL;

int drawBackround() {
    static int image_height;

    if (logo_image == NULL) {
        logo_image = tumDrawLoadImage(LOGO_FILENAME);
    }

    if ((image_height = tumDrawGetLoadedImageHeight(logo_image)) != -1) {
        tumDrawLoadedImage(logo_image, 0, 0);
        return 1;

    } else { 
        printf("Failed to get size of image '%s', does it exist?\n",
                LOGO_FILENAME);
        return 0;
    }
}

int drawButton(coord_t pos, char *text) {

    tumDrawFilledBox(pos.x, pos.y, BOX_WIDTH, BOX_HEIGHT, BOX_COLOUR);
    tumDrawBox(pos.x, pos.y, BOX_WIDTH, BOX_HEIGHT, BOX_FRAME_COLOUR);

    return 0;
}