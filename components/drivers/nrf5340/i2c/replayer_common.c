#ifdef LOG_DEBUG
#include<stdio.h>
#endif
#include<stdio.h>
#include<stdint.h>
#include"replayer_common.h"

void print_common(){
    static int num = 0;
    ++num;
    printf("[%s][%d] num: %d\r\n", __func__, __LINE__, num);
}

// #define LOG_DEBUG
int replayer_read32(uint32_t base_addr, uint32_t addr, char name[], uint32_t *value)
{
    volatile uint32_t * register_;
    #ifdef LOG_DEBUG
    printf("\033[1;33m[DEBUG] %s: [r][dw] 0x%x 0x%x\033[0m\r\n", name, addr + base_addr, *value);
    #endif
    register_ = (uint32_t *)(addr + base_addr);
    *value = *register_;
    // nrf_barrier_r();
    return 0;
}

int replayer_write32(uint32_t base_addr, uint32_t addr, char name[], uint32_t value)
{
    volatile uint32_t * register_;
    #ifdef LOG_DEBUG
    printf("\033[1;32m[DEBUG] %s: [w][dw] 0x%x 0x%x\033[0m\r\n", name, addr + base_addr, value);
    // printf(" [DEBUG] %s: [w][dw] 0x%x 0x%x\r\n", name, addr + base_addr, value);
    #endif
    register_ = (uint32_t *)(addr + base_addr);
    *register_ = value;
    // nrf_barrier_w();
    return 0;
}

