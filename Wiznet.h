/** @file Wiznet.c
 *  @brief Function prototypes for Wiznet initialization and use
 *  @author Ernie H
 */ 



#ifndef WIZNET_H_
#define WIZNET_H_


void wiznetSpiInit(void);
void SPI_WriteByte(uint16_t address, uint8_t block, uint8_t data);
uint8_t SPI_ReadByte(uint16_t address, uint8_t block);
void W5500_Init(void);
void W5500_Test(void);
void strobeCE(void);

#endif /* WIZNET_H_ */