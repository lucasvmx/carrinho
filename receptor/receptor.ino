/*
	Se trata de um carrinho que se movimenta como um tanque. Para virar, um lado precisa diminuir de velocidade.
	Já testei num carrinho e as seguintes funções estão funcionando: frente, parado, trás (ré), para direita parado e para esquerda parado.
	Não estou conseguindo fazer o carrinho virar enquanto se locomove, precisa parar, direcionar o carrinho e se locomover.
*/

#include "RF24.h"
#include "nRF24L01.h"
#define CE_PIN 9
#define CSN_PIN 10

#define TAMBUF	128

RF24 moduloRF(CE_PIN, CSN_PIN);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int canal[2];
char texto[TAMBUF];

int in1 = 22;
int in2 = 23;
int in3 = 24;
int in4 = 25;
int enA = 2;
int enB = 3;

//	DADOS RECEBIDOS
int pot_1 = 0;
int pot_2 = 0;

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
	moduloRF.setChannel(100);
	moduloRF.startListening();
	Serial.begin(9600);
}

void loop()
{
	moduloRF.openReadingPipe(1,pipe);

	if (moduloRF.available())
	{
		bool done = false;
		while(!done)
		{
			moduloRF.read(canal, sizeof(canal));
			done = moduloRF.available();
			
			pot_1 = canal[0];
			pot_2 = canal[1];
		}
	}

	// PARA TRÁS E ESQUERDA
	if ((canal[0] >= tra_Val_Min)&&(canal[0] <= tra_Val_Max)&&(canal[1] >= esq_Val_Min)&&(canal[1] <= esq_Val_Max))
	{
		velocidade = map(canal[0],tra_Val_Min,tra_Val_Max,0,255);
		direcao = map (canal[1], esq_Val_Max,esq_Val_Min, 0,255);
		digitalWrite(in1,HIGH);
		digitalWrite(in2,LOW);
		digitalWrite(in3,HIGH);
		digitalWrite(in4,LOW);
		analogWrite(enA,direcao);
		analogWrite(enB,velocidade);
	}

	// PARA TRÁS E RETO
	if ((canal[0] >= tra_Val_Min)&&(canal[0] <= tra_Val_Max)&&(canal[1] >= ret_Val_Min) && (canal[1] <= ret_Val_Max))
	{
		velocidade = map(canal[0],tra_Val_Max,tra_Val_Min,0,255);
		digitalWrite(in1,HIGH);
		digitalWrite(in2,LOW);
		digitalWrite(in3,HIGH);
		digitalWrite(in4,LOW);
		analogWrite(enA,velocidade);
		analogWrite(enB,velocidade);
	}

	// PARA TRÁS E DIREITA
	if ((canal[0] >= tra_Val_Min)&&(canal[0] <= tra_Val_Max)&&(canal[1] >= dir_Val_Min) && (canal[1] <= dir_Val_Max))
	{
		velocidade = map(canal[0],tra_Val_Min,tra_Val_Max,0,255);
		direcao = map(canal[1],dir_Val_Min,dir_Val_Max,0,255);
		digitalWrite(in1,HIGH);
		digitalWrite(in2,LOW);
		digitalWrite(in3,HIGH);
		digitalWrite(in4,LOW);
		analogWrite(enA,velocidade);
		analogWrite(enB,direcao);
	}

	// PARADO E ESQUERDA
	if((canal[0] >= par_Val_Min) && (canal[0] <= par_Val_Max)&&(canal[1] >= esq_Val_Min) && (canal[1] <= esq_Val_Max))
	{
		direcao = map (canal[1], esq_Val_Max,esq_Val_Min, 0,255);
		digitalWrite(in1,HIGH);
		digitalWrite(in2,LOW);
		digitalWrite(in3,LOW);
		digitalWrite(in4,HIGH);
		analogWrite(enA,direcao);
		analogWrite(enB,direcao);
	}

	// PARADO TOTALMENTE
	if((canal[0] >= par_Val_Min) && (canal[0] <= par_Val_Max)&&(canal[1] >= ret_Val_Min) && (canal[1] <= ret_Val_Max))
	{
		digitalWrite(in1,LOW);
		digitalWrite(in2,LOW);
		digitalWrite(in3,LOW);
		digitalWrite(in4,LOW);
		analogWrite(enA,LOW);
		analogWrite(enB,LOW);
	}

	//PARADO E DIREITA
	if((canal[0] >= par_Val_Min) && (canal[0] <= par_Val_Max)&&(canal[1] >= dir_Val_Min) && (canal[1] <= dir_Val_Max))
	{
		direcao = map (canal[1], dir_Val_Min,dir_Val_Max, 0,255);
		digitalWrite(in1,LOW);
		digitalWrite(in2,HIGH);
		digitalWrite(in3,HIGH);
		digitalWrite(in4,LOW);
		analogWrite(enA,direcao);
		analogWrite(enB,direcao);
	}

	// FRENTE E ESQUERDA
	if((canal[0] >= fre_Val_Min) && (canal[0] <= fre_Val_Max)&&(canal[1] >= esq_Val_Min)&&(canal[1] <= esq_Val_Max))
	{
		velocidade = map(canal[0],fre_Val_Min,fre_Val_Max,0,255);
		direcao = map (canal[1], esq_Val_Max,esq_Val_Min, 0,255);
		digitalWrite(in1,LOW);
		digitalWrite(in2,HIGH);
		digitalWrite(in3,LOW);
		digitalWrite(in4,HIGH);
		analogWrite(enA,direcao);
		analogWrite(enB,velocidade);
	}

	//FRENTE E RETO
	if ((canal[0] >= fre_Val_Min)&&(canal[0] <= fre_Val_Max)&&(canal[1] >= ret_Val_Min) && (canal[1] <= ret_Val_Max))
	{
		velocidade = map(canal[0],fre_Val_Min,fre_Val_Max,0,255);
		digitalWrite(in1,LOW);
		digitalWrite(in2,HIGH);
		digitalWrite(in3,LOW);
		digitalWrite(in4,HIGH);
		analogWrite(enA,velocidade);
		analogWrite(enB,velocidade);
	}

	//FRENTE E DIREITA
	if ((canal[0] >= fre_Val_Min)&&(canal[0] <= fre_Val_Max)&&(canal[1] >= dir_Val_Min) && (canal[1] <= dir_Val_Max))
	{
		velocidade = map(canal[0],fre_Val_Min,fre_Val_Max,0,255);
		direcao = map(canal[1],dir_Val_Min,dir_Val_Max,0,255);
		digitalWrite(in1,LOW);
		digitalWrite(in2,HIGH);
		digitalWrite(in3,LOW);
		digitalWrite(in4,HIGH);
		analogWrite(enA,velocidade);
		analogWrite(enB,direcao);
	}

	snprintf(texto, sizeof(char) * TAMBUF, "Canal 0: %d -> Velocidade: %d	|	Canal 1: %d -> Direcao: %d",
		canal[0], velocidade, canal[1], direcao);

	Serial.println(texto);
}
