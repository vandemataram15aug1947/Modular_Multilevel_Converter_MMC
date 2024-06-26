/*
 * Peripheral_Setup.c
 */
#include "Peripheral_Setup.h"

void Setup_GPIO(void){
    // pg 959 Table Mux, pg 965 Registers and  spruhm8i.pdf - Technical reference
    EALLOW;
    // LED 31 A, 2
    // LED 34 B, 1
    GpioCtrlRegs.GPAGMUX2.bit.GPIO31 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO31 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;
    GpioCtrlRegs.GPACSEL4.bit.GPIO31 = GPIO_MUX_CPU1;
    GpioCtrlRegs.GPBGMUX1.bit.GPIO34 = 0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO34 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
    GpioCtrlRegs.GPBCSEL1.bit.GPIO34 = GPIO_MUX_CPU2;

    // PHASE A 1A,B 2A,B 3A,C
    //PWM 1A/B
    GpioCtrlRegs.GPAGMUX1.bit.GPIO0 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO1 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;
    //PWM 2A/B
    GpioCtrlRegs.GPAGMUX1.bit.GPIO2 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO3 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;
    //PWM 3A/B
    GpioCtrlRegs.GPAGMUX1.bit.GPIO4 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO5 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;

    //PWM 1A/B - GPIO145 e 146
    GpioCtrlRegs.GPAGMUX1.bit.GPIO6 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO7 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;
    //PWM 2A/B - GPIO147 e 148
    GpioCtrlRegs.GPAGMUX1.bit.GPIO8 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO9 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;
    //PWM 3A/B - GPIO149 e 150
    GpioCtrlRegs.GPAGMUX1.bit.GPIO10 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO11 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;
    //GPIO 14 Out (debug)
    GpioCtrlRegs.GPAGMUX1.bit.GPIO14 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;
    EDIS;
}

void Setup_ePWM(void){
    // pg 1978 spruhm8i.pdf - Technical reference
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    CpuSysRegs.PCLKCR2.bit.EPWM1 = 1; // 1440 Hz 24x60
    CpuSysRegs.PCLKCR2.bit.EPWM2 = 1; // 1440 Hz
    CpuSysRegs.PCLKCR2.bit.EPWM3 = 1; // 1440 Hz

    CpuSysRegs.PCLKCR2.bit.EPWM4 = 1; // Trigger to ADC 18 khz 36kz (controle, adc, pll etc,....)
    EDIS;

    EPwm1Regs.TBPRD = 34725;                         // 60*24 Hz
    EPwm1Regs.CMPA.bit.CMPA = 0;
    EPwm1Regs.TBCTR = 0x0000;                       // Clear counter

    EPwm1Regs.TBPHS.bit.TBPHS = 0;                  // Phase is 0
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;     // Out pulse sincronization, master
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;         // Disable phase loading
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // Count up/down
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // Clock ratio to SYSCLKOUT
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;

    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;     // Load registers every ZERO
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;     // Load registers every ZERO
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;

    EPwm1Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // set actions for EPWM1A
    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;

    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;       // Active Hi complementary
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  // enable Dead-band module
    EPwm1Regs.DBFED.bit.DBFED = 10;                 // FED = 20 TBCLKs
    EPwm1Regs.DBRED.bit.DBRED = 10;                 // RED = 20 TBCLKs
    //---------------------------------------
    EPwm2Regs.TBPRD = EPwm1Regs.TBPRD;
    EPwm2Regs.CMPA.bit.CMPA = 0;
    EPwm2Regs.TBCTR = 0x0000;

    EPwm2Regs.TBPHS.bit.TBPHS = 0;
    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Slave phase
    EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;      // Enable phase loading
    EPwm2Regs.TBCTL.bit.PHSDIR = 0;             // Phase dir
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;

    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;     // Load registers every ZERO
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;

    EPwm2Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;

    EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;       // Active Hi complementary
    EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  // enable Dead-band module
    EPwm2Regs.DBFED.bit.DBFED = 10;                 // FED = 20 TBCLKs
    EPwm2Regs.DBRED.bit.DBRED = 10;                 // RED = 20 TBCLKs

    //======================================

    EPwm3Regs.TBPRD = EPwm1Regs.TBPRD;
    EPwm3Regs.CMPA.bit.CMPA = 0;
    EPwm3Regs.TBCTR = 0x0000;                       // Clear counter

    EPwm3Regs.TBPHS.bit.TBPHS = 0;                  // Phase is 0
    EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; // Out pulse disable
    EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;         // Disable phase loading
    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // Count up/down
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // Clock ratio to SYSCLKOUT
    EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;

    EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;     // Load registers every ZERO
    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
    EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;     // Load registers every ZERO
    EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;

    EPwm3Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // set actions for EPWM1A
    EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;

    EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;       // Active Hi complementary
    EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  // enable Dead-band module
    EPwm3Regs.DBFED.bit.DBFED = 10;                 // FED = 20 TBCLKs
    EPwm3Regs.DBRED.bit.DBRED = 10;                 // RED = 20 TBCLKs
    //==========================================

    EPwm4Regs.TBPRD = 2778;                         // 18 kHz (adc 36 kkz)
    EPwm4Regs.CMPA.bit.CMPA = EPwm4Regs.TBPRD>>1;
    EPwm4Regs.TBCTR = 0x0000;                       // Clear counter

    EPwm4Regs.TBPHS.bit.TBPHS = 0;                  // Phase is 0
    EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE; // Out pulse disable
    EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE;         // Disable phase loading
    EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // Count up/down
    EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // Clock ratio to SYSCLKOUT
    EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;

    EPwm4Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // set actions for EPWM1A
    EPwm4Regs.AQCTLA.bit.CAD = AQ_SET;

    //Trigger ADC
    EPwm4Regs.ETSEL.bit.SOCAEN = 1;                 // Enable SOC on A group
    EPwm4Regs.ETSEL.bit.SOCASEL = ET_CTR_PRDZERO;   // Dispara ADC no topo
    EPwm4Regs.ETPS.bit.SOCAPRD = ET_1ST;            // Trigger on every event

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}

void Setup_ADC_A(void){
    // pg 1592 spruhm8i.pdf - Technical reference
    Uint16 acqps;
    EALLOW;
    CpuSysRegs.PCLKCR13.bit.ADC_A = 1;
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6;          // set ADCCLK divider to /4
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;       // Set pulse um ciclo antes do resultado
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;          // power up the ADC
    DELAY_US(1000);                             // delay for 1ms to allow ADC time to power up
    // determine minimum acquisition window (in SYSCLKS) based on resolution
    if(ADC_RESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION)
        acqps = 14;                             // 75ns
    else                                        // resolution is 16-bit
        acqps = 63;                             // 320ns
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 3;          //SOC0 will convert pin ADCINA3 Va
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps;      //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = TRIG_SEL_ePWM4_SOCA;  //trigger on ePWM7 SOCA

    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 14;          //SOC1 will convert pin ADCIN14 Vb
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = acqps;
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = TRIG_SEL_ePWM4_SOCA;

    AdcaRegs.ADCSOC2CTL.bit.CHSEL = 5;          //SOC2 will convert pin ADCINA5 Vc
    AdcaRegs.ADCSOC2CTL.bit.ACQPS = acqps;
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = TRIG_SEL_ePWM4_SOCA;

    AdcaRegs.ADCSOC3CTL.bit.CHSEL = 4;          //SOC3 will convert pin ADCINA4 Vdc
    AdcaRegs.ADCSOC3CTL.bit.ACQPS = acqps;
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL = TRIG_SEL_ePWM4_SOCA;

    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 3;      // end of SOC4 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;        // enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;      // make sure INT1 flag is cleared

    EDIS;
}

void Setup_ADC_B(void){
    // pg 1592 spruhm8i.pdf - Technical reference
    Uint16 acqps;
    EALLOW;
    //write configurations
    CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
    AdcbRegs.ADCCTL2.bit.PRESCALE = 6;                      // Set ADCCLK divider to /4
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;                   // Set pulse positions to late
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;                      // power up the ADC
    DELAY_US(1000);                                         // delay for > 1ms to allow ADC time to power up

    //determine minimum acquisition window (in SYSCLKS) based on resolution
    if(ADC_RESOLUTION_12BIT == AdcbRegs.ADCCTL2.bit.RESOLUTION){
        acqps = 14;     //75ns
    }else{
        //resolution is 16-bit
        acqps = 63; //320ns
    }
    //Select the channels to convert and end of conversion flag ADCB
    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 3;                     //SOC0 will convert pin ADCINB3
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = acqps;                  //sample window is 100 SYSCLK cycles
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = TRIG_SEL_ePWM4_SOCA;  //trigger

    AdcbRegs.ADCSOC1CTL.bit.CHSEL = 2;                      //SOC1 will convert pin ADCINB2
    AdcbRegs.ADCSOC1CTL.bit.ACQPS = acqps;
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = TRIG_SEL_ePWM4_SOCA;

    AdcbRegs.ADCSOC2CTL.bit.CHSEL = 5;                      //SOC2 will convert pin ADCINB5
    AdcbRegs.ADCSOC2CTL.bit.ACQPS = acqps;
    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL = TRIG_SEL_ePWM4_SOCA;


    //pg 1569 spruhm8i.pdf - Technical reference
    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 2;                  // End of SOC3 will set INT1 flag
    AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;                    // Disable INT1 flag
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;                  // Make sure INT1 flag is cleared
    EDIS;
}

void Setup_DACA(void){
    // pg 1760 spruhm8i.pdf - Technical reference
    EALLOW;
    CpuSysRegs.PCLKCR16.bit.DAC_A = 1;
    DacaRegs.DACCTL.bit.SYNCSEL =       0x03; //DAC ePWMSyncPer 0-> ePWM1, 1->ePWM2, (x-1)->ePWMX
    DacaRegs.DACCTL.bit.LOADMODE =      0x01;
    DacaRegs.DACCTL.bit.DACREFSEL =     0x01;
    DacaRegs.DACVALS.bit.DACVALS =      0; //12 bits
    DacaRegs.DACOUTEN.bit.DACOUTEN =    1;
    DacaRegs.DACLOCK.all =              0x00;
    EDIS;
}
