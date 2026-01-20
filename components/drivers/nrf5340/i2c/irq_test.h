#ifndef _IRQ_H
#define _IRQ_H
#include <stdint.h>
#include <stdio.h>

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
void IRQ_PRINT(int irq_number)
{
	volatile uint32_t *addr;
	printf("*********NVIC********\n");
	printf("*********NVIC:ISER********\n");
	{
		addr = (uint32_t *)(NVIC + ISER + STEP * irq_number);
		printf("ISER:0x%x\t[0x%x]\n", *addr, addr);
	}
	printf("*********NVIC:ICER********\n");
	{
		addr = (uint32_t *)(NVIC + ICER + STEP * irq_number);
		printf("ICER:0x%x\t[0x%x]\n", *addr, addr);
	}
	printf("*********NVIC:ISPR********\n");
	{
		addr = (uint32_t *)(NVIC + ISPR + STEP * irq_number);
		printf("ISPR:0x%x\t[0x%x]\n", *addr, addr);
	}
	printf("*********NVIC:ICPR********\n");
	{
		addr = (uint32_t *)(NVIC + ICPR + STEP * irq_number);
		printf("ICER:0x%x\t[0x%x]\n", *addr, addr);
	}
	printf("*********NVIC:IABR********\n");
	{
		addr = (uint32_t *)(NVIC + IABR + STEP * irq_number);
		printf("IABR:0x%x\t\t[0x%x]\n", *addr, addr);
	}
	printf("*********NVIC:ITNS********\n");
	{
		addr = (uint32_t *)(NVIC + ITNS + STEP * irq_number);
		printf("ITNS:0x%x\t\t[0x%x]\n", *addr, addr);
	}
	printf("*********NVIC:IPR********\n");
	{
		addr = (uint32_t *)(NVIC + IPR * STEP * irq_number);
		printf("IPR[%d]:0x%x\t\t[0x%x]\n", irq_number, *addr, addr);
	}
	printf("***********************\n");
}

void IRQ_PRINT_ALL()
{
	volatile uint32_t *addr;
	printf("*********NVIC********\n");
	printf("*********NVIC:ISER********\n");
	for (int i = 0; i < 16; ++i)
	{
		addr = (uint32_t *)(NVIC + ISER + STEP * i);
		printf("0x%x\t", *addr);
	}
	printf("\n*********NVIC:ICER********\n");
	for (int i = 0; i < 16; ++i)
	{
		addr = (uint32_t *)(NVIC + ICER + STEP * i);
		printf("0x%x\t", *addr);
	}
	printf("\n*********NVIC:ISPR********\n");
	for (int i = 0; i < 16; ++i)
	{
		addr = (uint32_t *)(NVIC + ISPR + STEP * i);
		printf("0x%x\t", *addr);
	}
	printf("\n*********NVIC:ICPR********\n");
	for (int i = 0; i < 16; ++i)
	{
		addr = (uint32_t *)(NVIC + ICPR + STEP * i);
		printf("0x%x\t", *addr);
	}
	printf("\n*********NVIC:IABR********\n");
	for (int i = 0; i < 16; ++i)
	{
		addr = (uint32_t *)(NVIC + IABR + STEP * i);
		printf("0x%x\t", *addr);
	}
	printf("\n*********NVIC:ITNS********\n");
	for (int i = 0; i < 16; ++i)
	{
		addr = (uint32_t *)(NVIC + ITNS + STEP * i);
		printf("0x%x\t", *addr);
	}
	printf("\n*********NVIC:IPR********\n");
	for (int i = 0; i < IPR_NUM; ++i)
	{
		addr = (uint32_t *)(NVIC + IPR * STEP * i);
		printf("0x%x\t", i, *addr);
		if ((i + 1) % 16 == 0)
			printf("\n[%d]\n", i / 16);
	}
	printf("\n***********************\n");
}
#endif
