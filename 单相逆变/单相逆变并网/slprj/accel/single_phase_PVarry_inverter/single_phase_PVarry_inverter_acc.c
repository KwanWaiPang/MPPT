#include "__cf_single_phase_PVarry_inverter.h"
#include <math.h>
#include "single_phase_PVarry_inverter_acc.h"
#include "single_phase_PVarry_inverter_acc_private.h"
#include <stdio.h>
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
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
table [ iLeft ] ) * frac + table [ iLeft ] ; } static void mdlOutputs (
SimStruct * S , int_T tid ) { real_T ngnefchyvl ; real_T * lastU ; real_T
mi2toxz14i ; real_T hd3oex0alh ; real_T hisl2qgboq ; real_T kvhu00iagd ;
real_T acky4pacml ; real_T tmp ; n3qi1whofz * _rtB ; loikxjbxjg * _rtP ;
f1xhd02yjc * _rtX ; ew10rzwqr2 * _rtDW ; _rtDW = ( ( ew10rzwqr2 * )
ssGetRootDWork ( S ) ) ; _rtX = ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) ;
_rtP = ( ( loikxjbxjg * ) ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz *
) _ssGetBlockIO ( S ) ) ; if ( ssIsMajorTimeStep ( S ) ) { if ( ( (
f1xhd02yjc * ) ssGetContStates ( S ) ) -> cv44ycfzzj >= _rtP -> P_35 ) { ( (
f1xhd02yjc * ) ssGetContStates ( S ) ) -> cv44ycfzzj = _rtP -> P_35 ; } else
if ( ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) -> cv44ycfzzj <= _rtP -> P_36
) { ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) -> cv44ycfzzj = _rtP -> P_36 ;
} } _rtB -> gxi3zba2ap = ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) ->
cv44ycfzzj ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> piwy2agrh4 = _rtP
-> P_37 ; } mi2toxz14i = muDoubleScalarMod ( _rtB -> gxi3zba2ap , _rtB ->
piwy2agrh4 ) ; hd3oex0alh = muDoubleScalarSin ( mi2toxz14i ) ; { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> laahqtrsqb . TUbufferPtrs [ 0 ] ; real_T
* * tBuffer = ( real_T * * ) & _rtDW -> laahqtrsqb . TUbufferPtrs [ 1 ] ;
real_T simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_38 ;
_rtB -> nb2eeonhnh = single_phase_PVarry_inverter_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * tBuffer , * uBuffer , _rtDW -> dw1w2swcy5 .
CircularBufSize , & _rtDW -> dw1w2swcy5 . Last , _rtDW -> dw1w2swcy5 . Tail ,
_rtDW -> dw1w2swcy5 . Head , _rtP -> P_39 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> i2w2tzgiel = _rtP -> P_40
; } ssCallAccelRunBlock ( S , 4 , 7 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
evdmxwywtd = hd3oex0alh * _rtB -> gj1pwxushz ; _rtB -> lwbninpbvl = ( (
f1xhd02yjc * ) ssGetContStates ( S ) ) -> c2mfgg3wdj ; ngnefchyvl = ssGetT (
S ) ; _rtB -> huke35jpys = ssGetTStart ( S ) ; ngnefchyvl -= _rtB ->
huke35jpys ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> icmsz4y3ku = _rtP
-> P_42 ; } ngnefchyvl = muDoubleScalarRem ( ngnefchyvl , _rtB -> icmsz4y3ku
) ; _rtB -> bd20yvnrev = look1_binlxpw ( ngnefchyvl , _rtP -> P_44 , _rtP ->
P_43 , 2U ) ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> b5q31uocxe = _rtP
-> P_45 ; _rtB -> ptv0ouzymz = _rtP -> P_107 ; _rtB -> fcve20cmcq = _rtP ->
P_46 ; } _rtB -> ofsd2xvpf0 = muDoubleScalarSin ( _rtP -> P_49 *
ssGetTaskTime ( S , 0 ) + _rtP -> P_50 ) * _rtP -> P_47 + _rtP -> P_48 ;
ssCallAccelRunBlock ( S , 4 , 20 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> hpivsd2rhn
= _rtP -> P_51 * _rtB -> mwcvakbrfp [ 4 ] ; _rtB -> cki3ccfuch = _rtP -> P_52
* _rtB -> mwcvakbrfp [ 5 ] ; ssCallAccelRunBlock ( S , 4 , 23 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> dd40z5v0og = _rtP -> P_53 * _rtB ->
h03cs5sido ; ssCallAccelRunBlock ( S , 4 , 25 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 4 , 26 , SS_CALL_MDL_OUTPUTS ) ; if ( ssIsSampleHit
( S , 1 , 0 ) ) { _rtB -> chqqllh1qm = _rtP -> P_54 ; } _rtB -> jeayr5kc0m [
0 ] = _rtP -> P_55 * _rtB -> mwcvakbrfp [ 0 ] ; _rtB -> jeayr5kc0m [ 1 ] =
_rtP -> P_55 * _rtB -> mwcvakbrfp [ 1 ] ; _rtB -> jeayr5kc0m [ 2 ] = _rtP ->
P_55 * _rtB -> mwcvakbrfp [ 2 ] ; _rtB -> jeayr5kc0m [ 3 ] = _rtP -> P_55 *
_rtB -> mwcvakbrfp [ 3 ] ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB ->
hntfvdb55q [ 0 ] = _rtB -> cffmmjn3oz ; _rtB -> hntfvdb55q [ 1 ] = _rtB ->
mqf1wtuyhr ; _rtB -> hntfvdb55q [ 2 ] = _rtB -> ix5rvakikk ; _rtB ->
hntfvdb55q [ 3 ] = _rtB -> djmglmgszo ; } if ( ssIsMajorTimeStep ( S ) ) {
_rtDW -> i0b0rlrm1m [ 0 ] = ( _rtB -> cqhwdxdbgp [ 0 ] >= _rtP -> P_56 ) ;
_rtDW -> i0b0rlrm1m [ 1 ] = ( _rtB -> cqhwdxdbgp [ 1 ] >= _rtP -> P_56 ) ;
_rtDW -> i0b0rlrm1m [ 2 ] = ( _rtB -> cqhwdxdbgp [ 2 ] >= _rtP -> P_56 ) ;
_rtDW -> i0b0rlrm1m [ 3 ] = ( _rtB -> cqhwdxdbgp [ 3 ] >= _rtP -> P_56 ) ; }
if ( _rtDW -> i0b0rlrm1m [ 0 ] ) { _rtB -> c03rfbw3dp [ 0 ] = _rtB ->
jeayr5kc0m [ 0 ] ; } else { _rtB -> c03rfbw3dp [ 0 ] = _rtB -> chqqllh1qm ; }
if ( _rtDW -> i0b0rlrm1m [ 1 ] ) { _rtB -> c03rfbw3dp [ 1 ] = _rtB ->
jeayr5kc0m [ 1 ] ; } else { _rtB -> c03rfbw3dp [ 1 ] = _rtB -> chqqllh1qm ; }
if ( _rtDW -> i0b0rlrm1m [ 2 ] ) { _rtB -> c03rfbw3dp [ 2 ] = _rtB ->
jeayr5kc0m [ 2 ] ; } else { _rtB -> c03rfbw3dp [ 2 ] = _rtB -> chqqllh1qm ; }
if ( _rtDW -> i0b0rlrm1m [ 3 ] ) { _rtB -> c03rfbw3dp [ 3 ] = _rtB ->
jeayr5kc0m [ 3 ] ; } else { _rtB -> c03rfbw3dp [ 3 ] = _rtB -> chqqllh1qm ; }
if ( ssIsMajorTimeStep ( S ) ) { _rtDW -> nqghvcnn30 [ 0 ] = _rtB ->
c03rfbw3dp [ 0 ] >= _rtP -> P_57 ? 1 : _rtB -> c03rfbw3dp [ 0 ] > _rtP ->
P_58 ? 0 : - 1 ; _rtDW -> nqghvcnn30 [ 1 ] = _rtB -> c03rfbw3dp [ 1 ] >= _rtP
-> P_57 ? 1 : _rtB -> c03rfbw3dp [ 1 ] > _rtP -> P_58 ? 0 : - 1 ; _rtDW ->
nqghvcnn30 [ 2 ] = _rtB -> c03rfbw3dp [ 2 ] >= _rtP -> P_57 ? 1 : _rtB ->
c03rfbw3dp [ 2 ] > _rtP -> P_58 ? 0 : - 1 ; _rtDW -> nqghvcnn30 [ 3 ] = _rtB
-> c03rfbw3dp [ 3 ] >= _rtP -> P_57 ? 1 : _rtB -> c03rfbw3dp [ 3 ] > _rtP ->
P_58 ? 0 : - 1 ; } _rtB -> d0savt30lv [ 0 ] = _rtDW -> nqghvcnn30 [ 0 ] == 1
? _rtP -> P_57 : _rtDW -> nqghvcnn30 [ 0 ] == - 1 ? _rtP -> P_58 : _rtB ->
c03rfbw3dp [ 0 ] ; _rtB -> d0savt30lv [ 1 ] = _rtDW -> nqghvcnn30 [ 1 ] == 1
? _rtP -> P_57 : _rtDW -> nqghvcnn30 [ 1 ] == - 1 ? _rtP -> P_58 : _rtB ->
c03rfbw3dp [ 1 ] ; _rtB -> d0savt30lv [ 2 ] = _rtDW -> nqghvcnn30 [ 2 ] == 1
? _rtP -> P_57 : _rtDW -> nqghvcnn30 [ 2 ] == - 1 ? _rtP -> P_58 : _rtB ->
c03rfbw3dp [ 2 ] ; _rtB -> d0savt30lv [ 3 ] = _rtDW -> nqghvcnn30 [ 3 ] == 1
? _rtP -> P_57 : _rtDW -> nqghvcnn30 [ 3 ] == - 1 ? _rtP -> P_58 : _rtB ->
c03rfbw3dp [ 3 ] ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> l32mqyi5gz =
_rtP -> P_59 - _rtP -> P_61 ; acky4pacml = _rtP -> P_60 / _rtP -> P_62 - _rtP
-> P_63 ; _rtB -> boow32mcms = _rtP -> P_65 ; _rtB -> cbknislkuh = - _rtP ->
P_66 * _rtB -> l32mqyi5gz - ( _rtB -> l32mqyi5gz * _rtP -> P_64 * acky4pacml
+ acky4pacml * _rtB -> boow32mcms ) * _rtP -> P_67 ; acky4pacml = _rtP ->
P_69 - 1.0 / _rtB -> boow32mcms * _rtP -> P_68 ; kvhu00iagd = _rtP -> P_70 ;
_rtB -> akg01ramzh = _rtP -> P_71 ; hisl2qgboq = _rtP -> P_70 / _rtB ->
akg01ramzh - _rtP -> P_72 ; tmp = acky4pacml ; if ( ssIsMajorTimeStep ( S ) )
{ if ( _rtDW -> gld2jefi5o != 0 ) { ssSetSolverNeedsReset ( S ) ; _rtDW ->
gld2jefi5o = 0 ; } } else { if ( acky4pacml < 0.0 ) { tmp = 0.0 ; _rtDW ->
gld2jefi5o = 1 ; } } _rtB -> o2z1axfuiw = hisl2qgboq / muDoubleScalarLog (
tmp ) ; _rtB -> bkkrzw3dpa = muDoubleScalarExp ( kvhu00iagd / _rtB ->
o2z1axfuiw / _rtB -> akg01ramzh * _rtP -> P_73 ) * acky4pacml ; }
ssCallAccelRunBlock ( S , 4 , 71 , SS_CALL_MDL_OUTPUTS ) ; if ( ssIsSampleHit
( S , 1 , 0 ) ) { _rtB -> le0wqsuurd = _rtB -> g32fappqpj * _rtB ->
fmxufdyga1 ; } _rtB -> ojki23d4em = ( ( f1xhd02yjc * ) ssGetContStates ( S )
) -> d24x1wviqz ; { real_T * * uBuffer = ( real_T * * ) & _rtDW -> lvful1kguw
. TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
lvful1kguw . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; real_T
tMinusDelay = simTime - _rtP -> P_75 ; _rtB -> g0o32g0k5m =
single_phase_PVarry_inverter_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , _rtDW -> aq5jg3ks0r . CircularBufSize , & _rtDW ->
aq5jg3ks0r . Last , _rtDW -> aq5jg3ks0r . Tail , _rtDW -> aq5jg3ks0r . Head ,
_rtP -> P_76 , 0 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && (
ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) ) ) ; } _rtB -> j4d4cpsnpe =
_rtB -> ojki23d4em - _rtB -> g0o32g0k5m ; if ( ssIsMajorTimeStep ( S ) ) {
_rtDW -> gpmreqszwf = _rtB -> j4d4cpsnpe >= 0.0 ? 1 : 0 ; } _rtB ->
fesgcj0rl0 = _rtDW -> gpmreqszwf > 0 ? _rtB -> j4d4cpsnpe : - _rtB ->
j4d4cpsnpe ; if ( ssIsMajorTimeStep ( S ) ) { if ( _rtDW -> bp43bxk2t5 != 0 )
{ ssSetSolverNeedsReset ( S ) ; _rtDW -> bp43bxk2t5 = 0 ; } ngnefchyvl =
muDoubleScalarSqrt ( _rtB -> fesgcj0rl0 ) ; } else { if ( _rtB -> fesgcj0rl0
< 0.0 ) { ngnefchyvl = - muDoubleScalarSqrt ( muDoubleScalarAbs ( _rtB ->
fesgcj0rl0 ) ) ; } else { ngnefchyvl = muDoubleScalarSqrt ( _rtB ->
fesgcj0rl0 ) ; } _rtDW -> bp43bxk2t5 = 1 ; } _rtB -> h304fuqqa2 = _rtB ->
le0wqsuurd / ngnefchyvl ; _rtB -> hgjsxcwqy0 = _rtP -> P_77 * _rtB ->
flvxwn5v0t ; _rtB -> gyf5irieym = 0.0 ; _rtB -> gyf5irieym += _rtP -> P_80 *
_rtX -> mffbzrtcri [ 1 ] ; if ( ssIsSampleHit ( S , 3 , 0 ) ) { _rtB ->
p2d2xot5lb = _rtP -> P_82 * _rtB -> hpivsd2rhn ; } acky4pacml =
muDoubleScalarCos ( mi2toxz14i ) ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB
-> bomg4mg43n = _rtP -> P_83 ; if ( ssIsMajorTimeStep ( S ) ) { if ( _rtB ->
bomg4mg43n > 0.0 ) { if ( ! _rtDW -> hs0pegbvhq ) { ( void ) memset ( & ( ( (
biadadh31q * ) ssGetContStateDisabled ( S ) ) -> ovbrbhosi4 ) , 0 , 2 *
sizeof ( boolean_T ) ) ; if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) )
{ ssSetSolverNeedsReset ( S ) ; } _rtDW -> hs0pegbvhq = true ; } } else { if
( _rtDW -> hs0pegbvhq ) { ssSetSolverNeedsReset ( S ) ; ( void ) memset ( & (
( ( biadadh31q * ) ssGetContStateDisabled ( S ) ) -> ovbrbhosi4 ) , 1 , 2 *
sizeof ( boolean_T ) ) ; _rtDW -> hs0pegbvhq = false ; } } } } if ( _rtDW ->
hs0pegbvhq ) { _rtB -> ipqtlsgaxc = ( ( f1xhd02yjc * ) ssGetContStates ( S )
) -> ovbrbhosi4 ; if ( ssIsMajorTimeStep ( S ) ) { _rtDW -> lc3ylxguhk = _rtB
-> gyf5irieym >= _rtP -> P_23 ? 1 : _rtB -> gyf5irieym > _rtP -> P_24 ? 0 : -
1 ; } _rtB -> glp3f55wsy = _rtDW -> lc3ylxguhk == 1 ? _rtP -> P_23 : _rtDW ->
lc3ylxguhk == - 1 ? _rtP -> P_24 : _rtB -> gyf5irieym ; _rtB -> crh214xkau =
1.0 / _rtB -> glp3f55wsy ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
n4spsamp2o . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW
-> n4spsamp2o . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; real_T
appliedDelay ; appliedDelay = _rtB -> crh214xkau ; if ( appliedDelay > _rtP
-> P_25 ) { appliedDelay = _rtP -> P_25 ; } if ( appliedDelay < 0.0 ) {
appliedDelay = 0.0 ; } _rtB -> gxdj1b521u =
single_phase_PVarry_inverter_acc_rt_TDelayInterpolate ( simTime -
appliedDelay , 0.0 , * tBuffer , * uBuffer , _rtDW -> jdnfyd31tn .
CircularBufSize , & _rtDW -> jdnfyd31tn . Last , _rtDW -> jdnfyd31tn . Tail ,
_rtDW -> jdnfyd31tn . Head , _rtP -> P_26 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } _rtB -> dtqcmgzruj = _rtB -> ipqtlsgaxc - _rtB -> gxdj1b521u ; tmp =
muDoubleScalarPower ( _rtB -> dtqcmgzruj , 2.0 ) ; _rtB -> diekjodaio = ( (
f1xhd02yjc * ) ssGetContStates ( S ) ) -> ddqv420uug ; { real_T * * uBuffer =
( real_T * * ) & _rtDW -> jy5ke02sly . TUbufferPtrs [ 0 ] ; real_T * *
tBuffer = ( real_T * * ) & _rtDW -> jy5ke02sly . TUbufferPtrs [ 1 ] ; real_T
simTime = ssGetT ( S ) ; real_T appliedDelay ; appliedDelay = _rtB ->
crh214xkau ; if ( appliedDelay > _rtP -> P_28 ) { appliedDelay = _rtP -> P_28
; } if ( appliedDelay < 0.0 ) { appliedDelay = 0.0 ; } _rtB -> o1epvrdtjx =
single_phase_PVarry_inverter_acc_rt_TDelayInterpolate ( simTime -
appliedDelay , 0.0 , * tBuffer , * uBuffer , _rtDW -> gw3hjgnfvl .
CircularBufSize , & _rtDW -> gw3hjgnfvl . Last , _rtDW -> gw3hjgnfvl . Tail ,
_rtDW -> gw3hjgnfvl . Head , _rtP -> P_29 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } _rtB -> akvu3w51gt = _rtB -> diekjodaio - _rtB -> o1epvrdtjx ; _rtB ->
atcxojpxyy = tmp + muDoubleScalarPower ( _rtB -> akvu3w51gt , 2.0 ) ; if (
_rtB -> atcxojpxyy < 0.0 ) { _rtB -> ifw02obj0c = - muDoubleScalarSqrt ( -
_rtB -> atcxojpxyy ) ; } else { _rtB -> ifw02obj0c = muDoubleScalarSqrt (
_rtB -> atcxojpxyy ) ; } _rtB -> fefljzptia = _rtP -> P_30 * _rtB ->
gyf5irieym ; if ( ssIsMajorTimeStep ( S ) ) { _rtDW -> cpkwdqkqgc = _rtB ->
ifw02obj0c >= _rtP -> P_31 ? 1 : _rtB -> ifw02obj0c > _rtP -> P_32 ? 0 : - 1
; } _rtB -> pymfot4dei = _rtDW -> cpkwdqkqgc == 1 ? _rtP -> P_31 : _rtDW ->
cpkwdqkqgc == - 1 ? _rtP -> P_32 : _rtB -> ifw02obj0c ; _rtB -> bhlz3tirhc =
1.0 / _rtB -> pymfot4dei ; _rtB -> itetzdhzcd = _rtB -> p2d2xot5lb * _rtB ->
fefljzptia ; _rtB -> nxf1wendip = hd3oex0alh * _rtB -> itetzdhzcd ; _rtB ->
nb5dbps2gt = _rtB -> itetzdhzcd * acky4pacml ; if ( ssIsMajorTimeStep ( S ) )
{ srUpdateBC ( _rtDW -> f4ewkmdtny ) ; } } if ( ssIsMajorTimeStep ( S ) ) {
_rtDW -> gcc5n5ymek = _rtB -> gyf5irieym >= _rtP -> P_84 ? 1 : _rtB ->
gyf5irieym > _rtP -> P_85 ? 0 : - 1 ; } _rtB -> lxaq5qset0 = _rtDW ->
gcc5n5ymek == 1 ? _rtP -> P_84 : _rtDW -> gcc5n5ymek == - 1 ? _rtP -> P_85 :
_rtB -> gyf5irieym ; _rtB -> kivvrltf3v = 1.0 / _rtB -> lxaq5qset0 ; { real_T
* * uBuffer = ( real_T * * ) & _rtDW -> etgqnmnh3j . TUbufferPtrs [ 0 ] ;
real_T * * tBuffer = ( real_T * * ) & _rtDW -> etgqnmnh3j . TUbufferPtrs [ 1
] ; real_T simTime = ssGetT ( S ) ; real_T appliedDelay ; appliedDelay = _rtB
-> kivvrltf3v ; if ( appliedDelay > _rtP -> P_86 ) { appliedDelay = _rtP ->
P_86 ; } if ( appliedDelay < 0.0 ) { appliedDelay = 0.0 ; } _rtB ->
gsiwbvpppb = single_phase_PVarry_inverter_acc_rt_TDelayInterpolate ( simTime
- appliedDelay , 0.0 , * tBuffer , * uBuffer , _rtDW -> okhhuxnhin .
CircularBufSize , & _rtDW -> okhhuxnhin . Last , _rtDW -> okhhuxnhin . Tail ,
_rtDW -> okhhuxnhin . Head , _rtP -> P_87 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } _rtB -> j5jxti4ogc = ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) ->
kgsuae1o42 ; _rtB -> gbc24nmzvt = _rtB -> j5jxti4ogc - _rtB -> gsiwbvpppb ;
_rtB -> n3xmbpaw2f = _rtB -> gbc24nmzvt * _rtB -> gyf5irieym ; if (
ssIsSampleHit ( S , 1 , 0 ) ) { if ( ssIsMajorTimeStep ( S ) ) { _rtDW ->
o4zjk5u2sk = ( ssGetTaskTime ( S , 1 ) >= _rtP -> P_89 ) ; } if ( _rtDW ->
o4zjk5u2sk == 1 ) { _rtB -> gpwy33o5ms = _rtP -> P_91 ; } else { _rtB ->
gpwy33o5ms = _rtP -> P_90 ; } _rtB -> gtcewi1em0 = _rtP -> P_92 ; } if ( _rtB
-> gpwy33o5ms >= _rtP -> P_93 ) { _rtB -> as4uiavkp5 = _rtB -> n3xmbpaw2f ; }
else { _rtB -> as4uiavkp5 = _rtB -> gtcewi1em0 ; } _rtB -> ng2rpx1ks2 = _rtB
-> as4uiavkp5 * _rtB -> bhlz3tirhc ; _rtB -> fgxuklxk2y = _rtP -> P_94 * _rtB
-> ng2rpx1ks2 ; if ( ssIsMajorTimeStep ( S ) ) { if ( ( ( f1xhd02yjc * )
ssGetContStates ( S ) ) -> ac03fuu2hs >= _rtP -> P_96 ) { ( ( f1xhd02yjc * )
ssGetContStates ( S ) ) -> ac03fuu2hs = _rtP -> P_96 ; } else if ( ( (
f1xhd02yjc * ) ssGetContStates ( S ) ) -> ac03fuu2hs <= _rtP -> P_97 ) { ( (
f1xhd02yjc * ) ssGetContStates ( S ) ) -> ac03fuu2hs = _rtP -> P_97 ; } }
_rtB -> a4cjuicn31 = ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) -> ac03fuu2hs
; _rtB -> f4uige3ylp = _rtP -> P_98 * _rtB -> ng2rpx1ks2 ; if ( ( _rtDW ->
hjntozqqrx >= ssGetT ( S ) ) && ( _rtDW -> l5mtobqkp1 >= ssGetT ( S ) ) ) {
_rtB -> k43d2lhzhi = 0.0 ; } else { mi2toxz14i = _rtDW -> hjntozqqrx ; lastU
= & _rtDW -> nkxs5x1rjh ; if ( _rtDW -> hjntozqqrx < _rtDW -> l5mtobqkp1 ) {
if ( _rtDW -> l5mtobqkp1 < ssGetT ( S ) ) { mi2toxz14i = _rtDW -> l5mtobqkp1
; lastU = & _rtDW -> fwsgi4qud5 ; } } else { if ( _rtDW -> hjntozqqrx >=
ssGetT ( S ) ) { mi2toxz14i = _rtDW -> l5mtobqkp1 ; lastU = & _rtDW ->
fwsgi4qud5 ; } } _rtB -> k43d2lhzhi = ( _rtB -> f4uige3ylp - * lastU ) / (
ssGetT ( S ) - mi2toxz14i ) ; } _rtB -> ei4cwq53sd = ( _rtB -> fgxuklxk2y +
_rtB -> a4cjuicn31 ) + _rtB -> k43d2lhzhi ; if ( ssIsMajorTimeStep ( S ) ) {
_rtDW -> a55rpjmrxu = _rtB -> ei4cwq53sd >= _rtP -> P_99 ? 1 : _rtB ->
ei4cwq53sd > _rtP -> P_100 ? 0 : - 1 ; } _rtB -> dkezgv5rpr = _rtDW ->
a55rpjmrxu == 1 ? _rtP -> P_99 : _rtDW -> a55rpjmrxu == - 1 ? _rtP -> P_100 :
_rtB -> ei4cwq53sd ; _rtB -> o2fbmdjq5j = _rtP -> P_101 * _rtB -> dkezgv5rpr
; _rtB -> g5nlxuwomm = _rtP -> P_102 * _rtB -> ng2rpx1ks2 ; _rtB ->
hh3v5qyxgu = _rtB -> p2d2xot5lb * acky4pacml ; if ( ( _rtDW -> ggqb0oluyw >=
ssGetTaskTime ( S , 0 ) ) && ( _rtDW -> efwbzmlb2g >= ssGetTaskTime ( S , 0 )
) ) { _rtB -> pegtjdjblx = _rtB -> o2fbmdjq5j ; } else { if ( ( ( _rtDW ->
ggqb0oluyw < _rtDW -> efwbzmlb2g ) && ( _rtDW -> efwbzmlb2g < ssGetTaskTime (
S , 0 ) ) ) || ( ( _rtDW -> ggqb0oluyw >= _rtDW -> efwbzmlb2g ) && ( _rtDW ->
ggqb0oluyw >= ssGetTaskTime ( S , 0 ) ) ) ) { mi2toxz14i = ssGetTaskTime ( S
, 0 ) - _rtDW -> efwbzmlb2g ; hisl2qgboq = _rtDW -> nmggmye1jy ; } else {
mi2toxz14i = ssGetTaskTime ( S , 0 ) - _rtDW -> ggqb0oluyw ; hisl2qgboq =
_rtDW -> ns44sv4mq2 ; } acky4pacml = mi2toxz14i * _rtP -> P_103 ; kvhu00iagd
= _rtB -> o2fbmdjq5j - hisl2qgboq ; if ( kvhu00iagd > acky4pacml ) { _rtB ->
pegtjdjblx = hisl2qgboq + acky4pacml ; } else { mi2toxz14i *= _rtP -> P_104 ;
if ( kvhu00iagd < mi2toxz14i ) { _rtB -> pegtjdjblx = hisl2qgboq + mi2toxz14i
; } else { _rtB -> pegtjdjblx = _rtB -> o2fbmdjq5j ; } } } if ( ssIsSampleHit
( S , 1 , 0 ) ) { ssCallAccelRunBlock ( S , 4 , 106 , SS_CALL_MDL_OUTPUTS ) ;
} _rtB -> myc4w512hq = _rtB -> hpivsd2rhn * _rtB -> hpivsd2rhn * _rtP ->
P_105 ; UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { real_T * lastU ;
n3qi1whofz * _rtB ; loikxjbxjg * _rtP ; ew10rzwqr2 * _rtDW ; _rtDW = ( (
ew10rzwqr2 * ) ssGetRootDWork ( S ) ) ; _rtP = ( ( loikxjbxjg * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ;
if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtDW -> oir3mkgkur [ 0 ] = _rtB ->
d0savt30lv [ 0 ] ; _rtDW -> oir3mkgkur [ 1 ] = _rtB -> d0savt30lv [ 1 ] ;
_rtDW -> oir3mkgkur [ 2 ] = _rtB -> d0savt30lv [ 2 ] ; _rtDW -> oir3mkgkur [
3 ] = _rtB -> d0savt30lv [ 3 ] ; } { enum { INTG_NORMAL ,
INTG_LEAVING_UPPER_SAT , INTG_LEAVING_LOWER_SAT , INTG_UPPER_SAT ,
INTG_LOWER_SAT } ; if ( ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) ->
cv44ycfzzj == _rtP -> P_35 ) { switch ( _rtDW -> peomhw51tw ) { case
INTG_UPPER_SAT : if ( _rtB -> dkezgv5rpr < 0.0 ) { ssSetSolverNeedsReset ( S
) ; _rtDW -> peomhw51tw = INTG_LEAVING_UPPER_SAT ; } break ; case
INTG_LEAVING_UPPER_SAT : if ( _rtB -> dkezgv5rpr >= 0.0 ) { _rtDW ->
peomhw51tw = INTG_UPPER_SAT ; ssSetSolverNeedsReset ( S ) ; } break ; default
: ssSetSolverNeedsReset ( S ) ; if ( _rtB -> dkezgv5rpr < 0.0 ) { _rtDW ->
peomhw51tw = INTG_LEAVING_UPPER_SAT ; } else { _rtDW -> peomhw51tw =
INTG_UPPER_SAT ; } break ; } } else if ( ( ( f1xhd02yjc * ) ssGetContStates (
S ) ) -> cv44ycfzzj == _rtP -> P_36 ) { switch ( _rtDW -> peomhw51tw ) { case
INTG_LOWER_SAT : if ( _rtB -> dkezgv5rpr > 0.0 ) { ssSetSolverNeedsReset ( S
) ; _rtDW -> peomhw51tw = INTG_LEAVING_LOWER_SAT ; } break ; case
INTG_LEAVING_LOWER_SAT : if ( _rtB -> dkezgv5rpr <= 0.0 ) { _rtDW ->
peomhw51tw = INTG_LOWER_SAT ; ssSetSolverNeedsReset ( S ) ; } break ; default
: ssSetSolverNeedsReset ( S ) ; if ( _rtB -> dkezgv5rpr > 0.0 ) { _rtDW ->
peomhw51tw = INTG_LEAVING_LOWER_SAT ; } else { _rtDW -> peomhw51tw =
INTG_LOWER_SAT ; } break ; } } else { _rtDW -> peomhw51tw = INTG_NORMAL ; } }
{ real_T * * uBuffer = ( real_T * * ) & _rtDW -> laahqtrsqb . TUbufferPtrs [
0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> laahqtrsqb .
TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW -> dw1w2swcy5 .
Head = ( ( _rtDW -> dw1w2swcy5 . Head < ( _rtDW -> dw1w2swcy5 .
CircularBufSize - 1 ) ) ? ( _rtDW -> dw1w2swcy5 . Head + 1 ) : 0 ) ; if (
_rtDW -> dw1w2swcy5 . Head == _rtDW -> dw1w2swcy5 . Tail ) { if ( !
single_phase_PVarry_inverter_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
dw1w2swcy5 . CircularBufSize , & _rtDW -> dw1w2swcy5 . Tail , & _rtDW ->
dw1w2swcy5 . Head , & _rtDW -> dw1w2swcy5 . Last , simTime - _rtP -> P_38 ,
tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW ->
dw1w2swcy5 . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
dw1w2swcy5 . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> dw1w2swcy5 . Head ]
= _rtB -> h304fuqqa2 ; } if ( _rtDW -> ivzxketgv1 == ( rtInf ) ) { _rtDW ->
ivzxketgv1 = ssGetT ( S ) ; lastU = & _rtDW -> o1ezyn5fzo ; } else if ( _rtDW
-> a0chmotcqw == ( rtInf ) ) { _rtDW -> a0chmotcqw = ssGetT ( S ) ; lastU = &
_rtDW -> ltqpnqteqg ; } else if ( _rtDW -> ivzxketgv1 < _rtDW -> a0chmotcqw )
{ _rtDW -> ivzxketgv1 = ssGetT ( S ) ; lastU = & _rtDW -> o1ezyn5fzo ; } else
{ _rtDW -> a0chmotcqw = ssGetT ( S ) ; lastU = & _rtDW -> ltqpnqteqg ; } *
lastU = _rtB -> ik2qo5lukj ; if ( _rtDW -> dy5lox0tpq ) { if ( ssIsSampleHit
( S , 1 , 0 ) ) { _rtDW -> l0mmnsxs5a [ 0 ] = _rtB -> h2bih5ika5 [ 0 ] ;
_rtDW -> l0mmnsxs5a [ 1 ] = _rtB -> h2bih5ika5 [ 1 ] ; _rtDW -> l0mmnsxs5a [
2 ] = _rtB -> h2bih5ika5 [ 2 ] ; _rtDW -> l0mmnsxs5a [ 3 ] = _rtB ->
h2bih5ika5 [ 3 ] ; } } ssCallAccelRunBlock ( S , 2 , 11 , SS_CALL_MDL_UPDATE
) ; if ( ssIsSampleHit ( S , 2 , 0 ) ) { ssCallAccelRunBlock ( S , 0 , 0 ,
SS_CALL_MDL_UPDATE ) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
lvful1kguw . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW
-> lvful1kguw . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
aq5jg3ks0r . Head = ( ( _rtDW -> aq5jg3ks0r . Head < ( _rtDW -> aq5jg3ks0r .
CircularBufSize - 1 ) ) ? ( _rtDW -> aq5jg3ks0r . Head + 1 ) : 0 ) ; if (
_rtDW -> aq5jg3ks0r . Head == _rtDW -> aq5jg3ks0r . Tail ) { if ( !
single_phase_PVarry_inverter_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
aq5jg3ks0r . CircularBufSize , & _rtDW -> aq5jg3ks0r . Tail , & _rtDW ->
aq5jg3ks0r . Head , & _rtDW -> aq5jg3ks0r . Last , simTime - _rtP -> P_75 ,
tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW ->
aq5jg3ks0r . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
aq5jg3ks0r . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> aq5jg3ks0r . Head ]
= _rtB -> ojki23d4em ; } if ( _rtDW -> hs0pegbvhq ) { { real_T * * uBuffer =
( real_T * * ) & _rtDW -> n4spsamp2o . TUbufferPtrs [ 0 ] ; real_T * *
tBuffer = ( real_T * * ) & _rtDW -> n4spsamp2o . TUbufferPtrs [ 1 ] ; real_T
* * xBuffer = ( NULL ) ; real_T simTime = ssGetT ( S ) ; real_T appliedDelay
; appliedDelay = _rtB -> crh214xkau ; if ( appliedDelay > _rtP -> P_25 ) {
appliedDelay = _rtP -> P_25 ; } _rtDW -> jdnfyd31tn . Head = ( ( _rtDW ->
jdnfyd31tn . Head < ( _rtDW -> jdnfyd31tn . CircularBufSize - 1 ) ) ? ( _rtDW
-> jdnfyd31tn . Head + 1 ) : 0 ) ; if ( _rtDW -> jdnfyd31tn . Head == _rtDW
-> jdnfyd31tn . Tail ) { if ( !
single_phase_PVarry_inverter_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
jdnfyd31tn . CircularBufSize , & _rtDW -> jdnfyd31tn . Tail , & _rtDW ->
jdnfyd31tn . Head , & _rtDW -> jdnfyd31tn . Last , simTime - _rtP -> P_25 ,
tBuffer , uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 0 , & _rtDW ->
jdnfyd31tn . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
jdnfyd31tn . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> jdnfyd31tn . Head ]
= _rtB -> ipqtlsgaxc ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
jy5ke02sly . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW
-> jy5ke02sly . TUbufferPtrs [ 1 ] ; real_T * * xBuffer = ( NULL ) ; real_T
simTime = ssGetT ( S ) ; real_T appliedDelay ; appliedDelay = _rtB ->
crh214xkau ; if ( appliedDelay > _rtP -> P_28 ) { appliedDelay = _rtP -> P_28
; } _rtDW -> gw3hjgnfvl . Head = ( ( _rtDW -> gw3hjgnfvl . Head < ( _rtDW ->
gw3hjgnfvl . CircularBufSize - 1 ) ) ? ( _rtDW -> gw3hjgnfvl . Head + 1 ) : 0
) ; if ( _rtDW -> gw3hjgnfvl . Head == _rtDW -> gw3hjgnfvl . Tail ) { if ( !
single_phase_PVarry_inverter_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
gw3hjgnfvl . CircularBufSize , & _rtDW -> gw3hjgnfvl . Tail , & _rtDW ->
gw3hjgnfvl . Head , & _rtDW -> gw3hjgnfvl . Last , simTime - _rtP -> P_28 ,
tBuffer , uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 0 , & _rtDW ->
gw3hjgnfvl . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
gw3hjgnfvl . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> gw3hjgnfvl . Head ]
= _rtB -> diekjodaio ; } } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
etgqnmnh3j . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW
-> etgqnmnh3j . TUbufferPtrs [ 1 ] ; real_T * * xBuffer = ( NULL ) ; real_T
simTime = ssGetT ( S ) ; real_T appliedDelay ; appliedDelay = _rtB ->
kivvrltf3v ; if ( appliedDelay > _rtP -> P_86 ) { appliedDelay = _rtP -> P_86
; } _rtDW -> okhhuxnhin . Head = ( ( _rtDW -> okhhuxnhin . Head < ( _rtDW ->
okhhuxnhin . CircularBufSize - 1 ) ) ? ( _rtDW -> okhhuxnhin . Head + 1 ) : 0
) ; if ( _rtDW -> okhhuxnhin . Head == _rtDW -> okhhuxnhin . Tail ) { if ( !
single_phase_PVarry_inverter_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
okhhuxnhin . CircularBufSize , & _rtDW -> okhhuxnhin . Tail , & _rtDW ->
okhhuxnhin . Head , & _rtDW -> okhhuxnhin . Last , simTime - _rtP -> P_86 ,
tBuffer , uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 0 , & _rtDW ->
okhhuxnhin . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
okhhuxnhin . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> okhhuxnhin . Head ]
= _rtB -> j5jxti4ogc ; } { enum { INTG_NORMAL , INTG_LEAVING_UPPER_SAT ,
INTG_LEAVING_LOWER_SAT , INTG_UPPER_SAT , INTG_LOWER_SAT } ; if ( ( (
f1xhd02yjc * ) ssGetContStates ( S ) ) -> ac03fuu2hs == _rtP -> P_96 ) {
switch ( _rtDW -> f5mjmx3axc ) { case INTG_UPPER_SAT : if ( _rtB ->
g5nlxuwomm < 0.0 ) { ssSetSolverNeedsReset ( S ) ; _rtDW -> f5mjmx3axc =
INTG_LEAVING_UPPER_SAT ; } break ; case INTG_LEAVING_UPPER_SAT : if ( _rtB ->
g5nlxuwomm >= 0.0 ) { _rtDW -> f5mjmx3axc = INTG_UPPER_SAT ;
ssSetSolverNeedsReset ( S ) ; } break ; default : ssSetSolverNeedsReset ( S )
; if ( _rtB -> g5nlxuwomm < 0.0 ) { _rtDW -> f5mjmx3axc =
INTG_LEAVING_UPPER_SAT ; } else { _rtDW -> f5mjmx3axc = INTG_UPPER_SAT ; }
break ; } } else if ( ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) ->
ac03fuu2hs == _rtP -> P_97 ) { switch ( _rtDW -> f5mjmx3axc ) { case
INTG_LOWER_SAT : if ( _rtB -> g5nlxuwomm > 0.0 ) { ssSetSolverNeedsReset ( S
) ; _rtDW -> f5mjmx3axc = INTG_LEAVING_LOWER_SAT ; } break ; case
INTG_LEAVING_LOWER_SAT : if ( _rtB -> g5nlxuwomm <= 0.0 ) { _rtDW ->
f5mjmx3axc = INTG_LOWER_SAT ; ssSetSolverNeedsReset ( S ) ; } break ; default
: ssSetSolverNeedsReset ( S ) ; if ( _rtB -> g5nlxuwomm > 0.0 ) { _rtDW ->
f5mjmx3axc = INTG_LEAVING_LOWER_SAT ; } else { _rtDW -> f5mjmx3axc =
INTG_LOWER_SAT ; } break ; } } else { _rtDW -> f5mjmx3axc = INTG_NORMAL ; } }
if ( _rtDW -> hjntozqqrx == ( rtInf ) ) { _rtDW -> hjntozqqrx = ssGetT ( S )
; lastU = & _rtDW -> nkxs5x1rjh ; } else if ( _rtDW -> l5mtobqkp1 == ( rtInf
) ) { _rtDW -> l5mtobqkp1 = ssGetT ( S ) ; lastU = & _rtDW -> fwsgi4qud5 ; }
else if ( _rtDW -> hjntozqqrx < _rtDW -> l5mtobqkp1 ) { _rtDW -> hjntozqqrx =
ssGetT ( S ) ; lastU = & _rtDW -> nkxs5x1rjh ; } else { _rtDW -> l5mtobqkp1 =
ssGetT ( S ) ; lastU = & _rtDW -> fwsgi4qud5 ; } * lastU = _rtB -> f4uige3ylp
; if ( _rtDW -> ggqb0oluyw == ( rtInf ) ) { _rtDW -> ggqb0oluyw =
ssGetTaskTime ( S , 0 ) ; _rtDW -> ns44sv4mq2 = _rtB -> pegtjdjblx ; } else
if ( _rtDW -> efwbzmlb2g == ( rtInf ) ) { _rtDW -> efwbzmlb2g = ssGetTaskTime
( S , 0 ) ; _rtDW -> nmggmye1jy = _rtB -> pegtjdjblx ; } else if ( _rtDW ->
ggqb0oluyw < _rtDW -> efwbzmlb2g ) { _rtDW -> ggqb0oluyw = ssGetTaskTime ( S
, 0 ) ; _rtDW -> ns44sv4mq2 = _rtB -> pegtjdjblx ; } else { _rtDW ->
efwbzmlb2g = ssGetTaskTime ( S , 0 ) ; _rtDW -> nmggmye1jy = _rtB ->
pegtjdjblx ; } UNUSED_PARAMETER ( tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { n3qi1whofz * _rtB ; loikxjbxjg
* _rtP ; f1xhd02yjc * _rtX ; pqmvzr1kvu * _rtXdot ; ew10rzwqr2 * _rtDW ;
_rtDW = ( ( ew10rzwqr2 * ) ssGetRootDWork ( S ) ) ; _rtXdot = ( ( pqmvzr1kvu
* ) ssGetdX ( S ) ) ; _rtX = ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) ;
_rtP = ( ( loikxjbxjg * ) ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz *
) _ssGetBlockIO ( S ) ) ; { enum { INTG_NORMAL , INTG_LEAVING_UPPER_SAT ,
INTG_LEAVING_LOWER_SAT , INTG_UPPER_SAT , INTG_LOWER_SAT } ; if ( ( _rtDW ->
peomhw51tw != INTG_UPPER_SAT ) && ( _rtDW -> peomhw51tw != INTG_LOWER_SAT ) )
{ ( ( pqmvzr1kvu * ) ssGetdX ( S ) ) -> cv44ycfzzj = _rtB -> dkezgv5rpr ; ( (
biadadh31q * ) ssGetContStateDisabled ( S ) ) -> cv44ycfzzj = false ; } else
{ ( ( pqmvzr1kvu * ) ssGetdX ( S ) ) -> cv44ycfzzj = 0.0 ; if ( ( _rtDW ->
peomhw51tw == INTG_UPPER_SAT ) || ( _rtDW -> peomhw51tw == INTG_LOWER_SAT ) )
{ ( ( biadadh31q * ) ssGetContStateDisabled ( S ) ) -> cv44ycfzzj = true ; }
} } { ( ( pqmvzr1kvu * ) ssGetdX ( S ) ) -> c2mfgg3wdj = _rtB -> hgjsxcwqy0 ;
} if ( _rtDW -> dy5lox0tpq ) { { ( ( pqmvzr1kvu * ) ssGetdX ( S ) ) ->
kfjsvhipcr [ 0 ] = _rtB -> mhjjmdizlg ; ( ( pqmvzr1kvu * ) ssGetdX ( S ) ) ->
kfjsvhipcr [ 1 ] = _rtB -> mhjjmdizlg ; ( ( pqmvzr1kvu * ) ssGetdX ( S ) ) ->
kfjsvhipcr [ 2 ] = _rtB -> mhjjmdizlg ; ( ( pqmvzr1kvu * ) ssGetdX ( S ) ) ->
kfjsvhipcr [ 3 ] = _rtB -> mhjjmdizlg ; } } else { { real_T * dx ; int_T i ;
dx = & ( ( ( pqmvzr1kvu * ) ssGetdX ( S ) ) -> kfjsvhipcr [ 0 ] ) ; for ( i =
0 ; i < 4 ; i ++ ) { dx [ i ] = 0.0 ; } } } ssCallAccelRunBlock ( S , 2 , 11
, SS_CALL_MDL_DERIVATIVES ) ; { ( ( pqmvzr1kvu * ) ssGetdX ( S ) ) ->
d24x1wviqz = _rtB -> myc4w512hq ; } _rtXdot -> mffbzrtcri [ 0 ] = 0.0 ;
_rtXdot -> mffbzrtcri [ 1 ] = 0.0 ; _rtXdot -> mffbzrtcri [ 0U ] += _rtP ->
P_78 [ 0 ] * _rtX -> mffbzrtcri [ 0 ] ; _rtXdot -> mffbzrtcri [ 0U ] += _rtP
-> P_78 [ 1 ] * _rtX -> mffbzrtcri [ 1 ] ; _rtXdot -> mffbzrtcri [ 1U ] +=
_rtP -> P_78 [ 2 ] * _rtX -> mffbzrtcri [ 0 ] ; _rtXdot -> mffbzrtcri [ 0U ]
+= _rtP -> P_79 * _rtB -> pegtjdjblx ; if ( _rtDW -> hs0pegbvhq ) { { ( (
pqmvzr1kvu * ) ssGetdX ( S ) ) -> ovbrbhosi4 = _rtB -> nxf1wendip ; } { } { (
( pqmvzr1kvu * ) ssGetdX ( S ) ) -> ddqv420uug = _rtB -> nb5dbps2gt ; } { } }
else { { real_T * dx ; int_T i ; dx = & ( ( ( pqmvzr1kvu * ) ssGetdX ( S ) )
-> ovbrbhosi4 ) ; for ( i = 0 ; i < 2 ; i ++ ) { dx [ i ] = 0.0 ; } } } { } {
( ( pqmvzr1kvu * ) ssGetdX ( S ) ) -> kgsuae1o42 = _rtB -> hh3v5qyxgu ; } {
enum { INTG_NORMAL , INTG_LEAVING_UPPER_SAT , INTG_LEAVING_LOWER_SAT ,
INTG_UPPER_SAT , INTG_LOWER_SAT } ; if ( ( _rtDW -> f5mjmx3axc !=
INTG_UPPER_SAT ) && ( _rtDW -> f5mjmx3axc != INTG_LOWER_SAT ) ) { ( (
pqmvzr1kvu * ) ssGetdX ( S ) ) -> ac03fuu2hs = _rtB -> g5nlxuwomm ; ( (
biadadh31q * ) ssGetContStateDisabled ( S ) ) -> ac03fuu2hs = false ; } else
{ ( ( pqmvzr1kvu * ) ssGetdX ( S ) ) -> ac03fuu2hs = 0.0 ; if ( ( _rtDW ->
f5mjmx3axc == INTG_UPPER_SAT ) || ( _rtDW -> f5mjmx3axc == INTG_LOWER_SAT ) )
{ ( ( biadadh31q * ) ssGetContStateDisabled ( S ) ) -> ac03fuu2hs = true ; }
} } }
#define MDL_ZERO_CROSSINGS
static void mdlZeroCrossings ( SimStruct * S ) { n3qi1whofz * _rtB ;
loikxjbxjg * _rtP ; d15n1ti4kq * _rtZCSV ; ew10rzwqr2 * _rtDW ; _rtDW = ( (
ew10rzwqr2 * ) ssGetRootDWork ( S ) ) ; _rtZCSV = ( ( d15n1ti4kq * )
ssGetSolverZcSignalVector ( S ) ) ; _rtP = ( ( loikxjbxjg * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ;
{ enum { INTG_NORMAL , INTG_LEAVING_UPPER_SAT , INTG_LEAVING_LOWER_SAT ,
INTG_UPPER_SAT , INTG_LOWER_SAT } ; if ( _rtDW -> peomhw51tw ==
INTG_LEAVING_UPPER_SAT && ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) ->
cv44ycfzzj >= _rtP -> P_35 ) { ( ( d15n1ti4kq * ) ssGetSolverZcSignalVector (
S ) ) -> iqxcbkiqsl = 0.0 ; } else { ( ( d15n1ti4kq * )
ssGetSolverZcSignalVector ( S ) ) -> iqxcbkiqsl = ( ( f1xhd02yjc * )
ssGetContStates ( S ) ) -> cv44ycfzzj - _rtP -> P_35 ; } if ( _rtDW ->
peomhw51tw == INTG_LEAVING_LOWER_SAT && ( ( f1xhd02yjc * ) ssGetContStates (
S ) ) -> cv44ycfzzj <= _rtP -> P_36 ) { ( ( d15n1ti4kq * )
ssGetSolverZcSignalVector ( S ) ) -> ibkcjs2hqx = 0.0 ; } else { ( (
d15n1ti4kq * ) ssGetSolverZcSignalVector ( S ) ) -> ibkcjs2hqx = ( (
f1xhd02yjc * ) ssGetContStates ( S ) ) -> cv44ycfzzj - _rtP -> P_36 ; } {
boolean_T anyStateSaturated = false ; if ( ( _rtDW -> peomhw51tw ==
INTG_UPPER_SAT ) || ( _rtDW -> peomhw51tw == INTG_LOWER_SAT ) ) {
anyStateSaturated = true ; } if ( anyStateSaturated ) { ( ( d15n1ti4kq * )
ssGetSolverZcSignalVector ( S ) ) -> aqh30pjiac = _rtB -> dkezgv5rpr ; } else
{ ( ( d15n1ti4kq * ) ssGetSolverZcSignalVector ( S ) ) -> aqh30pjiac = 0.0 ;
} } } _rtZCSV -> gm0gn4ujsh = _rtB -> ifrqnqdbzs - _rtB -> b5q31uocxe ;
_rtZCSV -> aon3wdbxsm = _rtB -> glwv33kw5r - _rtB -> bd20yvnrev ; if ( _rtDW
-> dy5lox0tpq ) { _rtZCSV -> klcmtqwdp1 [ 0 ] = _rtB -> op1vortosm [ 0 ] -
_rtP -> P_7 ; _rtZCSV -> oi4hqdwyzq [ 0 ] = _rtB -> op1vortosm [ 0 ] - _rtP
-> P_8 ; _rtZCSV -> klcmtqwdp1 [ 1 ] = _rtB -> op1vortosm [ 1 ] - _rtP -> P_7
; _rtZCSV -> oi4hqdwyzq [ 1 ] = _rtB -> op1vortosm [ 1 ] - _rtP -> P_8 ;
_rtZCSV -> klcmtqwdp1 [ 2 ] = _rtB -> op1vortosm [ 2 ] - _rtP -> P_7 ;
_rtZCSV -> oi4hqdwyzq [ 2 ] = _rtB -> op1vortosm [ 2 ] - _rtP -> P_8 ;
_rtZCSV -> klcmtqwdp1 [ 3 ] = _rtB -> op1vortosm [ 3 ] - _rtP -> P_7 ;
_rtZCSV -> oi4hqdwyzq [ 3 ] = _rtB -> op1vortosm [ 3 ] - _rtP -> P_8 ;
_rtZCSV -> mwhk1hx43b [ 0 ] = _rtB -> fd43iomweh [ 0 ] - _rtP -> P_11 ;
_rtZCSV -> kkef1dvehp [ 0 ] = _rtB -> fd43iomweh [ 0 ] - _rtP -> P_12 ;
_rtZCSV -> mwhk1hx43b [ 1 ] = _rtB -> fd43iomweh [ 1 ] - _rtP -> P_11 ;
_rtZCSV -> kkef1dvehp [ 1 ] = _rtB -> fd43iomweh [ 1 ] - _rtP -> P_12 ;
_rtZCSV -> mwhk1hx43b [ 2 ] = _rtB -> fd43iomweh [ 2 ] - _rtP -> P_11 ;
_rtZCSV -> kkef1dvehp [ 2 ] = _rtB -> fd43iomweh [ 2 ] - _rtP -> P_12 ;
_rtZCSV -> mwhk1hx43b [ 3 ] = _rtB -> fd43iomweh [ 3 ] - _rtP -> P_11 ;
_rtZCSV -> kkef1dvehp [ 3 ] = _rtB -> fd43iomweh [ 3 ] - _rtP -> P_12 ; }
else { { real_T * zcsv = & ( ( ( d15n1ti4kq * ) ssGetSolverZcSignalVector ( S
) ) -> gh3zkkuupt [ 0 ] ) ; int_T i ; for ( i = 0 ; i < 20 ; i ++ ) { zcsv [
i ] = 0.0 ; } } } ssCallAccelRunBlock ( S , 2 , 11 ,
SS_CALL_MDL_ZERO_CROSSINGS ) ; _rtZCSV -> k3hd0i2tcn = _rtB -> g01gfakbyi -
_rtP -> P_18 ; _rtZCSV -> oqpfvy1dny = _rtB -> g01gfakbyi - _rtP -> P_19 ;
_rtZCSV -> jmyt5p2lv0 [ 0 ] = _rtB -> cqhwdxdbgp [ 0 ] - _rtP -> P_56 ;
_rtZCSV -> jmyt5p2lv0 [ 1 ] = _rtB -> cqhwdxdbgp [ 1 ] - _rtP -> P_56 ;
_rtZCSV -> jmyt5p2lv0 [ 2 ] = _rtB -> cqhwdxdbgp [ 2 ] - _rtP -> P_56 ;
_rtZCSV -> jmyt5p2lv0 [ 3 ] = _rtB -> cqhwdxdbgp [ 3 ] - _rtP -> P_56 ;
_rtZCSV -> pdnt4ivgz1 [ 0 ] = _rtB -> c03rfbw3dp [ 0 ] - _rtP -> P_57 ;
_rtZCSV -> aynosrjef4 [ 0 ] = _rtB -> c03rfbw3dp [ 0 ] - _rtP -> P_58 ;
_rtZCSV -> pdnt4ivgz1 [ 1 ] = _rtB -> c03rfbw3dp [ 1 ] - _rtP -> P_57 ;
_rtZCSV -> aynosrjef4 [ 1 ] = _rtB -> c03rfbw3dp [ 1 ] - _rtP -> P_58 ;
_rtZCSV -> pdnt4ivgz1 [ 2 ] = _rtB -> c03rfbw3dp [ 2 ] - _rtP -> P_57 ;
_rtZCSV -> aynosrjef4 [ 2 ] = _rtB -> c03rfbw3dp [ 2 ] - _rtP -> P_58 ;
_rtZCSV -> pdnt4ivgz1 [ 3 ] = _rtB -> c03rfbw3dp [ 3 ] - _rtP -> P_57 ;
_rtZCSV -> aynosrjef4 [ 3 ] = _rtB -> c03rfbw3dp [ 3 ] - _rtP -> P_58 ;
_rtZCSV -> luanwi4qfn = _rtB -> j4d4cpsnpe ; if ( _rtDW -> hs0pegbvhq ) {
_rtZCSV -> fjrckoi533 = _rtB -> gyf5irieym - _rtP -> P_23 ; _rtZCSV ->
d3vnczv3ye = _rtB -> gyf5irieym - _rtP -> P_24 ; _rtZCSV -> pcvsw1bene = _rtB
-> ifw02obj0c - _rtP -> P_31 ; _rtZCSV -> h4uj3fygha = _rtB -> ifw02obj0c -
_rtP -> P_32 ; } else { { real_T * zcsv = & ( ( ( d15n1ti4kq * )
ssGetSolverZcSignalVector ( S ) ) -> fjrckoi533 ) ; int_T i ; for ( i = 0 ; i
< 4 ; i ++ ) { zcsv [ i ] = 0.0 ; } } } _rtZCSV -> lgy4qzdyk5 = _rtB ->
gyf5irieym - _rtP -> P_84 ; _rtZCSV -> ftcokbolqr = _rtB -> gyf5irieym - _rtP
-> P_85 ; _rtZCSV -> exss1ouh0g = ssGetT ( S ) - _rtP -> P_89 ; { enum {
INTG_NORMAL , INTG_LEAVING_UPPER_SAT , INTG_LEAVING_LOWER_SAT ,
INTG_UPPER_SAT , INTG_LOWER_SAT } ; if ( _rtDW -> f5mjmx3axc ==
INTG_LEAVING_UPPER_SAT && ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) ->
ac03fuu2hs >= _rtP -> P_96 ) { ( ( d15n1ti4kq * ) ssGetSolverZcSignalVector (
S ) ) -> hw3dm4j5xv = 0.0 ; } else { ( ( d15n1ti4kq * )
ssGetSolverZcSignalVector ( S ) ) -> hw3dm4j5xv = ( ( f1xhd02yjc * )
ssGetContStates ( S ) ) -> ac03fuu2hs - _rtP -> P_96 ; } if ( _rtDW ->
f5mjmx3axc == INTG_LEAVING_LOWER_SAT && ( ( f1xhd02yjc * ) ssGetContStates (
S ) ) -> ac03fuu2hs <= _rtP -> P_97 ) { ( ( d15n1ti4kq * )
ssGetSolverZcSignalVector ( S ) ) -> get0bsyz2m = 0.0 ; } else { ( (
d15n1ti4kq * ) ssGetSolverZcSignalVector ( S ) ) -> get0bsyz2m = ( (
f1xhd02yjc * ) ssGetContStates ( S ) ) -> ac03fuu2hs - _rtP -> P_97 ; } {
boolean_T anyStateSaturated = false ; if ( ( _rtDW -> f5mjmx3axc ==
INTG_UPPER_SAT ) || ( _rtDW -> f5mjmx3axc == INTG_LOWER_SAT ) ) {
anyStateSaturated = true ; } if ( anyStateSaturated ) { ( ( d15n1ti4kq * )
ssGetSolverZcSignalVector ( S ) ) -> llvj4qptyf = _rtB -> g5nlxuwomm ; } else
{ ( ( d15n1ti4kq * ) ssGetSolverZcSignalVector ( S ) ) -> llvj4qptyf = 0.0 ;
} } } _rtZCSV -> dfbbuvv5as = _rtB -> ei4cwq53sd - _rtP -> P_99 ; _rtZCSV ->
m2iir1qd3y = _rtB -> ei4cwq53sd - _rtP -> P_100 ; } static void
mdlInitializeSizes ( SimStruct * S ) { ssSetChecksumVal ( S , 0 , 4238001071U
) ; ssSetChecksumVal ( S , 1 , 3905187501U ) ; ssSetChecksumVal ( S , 2 ,
4061141916U ) ; ssSetChecksumVal ( S , 3 , 1408979703U ) ; { mxArray *
slVerStructMat = NULL ; mxArray * slStrMat = mxCreateString ( "simulink" ) ;
char slVerChar [ 10 ] ; int status = mexCallMATLAB ( 1 , & slVerStructMat , 1
, & slStrMat , "ver" ) ; if ( status == 0 ) { mxArray * slVerMat = mxGetField
( slVerStructMat , 0 , "Version" ) ; if ( slVerMat == NULL ) { status = 1 ; }
else { status = mxGetString ( slVerMat , slVerChar , 10 ) ; } }
mxDestroyArray ( slStrMat ) ; mxDestroyArray ( slVerStructMat ) ; if ( (
status == 1 ) || ( strcmp ( slVerChar , "8.3" ) != 0 ) ) { return ; } }
ssSetOptions ( S , SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork (
S ) != sizeof ( ew10rzwqr2 ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file." ) ; } if ( ssGetSizeofGlobalBlockIO ( S
) != sizeof ( n3qi1whofz ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file." ) ; } { int ssSizeofParams ;
ssGetSizeofParams ( S , & ssSizeofParams ) ; if ( ssSizeofParams != sizeof (
loikxjbxjg ) ) { static char msg [ 256 ] ; sprintf ( msg ,
"Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file." ) ; } } _ssSetDefaultParam ( S , (
real_T * ) & o2iu0a2jke ) ; rt_InitInfAndNaN ( sizeof ( real_T ) ) ; ( (
loikxjbxjg * ) ssGetDefaultParam ( S ) ) -> P_35 = rtInf ; ( ( loikxjbxjg * )
ssGetDefaultParam ( S ) ) -> P_36 = rtMinusInf ; ( ( loikxjbxjg * )
ssGetDefaultParam ( S ) ) -> P_57 = rtInf ; } static void
mdlInitializeSampleTimes ( SimStruct * S ) { } static void mdlTerminate (
SimStruct * S ) { }
#include "simulink.c"
