#ifndef MAIN_H
#define MAIN_H

/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

extern SemaphoreHandle_t DrawSignal;

extern QueueHandle_t ColourQueue;


#endif //__MAIN_H__

