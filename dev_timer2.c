#include "usada109.h"

// 時計の１秒の長さを決める定数
#define TIMECONST 5000          // 標準値は 5000   値を大きくすると遅れ、小さくすると進みになる



const BYTE segdata[]={ // ０--F ==> セグメントデータ変換用の配列
    0b11000000, 0b11111001, 0b10100100, 0b10110000,   // 0, 1, 2, 3,
    0b10011001, 0b10010010, 0b10000010, 0b11011000,   // 4, 5, 6, 7,
    0b10000000, 0b10010000, 0b10001000, 0b10000011,   // 8, 9, A, b,
    0b11000110, 0b10100001, 0b10000110, 0b10001110    // C, d, E, F
};


WORD t2_icnt;                   // TMR2割り込み回数のカウンタ
BYTE sec_flag;                  // １秒経過のフラグ

static BYTE scanValue;
static BYTE msFlag;
BYTE msFlag2;

static struct{
    BYTE time;
    BYTE select;
    struct RC rc;
}DynamicWrk[8];


void setLED(BYTE Seg,BYTE kido,struct RC prt){
    DynamicWrk[Seg].time = kido;
    DynamicWrk[Seg].rc   = prt;
}

/////////////////////////////////////////////////////////
///////////////////////////////////////////// 割り込み処理
static void isr_timer2( void ){

    if(++t2_icnt > TIMECONST){
        t2_icnt   = 0;
        sec_flag += 1;
    }
    scanValue++;
    msFlag++;
}

void init_timer2( void ){
    TMR2_SetInterruptHandler( isr_timer2 );
    for(BYTE i=0;i<8;i++){
        DynamicWrk[i].rc.Reg = LED_OFF;
        DynamicWrk[i].time   = 0;
    }
    TMR2_StartTimer();    
}
 

//////////////////////////////////////////////////////////////
////////////////////////////////////////// ダイナミック点灯処理
void Dynamic_scan( void ){
    static BYTE scan;

    if(scanValue < DynamicWrk[scan].time) return;
    scanValue = 0;
    if(++scan >= 7) scan=0;
        
    register BYTE ie = INTCON0bits.GIE;     // 割込みフラグを退避
    INTERRUPT_GlobalInterruptDisable();     // 割込み禁止
    POC.Reg   = LED_OFF;
    POA.LS138 = SegNone;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");         // トランジスタの切れが悪いので時間を消費
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    POA.LS138 = scan;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");         // LS138のグリッジノイズの為、時間を消費
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    POC.Reg   = DynamicWrk[scan].rc.Reg;
    INTCON0bits.GIE = ie;                   // 割込みフラグを復帰
}

/////////////////////////////////////////////////////////////
///////////////////////////////////////////////// タイマー処理
static usrTmr* urtmr;
static cbTmr* cbtmr;

void decTimer( void ){
    if(msFlag < 5) return;
    register BYTE ie = INTCON0bits.GIE;     // 割込みフラグを退避
    INTERRUPT_GlobalInterruptDisable();     // 割込み禁止
    msFlag -= 5;
    INTCON0bits.GIE = ie;                   // 割込みフラグを復帰

    for(usrTmr* tmr=urtmr;tmr;tmr=tmr->Next){
        if(tmr->time){
            tmr->time--;
        }
    }
    for(cbTmr* tmr=cbtmr;tmr;tmr=tmr->Next){
        if(tmr->time){
            if(!--tmr->time){
                tmr->task();                // 時間になったら呼び出す
            }
        }
    }
    msFlag2++;
}


void setTimer(usrTmr* wrk,WORD val){
    wrk->time = val;
    wrk->Reg  = 0;
    
    for(usrTmr* tmr=urtmr;tmr;tmr=tmr->Next)
        if(tmr == wrk) return;      // 既にリンク済み
    /// リンクを結ぶ
    wrk->Next = urtmr;
    urtmr     = wrk;
}

bool chkTimer(usrTmr* wrk){
    return (wrk->time)? true:false;
}

bool chkTimerOnce(usrTmr* wrk){
    if( wrk->once ) return true;
    if( wrk->time ) return true;
    wrk->once = 1;
    return false;
}

void CallbackTimer(cbTmr* wrk,Func* call,WORD val){
    wrk->time = val;
    wrk->task = call;

    for(cbTmr* tmr=cbtmr;tmr;tmr=tmr->Next)
        if(tmr == wrk) return;      // 既にリンク済み
    /// リンクを結ぶ
    wrk->Next = cbtmr;
    cbtmr     = wrk;
}

//eof