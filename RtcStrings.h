/** @file RtcStrings.h
 *  @brief Function prototypes to build strings from the seven RTC time/date registers, tested on DS3231 
 *
 *  @author Ernie H
 * 
 */

#ifndef RTCSTRINGS_H_
#define RTCSTRINGS_H_

/** @brief Build time string from the seven RTC time/date registers 
 *  @param destString[] buffer to store string, use "00:00:00 "
 *  @param registers[] array of the seven time/date RTC registers
 *  @return pointer to destString[]
 */
char * timeStr ( char destString[], const uint8_t registers[7]);

/** @brief Build day string from the seven RTC time/date registers 
 *  @param destString[] buffer to store string, use "Sun "
 *  @param registers[] array of the seven time/date RTC registers
 *  @return pointer to destString[]
 */
char * dayStr ( char destString[], const uint8_t registers[7]);

/** @brief Build date string from the seven RTC time/date registers 
 *  @param destString[] buffer to store string, use "00/00/00 "
 *  @param registers[] array of the seven time/date RTC registers
 *  @return pointer to destString[]
 */
char * dateStr ( char destString[], const  uint8_t registers[7]);

/** @brief Convert number to BCD
 *  @param num unsigned int
 *  @return number in BCD
 */
uint8_t decToBcd(uint8_t num);

#endif /* RTCSTRINGS_H_ */