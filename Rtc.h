/*
 * Rtc.h
 *
 * Created: 1/25/2015 2:26:50 PM
 *  Author: Ernie
 */ 


#ifndef RTC_H_
#define RTC_H_

typedef enum {DS1307, DS3231, PCF2129T} clktype;
	
#define DALLAS_I2C_WR 0b11010000
#define DALLAS_I2C_RD 0b11010001
#define NXPRTC_I2C_WR 0b10100010
#define NXPRTC_I2C_RD 0b10100010

	
char * getClkString (clktype clockType);




#endif /* RTC_H_ */