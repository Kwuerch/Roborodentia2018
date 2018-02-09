#include "delay.h"

void delay_1_us(){
    int i;
    for(i = 0; i < 2; i++){ }
}

void delay_10_us(){
    int i, j;
    for(i = 0, j = 0; i < 38; i++){
        j++;
    }
}

void delay_100_us(){
    int i, j;
    for(i = 0, j = 0; i < 398; i++){
        j++;
    }
}

void delay_ms(int ms){
    int i, j;
    for(i = 0; i < ms; i++){
        for(j=0; j < DELAY_MS_60MHZ_CONST; j++){}
    }
}

