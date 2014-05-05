/** @file I2c.h
 *  @brief Function prototypes for i2c bus initialization and data transfer modes
 *  @author Ernie H
 */ 

#ifndef I2C_H_
#define I2C_H_

/** @brief Write 8 bit address then immediately read an arbitrary number of bytes
 *  @param slaveAddressW i2c write address, lsb is low
 *  @param slaveAddressR i2c read address, lsb is high
 *  @param reg address of first register to write to
 *  @param buffer[] array to hold data from registers
 *  @param length number of bytes to copy to array
 *  @return pointer to buffer param
 */
char * writePtrReadBytes(uint8_t slaveAddressW, uint8_t slaveAddressR, uint8_t reg, uint8_t buffer[], uint8_t length);

/** @brief Write 8 bit address then immediately write an arbitrary number of bytes
 *  @param slaveAddressW i2c write address, lsb is low
 *  @param slaveAddressR i2c read address, lsb is high
 *  @param reg address of first register to be read from
 *  @param buffer[] array of data to be copied to registers
 *  @param length number of bytes to copy from array and write to registers
 */
void twiMasterWrite(uint8_t slaveAddressW, uint8_t regAddress, uint8_t data[], uint8_t length);
/** @brief Initialize AVR as i2c bus master and run at a reasonable speed
 */
void twiInit();
#endif /* RTCI2C_H_ */