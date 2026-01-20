#ifndef REPLAYER_COMMON_H
#define REPLAYER_COMMON_H
#ifdef LOG_DEBUG
#include<stdio.h>
#endif
#include<stdint.h>
// #define LOG_DEBUG
void print_common();
int replayer_read32(uint32_t base_addr, uint32_t addr, char name[], uint32_t *value);
int replayer_write32(uint32_t base_addr, uint32_t addr, char name[], uint32_t value);
#endif