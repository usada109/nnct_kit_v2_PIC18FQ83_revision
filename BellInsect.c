/* 
 * File:   BellInsect.c
 * Author: usada109
 *
 */

#include "usada109.h"

//////////////////////////////////////////////////////////////////////////////////////
//   #5 Bell Insect
//////////////////////////////////////////////////////////////////////////////////////
//
// RB2 を L とし、圧電ブザー駆動トランジスタのコレクタ電圧を充電した後そのままにしておくと、
// 圧電ブザーの駆動によってじわじわと放電するので、時定数の長い減衰音になる
// 周期の異なる振動を繰り返すことでビブラートをかける
// 少し音を低めにして、コレクタ電圧を充電したまま持続音ににするとコオロギライクな音になる
// 暗くなるとCdSの抵抗が大きくなるので、RA1 が H となる
// すず虫音は７回鳴って８回分の長さ休憩
// 休憩中に暗くなると、コオロギ音に変わる
// コオロギ音が休憩中に暗くなると、すず虫音に戻る
//
void BellInsect(taskInfo* info){

    
    printf("\r\nInitialize BellInsect");
    printf("\r\ncompile timestamp %s %s",__DATE__,__TIME__);
    
    TMR1_StopTimer();
    TMR2_StopTimer();
    
    unsigned char a, b, c;
    unsigned char status;


    ANSELA = 0x01;                  // make RA1 digital

    sPDB.CHARGE = sPDB.DISCRG = DIR_OUT;
    sPOB.DISCRG = 1;
    
    status = 0;

    while(1){
        if(status == 0){                // Bell insect
            for(a = 0; a < 15; a++){
                if(a < 7){              // ring 7 times
                    
                    POB.CHARGE = 0;     // charge
                    L_wait(10);
                    POB.CHARGE = 1;     // charge

                }
                for(b = 0; b < 70; b++){        // ビブラート７０回
                    for(c = 0; c < 200; c++){   // 高い方の音（振動１００回）
                        POB.SPEAK ^= 1;
                        wait(30);
                    }
                    for(c = 0; c < 200; c++){   // 低い方の音（振動１００回）
                        POB.SPEAK ^= 1;
                        wait(45);
                    }
                }
            }
            if(PIA.CDS == 1) status = 1;  // 暗くなっていたらコオロギ音に変更
        }
        
        
        
        
        if(status == 1){                        // Cricket
            for(a = 0; a < 8; a++){
                for(b = 0; b < 10; b++){        // ビブラート１０回
                    for(c = 0; c < 200; c++){   // 高い方の音（振動１００回）
                        POB.SPEAK ^= 1;
                        wait(32);
                    }
                    for(c = 0; c < 200; c++){   // 低い方の音（振動１００回）
                        POB.SPEAK ^= 1;
                        wait(48);
                    }
                }
                L_wait(100) ;
            }
            for(a = 0; a < 100; a++) L_wait(250);
            if(PIA.CDS == 1) status = 0;  // 暗くなっていたらすず虫音に変更
        }
    }
}


//eof