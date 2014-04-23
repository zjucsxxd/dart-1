/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - Defines the interface for the time*
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
#ifndef TIME_H
#define TIME_H

// Define the 
#define MINSEC		60
#define HOURSEC		(60 * MINSEC)
#define DAYSEC		(24 * HOURSEC)
#define YEARDAYS(x)	(is_leap_year(x) ? 366 : 365)

// Frequency of generated timer ticks in hertz
#define TICK_FREQ_HZ		250

// 1-1-1970 was a thursday
#define EPOCH_WDAY	4

#define TIMESLICE			100
#define TIMESLICE_TICKS		(TIMESLICE * TICK_FREQ_HZ / 1000)

// Allow automatic user stack growth by at most 1024 pages at a time (4mb)
#define USTACK_MAXGROW		1024

// Per-process kernel stack size
#define KERN_STACK_SIZE		8192

typedef long time_t;

struct tm {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

static int is_leap_year(int yr);

static int mdays[2][12] = {
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

static char *wday[] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static char *mon[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

time_t time(time_t *tp);
char *asctime(struct tm *tm);
char *asctime_r(struct tm *tm, char *buf);

time_t mktime(struct tm *tm);
struct tm *gmtime(time_t *tp);
struct tm *gmtime_r(time_t *tp, struct tm *tm);

// non-standard helpers
int day_of_year(int year, int mon, int day);


#endif	// TIME_H
