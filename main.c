//
//  新居浜高専ＰＩＣマイコン学習キットVer.2
//
//      (2013 Oct.)
//      Copyright: 新居浜高専 電子制御工学科 出口幹雄
//

#include "usada109.h"

void init_timer1(void);
void init_timer2(void);
void Dynamic_scan( void );
void decTimer( void );
bool chk1ms( void );
extern BYTE msFlag2;
extern FuncT *job[];

static taskInfo task;

////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// メイン
void main( void ){
    SYSTEM_Initialize();

    /////// ポートの初期値
    sPDA.TX1   = DIR_OUT;    
    sPDA.LS138 = DIR_OUT;
    sPOA.LS138 = Seg8bit;
    sPDC.Reg   = DIR_OUT;   
    sPOC.Reg   = LED_OFF;   //　LEDは消灯
    sPDB.CHARGE = sPDB.DISCRG = DIR_OUT;

    ////////////////////////// 初期ボタンの情報をLEDに表示
    BYTE sw = getSwitch();
    POC.G_RED = ~sw;
    while( getSwitch() );   // スイッチを離すまで待つ
    POC.G_RED = LED_OFF;

    //// タイマー設定
    init_timer1();
    init_timer2();

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    sw = 8;
    makeTask( &task,job[sw] );
    
    printf("\r\nProgram Start for [%d]",sw);

    for(;;){
        Dynamic_scan();    
        decTimer();
        random(0);          // 乱数をシャッフル
        
        if( !chk1ms() ) continue;
        
        //// 1msごとの処理 ////
        makeSwitch();
        execTask();         // 各タスクの処理
    }
}


bool chk1ms( void ){
    if(!msFlag2) return false;
    register BYTE ie = INTCON0bits.GIE;     // 割込みフラグを退避
    INTERRUPT_GlobalInterruptDisable();     // 割込み禁止
    msFlag2--;
    INTCON0bits.GIE = ie;
    return true;
}

///////////////////////////////////////////////////
FuncT *job[] = {
    Clock,          // #0
    BinaryCount,    // #1    
    Wave,           // #2
    ElectricPiano,  // #3
    TouchArarm,     // #4
    BellInsect,     // #5
    IllusionLight,  // #6
    CandleLight,    // #7
    Moguratataki,   // #8
    Theremin,       // #9
    MorningBird,    // #10
    Saikoro,        // #11
    SoundSwitch,    // #12
    ADconverter,    // #13
    Dentaku,        // #14
    Timer,          // #15
}; 
    //    プログラムの選択
    // SW4    SW3    SW2    SW1
    //  -      -      -      -     #0 Clock()
    //  -      -      -      ON    #1 BinaryCount()
    //  -      -      ON     -     #2 Wave()
    //  -      -      ON     ON    #3 ElectricPiano()
    //  -      ON     -      -     #4 TouchArarm()
    //  -      ON     -      ON    #5 BellInsect()
    //  -      ON     ON     -     #6 IllusionLight()
    //  -      ON     ON     ON    #7 CandleLight()
    //  ON     -      -      -     #8 Moguratataki()
    //  ON     -      -      ON    #9 Theremin()
    //  ON     -      ON     -     #10 MorningBird()
    //  ON     -      ON     ON    #11 Saikoro()
    //  ON     ON     -      -     #12 SoundSwitch()
    //  ON     ON     -      ON    #13 ADconverter()
    //  ON     ON     ON     -     #14 RCservo()
    //  ON     ON     ON     ON    #15 Timer()

//eof