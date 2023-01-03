#ifndef __OBJECTS_H__
#define __OBJECTS_H__

/* Standard library includes */
#include <stdbool.h> // for bool

/* FreeRTOS includes  */
#include "semphr.h" // for .lock

/* TUM_Library includes  */
#include "TUM_Draw.h" // for image_handle_t

/* Defines */
#define GRAVITY 2           //is added to the bird velocity in Y-direction (downwards) each time UpdateBirdPosition is called.
#define UPWARDS_PUSH 60     //is added to the bird velocity in Y-direction (upwards) when pressing spacebar.
#define GROUND ( SCREEN_HEIGHT - 100)

typedef struct Bird {
//    image_handle_t image; 
    unsigned short height;
    unsigned short width;
    coord_t pos;
    double velocityY;
    SemaphoreHandle_t lock;
    bool dead;
} bird_t;

/* Pointer to Bird Object of Player */
extern bird_t* player;

/**
 * @brief Updates the players position and velocity each time the function is called.
 * @param xLastTimeUpdated is the time the function got called the last time. 
 * This is used to calculatethe distance the bird travelt and is thus actualized.
 */
void updateBirdPosition( TickType_t);

/**
 * @brief creates the SemaphoreMutex() for the player.lock to ensure safe read and write on shared resource.
 */
int initPlayer();

/**
 * @brief check Collision of the Bord with the Floor.
 * @return 1 is collision = TRUE, 0 if collision = FALSE.
 */
short checkCollision();

#endif // __OBJECTS_H__