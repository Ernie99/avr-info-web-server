#ifndef UARTTERMBASIC_H_
#define UARTTERMBASIC_H_

void uartInit(void);
void sendChar(char x);
void sendString(char data[]);
void ansiClear(void);
void ansiPlain(void);

#endif