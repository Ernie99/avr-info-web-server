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
void W5500_Init_Soc0(void);
void W5500_Test(void);
void strobeCE(void);
void readAndPrintSettings(void);
void writeIp(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint16_t reg);
uint8_t pollStatus(char* statusString, uint8_t block, uint8_t testIndex, uint8_t toTest);
void pollStatusPortZeroAndPrint(void);
void testPoll(void);
void pollPointersPortZeroAndPrint(void);
void readFew(void);

#endif /* WIZNET_H_ */