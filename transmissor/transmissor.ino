#include "nRF24L01.h"
#include "RF24.h"

#define CE_PIN 9
#define CSN_PIN 10
#define TAMBUF	128

RF24 moduloRF(CE_PIN, CSN_PIN);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int canal[2];
char texto[TAMBUF];

int pot_1 = A0;
int pot_2 = A1;

void setup()
{
	moduloRF.begin();
	moduloRF.setChannel(100);
	Serial.begin(9600);
}

void loop()
{
	canal[0] = analogRead(pot_1);
	canal[1] = analogRead(pot_2);
    
	snprintf(texto, sizeof(char) * TAMBUF, "Canal 0: %d\t|\tCanal 1: %d", canal[0], canal[1]);
	Serial.println(texto);

	moduloRF.openWritingPipe(pipe);
	moduloRF.write(&canal,sizeof(canal));
}
