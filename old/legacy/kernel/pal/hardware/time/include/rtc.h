#ifndef _RTC_H_
#define _RTC_H_

#include "time.h"

// CMOS I/O ports 
#define PORT_CTL	0x70
#define PORT_DATA	0x71

// CMOS RTC registers 
#define REG_SEC			0
#define REG_ALARM_SEC	1
#define REG_MIN			2
#define REG_ALARM_MIN	3
#define REG_HOUR		4
#define REG_ALARM_HOUR	5
#define REG_WEEKDAY		6
#define REG_DAY			7
#define REG_MONTH		8
#define REG_YEAR		9
#define REG_STATA		10
#define REG_STATB		11
#define REG_STATC		12
#define REG_STATD		13

#define STATA_BUSY	(1 << 7)
#define STATB_24HR	(1 << 1)
#define STATB_BIN	(1 << 2)

#define HOUR_PM_BIT		(1 << 7)

#define BCD_TO_BIN(x)	((((x) >> 4) & 0xf) * 10 + ((x) & 0xf))

/* the time read from rtc during init */
time_t start_time;

void init_rtc(void);

#endif	/* _RTC_H_ */
