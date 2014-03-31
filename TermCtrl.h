/*
 * StartUp.h
 *
 * Created: 3/2/2014 2:16:49 PM
 *  Author: Ernie
 */ 


#ifndef STARTUP_H_
#define STARTUP_H_

void initUart();
void sendString(char str[]);
void sendChar(char c);
void ansi_cl(void);
void ansi_me(void);


#endif /* STARTUP_H_ */