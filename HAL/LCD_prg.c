#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "LCD_int.h"
#include "DIO_int.h"
#include <util/delay.h>


void HLCD_vSendCommand(u8 A_u8Cmd) {
	/* Control RS and RW Pins */
	DIO_vSetPinVal(LCD_PORT_CONTROL, RS_PIN, DIO_LOW);
	DIO_vSetPinVal(LCD_PORT_CONTROL, RW_PIN, DIO_LOW);
	
	/* Write Command on the data pins */
	DIO_vSetPinVal(LCD_PORT_CONTROL, E_PIN, DIO_HIGH);
	_delay_us(100);
	DIO_vSetPortVal(LCD_PORT_DATA, A_u8Cmd);
	_delay_us(100);
	DIO_vSetPinVal(LCD_PORT_CONTROL, E_PIN, DIO_LOW);
	_delay_us(100);
	
}

void HLCD_vSendData(u8 A_u8Data) {
	/* Control RS and RW Pins */
	DIO_vSetPinVal(LCD_PORT_CONTROL, RS_PIN, DIO_HIGH);
	DIO_vSetPinVal(LCD_PORT_CONTROL, RW_PIN, DIO_LOW);
	
	/* Write Command on the data pins */
	DIO_vSetPinVal(LCD_PORT_CONTROL, E_PIN, DIO_HIGH);
	_delay_us(100);
	DIO_vSetPortVal(LCD_PORT_DATA, A_u8Data);
	_delay_us(100);
	DIO_vSetPinVal(LCD_PORT_CONTROL, E_PIN, DIO_LOW);
	_delay_us(100);

}

void HLCD_vInit(void) {
	/* Direction */
	DIO_vSetPortDir(LCD_PORT_DATA, 0xFF);
	DIO_vSetPinDir(LCD_PORT_CONTROL, RS_PIN, DIO_OUTPUT);
	DIO_vSetPinDir(LCD_PORT_CONTROL, RW_PIN, DIO_OUTPUT);
	DIO_vSetPinDir(LCD_PORT_CONTROL, E_PIN, DIO_OUTPUT);

	HLCD_vSendCommand(0x38); // FUNCTION SET COMMAND.
	HLCD_vSendCommand(0b00001100); // DISPLAY COMMAND
	HLCD_vSendCommand(0x01); // CLEAR COMMAND
}

void HLCD_vSendString(u8 *PA_u8String) {
	u8 L_u8I = 0;

	while(PA_u8String[L_u8I] != '\0') {
		HLCD_vSendData(PA_u8String[L_u8I]);
		L_u8I++;
	}
}

void HLCD_vClearScreen(void) {
	HLCD_vSendCommand(0x01);
}

void HLCD_vGoToRowCol(u8 A_u8Row, u8 A_u8Col) {
	u8 L_u8Address;
	switch(A_u8Row) {
		case 0:
			L_u8Address = A_u8Col + 0x00;
			break;
		case 1:
			L_u8Address = A_u8Col + 0x40;
			break;
	}

	HLCD_vSendCommand(L_u8Address | 0b10000000);
}

void HLCD_vDisplayStringRowColumn(u8 A_u8Row,u8 A_u8Col,const u8 *PCA_u8Str) {
	HLCD_vGoToRowCol(A_u8Row, A_u8Col);
	HLCD_vSendString(PCA_u8Str);
}

void HLCD_vItoS(s32 A_s32Data) {
	if(A_s32Data == 0) {
		HLCD_vSendData('0');
		return;
	}

	u8 LA_u8Buffer[10];
	u8 L_u8NegativeFlag = 0;
	u8 L_u8I = 0;
	s8 L_s8J;

	if(A_s32Data < 0) {
		A_s32Data *= -1;
		L_u8NegativeFlag = 1;
	}

	while(A_s32Data > 0) {
		LA_u8Buffer[L_u8I++] = (A_s32Data % 10);
		A_s32Data /= 10;
	}


	if(L_u8NegativeFlag) {
		HLCD_vSendData('-');
	}

	for(L_s8J = L_u8I - 1; L_s8J >= 0; L_s8J--) {
		HLCD_vSendData('0' + LA_u8Buffer[L_s8J]);
	}
}

void HLCD_vDisplayCustomChar(u8 A_u8CharIndex) {
	if(A_u8CharIndex <= 7) {
		HLCD_vSendData(A_u8CharIndex);
	}
}

void HLCD_vWriteCustomCharacter(u8 A_u8CharIndex, u8 *PA_u8Pattern) {
	u8 L_u8Iter;
	//HLCD_vSendCommand(0b01000000 + (A_u8CharIndex * 8));

	switch(A_u8CharIndex) {
		case 0: HLCD_vSendCommand(0x40); break;
		case 1: HLCD_vSendCommand(0x48); break;
		case 2: HLCD_vSendCommand(0x50); break;
		case 3: HLCD_vSendCommand(0x58); break;
		case 4: HLCD_vSendCommand(0x60); break;
		case 5: HLCD_vSendCommand(0x68); break;
		case 6: HLCD_vSendCommand(0x70); break;
		case 7: HLCD_vSendCommand(0x78); break;
	}

	for(L_u8Iter=0; L_u8Iter < 8; L_u8Iter++) {
		HLCD_vSendData(PA_u8Pattern[L_u8Iter]);
	}


	HLCD_vSendCommand(0x80);
}

u8 HLCD_u8GetAC(void) {
	DIO_vSetPinVal(LCD_PORT_CONTROL, RS_PIN, DIO_LOW);
	DIO_vSetPinVal(LCD_PORT_CONTROL, RW_PIN, DIO_OUTPUT);

	DIO_vSetPortDir(LCD_PORT_DATA, 0);
	DIO_vSetPinVal(LCD_PORT_CONTROL, E_PIN, DIO_LOW);

}



