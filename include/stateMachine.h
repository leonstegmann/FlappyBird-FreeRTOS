#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#define STATE_DEBOUNCE_DELAY 1000

#include "FreeRTOS.h"
#include "semphr.h"

typedef struct stateMachine {
    TickType_t last_change;
    SemaphoreHandle_t lock;
} StateMachine_t;

extern StateMachine_t stateMachine;

/**
 * @brief This function has to be modified if new stages are added.
 * It uses: 
 *  states_add();
 *  states_init();
 *  states_set_state();
 * from the states.h library to setup the StateMachine. 
 * @return 0 on Succes, 1 on Error.
 */
int initStateMachine();

void checkStateInput(TickType_t lastFrameTime);

/**
 * @brief This function deletes created Tasks running in the State machine.
 */
void deleteStateMachine();

#endif //__STATEMACHINE_H__