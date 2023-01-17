/**
 * @file main.h
 * @author Valentin Heubach
 * @brief provides resources from main.c
 */

#ifndef MAIN_H
#define MAIN_H

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"

extern SemaphoreHandle_t DrawSignal;

extern QueueHandle_t StateQueue;

#endif //__MAIN_H__

