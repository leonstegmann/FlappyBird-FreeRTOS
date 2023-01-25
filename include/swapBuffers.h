/**
 * @file swapBuffers.h
 * @author Valentin Heubach
 * @brief provides the Task to swap Buffers for the screen 
 */
#ifndef SWAPBUFFERS_H
#define SWAPBUFFERS_H

/**
 * @brief FreeRTOS Task for buffer swap
 * @param pvParameters void pointer input for any designated variable type
 */
void vSwapBuffers(void *pvParameters);

#endif //__SWAPBUFFERS_H__