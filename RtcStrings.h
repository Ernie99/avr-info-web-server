/*
 * RtcStrings.h
 *
 * Created: 3/2/2014 6:29:22 PM
 *  Author: Ernie
 */ 

#ifndef RTCSTRINGS_H_
#define RTCSTRINGS_H_

char * timeStr ( char destString[], const uint8_t registers[7]);
char * dayStr ( char destString[], const uint8_t registers[7]);
char * dateStr ( char destString[], const  uint8_t registers[7]);
uint8_t decToBcd(uint8_t num);

#endif /* RTCSTRINGS_H_ */