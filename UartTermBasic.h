/** @file UartTermBasic.h
 *  @brief Function prototypes for UART initialization, and communication with text terminal
 *  @author Ernie H 
 */

#ifndef UARTTERMBASIC_H_
#define UARTTERMBASIC_H_

/** @brief Initialize AVR uart (9600baud default, adjust constant in corresponding implementation file)
 */
void uartInit(void);

/** @brief Send single char over UART
 *  @x Char to be sent
 */
void sendChar(char x);

/** @brief Send string over UART
 *  @data[] null terminated string
 */
void sendString(char data[]);

/** @brief Clear text terminal
 */
void ansiClear(void);

/** @brief Set formating to plain
 */
void ansiPlain(void);

void printOctetDec(uint8_t octet);
void printOctetHex(uint8_t octet);

void sendChar(char data);

#endif