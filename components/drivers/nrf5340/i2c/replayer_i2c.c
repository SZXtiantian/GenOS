#include"replayer_i2c.h"
#include"replayer_gpio.h"
#include"replayer_common.h"
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>
// #include <nrfx.h>
// #include <zephyr/arch/cpu.h>
// #include <cmsis_core.h>
// #include <zephyr/sys/barrier.h>
// #include <zephyr/irq.h>
// #include"irq_test.h"
#include"replayer_rtc.h"
#ifdef LOG_DEBUG
#include<stdio.h>

#endif
#define READERBUFFER_SIZE   (100)
#define WRITERBUFFER_SIZE   (100)

uint8_t readerBuffer[READERBUFFER_SIZE];
uint8_t writeBuffer[READERBUFFER_SIZE];


#ifndef BASE_ADDR_SPU
#define BASE_ADDR_SPU 	(0x50003000)
#define FLASH_REGION 	(0x600)
#define RAM_REGION 		(0x700)
#define PERIPHID_REGION (0x800)
#define EXTDOMAIN_REGION 	(0x440)
#define DPPI_REGION 	(0x480)
#define GPIOPORT_REGION (0x4C0)
#define STEP 			(0x4)
#define NUM 			(64)
#define NETWORK			(0x50005000)
#define NETWORK_FORCEOFF	(0x614)
#define XIP_CTL			(0x5002B000)
#define XIPEN			(0x54C)
#define NVIC			(0xE000E100)
#define ISER			(0x000)
#define ICER			(0x080)
#define ISPR			(0x100)
#define ICPR			(0x180)
#define IABR			(0x200)
#define ITNS			(0x280)
#define IPR				(0x300)
#define IPR_NUM			(124)
#endif

int flag = 0;
static int irq_value = 0;
// void arm_isr_handler(const void *args)
// {
// 	ARG_UNUSED(args);
//     // printf("IRQ Interrupts[%d]\n", __LINE__);
//     uint32_t value;
//     NVIC_ClearPendingIRQ(68);
//     replayer_read32(__I2C_BASE_ADDR_1, __EVENTS_SUSPEND, __I2C_NAME, &value);
//     if ((value & 0x1) == 1)
//         irq_value = 1;
//     replayer_read32(__I2C_BASE_ADDR_1, __EVENTS_STOPPED, __I2C_NAME, &value);
//     if ((value & 0x1) == 1)
//         irq_value = 1;
//     // printf("irq_value:%d\n", irq_value);
// }
void replayer_i2c_init(void)
{
    uint32_t value;
    // select SDA gpio pin, P1.02
    // for (int i = 0; i < (5 * 16 * 16 + 16 * 8  + 16) / 4; ++i)
    //     replayer_read32(__I2C_BASE_ADDR_1, 0x4 * i, __I2C_NAME, &value);
    replayer_read32(__I2C_BASE_ADDR_1, __ENABLE, __I2C_NAME, &value);
    replayer_read32(__I2C_BASE_ADDR_1, __PSELSDA, __I2C_NAME, &value);
    replayer_write32(__I2C_BASE_ADDR_1, __PSELSDA, __I2C_NAME, 0x22);
    replayer_read32(REPLAYER_GPIO_BASE_ADDR_1, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * 2, REPLAYER_GPIO_NAME, &value);
    replayer_write32(REPLAYER_GPIO_BASE_ADDR_1, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * 2, REPLAYER_GPIO_NAME, 0x600);

    // select SCL gpio pin, P1.03
    replayer_read32(__I2C_BASE_ADDR_1, __PSELSCL, __I2C_NAME, &value);
    replayer_write32(__I2C_BASE_ADDR_1, __PSELSCL, __I2C_NAME, 0x23);
    replayer_read32(REPLAYER_GPIO_BASE_ADDR_1, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * 3, REPLAYER_GPIO_NAME, &value);
    replayer_write32(REPLAYER_GPIO_BASE_ADDR_1, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * 3, REPLAYER_GPIO_NAME, 0x600);
    /*
    0x01980000: 100kbps
    0x04000000: 250kbps
    0x06400000: 400kbps
    0x0FF00000: 1000kbps
    */
    // set frequency, 100kbps
    replayer_write32(__I2C_BASE_ADDR_1, __FREQUENCY, __I2C_NAME, 0x01980000);     
    // Enabled TWIM
    replayer_write32(__I2C_BASE_ADDR_1, __ENABLE, __I2C_NAME, 0x6);
    // if (flag == 0)
    // {
        // arch_irq_connect_dynamic(0x9, 0, arm_isr_handler, NULL, 0);
        // IRQ_CONNECT(68, 0, arm_isr_handler, NULL, 0);
        // NRFX_IRQ_PRIORITY_SET(0x9, 7);
        // NRFX_IRQ_ENABLE(68);
        // ++flag;
    // }
}

void replayer_i2c_reset(void)
{
    uint32_t value;
    replayer_read32(__I2C_BASE_ADDR_1, __PSELSCL, __I2C_NAME, &value);
    replayer_read32(__I2C_BASE_ADDR_1, __PSELSDA, __I2C_NAME, &value);
    replayer_write32(__I2C_BASE_ADDR_1, __INTENCLR, __I2C_NAME, 0x19C0202);

    replayer_read32(__I2C_BASE_ADDR_1, __SHORTS, __I2C_NAME, &value);
    replayer_write32(__I2C_BASE_ADDR_1, __SHORTS, __I2C_NAME, 0x0);

    replayer_write32(__I2C_BASE_ADDR_1, __ENABLE, __I2C_NAME, 0x0);

    replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_OUTSET, REPLAYER_GPIO_NAME, 0x1);
    replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_OUTSET, REPLAYER_GPIO_NAME, 0x1);

    replayer_read32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_PIN_CNF, REPLAYER_GPIO_NAME, &value);
    replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_PIN_CNF, REPLAYER_GPIO_NAME, 0x60D);
    replayer_read32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_PIN_CNF, REPLAYER_GPIO_NAME, &value);
    replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_PIN_CNF, REPLAYER_GPIO_NAME, 0x60D);
    
    replayer_read32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_IN, REPLAYER_GPIO_NAME, &value);
    replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_CLEAR, REPLAYER_GPIO_NAME, 0x1);
    replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_OUTSET, REPLAYER_GPIO_NAME, 0x1);
    replayer_read32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_IN, REPLAYER_GPIO_NAME, &value);

    replayer_write32(__I2C_BASE_ADDR_1, __PSELSDA, __I2C_NAME, 0x22);
    replayer_write32(REPLAYER_GPIO_BASE_ADDR_1, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * 2, REPLAYER_GPIO_NAME, 0x600);
    replayer_write32(__I2C_BASE_ADDR_1, __PSELSCL, __I2C_NAME, 0x23);
    replayer_write32(REPLAYER_GPIO_BASE_ADDR_1, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * 3, REPLAYER_GPIO_NAME, 0x600);
    
    replayer_write32(__I2C_BASE_ADDR_1, __ENABLE, __I2C_NAME, 0x6);
}

void replayer_i2c_rtx_template_init(uint8_t slave_addr)
{
    uint8_t address = slave_addr;
    replayer_write32(__I2C_BASE_ADDR_1, __INTENCLR, __I2C_NAME, 0x19C0202);
    // address set
    replayer_write32(__I2C_BASE_ADDR_1, __ADDRESS, __I2C_NAME, address);
    // clear event
    replayer_write32(__I2C_BASE_ADDR_1, __EVENTS_LASTTX, __I2C_NAME, 0x0);
    // replayer_read32(__I2C_BASE_ADDR_1, EVENTS_LASTTX, __I2C_NAME, &value);
    replayer_write32(__I2C_BASE_ADDR_1, __EVENTS_SUSPEND, __I2C_NAME, 0x0);
    // replayer_read32(__I2C_BASE_ADDR_1, EVENTS_SUSPEND, __I2C_NAME, &value);    
    replayer_write32(__I2C_BASE_ADDR_1, __EVENTS_ERROR, __I2C_NAME, 0x0);
    // replayer_read32(__I2C_BASE_ADDR_1, EVENTS_ERROR, __I2C_NAME, &value);    
    replayer_write32(__I2C_BASE_ADDR_1, __EVENTS_STOPPED, __I2C_NAME, 0x0);
    // replayer_read32(__I2C_BASE_ADDR_1, EVENTS_STOPPED, __I2C_NAME, &value);
    // NRFY_TWIM_HAS_ARRAY_LIST
    replayer_write32(__I2C_BASE_ADDR_1, __TXD_LIST, __I2C_NAME, 0x0);
    replayer_write32(__I2C_BASE_ADDR_1, __RXD_LIST, __I2C_NAME, 0x0);
}



bool replayer_i2c_rtx_template_reset(void)
{
    uint32_t value;
    replayer_write32(__I2C_BASE_ADDR_1, __EVENTS_LASTTX, __I2C_NAME, 0x0);
    replayer_write32(__I2C_BASE_ADDR_1, __EVENTS_LASTRX, __I2C_NAME, 0x0);
    replayer_write32(__I2C_BASE_ADDR_1, __SHORTS, __I2C_NAME, 0x0);
    replayer_write32(__I2C_BASE_ADDR_1, __INTENCLR, __I2C_NAME, 0x19C0202);
    replayer_read32(__I2C_BASE_ADDR_1, __ERRORSRC, __I2C_NAME, &value);
    replayer_write32(__I2C_BASE_ADDR_1, __ERRORSRC, __I2C_NAME, 0x0);
    replayer_write32(__I2C_BASE_ADDR_1, __INTENSET, __I2C_NAME, 0x202);
    return (value & 0x1);
}
uint8_t *rx_buffer_ = NULL;
uint8_t bytes_to_read_ = 0;
uint8_t primary_length_ = 0;
uint8_t secondary_length_ = 0;
uint8_t slave_addr_ = 0;
void I2C_handler_WRITE()
{   
    uint32_t value;
    replayer_write32(__I2C_BASE_ADDR_1, __EVENTS_STOPPED, __I2C_NAME, 0x0);
    replayer_read32(__I2C_BASE_ADDR_1, __TXD_AMOUNT, __I2C_NAME, &value);
    if (value != primary_length_)
        return -1;
        // printf("[ERROR]: primary_length: %d, value:%d\n", primary_length, value);
    // else
        // printf("[SUCCESS]: primary_length: %d, value:%d\n", primary_length, value);
#ifdef LOG_DEBUG
    if (value != primary_length_)
        printf("[ERROR]: primary_length: %d, value:%d\n", primary_length_, value);
#endif
    bool isOverRun = replayer_i2c_rtx_template_reset();
    isOverRun;
}

void I2C_handler_WRITE_0(){
    uint32_t value;

    replayer_write32(__I2C_BASE_ADDR_1, __EVENTS_SUSPEND, __I2C_NAME, 0x0);
    replayer_write32(__I2C_BASE_ADDR_1, __SHORTS, __I2C_NAME, 0x0);
    replayer_write32(__I2C_BASE_ADDR_1, __INTENCLR, __I2C_NAME, 0x19C0202);
    replayer_read32(__I2C_BASE_ADDR_1, __ERRORSRC, __I2C_NAME, &value);
    replayer_write32(__I2C_BASE_ADDR_1, __ERRORSRC, __I2C_NAME, 0x0);
    replayer_write32(__I2C_BASE_ADDR_1, __INTENSET, __I2C_NAME, 0x40202);   
    // tx
    replayer_i2c_rtx_template_init(slave_addr_);
    replayer_write32(__I2C_BASE_ADDR_1, __RXD_PTR, __I2C_NAME, (uint32_t)(uint32_t*)readerBuffer);
    replayer_write32(__I2C_BASE_ADDR_1, __RXD_MAXCUT, __I2C_NAME, secondary_length_);
    replayer_write32(__I2C_BASE_ADDR_1, __SHORTS, __I2C_NAME, 0x1000);
    replayer_write32(__I2C_BASE_ADDR_1, __TASKS_RESUME, __I2C_NAME, 0x1);
    replayer_write32(__I2C_BASE_ADDR_1, __TASKS_STARTRX, __I2C_NAME, 0x1);
}

void I2C_handler_READ_1()
{
    uint32_t value;
    replayer_write32(__I2C_BASE_ADDR_1, __EVENTS_STOPPED, __I2C_NAME, 0x0);
    replayer_read32(__I2C_BASE_ADDR_1, __RXD_AMOUNT, __I2C_NAME, &value);
    if (value != bytes_to_read_)
        return -1;
#ifdef LOG_DEBUG
        printf("[ERROR]: bytes_to_read: %d, value:%d\n", bytes_to_read_, value);
#endif
    // }while (value < secondary_length);    
    bool isOverRun = replayer_i2c_rtx_template_reset();
    isOverRun;
    memcpy(rx_buffer_, readerBuffer, bytes_to_read_);
}

int replayer_i2c_write_read(uint8_t slave_addr, uint8_t reg_num, uint8_t *rx_buffer, uint8_t bytes_to_read)
{
    // if (flag == 0)
    // {
    //     IRQ_CONNECT(68, 0, arm_isr_handler, NULL, 0);
    //     // NRFX_IRQ_PRIORITY_SET(0x9, 7);
    //     NRFX_IRQ_ENABLE(68);
    //     NVIC_ClearPendingIRQ(68);
    //     ++flag;
    // }
    uint32_t value;
    rx_buffer_ = rx_buffer;
    // uint8_t address = slave_addr;
    size_t primary_length = sizeof(reg_num);
    size_t secondary_length = bytes_to_read;
    secondary_length_ = secondary_length;
    primary_length_ = primary_length;
    slave_addr_ = slave_addr;
    memcpy(writeBuffer, &reg_num, primary_length);
    // interrupt disable all
    replayer_i2c_rtx_template_init(slave_addr);
    // tx buffer set
    replayer_write32(__I2C_BASE_ADDR_1, __TXD_PTR, __I2C_NAME, (uint32_t)(uint32_t*)writeBuffer);
    replayer_write32(__I2C_BASE_ADDR_1, __TXD_MAXCUT, __I2C_NAME, primary_length);
    // // rx buffer set
    // replayer_write32(__I2C_BASE_ADDR_1, RXD_PTR, __I2C_NAME, &readerBuffer);
    // replayer_write32(__I2C_BASE_ADDR_1, RXD_MAXCUT, __I2C_NAME, secondary_length);
    // shorts set NRF_TWIM_SHORT_LASTTX_STARTRX_MASK | NRF_TWIM_SHORT_LASTRX_STOP_MASK
    // todo: diff to replayer
    replayer_write32(__I2C_BASE_ADDR_1, __SHORTS, __I2C_NAME, 0x100);
    // task trigger
    replayer_write32(__I2C_BASE_ADDR_1, __TASKS_RESUME, __I2C_NAME, 0x1);
    replayer_write32(__I2C_BASE_ADDR_1, __TASKS_STARTTX, __I2C_NAME, 0x1);
    // replayer_write32(__I2C_BASE_ADDR_1, INTENSET, __I2C_NAME, 0x00202); 
    // replayer_sleepms_rtc(1);
    // NVIC_ClearPendingIRQ(68);
    
    irq_value = 0;
    
    do {
        replayer_read32(__I2C_BASE_ADDR_1, __EVENTS_SUSPEND, __I2C_NAME, &value);
        // IRQ_PRINT_ALL();
    }while((value & 0x1) == 0);
    // NVIC_SetPendingIRQ(68);
    // barrier_dsync_fence_full();
	// barrier_isync_fence_full();

    // // printf("[%d]\n", __LINE__);
    // while(irq_value != 1) {;}

    replayer_write32(__I2C_BASE_ADDR_1, __EVENTS_SUSPEND, __I2C_NAME, 0x0);
    replayer_write32(__I2C_BASE_ADDR_1, __SHORTS, __I2C_NAME, 0x0);
    replayer_write32(__I2C_BASE_ADDR_1, __INTENCLR, __I2C_NAME, 0x19C0202);
    replayer_read32(__I2C_BASE_ADDR_1, __ERRORSRC, __I2C_NAME, &value);
    replayer_write32(__I2C_BASE_ADDR_1, __ERRORSRC, __I2C_NAME, 0x0);
    replayer_write32(__I2C_BASE_ADDR_1, __INTENSET, __I2C_NAME, 0x40202);   
    // tx
    replayer_i2c_rtx_template_init(slave_addr);
    replayer_write32(__I2C_BASE_ADDR_1, __RXD_PTR, __I2C_NAME, (uint32_t)(uint32_t*)readerBuffer);
    replayer_write32(__I2C_BASE_ADDR_1, __RXD_MAXCUT, __I2C_NAME, secondary_length);
    replayer_write32(__I2C_BASE_ADDR_1, __SHORTS, __I2C_NAME, 0x1000);
    replayer_write32(__I2C_BASE_ADDR_1, __TASKS_RESUME, __I2C_NAME, 0x1);
    replayer_write32(__I2C_BASE_ADDR_1, __TASKS_STARTRX, __I2C_NAME, 0x1);
    // NVIC_ClearPendingIRQ(68);
    irq_value = 0;
    do {
        replayer_read32(__I2C_BASE_ADDR_1, __EVENTS_STOPPED, __I2C_NAME, &value);
    }while ((value & 0x1) == 0);    
    // NVIC_SetPendingIRQ(68);

    // barrier_dsync_fence_full();
	// barrier_isync_fence_full();

    // // printf("[%d]\n", __LINE__);
    // while(irq_value != 1) {;}
    replayer_write32(__I2C_BASE_ADDR_1, __EVENTS_STOPPED, __I2C_NAME, 0x0);
    replayer_read32(__I2C_BASE_ADDR_1, __RXD_AMOUNT, __I2C_NAME, &value);
    if (value != bytes_to_read)
        return -1;
#ifdef LOG_DEBUG
        printf("[ERROR]: bytes_to_read: %d, value:%d\n", bytes_to_read, value);
#endif
    // }while (value < secondary_length);    
    bool isOverRun = replayer_i2c_rtx_template_reset();
    isOverRun;
    memcpy(rx_buffer, readerBuffer, bytes_to_read);
    return 0;
}




void replayer_i2c_read(uint8_t slave_addr, uint8_t *rx_buffer, uint8_t bytes_to_read)
{
    uint32_t value;
    // uint8_t address = slave_addr;
    size_t secondary_length = bytes_to_read;
    // tx
    replayer_i2c_rtx_template_init(slave_addr);
    replayer_write32(__I2C_BASE_ADDR_1, __RXD_PTR, __I2C_NAME, (uint32_t)(uint32_t*)readerBuffer);
    replayer_write32(__I2C_BASE_ADDR_1, __RXD_MAXCUT, __I2C_NAME, secondary_length);
    replayer_write32(__I2C_BASE_ADDR_1, __SHORTS, __I2C_NAME, 0x1000);
    replayer_write32(__I2C_BASE_ADDR_1, __TASKS_RESUME, __I2C_NAME, 0x1);
    replayer_write32(__I2C_BASE_ADDR_1, __TASKS_STARTRX, __I2C_NAME, 0x1);
    do {
        replayer_read32(__I2C_BASE_ADDR_1, __EVENTS_STOPPED, __I2C_NAME, &value);
    }while ((value & 0x1) == 0);    
    
    replayer_write32(__I2C_BASE_ADDR_1, __EVENTS_STOPPED, __I2C_NAME, 0x0);
    // do {

    replayer_read32(__I2C_BASE_ADDR_1, __RXD_AMOUNT, __I2C_NAME, &value);

#ifdef LOG_DEBUG
    if (value != bytes_to_read)
        printf("[ERROR]: bytes_to_read: %d, value:%d\n", bytes_to_read, value);
#endif
    // }while (value < secondary_length);    
    bool isOverRun = replayer_i2c_rtx_template_reset();
    isOverRun;
    memcpy(rx_buffer, readerBuffer, bytes_to_read);
}

int replayer_i2c_write(uint8_t slave_addr, uint8_t reg_num, uint8_t *tx_buffer, uint8_t bytes_to_write)
{
    // if (flag == 0)
    // {
    //     // arch_irq_connect_dynamic(0x9, 0, arm_isr_handler, NULL, 0);
    //     // IRQ_CONNECT(68, 0, arm_isr_handler, NULL, 0);
    //     // NRFX_IRQ_PRIORITY_SET(0x9, 7);
    //     // NRFX_IRQ_ENABLE(68);
    //     ++flag;
    // }

    uint32_t value;
    // uint8_t address = slave_addr;
    size_t primary_length = sizeof(reg_num);
    memcpy(writeBuffer, &reg_num, primary_length);
    if (bytes_to_write > 0)
        memcpy(writeBuffer + primary_length, tx_buffer, bytes_to_write);
    primary_length += bytes_to_write;

    // interrupt disable all
    replayer_i2c_rtx_template_init(slave_addr);
    // tx buffer set
    replayer_write32(__I2C_BASE_ADDR_1, __TXD_PTR, __I2C_NAME, (uint32_t)(uint32_t*)writeBuffer);
    replayer_write32(__I2C_BASE_ADDR_1, __TXD_MAXCUT, __I2C_NAME, primary_length);

    replayer_write32(__I2C_BASE_ADDR_1, __SHORTS, __I2C_NAME, 0x200);
    // task trigger
    replayer_write32(__I2C_BASE_ADDR_1, __TASKS_RESUME, __I2C_NAME, 0x1);
    replayer_write32(__I2C_BASE_ADDR_1, __TASKS_STARTTX, __I2C_NAME, 0x1);
    // replayer_write32(__I2C_BASE_ADDR_1, INTENSET, __I2C_NAME, 0x00202);   
    do {
        replayer_read32(__I2C_BASE_ADDR_1, __EVENTS_STOPPED, __I2C_NAME, &value);
    }while ((value & 0x1) == 0);  
    // NVIC_SetPendingIRQ(68);

    replayer_write32(__I2C_BASE_ADDR_1, __EVENTS_STOPPED, __I2C_NAME, 0x0);
    replayer_read32(__I2C_BASE_ADDR_1, __TXD_AMOUNT, __I2C_NAME, &value);
    if (value != primary_length)
        return -1;
        // printf("[ERROR]: primary_length: %d, value:%d\n", primary_length, value);
    // else
        // printf("[SUCCESS]: primary_length: %d, value:%d\n", primary_length, value);
#ifdef LOG_DEBUG
    if (value != primary_length)
        printf("[ERROR]: primary_length: %d, value:%d\n", primary_length, value);
#endif
    bool isOverRun = replayer_i2c_rtx_template_reset();
    isOverRun;
    return 0;
}
