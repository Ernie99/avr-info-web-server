/** @file Wiznet.c
 *  @brief Implementation, see header for API
 *  @author Ernie H
 */
#define F_CPU 16000000UL
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include "UartTermBasic.h"
#include "Wiznet.h"

#define ETHCS PINB2
#define MOSI PINB3
#define MISO PINB4
#define SCK PINB5

#define SPI_DDR DDRB

// AVRJazz Mega168/328 SPI I/O
#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_CS   PORTB2

// Wiznet W5500 Register Addresses
#define MR   0x0000   // Mode Register
#define GAR  0x0001   // Gateway Address: 0x0001 to 0x0004
#define SUBR 0x0005   // Subnet mask Address: 0x0005 to 0x0008
#define SHAR  0x0009   // Source Hardware Address (MAC): 0x0009 to 0x000E
#define SIPR 0x000F   // Source IP Address: 0x000F to 0x0012
//#define RMSR 0x001A   // RX Memory Size Register
//#define TMSR 0x001B   // TX Memory Size Register

#define CS_ENABLE SPI_PORT&=~_BV(ETHCS)
#define CS_DISABLE SPI_PORT|=_BV(ETHCS)

// private methods
void spiOneByteSend(uint8_t data);
void spiTwoBytesSend(uint16_t data);
void strobeCE(void);

void spiOneByteSend(uint8_t data){
//	SPI_DDR &= ~_BV(MISO);
	SPDR = data;
	while(!(SPSR &_BV(SPIF)));
		;
}
void spiTwoBytesSend(uint16_t data){
	uint8_t msByte = data >> 8;
	uint8_t lsByte = data & 0x00ff;
	spiOneByteSend(msByte);
	spiOneByteSend(lsByte);
}

//public methods
void wiznetSpiInit(void)
{
	//set I/O directions for SPI pins
	SPI_DDR |= _BV(ETHCS);
	SPI_DDR |= _BV(MOSI);
	SPI_DDR |= _BV(SCK);
	SPI_DDR &= ~_BV(MISO);
	
	CS_DISABLE;
	
	// Enable SPI master mode
	SPCR |= _BV(SPE) | _BV(MSTR);
	// set SPI clock to FCPU / 128
	SPCR |= _BV(SPR1) | _BV(SPR0);
	
//	SPI_PORT |= _BV(ETHCS);
	
}

void SPI_WriteByte(uint16_t address, uint8_t block, uint8_t data){
	block = block << 3;
	block |= _BV(2); // enable write
	block |= _BV(0); // one byte size
	spiTwoBytesSend(address);
	spiOneByteSend(block);
	spiOneByteSend(data);
}

uint8_t SPI_ReadByte(uint16_t address, uint8_t block){
	block = block << 3;
	// read enabled
	block |= _BV(0); // one byte size
	spiTwoBytesSend(address);
	spiOneByteSend(block);
	spiOneByteSend(0xFF);//dummy
	return(SPDR);
}

void W5500_Test(void)
{
	readAndPrintSettings();
}

void W5500_Init(void)
{	
	CS_ENABLE;
	writeIp(10, 0, 1, 1, GAR);
	writeIp(255, 255, 255, 0, SUBR);
	writeIp(10, 0, 1, 55, SIPR);
	CS_DISABLE;


}

void strobeCE(void){
	CS_ENABLE;
	_delay_us(1);
	CS_DISABLE;
}

void readAndPrintSettings(void){
	strobeCE();
	_delay_us(1);
	CS_ENABLE;
	_delay_us(1);
	uint8_t gar0 =SPI_ReadByte(GAR,0);
	uint8_t gar1 =SPI_ReadByte(GAR+1,0);
	uint8_t gar2 =SPI_ReadByte(GAR+2,0);
	uint8_t gar3 =SPI_ReadByte(GAR+3,0);
	uint8_t subr0 =SPI_ReadByte(SUBR,0);
	uint8_t subr1 =SPI_ReadByte(SUBR+1,0);
	uint8_t subr2 =SPI_ReadByte(SUBR+2,0);
	uint8_t subr3 =SPI_ReadByte(SUBR+3,0);
	uint8_t mac0 =SPI_ReadByte(SHAR,0);
	uint8_t mac1 =SPI_ReadByte(SHAR+1,0);
	uint8_t mac2 =SPI_ReadByte(SHAR+2,0);
	uint8_t mac3 =SPI_ReadByte(SHAR+3,0);
	uint8_t mac4 =SPI_ReadByte(SHAR+4,0);
	uint8_t mac5 =SPI_ReadByte(SHAR+5,0);
	uint8_t sipr0 =SPI_ReadByte(SIPR,0);
	uint8_t sipr1 =SPI_ReadByte(SIPR+1,0);
	uint8_t sipr2 =SPI_ReadByte(SIPR+2,0);
	uint8_t sipr3 =SPI_ReadByte(SIPR+3,0);
	_delay_us(1);
	CS_DISABLE;
	sendString("Gateway Address: ");
	printOctetDec(gar0); sendChar('.');
	printOctetDec(gar1); sendChar('.');
	printOctetDec(gar2); sendChar('.');
	printOctetDec(gar3); sendChar('\n');
	sendString("Subnet Mask Address: ");
	printOctetDec(subr0); sendChar('.');
	printOctetDec(subr1); sendChar('.');
	printOctetDec(subr2); sendChar('.');
	printOctetDec(subr3); sendChar('\n');
	sendString("Source hardware Address: ");
	printOctetHex(mac0); sendChar('.');
	printOctetHex(mac1); sendChar('.');
	printOctetHex(mac2); sendChar('.');
	printOctetHex(mac3); sendChar('.');
	printOctetHex(mac4); sendChar('.');
	printOctetHex(mac5); sendChar('\n');
	sendString("Source IP Address: ");
	printOctetDec(sipr0); sendChar('.');
	printOctetDec(sipr1); sendChar('.');
	printOctetDec(sipr2); sendChar('.');
	printOctetDec(sipr3); sendChar('\n');
}

void writeIp(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint16_t reg)
{
	SPI_WriteByte(reg, 0, byte0);
	SPI_WriteByte(reg+1, 0, byte1);
	SPI_WriteByte(reg+2, 0, byte2);
	SPI_WriteByte(reg+3, 0, byte3);
}