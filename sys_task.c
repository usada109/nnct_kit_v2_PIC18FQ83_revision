

#include "usada109.h"

static taskInfo* tcb;

void execTask( void ){
    for(taskInfo* tsk=tcb;tsk;tsk=tsk->Next){
        FuncT* addr = (FuncT*)tsk->task;
        if(addr) addr( tsk );
        tsk->Reg = 0;
    }
}

void makeTask(taskInfo* wrk,FuncT* addr){
    /// 他と重複しないIDを生成
    BYTE tcnt=1,book;
    do{
        book=0;
        for(taskInfo* tsk=tcb;tsk;tsk=tsk->Next){
            if(tsk->id == tcnt){
                tcnt++;
                book = 1;           // IDが重複したのでやり直し
                break;
            }            
        }
    }while(book);
            
    wrk->task  = (Func*)addr;
    wrk->id    = tcnt;
    wrk->state = 0;
    wrk->Reg   = 0;
    wrk->init  = 1;                 // タスクに初期化を促す
    
    for(taskInfo* tsk=tcb;tsk;tsk=tsk->Next)
        if(tsk == wrk) return;      // 既にリンク済み
    /// リンクを結ぶ
    wrk->Next = tcb;
    tcb       = wrk;
}

static void destroy(taskInfo* wrk){
    wrk->init    = 1;
    wrk->destroy = 1;               // 後処理を促す

    FuncT* addr = (FuncT*)wrk->task;
    if(addr) addr( wrk );           // 初期化または後処理

    /// クリア ///
    wrk->Reg  = 0;
    wrk->task = (void*)0;
    wrk->id   = 0;
}

void deleteTask(taskInfo* wrk){
    taskInfo* tsk = (taskInfo*)&tcb;
    do{
        if(tsk->Next == wrk){
            tsk->Next = wrk->Next;  // リンクを解く
            destroy( wrk );
            return;
        }
        tsk = tsk->Next;
    }while(tsk);
}

void deleteTaskId(BYTE id){
    for(taskInfo* tsk=tcb;tsk;tsk=tsk->Next){
        if(tsk->id == id){
            deleteTask(tsk);
            return;
        }
    }
}


//eof
