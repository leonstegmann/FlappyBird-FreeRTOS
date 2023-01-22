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

/* Master UDP Socket for Receiving Data */
static aIO_handle_t master_UDP_handle = NULL;

/* Master Task running and Sending Data*/
static TaskHandle_t MasterTask = NULL;

void masterSend(char* ip_addr, int send_val){

    /* Sending via UDP from Master to Slave*/
    if(aIOSocketPut(UDP, (char *)  ip_addr, ip_and_port.port_out, (char *) &send_val, sizeof(send_val))){
        PRINT_ERROR("FAILED TO SEND from MASTER");
    }
    else {
        printf("Master SEDNING: (random number) %d\n", send_val); 
    }

}

void masterRecv(size_t recv_size, char *buffer, void *args){

    int recv_val = *((int*) buffer);

    printf(" Master Received %ld (bytes): %d\n", recv_size, recv_val);

}

void initUDPConnectionMaster(){
    if(master_UDP_handle == NULL){
        if (xSemaphoreTake(ip_and_port.lock, portMAX_DELAY) == pdTRUE) {
            ip_and_port.port_in = MISO_Port;
            ip_and_port.port_out = MOSI_Port;
            master_UDP_handle = aIOOpenUDPSocket(NULL, ip_and_port.port_in, UDP_BUFFER_SIZE, masterRecv, NULL );
            printf("Opened Master Connection on Localhost\n");
            xSemaphoreGive(ip_and_port.lock);
        }
    }
    if(master_UDP_handle == NULL){  
        PRINT_ERROR("FAILED TO OPEN SLave UDP Socket");
        exit(EXIT_FAILURE);
    }
}

void vMasterTask(void *pvParameters){

    char ip_str[12] = " "; // saves the ip adress locally to safe resources ad value is use permanently

    /* for Receiving on Master */
    initUDPConnectionMaster(); 

    if (xSemaphoreTake(ip_and_port.lock, portMAX_DELAY) == pdTRUE) {
        sprintf(ip_str,"%u.%u.%u.%u", ip_and_port.IP4[0],ip_and_port.IP4[1],ip_and_port.IP4[2],ip_and_port.IP4[3]);
        printf("Opened Master Connection to %s\n",ip_str);
        while (1) {
            masterSend(ip_str, rand()%10);
            vTaskDelay(1000);
        }
    }
}

void createMasterTask(){
    if(MasterTask == NULL)
        if (xTaskCreate(vMasterTask, "MasterTask", mainGENERIC_STACK_SIZE * 2,
                        NULL, mainGENERIC_PRIORITY + 7, &MasterTask) != pdPASS) {
            PRINT_ERROR("Failed to create FreeRTOS Task");
            exit(EXIT_FAILURE);
        }
}

void deleteMasterTask(){
    if( MasterTask != NULL)
        vTaskDelete(MasterTask);
    if(master_UDP_handle != NULL)
        aIOCloseConn(master_UDP_handle);
    printf("Closed Master Connection and Deleted Master Task\n");
    MasterTask = NULL;
    master_UDP_handle = NULL;
    xSemaphoreGive(ip_and_port.lock);
}

