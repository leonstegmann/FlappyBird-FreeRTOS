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
#define IP4_ADDR_LEON_VM "192.168.0.68" // Leons VM
#define IP4_ADDR_MANJARO "192.168.0.120" // Leons old Computer running Manjaro
#define LOCAL_HOST_IP "127.0.0.1"

#define UDP_BUFFER_SIZE 2000

static aIO_handle_t slave_UDP_handle = NULL;

void slaveRecv(size_t recv_size, char *buffer, void *args){

    //char recv_val[10] = *buffer;
    int recv_val = *((int*) buffer);

    printf(" Slave Received %ld (bytes): %d\n", recv_size, recv_val);


}

void slaveSend(int send_val){
    /* Sending via UDP from Slave to Master*/
    if(aIOSocketPut(UDP, IP4_ADDR_LEON_VM, MISO_Port, (char *) &send_val, sizeof(send_val)))
        PRINT_ERROR("FAILED TO SEND from SLAVE");
}

void initUDPConnectionSlave(){
    /* Opening UDP connection */
    slave_UDP_handle = aIOOpenUDPSocket((char*) LOCAL_HOST_IP, MOSI_Port, UDP_BUFFER_SIZE, slaveRecv, NULL );
    if(slave_UDP_handle == NULL){
        PRINT_ERROR("FAILED TO OPEN SLave UDP Socket");
        exit(EXIT_FAILURE);
    }
}
