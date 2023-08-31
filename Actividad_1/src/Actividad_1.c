// Ejercicio: P0.0 salida - repetir esta secuencia 1010100
// con un periodo de 10 ms
// P1.0 entrada - cuando detecta un 1, la secuencia cambia a 0101101

#include "LPC17xx.h"

#define SECUENCIA_1 0b1010100
#define SECUENCIA_2 0b0101101

void config_GPIO();
void delay (unsigned int count);
void rotar (short int secuencia);
void leer_cambio();
void mostrar (const short int secuencia);
void reset();
short int secuencia_actual = SECUENCIA_1;
uint8_t status = 0;

int main(void) {
	config_GPIO();

	while(1){
		for(int cont=0;cont<7;cont++){
			leer_cambio();
			mostrar(secuencia_actual);
			delay(1000);
			rotar(secuencia_actual);
		}
		reset();
	}
    return 0 ;
}
void config_GPIO(){
	LPC_PINCON -> PINSEL0 &= ~(0b11<<0); //configuramos el pin P0.0 para que funcione como GPIO
	LPC_PINCON -> PINSEL2 &= ~(0b11<<0);
	LPC_GPIO0  -> FIODIR0 |= (1<<0);    //configuración como salida del P0.0
	LPC_GPIO1  -> FIODIR1 &= ~(0b1<<0); //configuración como entrada del P1.0
	LPC_PINCON -> PINMODE2 |= (0b11<<0); //habilitamos la pull_down para la entrada
	LPC_GPIO0  -> FIOMASK0 &= ~(1<<0);   //fijamos una mascara para no afectar los otros puertos
}
void delay (unsigned int count){
	for(int i=0;i<count;i++){
		for(int j=0;j<1000;j++);
	}
}
void rotar (short int secuencia){
	secuencia = secuencia >> 1;
}
void leer_cambio(){
	if(LPC_GPIO1 -> FIOPIN0 && status){
			reset();
			status = LPC_GPIO1 -> FIOPIN0;
		}
}
void mostrar (const short int secuencia){
	LPC_GPIO0 -> FIOPIN0 = secuencia;
}
void reset(){
	if(LPC_GPIO1 -> FIOPIN0 && 1){
			secuencia_actual = SECUENCIA_2;
			}else{
				secuencia_actual = SECUENCIA_1;
			}
}
