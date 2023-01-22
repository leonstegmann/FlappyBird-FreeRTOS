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

#include "AsyncIO.h"

/* using Serial Peripheral Interface (SPI)*/
#define MISO_Port 1234 // Master in Slave out
#define MOSI_Port 4321 // Master out Slave in

#define UDP_BUFFER_SIZE 2000

static aIO_handle_t slave_UDP_handle = NULL;

void slaveRecv(size_t recv_size, char *buffer, void *args){

    int recv_val = *((int*) buffer);

    printf(" Slave Received %ld (bytes): %d\n", recv_size, recv_val);

    slaveSend(recv_val);

}

void slaveSend(int send_val){
    /* Sending via UDP from Slave to Master*/
    if(aIOSocketPut(UDP, IP4_HOST_ADDR, MISO_Port, (char *) &send_val, sizeof(send_val))){
        PRINT_ERROR("FAILED TO SEND from SLAVE");
    }
    else {
        printf("Slave SEDNING: %d\n", send_val); 
    }

}

void initUDPConnectionSlave(){
    /* Opening UDP connection */
    if(slave_UDP_handle == NULL){
        if (xSemaphoreTake(ip_and_port.lock, portMAX_DELAY) == pdTRUE) {
            ip_and_port.port_in = MOSI_Port;
            ip_and_port.port_out = MISO_Port; 
            slave_UDP_handle = aIOOpenUDPSocket(NULL, MOSI_Port, UDP_BUFFER_SIZE, slaveRecv, NULL );
            printf("Opened Slave Connection\n");
            xSemaphoreGive(ip_and_port.lock);
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
}