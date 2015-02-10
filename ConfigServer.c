/** @file ConfigServer.c
 *  @brief Implementation, see header for API
 *  @author Ernie H
 */

#include <avr/io.h>
#include <avr/eeprom.h>
#include "ConfigServer.h"
#include "UartTermBasic.h"
#include "Wiznet.h"

#define EEPROM_GATEWAY_ADDRESS_0 0
#define EEPROM_GATEWAY_ADDRESS_1 1
#define EEPROM_GATEWAY_ADDRESS_2 2
#define EEPROM_GATEWAY_ADDRESS_3 3

#define EEPROM_SUBNET_MASK_ADDRESS_0 4
#define EEPROM_SUBNET_MASK_ADDRESS_1 5
#define EEPROM_SUBNET_MASK_ADDRESS_2 6
#define EEPROM_SUBNET_MASK_ADDRESS_3 7

#define EEPROM_SOURCE_HARDWARE_ADDRESS_0 8
#define EEPROM_SOURCE_HARDWARE_ADDRESS_1 9
#define EEPROM_SOURCE_HARDWARE_ADDRESS_2 10
#define EEPROM_SOURCE_HARDWARE_ADDRESS_3 11

#define EEPROM_SOURCE_IP_ADDRESS_0 12
#define EEPROM_SOURCE_IP_ADDRESS_1 13
#define EEPROM_SOURCE_IP_ADDRESS_2 14
#define EEPROM_SOURCE_IP_ADDRESS_3 15

#define EEPROM_WAS_SET 40
#define MAGIC_NUMBER_FOR_SET 53

//prototypes (private for now)
void setEeprom();
void setFromErprom();


void configLoop(){
	sendString("start config: ");
	sendChar('\n');sendChar('\r');
	sendString("orig: ");
	sendChar('\n');sendChar('\r');

	readWnetAndPrintSettings();
	
	setEeprom();
	setFromErprom();
	
	sendString("new: ");
	sendChar('\n');sendChar('\r');
	readWnetAndPrintSettings();

	sendString("end config: ");
	
	W5500_Init_Soc(SOC0_REG);//start server on port 80
}

void initFromEeprom(){
	
}

void setEeprom(){
	eeprom_write_byte ((uint8_t*)EEPROM_GATEWAY_ADDRESS_0, 192);
	eeprom_write_byte ((uint8_t*)EEPROM_GATEWAY_ADDRESS_1, 168);
	eeprom_write_byte ((uint8_t*)EEPROM_GATEWAY_ADDRESS_2, 1);
	eeprom_write_byte ((uint8_t*)EEPROM_GATEWAY_ADDRESS_3, 1);
	
	eeprom_write_byte ((uint8_t*)EEPROM_SUBNET_MASK_ADDRESS_0, 255);
	eeprom_write_byte ((uint8_t*)EEPROM_SUBNET_MASK_ADDRESS_1, 255);
	eeprom_write_byte ((uint8_t*)EEPROM_SUBNET_MASK_ADDRESS_2, 255);
	eeprom_write_byte ((uint8_t*)EEPROM_SUBNET_MASK_ADDRESS_3, 0);
	
	eeprom_write_byte ((uint8_t*)EEPROM_SOURCE_IP_ADDRESS_0, 192);
	eeprom_write_byte ((uint8_t*)EEPROM_SOURCE_IP_ADDRESS_1, 168);
	eeprom_write_byte ((uint8_t*)EEPROM_SOURCE_IP_ADDRESS_2, 1);
	eeprom_write_byte ((uint8_t*)EEPROM_SOURCE_IP_ADDRESS_3, 123);
}

void setFromErprom(){
	uint8_t temp0;
	uint8_t temp1;
	uint8_t temp2;
	uint8_t temp3;
	
	CS_ENABLE;
	temp0 = eeprom_read_byte((uint8_t*)EEPROM_GATEWAY_ADDRESS_0);
	temp1 = eeprom_read_byte((uint8_t*)EEPROM_GATEWAY_ADDRESS_1);
	temp2 = eeprom_read_byte((uint8_t*)EEPROM_GATEWAY_ADDRESS_2);
	temp3 = eeprom_read_byte((uint8_t*)EEPROM_GATEWAY_ADDRESS_3);
	writeIp(temp0, temp1, temp2, temp3, GAR);
	
	temp0 = eeprom_read_byte((uint8_t*)EEPROM_SUBNET_MASK_ADDRESS_0);
	temp1 = eeprom_read_byte((uint8_t*)EEPROM_SUBNET_MASK_ADDRESS_1);
	temp2 = eeprom_read_byte((uint8_t*)EEPROM_SUBNET_MASK_ADDRESS_2);
	temp3 = eeprom_read_byte((uint8_t*)EEPROM_SUBNET_MASK_ADDRESS_3);
	writeIp(temp0, temp1, temp2, temp3, SUBR);
	
	temp0 = eeprom_read_byte((uint8_t*)EEPROM_SOURCE_IP_ADDRESS_0);
	temp1 = eeprom_read_byte((uint8_t*)EEPROM_SOURCE_IP_ADDRESS_1);
	temp2 = eeprom_read_byte((uint8_t*)EEPROM_SOURCE_IP_ADDRESS_2);
	temp3 = eeprom_read_byte((uint8_t*)EEPROM_SOURCE_IP_ADDRESS_3);
	writeIp(temp0, temp1, temp2, temp3, SIPR);
	
	CS_DISABLE;
}