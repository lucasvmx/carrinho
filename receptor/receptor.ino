/*
	Se trata de um carrinho que se movimenta como um tanque. Para virar, um lado precisa diminuir de velocidade.
	Já testei num carrinho e as seguintes funções estão funcionando: frente, parado, trás (ré), para direita parado e para esquerda parado.
	Não estou conseguindo fazer o carrinho virar enquanto se locomove, precisa parar, direcionar o carrinho e se locomover.
*/

#include "nRF24L01.h"
#include "RF24.h"

#define CE_PIN 9
#define CSN_PIN 10

#define TAMBUF	128

RF24 moduloRF(CE_PIN, CSN_PIN);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int canal[2] = { -1, -1 };
char texto[TAMBUF];

int in1 = 22;
int in2 = 23;
int in3 = 24;
int in4 = 25;
int enA = 2;
int enB = 3;

//	DADOS RECEBIDOS
int pot_1 = -1;
int pot_2 = -1;

//	DADOS GERADOS
int velocidade = 0;
int direcao = 0;

//	EIXO Y - SENTIDO - POT_1
int tra_Val_Min = 0;
int tra_Val_Max = 486;
int par_Val_Min = 487;
int par_Val_Max = 537;
int fre_Val_Min = 538;
int fre_Val_Max = 1023;

//	EIXO X - DIRECAO - POT_2
int esq_Val_Min = 0;
int esq_Val_Max = 486;
int ret_Val_Min = 487;
int ret_Val_Max = 537;
int dir_Val_Min = 538;
int dir_Val_Max = 1023;

void setup()
{
	Serial.begin(250000);
	pinMode(in1,OUTPUT);
	pinMode(in2,OUTPUT);
	pinMode(in3,OUTPUT);
	pinMode(in4,OUTPUT);
	pinMode(enA,OUTPUT);
	pinMode(enB,OUTPUT);
	digitalWrite(in1,LOW);
	digitalWrite(in2,LOW);
	digitalWrite(in3,LOW);
	digitalWrite(in4,LOW);
	digitalWrite(enA,LOW);
	digitalWrite(enB,LOW);
	moduloRF.begin();
	moduloRF.setPALevel(RF24_PA_MIN); /* Deixa o amplificador do rf24 na potência mínima */
	moduloRF.setChannel(100);
	moduloRF.startListening();
	moduloRF.openReadingPipe(1, pipe);
}

void loop()
{
	if (moduloRF.available())
	{
		moduloRF.read(&canal, sizeof(canal));

		pot_1 = canal[0];
		pot_2 = canal[1];
	}

	// O CARRINHO DEVE FICAR PARADO CASO NENHUM DADO SEJA RECEBIDO POR RF
	if (pot_1 == -1 || pot_2 == -1)
	{
		digitalWrite(in1, LOW);
		digitalWrite(in2, LOW);
		digitalWrite(in3, LOW);
		digitalWrite(in4, LOW);
		analogWrite(enA, 0);
		analogWrite(enB, 0);
	}
	else {

		// PARA TRÁS E ESQUERDA
		if ((pot_1 >= tra_Val_Min) && (pot_1 <= tra_Val_Max) && (pot_2 >= esq_Val_Min) && (pot_2 <= esq_Val_Max))
		{
			velocidade = map(pot_1, tra_Val_Min, tra_Val_Max, 0, 255);
			direcao = map(pot_2, esq_Val_Max, esq_Val_Min, 0, 255);
			digitalWrite(in1, HIGH);
			digitalWrite(in2, LOW);
			digitalWrite(in3, HIGH);
			digitalWrite(in4, LOW);
			analogWrite(enA, direcao);
			analogWrite(enB, velocidade);
			Serial.println("TRÁS E ESQUERDA");
		}

		// PARA TRÁS E RETO
		if ((pot_1 >= tra_Val_Min) && (pot_1 <= tra_Val_Max) && (pot_2 >= ret_Val_Min) && (pot_2 <= ret_Val_Max))
		{
			velocidade = map(pot_1, tra_Val_Max, tra_Val_Min, 0, 255);
			digitalWrite(in1, HIGH);
			digitalWrite(in2, LOW);
			digitalWrite(in3, HIGH);
			digitalWrite(in4, LOW);
			analogWrite(enA, velocidade);
			analogWrite(enB, velocidade);
			Serial.println("TRÁS E RETO");
		}

		// PARA TRÁS E DIREITA
		if ((pot_1 >= tra_Val_Min) && (pot_1 <= tra_Val_Max) && (pot_2 >= dir_Val_Min) && (pot_2 <= dir_Val_Max))
		{
			velocidade = map(canal[0], tra_Val_Min, tra_Val_Max, 0, 255);
			direcao = map(canal[1], dir_Val_Min, dir_Val_Max, 0, 255);
			digitalWrite(in1, HIGH);
			digitalWrite(in2, LOW);
			digitalWrite(in3, HIGH);
			digitalWrite(in4, LOW);
			analogWrite(enA, velocidade);
			analogWrite(enB, direcao);
			Serial.println("TRÁS E DIREITA");
		}

		//  PARADO E ESQUERDA
		if ((pot_1 >= par_Val_Min) && (pot_1 <= par_Val_Max) && (pot_2 >= esq_Val_Min) && (pot_2 <= esq_Val_Max))
		{
			direcao = map(canal[1], esq_Val_Max, esq_Val_Min, 0, 255);
			digitalWrite(in1, HIGH);
			digitalWrite(in2, LOW);
			digitalWrite(in3, LOW);
			digitalWrite(in4, HIGH);
			analogWrite(enA, direcao);
			analogWrite(enB, direcao);
			Serial.println("PARADO E ESQUERDA");
		}

		// PARADO TOTALMENTE
		if ((pot_1 >= par_Val_Min) && (pot_1 <= par_Val_Max) && (pot_2 >= ret_Val_Min) && (pot_2 <= ret_Val_Max))
		{
			digitalWrite(in1, LOW);
			digitalWrite(in2, LOW);
			digitalWrite(in3, LOW);
			digitalWrite(in4, LOW);
			analogWrite(enA, LOW);
			analogWrite(enB, LOW);
			Serial.println("PARADO TOTALMENTE");
		}

		//PARADO E DIREITA
		if ((pot_1 >= par_Val_Min) && (pot_1 <= par_Val_Max) && (pot_2 >= dir_Val_Min) && (pot_2 <= dir_Val_Max))
		{
			direcao = map(pot_2, dir_Val_Min, dir_Val_Max, 0, 255);
			digitalWrite(in1, LOW);
			digitalWrite(in2, HIGH);
			digitalWrite(in3, HIGH);
			digitalWrite(in4, LOW);
			analogWrite(enA, direcao);
			analogWrite(enB, direcao);
			Serial.println("PARADO E DIREITA");
		}

		// FRENTE E ESQUERDA
		if ((pot_1 >= fre_Val_Min) && (pot_1 <= fre_Val_Max) && (pot_2 >= esq_Val_Min) && (pot_2 <= esq_Val_Max))
		{
			velocidade = map(pot_1, fre_Val_Min, fre_Val_Max, 0, 255);
			direcao = map(pot_2, esq_Val_Max, esq_Val_Min, 0, 255);
			digitalWrite(in1, LOW);
			digitalWrite(in2, HIGH);
			digitalWrite(in3, LOW);
			digitalWrite(in4, HIGH);
			analogWrite(enA, direcao);
			analogWrite(enB, velocidade);
			Serial.println("FRENTE E ESQUERDA");
		}

		//FRENTE E RETO
		if ((pot_1 >= fre_Val_Min) && (pot_1 <= fre_Val_Max) && (pot_2 >= ret_Val_Min) && (pot_2 <= ret_Val_Max))
		{
			velocidade = map(pot_1, fre_Val_Min, fre_Val_Max, 0, 255);
			digitalWrite(in1, LOW);
			digitalWrite(in2, HIGH);
			digitalWrite(in3, LOW);
			digitalWrite(in4, HIGH);
			analogWrite(enA, velocidade);
			analogWrite(enB, velocidade);
			Serial.println("FRENTE E RETO");
		}

		//FRENTE E DIREITA
		if ((pot_1 >= fre_Val_Min) && (pot_1 <= fre_Val_Max) && (pot_2 >= dir_Val_Min) && (pot_2 <= dir_Val_Max))
		{
			velocidade = map(pot_1, fre_Val_Min, fre_Val_Max, 0, 255);
			direcao = map(pot_2, dir_Val_Min, dir_Val_Max, 0, 255);
			digitalWrite(in1, LOW);
			digitalWrite(in2, HIGH);
			digitalWrite(in3, LOW);
			digitalWrite(in4, HIGH);
			analogWrite(enA, velocidade);
			analogWrite(enB, direcao);
			Serial.println("FRENTE E DIREITA");
		}
	}

	snprintf(texto, sizeof(char) * TAMBUF, "Canal 0: %d -> Velocidade: %d	|	Canal 1: %d -> Direcao: %d",
		pot_1, velocidade, pot_2, direcao);

	Serial.println(texto);
}
