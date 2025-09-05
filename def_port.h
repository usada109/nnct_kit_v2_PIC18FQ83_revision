
#pragma once
#include <xc.h> // include processor files - each processor file is guarded.  


#define DIR_OUT 0b00000000
#define DIR_IN  0b11111111

#define LED_OFF 0b11111111
#define LED_ON  0b00000000

#define SW_ON   0b0
#define SW_OFF  0b1

//////////////////////////////////////////////////////
//////////////////////////////////////// PORTAピン定義
struct RA{
    union{
        BYTE Reg;
        struct{
            BYTE VR1	:1;	// (02)RA0:
            BYTE CDS	:1;	// (03)RA1:
            BYTE MIC	:1;	// (04)RA2:
            BYTE LS138	:3;	// (05)RA3:
        //	BYTE    	:1;	// (06)RA4:
        //	BYTE    	:1;	// (07)RA5:
            BYTE TX1   	:1;	// (10)RA6:XOUT
            BYTE    	:1;	// (09)RA7:XIN
        };
    };
};
#define POA (*(volatile struct RA*)&LATA)
#define sPOA (*(struct RA*)&LATA)
#define PIA (*(volatile struct RA*)&PORTA)
#define sPIA (*(struct RA*)&PORTA)
#define PDA (*(volatile struct RA*)&TRISA)
#define sPDA (*(struct RA*)&TRISA)

//////////////////////////////////////////////////////
//////////////////////////////////////// PORTBピン定義
struct RB{
    union{
        BYTE Reg;
        struct{
            BYTE    	:1;	// (21)RB0:
            BYTE    	:1;	// (22)RB1:
            BYTE CHARGE	:1;	// (23)RB2:     // 0:チャージ
            BYTE DISCRG	:1;	// (24)RB3:     // 0:放電
            BYTE SPEAK  :1;	// (25)RB4:
            BYTE TOUCH	:1;	// (26)RB5:
            BYTE PGC  	:1;	// (27)RB6:ICSPCLK
            BYTE PGD 	:1;	// (28)RB7:ICSPDAT
        };
        struct{
            BYTE SW_1_4	:4;	// (  )RB0-3:
            BYTE        :1;	// (25)RB4:
            BYTE    	:1;	// (26)RB5:
            BYTE    	:1;	// (27)RB6:ICSPCLK
            BYTE    	:1;	// (28)RB7:ICSPDAT
        };
    };
};
#define POB (*(volatile struct RB*)&LATB)
#define sPOB (*(struct RB*)&LATB)
#define PIB (*(volatile struct RB*)&PORTB)
#define sPIB (*(struct RB*)&PORTB)
#define PDB (*(volatile struct RB*)&TRISB)
#define sPDB (*(struct RB*)&TRISB)

/////////////////////////////////////////////////////
////////////////////////////////////////PORTCピン定義
struct RC{
    union{
        BYTE Reg;
        struct{
            BYTE SEGA	:1;	// (11)RC0: 
            BYTE SEGB	:1;	// (12)RC1: 
            BYTE SEGC	:1;	// (13)RC2:
            BYTE SEGD	:1;	// (14)RC3: 
            BYTE SEGE	:1;	// (15)RC4:
            BYTE SEGF	:1;	// (16)RC5:
            BYTE SEGG	:1;	// (17)RC6:
            BYTE SEGH  	:1;	// (18)RC7:
        };
        struct{
            BYTE SEG7	:7;	// (  )RC0-6: 
            BYTE CORON 	:1;	// (18)RC7:
        };
        struct{     // ８ビット
            BYTE D4 	:1;	// (11)RC0: 
            BYTE D5 	:1;	// (12)RC1: 
            BYTE D6 	:1;	// (13)RC2:
            BYTE D7 	:1;	// (14)RC3: 
            BYTE D8 	:1;	// (15)RC4:
            BYTE D9 	:1;	// (16)RC5:
            BYTE D10    :1;	// (17)RC6:
            BYTE D11	:1;	// (18)RC7:
        };
        struct{
            BYTE G_GREEN:4;
            BYTE G_RED:4;
        };
        struct{     // フルカラー
            BYTE RED 	:1;	// (11)RC0:
            BYTE GREEN 	:1;	// (12)RC1: 
            BYTE BLUE 	:1;	// (13)RC2: 
            BYTE     	:1;	// (14)RC3: 
            BYTE     	:1;	// (15)RC4:
            BYTE     	:1;	// (16)RC5:
            BYTE        :1;	// (17)RC6:
            BYTE    	:1;	// (18)RC7:
        };
        struct{     // さいころ
            BYTE D15 	:1;	// (11)RC0: 
            BYTE D16 	:1;	// (12)RC1: 
            BYTE D17 	:1;	// (13)RC2:
            BYTE D14 	:1;	// (14)RC3: 
            BYTE D13 	:1;	// (15)RC4:
            BYTE D12 	:1;	// (16)RC5:
            BYTE D3     :1;	// (17)RC6:
            BYTE    	:1;	// (18)RC7:
        };
    };
};
#define POC (*(volatile struct RC*)&LATC)
#define sPOC (*(struct RC*)&LATC)
#define PIC (*(volatile struct RC*)&PORTC)
#define sPIC (*(struct RC*)&PORTC)
#define PDC (*(volatile struct RC*)&TRISC)
#define sPDC (*(struct RC*)&TRISC)


typedef enum{
    SegLeft = 0,        // ７セグメントLED左端桁
    SegMLeft,           // ７セグメントLED左側の１桁目
    SegMRight,          // ７セグメントLED右側の２桁目
    SegRight,           // ７セグメントLED右端桁
    SegDice,            // さいころLED
    SegColor,           // フルカラーLED
    Seg8bit,            // 8bitLED
    SegNone,            // LEDの選択無効          
}LS138SEL;

//eof
