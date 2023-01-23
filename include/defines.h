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

/* using Serial Peripheral Interface (SPI)*/
#define MISO_Port 1234 // Master in Slave out
#define MOSI_Port 4321 // Master out Slave in

/* Number of bytes to be reserved as a buffer for the UDP connection */
#define UDP_BUFFER_SIZE 2000

/* our Ip Adresses for Testing */
#define IP4_HOST_ADDR "192.168.0.68" // Leons VM : "192.168.0.68"
#define IP4_SLAVE_ADDR "192.168.0.120" // Leons Manjaro: "192.168.0.120"

/* char Defines for the arrow key Buttons*/
#define NONE 'N'
#define LEFT 'L'
#define RIGHT 'R'
#define UP 'U'
#define DOWN 'D'

#endif //__DEFINES_H__
