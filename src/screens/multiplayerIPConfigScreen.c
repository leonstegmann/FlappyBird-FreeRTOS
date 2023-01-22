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
#include"draw.h"
#include "defines.h"
#include "buttons.h" // for vCheckArrowInput
#include "main.h" //for DrawSignal
#include "udpMaster.h"
#include "udpSlave.h"
#include "states.h"

static TaskHandle_t IPScreen = NULL;

ip_port_t ip_and_port =  { .lock = NULL, .IP4 = { 10, 181, 72, 199 }, .port_in = 12345 };


#define OCTET_Y SCREEN_HEIGHT * 4/10
#define FIRST_OCTET SCREEN_WIDTH / 2 - 100
#define SECOND_OCTET SCREEN_WIDTH / 2 - 70
#define THIRD_OCTET SCREEN_WIDTH / 2 - 40
#define FOURTH_OCTET SCREEN_WIDTH / 2 - 10
#define PORT_X SCREEN_WIDTH / 2 + 30

void vDrawIP(unsigned char IP[4], unsigned int port)
{
    char buff[5];
    sprintf(buff, "%u", IP[0]);
    tumDrawText(buff, FIRST_OCTET, OCTET_Y, Black);
    tumDrawText(".", FIRST_OCTET + 20, OCTET_Y, Black);
    sprintf(buff, "%u", IP[1]);
    tumDrawText(buff, SECOND_OCTET, OCTET_Y, Black);
    tumDrawText(".", SECOND_OCTET + 27, OCTET_Y, Black);
    sprintf(buff, "%u", IP[2]);
    tumDrawText(buff, THIRD_OCTET, OCTET_Y, Black);
    tumDrawText(".", THIRD_OCTET + 20, OCTET_Y, Black);
    sprintf(buff, "%u", IP[3]);
    tumDrawText(buff, FOURTH_OCTET, OCTET_Y, Black);
    tumDrawText(":", FOURTH_OCTET + 30, OCTET_Y, Black);
    sprintf(buff, "%u", port);
    tumDrawText(buff, PORT_X, OCTET_Y, Black);


    sprintf(buff, "ip");
    tumDrawText(buff, SCREEN_WIDTH / 2 - 50, OCTET_Y - 20, Black);
    sprintf(buff, "port");
    tumDrawText(buff, SCREEN_WIDTH / 2 + 30, OCTET_Y - 20, Black);
}

#define ARROW_OFFSET 50
#define ARROW_HEIGHT 10
#define ARROW_WIDTH 10

void vDrawArrow(char orientation, coord_t point)
{
    switch (orientation) {
        case UP: {
            coord_t points[3] = {
                { point.x, point.y },
                { point.x - ARROW_WIDTH / 2, point.y + ARROW_HEIGHT },
                { point.x + ARROW_WIDTH / 2, point.y + ARROW_HEIGHT }
            };
            tumDrawTriangle(points, Black);
        } break;
        case DOWN: {
            coord_t points[3] = {
                { point.x, point.y },
                { point.x - ARROW_WIDTH / 2, point.y - ARROW_HEIGHT },
                { point.x + ARROW_WIDTH / 2, point.y - ARROW_HEIGHT }
            };
            tumDrawTriangle(points, Black);
        } break;
    }
}

void drawArrows(unsigned int selected_octet){
    switch (selected_octet) {
                case 0: {
                    coord_t up = { FIRST_OCTET, OCTET_Y - ARROW_OFFSET };
                    vDrawArrow(UP, up);
                    coord_t down = { FIRST_OCTET, OCTET_Y + ARROW_OFFSET };
                    vDrawArrow(DOWN, down);
                } break;
                case 1: {
                    coord_t up = { SECOND_OCTET, OCTET_Y - ARROW_OFFSET };
                    vDrawArrow(UP, up);
                    coord_t down = { SECOND_OCTET, OCTET_Y + ARROW_OFFSET };
                    vDrawArrow(DOWN, down);
                } break;
                case 2: {
                    coord_t up = { THIRD_OCTET, OCTET_Y - ARROW_OFFSET };
                    vDrawArrow(UP, up);
                    coord_t down = { THIRD_OCTET, OCTET_Y + ARROW_OFFSET };
                    vDrawArrow(DOWN, down);
                } break;
                case 3: {
                    coord_t up = { FOURTH_OCTET, OCTET_Y - ARROW_OFFSET };
                    vDrawArrow(UP, up);
                    coord_t down = { FOURTH_OCTET, OCTET_Y + ARROW_OFFSET };
                    vDrawArrow(DOWN, down);
                } break;
                case 4: {
                    coord_t up = { PORT_X, OCTET_Y - ARROW_OFFSET };
                    vDrawArrow(UP, up);
                    coord_t down = { PORT_X, OCTET_Y + ARROW_OFFSET };
                    vDrawArrow(DOWN, down);
                } break;
                default:
                    break;
            }             
}

#define SCROLL_DELAY 50

void vIPScreen(void *pvParameters)
{
    char host_or_guest[20];
    sprintf( host_or_guest, " " );

    ip_port_t tmp_ip_and_port = ip_and_port;

    bool connected = false;

    TickType_t xLastFrameTime = xTaskGetTickCount();

    unsigned int selected_octet = 0;

    TickType_t prev_button_press = xTaskGetTickCount();

    while (1) {
        tumEventFetchEvents(FETCH_EVENT_NONBLOCK); // Query events backend for new events, ie. button presses
        
        if ((xTaskGetTickCount() - prev_button_press) >
            pdMS_TO_TICKS(SCROLL_DELAY)) {
            if (xSemaphoreTake(ip_and_port.lock, 0) == pdTRUE) {
                switch (vCheckArrowInput()) {
                    case UP:
                        if (selected_octet == 4) {
                            ip_and_port.port_in++;
                        }
                        else
                            ip_and_port.IP4[selected_octet] =
                                (ip_and_port.IP4
                                 [selected_octet] +
                                 1) %
                                255;
                        break;
                    case DOWN:
                        if (selected_octet == 4) {
                            ip_and_port.port_in--;
                        }
                        else
                            ip_and_port.IP4[selected_octet] =
                                (ip_and_port.IP4
                                 [selected_octet] -
                                 1) %
                                255;
                        break;
                    case LEFT:
                        selected_octet =
                            (selected_octet - 1) % 5;
                        break;
                    case RIGHT:
                        selected_octet =
                            (selected_octet + 1) % 5;
                        break;
                    default:
                        break;
                }
                xSemaphoreGive(ip_and_port.lock);
            }
            prev_button_press = xTaskGetTickCount();
        }
        tmp_ip_and_port = ip_and_port; //saving last set ip and port
        if(DrawSignal)
            if(xSemaphoreTake(DrawSignal, portMAX_DELAY) == pdTRUE) {

            tumDrawClear(White); // Clear screen

            /* DRAW */
            drawBackround();
            drawMultiplayerLogo(LOGO_POSITION);
            drawButton(LOWER_LEFT_BUTTON_POSITION, "Back", xLastFrameTime);
            drawButton(MIDDLE_BUTTON_POSITION, "Host", xLastFrameTime); // Master
            drawButton(LOWER_RIGHT_BUTTON_POSITION,"Guest", xLastFrameTime); // Slave
            tumDrawText( host_or_guest, MIDDLE_BUTTON_POSITION.x - 30, MIDDLE_BUTTON_POSITION.y - 50, Orange);
            drawFPS();
            if(connected){
                tumDrawText("CONNECTED!",RIGHT_BUTTON_POSITION.x, RIGHT_BUTTON_POSITION.y, Gray);
                drawButton(LOWER_RIGHT_BUTTON_POSITION,"Play", xLastFrameTime); // Slave
            }

            /* DRAW IP*/
            if (xSemaphoreTake(ip_and_port.lock, 0) == pdTRUE) {
                vDrawIP(ip_and_port.IP4, ip_and_port.port_in);
                xSemaphoreGive(ip_and_port.lock);
                /* DRAW ARROWS*/
                drawArrows(selected_octet);
            }
            else {
                vDrawIP(tmp_ip_and_port.IP4,tmp_ip_and_port.port_in);
            }
        } //END drawSignal
            

        /* Check Buttons*/
        xGetButtonInput();
        if(checkButton(KEYCODE(H))){
            printf("Host\n");
            sprintf(host_or_guest,"YOU ARE HOST NOW");
            closeUDPConnectionSlave();
            createMasterTask();
        }        
        if(checkButton(KEYCODE(G))){
            printf("Guest\n");
            sprintf(host_or_guest,"YOU ARE GUEST NOW");
            deleteMasterTask();
            initUDPConnectionSlave();
        }
        if (checkButton(KEYCODE(B))){
            deleteMasterTask();
            closeUDPConnectionSlave();
            states_set_state(0);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


int createIPConfigTask(void) {

    printf("Creating IP config\n");
    
    ip_and_port.lock = xSemaphoreCreateMutex();
    if (!ip_and_port.lock) {
        PRINT_ERROR("Failed to create ip and port lock");
        return 1;
    }

    if(!xTaskCreate(vIPScreen, "MultiplayerScreen",  mainGENERIC_STACK_SIZE *2, NULL,
			        mainGENERIC_PRIORITY + 6, &IPScreen)) {
                        return 1;
    }

    printf("Entering IP config\n");

    tumFUtilPrintTaskStateList();

    return 0;
}

void deleteIPConfigTask(){
    if (IPScreen){
        vTaskDelete(IPScreen);
        IPScreen = NULL;
    }
    if(ip_and_port.lock)
        vSemaphoreDelete(ip_and_port.lock);
    
}
