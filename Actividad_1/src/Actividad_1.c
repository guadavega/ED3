// Ejercicio: P0.0 salida - repetir esta secuencia 1010100
// con un periodo de 10 ms
// P1.0 entrada - cuando detecta un 1, la secuencia cambia a 0101101

#include "LPC17xx.h"

#define SECUENCIA_1 0b1011100
#define SECUENCIA_2 0b0101101

#define REDLED 		(1<<22)

void config_GPIO();
void delay (unsigned int count);
void rotar ();
void leer_cambio();
void mostrar (const short int secuencia);
void reset();


short int secuencia_actual = SECUENCIA_1;
uint8_t currentState;
uint8_t previousState = 1;                          //comienza en un estado en alto
short int cont = 0;

int main(void) {
	config_GPIO();

	while(1){

		for(cont=0; cont<7; cont++){
			leer_cambio();
			mostrar(secuencia_actual);
			delay(10);
			rotar();
		}
		reset();
	}
    return 0 ;
}


void config_GPIO(){

	LPC_PINCON -> PINSEL1 &= ~(3<<12);       //configuramos el pin como GPIO
	LPC_GPIO0  -> FIODIR |= REDLED;         //configuramos el pin como salida

	LPC_PINCON -> PINSEL4 &= ~(0b11<<20);   //configuramos el pin como GPIO
	LPC_GPIO2  -> FIODIR1 &= ~(0b1<<2);     //configuramos la dirección del pin como entrada

}


void delay (unsigned int count){
	for(int i=0;i<count;i++){
		for(int j=0;j<1000;j++);
	}
}

void rotar (){
	secuencia_actual = secuencia_actual >> 1;
}

void leer_cambio(){
	currentState = (LPC_GPIO2 -> FIOPIN1 >>2)  & 1;
	if(currentState != previousState){
			reset();
	}
	previousState = currentState;
}

void mostrar (const short int secuencia)
{
	if( (1 & secuencia) == 1 )             //solo comparamos el bit 0 (si es igual a 1)
	{
		LPC_GPIO0->FIOCLR |= REDLED;      //encendemos el led
	}else{
		LPC_GPIO0->FIOSET |= REDLED;      //apagamos el led
	}
}

void reset(){
	if(currentState == 1){
			secuencia_actual = SECUENCIA_1;
	}else{
			secuencia_actual = SECUENCIA_2;
	}
	cont = 0;
}
