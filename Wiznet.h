/** @file Wiznet.c
 *  @brief Function prototypes for Wiznet initialization and use
 *  @author Ernie H
 */ 



#ifndef WIZNET_H_
#define WIZNET_H_

#include <stdint.h>

void readWnetAndPrintSettings(void);

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