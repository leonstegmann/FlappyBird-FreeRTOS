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

bird_t player;

#endif // __OBJECTS_H__