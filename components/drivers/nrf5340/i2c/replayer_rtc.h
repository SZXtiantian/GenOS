#ifndef REPLAYER_RTC_H
#define REPLAYER_RTC_H
#include<stdint.h>
#define RTC_BASE_ADDR_0             (0x50014000)
// #define RTC_BASE_ADDR_1             (0x50015000)
#ifndef USE_TEE_CONFIG_REPLAYER
#define RTC_BASE_ADDR_1             (0x50014000)
#else
#define RTC_BASE_ADDR_1             (0x40014000)
#endif
#define RTC_TASKS_START             (0x000)
#define RTC_TASKS_STOP              (0x004)
#define RTC_TASKS_CLEAR             (0x008)
#define RTC_TASKS_TRIGOVRFLOW       (0x00c)
#define RTC_TASKS_CAPTURE           (0x040)
#define RTC_SUBSCRIBE_START         (0x080)
#define RTC_SUBSCRIBE_STOP          (0x084)
#define RTC_SUBSCRIBE_CLEAR         (0x088)
#define RTC_SUBSCRIBE_TRIGOVRFLOW   (0x08c)
#define RTC_SUBSCRIBE_CAPTURE       (0x0c0)
#define RTC_EVENT_TICKS             (0x100)
#define RTC_EVENT_OVRFLOW           (0x104)
#define RTC_EVENT_CAPTURE           (0x140)
#define RTC_PUBLISH_TICKS           (0x180)
#define RTC_PUBLISH_OVRFLOW         (0x184)
#define RTC_PUBLISH_CAPTURE         (0x1C0)
#define RTC_SHORTS                  (0x200)
#define RTC_INTENSET                (0x304)
#define RTC_INTENCLR                (0x308)
#define RTC_EVTEN                   (0x340)
#define RTC_EVTENSET                (0x344)
#define RTC_EVTENCLR                (0x348)
#define RTC_COUNTER                 (0x504)
#define RTC_PRESCALER               (0x508)
#define RTC_CC                      (0x540)
#define RTC_NAME                    ("RTC")
void replayer_rtc_init(void);
void replayer_sleep_rtc(int sleep_s);
void replayer_test_replayer(void);
void replayer_sleepms_rtc(int sleep_ms);
uint32_t replayer_counter(void);
void replayer_sleepus_rtc(int sleep_us);
uint32_t replayer_rtc_now();
#endif