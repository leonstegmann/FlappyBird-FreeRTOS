#ifndef __UDP_SLAVE_H_
#define __UDP_SLAVE_H_

typedef struct tx_package_Slave tx_packageS_t;

struct tx_package_Slave {
    unsigned short bird_pos_Y;
}; 

/**
 * @brief Opens the UDP connection from the slave's side.
 */
void initUDPConnectionSlave();

/**
 * @brief  Closes the UDP connection from the slave's side.
 */
void closeUDPConnectionSlave();

#endif // __UDP_SLAVE_H_