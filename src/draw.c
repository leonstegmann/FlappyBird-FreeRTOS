#include "TUM_Draw.h"
#include <stdio.h>

#include "draw.h"

#define LOGO_FILENAME "flappybird.png"

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
