/* 
 * File:   Theremin.c
 * Author: usada109
 *
 */

#include "usada109.h"

//////////////////////////////////////////////////////////////////////////////////////
//   #9 Theremin
//////////////////////////////////////////////////////////////////////////////////////
//
// CdSの電圧レベルに応じてTMR1の初期値を変え、音の高さを変化させる
// 暗くするほどCdSの抵抗が大きくなるので電圧レベルがあがり、音が高くなる
// 同時に、8bitLEDで電圧レベルをバーグラフ表示する
//
//
void Theremin(taskInfo* info){
    #define w (*info)

    if(w.init){
        printf("\r\nInitialize Theremin");
        printf("\r\ncompile timestamp %s %s",__DATE__,__TIME__);

        sPDB.TOUCH = sPDB.CHARGE = sPDB.DISCRG = DIR_OUT;
        POB.CHARGE = 0;         // charge（コレクタ電圧を供給）
        w.state = 0;
    }

    unsigned int a;
    struct RC rc;

    
    switch(w.state){
        case 0:
            ADCC_StartConversion( 1 );
            w.state++;
            break;
        
        case 1:
            if( !ADCC_IsConversionDone() )break;
            w.state++;
            break;
        case 2:
            a = ADCC_GetConversionResult() / 4;
            
            if(a < 200)      rc.Reg = 0b11111111;   // CdSの電圧レベルに応じてバーグラフ表示
            else if(a < 280) rc.Reg = 0b11111110;
            else if(a < 360) rc.Reg = 0b11111100;
            else if(a < 440) rc.Reg = 0b11111000;
            else if(a < 520) rc.Reg = 0b11110000;
            else if(a < 600) rc.Reg = 0b11100000;
            else if(a < 680) rc.Reg = 0b11000000;
            else if(a < 860) rc.Reg = 0b10000000;
            else             rc.Reg = 0b00000000;
            setLED(Seg8bit,10,rc);

            setBeep((a<<3) + 0xE000);
            w.state = 0;
            break;
    }
}

//eof
