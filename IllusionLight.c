/* 
 * File:   IllusionLight.c
 * Author: usada109
 *
 */

#include "usada109.h"

//////////////////////////////////////////////////////////////////////////////////////
//   #6 Illusion light
//////////////////////////////////////////////////////////////////////////////////////
//
// フルカラーLEDの R, G, B のduty比をランダムに増減する
// 光を乱反射するランプシェード（例えば、サランラップをクシャクシャにしたものなど）をかぶせると雰囲気が出る
//
//
void IllusionLight(taskInfo* info){
    
    printf("\r\nInitialize IllusionLight");
    printf("\r\ncompile timestamp %s %s",__DATE__,__TIME__);

    unsigned char R_duty, G_duty, B_duty;
    unsigned char R_ip, G_ip, B_ip;
    unsigned char R_f, G_f, B_f;
    unsigned char i, j, d;

    PDB.TOUCH = DIR_OUT;        // RB5を出力に変更
    
    TMR1_StopTimer();
    TMR2_StopTimer();
    
    
    POA.LS138 = SegColor;       // select full color LED
    R_duty = random(255);          // R duty
    G_duty = random(255);          // G duty
    B_duty = random(255);          // B duty
    R_ip = random(16);     // R duty increment period
    G_ip = random(16);     // G duty increment period
    B_ip = random(16);     // B duty increment period
    R_f = 0;                    // R duty inc/dec flag  0: increment, 1: decrement
    G_f = 0;                    // G duty inc/dec flag
    B_f = 0;                    // B duty inc/dec flag

    i = 0;

    while(1){

        struct RC d;
        d.Reg = POC.Reg;
        d.BLUE = d.GREEN = d.RED = LED_OFF;
        for(j = 0; j < 255; j++){
            if(j <= R_duty) d.RED   = LED_ON;
            else            d.RED   = LED_OFF;
            if(j <= G_duty) d.GREEN = LED_ON;
            else            d.GREEN = LED_OFF; 
            if(j <= B_duty) d.BLUE  = LED_ON;
            else            d.BLUE  = LED_OFF;
            POC.Reg = d.Reg;
        }

        if(i >= R_ip){          // if it is time to change duty,
            if(R_f == 0){       // 0 means increase duty
                if(R_duty < 255) R_duty++;  // if not yet full duty, increase duty.
                else R_f = 1;   // if reach full duty, change to decrement.
            }
            else{               // case of decrease duty
                if(R_duty > 0){ // if duty is not zero,
                    R_duty--;   // decrease duty.
                }
                else{
                    R_f = 0;    // if duty has become zero, change to increment.
                    R_ip = random(16); // and change the period of duty inc/dec
                }
            }
        }
        if(i >= G_ip){
            if(G_f == 0){
                if(G_duty < 255) G_duty++;
                else G_f = 1;
            }
            else{
                if(G_duty > 0){
                    G_duty--;
                }
                else{
                    G_f = 0;
                    G_ip = random(16);
               }
            }
        }
        if(i >= B_ip){
            if(B_f == 0){
                if(B_duty < 255) B_duty++;
                else B_f = 1;
            }
            else{
                if(B_duty > 0){
                    B_duty--;
                }
                else{
                    B_f = 0;
                    B_ip = random(16);
                }
            }
        }

        i++;    // count up the couter
        i &= 0x0F;
    }
}


//eof