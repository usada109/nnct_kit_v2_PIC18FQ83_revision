
#include "usada109.h"


/*
  * PCコンソールからシリアルで入力された計算結果をコンソールに返します。 
  */

WORD calc,work;
char sisoku;

static void disp(WORD val);




const char *kijutu = "*+/-\nc0123456789";





////////////////////////////////////////////////////////////////
void Dentaku(taskInfo* info){
    #define w (*info)
    
    if(w.init){
        printf("\r\nInitialize Dentaku");
        printf("\r\ncompile timestamp %s %s\r\n",__DATE__,__TIME__);

        stopSound();
        PDB.TOUCH   = DIR_IN;       // RB5をシリアル入力として使用
        
        calc = 0;
        disp( calc );
    }
    
    
    if( !UART1_is_rx_ready() ) return;
    
    BYTE c = UART1_Read();
    BYTE i;
    for(i=0;i<strlen(kijutu);i++){
        if(c == kijutu[i]) break;
    }
    if(i==strlen(kijutu)) return;   // 使用外文字
    
    UART1_Write( c );
    
    if((c>='0')&&(c<='9')){
        if(calc >= 1000) return;
        
        calc *= 10;
        calc += c - '0';
        disp( calc );
        return;
    }
    
    if(c == 'c'){
        calc = 0;
        disp( calc );
        return;
    }
    
    if((c=='*')||(c=='/')||(c=='+')||(c=='-')){
        sisoku = c;
        work = calc;
        calc = 0;
        return;
    }
    
    
    if( c=='\n' ){    //
        switch( sisoku ){
            case '*':
                work *= calc;
                disp(work);
                printf("answer = %d",work);
                break;
            case '+':
                work += calc;
                disp(work);
                printf("answer = %d",work);
                break;
            case '-':
                work -= calc;
                disp(work);
                printf("answer = %d",work);
                break;
            case '/':
                work /= calc;
                disp(work);
                printf("answer = %d",work);
                break;
        }
        
    }
}


//////////////////////////////////////////////////////////
static void disp(WORD val){
    bin16_to_bcd( val );
    
    struct RC rc;
    
    rc.Reg  = LED_OFF;
    if( bcd[3] ) rc.SEG7 = segdata[bcd[3]];  
    setLED(SegLeft,10,rc);          // 1000の位

    if( bcd[2] ||(rc.Reg != LED_OFF) ) rc.SEG7 = segdata[bcd[2]];   
    setLED(SegMLeft,10,rc);         // 100の位       

    if( bcd[1] ||(rc.Reg != LED_OFF) ) rc.SEG7 = segdata[bcd[1]];    
    setLED(SegMRight,10,rc);        // 10の位

    rc.SEG7 = segdata[bcd[0]];    
    setLED(SegRight,10,rc);         // 1の位
} 


//eof