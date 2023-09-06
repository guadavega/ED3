/*
 * Realizar un programa que configure el puerto P0.0 y P2.0 para que provoquen
 * una interrupción por flanco de subida. Si la interrupción es por P0.0 guardar
 * el valor binario 100111 en la variable "auxiliar", si es por P2.0 guardar el
 * valor binario 111001011010110.
 */

#include "LPC17xx.h"

void config_GPIO();
void NVICEnableIRQn(IRQn_Type IRQn);
void EINT3Handler();

#define SECUENCIA1 0b100111;
#define SECUENCIA2 0b111001011010110;

uint32_t auxiliar = 0;


int main(void) {

	NVIC_EnableIRQ(EINT3_IRQn);
	NVIC_SetPriority(EINT3_IRQn, 0x01);

	LPC_GPIOINT -> IO2IntEnR |=1;         //habilitamos la interrupción por flanco de subida en P0.0
	LPC_GPIOINT -> IO2IntEnR |=1;         //habilitamos la interrupción por flanco de subida en P2.0

	while(1) {


  }
    return 0 ;
}

void config_GPIO(){
	LPC_PINCON -> PINSEL0 &= ~(0b11<<0);   //configuramos el P0.0 como GPIO
	LPC_PINCON -> PINSEL4 &= ~(0b11<<0);   //configuramos el P2.0 como GPIO

	LPC_GPIO0  -> FIODIR0 &= ~(0b11<<0);            //configuramos como entrada P0.0
	LPC_GPIO0  -> FIODIR0 &= ~(0b11<<0);            //configuramos como entrada P2.0

	LPC_PINCON -> PINMODE0 &= ~(0b11<<0);  //habilitamos las pull-up en P0.0
	LPC_PINCON -> PINMODE4 &= ~(0b11<<0);  //habilitamos las pull-up en P2.0
}
void EINT3Handler(){
	if((LPC_GPIOINT -> IO2IntStatR >> 0) & 1){    //si se detecta la interrupción por P0.0 auxiliar guarda la secuencia1
		auxiliar = SECUENCIA1;
		LPC_GPIOINT -> IO2IntClr |= 1;          //bajamos la bandera
	}else{
		auxiliar = SECUENCIA2;
		LPC_GPIOINT -> IO2IntClr |= 1;
	}

}

