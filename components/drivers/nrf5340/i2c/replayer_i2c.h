#ifndef REPLAYER_I2C_H
#define __REPLAYER_I2C_H

#include<stdint.h>

// #ifndef USE_TEE_CONFIG_REPLAYER
#define __I2C_BASE_ADDR_0	    (0x50008000UL)
// #define __I2C_BASE_ADDR_1	    (0x50009000UL)
// #define __I2C_BASE_ADDR_1	    (0x40008000UL)
// #define __I2C_BASE_ADDR_1	    (0x4000B000UL)
#define __I2C_BASE_ADDR_2	    (0x5000B000UL)
#define __I2C_BASE_ADDR_3	    (0x5000C000UL)
#define __I2C_BASE_ADDR_1	    (0x5000B000UL)
// #else
// #define __I2C_BASE_ADDR_0	    (0x40008000UL)
// #define __I2C_BASE_ADDR_1	    (0x40009000UL)
// #define __I2C_BASE_ADDR_2	    (0x4000B000UL)
// #define __I2C_BASE_ADDR_3	    (0x4000C000UL)
// #endif
#define __TASKS_STARTRX       (0x000)
#define __TASKS_STARTTX       (0x008)
#define __TASKS_STOP          (0x014)
#define __TASKS_SUSPEND       (0x01C)
#define __TASKS_RESUME        (0x020)
#define __SUBSCRIBE_STARTRX   (0x080)
#define __SUBSCRIBE_STARTTX   (0x088)
#define __SUBSCRIBE_STOP      (0x094)
#define __SUBSCRIBE_SUSPEND   (0x09C)
#define __SUBSCRIBE_RESUME    (0x0A0)
#define __EVENTS_STOPPED      (0x104)
#define __EVENTS_ERROR        (0x124)
#define __EVENTS_SUSPEND      (0x148)
#define __EVENTS_RXSTARTED    (0x14C)
#define __EVENTS_TXSTARTED    (0x150)
#define __EVENTS_LASTRX       (0x15C)
#define __EVENTS_LASTTX       (0x160)
#define __PUBLISH_STOPPED     (0x184)
#define __PUBLISH_ERROR       (0x1A4)
#define __PUBLISH_SUSPEND     (0x1C8)
#define __PUBLISH_RXSTARTED   (0x1CC)
#define __PUBLISH_TXSTARTED   (0x1D0)
#define __PUBLISH_LASTRX      (0x1DC)
#define __PUBLISH_LASTTX      (0x1E0)
#define __SHORTS              (0x200)
#define __INTEN               (0x300)
#define __INTENSET            (0x304)
#define __INTENCLR            (0x308)
#define __ERRORSRC            (0x4C4)
#define __ENABLE              (0x500)
#define __PSELSCL             (0x508)
#define __PSELSDA             (0x50C)
#define __FREQUENCY           (0x524)
#define __RXD_PTR             (0x534)
#define __RXD_MAXCUT          (0x538)
#define __RXD_AMOUNT          (0x53C)
#define __RXD_LIST            (0x540)
#define __TXD_PTR             (0x544)
#define __TXD_MAXCUT          (0x548)
#define __TXD_AMOUNT          (0x54C)
#define __TXD_LIST            (0x550)
#define __ADDRESS             (0x588)

#define __MPL3110_ADDRESS     (0x60)
#define __I2C_NAME            "TWIM"
int replayer_i2c_write_read(uint8_t slave_addr, uint8_t reg_num, uint8_t *rx_buffer, uint8_t bytes_to_read);
int replayer_i2c_write(uint8_t slave_addr, uint8_t reg_num, uint8_t *tx_buffer, uint8_t bytes_to_write);
void replayer_i2c_init(void);
// void I2C_handler_READ();
void I2C_handler_WRITE();
void I2C_handler_READ_1();
void I2C_handler_WRITE_0();
void replayer_i2c_read(uint8_t slave_addr, uint8_t *rx_buffer, uint8_t bytes_to_read);
#endif
