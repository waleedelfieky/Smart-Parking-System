#ifndef LCD_INT_H
#define LCD_INT_H

#define LCD_PORT_DATA		DIO_PORTB
#define LCD_PORT_CONTROL 	DIO_PORTD

#define RS_PIN 				DIO_PIN4
#define RW_PIN 				DIO_PIN5
#define E_PIN 				DIO_PIN6

void HLCD_vInit(void);
void HLCD_vSendCommand(u8 A_u8Cmd);
void HLCD_vSendData(u8 A_u8Data);
void HLCD_vSendString(u8 *PA_u8String);
void HLCD_vClearScreen(void);
void HLCD_vGoToRowCol(u8 A_u8Row, u8 A_u8Col);
void HLCD_vDisplayStringRowColumn(u8 A_u8Row,u8 A_u8Col,const u8 *PCA_u8Str);
void HLCD_vItoS(s32 A_s32Data);
void HLCD_vWriteCustomCharacter(u8 A_u8PatternNo, u8 *PA_u8Pattern);
void HLCD_vDisplayCustomChar(u8 A_u8CharIndex);


#endif
