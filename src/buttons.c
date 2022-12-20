/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include "semphr.h" // for buttons.lock
#include"task.h" // for xTaskGetTickCount()
#include <SDL2/SDL_scancode.h>  // Defines keyboard scancodes

/* TUM_Library includes  */
#include"TUM_Draw.h"    // for coord_t
#include "TUM_Event.h"  // for xQueueReceive()
#include "TUM_Print.h" // for PRINT_ERROR()
 
 /* Project includes  */
#include "buttons.h"

/**
 * @brief stores the currrent state of every button
 *      and has a SemaphoreHandle to be locked
 */
typedef struct buttons_buffer {
    unsigned char prevState[SDL_NUM_SCANCODES]; /**to be able to compare current with previous state*/
    TickType_t lastTimePressed[SDL_NUM_SCANCODES]; /** to ensure debounce time */
    unsigned char currentState[SDL_NUM_SCANCODES]; /** current button state */
    char id[SDL_NUM_SCANCODES]; /* the button name */
    coord_t pos[SDL_NUM_SCANCODES]; /* position of the button on the GUI */
    unsigned int colour[SDL_NUM_SCANCODES]; /**< Hex RGB colour */
    SemaphoreHandle_t lock; /* FreeRTOS Semaphore handle*/
} buttons_buffer_t;

/* Instantiating and initializing the buttons Object */
buttons_buffer_t buttons = { 0 };

void xGetButtonInput(void)
{
	if (xSemaphoreTake(buttons.lock, 0) == pdTRUE) {
		xQueueReceive(buttonInputQueue, &buttons.currentState, 0);
		xSemaphoreGive(buttons.lock);
	}
}

int buttonsInit(void) {
    buttons.lock = xSemaphoreCreateMutex(); // Locking mechanism
    if (!buttons.lock) {
        PRINT_ERROR("Failed to create buttons lock");
        return -1;
    }

    return 0;
}

void buttonsExit(void) {
    vSemaphoreDelete(buttons.lock);
}


void setDisplayedButtonName(char id, int keyvalue){
    buttons.id[keyvalue]= id;
}

void setButtonPosition(coord_t pos, int keyvalue){
    buttons.pos[keyvalue] = pos;
}

void setButtonColour(unsigned int colour, int keyvalue){
    buttons.colour[keyvalue] = colour;
}

int checkButton(int keyvalue){
    int ret = 0;
    if (xSemaphoreTake(buttons.lock, 0) == pdTRUE) {
        if (buttons.currentState[keyvalue]) { // Equiv to SDL_SCANCODE_Q
            TickType_t now = xTaskGetTickCount();
            if ( buttons.currentState[keyvalue] > 0 && buttons.prevState[keyvalue] == 0){
                if ((now - buttons.lastTimePressed[keyvalue]) > DEBOUNCEDELAY ){
                    buttons.lastTimePressed[keyvalue] = now;
                    ret = 1;
                }
            }
        }
        xSemaphoreGive(buttons.lock);
		}
    buttons.prevState[keyvalue] = buttons.currentState[keyvalue];
    return ret;
}
