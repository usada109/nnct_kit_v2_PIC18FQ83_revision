/* 
 * File:   ADconverter.c
 * Author: usada109
 *
 */

#include "usada109.h"

//////////////////////////////////////////////////////////////////////////////////////
//   #13 A/D converter
//////////////////////////////////////////////////////////////////////////////////////
//
// 可変抵抗（VR）の電圧（RA0）をA/D変換し、0 -- 4095 の数値を７セグメントLEDで表示する
// 同時に、電圧レベルを8bitLEDでバーグラフ表示する
//
void ADconverter(taskInfo* info){
    #define w (*info)
    if(w.init){
        printf("\r\nInitialize A/Dconverter");
        printf("\r\ncompile timestamp %s %s",__DATE__,__TIME__);
        stopSound();
        PDB.TOUCH = DIR_OUT;    // RB5を出力に変更
        w.state = 0;
    }
    
    WORD ad_result;
    struct RC rc;
    
    switch(w.state){
        case 0:
            ADCC_StartConversion( 0 );
            w.state++;
            break;
        case 1:
            if(ADCC_IsConversionDone()) w.state++;
            break;
        case 2:
            // A/D変換結果に応じて8bitLEDでバーグラフ表示
            ad_result = ADCC_GetConversionResult();
            
            rc.Reg = LED_OFF;
            if(ad_result >= 4*64)  rc.D4  = LED_ON;
            if(ad_result >= 4*192) rc.D5  = LED_ON;
            if(ad_result >= 4*320) rc.D6  = LED_ON;
            if(ad_result >= 4*448) rc.D7  = LED_ON;
            if(ad_result >= 4*576) rc.D8  = LED_ON;
            if(ad_result >= 4*704) rc.D9  = LED_ON;
            if(ad_result >= 4*832) rc.D10 = LED_ON;
            if(ad_result >= 4*960) rc.D11 = LED_ON;
            setLED(Seg8bit,10,rc);

            bin16_to_bcd( ad_result );      // A/D変換結果を１０進数に直す

        
            rc.Reg  = LED_OFF;
            rc.SEG7 = segdata[bcd[3]];  
            setLED(SegLeft,10,rc);          // 1000の位

            rc.SEG7 = segdata[bcd[2]];   
            setLED(SegMLeft,10,rc);         // 100の位       

            rc.SEG7 = segdata[bcd[1]];    
            setLED(SegMRight,10,rc);        // 10の位

            rc.SEG7 = segdata[bcd[0]];    
            setLED(SegRight,10,rc);         // 1の位

            w.state = 0;
            break;
    }  
}


//eof