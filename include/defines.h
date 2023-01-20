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

/* our Ip Adresses for Testing */
#define IP4_HOST_ADDR "192.168.0.68" // Leons VM : "192.168.0.68"
#define IP4_SLAVE_ADDR "192.168.0.120" // Leons Manjaro: "192.168.0.120"

#endif //__DEFINES_H__