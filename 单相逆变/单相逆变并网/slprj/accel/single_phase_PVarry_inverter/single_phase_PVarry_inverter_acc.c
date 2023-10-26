#include "__cf_single_phase_PVarry_inverter.h"
#include <math.h>
#include "single_phase_PVarry_inverter_acc.h"
#include "single_phase_PVarry_inverter_acc_private.h"
#include <stdio.h>
#include "slexec_vm_simstruct_bridge.h"
#include "slexec_vm_zc_functions.h"
#include "slexec_vm_lookup_functions.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simtarget/slSimTgtMdlrefSfcnBridge.h"
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#include "simtarget/slAccSfcnBridge.h"
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T single_phase_PVarry_inverter_acc_rt_TDelayUpdateTailOrGrowBuf (
int_T * bufSzPtr , int_T * tailPtr , int_T * headPtr , int_T * lastPtr ,
real_T tMinusDelay , real_T * * tBufPtr , real_T * * uBufPtr , real_T * *
xBufPtr , boolean_T isfixedbuf , boolean_T istransportdelay , int_T *
maxNewBufSzPtr ) { int_T testIdx ; int_T tail = * tailPtr ; int_T bufSz = *
bufSzPtr ; real_T * tBuf = * tBufPtr ; real_T * xBuf = ( NULL ) ; int_T
numBuffer = 2 ; if ( istransportdelay ) { numBuffer = 3 ; xBuf = * xBufPtr ;
} testIdx = ( tail < ( bufSz - 1 ) ) ? ( tail + 1 ) : 0 ; if ( ( tMinusDelay
<= tBuf [ testIdx ] ) && ! isfixedbuf ) { int_T j ; real_T * tempT ; real_T *
tempU ; real_T * tempX = ( NULL ) ; real_T * uBuf = * uBufPtr ; int_T
newBufSz = bufSz + 1024 ; if ( newBufSz > * maxNewBufSzPtr ) { *
maxNewBufSzPtr = newBufSz ; } tempU = ( real_T * ) utMalloc ( numBuffer *
newBufSz * sizeof ( real_T ) ) ; if ( tempU == ( NULL ) ) { return ( false )
; } tempT = tempU + newBufSz ; if ( istransportdelay ) tempX = tempT +
newBufSz ; for ( j = tail ; j < bufSz ; j ++ ) { tempT [ j - tail ] = tBuf [
j ] ; tempU [ j - tail ] = uBuf [ j ] ; if ( istransportdelay ) tempX [ j -
tail ] = xBuf [ j ] ; } for ( j = 0 ; j < tail ; j ++ ) { tempT [ j + bufSz -
tail ] = tBuf [ j ] ; tempU [ j + bufSz - tail ] = uBuf [ j ] ; if (
istransportdelay ) tempX [ j + bufSz - tail ] = xBuf [ j ] ; } if ( * lastPtr
> tail ) { * lastPtr -= tail ; } else { * lastPtr += ( bufSz - tail ) ; } *
tailPtr = 0 ; * headPtr = bufSz ; utFree ( uBuf ) ; * bufSzPtr = newBufSz ; *
tBufPtr = tempT ; * uBufPtr = tempU ; if ( istransportdelay ) * xBufPtr =
tempX ; } else { * tailPtr = testIdx ; } return ( true ) ; } real_T
single_phase_PVarry_inverter_acc_rt_TDelayInterpolate ( real_T tMinusDelay ,
real_T tStart , real_T * tBuf , real_T * uBuf , int_T bufSz , int_T * lastIdx
, int_T oldestIdx , int_T newIdx , real_T initOutput , boolean_T discrete ,
boolean_T minorStepAndTAtLastMajorOutput ) { int_T i ; real_T yout , t1 , t2
, u1 , u2 ; if ( ( newIdx == 0 ) && ( oldestIdx == 0 ) && ( tMinusDelay >
tStart ) ) return initOutput ; if ( tMinusDelay <= tStart ) return initOutput
; if ( ( tMinusDelay <= tBuf [ oldestIdx ] ) ) { if ( discrete ) { return (
uBuf [ oldestIdx ] ) ; } else { int_T tempIdx = oldestIdx + 1 ; if (
oldestIdx == bufSz - 1 ) tempIdx = 0 ; t1 = tBuf [ oldestIdx ] ; t2 = tBuf [
tempIdx ] ; u1 = uBuf [ oldestIdx ] ; u2 = uBuf [ tempIdx ] ; if ( t2 == t1 )
{ if ( tMinusDelay >= t2 ) { yout = u2 ; } else { yout = u1 ; } } else {
real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ; real_T f2 = 1.0 - f1 ; yout
= f1 * u1 + f2 * u2 ; } return yout ; } } if ( minorStepAndTAtLastMajorOutput
) { if ( newIdx != 0 ) { if ( * lastIdx == newIdx ) { ( * lastIdx ) -- ; }
newIdx -- ; } else { if ( * lastIdx == newIdx ) { * lastIdx = bufSz - 1 ; }
newIdx = bufSz - 1 ; } } i = * lastIdx ; if ( tBuf [ i ] < tMinusDelay ) {
while ( tBuf [ i ] < tMinusDelay ) { if ( i == newIdx ) break ; i = ( i < (
bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } } else { while ( tBuf [ i ] >= tMinusDelay
) { i = ( i > 0 ) ? i - 1 : ( bufSz - 1 ) ; } i = ( i < ( bufSz - 1 ) ) ? ( i
+ 1 ) : 0 ; } * lastIdx = i ; if ( discrete ) { double tempEps = (
DBL_EPSILON ) * 128.0 ; double localEps = tempEps * muDoubleScalarAbs ( tBuf
[ i ] ) ; if ( tempEps > localEps ) { localEps = tempEps ; } localEps =
localEps / 2.0 ; if ( tMinusDelay >= ( tBuf [ i ] - localEps ) ) { yout =
uBuf [ i ] ; } else { if ( i == 0 ) { yout = uBuf [ bufSz - 1 ] ; } else {
yout = uBuf [ i - 1 ] ; } } } else { if ( i == 0 ) { t1 = tBuf [ bufSz - 1 ]
; u1 = uBuf [ bufSz - 1 ] ; } else { t1 = tBuf [ i - 1 ] ; u1 = uBuf [ i - 1
] ; } t2 = tBuf [ i ] ; u2 = uBuf [ i ] ; if ( t2 == t1 ) { if ( tMinusDelay
>= t2 ) { yout = u2 ; } else { yout = u1 ; } } else { real_T f1 = ( t2 -
tMinusDelay ) / ( t2 - t1 ) ; real_T f2 = 1.0 - f1 ; yout = f1 * u1 + f2 * u2
; } } return ( yout ) ; } real_T look1_binlxpw ( real_T u0 , const real_T bp0
[ ] , const real_T table [ ] , uint32_T maxIndex ) { real_T frac ; uint32_T
iRght ; uint32_T iLeft ; uint32_T bpIdx ; if ( u0 <= bp0 [ 0U ] ) { iLeft =
0U ; frac = ( u0 - bp0 [ 0U ] ) / ( bp0 [ 1U ] - bp0 [ 0U ] ) ; } else if (
u0 < bp0 [ maxIndex ] ) { bpIdx = maxIndex >> 1U ; iLeft = 0U ; iRght =
maxIndex ; while ( iRght - iLeft > 1U ) { if ( u0 < bp0 [ bpIdx ] ) { iRght =
bpIdx ; } else { iLeft = bpIdx ; } bpIdx = ( iRght + iLeft ) >> 1U ; } frac =
( u0 - bp0 [ iLeft ] ) / ( bp0 [ iLeft + 1U ] - bp0 [ iLeft ] ) ; } else {
iLeft = maxIndex - 1U ; frac = ( u0 - bp0 [ maxIndex - 1U ] ) / ( bp0 [
maxIndex ] - bp0 [ maxIndex - 1U ] ) ; } return ( table [ iLeft + 1U ] -
table [ iLeft ] ) * frac + table [ iLeft ] ; } void rt_ssGetBlockPath (
SimStruct * S , int_T sysIdx , int_T blkIdx , char_T * * path ) {
_ssGetBlockPath ( S , sysIdx , blkIdx , path ) ; } void rt_ssSet_slErrMsg (
SimStruct * S , void * diag ) { if ( ! _ssIsErrorStatusAslErrMsg ( S ) ) {
_ssSet_slErrMsg ( S , diag ) ; } else { _ssDiscardDiagnostic ( S , diag ) ; }
} void rt_ssReportDiagnosticAsWarning ( SimStruct * S , void * diag ) {
_ssReportDiagnosticAsWarning ( S , diag ) ; } void
single_phase_PVarry_inverter_Synthesized_Atomic_Subsystem_For_Alg_Loop_1_Init
( SimStruct * S ) { B_single_phase_PVarry_inverter_T * _rtB ;
DW_single_phase_PVarry_inverter_T * _rtDW ; _rtDW = ( (
DW_single_phase_PVarry_inverter_T * ) ssGetRootDWork ( S ) ) ; _rtB = ( (
B_single_phase_PVarry_inverter_T * ) _ssGetModelBlockIO ( S ) ) ;
ssCallAccelRunBlock ( S , 0 , 0 , SS_CALL_MDL_INITIALIZE_CONDITIONS ) ; }
void single_phase_PVarry_inverter_Synthesized_Atomic_Subsystem_For_Alg_Loop_1
( SimStruct * S ) { DW_single_phase_PVarry_inverter_T * _rtDW ; _rtDW = ( (
DW_single_phase_PVarry_inverter_T * ) ssGetRootDWork ( S ) ) ;
ssCallAccelRunBlock ( S , 3 , _rtDW ->
Synthesized_Atomic_Subsystem_For_Alg_Loop_1_blkIdxToRun , SS_CALL_MDL_OUTPUTS
) ; } void
single_phase_PVarry_inverter_Synthesized_Atomic_Subsystem_For_Alg_Loop_1_Update
( SimStruct * S ) { B_single_phase_PVarry_inverter_T * _rtB ;
DW_single_phase_PVarry_inverter_T * _rtDW ; _rtDW = ( (
DW_single_phase_PVarry_inverter_T * ) ssGetRootDWork ( S ) ) ; _rtB = ( (
B_single_phase_PVarry_inverter_T * ) _ssGetModelBlockIO ( S ) ) ;
ssCallAccelRunBlock ( S , 0 , 0 , SS_CALL_MDL_UPDATE ) ; } void
single_phase_PVarry_inverter_Synthesized_Atomic_Subsystem_For_Alg_Loop_1_Term
( SimStruct * const S ) { } static void mdlOutputs ( SimStruct * S , int_T
tid ) { real_T * lastU ; real_T riseValLimit ; boolean_T rtb_B_3_38_0 ;
boolean_T rtb_B_3_37_0 ; real_T rtb_B_3_13_0 ; real_T rtb_B_3_23_0 ; real_T
rtb_B_3_12_0 ; int32_T isHit ; B_single_phase_PVarry_inverter_T * _rtB ;
P_single_phase_PVarry_inverter_T * _rtP ; X_single_phase_PVarry_inverter_T *
_rtX ; DW_single_phase_PVarry_inverter_T * _rtDW ; _rtDW = ( (
DW_single_phase_PVarry_inverter_T * ) ssGetRootDWork ( S ) ) ; _rtX = ( (
X_single_phase_PVarry_inverter_T * ) ssGetContStates ( S ) ) ; _rtP = ( (
P_single_phase_PVarry_inverter_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_single_phase_PVarry_inverter_T * ) _ssGetModelBlockIO ( S ) ) ; _rtB ->
B_3_1_0 = muDoubleScalarSin ( _rtP -> P_15 * ssGetTaskTime ( S , 0 ) + _rtP
-> P_16 ) * _rtP -> P_13 + _rtP -> P_14 ; ssCallAccelRunBlock ( S , 3 , 2 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_3_3_0 = _rtP -> P_41 * _rtB -> B_3_2_0 [ 7
] ; _rtB -> B_3_4_0 = _rtP -> P_42 * _rtB -> B_3_2_0 [ 6 ] ; _rtB -> B_3_5_0
= _rtP -> P_43 * _rtB -> B_3_2_0 [ 4 ] ; _rtB -> B_3_6_0 = _rtP -> P_44 *
_rtB -> B_3_2_0 [ 5 ] ; ssCallAccelRunBlock ( S , 3 , 7 , SS_CALL_MDL_OUTPUTS
) ; _rtB -> B_3_8_0 = _rtP -> P_45 * _rtB -> B_3_4_0 ; ssCallAccelRunBlock (
S , 3 , 9 , SS_CALL_MDL_OUTPUTS ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { if
( _rtX -> Integrator_CSTATE >= _rtP -> P_47 ) { _rtX -> Integrator_CSTATE =
_rtP -> P_47 ; } else { if ( _rtX -> Integrator_CSTATE <= _rtP -> P_48 ) {
_rtX -> Integrator_CSTATE = _rtP -> P_48 ; } } } _rtB -> B_3_10_0 = _rtX ->
Integrator_CSTATE ; rtb_B_3_12_0 = muDoubleScalarMod ( _rtB -> B_3_10_0 ,
_rtB -> B_3_11_0 ) ; rtb_B_3_13_0 = muDoubleScalarSin ( rtb_B_3_12_0 ) ; {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay1_PWORK .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
TransportDelay1_PWORK . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_50 ; _rtB -> B_3_14_0 =
single_phase_PVarry_inverter_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , _rtDW -> TransportDelay1_IWORK . CircularBufSize , &
_rtDW -> TransportDelay1_IWORK . Last , _rtDW -> TransportDelay1_IWORK . Tail
, _rtDW -> TransportDelay1_IWORK . Head , _rtP -> P_51 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } if ( _rtP -> P_116 == 1 ) { _rtB -> B_3_16_0 = _rtB -> B_3_14_0 ; }
else { _rtB -> B_3_16_0 = _rtB -> B_3_15_0 ; } _rtB -> B_3_17_0 =
rtb_B_3_13_0 * _rtB -> B_3_16_0 ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if (
isHit != 0 ) { _rtB -> B_3_18_0 = _rtB -> B_3_4_0 ; } _rtB -> B_3_19_0 = _rtB
-> B_3_17_0 - _rtB -> B_3_18_0 ; _rtB -> B_3_21_0 = _rtX ->
Integrator_CSTATE_b ; _rtB -> B_3_22_0 = _rtP -> P_55 * _rtB -> B_3_19_0 ; if
( ( _rtDW -> TimeStampA >= ssGetT ( S ) ) && ( _rtDW -> TimeStampB >= ssGetT
( S ) ) ) { rtb_B_3_23_0 = 0.0 ; } else { rtb_B_3_23_0 = _rtDW -> TimeStampA
; lastU = & _rtDW -> LastUAtTimeA ; if ( _rtDW -> TimeStampA < _rtDW ->
TimeStampB ) { if ( _rtDW -> TimeStampB < ssGetT ( S ) ) { rtb_B_3_23_0 =
_rtDW -> TimeStampB ; lastU = & _rtDW -> LastUAtTimeB ; } } else { if ( _rtDW
-> TimeStampA >= ssGetT ( S ) ) { rtb_B_3_23_0 = _rtDW -> TimeStampB ; lastU
= & _rtDW -> LastUAtTimeB ; } } rtb_B_3_23_0 = ( _rtB -> B_3_22_0 - * lastU )
/ ( ssGetT ( S ) - rtb_B_3_23_0 ) ; } _rtB -> B_3_24_0 = ( _rtP -> P_53 *
_rtB -> B_3_19_0 + _rtB -> B_3_21_0 ) + rtb_B_3_23_0 ; if ( ssIsMajorTimeStep
( S ) != 0 ) { _rtDW -> Saturation_MODE = _rtB -> B_3_24_0 >= _rtP -> P_56 ?
1 : _rtB -> B_3_24_0 > _rtP -> P_57 ? 0 : - 1 ; } _rtB -> B_3_25_0 = _rtDW ->
Saturation_MODE == 1 ? _rtP -> P_56 : _rtDW -> Saturation_MODE == - 1 ? _rtP
-> P_57 : _rtB -> B_3_24_0 ; ssCallAccelRunBlock ( S , 3 , 26 ,
SS_CALL_MDL_OUTPUTS ) ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0
) { _rtB -> B_3_27_0 = _rtB -> B_3_3_0 ; _rtB -> B_3_28_0 = _rtP -> P_58 *
_rtB -> B_3_27_0 ; } _rtB -> B_3_30_0 = ( _rtB -> B_3_25_0 - _rtB -> B_3_28_0
) * _rtP -> P_59 ; _rtB -> B_3_36_0 = look1_binlxpw ( muDoubleScalarRem (
ssGetT ( S ) - _rtB -> B_3_32_0 , _rtB -> B_3_34_0 ) , _rtP -> P_62 , _rtP ->
P_61 , 2U ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if (
ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> RelationalOperator_Mode = ( _rtB ->
B_3_30_0 >= _rtB -> B_3_36_0 ) ; } rtb_B_3_37_0 = _rtDW ->
RelationalOperator_Mode ; rtb_B_3_38_0 = ! _rtDW -> RelationalOperator_Mode ;
} _rtB -> B_3_39_0 = _rtB -> B_3_30_0 + _rtB -> B_3_36_0 ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( ssIsMajorTimeStep ( S
) != 0 ) { _rtDW -> RelationalOperator1_Mode = ( _rtB -> B_3_39_0 >= _rtB ->
B_3_40_0 ) ; } _rtB -> B_3_43_0 [ 0 ] = rtb_B_3_37_0 ; _rtB -> B_3_43_0 [ 1 ]
= rtb_B_3_38_0 ; _rtB -> B_3_43_0 [ 2 ] = ! _rtDW -> RelationalOperator1_Mode
; _rtB -> B_3_43_0 [ 3 ] = _rtDW -> RelationalOperator1_Mode ; } _rtB ->
B_3_45_0 [ 0 ] = _rtP -> P_65 * _rtB -> B_3_2_0 [ 0 ] ; _rtB -> B_3_45_0 [ 1
] = _rtP -> P_65 * _rtB -> B_3_2_0 [ 1 ] ; _rtB -> B_3_45_0 [ 2 ] = _rtP ->
P_65 * _rtB -> B_3_2_0 [ 2 ] ; _rtB -> B_3_45_0 [ 3 ] = _rtP -> P_65 * _rtB
-> B_3_2_0 [ 3 ] ; if ( ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> Switch_Mode
[ 0 ] = ( _rtB -> B_3_2_1 [ 0 ] >= _rtP -> P_66 ) ; _rtDW -> Switch_Mode [ 1
] = ( _rtB -> B_3_2_1 [ 1 ] >= _rtP -> P_66 ) ; _rtDW -> Switch_Mode [ 2 ] =
( _rtB -> B_3_2_1 [ 2 ] >= _rtP -> P_66 ) ; _rtDW -> Switch_Mode [ 3 ] = (
_rtB -> B_3_2_1 [ 3 ] >= _rtP -> P_66 ) ; } if ( _rtDW -> Switch_Mode [ 0 ] )
{ _rtB -> B_3_46_0 [ 0 ] = _rtB -> B_3_45_0 [ 0 ] ; } else { _rtB -> B_3_46_0
[ 0 ] = _rtB -> B_3_44_0 ; } if ( _rtDW -> Switch_Mode [ 1 ] ) { _rtB ->
B_3_46_0 [ 1 ] = _rtB -> B_3_45_0 [ 1 ] ; } else { _rtB -> B_3_46_0 [ 1 ] =
_rtB -> B_3_44_0 ; } if ( _rtDW -> Switch_Mode [ 2 ] ) { _rtB -> B_3_46_0 [ 2
] = _rtB -> B_3_45_0 [ 2 ] ; } else { _rtB -> B_3_46_0 [ 2 ] = _rtB ->
B_3_44_0 ; } if ( _rtDW -> Switch_Mode [ 3 ] ) { _rtB -> B_3_46_0 [ 3 ] =
_rtB -> B_3_45_0 [ 3 ] ; } else { _rtB -> B_3_46_0 [ 3 ] = _rtB -> B_3_44_0 ;
} if ( ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> Saturation_MODE_d [ 0 ] =
_rtB -> B_3_46_0 [ 0 ] >= _rtP -> P_67 ? 1 : _rtB -> B_3_46_0 [ 0 ] > _rtP ->
P_68 ? 0 : - 1 ; _rtDW -> Saturation_MODE_d [ 1 ] = _rtB -> B_3_46_0 [ 1 ] >=
_rtP -> P_67 ? 1 : _rtB -> B_3_46_0 [ 1 ] > _rtP -> P_68 ? 0 : - 1 ; _rtDW ->
Saturation_MODE_d [ 2 ] = _rtB -> B_3_46_0 [ 2 ] >= _rtP -> P_67 ? 1 : _rtB
-> B_3_46_0 [ 2 ] > _rtP -> P_68 ? 0 : - 1 ; _rtDW -> Saturation_MODE_d [ 3 ]
= _rtB -> B_3_46_0 [ 3 ] >= _rtP -> P_67 ? 1 : _rtB -> B_3_46_0 [ 3 ] > _rtP
-> P_68 ? 0 : - 1 ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 )
{ single_phase_PVarry_inverter_Synthesized_Atomic_Subsystem_For_Alg_Loop_1 (
S ) ; _rtB -> B_3_88_0 = _rtB -> B_0_0_0 * _rtB -> B_0_1_0 ; } _rtB ->
B_3_89_0 = _rtX -> integ_CSTATE ; { real_T * * uBuffer = ( real_T * * ) &
_rtDW -> T_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) &
_rtDW -> T_PWORK . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_85 ; _rtB -> B_3_90_0 =
single_phase_PVarry_inverter_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , _rtDW -> T_IWORK . CircularBufSize , & _rtDW -> T_IWORK
. Last , _rtDW -> T_IWORK . Tail , _rtDW -> T_IWORK . Head , _rtP -> P_86 , 0
, ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) ==
ssGetT ( S ) ) ) ) ; } _rtB -> B_3_91_0 = _rtB -> B_3_89_0 - _rtB -> B_3_90_0
; if ( ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> Abs_MODE = ( _rtB ->
B_3_91_0 >= 0.0 ) ; } _rtB -> B_3_92_0 = _rtDW -> Abs_MODE > 0 ? _rtB ->
B_3_91_0 : - _rtB -> B_3_91_0 ; if ( ssIsMajorTimeStep ( S ) != 0 ) { if (
_rtDW -> Sqrt_DWORK1 != 0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S
) ; _rtDW -> Sqrt_DWORK1 = 0 ; } rtb_B_3_23_0 = muDoubleScalarSqrt ( _rtB ->
B_3_92_0 ) ; } else { if ( _rtB -> B_3_92_0 < 0.0 ) { rtb_B_3_23_0 = -
muDoubleScalarSqrt ( muDoubleScalarAbs ( _rtB -> B_3_92_0 ) ) ; } else {
rtb_B_3_23_0 = muDoubleScalarSqrt ( _rtB -> B_3_92_0 ) ; } if ( _rtB ->
B_3_92_0 < 0.0 ) { _rtDW -> Sqrt_DWORK1 = 1 ; } } _rtB -> B_3_94_0 = _rtB ->
B_3_88_0 / rtb_B_3_23_0 ; _rtB -> B_3_95_0 = _rtP -> P_87 * _rtB -> B_3_19_0
; _rtB -> B_3_96_0 = 0.0 ; _rtB -> B_3_96_0 += _rtP -> P_90 * _rtX ->
StateSpace_CSTATE_o [ 1 ] ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit
!= 0 ) { _rtB -> B_3_97_0 = _rtB -> B_3_5_0 ; _rtB -> B_3_98_0 = _rtP -> P_92
* _rtB -> B_3_97_0 ; } rtb_B_3_12_0 = muDoubleScalarCos ( rtb_B_3_12_0 ) ;
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) && (
ssIsMajorTimeStep ( S ) != 0 ) ) { if ( _rtB -> B_3_100_0 > 0.0 ) { if ( !
_rtDW -> AutomaticGainControl_MODE ) { if ( ssGetTaskTime ( S , 1 ) !=
ssGetTStart ( S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } (
void ) memset ( & ( ( ( XDis_single_phase_PVarry_inverter_T * )
ssGetContStateDisabled ( S ) ) -> integ1_CSTATE ) , 0 , 2 * sizeof (
boolean_T ) ) ; _rtDW -> AutomaticGainControl_MODE = true ; } } else { if (
_rtDW -> AutomaticGainControl_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void ) memset ( & ( ( (
XDis_single_phase_PVarry_inverter_T * ) ssGetContStateDisabled ( S ) ) ->
integ1_CSTATE ) , 1 , 2 * sizeof ( boolean_T ) ) ; _rtDW ->
AutomaticGainControl_MODE = false ; } } } if ( _rtDW ->
AutomaticGainControl_MODE ) { _rtB -> B_1_0_0 = _rtX -> integ1_CSTATE ; if (
ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> Toavoiddivisionbyzero_MODE_c = _rtB
-> B_3_96_0 >= _rtP -> P_2 ? 1 : _rtB -> B_3_96_0 > _rtP -> P_3 ? 0 : - 1 ; }
_rtB -> B_1_1_0 = _rtDW -> Toavoiddivisionbyzero_MODE_c == 1 ? _rtP -> P_2 :
_rtDW -> Toavoiddivisionbyzero_MODE_c == - 1 ? _rtP -> P_3 : _rtB -> B_3_96_0
; _rtB -> B_1_2_0 = 1.0 / _rtB -> B_1_1_0 ; { real_T * * uBuffer = ( real_T *
* ) & _rtDW -> VariableTransportDelay_PWORK_b . TUbufferPtrs [ 0 ] ; real_T *
* tBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_b .
TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; real_T appliedDelay ;
appliedDelay = _rtB -> B_1_2_0 ; if ( appliedDelay > _rtP -> P_4 ) {
appliedDelay = _rtP -> P_4 ; } if ( appliedDelay < 0.0 ) { appliedDelay = 0.0
; } _rtB -> B_1_3_0 = single_phase_PVarry_inverter_acc_rt_TDelayInterpolate (
simTime - appliedDelay , 0.0 , * tBuffer , * uBuffer , _rtDW ->
VariableTransportDelay_IWORK_m . CircularBufSize , & _rtDW ->
VariableTransportDelay_IWORK_m . Last , _rtDW ->
VariableTransportDelay_IWORK_m . Tail , _rtDW ->
VariableTransportDelay_IWORK_m . Head , _rtP -> P_5 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } _rtB -> B_1_4_0 = _rtB -> B_1_0_0 - _rtB -> B_1_3_0 ; rtb_B_3_23_0 =
muDoubleScalarPower ( _rtB -> B_1_4_0 , 2.0 ) ; _rtB -> B_1_6_0 = _rtX ->
Integ2_CSTATE ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay1_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = (
real_T * * ) & _rtDW -> VariableTransportDelay1_PWORK . TUbufferPtrs [ 1 ] ;
real_T simTime = ssGetT ( S ) ; real_T appliedDelay ; appliedDelay = _rtB ->
B_1_2_0 ; if ( appliedDelay > _rtP -> P_7 ) { appliedDelay = _rtP -> P_7 ; }
if ( appliedDelay < 0.0 ) { appliedDelay = 0.0 ; } _rtB -> B_1_7_0 =
single_phase_PVarry_inverter_acc_rt_TDelayInterpolate ( simTime -
appliedDelay , 0.0 , * tBuffer , * uBuffer , _rtDW ->
VariableTransportDelay1_IWORK . CircularBufSize , & _rtDW ->
VariableTransportDelay1_IWORK . Last , _rtDW -> VariableTransportDelay1_IWORK
. Tail , _rtDW -> VariableTransportDelay1_IWORK . Head , _rtP -> P_8 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) ==
ssGetT ( S ) ) ) ) ; } _rtB -> B_1_8_0 = _rtB -> B_1_6_0 - _rtB -> B_1_7_0 ;
_rtB -> B_1_10_0 = rtb_B_3_23_0 + muDoubleScalarPower ( _rtB -> B_1_8_0 , 2.0
) ; if ( _rtB -> B_1_10_0 < 0.0 ) { _rtB -> B_1_11_0 = - muDoubleScalarSqrt (
- _rtB -> B_1_10_0 ) ; } else { _rtB -> B_1_11_0 = muDoubleScalarSqrt ( _rtB
-> B_1_10_0 ) ; } _rtB -> B_1_12_0 = _rtP -> P_9 * _rtB -> B_3_96_0 ; if (
ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> Saturation_MODE_j = _rtB ->
B_1_11_0 >= _rtP -> P_10 ? 1 : _rtB -> B_1_11_0 > _rtP -> P_11 ? 0 : - 1 ; }
_rtB -> B_1_13_0 = _rtDW -> Saturation_MODE_j == 1 ? _rtP -> P_10 : _rtDW ->
Saturation_MODE_j == - 1 ? _rtP -> P_11 : _rtB -> B_1_11_0 ; _rtB -> B_1_14_0
= 1.0 / _rtB -> B_1_13_0 ; _rtB -> B_1_15_0 = _rtB -> B_3_98_0 * _rtB ->
B_1_12_0 ; _rtB -> B_1_16_0 = rtb_B_3_13_0 * _rtB -> B_1_15_0 ; _rtB ->
B_1_17_0 = _rtB -> B_1_15_0 * rtb_B_3_12_0 ; if ( ssIsMajorTimeStep ( S ) !=
0 ) { srUpdateBC ( _rtDW -> AutomaticGainControl_SubsysRanBC ) ; } } if (
ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> Toavoiddivisionbyzero_MODE = _rtB
-> B_3_96_0 >= _rtP -> P_94 ? 1 : _rtB -> B_3_96_0 > _rtP -> P_95 ? 0 : - 1 ;
} _rtB -> B_3_102_0 = _rtDW -> Toavoiddivisionbyzero_MODE == 1 ? _rtP -> P_94
: _rtDW -> Toavoiddivisionbyzero_MODE == - 1 ? _rtP -> P_95 : _rtB ->
B_3_96_0 ; _rtB -> B_3_103_0 = 1.0 / _rtB -> B_3_102_0 ; { real_T * * uBuffer
= ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK . TUbufferPtrs [ 0 ]
; real_T * * tBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK
. TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; real_T appliedDelay ;
appliedDelay = _rtB -> B_3_103_0 ; if ( appliedDelay > _rtP -> P_96 ) {
appliedDelay = _rtP -> P_96 ; } if ( appliedDelay < 0.0 ) { appliedDelay =
0.0 ; } _rtB -> B_3_104_0 =
single_phase_PVarry_inverter_acc_rt_TDelayInterpolate ( simTime -
appliedDelay , 0.0 , * tBuffer , * uBuffer , _rtDW ->
VariableTransportDelay_IWORK . CircularBufSize , & _rtDW ->
VariableTransportDelay_IWORK . Last , _rtDW -> VariableTransportDelay_IWORK .
Tail , _rtDW -> VariableTransportDelay_IWORK . Head , _rtP -> P_97 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) ==
ssGetT ( S ) ) ) ) ; } _rtB -> B_3_105_0 = _rtX -> Integrator_CSTATE_i ;
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { rtb_B_3_13_0 =
ssGetTaskTime ( S , 1 ) ; _rtDW -> Step_MODE = ( rtb_B_3_13_0 >= _rtP -> P_99
) ; if ( _rtDW -> Step_MODE == 1 ) { _rtB -> B_3_106_0 = _rtP -> P_101 ; }
else { _rtB -> B_3_106_0 = _rtP -> P_100 ; } } if ( _rtB -> B_3_106_0 >= _rtP
-> P_103 ) { _rtB -> B_2_0_0 = _rtB -> B_3_105_0 - _rtB -> B_3_104_0 ; _rtB
-> B_2_1_0 = _rtB -> B_2_0_0 * _rtB -> B_3_96_0 ; _rtB -> B_3_109_0 = _rtB ->
B_2_1_0 ; } else { _rtB -> B_3_109_0 = _rtB -> B_3_107_0 ; } _rtB ->
B_3_110_0 = _rtB -> B_3_109_0 * _rtB -> B_1_14_0 ; _rtB -> B_3_111_0 = _rtP
-> P_104 * _rtB -> B_3_110_0 ; if ( ssIsMajorTimeStep ( S ) != 0 ) { if (
_rtX -> Integrator_CSTATE_h >= _rtP -> P_106 ) { _rtX -> Integrator_CSTATE_h
= _rtP -> P_106 ; } else { if ( _rtX -> Integrator_CSTATE_h <= _rtP -> P_107
) { _rtX -> Integrator_CSTATE_h = _rtP -> P_107 ; } } } _rtB -> B_3_112_0 =
_rtX -> Integrator_CSTATE_h ; _rtB -> B_3_113_0 = _rtP -> P_108 * _rtB ->
B_3_110_0 ; if ( ( _rtDW -> TimeStampA_k >= ssGetT ( S ) ) && ( _rtDW ->
TimeStampB_p >= ssGetT ( S ) ) ) { _rtB -> B_3_114_0 = 0.0 ; } else {
rtb_B_3_23_0 = _rtDW -> TimeStampA_k ; lastU = & _rtDW -> LastUAtTimeA_o ; if
( _rtDW -> TimeStampA_k < _rtDW -> TimeStampB_p ) { if ( _rtDW ->
TimeStampB_p < ssGetT ( S ) ) { rtb_B_3_23_0 = _rtDW -> TimeStampB_p ; lastU
= & _rtDW -> LastUAtTimeB_i ; } } else { if ( _rtDW -> TimeStampA_k >= ssGetT
( S ) ) { rtb_B_3_23_0 = _rtDW -> TimeStampB_p ; lastU = & _rtDW ->
LastUAtTimeB_i ; } } _rtB -> B_3_114_0 = ( _rtB -> B_3_113_0 - * lastU ) / (
ssGetT ( S ) - rtb_B_3_23_0 ) ; } _rtB -> B_3_115_0 = ( _rtB -> B_3_111_0 +
_rtB -> B_3_112_0 ) + _rtB -> B_3_114_0 ; if ( ssIsMajorTimeStep ( S ) != 0 )
{ _rtDW -> Saturation2_MODE = _rtB -> B_3_115_0 >= _rtP -> P_109 ? 1 : _rtB
-> B_3_115_0 > _rtP -> P_110 ? 0 : - 1 ; } _rtB -> B_3_116_0 = _rtDW ->
Saturation2_MODE == 1 ? _rtP -> P_109 : _rtDW -> Saturation2_MODE == - 1 ?
_rtP -> P_110 : _rtB -> B_3_115_0 ; _rtB -> B_3_117_0 = _rtP -> P_111 * _rtB
-> B_3_116_0 ; _rtB -> B_3_118_0 = _rtP -> P_112 * _rtB -> B_3_110_0 ; _rtB
-> B_3_119_0 = _rtB -> B_3_98_0 * rtb_B_3_12_0 ; if ( ( _rtDW ->
LastMajorTimeA >= ssGetTaskTime ( S , 0 ) ) && ( _rtDW -> LastMajorTimeB >=
ssGetTaskTime ( S , 0 ) ) ) { _rtB -> B_3_120_0 = _rtB -> B_3_117_0 ; } else
{ if ( ( ( _rtDW -> LastMajorTimeA < _rtDW -> LastMajorTimeB ) && ( _rtDW ->
LastMajorTimeB < ssGetTaskTime ( S , 0 ) ) ) || ( ( _rtDW -> LastMajorTimeA
>= _rtDW -> LastMajorTimeB ) && ( _rtDW -> LastMajorTimeA >= ssGetTaskTime (
S , 0 ) ) ) ) { rtb_B_3_23_0 = ssGetTaskTime ( S , 0 ) - _rtDW ->
LastMajorTimeB ; rtb_B_3_13_0 = _rtDW -> PrevYB ; } else { rtb_B_3_23_0 =
ssGetTaskTime ( S , 0 ) - _rtDW -> LastMajorTimeA ; rtb_B_3_13_0 = _rtDW ->
PrevYA ; } riseValLimit = rtb_B_3_23_0 * _rtP -> P_113 ; rtb_B_3_12_0 = _rtB
-> B_3_117_0 - rtb_B_3_13_0 ; if ( rtb_B_3_12_0 > riseValLimit ) { _rtB ->
B_3_120_0 = rtb_B_3_13_0 + riseValLimit ; } else { rtb_B_3_23_0 *= _rtP ->
P_114 ; if ( rtb_B_3_12_0 < rtb_B_3_23_0 ) { _rtB -> B_3_120_0 = rtb_B_3_13_0
+ rtb_B_3_23_0 ; } else { _rtB -> B_3_120_0 = _rtB -> B_3_117_0 ; } } } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock ( S ,
3 , 121 , SS_CALL_MDL_OUTPUTS ) ; } _rtB -> B_3_123_0 = _rtB -> B_3_5_0 *
_rtB -> B_3_5_0 * _rtP -> P_115 ; UNUSED_PARAMETER ( tid ) ; } static void
mdlOutputsTID4 ( SimStruct * S , int_T tid ) { real_T rtb_B_3_77_0 ; real_T
tmp ; real_T rtb_B_3_66_0 ; real_T rtb_B_3_81_0 ;
B_single_phase_PVarry_inverter_T * _rtB ; P_single_phase_PVarry_inverter_T *
_rtP ; DW_single_phase_PVarry_inverter_T * _rtDW ; _rtDW = ( (
DW_single_phase_PVarry_inverter_T * ) ssGetRootDWork ( S ) ) ; _rtP = ( (
P_single_phase_PVarry_inverter_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_single_phase_PVarry_inverter_T * ) _ssGetModelBlockIO ( S ) ) ; _rtB ->
B_3_0_0 = _rtP -> P_12 ; _rtB -> B_3_11_0 = _rtP -> P_49 ; _rtB -> B_3_15_0 =
_rtP -> P_52 ; _rtB -> B_3_34_0 = _rtP -> P_60 ; _rtB -> B_3_40_0 = _rtP ->
P_63 ; _rtB -> B_3_44_0 = _rtP -> P_64 ; _rtB -> B_3_62_0 = _rtP -> P_69 -
_rtP -> P_71 ; rtb_B_3_66_0 = _rtP -> P_70 / _rtP -> P_72 - _rtP -> P_73 ;
_rtB -> B_3_68_0 = _rtP -> P_75 ; rtb_B_3_66_0 = _rtB -> B_3_62_0 * _rtP ->
P_74 * rtb_B_3_66_0 + rtb_B_3_66_0 * _rtB -> B_3_68_0 ; _rtB -> B_3_72_0 = -
_rtP -> P_76 * _rtB -> B_3_62_0 - _rtP -> P_77 * rtb_B_3_66_0 ; rtb_B_3_66_0
= _rtP -> P_79 - 1.0 / _rtB -> B_3_68_0 * _rtP -> P_78 ; rtb_B_3_77_0 = _rtP
-> P_80 ; _rtB -> B_3_78_0 = _rtP -> P_81 ; rtb_B_3_81_0 = _rtP -> P_80 /
_rtB -> B_3_78_0 - _rtP -> P_82 ; tmp = rtb_B_3_66_0 ; if ( ssIsMajorTimeStep
( S ) != 0 ) { if ( _rtDW -> C2_DWORK1 != 0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; _rtDW ->
C2_DWORK1 = 0 ; } } else { if ( rtb_B_3_66_0 < 0.0 ) { tmp = 0.0 ; _rtDW ->
C2_DWORK1 = 1 ; } } _rtB -> B_3_82_0 = rtb_B_3_81_0 / muDoubleScalarLog ( tmp
) ; _rtB -> B_3_86_0 = muDoubleScalarExp ( rtb_B_3_77_0 / _rtB -> B_3_82_0 /
_rtB -> B_3_78_0 * _rtP -> P_83 ) * rtb_B_3_66_0 ; _rtB -> B_3_100_0 = _rtP
-> P_93 ; _rtB -> B_3_107_0 = _rtP -> P_102 ; UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { real_T * lastU ;
int32_T isHit ; B_single_phase_PVarry_inverter_T * _rtB ;
P_single_phase_PVarry_inverter_T * _rtP ; X_single_phase_PVarry_inverter_T *
_rtX ; DW_single_phase_PVarry_inverter_T * _rtDW ; _rtDW = ( (
DW_single_phase_PVarry_inverter_T * ) ssGetRootDWork ( S ) ) ; _rtX = ( (
X_single_phase_PVarry_inverter_T * ) ssGetContStates ( S ) ) ; _rtP = ( (
P_single_phase_PVarry_inverter_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_single_phase_PVarry_inverter_T * ) _ssGetModelBlockIO ( S ) ) ;
ssCallAccelRunBlock ( S , 3 , 2 , SS_CALL_MDL_UPDATE ) ; if ( _rtX ->
Integrator_CSTATE == _rtP -> P_47 ) { switch ( _rtDW -> Integrator_MODE ) {
case 3 : if ( _rtB -> B_3_116_0 < 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE =
1 ; } break ; case 1 : if ( _rtB -> B_3_116_0 >= 0.0 ) { _rtDW ->
Integrator_MODE = 3 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_3_116_0 < 0.0 ) { _rtDW -> Integrator_MODE = 1 ; } else { _rtDW ->
Integrator_MODE = 3 ; } break ; } } else if ( _rtX -> Integrator_CSTATE ==
_rtP -> P_48 ) { switch ( _rtDW -> Integrator_MODE ) { case 4 : if ( _rtB ->
B_3_116_0 > 0.0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW
-> Integrator_MODE = 2 ; } break ; case 2 : if ( _rtB -> B_3_116_0 <= 0.0 ) {
_rtDW -> Integrator_MODE = 4 ; ssSetBlockStateForSolverChangedAtMajorStep ( S
) ; } break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if
( _rtB -> B_3_116_0 > 0.0 ) { _rtDW -> Integrator_MODE = 2 ; } else { _rtDW
-> Integrator_MODE = 4 ; } break ; } } else { _rtDW -> Integrator_MODE = 0 ;
} { real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay1_PWORK .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
TransportDelay1_PWORK . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ;
_rtDW -> TransportDelay1_IWORK . Head = ( ( _rtDW -> TransportDelay1_IWORK .
Head < ( _rtDW -> TransportDelay1_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW
-> TransportDelay1_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay1_IWORK . Head == _rtDW -> TransportDelay1_IWORK . Tail ) { if
( ! single_phase_PVarry_inverter_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW
-> TransportDelay1_IWORK . CircularBufSize , & _rtDW -> TransportDelay1_IWORK
. Tail , & _rtDW -> TransportDelay1_IWORK . Head , & _rtDW ->
TransportDelay1_IWORK . Last , simTime - _rtP -> P_50 , tBuffer , uBuffer , (
NULL ) , ( boolean_T ) 0 , false , & _rtDW -> TransportDelay1_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , "tdelay memory allocation error" )
; return ; } } ( * tBuffer ) [ _rtDW -> TransportDelay1_IWORK . Head ] =
simTime ; ( * uBuffer ) [ _rtDW -> TransportDelay1_IWORK . Head ] = _rtB ->
B_3_94_0 ; } if ( _rtDW -> TimeStampA == ( rtInf ) ) { _rtDW -> TimeStampA =
ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA ; } else if ( _rtDW ->
TimeStampB == ( rtInf ) ) { _rtDW -> TimeStampB = ssGetT ( S ) ; lastU = &
_rtDW -> LastUAtTimeB ; } else if ( _rtDW -> TimeStampA < _rtDW -> TimeStampB
) { _rtDW -> TimeStampA = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA ; }
else { _rtDW -> TimeStampB = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB ;
} * lastU = _rtB -> B_3_22_0 ; isHit = ssIsSampleHit ( S , 2 , 0 ) ; if (
isHit != 0 ) {
single_phase_PVarry_inverter_Synthesized_Atomic_Subsystem_For_Alg_Loop_1_Update
( S ) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW -> T_PWORK .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> T_PWORK .
TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW -> T_IWORK . Head
= ( ( _rtDW -> T_IWORK . Head < ( _rtDW -> T_IWORK . CircularBufSize - 1 ) )
? ( _rtDW -> T_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW -> T_IWORK . Head ==
_rtDW -> T_IWORK . Tail ) { if ( !
single_phase_PVarry_inverter_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
T_IWORK . CircularBufSize , & _rtDW -> T_IWORK . Tail , & _rtDW -> T_IWORK .
Head , & _rtDW -> T_IWORK . Last , simTime - _rtP -> P_85 , tBuffer , uBuffer
, ( NULL ) , ( boolean_T ) 0 , false , & _rtDW -> T_IWORK . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } (
* tBuffer ) [ _rtDW -> T_IWORK . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
T_IWORK . Head ] = _rtB -> B_3_89_0 ; } if ( _rtDW ->
AutomaticGainControl_MODE ) { { real_T * * uBuffer = ( real_T * * ) & _rtDW
-> VariableTransportDelay_PWORK_b . TUbufferPtrs [ 0 ] ; real_T * * tBuffer =
( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_b . TUbufferPtrs [ 1 ]
; real_T * * xBuffer = ( NULL ) ; real_T simTime = ssGetT ( S ) ; _rtDW ->
VariableTransportDelay_IWORK_m . Head = ( ( _rtDW ->
VariableTransportDelay_IWORK_m . Head < ( _rtDW ->
VariableTransportDelay_IWORK_m . CircularBufSize - 1 ) ) ? ( _rtDW ->
VariableTransportDelay_IWORK_m . Head + 1 ) : 0 ) ; if ( _rtDW ->
VariableTransportDelay_IWORK_m . Head == _rtDW ->
VariableTransportDelay_IWORK_m . Tail ) { if ( !
single_phase_PVarry_inverter_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
VariableTransportDelay_IWORK_m . CircularBufSize , & _rtDW ->
VariableTransportDelay_IWORK_m . Tail , & _rtDW ->
VariableTransportDelay_IWORK_m . Head , & _rtDW ->
VariableTransportDelay_IWORK_m . Last , simTime - _rtP -> P_4 , tBuffer ,
uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 0 , & _rtDW ->
VariableTransportDelay_IWORK_m . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
VariableTransportDelay_IWORK_m . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
VariableTransportDelay_IWORK_m . Head ] = _rtB -> B_1_0_0 ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay1_PWORK .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay1_PWORK . TUbufferPtrs [ 1 ] ; real_T * * xBuffer = (
NULL ) ; real_T simTime = ssGetT ( S ) ; _rtDW ->
VariableTransportDelay1_IWORK . Head = ( ( _rtDW ->
VariableTransportDelay1_IWORK . Head < ( _rtDW ->
VariableTransportDelay1_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
VariableTransportDelay1_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW ->
VariableTransportDelay1_IWORK . Head == _rtDW ->
VariableTransportDelay1_IWORK . Tail ) { if ( !
single_phase_PVarry_inverter_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
VariableTransportDelay1_IWORK . CircularBufSize , & _rtDW ->
VariableTransportDelay1_IWORK . Tail , & _rtDW ->
VariableTransportDelay1_IWORK . Head , & _rtDW ->
VariableTransportDelay1_IWORK . Last , simTime - _rtP -> P_7 , tBuffer ,
uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 0 , & _rtDW ->
VariableTransportDelay1_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
VariableTransportDelay1_IWORK . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
VariableTransportDelay1_IWORK . Head ] = _rtB -> B_1_6_0 ; } } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay_PWORK . TUbufferPtrs [ 1 ] ; real_T * * xBuffer = (
NULL ) ; real_T simTime = ssGetT ( S ) ; _rtDW ->
VariableTransportDelay_IWORK . Head = ( ( _rtDW ->
VariableTransportDelay_IWORK . Head < ( _rtDW -> VariableTransportDelay_IWORK
. CircularBufSize - 1 ) ) ? ( _rtDW -> VariableTransportDelay_IWORK . Head +
1 ) : 0 ) ; if ( _rtDW -> VariableTransportDelay_IWORK . Head == _rtDW ->
VariableTransportDelay_IWORK . Tail ) { if ( !
single_phase_PVarry_inverter_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
VariableTransportDelay_IWORK . CircularBufSize , & _rtDW ->
VariableTransportDelay_IWORK . Tail , & _rtDW -> VariableTransportDelay_IWORK
. Head , & _rtDW -> VariableTransportDelay_IWORK . Last , simTime - _rtP ->
P_96 , tBuffer , uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 0 , &
_rtDW -> VariableTransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus
( S , "vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [
_rtDW -> VariableTransportDelay_IWORK . Head ] = simTime ; ( * uBuffer ) [
_rtDW -> VariableTransportDelay_IWORK . Head ] = _rtB -> B_3_105_0 ; } if (
_rtX -> Integrator_CSTATE_h == _rtP -> P_106 ) { switch ( _rtDW ->
Integrator_MODE_l ) { case 3 : if ( _rtB -> B_3_118_0 < 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE_l
= 1 ; } break ; case 1 : if ( _rtB -> B_3_118_0 >= 0.0 ) { _rtDW ->
Integrator_MODE_l = 3 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_3_118_0 < 0.0 ) { _rtDW -> Integrator_MODE_l = 1 ; } else { _rtDW
-> Integrator_MODE_l = 3 ; } break ; } } else if ( _rtX ->
Integrator_CSTATE_h == _rtP -> P_107 ) { switch ( _rtDW -> Integrator_MODE_l
) { case 4 : if ( _rtB -> B_3_118_0 > 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE_l
= 2 ; } break ; case 2 : if ( _rtB -> B_3_118_0 <= 0.0 ) { _rtDW ->
Integrator_MODE_l = 4 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_3_118_0 > 0.0 ) { _rtDW -> Integrator_MODE_l = 2 ; } else { _rtDW
-> Integrator_MODE_l = 4 ; } break ; } } else { _rtDW -> Integrator_MODE_l =
0 ; } if ( _rtDW -> TimeStampA_k == ( rtInf ) ) { _rtDW -> TimeStampA_k =
ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_o ; } else if ( _rtDW ->
TimeStampB_p == ( rtInf ) ) { _rtDW -> TimeStampB_p = ssGetT ( S ) ; lastU =
& _rtDW -> LastUAtTimeB_i ; } else if ( _rtDW -> TimeStampA_k < _rtDW ->
TimeStampB_p ) { _rtDW -> TimeStampA_k = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeA_o ; } else { _rtDW -> TimeStampB_p = ssGetT ( S ) ; lastU = &
_rtDW -> LastUAtTimeB_i ; } * lastU = _rtB -> B_3_113_0 ; if ( _rtDW ->
LastMajorTimeA == ( rtInf ) ) { _rtDW -> LastMajorTimeA = ssGetTaskTime ( S ,
0 ) ; _rtDW -> PrevYA = _rtB -> B_3_120_0 ; } else if ( _rtDW ->
LastMajorTimeB == ( rtInf ) ) { _rtDW -> LastMajorTimeB = ssGetTaskTime ( S ,
0 ) ; _rtDW -> PrevYB = _rtB -> B_3_120_0 ; } else if ( _rtDW ->
LastMajorTimeA < _rtDW -> LastMajorTimeB ) { _rtDW -> LastMajorTimeA =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA = _rtB -> B_3_120_0 ; } else {
_rtDW -> LastMajorTimeB = ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYB = _rtB ->
B_3_120_0 ; } UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdateTID4 ( SimStruct * S , int_T tid ) { UNUSED_PARAMETER (
tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) {
B_single_phase_PVarry_inverter_T * _rtB ; P_single_phase_PVarry_inverter_T *
_rtP ; X_single_phase_PVarry_inverter_T * _rtX ;
XDot_single_phase_PVarry_inverter_T * _rtXdot ;
XDis_single_phase_PVarry_inverter_T * _rtXdis ;
DW_single_phase_PVarry_inverter_T * _rtDW ; _rtDW = ( (
DW_single_phase_PVarry_inverter_T * ) ssGetRootDWork ( S ) ) ; _rtXdis = ( (
XDis_single_phase_PVarry_inverter_T * ) ssGetContStateDisabled ( S ) ) ;
_rtXdot = ( ( XDot_single_phase_PVarry_inverter_T * ) ssGetdX ( S ) ) ; _rtX
= ( ( X_single_phase_PVarry_inverter_T * ) ssGetContStates ( S ) ) ; _rtP = (
( P_single_phase_PVarry_inverter_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_single_phase_PVarry_inverter_T * ) _ssGetModelBlockIO ( S ) ) ;
ssCallAccelRunBlock ( S , 3 , 2 , SS_CALL_MDL_DERIVATIVES ) ; if ( ( _rtDW ->
Integrator_MODE != 3 ) && ( _rtDW -> Integrator_MODE != 4 ) ) { _rtXdot ->
Integrator_CSTATE = _rtB -> B_3_116_0 ; _rtXdis -> Integrator_CSTATE = false
; } else { _rtXdot -> Integrator_CSTATE = 0.0 ; if ( ( _rtDW ->
Integrator_MODE == 3 ) || ( _rtDW -> Integrator_MODE == 4 ) ) { _rtXdis ->
Integrator_CSTATE = true ; } } _rtXdot -> Integrator_CSTATE_b = _rtB ->
B_3_95_0 ; _rtXdot -> integ_CSTATE = _rtB -> B_3_123_0 ; _rtXdot ->
StateSpace_CSTATE_o [ 0 ] = 0.0 ; _rtXdot -> StateSpace_CSTATE_o [ 1 ] = 0.0
; _rtXdot -> StateSpace_CSTATE_o [ 0 ] += _rtP -> P_88 [ 0 ] * _rtX ->
StateSpace_CSTATE_o [ 0 ] ; _rtXdot -> StateSpace_CSTATE_o [ 0 ] += _rtP ->
P_88 [ 1 ] * _rtX -> StateSpace_CSTATE_o [ 1 ] ; _rtXdot ->
StateSpace_CSTATE_o [ 1 ] += _rtP -> P_88 [ 2 ] * _rtX -> StateSpace_CSTATE_o
[ 0 ] ; _rtXdot -> StateSpace_CSTATE_o [ 0 ] += _rtP -> P_89 * _rtB ->
B_3_120_0 ; if ( _rtDW -> AutomaticGainControl_MODE ) { _rtXdot ->
integ1_CSTATE = _rtB -> B_1_16_0 ; { } _rtXdot -> Integ2_CSTATE = _rtB ->
B_1_17_0 ; { } } else { { real_T * dx ; int_T i ; dx = & ( ( (
XDot_single_phase_PVarry_inverter_T * ) ssGetdX ( S ) ) -> integ1_CSTATE ) ;
for ( i = 0 ; i < 2 ; i ++ ) { dx [ i ] = 0.0 ; } } } { } _rtXdot ->
Integrator_CSTATE_i = _rtB -> B_3_119_0 ; if ( ( _rtDW -> Integrator_MODE_l
!= 3 ) && ( _rtDW -> Integrator_MODE_l != 4 ) ) { _rtXdot ->
Integrator_CSTATE_h = _rtB -> B_3_118_0 ; _rtXdis -> Integrator_CSTATE_h =
false ; } else { _rtXdot -> Integrator_CSTATE_h = 0.0 ; if ( ( _rtDW ->
Integrator_MODE_l == 3 ) || ( _rtDW -> Integrator_MODE_l == 4 ) ) { _rtXdis
-> Integrator_CSTATE_h = true ; } } }
#define MDL_ZERO_CROSSINGS
static void mdlZeroCrossings ( SimStruct * S ) { boolean_T anyStateSaturated
; B_single_phase_PVarry_inverter_T * _rtB ; P_single_phase_PVarry_inverter_T
* _rtP ; X_single_phase_PVarry_inverter_T * _rtX ;
ZCV_single_phase_PVarry_inverter_T * _rtZCSV ;
DW_single_phase_PVarry_inverter_T * _rtDW ; _rtDW = ( (
DW_single_phase_PVarry_inverter_T * ) ssGetRootDWork ( S ) ) ; _rtZCSV = ( (
ZCV_single_phase_PVarry_inverter_T * ) ssGetSolverZcSignalVector ( S ) ) ;
_rtX = ( ( X_single_phase_PVarry_inverter_T * ) ssGetContStates ( S ) ) ;
_rtP = ( ( P_single_phase_PVarry_inverter_T * ) ssGetModelRtp ( S ) ) ; _rtB
= ( ( B_single_phase_PVarry_inverter_T * ) _ssGetModelBlockIO ( S ) ) ;
ssCallAccelRunBlock ( S , 3 , 2 , SS_CALL_MDL_ZERO_CROSSINGS ) ; if ( ( _rtDW
-> Integrator_MODE == 1 ) && ( _rtX -> Integrator_CSTATE >= _rtP -> P_47 ) )
{ _rtZCSV -> Integrator_IntgUpLimit_ZC = 0.0 ; } else { _rtZCSV ->
Integrator_IntgUpLimit_ZC = _rtX -> Integrator_CSTATE - _rtP -> P_47 ; } if (
( _rtDW -> Integrator_MODE == 2 ) && ( _rtX -> Integrator_CSTATE <= _rtP ->
P_48 ) ) { _rtZCSV -> Integrator_IntgLoLimit_ZC = 0.0 ; } else { _rtZCSV ->
Integrator_IntgLoLimit_ZC = _rtX -> Integrator_CSTATE - _rtP -> P_48 ; }
anyStateSaturated = false ; if ( ( _rtDW -> Integrator_MODE == 3 ) || ( _rtDW
-> Integrator_MODE == 4 ) ) { anyStateSaturated = true ; } if (
anyStateSaturated ) { _rtZCSV -> Integrator_LeaveSaturate_ZC = _rtB ->
B_3_116_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC = 0.0 ; } _rtZCSV
-> Saturation_UprLim_ZC = _rtB -> B_3_24_0 - _rtP -> P_56 ; _rtZCSV ->
Saturation_LwrLim_ZC = _rtB -> B_3_24_0 - _rtP -> P_57 ; _rtZCSV ->
RelationalOperator_RelopInput_ZC = _rtB -> B_3_30_0 - _rtB -> B_3_36_0 ;
_rtZCSV -> RelationalOperator1_RelopInput_ZC = _rtB -> B_3_39_0 - _rtB ->
B_3_40_0 ; _rtZCSV -> Switch_SwitchCond_ZC [ 0 ] = _rtB -> B_3_2_1 [ 0 ] -
_rtP -> P_66 ; _rtZCSV -> Saturation_UprLim_ZC_g [ 0 ] = _rtB -> B_3_46_0 [ 0
] - _rtP -> P_67 ; _rtZCSV -> Saturation_LwrLim_ZC_j [ 0 ] = _rtB -> B_3_46_0
[ 0 ] - _rtP -> P_68 ; _rtZCSV -> Switch_SwitchCond_ZC [ 1 ] = _rtB ->
B_3_2_1 [ 1 ] - _rtP -> P_66 ; _rtZCSV -> Saturation_UprLim_ZC_g [ 1 ] = _rtB
-> B_3_46_0 [ 1 ] - _rtP -> P_67 ; _rtZCSV -> Saturation_LwrLim_ZC_j [ 1 ] =
_rtB -> B_3_46_0 [ 1 ] - _rtP -> P_68 ; _rtZCSV -> Switch_SwitchCond_ZC [ 2 ]
= _rtB -> B_3_2_1 [ 2 ] - _rtP -> P_66 ; _rtZCSV -> Saturation_UprLim_ZC_g [
2 ] = _rtB -> B_3_46_0 [ 2 ] - _rtP -> P_67 ; _rtZCSV ->
Saturation_LwrLim_ZC_j [ 2 ] = _rtB -> B_3_46_0 [ 2 ] - _rtP -> P_68 ;
_rtZCSV -> Switch_SwitchCond_ZC [ 3 ] = _rtB -> B_3_2_1 [ 3 ] - _rtP -> P_66
; _rtZCSV -> Saturation_UprLim_ZC_g [ 3 ] = _rtB -> B_3_46_0 [ 3 ] - _rtP ->
P_67 ; _rtZCSV -> Saturation_LwrLim_ZC_j [ 3 ] = _rtB -> B_3_46_0 [ 3 ] -
_rtP -> P_68 ; _rtZCSV -> Abs_AbsZc_ZC = _rtB -> B_3_91_0 ; if ( _rtDW ->
AutomaticGainControl_MODE ) { _rtZCSV -> Toavoiddivisionbyzero_UprLim_ZC_l =
_rtB -> B_3_96_0 - _rtP -> P_2 ; _rtZCSV -> Toavoiddivisionbyzero_LwrLim_ZC_e
= _rtB -> B_3_96_0 - _rtP -> P_3 ; _rtZCSV -> Saturation_UprLim_ZC_p = _rtB
-> B_1_11_0 - _rtP -> P_10 ; _rtZCSV -> Saturation_LwrLim_ZC_j5 = _rtB ->
B_1_11_0 - _rtP -> P_11 ; } else { { real_T * zcsv = & ( ( (
ZCV_single_phase_PVarry_inverter_T * ) ssGetSolverZcSignalVector ( S ) ) ->
Toavoiddivisionbyzero_UprLim_ZC_l ) ; int_T i ; for ( i = 0 ; i < 4 ; i ++ )
{ zcsv [ i ] = 0.0 ; } } } _rtZCSV -> Toavoiddivisionbyzero_UprLim_ZC = _rtB
-> B_3_96_0 - _rtP -> P_94 ; _rtZCSV -> Toavoiddivisionbyzero_LwrLim_ZC =
_rtB -> B_3_96_0 - _rtP -> P_95 ; _rtZCSV -> Step_StepTime_ZC = ssGetT ( S )
- _rtP -> P_99 ; if ( ( _rtDW -> Integrator_MODE_l == 1 ) && ( _rtX ->
Integrator_CSTATE_h >= _rtP -> P_106 ) ) { _rtZCSV ->
Integrator_IntgUpLimit_ZC_d = 0.0 ; } else { _rtZCSV ->
Integrator_IntgUpLimit_ZC_d = _rtX -> Integrator_CSTATE_h - _rtP -> P_106 ; }
if ( ( _rtDW -> Integrator_MODE_l == 2 ) && ( _rtX -> Integrator_CSTATE_h <=
_rtP -> P_107 ) ) { _rtZCSV -> Integrator_IntgLoLimit_ZC_g = 0.0 ; } else {
_rtZCSV -> Integrator_IntgLoLimit_ZC_g = _rtX -> Integrator_CSTATE_h - _rtP
-> P_107 ; } anyStateSaturated = false ; if ( ( _rtDW -> Integrator_MODE_l ==
3 ) || ( _rtDW -> Integrator_MODE_l == 4 ) ) { anyStateSaturated = true ; }
if ( anyStateSaturated ) { _rtZCSV -> Integrator_LeaveSaturate_ZC_j = _rtB ->
B_3_118_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC_j = 0.0 ; }
_rtZCSV -> Saturation2_UprLim_ZC = _rtB -> B_3_115_0 - _rtP -> P_109 ;
_rtZCSV -> Saturation2_LwrLim_ZC = _rtB -> B_3_115_0 - _rtP -> P_110 ; }
static void mdlInitializeSizes ( SimStruct * S ) { ssSetChecksumVal ( S , 0 ,
2735379883U ) ; ssSetChecksumVal ( S , 1 , 1975621300U ) ; ssSetChecksumVal (
S , 2 , 3417197997U ) ; ssSetChecksumVal ( S , 3 , 3895525719U ) ; { mxArray
* slVerStructMat = NULL ; mxArray * slStrMat = mxCreateString ( "simulink" )
; char slVerChar [ 10 ] ; int status = mexCallMATLAB ( 1 , & slVerStructMat ,
1 , & slStrMat , "ver" ) ; if ( status == 0 ) { mxArray * slVerMat =
mxGetField ( slVerStructMat , 0 , "Version" ) ; if ( slVerMat == NULL ) {
status = 1 ; } else { status = mxGetString ( slVerMat , slVerChar , 10 ) ; }
} mxDestroyArray ( slStrMat ) ; mxDestroyArray ( slVerStructMat ) ; if ( (
status == 1 ) || ( strcmp ( slVerChar , "9.3" ) != 0 ) ) { return ; } }
ssSetOptions ( S , SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork (
S ) != sizeof ( DW_single_phase_PVarry_inverter_T ) ) { ssSetErrorStatus ( S
, "Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file." ) ; } if ( ssGetSizeofGlobalBlockIO ( S
) != sizeof ( B_single_phase_PVarry_inverter_T ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file." ) ; } { int ssSizeofParams ;
ssGetSizeofParams ( S , & ssSizeofParams ) ; if ( ssSizeofParams != sizeof (
P_single_phase_PVarry_inverter_T ) ) { static char msg [ 256 ] ; sprintf (
msg , "Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file." ) ; } } _ssSetModelRtp ( S , ( real_T *
) & single_phase_PVarry_inverter_rtDefaultP ) ; rt_InitInfAndNaN ( sizeof (
real_T ) ) ; ( ( P_single_phase_PVarry_inverter_T * ) ssGetModelRtp ( S ) )
-> P_47 = rtInf ; ( ( P_single_phase_PVarry_inverter_T * ) ssGetModelRtp ( S
) ) -> P_48 = rtMinusInf ; ( ( P_single_phase_PVarry_inverter_T * )
ssGetModelRtp ( S ) ) -> P_67 = rtInf ; } static void
mdlInitializeSampleTimes ( SimStruct * S ) { slAccRegPrmChangeFcn ( S ,
mdlOutputsTID4 ) ; } static void mdlTerminate ( SimStruct * S ) { }
#include "simulink.c"
