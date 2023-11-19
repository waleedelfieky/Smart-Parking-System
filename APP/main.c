#include "LIB/STD_TYPES.h"
#include "LIB/BIT_MATH.h"
#include "MCAL/Timer0.h"
#include "MCAL/DIO_int.h"
#include "MCAL/EXT_int.h"
#include "HAL/LCD_int.h"
#include "MCAL/GIE_int.h"
#include <util/delay.h>

#define SYSTEM_SSD_DEVICES					(4)

#define SYSTEM_SSD_DEVICES_CONTROL_PORT		DIO_PORTD
#define SYSTEM_SSD_DATA_PORT				DIO_PORTC
#define SYSTEM_SWITCHES_PORT				DIO_PORTA

#define SYSTEM_PLACES_NUMBER 				(8)

#define FIRST_GATE_OPENED					(1)
#define SECOND_GATE_OPENED					(1)

#define NONE_MESSAGE_CODE 					(8)
typedef enum {
	FIRST_SSD  = 0,
	SECOND_SSD,
	THIRD_SSD,
	FOURTH_SSD,
} SystemSSD_t;

typedef enum {
	NOT_AVALIABLE = 0,
	AVALIABLE,
} PlaceState_t;

typedef enum {
	ZERO_C = 0b00111111,
	ONE_C = 0b00000110,
	TWO_C = 0b01011011,
	THREE_C = 0b01001111,
	FOUR_C = 0b01100110,
	FIVE_C = 0b01101101,
	SIX_C = 0b01111101,
	SEVEN_C = 0b00000111,
	EIGHT_C = 0b11111111,
	NINE_C  = 0b01101111
} SSD_NumbersCathode;

typedef enum {
	ZERO_A  = 0b11000000,
	ONE_A   = 0b11111001,
	TWO_A   = 0b10100100,
	THREE_A = 0b10110000,
	FOUR_A  = 0b10011001,
	FIVE_A  = 0b10010010,
	SIX_A   = 0b10000010,
	SEVEN_A = 0b11111000,
	EIGHT_A = 0b10000000,
	NINE_A  = 0b10010000
} SSD_NumbersAnode;

static const SSD_NumbersCathode CGS_u8SSD_Nums[10] = {ZERO_C, ONE_C, TWO_C, THREE_C, FOUR_C, FIVE_C, SIX_C, SEVEN_C, EIGHT_C, NINE_C};

static u8 *firstGateMessages[9]  = {"FL1L1", "FR1R1", "FL2R1", "FR1L1", "SL1L1", "SR1R1", "SL2R1", "SR1L1", "None"};
static u8 *secondGateMessages[9] = {"SL1L1", "SR1R1", "SL2R1", "SR1L1", "FL1L1", "FR1R1", "FL2R1", "FR1L1", "None"};
static s8 shortestPathIndexFirstGate  = NONE_MESSAGE_CODE;
static s8 shortestPathIndexSecondGate = NONE_MESSAGE_CODE;

static SystemSSD_t systemSSDs[SYSTEM_SSD_DEVICES];

static PlaceState_t places[SYSTEM_PLACES_NUMBER] = {1, 1, 1, 1, 1, 1, 1, 1};

static u8 totalAvailablePlaces;

static u8 firstGateOpenedFlag  = 0;
static u8 secondGateOpenedFlag = 0;

void SevenSegmentsDisplayTask(void) {
	static SystemSSD_t prevSSD = FIRST_SSD;
	SystemSSD_t	currentSSD = (prevSSD + 1) % (SYSTEM_SSD_DEVICES);

	/* Disable previous SSD */
	DIO_vSetPinVal(SYSTEM_SSD_DEVICES_CONTROL_PORT, prevSSD, DIO_HIGH);

	/* Enable Current SSD */
	DIO_vSetPinVal(SYSTEM_SSD_DEVICES_CONTROL_PORT, currentSSD, DIO_LOW);

	/* Write data to the seven segment port */
	DIO_vSetPinVal(SYSTEM_SSD_DATA_PORT, DIO_PIN0, GET_BIT(CGS_u8SSD_Nums[systemSSDs[currentSSD]], 0));
	DIO_vSetPinVal(SYSTEM_SSD_DATA_PORT, DIO_PIN1, GET_BIT(CGS_u8SSD_Nums[systemSSDs[currentSSD]], 1));
	DIO_vSetPinVal(SYSTEM_SSD_DATA_PORT, DIO_PIN2, GET_BIT(CGS_u8SSD_Nums[systemSSDs[currentSSD]], 2));
	DIO_vSetPinVal(SYSTEM_SSD_DATA_PORT, DIO_PIN3, GET_BIT(CGS_u8SSD_Nums[systemSSDs[currentSSD]], 3));
	DIO_vSetPinVal(SYSTEM_SSD_DATA_PORT, DIO_PIN4, GET_BIT(CGS_u8SSD_Nums[systemSSDs[currentSSD]], 4));
	DIO_vSetPinVal(SYSTEM_SSD_DATA_PORT, DIO_PIN5, GET_BIT(CGS_u8SSD_Nums[systemSSDs[currentSSD]], 5));
	DIO_vSetPinVal(SYSTEM_SSD_DATA_PORT, DIO_PIN6, GET_BIT(CGS_u8SSD_Nums[systemSSDs[currentSSD]], 6));

	/* Set current SSD to the previous variable for the next call */
	prevSSD = currentSSD;
}

void GetPlacesStatus(void) {
	static u8 Gate1;
	static u8 flagGate1 = 1;

	static u8 Gate2;
	static u8 flagGate2 = 1;

	places[0] = DIO_u8GetPinVal(SYSTEM_SWITCHES_PORT, 0);
	places[1] = DIO_u8GetPinVal(SYSTEM_SWITCHES_PORT, 1);
	places[2] = DIO_u8GetPinVal(SYSTEM_SWITCHES_PORT, 2);
	places[3] = DIO_u8GetPinVal(SYSTEM_SWITCHES_PORT, 3);
	places[4] = DIO_u8GetPinVal(SYSTEM_SWITCHES_PORT, 4);
	places[5] = DIO_u8GetPinVal(SYSTEM_SWITCHES_PORT, 5);
	places[6] = DIO_u8GetPinVal(SYSTEM_SWITCHES_PORT, 6);
	places[7] = DIO_u8GetPinVal(SYSTEM_SWITCHES_PORT, 7);

	Gate1 = DIO_u8GetPinVal(SYSTEM_SSD_DATA_PORT, 7);
	Gate2 = DIO_u8GetPinVal(SYSTEM_SSD_DEVICES_CONTROL_PORT, 7);

	if(Gate1 == DIO_LOW && flagGate1 == 1) {
		firstGateOpenedFlag = 1;
		flagGate1 = 0;
	}
	else if(Gate1 == DIO_HIGH){
		flagGate1 = 1;
	}

	if(Gate2 == DIO_LOW && flagGate2 == 1) {
		secondGateOpenedFlag = 1;
		flagGate2 = 0;
	}
	else if(Gate2 == DIO_HIGH) {
		flagGate2 = 1;
	}

	systemSSDs[FIRST_SSD]   = places[0] + places[2];
	systemSSDs[SECOND_SSD]  = places[1] + places[3];
	systemSSDs[THIRD_SSD]   = places[4] + places[6];
	systemSSDs[FOURTH_SSD]  = places[5] + places[7];

	/* Calculating the total available places */
	totalAvailablePlaces = systemSSDs[FIRST_SSD] + systemSSDs[SECOND_SSD] + systemSSDs[THIRD_SSD]  + systemSSDs[FOURTH_SSD];
}

void LCD_Display(void) {
	/* Getting the shortest path */
	static u8 L_u8LcdStateFlag = 0;
	static u8 L_u8Counter = 0;
	s8 i;

	shortestPathIndexFirstGate  = NONE_MESSAGE_CODE;
	shortestPathIndexSecondGate = NONE_MESSAGE_CODE;
	for(i = 0; i < SYSTEM_PLACES_NUMBER; i++) {
		if(places[i] == AVALIABLE) {
			shortestPathIndexFirstGate = i;
			break;
		}
	}

	for(i = SYSTEM_PLACES_NUMBER - 1; i >= 0; i--) {
		if(places[i] == AVALIABLE) {
			shortestPathIndexSecondGate = i;
			break;
		}
	}

	shortestPathIndexSecondGate = SYSTEM_PLACES_NUMBER - i - 1;



	if(firstGateOpenedFlag == 1) {
		L_u8Counter++;
		HLCD_vClearScreen();
		HLCD_vSendString("Gate1 Welcome");
		HLCD_vGoToRowCol(1, 0);
		HLCD_vSendString("SP:");
		HLCD_vSendString(firstGateMessages[shortestPathIndexFirstGate]);
		if(L_u8Counter == 4) {
			firstGateOpenedFlag = 0;
			L_u8Counter = 0;
		}
	}
	else if(secondGateOpenedFlag == 1) {
		L_u8Counter++;
		HLCD_vClearScreen();
		HLCD_vSendString("Gate2 Welcome");
		HLCD_vGoToRowCol(1, 0);
		HLCD_vSendString("SP:");
		HLCD_vSendString(secondGateMessages[shortestPathIndexSecondGate]);
		if(L_u8Counter == 4) {
			secondGateOpenedFlag = 0;
			L_u8Counter = 0;
		}
	}
	else {
		HLCD_vClearScreen();
		HLCD_vSendString("T=");
		HLCD_vSendData('0' + totalAvailablePlaces);
		HLCD_vGoToRowCol(1, 0);
		HLCD_vSendString("F=");
		HLCD_vSendData(systemSSDs[FIRST_SSD] + systemSSDs[SECOND_SSD] + '0');
		HLCD_vSendString(", S=");
		HLCD_vSendData(systemSSDs[THIRD_SSD] + systemSSDs[FOURTH_SSD] + '0');
	}
}


int main(void) {
	DIO_vSetPortDir(SYSTEM_SSD_DATA_PORT, 0xFF);
	TIMERZERO_vInit();

	HLCD_vInit();

	/* For control */
	DIO_vSetPortDir(DIO_PORTD, 0xFF);

	DIO_vSetPortDir(SYSTEM_SWITCHES_PORT, 0x00);
	/* Activating pull up resistor for switches */
	DIO_vSetPortVal(SYSTEM_SWITCHES_PORT, 0xFF);


	/* Pin Directions for EXTI0 and EXTI1 */
	DIO_vSetPinDir(DIO_PORTC, DIO_PIN7, DIO_INPUT);
	DIO_vSetPinDir(DIO_PORTD, DIO_PIN7, DIO_INPUT);

	/* Aticevating Pull Up */
	DIO_vSetPinVal(DIO_PORTC, DIO_PIN7, DIO_HIGH);
	DIO_vSetPinVal(DIO_PORTD, DIO_PIN7, DIO_HIGH);
	GIE_vEnable();

	RTOS_u8CreateTask(SevenSegmentsDisplayTask, 8, 0, 2);
	RTOS_u8CreateTask(GetPlacesStatus, 207, 1, 1);
	RTOS_u8CreateTask(LCD_Display , 402, 2, 4);
//
	RTOS_voidStartOS();

	while(1) {

	}

	return 0;
}
