#include "__cf_Copy_of_single_phase_PVarry_inverter.h"
#include <math.h>
#include "Copy_of_single_phase_PVarry_inverter_acc.h"
#include "Copy_of_single_phase_PVarry_inverter_acc_private.h"
#include <stdio.h>
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T
Copy_of_single_phase_PVarry_inverter_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T
* bufSzPtr , int_T * tailPtr , int_T * headPtr , int_T * lastPtr , real_T
tMinusDelay , real_T * * tBufPtr , real_T * * uBufPtr , real_T * * xBufPtr ,
boolean_T isfixedbuf , boolean_T istransportdelay , int_T * maxNewBufSzPtr )
{ int_T testIdx ; int_T tail = * tailPtr ; int_T bufSz = * bufSzPtr ; real_T
* tBuf = * tBufPtr ; real_T * xBuf = ( NULL ) ; int_T numBuffer = 2 ; if (
istransportdelay ) { numBuffer = 3 ; xBuf = * xBufPtr ; } testIdx = ( tail <
( bufSz - 1 ) ) ? ( tail + 1 ) : 0 ; if ( ( tMinusDelay <= tBuf [ testIdx ] )
&& ! isfixedbuf ) { int_T j ; real_T * tempT ; real_T * tempU ; real_T *
tempX = ( NULL ) ; real_T * uBuf = * uBufPtr ; int_T newBufSz = bufSz + 1024
; if ( newBufSz > * maxNewBufSzPtr ) { * maxNewBufSzPtr = newBufSz ; } tempU
= ( real_T * ) utMalloc ( numBuffer * newBufSz * sizeof ( real_T ) ) ; if (
tempU == ( NULL ) ) { return ( false ) ; } tempT = tempU + newBufSz ; if (
istransportdelay ) tempX = tempT + newBufSz ; for ( j = tail ; j < bufSz ; j
++ ) { tempT [ j - tail ] = tBuf [ j ] ; tempU [ j - tail ] = uBuf [ j ] ; if
( istransportdelay ) tempX [ j - tail ] = xBuf [ j ] ; } for ( j = 0 ; j <
tail ; j ++ ) { tempT [ j + bufSz - tail ] = tBuf [ j ] ; tempU [ j + bufSz -
tail ] = uBuf [ j ] ; if ( istransportdelay ) tempX [ j + bufSz - tail ] =
xBuf [ j ] ; } if ( * lastPtr > tail ) { * lastPtr -= tail ; } else { *
lastPtr += ( bufSz - tail ) ; } * tailPtr = 0 ; * headPtr = bufSz ; utFree (
uBuf ) ; * bufSzPtr = newBufSz ; * tBufPtr = tempT ; * uBufPtr = tempU ; if (
istransportdelay ) * xBufPtr = tempX ; } else { * tailPtr = testIdx ; }
return ( true ) ; } real_T
Copy_of_single_phase_PVarry_inverter_acc_rt_TDelayInterpolate ( real_T
tMinusDelay , real_T tStart , real_T * tBuf , real_T * uBuf , int_T bufSz ,
int_T * lastIdx , int_T oldestIdx , int_T newIdx , real_T initOutput ,
boolean_T discrete , boolean_T minorStepAndTAtLastMajorOutput ) { int_T i ;
real_T yout , t1 , t2 , u1 , u2 ; if ( ( newIdx == 0 ) && ( oldestIdx == 0 )
&& ( tMinusDelay > tStart ) ) return initOutput ; if ( tMinusDelay <= tStart
) return initOutput ; if ( ( tMinusDelay <= tBuf [ oldestIdx ] ) ) { if (
discrete ) { return ( uBuf [ oldestIdx ] ) ; } else { int_T tempIdx =
oldestIdx + 1 ; if ( oldestIdx == bufSz - 1 ) tempIdx = 0 ; t1 = tBuf [
oldestIdx ] ; t2 = tBuf [ tempIdx ] ; u1 = uBuf [ oldestIdx ] ; u2 = uBuf [
tempIdx ] ; if ( t2 == t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else {
yout = u1 ; } } else { real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ;
real_T f2 = 1.0 - f1 ; yout = f1 * u1 + f2 * u2 ; } return yout ; } } if (
minorStepAndTAtLastMajorOutput ) { if ( newIdx != 0 ) { if ( * lastIdx ==
newIdx ) { ( * lastIdx ) -- ; } newIdx -- ; } else { if ( * lastIdx == newIdx
) { * lastIdx = bufSz - 1 ; } newIdx = bufSz - 1 ; } } i = * lastIdx ; if (
tBuf [ i ] < tMinusDelay ) { while ( tBuf [ i ] < tMinusDelay ) { if ( i ==
newIdx ) break ; i = ( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } } else { while
( tBuf [ i ] >= tMinusDelay ) { i = ( i > 0 ) ? i - 1 : ( bufSz - 1 ) ; } i =
( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } * lastIdx = i ; if ( discrete ) {
double tempEps = ( DBL_EPSILON ) * 128.0 ; double localEps = tempEps *
muDoubleScalarAbs ( tBuf [ i ] ) ; if ( tempEps > localEps ) { localEps =
tempEps ; } localEps = localEps / 2.0 ; if ( tMinusDelay >= ( tBuf [ i ] -
localEps ) ) { yout = uBuf [ i ] ; } else { if ( i == 0 ) { yout = uBuf [
bufSz - 1 ] ; } else { yout = uBuf [ i - 1 ] ; } } } else { if ( i == 0 ) {
t1 = tBuf [ bufSz - 1 ] ; u1 = uBuf [ bufSz - 1 ] ; } else { t1 = tBuf [ i -
1 ] ; u1 = uBuf [ i - 1 ] ; } t2 = tBuf [ i ] ; u2 = uBuf [ i ] ; if ( t2 ==
t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else { yout = u1 ; } } else {
real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ; real_T f2 = 1.0 - f1 ; yout
= f1 * u1 + f2 * u2 ; } } return ( yout ) ; } real_T look1_binlxpw ( real_T
u0 , const real_T bp0 [ ] , const real_T table [ ] , uint32_T maxIndex ) {
real_T frac ; uint32_T iRght ; uint32_T iLeft ; uint32_T bpIdx ; if ( u0 <=
bp0 [ 0U ] ) { iLeft = 0U ; frac = ( u0 - bp0 [ 0U ] ) / ( bp0 [ 1U ] - bp0 [
0U ] ) ; } else if ( u0 < bp0 [ maxIndex ] ) { bpIdx = maxIndex >> 1U ; iLeft
= 0U ; iRght = maxIndex ; while ( iRght - iLeft > 1U ) { if ( u0 < bp0 [
bpIdx ] ) { iRght = bpIdx ; } else { iLeft = bpIdx ; } bpIdx = ( iRght +
iLeft ) >> 1U ; } frac = ( u0 - bp0 [ iLeft ] ) / ( bp0 [ iLeft + 1U ] - bp0
[ iLeft ] ) ; } else { iLeft = maxIndex - 1U ; frac = ( u0 - bp0 [ maxIndex -
1U ] ) / ( bp0 [ maxIndex ] - bp0 [ maxIndex - 1U ] ) ; } return ( table [
iLeft + 1U ] - table [ iLeft ] ) * frac + table [ iLeft ] ; } static void
mdlOutputs ( SimStruct * S , int_T tid ) { real_T lvyn0qqrsb ; real_T * lastU
; real_T fwpbii2lw0 ; real_T o5b4tcj1fp ; real_T fh0501wpui ; real_T
ic3wdyrovx_idx_0 ; real_T ic3wdyrovx_idx_1 ; real_T ic3wdyrovx_idx_2 ; real_T
ic3wdyrovx_idx_3 ; real_T m2krcgtgey_idx_0 ; real_T m2krcgtgey_idx_1 ; real_T
m2krcgtgey_idx_2 ; real_T m2krcgtgey_idx_3 ; real_T u0 ; n3qi1whofz * _rtB ;
loikxjbxjg * _rtP ; ew10rzwqr2 * _rtDW ; _rtDW = ( ( ew10rzwqr2 * )
ssGetRootDWork ( S ) ) ; _rtP = ( ( loikxjbxjg * ) ssGetDefaultParam ( S ) )
; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ; _rtB -> eraw4hoh2g = ( (
f1xhd02yjc * ) ssGetContStates ( S ) ) -> d24x1wviqz ; { real_T * * uBuffer =
( real_T * * ) & _rtDW -> lvful1kguw . TUbufferPtrs [ 0 ] ; real_T * *
tBuffer = ( real_T * * ) & _rtDW -> lvful1kguw . TUbufferPtrs [ 1 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_43 ; _rtB
-> l5axnpe3c4 = Copy_of_single_phase_PVarry_inverter_acc_rt_TDelayInterpolate
( tMinusDelay , 0.0 , * tBuffer , * uBuffer , _rtDW -> aq5jg3ks0r .
CircularBufSize , & _rtDW -> aq5jg3ks0r . Last , _rtDW -> aq5jg3ks0r . Tail ,
_rtDW -> aq5jg3ks0r . Head , _rtP -> P_44 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } _rtB -> p5xbubtydy = _rtB -> eraw4hoh2g - _rtB -> l5axnpe3c4 ; if (
ssIsMajorTimeStep ( S ) ) { _rtDW -> gpmreqszwf = _rtB -> p5xbubtydy >= 0.0 ?
1 : 0 ; } _rtB -> hi5trkgyxh = _rtDW -> gpmreqszwf > 0 ? _rtB -> p5xbubtydy :
- _rtB -> p5xbubtydy ; if ( ssIsMajorTimeStep ( S ) ) { if ( _rtDW ->
bp43bxk2t5 != 0 ) { ssSetSolverNeedsReset ( S ) ; _rtDW -> bp43bxk2t5 = 0 ; }
_rtB -> mtgffifgwz = muDoubleScalarSqrt ( _rtB -> hi5trkgyxh ) ; } else { if
( _rtB -> hi5trkgyxh < 0.0 ) { _rtB -> mtgffifgwz = - muDoubleScalarSqrt (
muDoubleScalarAbs ( _rtB -> hi5trkgyxh ) ) ; } else { _rtB -> mtgffifgwz =
muDoubleScalarSqrt ( _rtB -> hi5trkgyxh ) ; } _rtDW -> bp43bxk2t5 = 1 ; }
ssCallAccelRunBlock ( S , 3 , 5 , SS_CALL_MDL_OUTPUTS ) ; if ( ssIsSampleHit
( S , 2 , 0 ) ) { _rtB -> eptoge5nvj [ 0 ] = _rtP -> P_45 [ 0 ] ; _rtB ->
eptoge5nvj [ 1 ] = _rtP -> P_45 [ 1 ] ; _rtB -> eptoge5nvj [ 2 ] = _rtP ->
P_45 [ 2 ] ; _rtB -> eptoge5nvj [ 3 ] = _rtP -> P_45 [ 3 ] ; _rtB ->
e1r5bc30bt [ 0 ] = _rtDW -> baicdkdr1f [ 0 ] ; _rtB -> e1r5bc30bt [ 1 ] =
_rtDW -> baicdkdr1f [ 1 ] ; _rtB -> e1r5bc30bt [ 2 ] = _rtDW -> baicdkdr1f [
2 ] ; _rtB -> e1r5bc30bt [ 3 ] = _rtDW -> baicdkdr1f [ 3 ] ; } _rtB ->
cjiamez2rz = muDoubleScalarSin ( _rtP -> P_49 * ssGetTaskTime ( S , 0 ) +
_rtP -> P_50 ) * _rtP -> P_47 + _rtP -> P_48 ; if ( ssIsSampleHit ( S , 1 , 0
) ) { _rtB -> gltafyopjy = _rtP -> P_51 ; } _rtB -> n0c3qawger = _rtB ->
cjiamez2rz * _rtB -> gltafyopjy ; lvyn0qqrsb = ssGetT ( S ) ; _rtB ->
fdpn5uittm = ssGetTStart ( S ) ; lvyn0qqrsb -= _rtB -> fdpn5uittm ; if (
ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> kbsum2nagl = _rtP -> P_52 ; }
lvyn0qqrsb = muDoubleScalarRem ( lvyn0qqrsb , _rtB -> kbsum2nagl ) ; _rtB ->
fa5fdcf00w = look1_binlxpw ( lvyn0qqrsb , _rtP -> P_54 , _rtP -> P_53 , 2U )
; if ( ssIsSampleHit ( S , 1 , 0 ) ) { if ( ssIsMajorTimeStep ( S ) ) { _rtDW
-> nohmdkg0j5 = ( _rtB -> n0c3qawger >= _rtB -> fa5fdcf00w ) ; } _rtB ->
dbsx2m0w2l = _rtDW -> nohmdkg0j5 ; _rtB -> iamijko5g5 = ! _rtB -> dbsx2m0w2l
; } _rtB -> ifeybmu5cv = _rtB -> n0c3qawger + _rtB -> fa5fdcf00w ; if (
ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> ih14wg2u1o = _rtP -> P_55 ; if (
ssIsMajorTimeStep ( S ) ) { _rtDW -> ozaztxkn2g = ( _rtB -> ifeybmu5cv >=
_rtB -> ih14wg2u1o ) ; } _rtB -> lm1c1hnyuc = _rtDW -> ozaztxkn2g ; _rtB ->
b30rvf4zhg = ! _rtB -> lm1c1hnyuc ; _rtB -> newlqhcsqy = _rtP -> P_90 ; if (
ssIsMajorTimeStep ( S ) ) { if ( _rtB -> newlqhcsqy ) { if ( ! _rtDW ->
pv0a0wf1ed ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetSolverNeedsReset ( S ) ; } _rtDW -> lmvaxbaj01 [ 0 ] = _rtP -> P_32 ;
_rtDW -> lmvaxbaj01 [ 1 ] = _rtP -> P_32 ; _rtDW -> lmvaxbaj01 [ 2 ] = _rtP
-> P_32 ; _rtDW -> lmvaxbaj01 [ 3 ] = _rtP -> P_32 ; _rtDW -> chvkdrjpen [ 0
] = 2 ; _rtDW -> chvkdrjpen [ 1 ] = 2 ; _rtDW -> chvkdrjpen [ 2 ] = 2 ; _rtDW
-> chvkdrjpen [ 3 ] = 2 ; _rtDW -> kz1spygazc [ 0 ] = _rtP -> P_41 ; _rtDW ->
kz1spygazc [ 1 ] = _rtP -> P_41 ; _rtDW -> kz1spygazc [ 2 ] = _rtP -> P_41 ;
_rtDW -> kz1spygazc [ 3 ] = _rtP -> P_41 ; _rtDW -> pv0a0wf1ed = true ; } }
else { if ( _rtDW -> pv0a0wf1ed ) { ssSetSolverNeedsReset ( S ) ; _rtDW ->
pv0a0wf1ed = false ; } } } } if ( _rtDW -> pv0a0wf1ed ) { if ( ssIsSampleHit
( S , 2 , 0 ) ) { _rtB -> dojyakp2gd = _rtP -> P_29 ; } if ( ssIsSampleHit (
S , 1 , 0 ) ) { o5b4tcj1fp = _rtP -> P_30 ; } if ( ssIsSampleHit ( S , 2 , 0
) ) { if ( ( ! _rtB -> dbsx2m0w2l ) && ( _rtDW -> chvkdrjpen [ 0 ] == 1 ) ) {
_rtDW -> lmvaxbaj01 [ 0 ] = _rtP -> P_32 ; } if ( ( ! _rtB -> iamijko5g5 ) &&
( _rtDW -> chvkdrjpen [ 1 ] == 1 ) ) { _rtDW -> lmvaxbaj01 [ 1 ] = _rtP ->
P_32 ; } if ( ( ! _rtB -> b30rvf4zhg ) && ( _rtDW -> chvkdrjpen [ 2 ] == 1 )
) { _rtDW -> lmvaxbaj01 [ 2 ] = _rtP -> P_32 ; } if ( ( ! _rtB -> lm1c1hnyuc
) && ( _rtDW -> chvkdrjpen [ 3 ] == 1 ) ) { _rtDW -> lmvaxbaj01 [ 3 ] = _rtP
-> P_32 ; } _rtB -> pete2ryyot [ 0 ] = _rtDW -> lmvaxbaj01 [ 0 ] ; _rtB ->
pete2ryyot [ 1 ] = _rtDW -> lmvaxbaj01 [ 1 ] ; _rtB -> pete2ryyot [ 2 ] =
_rtDW -> lmvaxbaj01 [ 2 ] ; _rtB -> pete2ryyot [ 3 ] = _rtDW -> lmvaxbaj01 [
3 ] ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { fh0501wpui = _rtP -> P_33 ; } if
( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> e33thb05at [ 0 ] = _rtP -> P_34 *
_rtB -> pete2ryyot [ 0 ] ; _rtB -> e33thb05at [ 1 ] = _rtP -> P_34 * _rtB ->
pete2ryyot [ 1 ] ; _rtB -> e33thb05at [ 2 ] = _rtP -> P_34 * _rtB ->
pete2ryyot [ 2 ] ; _rtB -> e33thb05at [ 3 ] = _rtP -> P_34 * _rtB ->
pete2ryyot [ 3 ] ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { ic3wdyrovx_idx_3 =
fh0501wpui + _rtB -> e33thb05at [ 0 ] ; u0 = ( _rtP -> P_37 - _rtB ->
pete2ryyot [ 0 ] ) * _rtP -> P_38 ; if ( ic3wdyrovx_idx_3 > _rtP -> P_35 ) {
ic3wdyrovx_idx_3 = _rtP -> P_35 ; } else { if ( ic3wdyrovx_idx_3 < _rtP ->
P_36 ) { ic3wdyrovx_idx_3 = _rtP -> P_36 ; } } if ( u0 > _rtP -> P_39 ) { u0
= _rtP -> P_39 ; } else { if ( u0 < _rtP -> P_40 ) { u0 = _rtP -> P_40 ; } }
ic3wdyrovx_idx_0 = ic3wdyrovx_idx_3 + u0 ; ic3wdyrovx_idx_3 = fh0501wpui +
_rtB -> e33thb05at [ 1 ] ; u0 = ( _rtP -> P_37 - _rtB -> pete2ryyot [ 1 ] ) *
_rtP -> P_38 ; if ( ic3wdyrovx_idx_3 > _rtP -> P_35 ) { ic3wdyrovx_idx_3 =
_rtP -> P_35 ; } else { if ( ic3wdyrovx_idx_3 < _rtP -> P_36 ) {
ic3wdyrovx_idx_3 = _rtP -> P_36 ; } } if ( u0 > _rtP -> P_39 ) { u0 = _rtP ->
P_39 ; } else { if ( u0 < _rtP -> P_40 ) { u0 = _rtP -> P_40 ; } }
ic3wdyrovx_idx_1 = ic3wdyrovx_idx_3 + u0 ; ic3wdyrovx_idx_3 = fh0501wpui +
_rtB -> e33thb05at [ 2 ] ; u0 = ( _rtP -> P_37 - _rtB -> pete2ryyot [ 2 ] ) *
_rtP -> P_38 ; if ( ic3wdyrovx_idx_3 > _rtP -> P_35 ) { ic3wdyrovx_idx_3 =
_rtP -> P_35 ; } else { if ( ic3wdyrovx_idx_3 < _rtP -> P_36 ) {
ic3wdyrovx_idx_3 = _rtP -> P_36 ; } } if ( u0 > _rtP -> P_39 ) { u0 = _rtP ->
P_39 ; } else { if ( u0 < _rtP -> P_40 ) { u0 = _rtP -> P_40 ; } }
ic3wdyrovx_idx_2 = ic3wdyrovx_idx_3 + u0 ; ic3wdyrovx_idx_3 = fh0501wpui +
_rtB -> e33thb05at [ 3 ] ; u0 = ( _rtP -> P_37 - _rtB -> pete2ryyot [ 3 ] ) *
_rtP -> P_38 ; if ( ic3wdyrovx_idx_3 > _rtP -> P_35 ) { ic3wdyrovx_idx_3 =
_rtP -> P_35 ; } else { if ( ic3wdyrovx_idx_3 < _rtP -> P_36 ) {
ic3wdyrovx_idx_3 = _rtP -> P_36 ; } } if ( u0 > _rtP -> P_39 ) { u0 = _rtP ->
P_39 ; } else { if ( u0 < _rtP -> P_40 ) { u0 = _rtP -> P_40 ; } }
ic3wdyrovx_idx_3 += u0 ; } if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB ->
ce342lt12z [ 0 ] = _rtDW -> kz1spygazc [ 0 ] ; _rtB -> ce342lt12z [ 1 ] =
_rtDW -> kz1spygazc [ 1 ] ; _rtB -> ce342lt12z [ 2 ] = _rtDW -> kz1spygazc [
2 ] ; _rtB -> ce342lt12z [ 3 ] = _rtDW -> kz1spygazc [ 3 ] ; } if (
ssIsSampleHit ( S , 1 , 0 ) ) { if ( _rtB -> dbsx2m0w2l ) { _rtB ->
pndsy5jc4j [ 0 ] = _rtB -> e1r5bc30bt [ 0 ] ; } else { _rtB -> pndsy5jc4j [ 0
] = _rtB -> ce342lt12z [ 0 ] ; } if ( _rtB -> iamijko5g5 ) { _rtB ->
pndsy5jc4j [ 1 ] = _rtB -> e1r5bc30bt [ 1 ] ; } else { _rtB -> pndsy5jc4j [ 1
] = _rtB -> ce342lt12z [ 1 ] ; } if ( _rtB -> b30rvf4zhg ) { _rtB ->
pndsy5jc4j [ 2 ] = _rtB -> e1r5bc30bt [ 2 ] ; } else { _rtB -> pndsy5jc4j [ 2
] = _rtB -> ce342lt12z [ 2 ] ; } if ( _rtB -> lm1c1hnyuc ) { _rtB ->
pndsy5jc4j [ 3 ] = _rtB -> e1r5bc30bt [ 3 ] ; } else { _rtB -> pndsy5jc4j [ 3
] = _rtB -> ce342lt12z [ 3 ] ; } _rtB -> jxsdu1xspc [ 0 ] = ic3wdyrovx_idx_0
* _rtB -> pndsy5jc4j [ 0 ] * o5b4tcj1fp ; _rtB -> jxsdu1xspc [ 1 ] =
ic3wdyrovx_idx_1 * _rtB -> pndsy5jc4j [ 1 ] * o5b4tcj1fp ; _rtB -> jxsdu1xspc
[ 2 ] = ic3wdyrovx_idx_2 * _rtB -> pndsy5jc4j [ 2 ] * o5b4tcj1fp ; _rtB ->
jxsdu1xspc [ 3 ] = ic3wdyrovx_idx_3 * _rtB -> pndsy5jc4j [ 3 ] * o5b4tcj1fp ;
} if ( ssIsMajorTimeStep ( S ) ) { srUpdateBC ( _rtDW -> gynuk4p2jh ) ; } }
if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> cjpzarx0n4 = _rtP -> P_56 ;
ssCallAccelRunBlock ( S , 3 , 26 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> hrelajakuw
= _rtP -> P_57 * _rtB -> jpxpl0muvi [ 10 ] ; } ssCallAccelRunBlock ( S , 3 ,
28 , SS_CALL_MDL_OUTPUTS ) ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB ->
cufm0ivekq = _rtP -> P_58 ; } _rtB -> nfplosnjhr = _rtB -> cufm0ivekq - _rtB
-> mtgffifgwz ; _rtB -> ot35ovqeag = _rtP -> P_59 * _rtB -> nfplosnjhr ; if (
( _rtDW -> ivzxketgv1 >= ssGetT ( S ) ) && ( _rtDW -> a0chmotcqw >= ssGetT (
S ) ) ) { lvyn0qqrsb = 0.0 ; } else { o5b4tcj1fp = _rtDW -> ivzxketgv1 ;
lastU = & _rtDW -> o1ezyn5fzo ; if ( _rtDW -> ivzxketgv1 < _rtDW ->
a0chmotcqw ) { if ( _rtDW -> a0chmotcqw < ssGetT ( S ) ) { o5b4tcj1fp = _rtDW
-> a0chmotcqw ; lastU = & _rtDW -> ltqpnqteqg ; } } else { if ( _rtDW ->
ivzxketgv1 >= ssGetT ( S ) ) { o5b4tcj1fp = _rtDW -> a0chmotcqw ; lastU = &
_rtDW -> ltqpnqteqg ; } } lvyn0qqrsb = ( _rtB -> ot35ovqeag - * lastU ) / (
ssGetT ( S ) - o5b4tcj1fp ) ; } _rtB -> i3diaivwnf = _rtP -> P_60 * _rtB ->
nfplosnjhr ; _rtB -> eupwznu2ui = ( ( f1xhd02yjc * ) ssGetContStates ( S ) )
-> c2mfgg3wdj ; _rtB -> ojbxxbpc2o = ( _rtP -> P_62 * _rtB -> nfplosnjhr +
_rtB -> eupwznu2ui ) + lvyn0qqrsb ; if ( ssIsMajorTimeStep ( S ) ) { _rtDW ->
bux4425y54 = _rtB -> ojbxxbpc2o >= _rtP -> P_63 ? 1 : _rtB -> ojbxxbpc2o >
_rtP -> P_64 ? 0 : - 1 ; } if ( ssIsSampleHit ( S , 4 , 0 ) ) { _rtB ->
dpgmiyas1i = _rtB -> hrelajakuw * _rtB -> hrelajakuw * _rtP -> P_65 ; } if (
ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> m4nfvw1rn0 [ 0 ] = _rtDW ->
o4e1fsfulk [ 0 ] ; _rtB -> m4nfvw1rn0 [ 1 ] = _rtDW -> o4e1fsfulk [ 1 ] ;
_rtB -> m4nfvw1rn0 [ 2 ] = _rtDW -> o4e1fsfulk [ 2 ] ; _rtB -> m4nfvw1rn0 [ 3
] = _rtDW -> o4e1fsfulk [ 3 ] ; } _rtB -> bccdh44nii = _rtP -> P_67 * _rtB ->
cjiamez2rz ; _rtB -> ek5rzq1ns1 = _rtP -> P_68 * _rtB -> cjiamez2rz ; if (
ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> hvdlqlaqzp = _rtP -> P_69 ; } if (
ssIsSampleHit ( S , 3 , 0 ) ) { _rtB -> fl024uen4c = _rtDW -> otmp0asolx ; }
lvyn0qqrsb = ssGetT ( S ) ; _rtB -> bc0m50zxeh = ssGetTStart ( S ) ;
lvyn0qqrsb -= _rtB -> bc0m50zxeh ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB
-> mwq45evkig = _rtP -> P_74 ; } lvyn0qqrsb = muDoubleScalarRem ( lvyn0qqrsb
, _rtB -> mwq45evkig ) ; _rtB -> fjfh3iqyzs = look1_binlxpw ( lvyn0qqrsb ,
_rtP -> P_76 , _rtP -> P_75 , 2U ) ; if ( ssIsSampleHit ( S , 1 , 0 ) ) {
_rtB -> kyxnep5jcw = _rtP -> P_91 ; } ssCallAccelRunBlock ( S , 3 , 56 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 3 , 57 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 3 , 58 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 3 , 59 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssIsSampleHit ( S , 2 , 0 ) ) { fwpbii2lw0 =
_rtP -> P_77 ; m2krcgtgey_idx_0 = _rtP -> P_78 * _rtB -> jpxpl0muvi [ 4 ] ;
m2krcgtgey_idx_1 = _rtP -> P_78 * _rtB -> jpxpl0muvi [ 5 ] ; m2krcgtgey_idx_2
= _rtP -> P_78 * _rtB -> jpxpl0muvi [ 6 ] ; m2krcgtgey_idx_3 = _rtP -> P_78 *
_rtB -> jpxpl0muvi [ 7 ] ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB ->
kbbgcbgjav [ 0 ] = _rtB -> dbsx2m0w2l ; _rtB -> kbbgcbgjav [ 1 ] = _rtB ->
iamijko5g5 ; _rtB -> kbbgcbgjav [ 2 ] = _rtB -> b30rvf4zhg ; _rtB ->
kbbgcbgjav [ 3 ] = _rtB -> lm1c1hnyuc ; } if ( ssIsSampleHit ( S , 2 , 0 ) )
{ if ( ! ( _rtB -> nyxb5uqudu [ 4 ] >= _rtP -> P_79 ) ) { m2krcgtgey_idx_0 =
fwpbii2lw0 ; } if ( m2krcgtgey_idx_0 > _rtP -> P_80 ) { _rtB -> od1mnsxrhg [
0 ] = _rtP -> P_80 ; } else if ( m2krcgtgey_idx_0 < _rtP -> P_81 ) { _rtB ->
od1mnsxrhg [ 0 ] = _rtP -> P_81 ; } else { _rtB -> od1mnsxrhg [ 0 ] =
m2krcgtgey_idx_0 ; } if ( _rtB -> nyxb5uqudu [ 5 ] >= _rtP -> P_79 ) {
m2krcgtgey_idx_0 = m2krcgtgey_idx_1 ; } else { m2krcgtgey_idx_0 = fwpbii2lw0
; } if ( m2krcgtgey_idx_0 > _rtP -> P_80 ) { _rtB -> od1mnsxrhg [ 1 ] = _rtP
-> P_80 ; } else if ( m2krcgtgey_idx_0 < _rtP -> P_81 ) { _rtB -> od1mnsxrhg
[ 1 ] = _rtP -> P_81 ; } else { _rtB -> od1mnsxrhg [ 1 ] = m2krcgtgey_idx_0 ;
} if ( _rtB -> nyxb5uqudu [ 6 ] >= _rtP -> P_79 ) { m2krcgtgey_idx_0 =
m2krcgtgey_idx_2 ; } else { m2krcgtgey_idx_0 = fwpbii2lw0 ; } if (
m2krcgtgey_idx_0 > _rtP -> P_80 ) { _rtB -> od1mnsxrhg [ 2 ] = _rtP -> P_80 ;
} else if ( m2krcgtgey_idx_0 < _rtP -> P_81 ) { _rtB -> od1mnsxrhg [ 2 ] =
_rtP -> P_81 ; } else { _rtB -> od1mnsxrhg [ 2 ] = m2krcgtgey_idx_0 ; } if (
_rtB -> nyxb5uqudu [ 7 ] >= _rtP -> P_79 ) { m2krcgtgey_idx_0 =
m2krcgtgey_idx_3 ; } else { m2krcgtgey_idx_0 = fwpbii2lw0 ; } if (
m2krcgtgey_idx_0 > _rtP -> P_80 ) { _rtB -> od1mnsxrhg [ 3 ] = _rtP -> P_80 ;
} else if ( m2krcgtgey_idx_0 < _rtP -> P_81 ) { _rtB -> od1mnsxrhg [ 3 ] =
_rtP -> P_81 ; } else { _rtB -> od1mnsxrhg [ 3 ] = m2krcgtgey_idx_0 ; } if (
_rtB -> dg10ogpa34 [ 0 ] >= _rtP -> P_84 ) { ic3wdyrovx_idx_3 = _rtP -> P_83
* _rtB -> mkzhmubogp [ 0 ] ; } else { ic3wdyrovx_idx_3 = _rtP -> P_82 ; } if
( ic3wdyrovx_idx_3 > _rtP -> P_85 ) { _rtB -> cnklbnp32q [ 0 ] = _rtP -> P_85
; } else if ( ic3wdyrovx_idx_3 < _rtP -> P_86 ) { _rtB -> cnklbnp32q [ 0 ] =
_rtP -> P_86 ; } else { _rtB -> cnklbnp32q [ 0 ] = ic3wdyrovx_idx_3 ; } if (
_rtB -> dg10ogpa34 [ 1 ] >= _rtP -> P_84 ) { ic3wdyrovx_idx_3 = _rtP -> P_83
* _rtB -> mkzhmubogp [ 1 ] ; } else { ic3wdyrovx_idx_3 = _rtP -> P_82 ; } if
( ic3wdyrovx_idx_3 > _rtP -> P_85 ) { _rtB -> cnklbnp32q [ 1 ] = _rtP -> P_85
; } else if ( ic3wdyrovx_idx_3 < _rtP -> P_86 ) { _rtB -> cnklbnp32q [ 1 ] =
_rtP -> P_86 ; } else { _rtB -> cnklbnp32q [ 1 ] = ic3wdyrovx_idx_3 ; } if (
_rtB -> dg10ogpa34 [ 2 ] >= _rtP -> P_84 ) { ic3wdyrovx_idx_3 = _rtP -> P_83
* _rtB -> mkzhmubogp [ 2 ] ; } else { ic3wdyrovx_idx_3 = _rtP -> P_82 ; } if
( ic3wdyrovx_idx_3 > _rtP -> P_85 ) { _rtB -> cnklbnp32q [ 2 ] = _rtP -> P_85
; } else if ( ic3wdyrovx_idx_3 < _rtP -> P_86 ) { _rtB -> cnklbnp32q [ 2 ] =
_rtP -> P_86 ; } else { _rtB -> cnklbnp32q [ 2 ] = ic3wdyrovx_idx_3 ; } if (
_rtB -> dg10ogpa34 [ 3 ] >= _rtP -> P_84 ) { ic3wdyrovx_idx_3 = _rtP -> P_83
* _rtB -> mkzhmubogp [ 3 ] ; } else { ic3wdyrovx_idx_3 = _rtP -> P_82 ; } if
( ic3wdyrovx_idx_3 > _rtP -> P_85 ) { _rtB -> cnklbnp32q [ 3 ] = _rtP -> P_85
; } else if ( ic3wdyrovx_idx_3 < _rtP -> P_86 ) { _rtB -> cnklbnp32q [ 3 ] =
_rtP -> P_86 ; } else { _rtB -> cnklbnp32q [ 3 ] = ic3wdyrovx_idx_3 ; } } if
( ssIsSampleHit ( S , 3 , 0 ) ) { _rtB -> mzarax0zdn = _rtP -> P_87 * _rtB ->
j3w2urmcq2 ; } if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> fryocpnjtr = _rtP
-> P_88 * _rtB -> jpxpl0muvi [ 8 ] ; } ssCallAccelRunBlock ( S , 3 , 90 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB ->
obuznn4bep [ 0 ] = _rtP -> P_89 [ 0 ] ; _rtB -> obuznn4bep [ 1 ] = _rtP ->
P_89 [ 1 ] ; _rtB -> obuznn4bep [ 2 ] = _rtP -> P_89 [ 2 ] ; _rtB ->
obuznn4bep [ 3 ] = _rtP -> P_89 [ 3 ] ; } UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { real_T * lastU ; real_T
tmp ; n3qi1whofz * _rtB ; loikxjbxjg * _rtP ; ew10rzwqr2 * _rtDW ; _rtDW = (
( ew10rzwqr2 * ) ssGetRootDWork ( S ) ) ; _rtP = ( ( loikxjbxjg * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ;
{ real_T * * uBuffer = ( real_T * * ) & _rtDW -> lvful1kguw . TUbufferPtrs [
0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> lvful1kguw .
TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW -> aq5jg3ks0r .
Head = ( ( _rtDW -> aq5jg3ks0r . Head < ( _rtDW -> aq5jg3ks0r .
CircularBufSize - 1 ) ) ? ( _rtDW -> aq5jg3ks0r . Head + 1 ) : 0 ) ; if (
_rtDW -> aq5jg3ks0r . Head == _rtDW -> aq5jg3ks0r . Tail ) { if ( !
Copy_of_single_phase_PVarry_inverter_acc_rt_TDelayUpdateTailOrGrowBuf ( &
_rtDW -> aq5jg3ks0r . CircularBufSize , & _rtDW -> aq5jg3ks0r . Tail , &
_rtDW -> aq5jg3ks0r . Head , & _rtDW -> aq5jg3ks0r . Last , simTime - _rtP ->
P_43 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW ->
aq5jg3ks0r . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
aq5jg3ks0r . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> aq5jg3ks0r . Head ]
= _rtB -> eraw4hoh2g ; } if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtDW ->
baicdkdr1f [ 0 ] = _rtB -> od1mnsxrhg [ 0 ] ; _rtDW -> baicdkdr1f [ 1 ] =
_rtB -> od1mnsxrhg [ 1 ] ; _rtDW -> baicdkdr1f [ 2 ] = _rtB -> od1mnsxrhg [ 2
] ; _rtDW -> baicdkdr1f [ 3 ] = _rtB -> od1mnsxrhg [ 3 ] ; } if ( _rtDW ->
pv0a0wf1ed && ssIsSampleHit ( S , 2 , 0 ) ) { tmp = _rtP -> P_31 * _rtB ->
dojyakp2gd ; _rtDW -> lmvaxbaj01 [ 0 ] += tmp ; _rtDW -> lmvaxbaj01 [ 1 ] +=
tmp ; _rtDW -> lmvaxbaj01 [ 2 ] += tmp ; _rtDW -> lmvaxbaj01 [ 3 ] += tmp ;
if ( _rtB -> dbsx2m0w2l ) { _rtDW -> chvkdrjpen [ 0 ] = 1 ; } else { _rtDW ->
chvkdrjpen [ 0 ] = 0 ; } if ( _rtB -> iamijko5g5 ) { _rtDW -> chvkdrjpen [ 1
] = 1 ; } else { _rtDW -> chvkdrjpen [ 1 ] = 0 ; } if ( _rtB -> b30rvf4zhg )
{ _rtDW -> chvkdrjpen [ 2 ] = 1 ; } else { _rtDW -> chvkdrjpen [ 2 ] = 0 ; }
if ( _rtB -> lm1c1hnyuc ) { _rtDW -> chvkdrjpen [ 3 ] = 1 ; } else { _rtDW ->
chvkdrjpen [ 3 ] = 0 ; } _rtDW -> kz1spygazc [ 0 ] = _rtB -> pndsy5jc4j [ 0 ]
; _rtDW -> kz1spygazc [ 1 ] = _rtB -> pndsy5jc4j [ 1 ] ; _rtDW -> kz1spygazc
[ 2 ] = _rtB -> pndsy5jc4j [ 2 ] ; _rtDW -> kz1spygazc [ 3 ] = _rtB ->
pndsy5jc4j [ 3 ] ; } if ( ssIsSampleHit ( S , 2 , 0 ) ) { ssCallAccelRunBlock
( S , 3 , 26 , SS_CALL_MDL_UPDATE ) ; } if ( _rtDW -> ivzxketgv1 == ( rtInf )
) { _rtDW -> ivzxketgv1 = ssGetT ( S ) ; lastU = & _rtDW -> o1ezyn5fzo ; }
else if ( _rtDW -> a0chmotcqw == ( rtInf ) ) { _rtDW -> a0chmotcqw = ssGetT (
S ) ; lastU = & _rtDW -> ltqpnqteqg ; } else if ( _rtDW -> ivzxketgv1 < _rtDW
-> a0chmotcqw ) { _rtDW -> ivzxketgv1 = ssGetT ( S ) ; lastU = & _rtDW ->
o1ezyn5fzo ; } else { _rtDW -> a0chmotcqw = ssGetT ( S ) ; lastU = & _rtDW ->
ltqpnqteqg ; } * lastU = _rtB -> ot35ovqeag ; if ( ssIsSampleHit ( S , 2 , 0
) ) { _rtDW -> o4e1fsfulk [ 0 ] = _rtB -> cnklbnp32q [ 0 ] ; _rtDW ->
o4e1fsfulk [ 1 ] = _rtB -> cnklbnp32q [ 1 ] ; _rtDW -> o4e1fsfulk [ 2 ] =
_rtB -> cnklbnp32q [ 2 ] ; _rtDW -> o4e1fsfulk [ 3 ] = _rtB -> cnklbnp32q [ 3
] ; } if ( ssIsSampleHit ( S , 3 , 0 ) ) { _rtDW -> otmp0asolx += _rtP ->
P_70 * _rtB -> mzarax0zdn ; if ( _rtDW -> otmp0asolx >= _rtP -> P_72 ) {
_rtDW -> otmp0asolx = _rtP -> P_72 ; } else { if ( _rtDW -> otmp0asolx <=
_rtP -> P_73 ) { _rtDW -> otmp0asolx = _rtP -> P_73 ; } } } if ( _rtDW ->
m3ydbcfsdy && ssIsSampleHit ( S , 2 , 0 ) ) { tmp = _rtP -> P_3 * _rtB ->
ob52ny2ymy ; _rtDW -> clpfe00m2f [ 0 ] += tmp ; _rtDW -> clpfe00m2f [ 1 ] +=
tmp ; _rtDW -> clpfe00m2f [ 2 ] += tmp ; _rtDW -> clpfe00m2f [ 3 ] += tmp ;
if ( _rtB -> fqi4ergijf > 0.0 ) { _rtDW -> dp2r0fu5bm [ 0 ] = 1 ; } else if (
_rtB -> fqi4ergijf < 0.0 ) { _rtDW -> dp2r0fu5bm [ 0 ] = - 1 ; } else if (
_rtB -> fqi4ergijf == 0.0 ) { _rtDW -> dp2r0fu5bm [ 0 ] = 0 ; } else { _rtDW
-> dp2r0fu5bm [ 0 ] = 2 ; } if ( _rtB -> nw0fumydo1 > 0.0 ) { _rtDW ->
dp2r0fu5bm [ 1 ] = 1 ; } else if ( _rtB -> nw0fumydo1 < 0.0 ) { _rtDW ->
dp2r0fu5bm [ 1 ] = - 1 ; } else if ( _rtB -> nw0fumydo1 == 0.0 ) { _rtDW ->
dp2r0fu5bm [ 1 ] = 0 ; } else { _rtDW -> dp2r0fu5bm [ 1 ] = 2 ; } if ( _rtB
-> nw0fumydo1 > 0.0 ) { _rtDW -> dp2r0fu5bm [ 2 ] = 1 ; } else if ( _rtB ->
nw0fumydo1 < 0.0 ) { _rtDW -> dp2r0fu5bm [ 2 ] = - 1 ; } else if ( _rtB ->
nw0fumydo1 == 0.0 ) { _rtDW -> dp2r0fu5bm [ 2 ] = 0 ; } else { _rtDW ->
dp2r0fu5bm [ 2 ] = 2 ; } if ( _rtB -> fqi4ergijf > 0.0 ) { _rtDW ->
dp2r0fu5bm [ 3 ] = 1 ; } else if ( _rtB -> fqi4ergijf < 0.0 ) { _rtDW ->
dp2r0fu5bm [ 3 ] = - 1 ; } else if ( _rtB -> fqi4ergijf == 0.0 ) { _rtDW ->
dp2r0fu5bm [ 3 ] = 0 ; } else { _rtDW -> dp2r0fu5bm [ 3 ] = 2 ; } _rtDW ->
f4matkdx2j [ 0 ] = _rtB -> ajbepahkld [ 0 ] ; _rtDW -> f4matkdx2j [ 1 ] =
_rtB -> ajbepahkld [ 1 ] ; _rtDW -> f4matkdx2j [ 2 ] = _rtB -> ajbepahkld [ 2
] ; _rtDW -> f4matkdx2j [ 3 ] = _rtB -> ajbepahkld [ 3 ] ; } if (
ssIsSampleHit ( S , 2 , 0 ) ) { ssCallAccelRunBlock ( S , 1 , 9 ,
SS_CALL_MDL_UPDATE ) ; } UNUSED_PARAMETER ( tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { n3qi1whofz * _rtB ; loikxjbxjg
* _rtP ; _rtP = ( ( loikxjbxjg * ) ssGetDefaultParam ( S ) ) ; _rtB = ( (
n3qi1whofz * ) _ssGetBlockIO ( S ) ) ; { ( ( pqmvzr1kvu * ) ssGetdX ( S ) )
-> d24x1wviqz = _rtB -> dpgmiyas1i ; } { ( ( pqmvzr1kvu * ) ssGetdX ( S ) )
-> c2mfgg3wdj = _rtB -> i3diaivwnf ; } }
#define MDL_ZERO_CROSSINGS
static void mdlZeroCrossings ( SimStruct * S ) { n3qi1whofz * _rtB ;
loikxjbxjg * _rtP ; d15n1ti4kq * _rtZCSV ; ew10rzwqr2 * _rtDW ; _rtDW = ( (
ew10rzwqr2 * ) ssGetRootDWork ( S ) ) ; _rtZCSV = ( ( d15n1ti4kq * )
ssGetSolverZcSignalVector ( S ) ) ; _rtP = ( ( loikxjbxjg * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ;
_rtZCSV -> luanwi4qfn = _rtB -> p5xbubtydy ; _rtZCSV -> aon3wdbxsm = _rtB ->
n0c3qawger - _rtB -> fa5fdcf00w ; _rtZCSV -> gm0gn4ujsh = _rtB -> ifeybmu5cv
- _rtB -> ih14wg2u1o ; _rtZCSV -> k3hd0i2tcn = _rtB -> ojbxxbpc2o - _rtP ->
P_63 ; _rtZCSV -> oqpfvy1dny = _rtB -> ojbxxbpc2o - _rtP -> P_64 ; _rtZCSV ->
iprs514q1y = _rtB -> oe4tea5444 - _rtP -> P_17 ; _rtZCSV -> k0e0aevr3u = _rtB
-> oe4tea5444 - _rtP -> P_18 ; if ( _rtDW -> abk5tfl0eh ) { _rtZCSV ->
m2nefpt2nd = _rtB -> akgbpr51hg - _rtP -> P_20 ; } else { _rtZCSV ->
m2nefpt2nd = _rtB -> akgbpr51hg - _rtP -> P_19 ; } } static void
mdlInitializeSizes ( SimStruct * S ) { ssSetChecksumVal ( S , 0 , 63040129U )
; ssSetChecksumVal ( S , 1 , 3818416564U ) ; ssSetChecksumVal ( S , 2 ,
4106472127U ) ; ssSetChecksumVal ( S , 3 , 2342458752U ) ; { mxArray *
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
loikxjbxjg * ) ssGetDefaultParam ( S ) ) -> P_80 = rtInf ; ( ( loikxjbxjg * )
ssGetDefaultParam ( S ) ) -> P_85 = rtInf ; } static void
mdlInitializeSampleTimes ( SimStruct * S ) { } static void mdlTerminate (
SimStruct * S ) { }
#include "simulink.c"
