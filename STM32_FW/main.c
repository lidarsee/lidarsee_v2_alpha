#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "math.h"
#include "string.h"
#include "Serial.h"
#include "Serial2.h"
#include "Delay.h"


#define XV11_PACKAGE_LENGTH	22
#define XV11_START_BYTE	0xFA


// Functions
uint16_t PackageChecksum(uint8_t * packagePointer);


// Global Variables
uint16_t GoodReadings = 0, BadReadings = 0;
uint16_t AnglesCovered = 0;

uint8_t XV11_Package[22];

uint16_t Distance[360];
uint32_t PrintTimestamp;
uint32_t DisplayTimestamp;




int main(void)
{
	RCC_ClocksTypeDef RCC_Clocks;

	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 1000);


	Serial_Init();
	Serial2_Init();

	SyncUp();

    while(1)
    {
    	//SH_SendChar('2');
    	//Serial_WriteByte(0xBB);
    	//Serial2_WriteByte(0x0A);

	while (Serial_Buffer_Count() < XV11_PACKAGE_LENGTH);
		LoadPackage(XV11_Package);
		if (XV11_Package[0] != XV11_START_BYTE) {
			SyncUp();
		} else {
			ParsePackage(XV11_Package);

			Serial2_WriteByte(0xAA);
			Serial2_WriteByte(0xBB);
			Serial2_WriteByte(0xCC);
			Serial2_WriteWords(Distance, 360);
			Serial2_WriteByte(0xCC);
			Serial2_WriteByte(0xBB);
			Serial2_WriteByte(0xAA);
		}


    }
}

void ParsePackage(uint8_t * packagePointer)
{
uint16_t i;
uint16_t Index;
uint8_t Speed;
uint8_t InvalidFlag[4];
uint8_t WarningFlag[4];
uint16_t Checksum, ChecksumCalculated;

Checksum = ((uint16_t)packagePointer[21] << 8) | packagePointer[20];
ChecksumCalculated = PackageChecksum(packagePointer);
if (Checksum != ChecksumCalculated) {
	BadReadings += 4;
}

Index = (packagePointer[1] - 0xA0) * 4;
Speed = ((uint16_t)packagePointer[3] << 8) | packagePointer[2];
InvalidFlag[0] = (packagePointer[5] & 0x80) >> 7;
InvalidFlag[1] = (packagePointer[9] & 0x80) >> 7;
InvalidFlag[2] = (packagePointer[13] & 0x80) >> 7;
InvalidFlag[3] = (packagePointer[17] & 0x80) >> 7;
WarningFlag[0] = (packagePointer[5] & 0x40) >> 6;
WarningFlag[1] = (packagePointer[9] & 0x40) >> 6;
WarningFlag[2] = (packagePointer[13] & 0x40) >> 6;
WarningFlag[3] = (packagePointer[17] & 0x40) >> 6;

if (Index == 0) {
	AnglesCovered = 0;
	for (i = 0; i < 360; i++) {
		if (Distance[i] > 0) AnglesCovered++;
	}

	GoodReadings = 0;
	BadReadings = 0;
}

for (i = 0; i < 4; i++) {
	if (!InvalidFlag[i])
	{
		Distance[Index+i] = packagePointer[4+(i*4)] | ((uint16_t)(packagePointer[5+(i*4)] & 0x3F) << 8);
		GoodReadings++;
	} else {
		Distance[Index+i] = 0;
		BadReadings++;
	}
}

}


void SyncUp(void)
{
	uint8_t i;
	int16_t ch = 0;
	int16_t ch2 = 0;

	//Serial_Buffer_Clear();
	Serial2_Buffer_Clear();


	while (ch != XV11_START_BYTE) {
		while (Serial_Buffer_Get() != XV11_START_BYTE);

		i = XV11_PACKAGE_LENGTH;
		while (i > 0) {
			ch = Serial_Buffer_Get();
			if (ch >= 0) i--;
		}

		// read the rest
		i = XV11_PACKAGE_LENGTH - 1;
		while (i > 0) {
			ch2 = Serial_Buffer_Get();
			if (ch2 >= 0) i--;
		}
	}
}


void LoadPackage(uint8_t * packagePointer)
{
uint8_t i = 0;
uint8_t ch;
int16_t temp;

while (i < XV11_PACKAGE_LENGTH) {
	temp = Serial_Buffer_Get();
	if (temp >= 0) {
		packagePointer[i] = temp;
		i++;
	} else {
		temp = 0;
	}
}
}



uint16_t PackageChecksum(uint8_t * packagePointer)
{
uint8_t i;
uint16_t data[10];
uint16_t checksum;
uint32_t chk32;

// group the data by word, little-endian
for (i = 0; i < 10; i++) {
	data[i] = packagePointer[2*i] | (((uint16_t)packagePointer[2*i+1]) << 8);
}

// compute the checksum on 32 bits
chk32 = 0;
for (i = 0; i < 10; i++) {
	chk32 = (chk32 << 1) + data[i];
}

// return a value wrapped around on 15bits, and truncated to still fit into 15 bits
checksum = (chk32 & 0x7FFF) + ( chk32 >> 15 ); // wrap around to fit into 15 bits
checksum = checksum & 0x7FFF; // truncate to 15 bits
}



void Debug_WriteWords(u16 *word, u16 numWords)
{
	u16 i;
	for (i = 0; i < numWords; i++)
	{
		  SH_SendChar(word[i] & 0x00FF);
		  SH_SendChar((word[i] & 0xFF00) >> 8);
	}
}
