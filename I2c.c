/** @file I2c.c
 *  @brief Implementation, see header for API
 *  @author Ernie H
 */

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <util/twi.h>
#include "I2c.h"

#define TWI_SPEED_VAR 0x7D; //1kHz with ps of 3
#define WAIT_FOR_FLAG while (!(TWCR & (1<<TWINT))) //wait for twi bus operation to finish
#define ERR_LED_DDR DDRB
#define ERR_LED_PORT PORTB
#define ERR_LED_PIN_NUM PINB1

#define SEND_START (_BV(TWINT) | _BV(TWSTA) | _BV(TWEN))
#define SEND_STOP  (_BV(TWINT)|_BV(TWEN)|_BV(TWSTO))
#define SEND_TWDR (_BV(TWINT) | _BV(TWEN))
#define RECV_TWDR_ACK (_BV(TWINT) | _BV(TWEN) | _BV(TWEA))
#define RECV_TWDR_NACK (_BV(TWINT) | _BV(TWEN))

const void errIndicate();

void twiInit(){
	TWBR=TWI_SPEED_VAR;
	TWSR=_BV(TWPS0)|_BV(TWPS1);
	TWCR|=_BV(TWEN);
}

char * writePtrReadBytes(uint8_t slaveAddressW, uint8_t slaveAddressR, uint8_t reg, uint8_t buffer[], uint8_t length)
{
	
	TWCR = SEND_START; //send start
	WAIT_FOR_FLAG;
	if((TWSR & 0xF8)!=TW_START){
		errIndicate();
//		return;
	}
	TWDR=slaveAddressW;// write addr
	TWCR=SEND_TWDR;// send addr
	WAIT_FOR_FLAG;
	if((TWSR& 0xF8)!=TW_MT_SLA_ACK){
		errIndicate();
//		return;
	}
	TWDR=reg;// write location
	TWCR=SEND_TWDR;// send locatinook
	
	WAIT_FOR_FLAG;
	if((TWSR& 0xF8)!=TW_MT_DATA_ACK){
		errIndicate();
//		return;
	}
	TWCR=SEND_START; //send repeated start
	WAIT_FOR_FLAG;
	if((TWSR& 0xF8)!=TW_REP_START){
		errIndicate();
//		return;
	}
	TWDR=slaveAddressR;// tw read addr
	TWCR=SEND_TWDR;// send addr
	WAIT_FOR_FLAG;
	if((TWSR& 0xF8)!=TW_MR_SLA_ACK){
		errIndicate();
//		return;
	}

	for(int i = 0; i<(length-1); i++){ // for multi byte read, send ack
		TWCR=RECV_TWDR_ACK;
		WAIT_FOR_FLAG;
		if((TWSR& 0xF8)!=TW_MR_DATA_ACK){
			errIndicate();
//			return;
		}
		*(buffer+i)=TWDR;// read byte to buffer
	}

	TWCR=RECV_TWDR_NACK;
	WAIT_FOR_FLAG;
	if((TWSR& 0xF8)!=TW_MR_DATA_NACK){
		errIndicate();
//		return;
	}
	if(length==1)
	*(buffer)=TWDR; // if only reading a single bit
	else
	*(buffer+(length-1))=TWDR;
	TWCR = SEND_STOP;
	_delay_ms(1);
	//WAIT_FOR_FLAG;
	return buffer;
}

void twiMasterWrite(uint8_t slaveAddressW, uint8_t regAddress, uint8_t data[], uint8_t length)
{
	TWCR = SEND_START; //send start
	WAIT_FOR_FLAG;
	if((TWSR& 0xF8)!=TW_START){
		errIndicate();
//		return;
	}
	TWDR=slaveAddressW;// write addr
	TWCR=SEND_TWDR;// send addr
	WAIT_FOR_FLAG;
	if((TWSR& 0xF8)!=TW_MT_SLA_ACK){
		errIndicate();
//		return;
	}
	TWDR=regAddress;// write reg address
	TWCR=SEND_TWDR;
	WAIT_FOR_FLAG;
	for(int i = 0; i<length; i++){
		TWDR=*(data+i);// write byte
		TWCR=SEND_TWDR;
		WAIT_FOR_FLAG;
	}
	TWCR =SEND_STOP;
	_delay_ms(1);
}


const void errIndicate()
{
	ERR_LED_DDR |= _BV(ERR_LED_PIN_NUM);
	while(1){
		ERR_LED_PORT ^= _BV(ERR_LED_PIN_NUM);
		_delay_ms(300);
	}
	
}