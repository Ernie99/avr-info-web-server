/** @file RtcStrings.c
 *  @brief Implementation, see header for API
 *  @author Ernie H
 */

#include <avr/io.h>

char getAsciiFromNibble(uint8_t field, uint8_t shiftR);

char * timeStr ( char destString[], const uint8_t registers[7] )
{
	uint8_t index = 0;
	uint8_t tmpField;
	uint8_t tmpVal;
	tmpField = registers[2]; // hours address
	tmpVal = getAsciiFromNibble(tmpField, 4); // tens place
	destString[index] = tmpVal;
	index++;
	tmpVal = getAsciiFromNibble(tmpField, 0); // zeros place
	destString[index] = tmpVal;
	index++;
	destString[index] = ':';
	index++;
	tmpField = registers[1]; // minutes address
	tmpVal = getAsciiFromNibble(tmpField, 4); // tens place
	destString[index] = tmpVal;
	index++;
	tmpVal = getAsciiFromNibble(tmpField, 0); // zeros place
	destString[index] = tmpVal;
	index++;
	destString[index] = ':';
	index++;
	tmpField = registers[0]; // seconds address
	tmpVal = getAsciiFromNibble(tmpField, 4); // tens place
	destString[index] = tmpVal;
	index++;
	tmpVal = getAsciiFromNibble(tmpField, 0); // zeros place
	destString[index] = tmpVal;
	index++;
	destString[index] = ' ';
	index++;
	destString [index] = '\0';
	return destString;
}

char * dayStr ( char destString[], const uint8_t registers[7] )
{
	char tmpField;
	tmpField = registers[3];
	switch( tmpField )
	{
		case 1:
			destString="Sun ";
			break;
		case 2:
			destString="Mon ";
			break;
		case 3:
			destString="Tues ";
			break;
		case 4:
			destString="Wed ";
			break;
		case 5:
			destString="Thurs ";
			break;
		case 6:
			destString="Fri ";
			break;
		case 7:
			destString="Sat ";
			break;
	}
	return destString;
	
}

char * dateStr ( char destString[],  const uint8_t registers[7] )
{
	uint8_t index = 0;
	uint8_t tmpField;
	uint8_t tmpVal;
	tmpField = registers[5]; // month address
	tmpField &= 0b01111111; // ignore century
	tmpVal = getAsciiFromNibble(tmpField, 4); // tens place
	destString[index] = tmpVal;
	index++;
	tmpVal = getAsciiFromNibble(tmpField, 0); // zeros place
	destString[index] = tmpVal;
	index++;
	destString[index] = '/';
	index++;
	tmpField = registers[4]; // date address
	tmpVal = getAsciiFromNibble(tmpField, 4); // tens place
	destString[index] = tmpVal;
	index++;
	tmpVal = getAsciiFromNibble(tmpField, 0); // zeros place
	destString[index] = tmpVal;
	index++;
	destString[index] = '/';
	index++;
	tmpField = registers[6]; // year address
	tmpVal = getAsciiFromNibble(tmpField, 4); // tens place
	destString[index] = tmpVal;
	index++;
	tmpVal = getAsciiFromNibble(tmpField, 0); // zeros place
	destString[index] = tmpVal;
	index++;
	destString[index] = ' ';
	index++;
	destString [index] = '\0';
	return destString;
}

uint8_t decToBcd(uint8_t num) // two nibbles g
{
	uint8_t ones = num % 10;
	uint8_t tens = num / 10;
	// now pack it into a single byte
	uint8_t reg = tens << 4;
	reg |= ones;
	return reg;
}

char getAsciiFromNibble(uint8_t field, uint8_t shiftR)
{
	field = field >> shiftR; // get hour (tens place)
	field &= 0x0F;
	field += 48; // convert to ascii
	return field;
}