/* 
 * File:   misc.c
 * Author: usada109
 *
 */

#include "usada109.h"

///////////////////////////////////////////////////////////////////////
void wait(unsigned char n){
    unsigned long nn= n*5;
    for(unsigned long i=0; i< nn; i++);   
}

void L_wait(unsigned char n){
    for(unsigned char i=0; i<n; i++){
        wait(255);
    }
}




static union{
    u64 Reg;
    BYTE b[8];
    WORD w[4];
    struct{
        BYTE   in:1;
    };
}rndwrk;
static BYTE addwrk;

static BYTE __random( void ){
    BYTE c =  rndwrk.b[7]>>3;
         c ^= rndwrk.b[7];
         c >>= 4;
    rndwrk.Reg <<= 1;
    rndwrk.in = ~c;
    addwrk += 21;
    return rndwrk.b[0] + addwrk;
}

unsigned char random(BYTE val){
    BYTE tmp = ((WORD)__random() * val) >> 8; 
    return tmp;
}


BYTE bcd[5];                    // bin16_to_bcd()関数の戻り値

void bin16_to_bcd(unsigned int bin16){
    memset(bcd,0,sizeof(bcd));

    if(!bin16) return;
    unsigned short mask;
    for(mask=0x8000;!(mask & bin16);mask>>=1);
   
    for(;mask;mask>>=1){
        BYTE carry = (bin16 & mask)? 1:0;

        for(BYTE tmp,i=0; i<sizeof(bcd); i++){
            tmp  = bcd[i]*2;
            tmp |= carry;
            if( tmp >= 10 ){
                tmp -= 10;
                  carry = 1;
            }else carry = 0;
            bcd[i] = tmp;
        }
    }
}



tSwitch swLevel,swTrig;


BYTE getSwitch( void ){
    register BYTE bk = PDB.Reg;     // ポート入出力状態退避
    PDB.SW_1_4 = DIR_IN;
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    BYTE ans = PIB.SW_1_4 ^ 0b1111;
    PDB.Reg = bk;                   // ポート入出力状態復帰
    return ans;
}

void makeSwitch( void ){
    static BYTE SwLog[4];
    swLevel.Reg  = getSwitch();
    SwLog[0] = (SwLog[0]<<1)|swLevel.SW1;
    SwLog[1] = (SwLog[1]<<1)|swLevel.SW2;
    SwLog[2] = (SwLog[2]<<1)|swLevel.SW3;
    SwLog[3] = (SwLog[3]<<1)|swLevel.SW4;
    //////////// トリガ生成
    swTrig.Reg = 0;
    if(SwLog[0] == 1) swTrig.SW1 = 1;
    if(SwLog[1] == 1) swTrig.SW2 = 1;
    if(SwLog[2] == 1) swTrig.SW3 = 1;
    if(SwLog[3] == 1) swTrig.SW4 = 1;
}


//eof