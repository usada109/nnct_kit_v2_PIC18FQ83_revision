/* 
 * File:   Saikoro.c
 * Author: usada109
 *
 * Created on July 2, 2020, 8:53 PM
 */

#include "usada109.h"

//////////////////////////////////////////////////////////////////////////////////////
//   #11 Denshi Saikoro
//////////////////////////////////////////////////////////////////////////////////////
//
// ７セグメントLEDを８の字ダンス状に光らせてルーレット表示
// SW1を押すと、その瞬間の値がサイコロの目で表示される
// 訓練すれば「目押し」ができるようになり、動体視力を養うことができるかも...
//
void Saikoro(taskInfo* info){
    #define w (*info)
    
    if(w.init){
        printf("\r\nInitialize Saikoro");
        printf("\r\ncompile timestamp %s %s",__DATE__,__TIME__);

        PDB.TOUCH = DIR_OUT;        // RB5を出力に変更
        stopSound();
        w.state=0;
    }
    static usrTmr timer;
    
    struct RC rc0,rc1,rc2,rc3;
    rc0.Reg = LED_OFF;
    rc1.Reg = LED_OFF;
    rc2.Reg = LED_OFF;
    rc3.Reg = LED_OFF;
    
    BYTE set=0;
    switch(w.state){
        case 1:
        case 3:
        case 5:
        case 7:
        case 9:
        case 11:
        case 13:
        case 15:
        case 17:
        case 19:
        case 21:
        case 23:
        case 25:
        case 27:
        case 29:
            if(chkTimerOnce(&timer)) break;
            w.state++;
            break;


        case 0:
            set=1;
            rc0.SEGA  = LED_ON;
            rc3.SEGA  = LED_ON;
            setTimer(&timer,50);
            w.state++;
            break;
        case 2:
            set=1;
            rc3.SEGB  = LED_ON;
            rc0.SEGF  = LED_ON;
            setTimer(&timer,50);
            w.state++;
            break;
        case 4:
            set=1;
            rc3.SEGC  = LED_ON;
            rc0.SEGE  = LED_ON;
            setTimer(&timer,50);
            w.state++;
            break;
        case 6:
            set=1;
            rc3.SEGD  = LED_ON;
            rc0.SEGD  = LED_ON;
            setTimer(&timer,50);
            w.state++;
           break;
        case 8:
            set=1;
            rc2.SEGC  = LED_ON;
            rc1.SEGE  = LED_ON;
            setTimer(&timer,50);
            w.state++;
            break;
        case 10:
            set=1;
            rc2.SEGB  = LED_ON;
            rc1.SEGF  = LED_ON;
            setTimer(&timer,50);
            w.state++;
            break;
        case 12:
            set=1;
            rc2.SEGA  = LED_ON;
            rc1.SEGA  = LED_ON;
            setTimer(&timer,50);
            w.state++;
            break;
        case 14:
            set=1;
            rc2.SEGF  = LED_ON;
            rc1.SEGB  = LED_ON;
            setTimer(&timer,50);
            w.state++;
            break;
        case 16:
            set=1;
            rc2.SEGE  = LED_ON;
            rc1.SEGC  = LED_ON;
            setTimer(&timer,50);
            w.state++;
            break;
        case 18:
            set=1;
            rc2.SEGD  = LED_ON;
            rc1.SEGD  = LED_ON;
            setTimer(&timer,50);
            w.state++;
            break;
        case 20:
            set=1;
            rc3.SEGE  = LED_ON;
            rc0.SEGC  = LED_ON;
            setTimer(&timer,50);
            w.state++;
            break;
        case 22:
            set=1;
            rc3.SEGF  = LED_ON;
            rc0.SEGB  = LED_ON;
            setTimer(&timer,50);
            w.state++;
            break;
        case 24:
            w.state=0;
            break;
            
        default:
            set=1;
            break;
    }

    if(set){
        setLED(SegLeft  ,10,rc0);
        setLED(SegMLeft ,10,rc1);
        setLED(SegMRight,10,rc2);
        setLED(SegRight ,10,rc3);
    }

    
    ///////////////////////////////////////////// 操作
    static BYTE ctrl;
    static BYTE dice;
    if(w.init){
        ctrl = 0;
    }
    

    struct RC rc;
    rc.Reg = LED_OFF;
    
    switch(ctrl){
        case 0:
            if(swTrig.SW1){
                ctrl++;
                w.state = 30;
                dice  = random(6);
            }
            break;

        case 1:
            if     (dice == 0) rc.D3  = LED_ON;
            else if(dice == 1) rc.D15 = rc.D14 = LED_ON;
            else if(dice == 2) rc.D15 = rc.D14 = rc.D3  = LED_ON;
            else if(dice == 3) rc.D15 = rc.D14 = rc.D12 = rc.D17 = LED_ON;
            else if(dice == 4) rc.D15 = rc.D14 = rc.D12 = rc.D17 = rc.D3  = LED_ON;
            else if(dice == 5) rc.D15 = rc.D14 = rc.D12 = rc.D17 = rc.D13 = rc.D16 = LED_ON;

            if(swTrig.SW1){
                ctrl=0;
                w.state = 0;
            }
            break;
    }
    setLED(SegDice,10,rc);
}

//eof