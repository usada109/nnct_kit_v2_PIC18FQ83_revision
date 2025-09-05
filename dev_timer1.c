/*
 * File:   dev_timer1.c
 * Author: usada109
 *
 */
#include "usada109.h"

static union{
    WORD w;
    struct{
        BYTE t1l_0;
        BYTE t1h_0;
    };
}tw;

///////////////////////////////////////////// 割り込み処理
//////////////////////////////// 16bit オーバーフロー割込み
static void isr_timer1( void ){

    TMR1H = tw.t1h_0;       // t1h_0, t1l_0 の値が大きいほど、音は高くなる
    TMR1L = tw.t1l_0;       //

    if(!tw.w) return;       // 0の時は音を出さない

    POB.SPEAK ^= 1;         // RB4 を0/1入れ替え
}


static cbTmr timer;

void init_timer1(void){
    TMR1_SetInterruptHandler( isr_timer1 );
    TMR1_StopTimer();    
}

static void callback_sound( void ){
    sPOB.DISCRG = 0;        // discharge
    sPOB.CHARGE = 1;
}

void setSound(WORD val){
    tw.w = val;
    TMR1_StartTimer();    

    CallbackTimer(&timer,callback_sound,10);
    sPOB.CHARGE = 0;        // charge
    sPOB.DISCRG = 1;
}

void setBeep(WORD val){
    tw.w = val;
    TMR1_StartTimer();    
    sPOB.CHARGE = 0;        // charge
    sPOB.DISCRG = 1;
}

void stopSound( void ){
    TMR1_StopTimer();
    tw.w = 0;
    sPOB.DISCRG = 0;        // discharge
    sPOB.CHARGE = 1;
}

//eof
