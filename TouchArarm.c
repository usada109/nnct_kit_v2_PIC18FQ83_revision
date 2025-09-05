/* 
 * File:   TouchArarm.c
 * Author: usada109
 *
 */

#include "usada109.h"

//////////////////////////////////////////////////////////////////////////////////////
//   #4 Touch Ararm
//////////////////////////////////////////////////////////////////////////////////////
//
// RB5 が L になったらアラームを鳴らす
// プルアップ抵抗が高抵抗のため、電位が中途半端になりやすいので、
// このプログラム以外では、RB5 は出力に設定しておく
//

static taskInfo task;

void TouchArarm(taskInfo* info){
    #define w (*info)
    
    if(w.init){
        printf("\r\nInitialize ToutchArarm");
        printf("\r\ncompile timestamp %s %s",__DATE__,__TIME__);
        
        PDB.TOUCH = DIR_IN;
        WPUB5 = 0;              // 内部プルアップを解除
        //TMR1_StopTimer();
        
        w.state = 0;
    }
    
    
    switch(w.state){
        
        case 0:
            if(PIB.TOUCH == 1) break;   // RB5＝Lとなったらアラームを鳴らす
            makeTask( &task,ararm );
            w.state = 1;
            break;
            
        case 1:
            break;           
    }
}


//eof