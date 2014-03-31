/*
 * I2c.h
 *
 * Created: 3/2/2014 4:23:44 PM
 *  Author: Ernie
 */ 


#ifndef I2C_H_
#define I2C_H_

char * writePtrReadBytes(uint8_t slaveAddressW, uint8_t slaveAddressR, uint8_t reg, uint8_t buffer[], uint8_t length);
void twiMasterWrite(uint8_t slaveAddressW, uint8_t regAddress, uint8_t data[], uint8_t length);
void twiInit();
#endif /* RTCI2C_H_ */