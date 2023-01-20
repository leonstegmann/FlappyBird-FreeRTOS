#ifndef __UDP_MASTER_H_
#define __UDP_MASTER_H_

/**
 * @brief creates a FreeRTOS Task that initiates the UDP-connection from the Master's
 * Side and keeps sending Data to the Slave.
 */
void createMasterTask();

/**
 * @brief deletes the FreeRTOS Task
 */
void deleteMasterTask();

#endif // __UDP_MASTER_H_