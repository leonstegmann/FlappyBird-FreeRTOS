/* Standard library includes */
#include <math.h>
#include <stdio.h>
#include <stdlib.h> // for atexit

/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

/* other library includes*/
#include "AsyncIO.h" // for aIODeinit
#include <SDL2/SDL_scancode.h> // for KEYCODE() : Defines keyboard scancodes

/* TUM_Library includes  */
#include "TUM_FreeRTOS_Utils.h"
#include "TUM_Draw.h"
#include "TUM_Utils.h"
#include "TUM_Event.h"
#include "TUM_Font.h"
#include "TUM_Sound.h"

/* Project includes */
#include "main.h"
#include "swapBuffers.h"
#include "draw.h"
#include "buttons.h"
#include "menuScreen.h" // for the Flappy Bird Logo
#include "stateMachine.h"
#include "objects.h"

#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)

TaskHandle_t BufferSwap = NULL;

SemaphoreHandle_t DrawSignal = NULL;

QueueHandle_t ColourQueue = NULL;

int main(int argc, char *argv[])
{
    char *bin_folder_path = tumUtilGetBinFolderPath(argv[0]);

	printf("Initializing: \n");

	if (tumDrawInit(bin_folder_path)) {
		printf("Failed to initialize drawing\n");
        goto err_tumDrawInit;
	}

    if (tumEventInit()) {
		printf("Failed to initialize Events\n");
        goto err_tumEventInit;
	}

    if (tumSoundInit(bin_folder_path)) {
        printf("Failed to initialize sound\n");
        goto err_tumSoundInit;
    }

    DrawSignal = xSemaphoreCreateBinary(); // Screen buffer locking
    if (!DrawSignal) {
        printf("Failed to create draw signal\n");
        goto err_draw_signal;
    }

    ColourQueue = xQueueCreate(1, sizeof(short));
    if (!ColourQueue) {
        printf("Could not open colour queue");
        goto err_collourQueueDelete;
    }

    // Load Font
    tumFontLoadFont(BUTTON_FONT, BUTTON_FONT_SIZE);
    tumFontLoadFont(FPS_FONT, DEFAULT_FONT_SIZE);
    tumFontLoadFont(HIGHSCORE_FONT, DEFAULT_FONT_SIZE);
    tumFontLoadFont(SCORE_FONT, DEFAULT_FONT_SIZE*2);
    
    if (buttonsInit()) {
        printf("Failed to create buttons lock\n");
        goto err_buttonsInit;
    }
    
    printf("\nInitialization SUCCESS!! \nMoving on to create tasks... \n");    

    /*-----------------------------------------------------------------------------------------------*/	
    /* FreeRTOS Task creation*/
    
    if(xTaskCreate(vSwapBuffers, "BufferSwap", 
            mainGENERIC_STACK_SIZE/2 , NULL,
			configMAX_PRIORITIES, &BufferSwap)!= pdPASS){
        printf("Failed to create Buffer Task\n");
        goto err_bufferSwapTask;
    }

    if (initStateMachine()){
        printf("Failed to initalize State Machine\n");
        goto err_stateMachineInit;
    }
    
    /*-----------------------------------------------------------------------------------------------*/	
	/* start FreeRTOS Sceduler: Should never get passed the function vTaskStartScheduler() */

    tumFUtilPrintTaskStateList();

	vTaskStartScheduler(); // FreeRTOS Task Sceduler

	atexit(aIODeinit); // standart C library: passed function pointer gets called when function exits -> ensures to clean sockets, Queues etc 

	return EXIT_SUCCESS;

    /*-----------------------------------------------------------------------------------------------*/	
    /* Error handling -> delete everything that has been initialized so far (Backwards the Init Order) */
        deleteStateMachine();
    err_stateMachineInit:
        vTaskDelete(BufferSwap);
    err_bufferSwapTask:
        buttonsExit();
    err_buttonsInit:
        vQueueDelete(ColourQueue);
    err_collourQueueDelete:
        tumSoundExit();
    err_tumSoundInit:
	    vSemaphoreDelete(DrawSignal);
    err_draw_signal:
	    tumEventExit();
    err_tumEventInit:
    	tumDrawExit();
    err_tumDrawInit:

	return EXIT_FAILURE; // Return State Main
}

// cppcheck-suppress unusedFunction
__attribute__((unused)) void vMainQueueSendPassed(void)
{
	/* This is just an example implementation of the "queue send" trace hook. */
}

// cppcheck-suppress unusedFunction
__attribute__((unused)) void vApplicationIdleHook(void)
{
#ifdef __GCC_POSIX__
	struct timespec xTimeToSleep, xTimeSlept;
	/* Makes the process more agreeable when using the Posix simulator. */
	xTimeToSleep.tv_sec = 1;
	xTimeToSleep.tv_nsec = 0;
	nanosleep(&xTimeToSleep, &xTimeSlept);
#endif
}
