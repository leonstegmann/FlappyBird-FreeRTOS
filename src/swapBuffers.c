#include "FreeRTOS.h"
#include "task.h"

#include "TUM_Draw.h"
#include "TUM_Event.h"
#include "main.h"
#include "swapBuffers.h"
#include "stateMachine.h"
#include "states.h"

void vSwapBuffers(void *pvParameters)
{
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    const TickType_t frameratePeriod = 20;

    while (1) {
        tumDrawUpdateScreen();
        tumEventFetchEvents(FETCH_EVENT_BLOCK);
        states_run();
        xSemaphoreGive(DrawSignal);
        xSemaphoreGive(stateMachine.lock);
        vTaskDelayUntil(&xLastWakeTime,
                        pdMS_TO_TICKS(frameratePeriod));
    }
}