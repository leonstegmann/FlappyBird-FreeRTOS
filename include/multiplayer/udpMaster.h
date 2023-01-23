#ifndef __UDP_MASTER_H_
#define __UDP_MASTER_H_

typedef struct tx_package_Master tx_packageM_t;

struct tx_package_Master {
    unsigned short bird_pos_Y; // 2 bytes
    coord_t pipe_pos[2]; // pipe_pos_X and pipe_gap_center_Y
}; 

/**
 * @brief creates a FreeRTOS Task that initiates the UDP-connection from the Master's
 * Side and keeps sending Data to the Slave.
 */
void createMasterTask();

/**
 * @brief deletes the FreeRTOS Task
 */
void deleteMasterTask();

/**
 * @brief Opens the UDP connection from the master's side.
 * This Function is already called once when creating the master FreeRTOS-task in 'createMasterTask()'.
 */
void initUDPConnectionMaster();

/**
 * @brief  Closes the UDP connection from the master's side.
 */
void closeUDPConnectionMaster();

#endif // __UDP_MASTER_H_