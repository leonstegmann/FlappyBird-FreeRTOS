#ifndef __IPConfig_H_
#define __IPConfig_H_

#define NONE 'N'
#define LEFT 'L'
#define RIGHT 'R'
#define UP 'U'
#define DOWN 'D'

#define IPv4_addr "192.168.0.68" // my Ip Adress

typedef struct ip_port {
    SemaphoreHandle_t lock;
    unsigned char IP[4];
    unsigned int port;
} ip_port_t; 

extern ip_port_t ip_and_port;

/**
 * @brief Creates the FreeRTOS Task.
 * @return Returns 0 on Succes, 1 on Error.
 */
int createIPConfigTask();

/* Enter and Exit functions for the State Machine */
void deleteIPConfigTask();
void enterIPConfigTask();
void exitIPConfigTask();


#endif // __IPConfig_H_