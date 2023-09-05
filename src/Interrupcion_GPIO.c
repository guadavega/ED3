/*
 * Realizar un programa que configure el puerto P2.0 y P2.1 para que
 * provoquen una interrupción por flanco de subida para el primer pin y
 * por flanco de bajada para el segundo. Cuando la interrupción sea por
 * P2.0 se enviará por el pin P0.0 la secuencia de bits 010011010.
 * Si la interrupción es por P2.1 se enviará por el pin P0.1 la
 * secuencia 011100110. Las secuencias se envían únicamente cuando se produce
 * una interrupción, en caso contrario la salida de los pines tienen valores 1 lógicos.
 */

#include "LPC17xx.h"

#define SECUENCIA_1  0b010011010
#define SECUENCIA_2  0b011100110
#define SECUENCIA_ON 0b111111111

uint16_t secuencia_actual = SECUENCIA_ON;
uint16_t secuencia_actual2 = SECUENCIA_ON;
uint8_t  i = 0;

void NVIC_EnableIRQ(IRQn_Type IRQn);
void EINT3_IRQHandler(void);
void config_GPIO(void);
void mostrar();
void rotar();
void reset();

int main(void) {

NVIC_EnableIRQ(EINT3_IRQn);
NVIC_SetPriority(EINT3_IRQn, 0x01);

	LPC_GPIOINT->IO2IntEnR |= 1;           //habilitamos la interrupción en P2.0
    LPC_GPIOINT->IO2IntEnF |= (0b1<<1);    //habilitamos la interrupción en P2.1

    while(1){
    	for(i=0; i<9; i++){
    		mostrar();
    		rotar();
    	}
    	reset();
    }
    return 0 ;
}

void config_GPIO(void){

	LPC_PINCON -> PINSEL0 &= ~(0b11<<0);   //configuramos como GPIO P0.0
	LPC_GPIO0  -> FIODIR0 |= 1;            //configuramos como salida el P0.0
	LPC_PINCON -> PINSEL0 &= ~(3<<2); 	   //configuramos como GPIO P0.1
	LPC_GPIO0  -> FIODIR0 |= (0b1<<1);     //configuramos como salida el P0.1

	LPC_PINCON -> PINSEL4 &= ~(0b11<<0);  //configuramos como GPIO P2.0
	LPC_GPIO2  -> FIODIR0 |= 1;           //configuramos el P2.0 como salida
	LPC_PINCON -> PINSEL4 &= ~(3<<2);     //configuramos como GPIO P2.1
	LPC_GPIO2  -> FIODIR0 |= (0b1<<1);    //configuramos el P2.1 como salida

	LPC_PINCON  -> PINMODE4 &= ~(0b11<<0); //configuramos pull-up en P2.0
	LPC_PINCON  -> PINMODE4 &= ~(3<<2);    //configuramos pull-up en P2.1

}

void EINT3_IRQHandler(void){

	if((LPC_GPIOINT -> IO2IntStatR >> 0 & 1) == 1){
		secuencia_actual = SECUENCIA_1;
		LPC_GPIOINT -> IO2IntClr |= 1;    //bajamos la bandera
	}else if (LPC_GPIOINT -> IO2IntStatF >> 1 & 1){
		secuencia_actual2 = SECUENCIA_2;
		LPC_GPIOINT -> IO2IntClr |= (0b1<<1);    //bajamos la bandera
	}else {
		secuencia_actual = SECUENCIA_ON;
		secuencia_actual2 = SECUENCIA_ON;
	}
	i=0;                   //para asegurarnos de mostrar los 9 bits, reseteamos la secuencia

}
void mostrar(){
	LPC_GPIO2 -> FIOSET = secuencia_actual & 1;

	if((secuencia_actual & 1) == 0){
		LPC_GPIO2 -> FIOCLR = secuencia_actual & 1;
	}else{
		  LPC_GPIO2 -> FIOSET = secuencia_actual & 1;
}
	if((secuencia_actual2 & 1) == 0){
		LPC_GPIO2 -> FIOCLR = secuencia_actual2 & 1;
	}else{
		LPC_GPIO2 -> FIOSET = secuencia_actual2 & 1;
	}

}
void rotar(){
	secuencia_actual = secuencia_actual >> 1;
	secuencia_actual2 = secuencia_actual2 >> 1;
}
void reset(){
	secuencia_actual = SECUENCIA_ON;
	secuencia_actual2 = SECUENCIA_ON;
}
