/* 
 * File:   BinaryCount.c
 * Author: usada109
 *
 */

#include "usada109.h"

//////////////////////////////////////////////////////////////////////////////////////
//  #1  Binary Count
//////////////////////////////////////////////////////////////////////////////////////
//
// a : カウンタ
// SW1で a をカウントアップ
// SW2で a をカウントダウン
// SW3で a を0クリア
// SW4で a を１秒毎にカウントアップのスタート／ストップ
// a の値を8bitLEDで２進数表示、７セグメントLED（右側）で１６進数表示
//
void BinaryCount(taskInfo* info){
    #define w (*info)
    static BYTE a,c;
    
    if(w.init){
        printf("\r\nInitialize BinaryCount");
        printf("\r\ncompile timestamp %s %s",__DATE__,__TIME__);

        PDB.TOUCH = DIR_OUT;    // RB5を出力に変更
        stopSound();
        a = 0;
        w.state = 0;
        sec_flag = 0;
    }
    
    if(swTrig.SW1) a++;
    if(swTrig.SW2) a--;
    if(swTrig.SW3) a=0;
    if(swTrig.SW4) w.state ^= 1;
    

    if(sec_flag){       // when 1 second passed
       sec_flag--;

       if(w.state) a++;
    }

    struct RC rc;
    
    rc.Reg = ~a;    
    setLED(Seg8bit,10,rc);
    rc.Reg = LED_OFF;
    rc.SEG7 = segdata[a & 15];
    setLED(SegRight,10,rc);
    rc.SEG7 = segdata[a >> 4];
    setLED(SegMRight,10,rc);
}

//eof