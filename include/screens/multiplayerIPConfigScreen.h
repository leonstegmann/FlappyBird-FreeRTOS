#ifndef __IPConfig_H_
#define __IPConfig_H_

typedef struct ip_port {
    SemaphoreHandle_t lock;
    unsigned char IP4[4];
    unsigned int port_in;
    unsigned int port_out;
} ip_port_t; 

extern ip_port_t ip_and_port;

/**
 * @brief Creates the FreeRTOS Task.
 * @return Returns 0 on Succes, 1 on Error.
 */
int createIPConfigTask();

/* Enter and Exit functions for the State Machine */
void deleteIPConfigTask();


#endif // __IPConfig_H_