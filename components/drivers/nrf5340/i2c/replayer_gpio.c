#include"replayer_gpio.h"
#include"replayer_common.h"
#include<stdint.h>

void replayer_gpio_init(void)
{
	uint32_t pin_id = 0;
	uint32_t value;
    replayer_read32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * pin_id, REPLAYER_GPIO_NAME, &value);
	value = value | (0x30000000);
    replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * pin_id, REPLAYER_GPIO_NAME, value);

	pin_id = 1;
    replayer_read32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * pin_id, REPLAYER_GPIO_NAME, &value);
	value = value | (0x30000000);
    replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * pin_id, REPLAYER_GPIO_NAME, value);

	replayer_init_pin(20, 1, 1);
	replayer_init_pin(19, 1, 1);
	pin_id = 22;
    replayer_read32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * pin_id, REPLAYER_GPIO_NAME, &value);
	value = 0xc;
    replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * pin_id, REPLAYER_GPIO_NAME, value);

	pin_id = 21;
    replayer_read32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * pin_id, REPLAYER_GPIO_NAME, &value);
	value = 0xc;
    replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * pin_id, REPLAYER_GPIO_NAME, value);
}

void replayer_init_pin(uint8_t pin_id, uint8_t direction, uint8_t output)
{
    uint32_t value = (1 << pin_id);
	if (direction)
	{
		if (output)
			replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_OUTSET, REPLAYER_GPIO_NAME, value);
		else
			replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_CLEAR, REPLAYER_GPIO_NAME, value);
	}
    replayer_read32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * pin_id, REPLAYER_GPIO_NAME, &value);
	// printf("[0x%x][p]0x%x\r\n", REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * pin_id, value);
	value = value | direction;
    replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * pin_id, REPLAYER_GPIO_NAME, value);
	// printf("[0x%x][a]0x%x\r\n", REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * pin_id, value);
}

void replayer_gpio_configure(uint8_t pin_id, uint32_t mode)
{
	switch (mode)
	{
	case MODE_GPIO_IN:
	case MODE_GPIO_OUT:
	case MODE_GPIO_IN_PD:
	case MODE_GPIO_IN_PU:
	case MODE_GPIO_IN_OD_PU:
	    replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_PIN_CNF + REPLAYER_GPIO_STEP * pin_id, REPLAYER_GPIO_NAME, mode);
		/* code */
		break;
	default:
		break;
	}
}
int replayer_pin_read(uint8_t pin_id)
{
	int value = 0;
    replayer_read32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_IN, REPLAYER_GPIO_NAME, &value);
	// printf("[GPIO] value: 0x%x\r\n", value);
	return ((value >> pin_id) & 0x1);
}
void replayer_pin_toggle(uint8_t pin_id)
{
	uint32_t value = 0;
    replayer_read32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_OUT, REPLAYER_GPIO_NAME, &value);
    uint32_t set_v = (1 << pin_id);
	if ((value >> pin_id) & 0x1)
	{
        replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_OUTSET, REPLAYER_GPIO_NAME, 0x0);
        replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_CLEAR, REPLAYER_GPIO_NAME, set_v);
	}
	else
	{
        replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_OUTSET, REPLAYER_GPIO_NAME, set_v);
        replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_CLEAR, REPLAYER_GPIO_NAME, 0x0);
	}
}

void replayer_pinout_set(uint8_t pin_id, uint8_t output)
{
	// uint32_t value = 0;
    uint32_t set_v = (1 << pin_id);
	if (output == 0)
	{
        replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_OUTSET, REPLAYER_GPIO_NAME, 0x0);
        replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_CLEAR, REPLAYER_GPIO_NAME, set_v);
	}
	else
	{
        replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_OUTSET, REPLAYER_GPIO_NAME, set_v);
        replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_CLEAR, REPLAYER_GPIO_NAME, 0x0);
	}
}


// uint8_t replayer_pinout_set(uint8_t pin_id, uint8_t output)
// {
// 	// uint32_t value = 0;
//     uint32_t set_v = (1 << pin_id);
// 	if (output == 0)
// 	{
//         replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_OUTSET, REPLAYER_GPIO_NAME, 0x0);
//         replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_CLEAR, REPLAYER_GPIO_NAME, set_v);
// 	}
// 	else
// 	{
//         replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_OUTSET, REPLAYER_GPIO_NAME, set_v);
//         replayer_write32(REPLAYER_GPIO_BASE_ADDR, REPLAYER_GPIO_CLEAR, REPLAYER_GPIO_NAME, 0x0);
// 	}
// }