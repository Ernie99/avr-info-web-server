/*
 * CFile1.c
 *
 * Created: 10/11/2014 2:31:54 PM
 *  Author: Ernie
 */ 

#include "Wiznet.h"

void startServer(uint8_t socReg){
	wiznetSpiInit();
	W5500_Init();
	readWnetAndPrintSettings();
	W5500_Init_Soc(SOC0_REG);
	
}

uint8_t count scanForString(char * string, uint8_t maxLength){ // blocking
	while(1)
	{
		while(pollForNewToken(uint8_t socReg)){
			
		}	
	}
}