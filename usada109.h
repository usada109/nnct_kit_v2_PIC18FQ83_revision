#pragma once

//https://github.com/usada109/ 提供

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include "mcc_generated_files/mcc.h"


typedef unsigned char BYTE; 
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned long long u64;
//typedef unsigned char bool;

typedef union{
    BYTE Reg;
    struct{
        BYTE SW1   :1;
        BYTE SW2   :1;
        BYTE SW3   :1;
        BYTE SW4   :1;
    };
    struct{
        BYTE SW_1_4 :4;
    };
}tSwitch;


typedef void Func( void );
typedef void Func1( BYTE ); 


#include "def_port.h"
#include "dev_timer2.h"
#include "sys_task.h"


extern BYTE bcd[5];             // bin16_to_bcd()関数の戻り値
extern WORD t2_icnt;            // TMR2割り込み回数のカウンタ
extern BYTE sec_flag;           // １秒経過のフラグ
extern tSwitch swLevel,swTrig;
extern const BYTE segdata[];    // ０--F ==> セグメントデータ変換用の配列


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    
    // 関数のプロトタイプ宣言
// プログラム
void Clock(taskInfo* info);           // #0
void BinaryCount(taskInfo* info);     // #1
void Wave(taskInfo* info);            // #2
void ElectricPiano(taskInfo* info);   // #3
void TouchArarm(taskInfo* info);      // #4
void BellInsect(taskInfo* info);      // #5
void IllusionLight(taskInfo* info);   // #6
void CandleLight(taskInfo* info);     // #7
void Moguratataki(taskInfo* info);    // #8
void Theremin(taskInfo* info);        // #9
void MorningBird(taskInfo* info);     // #10
void Saikoro(taskInfo* info);         // #11
void SoundSwitch(taskInfo* info);     // #12
void ADconverter(taskInfo* info);     // #13
void Dentaku(taskInfo* info);         // #14
void Timer(taskInfo* info);           // #15
// サブルーチン
void wait(unsigned char n);
void L_wait(unsigned char n);
void ararm(taskInfo* info);
unsigned char random(BYTE);   
void bin16_to_bcd(unsigned int bin16);
BYTE getSwitch( void );    
void makeSwitch( void );
void setSound(WORD val);
void setBeep(WORD val);
void stopSound( void );


#ifdef	__cplusplus
}
#endif /* __cplusplus */

//eof


