#pragma once
#include <xc.h> // include processor files - each processor file is guarded.  


typedef struct{
    void *Next;
    BYTE id;
    union{
        BYTE Reg;
        struct{
            BYTE init:1;
            BYTE destroy:1;
        };
    };
    BYTE state;
    Func* task;
}taskInfo;

typedef void FuncT( taskInfo* );

void makeTask( taskInfo*,FuncT* );
void execTask( void );
void deleteTaskId(BYTE id);
void deleteTask(taskInfo*);

//eof

