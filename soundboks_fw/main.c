#include "timer.h"

#ifdef PIC32_MCU
// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 4)
//#pragma config FPLLODIV = DIV_4         // System PLL Output Clock Divider (PLL Divide by 4)
 
// DEVCFG1
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
//#pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = HS            // Primary Oscillator Configuration (Primary osc disabled)
//#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = ON            // CLKO Output Signal Active on the OSCO Pin (Enabled)
#pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
 
// DEVCFG0
#pragma config DEBUG = OFF              // Background Debugger Enable (Debugger is disabled)
#pragma config ICESEL = ICS_PGx2        // ICE/ICD Comm Channel Select (ICE EMUC2/EMUD2 pins shared with PGC2/PGD2)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

#include <xc.h>
#include <sys/attribs.h>

#define PFREQ 40000000

#ifdef PIC32_USE_TMR_INT
#define TMR_FREQ 1000
#define CLOCKS_PER_SEC TMR_FREQ //Pfreq

static uint32_t cclock = 0;
inline uint32_t get_clock() {
    return cclock;
}

void __ISR(_TIMER_3_VECTOR, IPL7SRS) T3Interrupt(void) {
    ++cclock;
    update_timers();
 
    // Reset interrupt flag
    IFS0bits.T3IF = 0;
}
#else 
#define CLOCKS_PER_SEC PFREQ
inline uint32_t get_clock() {
    return TMR2;
}
#endif

#define LED1        LATDbits.LATD0
#define LED2        LATDbits.LATD1
#define LED3        LATDbits.LATD2
#define SW1         PORTDbits.RD6
#define SW2         PORTDbits.RD7
#define SW3         PORTDbits.RD13

#else
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

inline uint32_t get_clock() {
    return clock();
}
#endif //PIC32_MCU

int cb(void *ctx) {
#ifdef PIC32_MCU
    LED1 = !LED1;
#else
    printf("Tick\n");
#endif
    // stop = true;
    return 0;
}

int cb2(void * arg) {
#ifdef PIC32_MCU
    LED2 = !LED2;
#else
    printf("Tock\n");
#endif
    return 0;
}

int main(void) {
    Timer_t * tmr;

#ifdef PIC32_MCU
//    TRISDbits.TRISD6 = 1;   //SW1 as input
//    TRISDbits.TRISD7 = 1;   //SW2 as input
//    TRISDbits.TRISD13 = 1;  //SW3 as input
     
    TRISDbits.TRISD0 = 0;   //LED1 as output
    TRISDbits.TRISD1 = 0;   //LED2 as output
    TRISDbits.TRISD2 = 0;   //LED3 as output
    LED1 = 0;
    LED2 = 0;
    
    //Configure T2/3 timers into 32-bit counter mode, prescaler = 1:1
    T2CONbits.T32 = 1;
    T2CONbits.TCS = 0;
    T2CONbits.TGATE = 0;
    
#ifdef PIC32_USE_TMR_INT
    PR2 = PFREQ/TMR_FREQ; //1ms period
    IFS0bits.T3IF = 0;
    IPC3bits.T3IP = 0x7; 
    IEC0bits.T3IE = 1; //Enable timer interrupt
    INTCONbits.MVEC = 1; //Multivector mode
    __builtin_enable_interrupts();
#else
    PR2 = (uint32_t)(-1);
#endif    
   
    T2CONbits.ON = 1;
#else
    clock_t t1, t2;
    double cpu_time_used;
#endif

    tmr = timer_start(TIME_SEC(1.0), true, cb, NULL);
    timer_start(TIME_SEC(5.0), true, cb2, NULL);

#ifndef PIC32_MCU
    t1 = clock();
#endif
    while(is_running(tmr)) {
        //Do stuff
#if !defined(PIC32_USE_TMR_INT)
        update_timers();
#endif
    }
#ifndef PIC32_MCU
    t2 = clock();
    cpu_time_used = (double)(t2 - t1)/CLOCKS_PER_SEC; 
    printf("Elapsed time: %.5fs\n", cpu_time_used);
#endif

    return EXIT_SUCCESS;
}

