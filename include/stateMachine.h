#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

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

/**
 * @brief This function deletes created Tasks running in the State machine.
 */
void deleteStateMachine();

#endif //__STATEMACHINE_H__