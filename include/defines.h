#ifndef __DEFINES_H__
#define __DEFINES_H__

#include "TUM_Draw.h" //for coord_t

/*Screen middle*/
#define  SCREEN_CENTER (coord_t) {SCREEN_WIDTH/2 , SCREEN_HEIGHT/2} 

#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)

/* Resource Paths */
#define LOWER_PIPE_FILENAME "bottomPipe.png"
#define UPPER_PIPE_FILENAME "topPipe.png"

/* Bird colours */
#define YELLOW 1
#define RED 2
#define BLUE 3

#endif //__DEFINES_H__