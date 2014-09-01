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
#define SAR  0x0009   // Source Hardware Address (MAC): 0x0009 to 0x000E
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
/*	char test;
	for(unsigned int i = 0; i < 10 ; i++)
	{
		SPI_Write(GAR,i);
		_delay_ms(1000);
		test = SPI_Read(GAR);
		sendChar(test + 48);
		sendChar(32);
		_delay_ms(1000);
	}*/
	/*
	sendString("lets test some info\n");
	printOctet(200);
	sendString("now again\n");
	printOctet(42);
	sendString("now again\n");
	printOctet(123);
	sendString("all done\n");*/
	sendString("reeding the default MAC\n");
	strobeCE();
	_delay_us(1);
	CS_ENABLE;
	_delay_us(1);
	uint8_t mac0 =SPI_ReadByte(SAR,0);
	uint8_t mac1 =SPI_ReadByte(SAR+1,0);
	uint8_t mac2 =SPI_ReadByte(SAR+2,0);
	uint8_t mac3 =SPI_ReadByte(SAR+3,0);
	uint8_t mac4 =SPI_ReadByte(SAR+4,0);
	uint8_t mac5 =SPI_ReadByte(SAR+5,0);
	printOctet(mac0); sendChar('.');
	printOctet(mac1); sendChar('.');
	printOctet(mac2); sendChar('.');
	printOctet(mac3); sendChar('.');
	printOctet(mac4); sendChar('.');
	printOctet(mac5);
	_delay_us(1);
	CS_DISABLE;
}

void W5500_Init(void)
{
	/*
	// Ethernet Setup
	unsigned char mac_addr[] = {0x00,0x16,0x36,0xDE,0x58,0xF6};
	unsigned char ip_addr[] = {10,0,1,69};//{192,168,2,10};
	unsigned char sub_mask[] = {255,255,255,0};
	unsigned char gtw_addr[] = {1,0,1,1};
	// Setting the Wiznet W5100 Mode Register: 0x0000
	SPI_Write(MR,0x80);            // MR = 0b10000000;
	_delay_ms(1);
	sendString("reading MO \n");
//	printf("Reading MR: %d\n\n",SPI_Read(MR));
	// Setting the Wiznet W5100 Gateway Address (GAR): 0x0001 to 0x0004
//	printf("Setting Gateway Address %d.%d.%d.%d\n",gtw_addr[0],gtw_addr[1],\
	gtw_addr[2],gtw_addr[3]);
	SPI_Write(GAR + 0,gtw_addr[0]);
	SPI_Write(GAR + 1,gtw_addr[1]);
	SPI_Write(GAR + 2,gtw_addr[2]);
	SPI_Write(GAR + 3,gtw_addr[3]);
	_delay_ms(1);
	sendString("reading GAR \n");
	
	
	SPI_Write(GAR,4);
	_delay_ms(1000);
	
	char test;
	test = SPI_Read(GAR);
	sendChar(test + 48);
	sendChar(SPI_Read(GAR + 0) + 48);
	sendChar(SPI_Read(GAR + 1) + 48);
	sendChar(SPI_Read(GAR + 2) + 48);
	sendChar(SPI_Read(GAR + 3) + 48);
//	printf("Reading GAR: %d.%d.%d.%d\n\n",SPI_Read(GAR + 0),SPI_Read(GAR + 1),\
	SPI_Read(GAR + 2),SPI_Read(GAR + 3));
	// Setting the Wiznet W5100 Source Address Register (SAR): 0x0009 to 0x000E
//	printf("Setting Source Address %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",mac_addr[0],mac_addr[1],\
	mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
	SPI_Write(SAR + 0,mac_addr[0]);
	SPI_Write(SAR + 1,mac_addr[1]);
	SPI_Write(SAR + 2,mac_addr[2]);
	SPI_Write(SAR + 3,mac_addr[3]);
	SPI_Write(SAR + 4,mac_addr[4]);
	SPI_Write(SAR + 5,mac_addr[5]);
	_delay_ms(1);
//	printf("Reading SAR: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n\n",SPI_Read(SAR + 0),SPI_Read(SAR + 1),\
	SPI_Read(SAR + 2),SPI_Read(SAR + 3),SPI_Read(SAR + 4),SPI_Read(SAR + 5));
	// Setting the Wiznet W5100 Sub Mask Address (SUBR): 0x0005 to 0x0008
//	printf("Setting Sub Mask Address %d.%d.%d.%d\n",sub_mask[0],sub_mask[1],\
	sub_mask[2],sub_mask[3]);
	SPI_Write(SUBR + 0,sub_mask[0]);
	SPI_Write(SUBR + 1,sub_mask[1]);
	SPI_Write(SUBR + 2,sub_mask[2]);
	SPI_Write(SUBR + 3,sub_mask[3]);
	_delay_ms(1);
//	printf("Reading SUBR: %d.%d.%d.%d\n\n",SPI_Read(SUBR + 0),SPI_Read(SUBR + 1),\
	SPI_Read(SUBR + 2),SPI_Read(SUBR + 3));
	// Setting the Wiznet W5100 IP Address (SIPR): 0x000F to 0x0012
//	printf("Setting IP Address %d.%d.%d.%d\n",ip_addr[0],ip_addr[1],\
	ip_addr[2],ip_addr[3]);
	SPI_Write(SIPR + 0,ip_addr[0]);
	SPI_Write(SIPR + 1,ip_addr[1]);
	SPI_Write(SIPR + 2,ip_addr[2]);
	SPI_Write(SIPR + 3,ip_addr[3]);
	_delay_ms(1);
//	printf("Reading SIPR: %d.%d.%d.%d\n\n",SPI_Read(SIPR + 0),SPI_Read(SIPR + 1),\
	SPI_Read(SIPR + 2),SPI_Read(SIPR + 3));

	// Setting the Wiznet W5100 RX and TX Memory Size, we use 2KB for Rx/Tx 4 channels
//	printf("Setting Wiznet RMSR and TMSR\n\n");
	SPI_Write(RMSR,0x55);
	SPI_Write(TMSR,0x55);
//	printf("Done Wiznet W5100 Initialized!\n");*/
}

void strobeCE(void){
	CS_ENABLE;
	_delay_us(1);
	CS_DISABLE;
}