/** @file Wiznet.c
 *  @brief Function prototypes for Wiznet initialization and use
 *  @author Ernie H
 */ 



#ifndef WIZNET_H_
#define WIZNET_H_


void wiznetSpiInit(void);
void spiSendChar(char c);
void SPI_Write(unsigned int addr,unsigned char data);
char SPI_Read(unsigned int addr);
void W5100_Init(void);
void W5100_test(void);


#endif /* WIZNET_H_ */