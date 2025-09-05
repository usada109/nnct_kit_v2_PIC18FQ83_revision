/* 
 * File:   ElectricPiano.c
 * Author: usada109
 *
 */
#include "usada109.h"


//////////////////////////////////////////////////////////////////////////////////////
//   #3 Electric Piano
//////////////////////////////////////////////////////////////////////////////////////
//
// このプログラムのみ PORTC を入力として使う
// TMR1割り込み毎に RB4 の H/L を入れ替えて音信号を作る
// t1h_0 : TMR1初期値の上位バイト
// t1l_0 : TMR1初期値の下位バイト
// TMR1の初期値を大きくするほど、割り込み周期が短くなるので音が高くなる
// RB2 を L にすると、圧電ブザーを駆動するトランジスタのコレクタ電圧が充電される
// RB3 を L にすると、コレクタ電圧を一定の時定数で放電するので、ピアノライクな減衰音になる
//
void ElectricPiano(taskInfo* info){
    #define w (*info)
    if(w.init){
        printf("\r\nInitialize ElectricPiano");
        printf("\r\ncompile timestamp %s %s",__DATE__,__TIME__);

        sPDB.CHARGE = sPDB.DISCRG = sPDB.TOUCH = DIR_OUT;  
        PDC.Reg = DIR_IN;       // make PORTC all input

        struct RC rc;
        rc.Reg = LED_OFF;
        setLED(Seg8bit,200,rc);
    }
    
    if(PORTCbits.RC0 == 0){ // do2
        setSound(0xED57);
    }
    if(PORTCbits.RC1 == 0){ // si
        setSound(0xEC3A);
    }
    if(PORTCbits.RC2 == 0){ // ra
        setSound(0xE9CE);
    }
    if(PORTCbits.RC3 == 0){ // so
        setSound(0xE716);
    }
    if(PORTCbits.RC4 == 0){ // fa
        setSound(0xE409);
    }
    if(PORTCbits.RC5 == 0){ // mi
        setSound(0xE25F);
    }
    if(PORTCbits.RC6 == 0){ //re
        setSound(0xDEC0);
    }
    if(PORTCbits.RC7 == 0){ //do1
        setSound(0xDAAB);
    }

}

    //
    //     f[Hz]   5MHz/f   ÷2=x  65536-x   t1h_0   t1l_0
    // do1 261.6   19113    9557    55979    0xDA    0xAB
    // re  293.7   17024    8512    57024    0xDE    0xC0
    // mi  329.6   15170    7585    57951    0xE2    0x5F
    // fa  349.2   14318    7159    58377    0xE4    0x09
    // so  392.0   12755    6378    59158    0xE7    0x16
    // ra  440     11364    5682    59854    0xE9    0xCE
    // si  493.9   10124    5062    60474    0xEC    0x3A
    // do2 523.3    9555    4777    60759    0xED    0x57
    //

//eof