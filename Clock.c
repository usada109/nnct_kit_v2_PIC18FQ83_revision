/* 
 * File:   clock.c
 * Author: usada109
 *
 */

#include "usada109.h"

//////////////////////////////////////////////////////////////////////////////////////
//   #0 Clock
//////////////////////////////////////////////////////////////////////////////////////
//
// 単純なデジタル時計（12時間表示）、７セグメントLEDに時刻を表示
// SW4 を押しながら SW3 で時間の値の設定（１ずつ増加するだけ）
// SW4 を押しながら SW2 で10分の位の値の設定（１ずつ増加するだけ）
// SW4 を押しながら SW1 で1分の位の値の設定（１ずつ増加するだけ）
// SW4/SW1 で1分の位を変更した瞬間に秒のカウントがリセットされる
// 進む時は TIMECONST の値を微増、遅れる時は微減して調節
// さらに微妙な調節をするためには、若干進み気味に TIMECONST を設定した上で、
// OSC2ピンにつながる 15pF のセラミックコンデンサと並列に10pF程度のコンデンサ
// あるいはトリマコンデンサをつないで微調節する
//
// TMR2割り込み（200us周期）5000回毎に sec_falg が 1 になるので、
// それを見て s をカウントアップ。
// s が 60 になったら、時間の値をカウントアップ。
// hour10 : 時間の10の位
// hour1  : 時間の1の位
// min10  : 分の10の位
// min1   : 分の1の位
//
void Clock(taskInfo* info){
    #define w (*info)   
    static BYTE hour10, hour1, min10, min1, s;

    if(w.init){
        printf("\r\nInitialize Clock");
        printf("\r\ncompile timestamp %s %s",__DATE__,__TIME__);

        PDB.TOUCH = DIR_OUT;    // RB5を出力に変更
        stopSound();
        hour10 = 0;
        hour1 = 0;
        min10 = 0;
        min1 = 0;
        s = 0;
        sec_flag = 0;
        t2_icnt = 0;
    }
        

    /////////////////////////////////////////////////////
    //////////////////////////////////////////// 計時処理        
    while(sec_flag){        // １秒経過時の処理
        sec_flag--;
        if(++s == 60){      // １分経過時の処理
            s = 0;
            if(++min1 == 10){
                min1 = 0;
                if(++min10 == 6){
                    min10 = 0;
                    hour1++;
                    if(hour10 == 0){
                        if(hour1 == 10){
                            hour1 = 0;
                            hour10++;
                        }
                    }
                    else{
                        if(hour1 == 2){     // case 12:00
                            hour10 = 0;
                            hour1 = 0;
                            min10 = 0;
                            min1 = 0;
                        }
                    }
                }
            }
        }
    }  


    ///////////////////////////////////////////////////////////////
    if(swLevel.SW4){            // SW4を押している場合
        if(swTrig.SW3){         // SW3も押している場合
            hour1++;
            if(hour10 == 0){
                if(hour1 == 10){
                    hour1 = 0;
                    hour10++;
                }
            }
            else{   // case hour10 = 1
                if(hour1 == 2){
                    hour1 = 0;
                    hour10 = 0;
                }
            }
        }
        if(swTrig.SW2){         // SW2も押している場合
            if(++min10 == 6) min10 = 0;
        }
        if(swTrig.SW1){         // SW1も押している場合
            if(++min1 == 10) min1 = 0;
            s = 0;
            t2_icnt = 0;
        }
    }
    
      
    //////////////////////////////////////
    ///////////////////////////////// 表示
    struct RC rc;
    rc.Reg = LED_OFF;
    
    if(hour10 == 1) rc.SEG7 = segdata[hour10];      // 時間は１０の位が０の時は点灯しない
    setLED(SegLeft,10,rc);

    rc.SEG7 = segdata[min10];
    setLED(SegMRight,10,rc);
    
    rc.SEG7 = segdata[min1];
    setLED(SegRight,10,rc);

    rc.CORON = (t2_icnt < 2500)? LED_ON:LED_OFF;    // 「：」を１秒毎に点滅
    rc.SEG7  = segdata[hour1];
    setLED(SegMLeft,10,rc);
}


//eof