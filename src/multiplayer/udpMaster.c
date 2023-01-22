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

static aIO_handle_t master_UDP_handle = NULL;

static TaskHandle_t MasterTask = NULL;

void masterRecv(size_t recv_size, char *buffer, void *args){

}

void initUDPConnectionMaster(){
    if(master_UDP_handle == NULL){
        master_UDP_handle = aIOOpenUDPSocket(NULL, MISO_Port, UDP_BUFFER_SIZE, masterRecv, NULL );
        printf("Opened Master Connection\n");
    }
    if(master_UDP_handle == NULL){  
        PRINT_ERROR("FAILED TO OPEN SLave UDP Socket");
        exit(EXIT_FAILURE);
    }
    if (xSemaphoreTake(ip_and_port.lock, portMAX_DELAY) == pdTRUE) {
        ip_and_port.port_in = MISO_Port;
        ip_and_port.port_out = MOSI_Port;
        xSemaphoreGive(ip_and_port.lock);
    }
}

void closeUDPConnectionMaster(){
    if(master_UDP_handle != NULL){
        aIOCloseConn(master_UDP_handle);
        printf("Closed Master Connection\n");
    }
}
void masterSend(){

    int send_val = rand()%10;

    /* Sending via UDP from Master to Slave*/
    if(aIOSocketPut(UDP, IP4_SLAVE_ADDR, MOSI_Port, (char *) &send_val, sizeof(send_val))){
        PRINT_ERROR("FAILED TO SEND from MASTER");
    }
    else {
        printf("SEDNING: (random number) %d\n", send_val); 
    }

}


void vMasterTask(void *pvParameters){

    /* for Receiving on Master */
    initUDPConnectionMaster(); 

    while (1) {
        masterSend();
        vTaskDelay(500);
    }
    
}


void createMasterTask(){
    if (xTaskCreate(vMasterTask, "MasterTask", mainGENERIC_STACK_SIZE * 2,
                    NULL, mainGENERIC_PRIORITY + 7, &MasterTask) != pdPASS) {
        PRINT_ERROR("Failed to create FreeRTOS Task");
        exit(EXIT_FAILURE);
    }
}

void deleteMasterTask(){
    if( MasterTask != NULL)
        vTaskDelete(MasterTask);
}

