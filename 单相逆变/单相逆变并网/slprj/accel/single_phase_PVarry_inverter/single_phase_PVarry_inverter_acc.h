#include "__cf_single_phase_PVarry_inverter.h"
#ifndef RTW_HEADER_single_phase_PVarry_inverter_acc_h_
#define RTW_HEADER_single_phase_PVarry_inverter_acc_h_
#include <stddef.h>
#include <float.h>
#include <string.h>
#ifndef single_phase_PVarry_inverter_acc_COMMON_INCLUDES_
#define single_phase_PVarry_inverter_acc_COMMON_INCLUDES_
#include <stdlib.h>
#define S_FUNCTION_NAME simulink_only_sfcn 
#define S_FUNCTION_LEVEL 2
#define RTW_GENERATED_S_FUNCTION
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#endif
#include "single_phase_PVarry_inverter_acc_types.h"
#include "multiword_types.h"
#include "rtGetInf.h"
#include "rt_nonfinite.h"
#include "mwmathutil.h"
#include "rt_defines.h"
typedef struct { real_T B_3_0_0 ; real_T B_3_1_0 ; real_T B_3_2_0 [ 8 ] ;
real_T B_3_2_1 [ 4 ] ; real_T B_3_3_0 ; real_T B_3_4_0 ; real_T B_3_5_0 ;
real_T B_3_6_0 ; real_T B_3_8_0 ; real_T B_3_10_0 ; real_T B_3_11_0 ; real_T
B_3_14_0 ; real_T B_3_15_0 ; real_T B_3_16_0 ; real_T B_3_17_0 ; real_T
B_3_18_0 ; real_T B_3_19_0 ; real_T B_3_21_0 ; real_T B_3_22_0 ; real_T
B_3_24_0 ; real_T B_3_25_0 ; real_T B_3_27_0 ; real_T B_3_28_0 ; real_T
B_3_30_0 ; real_T B_3_32_0 ; real_T B_3_34_0 ; real_T B_3_36_0 ; real_T
B_3_39_0 ; real_T B_3_40_0 ; real_T B_3_43_0 [ 4 ] ; real_T B_3_44_0 ; real_T
B_3_45_0 [ 4 ] ; real_T B_3_46_0 [ 4 ] ; real_T B_3_62_0 ; real_T B_3_68_0 ;
real_T B_3_72_0 ; real_T B_3_78_0 ; real_T B_3_82_0 ; real_T B_3_86_0 ;
real_T B_3_88_0 ; real_T B_3_89_0 ; real_T B_3_90_0 ; real_T B_3_91_0 ;
real_T B_3_92_0 ; real_T B_3_94_0 ; real_T B_3_95_0 ; real_T B_3_96_0 ;
real_T B_3_97_0 ; real_T B_3_98_0 ; real_T B_3_100_0 ; real_T B_3_102_0 ;
real_T B_3_103_0 ; real_T B_3_104_0 ; real_T B_3_105_0 ; real_T B_3_106_0 ;
real_T B_3_107_0 ; real_T B_3_109_0 ; real_T B_3_110_0 ; real_T B_3_111_0 ;
real_T B_3_112_0 ; real_T B_3_113_0 ; real_T B_3_114_0 ; real_T B_3_115_0 ;
real_T B_3_116_0 ; real_T B_3_117_0 ; real_T B_3_118_0 ; real_T B_3_119_0 ;
real_T B_3_120_0 ; real_T B_3_123_0 ; real_T B_2_0_0 ; real_T B_2_1_0 ;
real_T B_1_0_0 ; real_T B_1_1_0 ; real_T B_1_2_0 ; real_T B_1_3_0 ; real_T
B_1_4_0 ; real_T B_1_6_0 ; real_T B_1_7_0 ; real_T B_1_8_0 ; real_T B_1_10_0
; real_T B_1_11_0 ; real_T B_1_12_0 ; real_T B_1_13_0 ; real_T B_1_14_0 ;
real_T B_1_15_0 ; real_T B_1_16_0 ; real_T B_1_17_0 ; real_T B_0_0_0 ; real_T
B_0_1_0 ; real_T B_0_2_0 [ 2 ] ; } B_single_phase_PVarry_inverter_T ; typedef
struct { real_T SFunction_DSTATE [ 3 ] ; real_T TimeStampA ; real_T
LastUAtTimeA ; real_T TimeStampB ; real_T LastUAtTimeB ; real_T TimeStampA_k
; real_T LastUAtTimeA_o ; real_T TimeStampB_p ; real_T LastUAtTimeB_i ;
real_T PrevYA ; real_T PrevYB ; real_T LastMajorTimeA ; real_T LastMajorTimeB
; real_T StateSpace_RWORK ; struct { real_T modelTStart ; }
TransportDelay1_RWORK ; struct { real_T modelTStart ; } T_RWORK ; struct {
real_T modelTStart ; } VariableTransportDelay_RWORK ; struct { real_T
modelTStart ; } VariableTransportDelay_RWORK_d ; struct { real_T modelTStart
; } VariableTransportDelay1_RWORK ; void * StateSpace_PWORK [ 19 ] ; void *
Scope_PWORK [ 4 ] ; void * Scope1_PWORK ; struct { void * TUbufferPtrs [ 2 ]
; } TransportDelay1_PWORK ; void * Scope2_PWORK ; struct { void *
TUbufferPtrs [ 2 ] ; } T_PWORK ; struct { void * TUbufferPtrs [ 2 ] ; }
VariableTransportDelay_PWORK ; void * Scope2_PWORK_k ; struct { void *
TUbufferPtrs [ 2 ] ; } VariableTransportDelay_PWORK_b ; struct { void *
TUbufferPtrs [ 2 ] ; } VariableTransportDelay1_PWORK ; void *
Synthesized_Atomic_Subsystem_For_Alg_Loop_1_AlgLoopData ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; int32_T
AutomaticGainControl_sysIdxToRun ; int32_T
Synthesized_Atomic_Subsystem_For_Alg_Loop_1_sysIdxToRun ; int32_T
Synthesized_Atomic_Subsystem_For_Alg_Loop_1_blkIdxToRun ; int32_T
Synthesized_Atomic_Subsystem_For_Alg_Loop_1_parentSysIdxToRun ; int_T
StateSpace_IWORK [ 9 ] ; struct { int_T Tail ; int_T Head ; int_T Last ;
int_T CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay1_IWORK ;
struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } T_IWORK ; struct { int_T Tail ; int_T Head ; int_T Last ;
int_T CircularBufSize ; int_T MaxNewBufSize ; } VariableTransportDelay_IWORK
; struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ;
int_T MaxNewBufSize ; } VariableTransportDelay_IWORK_m ; struct { int_T Tail
; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
VariableTransportDelay1_IWORK ; char StateSpace_MODE [ 4 ] ; int_T
Integrator_MODE ; int_T Saturation_MODE ; int_T Saturation_MODE_d [ 4 ] ;
int_T Abs_MODE ; int_T Toavoiddivisionbyzero_MODE ; int_T Step_MODE ; int_T
Integrator_MODE_l ; int_T Saturation2_MODE ; int_T
Toavoiddivisionbyzero_MODE_c ; int_T Saturation_MODE_j ; int8_T C2_DWORK1 ;
int8_T Sqrt_DWORK1 ; int8_T AutomaticGainControl_SubsysRanBC ; boolean_T
RelationalOperator_Mode ; boolean_T RelationalOperator1_Mode ; boolean_T
Switch_Mode [ 4 ] ; boolean_T AutomaticGainControl_MODE ; char_T
pad_AutomaticGainControl_MODE [ 2 ] ; } DW_single_phase_PVarry_inverter_T ;
typedef struct { real_T StateSpace_CSTATE [ 3 ] ; real_T Integrator_CSTATE ;
real_T Integrator_CSTATE_b ; real_T integ_CSTATE ; real_T StateSpace_CSTATE_o
[ 2 ] ; real_T Integrator_CSTATE_i ; real_T Integrator_CSTATE_h ; real_T
integ1_CSTATE ; real_T Integ2_CSTATE ; } X_single_phase_PVarry_inverter_T ;
typedef struct { real_T StateSpace_CSTATE [ 3 ] ; real_T Integrator_CSTATE ;
real_T Integrator_CSTATE_b ; real_T integ_CSTATE ; real_T StateSpace_CSTATE_o
[ 2 ] ; real_T Integrator_CSTATE_i ; real_T Integrator_CSTATE_h ; real_T
integ1_CSTATE ; real_T Integ2_CSTATE ; } XDot_single_phase_PVarry_inverter_T
; typedef struct { boolean_T StateSpace_CSTATE [ 3 ] ; boolean_T
Integrator_CSTATE ; boolean_T Integrator_CSTATE_b ; boolean_T integ_CSTATE ;
boolean_T StateSpace_CSTATE_o [ 2 ] ; boolean_T Integrator_CSTATE_i ;
boolean_T Integrator_CSTATE_h ; boolean_T integ1_CSTATE ; boolean_T
Integ2_CSTATE ; } XDis_single_phase_PVarry_inverter_T ; typedef struct {
real_T StateSpace_CSTATE [ 3 ] ; real_T Integrator_CSTATE ; real_T
Integrator_CSTATE_b ; real_T integ_CSTATE ; real_T StateSpace_CSTATE_o [ 2 ]
; real_T Integrator_CSTATE_i ; real_T Integrator_CSTATE_h ; real_T
integ1_CSTATE ; real_T Integ2_CSTATE ; }
CStateAbsTol_single_phase_PVarry_inverter_T ; typedef struct { real_T
StateSpace_Sf0_ZC [ 5 ] ; real_T Integrator_IntgUpLimit_ZC ; real_T
Integrator_IntgLoLimit_ZC ; real_T Integrator_LeaveSaturate_ZC ; real_T
Saturation_UprLim_ZC ; real_T Saturation_LwrLim_ZC ; real_T
RelationalOperator_RelopInput_ZC ; real_T RelationalOperator1_RelopInput_ZC ;
real_T Switch_SwitchCond_ZC [ 4 ] ; real_T Saturation_UprLim_ZC_g [ 4 ] ;
real_T Saturation_LwrLim_ZC_j [ 4 ] ; real_T Abs_AbsZc_ZC ; real_T
Toavoiddivisionbyzero_UprLim_ZC ; real_T Toavoiddivisionbyzero_LwrLim_ZC ;
real_T Step_StepTime_ZC ; real_T Integrator_IntgUpLimit_ZC_d ; real_T
Integrator_IntgLoLimit_ZC_g ; real_T Integrator_LeaveSaturate_ZC_j ; real_T
Saturation2_UprLim_ZC ; real_T Saturation2_LwrLim_ZC ; real_T
Toavoiddivisionbyzero_UprLim_ZC_l ; real_T Toavoiddivisionbyzero_LwrLim_ZC_e
; real_T Saturation_UprLim_ZC_p ; real_T Saturation_LwrLim_ZC_j5 ; }
ZCV_single_phase_PVarry_inverter_T ; typedef struct { ZCSigState
StateSpace_Sf0_ZCE [ 5 ] ; ZCSigState Integrator_IntgUpLimit_ZCE ; ZCSigState
Integrator_IntgLoLimit_ZCE ; ZCSigState Integrator_LeaveSaturate_ZCE ;
ZCSigState Saturation_UprLim_ZCE ; ZCSigState Saturation_LwrLim_ZCE ;
ZCSigState RelationalOperator_RelopInput_ZCE ; ZCSigState
RelationalOperator1_RelopInput_ZCE ; ZCSigState Switch_SwitchCond_ZCE [ 4 ] ;
ZCSigState Saturation_UprLim_ZCE_p [ 4 ] ; ZCSigState Saturation_LwrLim_ZCE_h
[ 4 ] ; ZCSigState Abs_AbsZc_ZCE ; ZCSigState
Toavoiddivisionbyzero_UprLim_ZCE ; ZCSigState
Toavoiddivisionbyzero_LwrLim_ZCE ; ZCSigState Step_StepTime_ZCE ; ZCSigState
Integrator_IntgUpLimit_ZCE_i ; ZCSigState Integrator_IntgLoLimit_ZCE_a ;
ZCSigState Integrator_LeaveSaturate_ZCE_p ; ZCSigState Saturation2_UprLim_ZCE
; ZCSigState Saturation2_LwrLim_ZCE ; ZCSigState
Toavoiddivisionbyzero_UprLim_ZCE_i ; ZCSigState
Toavoiddivisionbyzero_LwrLim_ZCE_b ; ZCSigState Saturation_UprLim_ZCE_l ;
ZCSigState Saturation_LwrLim_ZCE_f ; } PrevZCX_single_phase_PVarry_inverter_T
; struct P_single_phase_PVarry_inverter_T_ { real_T P_0 ; real_T P_1 ; real_T
P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ; real_T P_7 ; real_T
P_8 ; real_T P_9 ; real_T P_10 ; real_T P_11 ; real_T P_12 ; real_T P_13 ;
real_T P_14 ; real_T P_15 ; real_T P_16 ; real_T P_17 [ 2 ] ; real_T P_18 [ 9
] ; real_T P_19 [ 2 ] ; real_T P_20 [ 18 ] ; real_T P_21 [ 2 ] ; real_T P_22
[ 24 ] ; real_T P_23 [ 2 ] ; real_T P_24 [ 48 ] ; real_T P_25 [ 2 ] ; real_T
P_26 [ 3 ] ; real_T P_27 [ 2 ] ; real_T P_28 ; real_T P_29 [ 2 ] ; real_T
P_30 [ 4 ] ; real_T P_31 [ 2 ] ; real_T P_32 [ 4 ] ; real_T P_33 [ 2 ] ;
real_T P_34 [ 4 ] ; real_T P_35 [ 2 ] ; real_T P_36 [ 4 ] ; real_T P_37 [ 2 ]
; real_T P_38 ; real_T P_39 [ 2 ] ; real_T P_40 ; real_T P_41 ; real_T P_42 ;
real_T P_43 ; real_T P_44 ; real_T P_45 ; real_T P_46 ; real_T P_47 ; real_T
P_48 ; real_T P_49 ; real_T P_50 ; real_T P_51 ; real_T P_52 ; real_T P_53 ;
real_T P_54 ; real_T P_55 ; real_T P_56 ; real_T P_57 ; real_T P_58 ; real_T
P_59 ; real_T P_60 ; real_T P_61 [ 3 ] ; real_T P_62 [ 3 ] ; real_T P_63 ;
real_T P_64 ; real_T P_65 ; real_T P_66 ; real_T P_67 ; real_T P_68 ; real_T
P_69 ; real_T P_70 ; real_T P_71 ; real_T P_72 ; real_T P_73 ; real_T P_74 ;
real_T P_75 ; real_T P_76 ; real_T P_77 ; real_T P_78 ; real_T P_79 ; real_T
P_80 ; real_T P_81 ; real_T P_82 ; real_T P_83 ; real_T P_84 ; real_T P_85 ;
real_T P_86 ; real_T P_87 ; real_T P_88 [ 3 ] ; real_T P_89 ; real_T P_90 ;
real_T P_91 [ 2 ] ; real_T P_92 ; real_T P_93 ; real_T P_94 ; real_T P_95 ;
real_T P_96 ; real_T P_97 ; real_T P_98 ; real_T P_99 ; real_T P_100 ; real_T
P_101 ; real_T P_102 ; real_T P_103 ; real_T P_104 ; real_T P_105 ; real_T
P_106 ; real_T P_107 ; real_T P_108 ; real_T P_109 ; real_T P_110 ; real_T
P_111 ; real_T P_112 ; real_T P_113 ; real_T P_114 ; real_T P_115 ; uint8_T
P_116 ; char_T pad_P_116 [ 7 ] ; } ; extern P_single_phase_PVarry_inverter_T
single_phase_PVarry_inverter_rtDefaultP ;
#endif
