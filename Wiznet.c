/** @file Wiznet.c
 *  @brief Implementation, see header for API
 *  @author Ernie H
 */
#define F_CPU 16000000UL
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "UartTermBasic.h"
#include "Wiznet.h"

// ports and pins, etc
#define ETHCS PINB2
#define MOSI PINB3
#define MISO PINB4
#define SCK PINB5

#define SPI_DDR DDRB

// AVRJazz Mega168/328 SPI I/O
#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_CS   PORTB2

// Wiznet W5500 Block Addresses (5 bits)
#define COMMON_REG 0b00000
#define SOC0_TXBUF 0b00010
#define SOC0_RXBUF 0b00011


// Wiznet W5500 Register Addresses COMMON REGISTER
#define MR   0x0000   // Mode Register
#define GAR  0x0001   // Gateway Address: 0x0001 to 0x0004
#define SUBR 0x0005   // Subnet mask Address: 0x0005 to 0x0008
#define SHAR  0x0009   // Source Hardware Address (MAC): 0x0009 to 0x000E
#define SIPR 0x000F   // Source IP Address: 0x000F to 0x0012
//#define RMSR 0x001A   // RX Memory Size Register
//#define TMSR 0x001B   // TX Memory Size Register

// Wiznet W5500 Register Addresses SOCKET REGISTER
#define Sn_RX_WR 0
#define Sn_TX_RD 0
#define Sn_MR 0x0000 //socket n mode register r/w 0x0000, 0x00
#define Sn_CR 0x0001 //socket n command register r/w 0x0001, 0x00
#define Sn_SR 0x0003 // socket n status register
#define Sn_PORT1 0x0005

// Sn_PORT
// commands for SOCKET REGISTER
#define Sn_MR_TCP 0b00000001 // TCP mode
#define Sn_CR_OPEN 0x01 // open port, p69
#define Sn_CR_LISTEN 0x02
#define Sn_PORT1_80 80 // port 80
#define Sn_RECV 0x40// RECV command

// pointers and memory
#define Sn_TX_FSR_H 0x0020 //(Socket n TX Free Size Register)[R][0x0800]
#define Sn_TX_FSR_L 0x0021 //(Socket n TX Free Size Register)[R][0x0800]
#define Sn_TX_RD_H 0x0022 //(Socket n TX Read Pointer Register)[R][0x0000]
#define Sn_TX_RD_L 0x0023 //(Socket n TX Read Pointer Register)[R][0x0000]
#define Sn_TX_WR_H 0x0024 //(Socket n TX Write Pointer Register)[R/W][0x0000]
#define Sn_TX_WR_L 0x0025 //(Socket n TX Write Pointer Register)[R/W][0x0000]

#define Sn_RX_RSR_H 0x0026 //(Socket n RX Received Size Register)[R][0x0000]
#define Sn_RX_RSR_L 0x0027 //(Socket n RX Received Size Register)[R][0x0000]
#define Sn_RX_RD_H 0x0028 //(Socket n RX Read Data Pointer Register)[R/W][0x0000]
#define Sn_RX_RD_L 0x0029 //(Socket n RX Read Data Pointer Register)[R/W][0x0000]
#define Sn_RX_WR_H 0x002A //(Socket n RX Write Pointer Register)[R][0x0000]
#define Sn_RX_WR_L 0x002B //(Socket n RX Write Pointer Register)[R][0x0000]

#define CS_ENABLE SPI_PORT&=~_BV(ETHCS)
#define CS_DISABLE SPI_PORT|=_BV(ETHCS)

// private methods
void spiOneByteSend(uint8_t data);
void spiTwoBytesSend(uint16_t data);
void strobeCE(void);
void printIfNewRcv(uint8_t socReg);
void testTx(uint8_t socReg);
uint16_t getLongReg(uint8_t socReg, uint8_t lsbAddr);
void setLongReg(uint8_t socReg, uint8_t lsbAddr, uint16_t data);

uint8_t blockToSocNum(uint8_t socReg);

// private methods to print stuff
//void pollPointersAndPrint(uint8_t socReg);
void readFew(uint8_t socReg);

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
	CS_ENABLE;
	spiTwoBytesSend(address);
	spiOneByteSend(block);
	spiOneByteSend(data);
	CS_DISABLE;
}

uint8_t SPI_ReadByte(uint16_t address, uint8_t block){
	block = block << 3;
	// read enabled
	block |= _BV(0); // one byte size
	CS_ENABLE;
	spiTwoBytesSend(address);
	spiOneByteSend(block);
	spiOneByteSend(0xFF);//dummy
	CS_DISABLE;
	return(SPDR);
}

void W5500_Test(void)
{
	sendString("old data is \n");
	readWnetAndPrintSettings();
	W5500_Init();
	sendString("new data is \n");
	readWnetAndPrintSettings();
	W5500_Init_Soc(SOC0_REG);
	W5500_Init_Soc(SOC1_REG);
	uint8_t codeSoc0 = 0;
	uint8_t codeSoc1 = 0;
	char blank0[] = "123456789ABCDEEF";
	char blank1[] = "123456789ABCDEEF";
	while(1){

			_delay_ms(2000);
			sendString("portA ");
			codeSoc0 = pollStatus(blank0,SOC0_REG,0,0);
			sendString(blank0);
			sendString(" ");
			pollPointersPortAndPrint(SOC0_REG);
			sendString("\n");
			printIfNewRcv(SOC0_REG);
			sendString("portB ");
			codeSoc1 = pollStatus(blank1,SOC1_REG,0,0);
			sendString(blank1);
			sendString(" ");
			pollPointersPortAndPrint(SOC1_REG);
			sendString("\n\n");			
//			sendString(blank);
//			readFew(socReg);

			printIfNewRcv(SOC0_REG);
		}
}

void W5500_Init(void) // Write IP address, etc
{	
	CS_ENABLE;
	writeIp(10, 0, 1, 1, GAR);
	writeIp(255, 255, 255, 0, SUBR);
	writeIp(10, 0, 1, 55, SIPR);
	CS_DISABLE;


}

void W5500_Init_Soc(uint8_t socReg){
	//pollStatusPortAndPrint(socReg);
	SPI_WriteByte(Sn_MR,socReg,Sn_MR_TCP); // set to TCP mode
	//pollStatusPortAndPrint(socReg);
	SPI_WriteByte(Sn_PORT1,socReg,Sn_PORT1_80);// set to port 80
	//pollStatusPortAndPrint(socReg);
	SPI_WriteByte(Sn_CR,socReg, Sn_CR_OPEN); // open
	//pollStatusPortAndPrint(socReg);
	SPI_WriteByte(Sn_CR,socReg, Sn_CR_LISTEN);
}

void strobeCE(void){
	CS_ENABLE;
	_delay_us(1);
	CS_DISABLE;
}

void readWnetAndPrintSettings(void){
//	strobeCE();
//	_delay_us(1);
//	CS_ENABLE;
//	_delay_us(1);
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
//	_delay_us(1);
//	CS_DISABLE;
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

// Sn_SR status codes
const char symbol0x00[] PROGMEM = "SOCK_CLOSED";
const char symbol0x13[] PROGMEM = "SOCK_INIT";
const char symbol0x14[] PROGMEM = "SOCK_LISTEN";
const char symbol0x17[] PROGMEM = "SOCK_ESTABLISHED";
const char symbol0x1C[] PROGMEM = "SOCK_CLOSE_WAIT";
const char symbol0x22[] PROGMEM = "SOCK_UDP";
const char symbol0x42[] PROGMEM = "SOCK_MACRAW";
// Sn_SR temp status codes
const char symbol0x15[] PROGMEM = "SOCK_SYNSENT";
const char symbol0x16[] PROGMEM = "SOCK_SYNRECV";
const char symbol0x18[] PROGMEM = "SOCK_FIN_WAIT";
const char symbol0x1A[] PROGMEM = "SOCK_CLOSING";
const char symbol0x1B[] PROGMEM = "SOCK_TIME_WAIT";
const char symbol0x1D[] PROGMEM = "SOCK_LAST_ACK";
const char symbol0xEF[] PROGMEM = "DIPSHIT";
const char symbol0xFF[] PROGMEM = "FUCKEDITUP";

const char* const symbolPointers[] PROGMEM = {symbol0x00, symbol0x13,
	symbol0x14, symbol0x17, symbol0x1C, symbol0x22, symbol0x42,
	symbol0x15, symbol0x16, symbol0x18, symbol0x1A, symbol0x1B,
	symbol0x1D, symbol0xEF, symbol0xFF};


uint8_t pollStatus(char* statusString, uint8_t block, uint8_t testIndex, uint8_t toTest){
	uint8_t code;
	if (toTest)
		code = testIndex;
	else{
		CS_ENABLE;
		code = SPI_ReadByte(Sn_SR, block);
		CS_DISABLE;
	}
	uint8_t index = 14; // last
	
	switch (code) {
		case 0x00: index = 0; break;
		case 0x13: index = 1; break;
		case 0x14: index = 2; break;
		case 0x17: index = 3; break;
		case 0x1C: index = 4; break;
		case 0x22: index = 5; break;
		case 0x42: index = 6; break;
		case 0x15: index = 7; break;
		case 0x16: index = 8; break;
		case 0x18: index = 9; break;
		case 0x1A: index = 10; break;
		case 0x1B: index = 11; break;
		case 0x1D: index = 12; break;
		default: index = 13; break;
	}
	strcpy_PF(statusString, pgm_read_word(&symbolPointers[index]));
	return 0;
	
}

void pollStatusPortAndPrint(uint8_t socReg){
	char blank[] = "123456789ABCDEEF";
	pollStatus(blank,socReg,0,0);
	sendString("\n");
	sendString(blank);
//	sendString("\n");
}

void testPoll(uint8_t socReg){
	sendString("\n test SR codes \n");
	char blank[] = "123456789ABCDEEF";
	uint8_t codes[] = {0x00, 0x13, 0x14, 0x17, 0x1C, 0x22, 0x42, 0x15, 0x16, 0x18, 0x1A, 0x1B, 0x1D};
	uint8_t length = 13;
	for (uint8_t i = 0; i < length ; i++ ){
		pollStatus(blank, 0, codes[i], 1);
		sendString(blank);
		sendString("\n");
	}
	sendString("\n done test SR codes \n");
	W5500_Init_Soc(socReg);
	uint8_t realCode = 0;
	while(1){
		_delay_ms(2000);
		sendString("\n real code \n");
		realCode = pollStatus(blank,socReg,0,0);
		pollPointersPortAndPrint(socReg);
		sendString(blank);
		readFew(socReg);
	}
}

// private utility methods


// debug printing methods
void pollPointersPortAndPrint(uint8_t socReg){
	uint8_t data;
	uint8_t soc = blockToSocNum(socReg);
	soc += 48;
	sendChar(soc);
	sendString("TX--> ");
	for (uint8_t i = 0x20; i <= 0x24; i+=2 ) // hit all the MSB's by incrementing by 2
	{
		data = SPI_ReadByte(i,socReg);
		printOctetHex(data);
		data = SPI_ReadByte(i+1,socReg);
		printOctetHex(data);
		sendString(" ");
	}
	sendString("RX--> ");
	for (uint8_t i = 0x26; i <= 0x2A; i+=2 ) // hit all the MSB's by incrementing by 2
	{
		data = SPI_ReadByte(i,socReg);
		printOctetHex(data);
		data = SPI_ReadByte(i+1,socReg);
		printOctetHex(data);
		sendString(" ");
	}
}

void readFew(uint8_t socReg){
	char data;
	sendChar('\n');
	for(uint8_t i; i < 20; i++ ){
		data = SPI_ReadByte(i,socReg + 2); // + 2 to get RXBUF
		sendChar(data);	
	}
	sendChar('\n');
}

uint8_t blockToSocNum(uint8_t socReg){
	uint8_t port = 9; // also the error code
	switch (socReg)
	{
		case SOC0_REG: port = 0; break;
		case SOC1_REG: port = 1; break;
		case SOC2_REG: port = 2; break;
		case SOC3_REG: port = 3; break;
		case SOC4_REG: port = 4; break;
		case SOC5_REG: port = 5; break;
		case SOC6_REG: port = 6; break;
		case SOC7_REG: port = 7; break;
		default: port = 8; break; // also an error
	}
	return port;
}

printIfNewRcv(uint8_t socReg){
	uint16_t buffSize = getLongReg(socReg, Sn_RX_RSR_L);
	uint16_t wr;
	uint16_t rd = getLongReg(socReg, Sn_RX_RD_L);
	char data;
	if(buffSize == 0);
	else
	{
		for(uint16_t i; i < buffSize; i++ )
		{
			data = SPI_ReadByte(rd+i,socReg + 2); // + 2 to get RXBUF
			sendChar(data);
		}
		wr = getLongReg(socReg, Sn_RX_WR_L);
		//SPI_WriteByte(Sn_RX_RD_L,socReg,wrL);
		setLongReg(socReg, Sn_RX_RD_L, wr);
		SPI_WriteByte(Sn_CR,socReg,Sn_RECV);
	}
	
	return;
}
testTx(uint8_t socReg){
	// Read the Tx Write Pointer
	uint8_t wrL = SPI_ReadByte(Sn_TX_WR_L, socReg);
	char bla = 'a';
//	for(uint8_t i; i < 5; i++)
//	{
//		SPI_WriteByte(wrL)
//	}
	
	
}

// methods for low level data access
uint16_t getLongReg(uint8_t socReg, uint8_t lsbAddr)
{
	uint16_t buffSizeL = SPI_ReadByte(lsbAddr,socReg);
	uint16_t buffSizeH = SPI_ReadByte(lsbAddr - 1, socReg);
	buffSizeH = buffSizeH << 8;
	return(buffSizeH + buffSizeL);
}

setLongReg(uint8_t socReg, uint8_t lsbAddr, uint16_t data)
{
	uint16_t lsb = data;
	uint16_t msb = data;
	lsb = lsb & 0x00FF;
	msb = msb >> 8;
	SPI_WriteByte(lsbAddr,socReg,lsb);
	SPI_WriteByte(lsbAddr - 1,socReg,msb);	
}