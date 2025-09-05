/* 
 * File:   MorningBird.c
 * Author: usada109
 *
 * Created on July 2, 2020, 8:53 PM
 */

#include "usada109.h"

//////////////////////////////////////////////////////////////////////////////////////
//   #10 MorningBird
//////////////////////////////////////////////////////////////////////////////////////
//
// 一旦暗くなって（夜になって）から、明るくなる（朝が来る）とピヨピヨ音が鳴り始める
// ピヨピヨ音が鳴る間隔はランダム
// CdSの電圧はアナログ値で読み取り、暗くなったと判断するレベルと
// 明るくなったと判断するレベルに差をつけることで、ヒステリシスを持たせている
//
void MorningBird(taskInfo* info){
    printf("\r\nInitialize MorningBird");
    printf("\r\ncompile timestamp %s %s",__DATE__,__TIME__);

    unsigned int ad,c;
    unsigned char a, b;

    PDB.CHARGE = DIR_OUT;       // make RB2 output
    POB.CHARGE = 0;
    
    
    TMR1_StopTimer();
    
    ad = 0;
    while(ad < 600*4){          // wait darkness
        ADCC_StartConversion( 1 );
        while(!ADCC_IsConversionDone());
        ad = ADCC_GetConversionResult();
    }
    while(ad > 300*4){          // wait brightness
        ADCC_StartConversion( 1 );
        while(!ADCC_IsConversionDone());
        ad = ADCC_GetConversionResult();
    }

    while(1){

        for(b = 0; b < 4; b++){   // ピヨピヨピヨピヨ
            for(c = 0; c < 300; c++){   // 高い音（振動１５０回）
                POB.SPEAK ^= 1;
                wait(27);
            }
            for(c = 0; c < 300; c++){
                POB.SPEAK ^= 1;
                wait(30);
            }
            for(c = 0; c < 300; c++){
                POB.SPEAK ^= 1;
                wait(33);
            }
            for(c = 0; c < 300; c++){
                POB.SPEAK ^= 1;
                wait(34);
            }
            for(c = 0; c < 300; c++){   // 低い音（振動１５０回）
                POB.SPEAK ^= 1;
                wait(37);
            }
        }

        a = 0;
        b = 0x0F & random(15);   // 鳴く間隔をランダムに決める
        b += 3;   // 間隔は最低３秒
        while(a < b){
            if(sec_flag){
                sec_flag--;
                a++;
            }
        }
    }
}

//eof