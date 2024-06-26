/* =================================================================================
File name:       PID_REG3.H  (IQ version)                    
                    
Originator:	Digital Control Systems Group
			Texas Instruments

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the PIDREG3.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005	Version 3.20
------------------------------------------------------------------------------*/
#ifndef __PIDREG3_H__
#define __PIDREG3_H__

typedef struct {
	float  Ref;   			// Input: Reference input
	float  Fdb;   			// Input: Feedback input
	float  Err;				// Variable: Error
	float  Kp;				// Parameter: Proportional gain
	float  Up;				// Variable: Proportional output
	float  Ui;				// Variable: Integral output
	float  Ud;				// Variable: Derivative output
	float  OutPreSat; 		// Variable: Pre-saturated output
	float  OutMax;		    // Parameter: Maximum output
	float  OutMin;	    	// Parameter: Minimum output
	float  Out;   			// Output: PID output
	float  SatErr;			// Variable: Saturated difference
	float  Ki;			    // Parameter: Integral gain
	float  Kc;		     	// Parameter: Integral correction gain
	float  Kd; 		        // Parameter: Derivative gain
	float  Up1;		   	    // History: Previous proportional output
	void  (*calc)();	  	// Pointer to calculation function
	void  (*reset)();	  	// Pointer to reset function
} PIDREG3;

typedef PIDREG3 *PIDREG3_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PIDREG3 object.
-----------------------------------------------------------------------------*/                     
#define PIDREG3_DEFAULTS { 0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           1.0,\
                           0, \
                           0, \
              			  (void (*)(Uint32))pid_reg3_calc,\
						  (void (*)(Uint32))pid_reg3_reset}

/*------------------------------------------------------------------------------
Prototypes for the functions in PIDREG3.C
------------------------------------------------------------------------------*/
void pid_reg3_calc(PIDREG3_handle);
void pid_reg3_reset(PIDREG3 *v);

#define PID_CALC_MACRO(v)\
    v.Up = v.Kp*v.Err;\
    v.Ui = v.Ui + v.Ki*(v.Up+v.Up1)*0.5 + (v.Kc*v.SatErr);\
    v.OutPreSat = v.Up + v.Ui;\
    if (v.OutPreSat > v.OutMax)\
      v.Out =  v.OutMax;\
    else if (v.OutPreSat < v.OutMin)\
      v.Out =  v.OutMin;\
    else\
      v.Out = v.OutPreSat;\
    v.SatErr = v.Out - v.OutPreSat;\
    v.Up1 = v.Up;\

#define PID_RESET_MACRO(v)\
    v.Err = 0;\
    v.Fdb = 0;\
    v.Out = 0;\
    v.OutPreSat = 0;\
    v.Ui = 0;\
    v.Up1 = 0;\
    v.Ud = 0;\
    v.SatErr = 0;\
    v.Out = 0;\

#endif // __PIDREG3_H__
