#ifndef __OBJECTS_H__
#define __OBJECTS_H__

/* Standard library includes */
#include <stdbool.h> // for bool

/* FreeRTOS includes  */
#include "semphr.h" // for .lock

/* TUM_Library includes  */
#include "TUM_Draw.h" // for image_handle_t

/* Defines */
/* ATTENTION: THESE DEFINES ARE RANDOMLY SET */
#define GRAVITY 6           //is added to the bird velocity in Y-direction (downwards) each time UpdateBirdPosition is called.
#define UPWARDS_PUSH 100     //is added to the bird velocity in Y-direction (upwards) when pressing spacebar.
#define PIPE_VELOCITY 300
#define GAP_HEIGHT 100
#define FLOOR_HEIGHT 50
#define GAP_LOWEST 150
#define GAP_HIGHEST 220

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

typedef struct Pipes {
    image_handle_t lowerPipeImage;
    image_handle_t upperPipeImage; 
    unsigned short image_height;
    unsigned short image_width;
    short positionX; //upper left corner of the image
    double velocityX;
    short gap_center; // Y-position of the Gap middle on the screen
    SemaphoreHandle_t lock;
} pipes_t;


/**
 * @brief create a pipe object returning the corresponding pointer. 
 * @return pointer to the created Pipe Object.
 */
pipes_t* newPipe();

/**
 * @brief updates the Pipes position. 
 * @return returns 1 on success 0 if failed.
 */
short updatePipePosition( TickType_t, pipes_t*);

/**
 * @brief generates a Random Number in a specific range  
 * @param min lower range end
 * @param max higher range end
 * @return returns the random number
 */
short randomGenerator(short, short);

#endif // __OBJECTS_H__