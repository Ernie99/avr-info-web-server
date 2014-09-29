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
void W5500_Init_Soc(uint8_t socReg);
void W5500_Test(void);
void strobeCE(void);
void readWnetAndPrintSettings(void);
void writeIp(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint16_t reg);
uint8_t pollStatus(char* statusString, uint8_t block, uint8_t testIndex, uint8_t toTest);
void pollStatusPortAndPrint(uint8_t socReg);
void testPoll(uint8_t socReg);

#define SOC0_REG 0b00001
#define SOC1_REG 0b00101
#define SOC2_REG 0b01001
#define SOC3_REG 0b01101
#define SOC4_REG 0b10001
#define SOC5_REG 0b10101
#define SOC6_REG 0b11001
#define SOC7_REG 0b11101


#endif /* WIZNET_H_ */