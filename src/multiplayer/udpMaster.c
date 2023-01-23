/* Standard library includes */
#include "stdio.h" // for sprintf()
#include "stdlib.h" // for malloc()

/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include"task.h" // for Task handles
#include "semphr.h" // for Semaphore Handles
#include <SDL2/SDL_scancode.h>  // Defines keyboard scancodes

/* TUM_Library includes  */
#include "TUM_FreeRTOS_Utils.h" //for tumFUtilPrintTaskStateList()
#include "TUM_Print.h" // for PRINT_ERROR()
#include "AsyncIO.h" // UDP Connection includes

/* Project includes  */
#include "multiplayerIPConfigScreen.h" // for IP and Port
#include"draw.h" // for Positioning
#include "defines.h" // for Global Defines
#include "objects.h" // for players and pipes
#include "udpSlave.h" // for package Slave Struct
#include "udpMaster.h" // for package Master Struct

/* Master UDP Socket for Receiving Data */
static aIO_handle_t master_UDP_handle = NULL;

/* Master Task running and Sending Data*/
static TaskHandle_t MasterTask = NULL;

void masterSend(char* ip_addr, tx_packageM_t* send_data){

    long int send_size = sizeof(send_data);

    /* Sending via UDP from Master to Slave*/
    if(aIOSocketPut(UDP, (char *)  ip_addr, ip_and_port.port_out, (char *) send_data, send_size)){
        PRINT_ERROR("FAILED TO SEND from MASTER");
    }
    else {
        printf("Bird 1 pos Y : %u\n" , send_data->bird_pos_Y);

        printf("Master SEDNING %ld (bytes)\n", send_size); 
    }

}

void unpackRxPackageMaster(char* buffer){

    tx_packageS_t* tmp_package = (tx_packageS_t*) buffer;
    
    printf("Unpacking Data\n");
    printf("Bird 2 pos %u\n", tmp_package->bird_pos_Y);
    
    if(xSemaphoreTake(player2->lock,portMAX_DELAY)==pdTRUE){
        player2->pos.y = tmp_package->bird_pos_Y;
        xSemaphoreGive(player2->lock);
    }     
}

void masterRecv(size_t recv_size, char *buffer, void *args){

    printf(" Master Received %ld (bytes)\n", recv_size);

    /* Unpack received Data */ 
    unpackRxPackageMaster(buffer);

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

tx_packageM_t packTxPackageMaster(){
    
    tx_packageM_t tmp_package;

    if(xSemaphoreTake(player1->lock,portMAX_DELAY)==pdTRUE){
        tmp_package.bird_pos_Y = player1->pos.y;
        xSemaphoreGive(player1->lock);
    }   
    if(xSemaphoreTake(pipe1->lock, portMAX_DELAY)==pdTRUE){
        tmp_package.pipe_pos[0].x = pipe2->positionX;
        tmp_package.pipe_pos[0].y = pipe2->gap_center;
        xSemaphoreGive(pipe1->lock);
    }
    if(xSemaphoreTake(pipe2->lock, portMAX_DELAY)==pdTRUE){
        tmp_package.pipe_pos[1].x = pipe2->positionX;
        tmp_package.pipe_pos[1].y = pipe2->gap_center;
        xSemaphoreGive(pipe2->lock);
    }
    return tmp_package;
}

void vMasterTask(void *pvParameters){

    char ip_str[20] = " "; // saves the ip adress locally to safe resources ad value is use permanently

    /* for Receiving on Master */
    initUDPConnectionMaster(); 

    if (xSemaphoreTake(ip_and_port.lock, portMAX_DELAY) == pdTRUE) {
        sprintf(ip_str,"%u.%u.%u.%u", ip_and_port.IP4[0],ip_and_port.IP4[1],ip_and_port.IP4[2],ip_and_port.IP4[3]);
        printf("Opened Master Connection to %s\n",ip_str);

        while (1) {

            /* Pack Data for Sending*/
            tx_packageM_t tmp_data = packTxPackageMaster();

            /* Sending Data */
            masterSend(ip_str, &tmp_data);

            vTaskDelay(10);
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
    tumFUtilPrintTaskStateList();
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

