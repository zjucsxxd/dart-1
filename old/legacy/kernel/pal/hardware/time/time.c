/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - Interface for the time            *
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
#include "time.h"
#include "rtc.h"
#include "timer.h"

time_t time(time_t *tp)
{
	time_t res = start_time + tick / TICK_FREQ_HZ;

	if(tp) *tp = res;
	return res;
}

char *asctime(struct tm *tm)
{
	static char buf[64];
	return asctime_r(tm, buf);
}

char *asctime_r(struct tm *tm, char *buf)
{
	sprintf(buf, "%s %s %d %02d:%02d:%02d %d\n", wday[tm->tm_wday],
			mon[tm->tm_mon], tm->tm_mday, tm->tm_hour, tm->tm_min,
			tm->tm_sec, tm->tm_year + 1900);
	return buf;
}

time_t mktime(struct tm *tm)
{
	int i, num_years = tm->tm_year - 70;
	int year = 1970;
	int days = day_of_year(tm->tm_year + 1900, tm->tm_mon, tm->tm_mday - 1);

	/* set correct yearday */
	tm->tm_yday = days;

	for(i=0; i<num_years; i++) {
		days += YEARDAYS(year++);
	}

	/* set wday correctly */
	tm->tm_wday = (days + EPOCH_WDAY) % 7;

	return (time_t)days * DAYSEC + tm->tm_hour * HOURSEC +
		tm->tm_min * MINSEC + tm->tm_sec;
}

struct tm *gmtime(time_t *tp)
{
	static struct tm tm;
	return gmtime_r(tp, &tm);
}

struct tm *gmtime_r(time_t *tp, struct tm *tm)
{
	int year, days, leap, yrdays;
	time_t t;

	year = 1970;
	days = *tp / DAYSEC;
	t = *tp % DAYSEC;

	tm->tm_wday = (days + EPOCH_WDAY) % 7;

	while(days >= (yrdays = YEARDAYS(year))) {
		days -= yrdays;
		year++;
	}
	tm->tm_year = year - 1900;
	tm->tm_yday = days;

	leap = is_leap_year(year);
	tm->tm_mon = 0;
	while(days >= mdays[leap][tm->tm_mon]) {
		days -= mdays[leap][tm->tm_mon++];
	}

	tm->tm_mday = days + 1;

	tm->tm_hour = t / HOURSEC;
	t %= HOURSEC;
	tm->tm_min = t / MINSEC;
	tm->tm_sec = t % MINSEC;
	return tm;
}

int day_of_year(int year, int mon, int day)
{
	int i, yday, leap;

	leap = is_leap_year(year) ? 1 : 0;
	yday = day;

	for(i=0; i<mon; i++) {
		yday += mdays[leap][i];
	}
	return yday;
}

static int is_leap_year(int yr)
{
	/* exceptions first */
	if(yr % 400 == 0) {
		return 1;
	}
	if(yr % 100 == 0) {
		return 0;
	}
	/* standard case */
	return yr % 4 == 0;
}
