/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - Defines the interface for the rtc *
 * Copyright (C) 2011  Constantine M. Apostolou                          *
 * Author: Constantine M. Apostolou - conmarap@osarena.net               *
 *                                                                       *
 * This program is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "stdio.h"
#include "common.h"
#include "rtc.h"

static void read_rtc(struct tm *tm);
static int read_reg(int reg);

void init_rtc(void)
{
	struct tm tm;

	read_rtc(&tm);
	start_time = mktime(&tm);

	printf("*   Time: %s", asctime(&tm));
}


static void read_rtc(struct tm *tm)
{
	int statb, pm;

	// wait for any clock updates to finish 
	while(read_reg(REG_STATA) & STATA_BUSY);

	tm->tm_sec = read_reg(REG_SEC);
	tm->tm_min = read_reg(REG_MIN);
	tm->tm_hour = read_reg(REG_HOUR);
	tm->tm_mday = read_reg(REG_DAY);
	tm->tm_mon = read_reg(REG_MONTH);
	tm->tm_year = read_reg(REG_YEAR);

	// in 12hour mode, bit 7 means post-meridiem 
	pm = tm->tm_hour & HOUR_PM_BIT;
	tm->tm_hour &= ~HOUR_PM_BIT;

	// convert to binary if needed 
	statb = read_reg(REG_STATB);
	if(!(statb & STATB_BIN)) {
		tm->tm_sec = BCD_TO_BIN(tm->tm_sec);
		tm->tm_min = BCD_TO_BIN(tm->tm_min);
		tm->tm_hour = BCD_TO_BIN(tm->tm_hour);
		tm->tm_mday = BCD_TO_BIN(tm->tm_mday);
		tm->tm_mon = BCD_TO_BIN(tm->tm_mon);
		tm->tm_year = BCD_TO_BIN(tm->tm_year);
	}

	// make the year an offset from 1900 
	if(tm->tm_year < 100) {
		tm->tm_year += 100;
	} else {
		tm->tm_year -= 1900;
	}

	// if tm_hour is in 12h mode, convert to 24h 
	if(!(statb & STATB_24HR)) {
		if(tm->tm_hour == 12) {
			tm->tm_hour = 0;
		}
		if(pm) {
			tm->tm_hour += 12;
		}
	}

	tm->tm_mon -= 1;	// we want months to start from 0
}

static int read_reg(int reg)
{
	unsigned char val;
	outb(reg, PORT_CTL);
        int o = 0;
        while (o<10000){ o++; }
        o = 0;
	inb(val);//, PORT_DATA);
        while (o<10000){ o++; }
	return val;
}
