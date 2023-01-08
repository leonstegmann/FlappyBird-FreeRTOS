/**
 * @file buttons.h
 * @author Leon Stegmann
 * @brief provides the Structs and Functions for the use of buttons
 *
 * The Implementation is as follows:
 * 
 * first use the following function. It creates the xSemaphoreCreateMutex()
 * for the button.lock and checks for errors in this step. Returns 1 if the Button has been pressed and 0 if not.
 * @code
 * if( buttonsInit(void))
 * @endcode
 * 
 * Everything is stored in the in this file instatianted object calles buttons
 * @code
 * extern buttons_buffer_t buttons
 * @endcode
 * 
 * To call simply use: (here example with button Q)
 * @code
 * if( checkButton(KEYCODE(Q)))
 * @endcode
 * To check if a specific Button was pressed. 
 * This function already considers the Debounce Delay,
 * defined in DEBOUNCEDELAY .
 * 
 */

#ifndef __BUTTONS_H__
#define __BUTTONS_H__

/* necesarry Includes  */
#include "TUM_Draw.h" // for coord_t

/* Defines variables  */
#define KEYCODE(CHAR) SDL_SCANCODE_##CHAR   // needed for the KEYCODE(X) calls
#define  DEBOUNCEDELAY (TickType_t) 50      // the debounce time

/**
 * @brief writes all current button values to the object "extern buttons_buffer_t buttons"
 */
void xGetButtonInput(void);

/**
 * @brief creates the locking Semaphore
 * @return returns 0 if successful and -1 if an error occured
 */
int buttonsInit(void);

/**
 * @brief deletes the Semaphore 
 */
void buttonsExit(void);

/**
 * @brief ensures the debounce of the buttons 
 * @param keyvalue intakes the "#define KEYCODE(CHAR)" from the "SDL_SCANCODE_##CHAR" as "int"
 * @return  Returns 1 if the Button has been pressed and 0 if not.
 */
int checkButton(int );

/**
 * @brief Setter for the Button Name displayed on the GUI 
 * @param id Button Name
 * @param keyvalue intakes the "#define KEYCODE(CHAR)" from the "SDL_SCANCODE_##CHAR" as "int"
 */ 
void setDisplayedButtonName(char , int );

/**
 * @brief Setter for the Button Position where it is displayed on the GUI 
 * @param position Button position  "coord_t"
 * @param keyvalue intakes the "#define KEYCODE(CHAR)" from the "SDL_SCANCODE_##CHAR" as "int"
 */ 
void setButtonPosition(coord_t , int );

/**
 * @brief Setter for the Button Colour displayed on the GUI 
 * @param colour Button Colour
 * @param keyvalue intakes the "#define KEYCODE(CHAR)" from the "SDL_SCANCODE_##CHAR" as "int"
 */ 
void setButtonColour(unsigned int , int );

#endif //__BUTTONS_H__