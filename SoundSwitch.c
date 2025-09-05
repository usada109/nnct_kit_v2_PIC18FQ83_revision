/* 
 * File:   SoundSwitch.c
 * Author: usada109
 *
 */

#include "usada109.h"

//////////////////////////////////////////////////////////////////////////////////////
//   #12 Sound Switch
//////////////////////////////////////////////////////////////////////////////////////
//
// 大きな音をマイクで拾うと RA2 が H になる
// 音が入ると、8bitLEDの光る位置が１つずつ左シフトする（デフォルト、SW1）
// SW2を押すと、音が入る毎にフルカラーLEDの色が変わる
// SW3を押すと、音が入った回数が７セグメントLED（右側）に表示される
// マイクの近くで手を叩く、あるいはマイクをトントンとタッピングするとよい
//

inline static BYTE chkSwitch(BYTE sel){
    return getSwitch() & (1<<sel);
}



void SoundSwitch(taskInfo* info){
    printf("\r\nInitialize SoundSwitch");
    printf("\r\ncompile timestamp %s %s",__DATE__,__TIME__);

    unsigned char s, n, mode, i;
    unsigned char p10, p1;
    WORD waitCnt;

    TMR1_StopTimer();
    TMR2_StopTimer();

    PDB.TOUCH = DIR_OUT;    // RB5を出力に変更


    s = 0;
    n = 0;
    mode = 0;
    sPOC.Reg   = LED_OFF;
    sPOA.LS138 = Seg8bit;    // select 8-LEDs

    PDA.MIC    = DIR_IN;
    
    p1 = 0;
    p10 = 0;
    i = 0;

    printf("\r\n Start SoundSwitch");

    while(1){

        if(mode == 0){                  // 8bitLEDの場合
            if(PIA.MIC == 1){           // 音が鳴った場合
                if(n == 0) n = 1;       // 1bit左シフト
                else n <<= 1;
                while(PIA.MIC == 1);    // 信号レベルが下がるまで待つ
                POC.Reg = ~n;
                waitCnt = 0;
                while(++waitCnt < 2500);// チャタリング防止
            }
        }
        else if(mode == 1){
            if(PIA.MIC == 1){           // 音が鳴った場合
                n++;                    // カウントアップ
                while(PIA.MIC == 1);    // 信号レベルが下がるまで待つ
                POC.Reg = ~n;
                waitCnt = 0;
                while(++waitCnt < 2500);// チャタリング防止
            }
        }
        else if(mode == 3){
            if(PIA.MIC == 1){           // 音が鳴った場合
                p1++;                   // 回数をカウントアップ
                if(p1 == 10){
                    p1 = 0;
                    p10++;
                }
                while(PIA.MIC == 1);    // 信号レベルが下がるまで待つ
                waitCnt = 0;
                while(++waitCnt < 2500);// チャタリング防止
            }
            if(i < 128){                // ７セグメントLED（右側）をダイナミック点灯
                POC.Reg   = LED_OFF;
                POA.LS138 = SegMRight;
                POC.SEG7  = segdata[p10];
            }
            else{
                POC.Reg   = LED_OFF;
                POA.LS138 = SegRight;
                POC.SEG7  = segdata[p1];
            }
            i++;
        }
        

        //
        if( chkSwitch(0) ){             // SW1が押された場合
            mode = 0;
            n    = 0;
            POC.Reg   = LED_OFF;
            POA.LS138 = Seg8bit;        // select 8-LEDs
            while( chkSwitch(0) );
        }
        if( chkSwitch(1) ){             // SW2が押された場合
            mode = 1;
            n    = 0;
            POC.Reg   = LED_OFF;
            POA.LS138 = SegColor;       // select full color LED
            while( chkSwitch(1) );
        }
        if( chkSwitch(2) ){             // SW3が押された場合
            mode = 3;
            p1   = 0;
            p10  = 0;
            while( chkSwitch(2) );
        }
    }
}

//eof