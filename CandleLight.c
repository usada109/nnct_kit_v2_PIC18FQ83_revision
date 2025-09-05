/* 
 * File:   CandleLight.c
 * Author: usada109
 *
 */

#include "usada109.h"

//////////////////////////////////////////////////////////////////////////////////////
//   #7 Candle light
//////////////////////////////////////////////////////////////////////////////////////
//
// フルカラーLEDを黄色に点灯し、duty比を擬似的な「1/fゆらぎ」で変化させる
// 光をボーッと拡散するランプシェードをかぶせると雰囲気が出る
// duty比を時間に対して直線的に変化させ、増加／減少を切り替えるまでの時間をランダムに決めることで
// 擬似的な1/fゆらぎを作る
// 時々、dutyに急激な変化を与え、炎が風でゆらめく効果を模擬する
//
void CandleLight(taskInfo* info){
    printf("\r\nInitialize CandleLight");
    printf("\r\ncompile timestamp %s %s",__DATE__,__TIME__);

    TMR1_StopTimer();
    TMR2_StopTimer();
    
    unsigned char i,j,k;
    unsigned char duty, flag, priod;

    PDB.TOUCH = DIR_OUT;    // RB5を出力に変更
    
    
    POA.LS138 = SegColor;   // select full color LED
    i = 0;
    duty = 128;             // duty の初期値
    priod = random(16);     // duty の増減を切り替えるまでの周期
    flag = 0;
    while(1){

        for(j = 0; j < 20; j++){            // PWMのパルスを２０個
            for(k = 0; k < 255; k++){       // PWMの処理
                if(k <= duty) POC.GREEN = POC.RED = LED_ON; // R,G on（黄色）
                else POC.Reg = LED_OFF;     // R,G off
            }
        }

        
        if(++i >= priod){ // duty の増減を切り替える時がきたか
            i = 0;
            priod = random(16);    // 次に切り替えるまでの時間はランダム
            if((duty > 0) && (duty < 255)) flag ^= 1;
        }

        if(flag == 0){      // dutyを増加させる場合
            if(++duty == 255) flag = 1;
        }
        else{               // dutyを減少させる場合
            if(--duty ==   0) flag = 0;
        }

        if(random(250) == 0) duty -= 50;   // 時々dutyを急激に変化させる
    }
}

//eof