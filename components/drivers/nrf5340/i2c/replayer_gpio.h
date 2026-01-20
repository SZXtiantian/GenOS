#ifndef REPLAYER_GPIO_H
#define REPLAYER_GPIO_H
#define REPLAYER_GPIO_BASE_ADDR		(0x50842500UL)      // GPIO S APPLICATION
#define REPLAYER_GPIO_BASE_ADDR_1	(0x50842800UL)      // GPIO S APPLICATION
#define REPLAYER_GPIO_OUT 		    (0x004)             // RW Write GPIO port, the register is retained.
#define REPLAYER_GPIO_OUTSET 	    (0x008)             // RW Set indiviual bits in GPIO port, reads value of OUT register
#define REPLAYER_GPIO_CLEAR 	     (0x00c)             // RW Clear individual bits in GPIO port, reads value of OUT Register
#define REPLAYER_GPIO_IN             (0x010)             // R  Read GPIO port
#define REPLAYER_GPIO_DIR            (0x014)             // RW Direction of GPIO pins The register is retained.
#define REPLAYER_GPIO_DIRSET         (0x018)             // RW DIR set register. reads value of DIR register.
#define REPLAYER_GPIO_DIRCLR         (0x01C)             // RW DIR clear register. reads value of DIR register.
#define REPLAYER_GPIO_LATCH          (0x020)             // RW Latch register indicating what GPIO pins that have met the criteria set in the PIN_CNF[n].SENSE registers. The register is retained.
#define REPLAYER_GPIO_DETECTMODE     (0x024)             // Select between default DETECT signal behavior and LDETECT mode. The register applies to non-secure pins. The register is retained.
#define REPLAYER_GPIO_DETECTMODE_SEC (0x028)             // Select between default DETECT signal behavior and LDETECT mode. The register applies to secure pins. The register is retained.
#define REPLAYER_GPIO_PIN_CNF 		(0x200)             // Address offset: 0x200 + [n * 0x4], Configuration of GPIO pins. The register is retained.
#define REPLAYER_GPIO_STEP			(0x4) 
#define REPLAYER_GPIO_NAME           "GPIO"
#include<stdint.h>
void replayer_init_pin(uint8_t pin_id, uint8_t direction, uint8_t output);
void replayer_gpio_init(void);
void replayer_pin_toggle(uint8_t pin_id);
void replayer_pinout_set(uint8_t pin_id, uint8_t output);
int replayer_pin_read(uint8_t pin_id);
void replayer_gpio_configure(uint8_t pin_id, uint32_t mode);
typedef enum {
    MODE_GPIO_IN=0x0,
    MODE_GPIO_OUT=0x3,
    MODE_GPIO_IN_PD=0x4,
    MODE_GPIO_IN_PU=0xc,
    MODE_GPIO_IN_OD_PU=0x60c,
} REPLAYER_GPIO_TYPE;
#endif