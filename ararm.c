#include "usada109.h"


/////////////////////////////////////////////////////////////////////
void ararm(taskInfo* info){
    #define w (*info)
    static struct RC rc;
    
    if(w.destroy){
        stopSound();
        rc.Reg = LED_OFF;
        setLED(Seg8bit,0,rc);
        return;
    }
    
    if(w.init){
        w.state = 0;
        stopSound();
        
        rc.Reg = LED_OFF;
        setLED(Seg8bit,0,rc);
        return;
    }
    
    static usrTmr timer;
    static BYTE cnt;
    
    switch(w.state){
 
        case 0:
            rc.Reg = 0x55;
            cnt = 10;
            w.state++;
        case 1:
            setBeep(65536 - 875*2);
            setTimer(&timer,40);
            w.state++;
            break;
        case 2:
            if( chkTimerOnce(&timer) )break;
            w.state++;
            break;
        case 3:
            setBeep(65536 - 1166*2);
            setTimer(&timer,60);
            w.state++;
            break;
        case 4:
            if( chkTimerOnce(&timer) )break;
            if(!--cnt){
                w.state++;
                stopSound();
                rc.Reg = LED_OFF;
                break;
            }
            rc.Reg ^= 0xFF; // 反転
            w.state = 1;      // 10回繰り返し
            break;
        case 5:
            setTimer(&timer,1000);
            w.state++;
            break;
        case 6:
            if( chkTimerOnce(&timer) )break;
            w.state = 0;
            break;
    }
    
    setLED(Seg8bit,10,rc);
}


//eof