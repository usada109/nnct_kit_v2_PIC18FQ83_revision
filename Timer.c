/* 
 * File:   Timer.c
 * Author: usada109
 *
 */

#include "usada109.h"

//////////////////////////////////////////////////////////////////////////////////////
//   #15 Timer
//////////////////////////////////////////////////////////////////////////////////////
//
// SW4 を押しながら SW3 で時間の値の設定（１ずつ増加するだけ）
// SW4 を押しながら SW2 で10分の位の値の設定（１ずつ増加するだけ）
// SW4 を押しながら SW1 で1分の位の値の設定（１ずつ増加するだけ）
// SW1 でスタート／ポーズ
// カウント終了でアラーム、電源切るまで鳴り続ける
// 一旦カウントをスタートさせると、途中でポーズはできるが設定時間の変更はできない
// １秒毎にサイコロLEDが点滅、5秒毎に点滅箇所が移動、30秒経過後は中央の赤LEDも一緒に点滅
//


typedef enum{
    stand_by,
    running,
    end,
    pause,
}State;


static taskInfo task;

void Timer(taskInfo* info){
    #define w (*info)
    
    static BYTE hour10, hour1, min10, min1, s;
    
    if(w.init){
        printf("\r\nInitialize Timer");
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
        w.state = stand_by;
    }
    

                
    ////////////////////////////////////////////////////
    for(;sec_flag;sec_flag--){
        if(w.state != running) continue;
        
        s++;                    // 秒の値をカウントアップ
        if(s == 60){            // １分経過時の処理
            s = 0;
            if(min1 > 0){
                min1--;         // 分の値をカウントダウン
                if(min1 == 0){
                    if(min10 == 0){
                        if(hour1 == 0){
                            if(hour10 == 0) w.state = end; // 全部ゼロになったら終了
                        }
                    }
                }
            }
            else{               // case min1 = 0
                if(min10 > 0){
                    min10--;
                    min1 = 9;
                }
                else{               // case min10 = 0
                    if(hour1 > 0){
                        hour1--;
                        min10 = 5;
                        min1  = 9;
                    }
                else{   // case hour1 = 0
                    if(hour10 > 0){
                        hour10--;
                        hour1 = 9;
                        min10 = 5;
                        min1  = 9;
                    }
                    else{
                        w.state = end; // end of time count
                        }
                    }
                }
            }
        }
    }
    
    
    if(w.state == stand_by){      // 時間のセット
        if(swLevel.SW4){        // SW4が押されている場合
            if(swTrig.SW3){     // SW3も押されている場合
                hour1++;
                if(hour1 == 10){
                    hour1 = 0;
                    if(++hour10 == 10) hour10 = 0;
                }
            }
            if(swTrig.SW2){     // SW2も押されている場合
                if(++min10 == 6) min10 = 0;
            }
            if(swTrig.SW1){     // SW1も押されている場合
                if(++min1 == 10) min1 = 0;
            }
        }
        if(!swLevel.SW4){       // SW4が離されている場合
            if(swTrig.SW1){     // SW1が押された場合
                if((min1 > 0) || (min10 > 0) || (hour1 > 0) || (hour10 > 0)){
                    w.state   = running;
                    t2_icnt = 0;
                    s       = 0;
                }
            }
        }
    }
    //
    if(w.state == running){       // カウント中にSW1が押されたらポーズ
        if(swTrig.SW1){
            w.state = pause;
        }
    }
    //
    if(w.state == 2) makeTask( &task,ararm );    // カウント終了したらアラームを鳴らす
    //
    if(w.state == pause){         // ポーズ中にSW1が押されたら再スタート
        if(swTrig.SW1){
            w.state = running;
        }            
    }
    
    ///////////////////////////////////////////
    ////////////////////////////////////// 表示
    struct RC rc;
    rc.Reg = LED_OFF;
           
    if(w.state == running){
        if(t2_icnt < 2500){     // サイコロLEDの点灯は0.5秒間のみ
            if(s <= 5)       rc.D15 = LED_ON;
            else if(s <= 10) rc.D16 = LED_ON;
            else if(s <= 15) rc.D17 = LED_ON;
            else if(s <= 20) rc.D14 = LED_ON;
            else if(s <= 25) rc.D13 = LED_ON;
            else if(s <= 30) rc.D12 = LED_ON;
            else if(s <= 35) {rc.D3 = LED_ON; rc.D15 = LED_ON;}   // 30秒経過後は中央の赤LEDも一緒に点滅
            else if(s <= 40) {rc.D3 = LED_ON; rc.D16 = LED_ON;}
            else if(s <= 45) {rc.D3 = LED_ON; rc.D17 = LED_ON;}
            else if(s <= 50) {rc.D3 = LED_ON; rc.D14 = LED_ON;}
            else if(s <= 55) {rc.D3 = LED_ON; rc.D13 = LED_ON;}
            else             {rc.D3 = LED_ON; rc.D12 = LED_ON;}
        }
    }
    setLED(SegDice,10,rc);
    
    
    rc.Reg = LED_OFF;
    if(hour10 > 0) rc.SEG7 = segdata[hour10];
    setLED(SegLeft,10,rc);
                
    rc.SEG7  = segdata[hour1];  
    rc.CORON = LED_ON;        // 「：」は常時点灯
    setLED(SegMLeft,10,rc);
                
    rc.SEG7  = segdata[min10];
    setLED(SegMRight,10,rc);

    rc.SEG7  = segdata[min1];
    setLED(SegRight,10,rc);
}

//eof