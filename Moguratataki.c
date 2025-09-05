/* 
 * File:   Moguratataki.c
 * Author: usada109
 *
 */

#include "usada109.h"

//////////////////////////////////////////////////////////////////////////////////////
//   #8 Moguratataki
//////////////////////////////////////////////////////////////////////////////////////
//
// 8bitLEDのどれかが１つ光る
// 光ったbitの番号（0 -- 7）を３桁の２進数で表した時 1 になる桁を SW3, SW2, SW1 を押して答える
// 右端（bit 0 ）が光った時は、SW4 を押す
// スイッチは 0.3秒以上押さえる
// 正解なら「ピン・ポン」、間違えると「ブーー」が鳴る
// 正解数が７セグメントLED（右側）に表示される
//
void Moguratataki(taskInfo* info){
    #define w (*info)
    static BYTE a, b, n;
    static BYTE p10, p1;

    if(w.init){
        printf("\r\nInitialize Moguratataki");
        printf("\r\ncompile timestamp %s %s",__DATE__,__TIME__);

        stopSound();
        PDB.TOUCH   = DIR_OUT;      // RB5を出力に変更
        w.state = 0;                // 0: initiallize, 1: wait , 2: MOGURA appears, 3: SW pressed
        p10 = 0;                    // 正解数の１０の位
        p1 = 0;                     // 正解数の１の位
    }

    static usrTmr tmr0,tmr1,tmr2;
    
    switch(w.state){
        case 0:                         // initiallize
            stopSound();                // SW3が押されると勝手にチャージされて、
                                        // 前回音が鳴ってしまうので周波数を０にセット
            n = random(8);
            b = 1;
            if(n > 0) b <<= n;
            w.state = 1;
            setTimer(&tmr0,2000);
            setTimer(&tmr1,5000);
            break;
        case 1:
            if( chkTimerOnce(&tmr0) )break;  // ２秒待ってから光らせる
            w.state = 2;
            break;
        case 2:
            if( !chkTimerOnce(&tmr1) ){
                w.state = 0;   // 時間切れ、次の問題へ
                printf("\r\n時間切れ");
                break;
            }
            if(swLevel.SW_1_4){
                w.state = 3;
                setTimer(&tmr0,333);    // SW must be pressed for more than 0.3s
            }
            break;
        case 3:
            if( chkTimerOnce(&tmr0) )break;
            a = swLevel.SW_1_4;
            if(a){
                if(a == n){
                    w.state = 4;    // correct
                }
                else if((n == 0) && (a == 8)){
                    w.state = 4;    // correct
                }
                else{
                    w.state = 6;    // wrong
                }
            }
            else{
                w.state = 6;
            }
            break;
        case 4:                     // 正解の場合
            if(!swLevel.SW_1_4){    // スイッチが離されるまで待つ
                w.state = 5;        // if SW is released

                printf("\r\n正解");
                setTimer(&tmr0,10);
                setTimer(&tmr1,500);
                setTimer(&tmr2,1500);
            }
            break;
        case 5:                     // 正解してスイッチが離された場合
            if( !chkTimerOnce(&tmr0) ){
                setSound(0xEC3A);   // si
                if(++p1 == 10){     // ポイントをカウントアップ
                    p1= 0;
                    p10++;
                }                
            }
            if( !chkTimerOnce(&tmr1) ){
                setSound(0xE716);   // so
            }
            if( !chkTimerOnce(&tmr2) ){
                w.state = 0;        // 元に戻る
            }
            break;
        case 6:                     // 不正解の場合
            if(!swLevel.SW_1_4){    // if SW is released
                printf("\r\nダメ");
                w.state = 7;
                setTimer(&tmr0,10);
                setTimer(&tmr1,800);
                setTimer(&tmr2,1500);
            }
            break;
        case 7:             // 不正解でスイッチが離された場合
            if( !chkTimerOnce(&tmr0) ){
                setBeep(0xDAAB);
            }
            if( !chkTimerOnce(&tmr1) ){
                stopSound();
            }
            if( !chkTimerOnce(&tmr2) ){
                w.state = 0;
            }
            break;
    }
    
    //////////////////////////////////////////////////
    ///////////////////////////////////////////// 表示
    struct RC rc;
    rc.Reg = LED_OFF;

    rc.SEG7  = segdata[p10];
    setLED(SegMRight,10,rc);
    
    rc.SEG7  = segdata[p1];
    setLED(SegRight,10,rc);
    
    rc.Reg = LED_OFF;
    if(w.state >= 2) rc.Reg = ~b;  // MOGURA appears
    setLED(Seg8bit,10,rc);           
}

//eof