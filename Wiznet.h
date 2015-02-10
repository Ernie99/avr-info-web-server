/** @file Wiznet.c
 *  @brief Function prototypes for Wiznet initialization and use
 *  @author Ernie H
 */ 



#ifndef WIZNET_H_
#define WIZNET_H_

#include <stdint.h>

#define SPI_PORT PORTB
#define ETHCS PINB2

#define CS_ENABLE SPI_PORT&=~_BV(ETHCS)
#define CS_DISABLE SPI_PORT|=_BV(ETHCS)

// Wiznet W5500 Register Addresses COMMON REGISTER
#define MR   0x0000   // Mode Register
#define GAR  0x0001   // Gateway Address: 0x0001 to 0x0004
#define SUBR 0x0005   // Subnet mask Address: 0x0005 to 0x0008
#define SHAR  0x0009   // Source Hardware Address (MAC): 0x0009 to 0x000E
#define SIPR 0x000F   // Source IP Address: 0x000F to 0x0012

void readWnetAndPrintSettings(void);

// must toggle CS to use
void writeIp(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint16_t reg);

// blocking methods
void waitForEstablished(uint8_t socReg);
char * getNewToken(uint8_t socReg, char delimiter, char * myBuff);
// init function
void wiznetInitAll(void);
// test method
void mainWiznet(void);


#define SOC0_REG 0b00001
#define SOC1_REG 0b00101
#define SOC2_REG 0b01001
#define SOC3_REG 0b01101
#define SOC4_REG 0b10001
#define SOC5_REG 0b10101
#define SOC6_REG 0b11001
#define SOC7_REG 0b11101


#endif /* WIZNET_H_ */