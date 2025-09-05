/* 
 * File:   Wave.c
 * Author: usada109
 *
 */

#include "usada109.h"

//////////////////////////////////////////////////////////////////////////////////////
//   #2 Wave
//////////////////////////////////////////////////////////////////////////////////////
//
// duty[n] : 8bitLEDの右から n 番目（n = 0 -- 7）のduty比
// direction = 0 の時、duty[n]をduty[n-1]にシフト
// direction = 1 の時、duty[n]をduty[n+1]にシフト
// duty比はsin波状に変化させる
// direction を変更するまでのシフト回数はランダム
//
void Wave(taskInfo* info){
    printf("\r\nInitialize Wave");
    printf("\r\ncompile timestamp %s %s",__DATE__,__TIME__);
    
    unsigned char i, j, k, m, n, d, direction;
    unsigned char duty[8];      // 各LEDのduty比
    unsigned char sine[8]={0, 37, 128, 218, 255, 218, 128, 37};

    TMR1_StopTimer();
    TMR2_StopTimer();
    
    PDB.TOUCH = DIR_OUT;        // RB5を出力に変更
    POA.LS138 = Seg8bit;        // select 8-LEDs
    direction = 0;
    
    while(1){

        m = random(32);              // 向きを変えるまでの周期はランダム
        for(n = 0; n < m; n++){
            for(j = 0; j < 10; j++){        // PWMのパルスを１０回出す
                for(i = 0; i < 255; i++){   // PWMの処理
                    struct RC d;
                    d.Reg = LED_OFF;
                    if(i <= duty[0]) d.D4 = LED_ON;
                    if(i <= duty[1]) d.D5 = LED_ON;
                    if(i <= duty[2]) d.D6 = LED_ON;
                    if(i <= duty[3]) d.D7 = LED_ON;
                    if(i <= duty[4]) d.D8 = LED_ON;
                    if(i <= duty[5]) d.D9 = LED_ON;
                    if(i <= duty[6]) d.D10= LED_ON;
                    if(i <= duty[7]) d.D11= LED_ON;
                    POC.Reg = d.Reg;
                }
            }
            if(direction == 0){
                for(i = 0; i < 7; i++){     // dutyのデータを右にシフト
                    duty[i] = duty[i+1];
                }
                duty[7] = sine[k];
            }
            else{
                for(i = 7; i > 0; i--){     // dutyのデータを左にシフト
                    duty[i] = duty[i-1];
                }
                duty[0] = sine[k];                
            }
            k++;
            if(k > 7) k = 0;
        }
        if(random(128) & 1) direction = 0;   // 向きをランダムに決める
        else direction = 1;
    }
    
}

//eof