#include "replayer_rtc.h"
#include "replayer_common.h"
#include<stdint.h>

void replayer_rtc_init(void)
{
    uint32_t value;
    replayer_write32(RTC_BASE_ADDR_1, RTC_INTENCLR, RTC_NAME, 0xf0003);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVTENCLR, RTC_NAME, 0xf0003);
    replayer_write32(RTC_BASE_ADDR_1, RTC_PRESCALER, RTC_NAME, 0x0);
    replayer_write32(RTC_BASE_ADDR_1, RTC_INTENSET, RTC_NAME, 0x10000);
    replayer_write32(RTC_BASE_ADDR_1, RTC_INTENSET, RTC_NAME, 0x20000);
    replayer_write32(RTC_BASE_ADDR_1, RTC_INTENSET, RTC_NAME, 0x2);
    replayer_write32(RTC_BASE_ADDR_1, RTC_TASKS_CLEAR, RTC_NAME, 0x1);
    replayer_write32(RTC_BASE_ADDR_1, RTC_TASKS_START, RTC_NAME, 0x1);

    replayer_write32(RTC_BASE_ADDR_1, RTC_INTENCLR, RTC_NAME, 0x10000);
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &value);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, 0x0);
    replayer_read32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, &value);
    replayer_write32(RTC_BASE_ADDR_1, RTC_CC, RTC_NAME, 0x7fffff);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVTENSET, RTC_NAME, 0x10000);
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &value);
    replayer_write32(RTC_BASE_ADDR_1, RTC_INTENSET, RTC_NAME, 0x10000);
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &value);
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &value);
}

uint32_t replayer_counter(void)
{
    uint32_t counter;
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    return counter;
}

void replayer_sleep_rtc(int sleep_s)
{
    uint32_t base = 32768;
    uint32_t counter;
    uint32_t value;
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    // replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    // replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    // replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    replayer_write32(RTC_BASE_ADDR_1, RTC_INTENCLR, RTC_NAME, 0x10000);
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVTENCLR, RTC_NAME, 0x10000);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, 0x0);
    replayer_read32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, &value);
    // replayer_write32(RTC_BASE_ADDR_1, RTC_CC, RTC_NAME, counter + sleep_s * base);
    // replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    // printf("counter: %d\n", counter);
    replayer_write32(RTC_BASE_ADDR_1, RTC_CC, RTC_NAME, counter + sleep_s * base);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVTENSET, RTC_NAME, 0x10000);

    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    replayer_write32(RTC_BASE_ADDR_1, RTC_INTENSET, RTC_NAME, 0x10000);
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);

    // TODO: convert to irq
    do {
        replayer_read32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, &value);
        // printf("replayer_rtc:[%d]\n", value);
    }while((value & 0x1) == 0);
    // zu se

    // waiting for IRQ triggered
    replayer_read32(RTC_BASE_ADDR_1, RTC_INTENSET, RTC_NAME, &value);
    replayer_read32(RTC_BASE_ADDR_1, RTC_INTENSET, RTC_NAME, &value);

    replayer_read32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, &value);
    // 
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, 0x0);
    replayer_read32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, &value);
    

    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVTENCLR, RTC_NAME, 0x10000);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, 0x0);
}


void replayer_sleepms_rtc(int sleep_ms)
{
    uint32_t base = 32768 * sleep_ms / 1000;
    uint32_t counter;
    uint32_t value;
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    // replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    // replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    // replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    replayer_write32(RTC_BASE_ADDR_1, RTC_INTENCLR, RTC_NAME, 0x10000);
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVTENCLR, RTC_NAME, 0x10000);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, 0x0);
    replayer_read32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, &value);
    // replayer_write32(RTC_BASE_ADDR_1, RTC_CC, RTC_NAME, counter + sleep_s * base);
    // replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    // printf("counter: %d\n", counter);
    replayer_write32(RTC_BASE_ADDR_1, RTC_CC, RTC_NAME, counter + base);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVTENSET, RTC_NAME, 0x10000);

    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    replayer_write32(RTC_BASE_ADDR_1, RTC_INTENSET, RTC_NAME, 0x10000);
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);

    // TODO: convert to irq
    do {
        replayer_read32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, &value);
        // printf("replayer_rtc:[%d]\n", value);
    }while((value & 0x1) == 0);
    // zu se

    // waiting for IRQ triggered
    replayer_read32(RTC_BASE_ADDR_1, RTC_INTENSET, RTC_NAME, &value);
    replayer_read32(RTC_BASE_ADDR_1, RTC_INTENSET, RTC_NAME, &value);

    replayer_read32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, &value);
    // 
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, 0x0);
    replayer_read32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, &value);
    

    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVTENCLR, RTC_NAME, 0x10000);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, 0x0);
}

uint32_t replayer_rtc_now()
{
    uint32_t value;
    uint32_t counter;
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    return counter * 1000 / 32768 * 1000 ;
}

void replayer_sleepus_rtc(int sleep_us)
{
    uint32_t base = sleep_us / 1000 * 32768 / 1000;
    // printf("base: %d\r\n", base);
    uint32_t counter;
    uint32_t value;
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    // replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    // replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    // replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    replayer_write32(RTC_BASE_ADDR_1, RTC_INTENCLR, RTC_NAME, 0x10000);
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVTENCLR, RTC_NAME, 0x10000);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, 0x0);
    replayer_read32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, &value);
    // replayer_write32(RTC_BASE_ADDR_1, RTC_CC, RTC_NAME, counter + sleep_s * base);
    // replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    // printf("counter: %d\n", counter);
    replayer_write32(RTC_BASE_ADDR_1, RTC_CC, RTC_NAME, counter + base);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVTENSET, RTC_NAME, 0x10000);

    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    replayer_write32(RTC_BASE_ADDR_1, RTC_INTENSET, RTC_NAME, 0x10000);
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);

    // TODO: convert to irq
    do {
        replayer_read32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, &value);
        // printf("replayer_rtc:[%d]\n", value);
    }while((value & 0x1) == 0);
    // zu se

    // waiting for IRQ triggered
    replayer_read32(RTC_BASE_ADDR_1, RTC_INTENSET, RTC_NAME, &value);
    replayer_read32(RTC_BASE_ADDR_1, RTC_INTENSET, RTC_NAME, &value);

    replayer_read32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, &value);
    // 
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, 0x0);
    replayer_read32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, &value);
    

    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVTENCLR, RTC_NAME, 0x10000);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, 0x0);
}

void replayer_test_replayer(void){
    uint32_t counter;
    uint32_t value;
    replayer_read32(RTC_BASE_ADDR_1, RTC_INTENSET, RTC_NAME, &value);
    replayer_read32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, &value);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, 0x0);
    replayer_read32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, &value);
    replayer_read32(RTC_BASE_ADDR_1, RTC_COUNTER, RTC_NAME, &counter);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVTENCLR, RTC_NAME, 0x10000);
    replayer_write32(RTC_BASE_ADDR_1, RTC_EVENT_CAPTURE, RTC_NAME, 0x0);
}