/* Standard library includes */
#include "stdio.h" // for sprintf()

/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include"task.h" // for Task_handle
#include "semphr.h"
#include <SDL2/SDL_scancode.h>  // Defines keyboard scancodes

/* TUM_Library includes  */
#include "TUM_Draw.h"
#include "TUM_Event.h" // for FETCH_EVENT_NONBLOCK
#include "TUM_Print.h" // for PRINT_ERROR
#include "TUM_FreeRTOS_Utils.h" //for tumFUtilPrintTaskStateList

/* Project includes  */
#include "multiplayerIPConfigScreen.h"
#include"draw.h" // for Positioning
#include "defines.h"
#include "buttons.h" // for vCheckArrowInput
#include "udpSlave.h"

#include "AsyncIO.h"

/* using Serial Peripheral Interface (SPI)*/
#define MISO_Port 1234 // Master in Slave out
#define MOSI_Port 4321 // Master out Slave in

#define UDP_BUFFER_SIZE 2000

static aIO_handle_t slave_UDP_handle = NULL;

void slaveSend(char* ip_addr, int send_val){

    /* Sending via UDP from Slave to Master*/
    if(aIOSocketPut(UDP, ip_addr, ip_and_port.port_out, (char *) &send_val, sizeof(send_val))){
        PRINT_ERROR("FAILED TO SEND from SLAVE");
    }
    else {
        printf("Slave SEDNING: %d\n", send_val); 
    }

}

void slaveRecv(size_t recv_size, char *buffer, void *args){
    char* ip_str = (char*) args ;
    int recv_val = *((int*) buffer);

    printf(" Slave Received %ld (bytes): %d\n", recv_size, recv_val);
    slaveSend( (char*) ip_str, recv_val);

}

void initUDPConnectionSlave(){
    
    char ip_str[12] = " ";
    /* Opening UDP connection */
    if(slave_UDP_handle == NULL){
        if (xSemaphoreTake(ip_and_port.lock, portMAX_DELAY) == pdTRUE) {
            ip_and_port.port_in = MOSI_Port;
            ip_and_port.port_out = MISO_Port; 
            sprintf(ip_str,"%u.%u.%u.%u", ip_and_port.IP4[0],ip_and_port.IP4[1],ip_and_port.IP4[2],ip_and_port.IP4[3]);
            printf("Opened Slave Connection on Localhost\n");
            slave_UDP_handle = aIOOpenUDPSocket(NULL, ip_and_port.port_in, UDP_BUFFER_SIZE, slaveRecv, (void*) &ip_str);
            printf("Opened Slave Connection to %s\n", ip_str);
        }
    }
    if(slave_UDP_handle == NULL){
        PRINT_ERROR("FAILED TO OPEN SLave UDP Socket");
        exit(EXIT_FAILURE);
    }
}

void closeUDPConnectionSlave(){
    if(slave_UDP_handle != NULL){
        aIOCloseConn(slave_UDP_handle);
        printf("Closed Slave Connection\n");
    }
    slave_UDP_handle = NULL;
    xSemaphoreGive(ip_and_port.lock);
}