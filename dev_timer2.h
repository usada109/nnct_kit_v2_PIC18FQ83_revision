#pragma once
#include <xc.h> // include processor files - each processor file is guarded.  

typedef struct{
    void* Next;
    WORD time;
    union{
        BYTE Reg;
        struct{
            BYTE once:1;
        };
    };
}usrTmr;
typedef struct{
    void* Next;
    WORD  time;
    Func* task;
}cbTmr;

void setLED(BYTE Seg,BYTE kido,struct RC prt);
void setTimer(usrTmr*,WORD);
bool chkTimer(usrTmr*);
bool chkTimerOnce(usrTmr*);
void CallbackTimer(cbTmr*,Func*,WORD);

//eof

