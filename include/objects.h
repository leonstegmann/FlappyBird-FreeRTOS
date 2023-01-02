#ifndef __OBJECTS_H__
#define __OBJECTS_H__

/* Standard library includes */
#include <stdbool.h>

/* TUM_Library includes  */
#include "TUM_Draw.h"

#define Gravity 0.8

typedef struct Bird {
//    image_handle_t image; 
    unsigned short height;
    unsigned short width;
    coord_t pos;
    double velocityY;
} bird_t;

bird_t player = {.height = 34, .width = 34 , .velocityY = 0, .pos = (coord_t) {SCREEN_WIDTH/2 - 34/2, SCREEN_HEIGHT - 300} };

#endif // __OBJECTS_H__