#include "Peripheral_Setup.h"
#include "pid_reg3.h"
#include "3ph_abc_dq0.h"


// Interrupcoes ---------------------------------
__interrupt void isr_Cpu_Timer0(void);
__interrupt void isr_ADC(void);
//-----------------------------------------------

PIDREG3 C_iLd = PIDREG3_DEFAULTS;
PIDREG3 C_iLq = PIDREG3_DEFAULTS;

ABC_DQ0_POS abc_dq0;
SPLL_3ph_SRF spll;

DQ0_ABC iL_dq0_abc;
ABC_DQ0_POS iL_abc_dq0;

float iLa, iLb, iLc;
float Va, Vb, Vc;
float Vdc;
float duty[3];
float id_ref, iq_ref, vd_ref, vq_ref;
unsigned short int counter_adc = 0;

uint16_t index = 0;
int main(void){
    InitSysCtrl();                          // Initialize System Control:
    DINT;                                   // Disable CPU interrupts
    InitPieCtrl();                          // Initialize the PIE control registers to their default state
    IER = 0x0000;                           // Disable CPU interrupts
    IFR = 0x0000;                           // Clear all CPU interrupt flags:

    EALLOW;
    // pg 120 spruhm8i.pdf - Technical reference
    CpuSysRegs.PCLKCR0.bit.CPUTIMER0 = 1;
    EDIS;
    InitPieVectTable();                     // Initialize the PIE vector table
    EALLOW;
    // Redirect function to interruptions
    PieVectTable.TIMER0_INT = &isr_Cpu_Timer0;
    PieVectTable.ADCA1_INT = &isr_ADC;
    EDIS;
    //pg. 102 PIE Channel Mapping spruhm8i.pdf
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;      // Timer 0
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;      // ADCA1
    IER |= M_INT1;

    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer0, 200, 250000);
    CpuTimer0Regs.TCR.all = 0x4001;

    Setup_GPIO();
    Setup_ePWM();
    Setup_ADC_A();
    Setup_ADC_B();
    Setup_DACA();

    iq_ref = 0;
    id_ref = 0;

    C_iLd.Kp = 5.0;
    C_iLd.Ki = 500.0/36000.0/C_iLd.Kp;
    C_iLd.Kc = 1.0;
    C_iLd.OutMax = 100.0;
    C_iLd.OutMin = -100.0;

    C_iLq.Kp = 5.0;
    C_iLq.Ki = 500.0/36000.0/C_iLq.Kp;
    C_iLq.Kc = 1.0;
    C_iLq.OutMax = 100.0;
    C_iLq.OutMin = -100.0;

    ABC_DQ0_POS_init(&abc_dq0);
    SPLL_3ph_SRF_init(60.0,1/36000.0,&spll);

    DQ0_ABC_init(&iL_dq0_abc);
    ABC_DQ0_POS_init(&iL_abc_dq0);

    EINT;                                   // Enable Global interrupt INTM
    ERTM;                                   // Enable Global realtime interrupt DBGM

    GpioDataRegs.GPADAT.bit.GPIO31 = 1;

    while(1){

    }
    return 0;
}

__interrupt void isr_Cpu_Timer0(void){
    GpioDataRegs.GPATOGGLE.bit.GPIO31 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}

__interrupt void isr_ADC(void){
    GpioDataRegs.GPASET.bit.GPIO14 = 1;
    Va = 0.01*((int)AdcaResultRegs.ADCRESULT0 - 0x7FF);  // Va in PU
    iLa = -0.01*((int)AdcbResultRegs.ADCRESULT0 - 0x7FF);
    Vb = 0.01*((int)AdcaResultRegs.ADCRESULT1 - 0x7FF);  // Vb in PU
    iLb = -0.01*((int)AdcbResultRegs.ADCRESULT1 - 0x7FF);
    Vc = 0.01*((int)AdcaResultRegs.ADCRESULT2 - 0x7FF);  // Vc in PU
    iLc = -0.01*((int)AdcbResultRegs.ADCRESULT2 - 0x7FF);

    Vdc = 0.01*((int)AdcaResultRegs.ADCRESULT3 - 0x7FF);

    // Protection?

    // PLL 3ph
    spll.v_q[0] = (abc_dq0.q);
    SPLL_3ph_SRF_FUNC(&spll);

    // Vabc to dq
    abc_dq0.a = Va;
    abc_dq0.b = Vb;
    abc_dq0.c = Vc;
    abc_dq0.sin = __sin(spll.theta[1]); // Intrinsics for function TMU
    abc_dq0.cos = __cos(spll.theta[1]); // Intrinsics for function TMU
    ABC_DQ0_POS_FUNC(&abc_dq0);

    // iLabc to dq
    iL_abc_dq0.a = iLa;
    iL_abc_dq0.b = iLb;
    iL_abc_dq0.c = iLc;
    iL_abc_dq0.sin = abc_dq0.sin;
    iL_abc_dq0.cos = abc_dq0.cos;
    ABC_DQ0_POS_FUNC(&iL_abc_dq0);

    C_iLd.Err = (id_ref - iL_abc_dq0.d);
    C_iLd.calc(&C_iLd);
    vd_ref = C_iLd.Out - 376.99*0.01*iL_abc_dq0.q; // Decoupling 2*PI*f*L

    C_iLq.Err = (iq_ref - iL_abc_dq0.q);
    C_iLq.calc(&C_iLq);
    vq_ref = C_iLq.Out + 376.99*0.01*iL_abc_dq0.d; // Decoupling 2*PI*f*L

    // iLdq to abc
    iL_dq0_abc.d = (abc_dq0.d*180.0 - vd_ref)/(Vdc*0.5);
    iL_dq0_abc.q = (abc_dq0.q*180.0 - vq_ref)/(Vdc*0.5);
    iL_dq0_abc.z = 0;
    iL_dq0_abc.sin = abc_dq0.sin;
    iL_dq0_abc.cos = abc_dq0.cos;
    DQ0_ABC_FUNC(&iL_dq0_abc);

    duty[0] = (iL_dq0_abc.a);
    duty[1] = (iL_dq0_abc.b);
    duty[2] = (iL_dq0_abc.c);

    // For debug purpose ================
    duty[0] = __sin(6.28318531/300.0*((float)index)) * 0.95;
    index = (index == (300 - 1)) ? 0 : (index+1);

    EALLOW;
    DacaRegs.DACVALS.bit.DACVALS = (uint16_t) (2047.0 * (1.0 + duty[0]));
    EDIS;
    //===================================


    //Phase A -> EPwm1a,b EPwm2a,b EPwm3a,b

    // Phase A
    //duty[0] = -duty[0];
    if(duty[0] >= 0.333){
        // [1, 0.33333]
        EPwm1Regs.CMPA.bit.CMPA = (uint16_t)((duty[0]+1.0) * ((float)(EPwm1Regs.TBPRD>>1)) );
        EPwm2Regs.CMPA.bit.CMPA = (uint16_t) EPwm1Regs.TBPRD; // igual duty = 1
        EPwm3Regs.CMPA.bit.CMPA = (uint16_t) EPwm1Regs.TBPRD;
    }else if (duty[0] >= -0.333){
        // [-0.3333, 0.33333[
        EPwm1Regs.CMPA.bit.CMPA = (uint16_t) 0; // duty = 0
        EPwm2Regs.CMPA.bit.CMPA = (uint16_t) ((duty[0]+1.0) * ((float)(EPwm1Regs.TBPRD>>1)) );       
        EPwm3Regs.CMPA.bit.CMPA = (uint16_t) EPwm1Regs.TBPRD;
    }else{
        // [-1, -0.3333[
        EPwm1Regs.CMPA.bit.CMPA = (uint16_t) 0;
        EPwm2Regs.CMPA.bit.CMPA = (uint16_t) 0;
        EPwm3Regs.CMPA.bit.CMPA = (uint16_t) ((duty[0]+1.0) * ((float)(EPwm1Regs.TBPRD>>1)) );
    }
    // Phase B ........

    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;              //clear INT1 flag
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;              //clear INT1 flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;
}
