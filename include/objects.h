#ifndef __OBJECTS_H__
#define __OBJECTS_H__

/* Standard library includes */
#include <stdbool.h>

/* FreeRTOS includes  */
#include "semphr.h" // for .lock

/* TUM_Library includes  */
#include "TUM_Draw.h"

#define GRAVITY 0.8


typedef struct Bird {
//    image_handle_t image; 
    unsigned short height;
    unsigned short width;
    coord_t pos;
    double velocityY;
    SemaphoreHandle_t lock;
} bird_t;


extern bird_t* player;

void updateBirdPosition( TickType_t);

int initPlayer();

#endif // __OBJECTS_H__