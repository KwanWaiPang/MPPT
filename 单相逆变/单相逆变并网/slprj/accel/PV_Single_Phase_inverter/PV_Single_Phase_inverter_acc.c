#include "__cf_PV_Single_Phase_inverter.h"
#include <math.h>
#include "PV_Single_Phase_inverter_acc.h"
#include "PV_Single_Phase_inverter_acc_private.h"
#include <stdio.h>
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T PV_Single_Phase_inverter_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T *
bufSzPtr , int_T * tailPtr , int_T * headPtr , int_T * lastPtr , real_T
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
return ( true ) ; } real_T PV_Single_Phase_inverter_acc_rt_TDelayInterpolate
( real_T tMinusDelay , real_T tStart , real_T * tBuf , real_T * uBuf , int_T
bufSz , int_T * lastIdx , int_T oldestIdx , int_T newIdx , real_T initOutput
, boolean_T discrete , boolean_T minorStepAndTAtLastMajorOutput ) { int_T i ;
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
mdlOutputs ( SimStruct * S , int_T tid ) { real_T lvy0y3atfy ; real_T
jp5ua23i4a ; real_T ckqv2ilyre ; real_T fmbjzdfypj ; real_T hwjiit4ixv ;
real_T m41w1amzyn ; real_T hvhzmisfej ; real_T eintqa3vub ; real_T daqejettql
; boolean_T ckeb1s2dlz ; boolean_T hitkasikyj ; real_T oossyunrst ; real_T
eo3w5kdmqj ; real_T jsxxec1j5w ; real_T nyuda2vtrc ; real_T cvxn31ydno ;
real_T anh5zrqjrh ; real_T gqbesqi422 ; real_T bpjo4wesci_idx_0 ; real_T
bpjo4wesci_idx_1 ; real_T bpjo4wesci_idx_2 ; real_T bpjo4wesci_idx_3 ; real_T
u0 ; n3qi1whofz * _rtB ; loikxjbxjg * _rtP ; f1xhd02yjc * _rtX ; ew10rzwqr2 *
_rtDW ; _rtDW = ( ( ew10rzwqr2 * ) ssGetRootDWork ( S ) ) ; _rtX = ( (
f1xhd02yjc * ) ssGetContStates ( S ) ) ; _rtP = ( ( loikxjbxjg * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ;
if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> mtebedot43 [ 0 ] = _rtP -> P_44
[ 0 ] ; _rtB -> mtebedot43 [ 1 ] = _rtP -> P_44 [ 1 ] ; _rtB -> mtebedot43 [
2 ] = _rtP -> P_44 [ 2 ] ; _rtB -> k0eoach3d0 [ 0 ] = _rtDW -> pd3luz4avz [ 0
] ; _rtB -> k0eoach3d0 [ 1 ] = _rtDW -> pd3luz4avz [ 1 ] ; _rtB -> k0eoach3d0
[ 2 ] = _rtDW -> pd3luz4avz [ 2 ] ; _rtB -> k0eoach3d0 [ 3 ] = _rtDW ->
pd3luz4avz [ 3 ] ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> hpvnminlgh
[ 0 ] = _rtDW -> b2gcy3vb2a [ 0 ] ; _rtB -> hpvnminlgh [ 1 ] = _rtDW ->
b2gcy3vb2a [ 1 ] ; _rtB -> hpvnminlgh [ 2 ] = _rtDW -> b2gcy3vb2a [ 2 ] ;
_rtB -> hpvnminlgh [ 3 ] = _rtDW -> b2gcy3vb2a [ 3 ] ; _rtB -> is1k135npl =
_rtP -> P_172 ; if ( ssIsMajorTimeStep ( S ) ) { if ( _rtB -> is1k135npl ) {
if ( ! _rtDW -> byh0btktux ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart (
S ) ) { ssSetSolverNeedsReset ( S ) ; } _rtDW -> l4dlj3krq0 [ 0 ] = _rtP ->
P_33 ; _rtDW -> l4dlj3krq0 [ 1 ] = _rtP -> P_33 ; _rtDW -> l4dlj3krq0 [ 2 ] =
_rtP -> P_33 ; _rtDW -> l4dlj3krq0 [ 3 ] = _rtP -> P_33 ; _rtDW -> b13awnigwz
[ 0 ] = 2 ; _rtDW -> b13awnigwz [ 1 ] = 2 ; _rtDW -> b13awnigwz [ 2 ] = 2 ;
_rtDW -> b13awnigwz [ 3 ] = 2 ; _rtDW -> bxpfaqqn5g [ 0 ] = _rtP -> P_42 ;
_rtDW -> bxpfaqqn5g [ 1 ] = _rtP -> P_42 ; _rtDW -> bxpfaqqn5g [ 2 ] = _rtP
-> P_42 ; _rtDW -> bxpfaqqn5g [ 3 ] = _rtP -> P_42 ; _rtDW -> byh0btktux =
true ; } } else { if ( _rtDW -> byh0btktux ) { ssSetSolverNeedsReset ( S ) ;
_rtDW -> byh0btktux = false ; } } } } if ( _rtDW -> byh0btktux ) { if (
ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> ecey0q2vme = _rtP -> P_30 ; } if (
ssIsSampleHit ( S , 1 , 0 ) ) { eo3w5kdmqj = _rtP -> P_31 ; } if (
ssIsSampleHit ( S , 2 , 0 ) ) { if ( ( _rtB -> hpvnminlgh [ 0 ] <= 0.0 ) && (
_rtDW -> b13awnigwz [ 0 ] == 1 ) ) { _rtDW -> l4dlj3krq0 [ 0 ] = _rtP -> P_33
; } if ( ( _rtB -> hpvnminlgh [ 1 ] <= 0.0 ) && ( _rtDW -> b13awnigwz [ 1 ]
== 1 ) ) { _rtDW -> l4dlj3krq0 [ 1 ] = _rtP -> P_33 ; } if ( ( _rtB ->
hpvnminlgh [ 2 ] <= 0.0 ) && ( _rtDW -> b13awnigwz [ 2 ] == 1 ) ) { _rtDW ->
l4dlj3krq0 [ 2 ] = _rtP -> P_33 ; } if ( ( _rtB -> hpvnminlgh [ 3 ] <= 0.0 )
&& ( _rtDW -> b13awnigwz [ 3 ] == 1 ) ) { _rtDW -> l4dlj3krq0 [ 3 ] = _rtP ->
P_33 ; } _rtB -> igl1rmdosy [ 0 ] = _rtDW -> l4dlj3krq0 [ 0 ] ; _rtB ->
igl1rmdosy [ 1 ] = _rtDW -> l4dlj3krq0 [ 1 ] ; _rtB -> igl1rmdosy [ 2 ] =
_rtDW -> l4dlj3krq0 [ 2 ] ; _rtB -> igl1rmdosy [ 3 ] = _rtDW -> l4dlj3krq0 [
3 ] ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { jsxxec1j5w = _rtP -> P_34 ; } if
( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> hceqogjf5t [ 0 ] = _rtP -> P_35 *
_rtB -> igl1rmdosy [ 0 ] ; _rtB -> hceqogjf5t [ 1 ] = _rtP -> P_35 * _rtB ->
igl1rmdosy [ 1 ] ; _rtB -> hceqogjf5t [ 2 ] = _rtP -> P_35 * _rtB ->
igl1rmdosy [ 2 ] ; _rtB -> hceqogjf5t [ 3 ] = _rtP -> P_35 * _rtB ->
igl1rmdosy [ 3 ] ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { bpjo4wesci_idx_3 =
jsxxec1j5w + _rtB -> hceqogjf5t [ 0 ] ; u0 = ( _rtP -> P_38 - _rtB ->
igl1rmdosy [ 0 ] ) * _rtP -> P_39 ; if ( bpjo4wesci_idx_3 > _rtP -> P_36 ) {
bpjo4wesci_idx_3 = _rtP -> P_36 ; } else { if ( bpjo4wesci_idx_3 < _rtP ->
P_37 ) { bpjo4wesci_idx_3 = _rtP -> P_37 ; } } if ( u0 > _rtP -> P_40 ) { u0
= _rtP -> P_40 ; } else { if ( u0 < _rtP -> P_41 ) { u0 = _rtP -> P_41 ; } }
bpjo4wesci_idx_0 = bpjo4wesci_idx_3 + u0 ; bpjo4wesci_idx_3 = jsxxec1j5w +
_rtB -> hceqogjf5t [ 1 ] ; u0 = ( _rtP -> P_38 - _rtB -> igl1rmdosy [ 1 ] ) *
_rtP -> P_39 ; if ( bpjo4wesci_idx_3 > _rtP -> P_36 ) { bpjo4wesci_idx_3 =
_rtP -> P_36 ; } else { if ( bpjo4wesci_idx_3 < _rtP -> P_37 ) {
bpjo4wesci_idx_3 = _rtP -> P_37 ; } } if ( u0 > _rtP -> P_40 ) { u0 = _rtP ->
P_40 ; } else { if ( u0 < _rtP -> P_41 ) { u0 = _rtP -> P_41 ; } }
bpjo4wesci_idx_1 = bpjo4wesci_idx_3 + u0 ; bpjo4wesci_idx_3 = jsxxec1j5w +
_rtB -> hceqogjf5t [ 2 ] ; u0 = ( _rtP -> P_38 - _rtB -> igl1rmdosy [ 2 ] ) *
_rtP -> P_39 ; if ( bpjo4wesci_idx_3 > _rtP -> P_36 ) { bpjo4wesci_idx_3 =
_rtP -> P_36 ; } else { if ( bpjo4wesci_idx_3 < _rtP -> P_37 ) {
bpjo4wesci_idx_3 = _rtP -> P_37 ; } } if ( u0 > _rtP -> P_40 ) { u0 = _rtP ->
P_40 ; } else { if ( u0 < _rtP -> P_41 ) { u0 = _rtP -> P_41 ; } }
bpjo4wesci_idx_2 = bpjo4wesci_idx_3 + u0 ; bpjo4wesci_idx_3 = jsxxec1j5w +
_rtB -> hceqogjf5t [ 3 ] ; u0 = ( _rtP -> P_38 - _rtB -> igl1rmdosy [ 3 ] ) *
_rtP -> P_39 ; if ( bpjo4wesci_idx_3 > _rtP -> P_36 ) { bpjo4wesci_idx_3 =
_rtP -> P_36 ; } else { if ( bpjo4wesci_idx_3 < _rtP -> P_37 ) {
bpjo4wesci_idx_3 = _rtP -> P_37 ; } } if ( u0 > _rtP -> P_40 ) { u0 = _rtP ->
P_40 ; } else { if ( u0 < _rtP -> P_41 ) { u0 = _rtP -> P_41 ; } }
bpjo4wesci_idx_3 += u0 ; } if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB ->
adlprvfww0 [ 0 ] = _rtDW -> bxpfaqqn5g [ 0 ] ; _rtB -> adlprvfww0 [ 1 ] =
_rtDW -> bxpfaqqn5g [ 1 ] ; _rtB -> adlprvfww0 [ 2 ] = _rtDW -> bxpfaqqn5g [
2 ] ; _rtB -> adlprvfww0 [ 3 ] = _rtDW -> bxpfaqqn5g [ 3 ] ; } if (
ssIsSampleHit ( S , 1 , 0 ) ) { if ( _rtB -> hpvnminlgh [ 0 ] >= _rtP -> P_43
) { _rtB -> peonb2s13j [ 0 ] = _rtB -> k0eoach3d0 [ 0 ] ; } else { _rtB ->
peonb2s13j [ 0 ] = _rtB -> adlprvfww0 [ 0 ] ; } if ( _rtB -> hpvnminlgh [ 1 ]
>= _rtP -> P_43 ) { _rtB -> peonb2s13j [ 1 ] = _rtB -> k0eoach3d0 [ 1 ] ; }
else { _rtB -> peonb2s13j [ 1 ] = _rtB -> adlprvfww0 [ 1 ] ; } if ( _rtB ->
hpvnminlgh [ 2 ] >= _rtP -> P_43 ) { _rtB -> peonb2s13j [ 2 ] = _rtB ->
k0eoach3d0 [ 2 ] ; } else { _rtB -> peonb2s13j [ 2 ] = _rtB -> adlprvfww0 [ 2
] ; } if ( _rtB -> hpvnminlgh [ 3 ] >= _rtP -> P_43 ) { _rtB -> peonb2s13j [
3 ] = _rtB -> k0eoach3d0 [ 3 ] ; } else { _rtB -> peonb2s13j [ 3 ] = _rtB ->
adlprvfww0 [ 3 ] ; } _rtB -> auwitbs4pp [ 0 ] = bpjo4wesci_idx_0 * _rtB ->
peonb2s13j [ 0 ] * eo3w5kdmqj ; _rtB -> auwitbs4pp [ 1 ] = bpjo4wesci_idx_1 *
_rtB -> peonb2s13j [ 1 ] * eo3w5kdmqj ; _rtB -> auwitbs4pp [ 2 ] =
bpjo4wesci_idx_2 * _rtB -> peonb2s13j [ 2 ] * eo3w5kdmqj ; _rtB -> auwitbs4pp
[ 3 ] = bpjo4wesci_idx_3 * _rtB -> peonb2s13j [ 3 ] * eo3w5kdmqj ; } if (
ssIsMajorTimeStep ( S ) ) { srUpdateBC ( _rtDW -> cvc2xojcfc ) ; } } if (
ssIsSampleHit ( S , 2 , 0 ) ) { if ( _rtDW -> fueor4l32w != 0 ) { _rtDW ->
gzppta2yd5 = muDoubleScalarSin ( _rtP -> P_49 * ssGetTaskTime ( S , 2 ) ) ;
_rtDW -> h4dq2buhxq = muDoubleScalarCos ( _rtP -> P_49 * ssGetTaskTime ( S ,
2 ) ) ; _rtDW -> fueor4l32w = 0 ; } _rtB -> fh3kyg4a5x = ( ( _rtDW ->
gzppta2yd5 * _rtP -> P_53 + _rtDW -> h4dq2buhxq * _rtP -> P_52 ) * _rtP ->
P_51 + ( _rtDW -> h4dq2buhxq * _rtP -> P_53 - _rtDW -> gzppta2yd5 * _rtP ->
P_52 ) * _rtP -> P_50 ) * _rtP -> P_47 + _rtP -> P_48 ; bpjo4wesci_idx_3 =
_rtP -> P_54 / _rtP -> P_55 ; eo3w5kdmqj = bpjo4wesci_idx_3 - _rtP -> P_56 ;
jp5ua23i4a = _rtP -> P_56 - _rtP -> P_57 / _rtP -> P_58 ; jsxxec1j5w =
jp5ua23i4a ; if ( ssIsMajorTimeStep ( S ) ) { if ( _rtDW -> ccnsfwllyk != 0 )
{ ssSetSolverNeedsReset ( S ) ; _rtDW -> ccnsfwllyk = 0 ; } } else { if (
jp5ua23i4a < 0.0 ) { jsxxec1j5w = 0.0 ; _rtDW -> ccnsfwllyk = 1 ; } }
eo3w5kdmqj /= muDoubleScalarLog ( jsxxec1j5w ) ; _rtB -> akemzmcvei = 1.0 /
muDoubleScalarExp ( bpjo4wesci_idx_3 / eo3w5kdmqj ) * jp5ua23i4a ; _rtB ->
beonv2fsee = eo3w5kdmqj * _rtP -> P_59 ; bpjo4wesci_idx_3 = _rtP -> P_63 /
_rtP -> P_64 ; jp5ua23i4a = _rtP -> P_67 - _rtP -> P_68 ; _rtB -> mqoejox33w
= _rtP -> P_66 * _rtP -> P_65 * bpjo4wesci_idx_3 * jp5ua23i4a + (
bpjo4wesci_idx_3 - _rtP -> P_62 ) * _rtP -> P_61 ; _rtB -> ipu5qysy5u = ( 0.0
- _rtP -> P_60 * _rtB -> mqoejox33w ) - _rtP -> P_69 * jp5ua23i4a ; _rtB ->
esubiw2vje = _rtP -> P_70 ; _rtB -> kc0aufcser = _rtP -> P_71 ; _rtB ->
ptix3hif0l = _rtP -> P_72 ; ssCallAccelRunBlock ( S , 3 , 46 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> p11ibpnyrl = _rtP -> P_73 * _rtB ->
at4svfakyj [ 9 ] ; _rtB -> cf1glnoy5m = _rtP -> P_74 * _rtB -> at4svfakyj [
12 ] ; _rtB -> o0rsqrdpoy = _rtP -> P_75 * _rtB -> cf1glnoy5m ;
ssCallAccelRunBlock ( S , 3 , 50 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> ivri3xnnra
= _rtP -> P_76 * _rtB -> at4svfakyj [ 11 ] ; _rtB -> j0xmonptfz = _rtB ->
gtvmdf51i0 * _rtB -> ivri3xnnra ; ssCallAccelRunBlock ( S , 3 , 53 ,
SS_CALL_MDL_OUTPUTS ) ; jp5ua23i4a = _rtP -> P_77 ; if ( _rtB -> gfa3kvzw15 [
3 ] >= _rtP -> P_79 ) { bpjo4wesci_idx_3 = _rtP -> P_78 * _rtB -> at4svfakyj
[ 3 ] ; } else { bpjo4wesci_idx_3 = _rtP -> P_77 ; } if ( bpjo4wesci_idx_3 >
_rtP -> P_80 ) { _rtB -> itxnx2wsi5 [ 0 ] = _rtP -> P_80 ; } else if (
bpjo4wesci_idx_3 < _rtP -> P_81 ) { _rtB -> itxnx2wsi5 [ 0 ] = _rtP -> P_81 ;
} else { _rtB -> itxnx2wsi5 [ 0 ] = bpjo4wesci_idx_3 ; } if ( _rtB ->
gfa3kvzw15 [ 4 ] >= _rtP -> P_79 ) { bpjo4wesci_idx_3 = _rtP -> P_78 * _rtB
-> at4svfakyj [ 4 ] ; } else { bpjo4wesci_idx_3 = _rtP -> P_77 ; } if (
bpjo4wesci_idx_3 > _rtP -> P_80 ) { _rtB -> itxnx2wsi5 [ 1 ] = _rtP -> P_80 ;
} else if ( bpjo4wesci_idx_3 < _rtP -> P_81 ) { _rtB -> itxnx2wsi5 [ 1 ] =
_rtP -> P_81 ; } else { _rtB -> itxnx2wsi5 [ 1 ] = bpjo4wesci_idx_3 ; } if (
_rtB -> gfa3kvzw15 [ 5 ] >= _rtP -> P_79 ) { bpjo4wesci_idx_3 = _rtP -> P_78
* _rtB -> at4svfakyj [ 5 ] ; } else { bpjo4wesci_idx_3 = _rtP -> P_77 ; } if
( bpjo4wesci_idx_3 > _rtP -> P_80 ) { _rtB -> itxnx2wsi5 [ 2 ] = _rtP -> P_80
; } else if ( bpjo4wesci_idx_3 < _rtP -> P_81 ) { _rtB -> itxnx2wsi5 [ 2 ] =
_rtP -> P_81 ; } else { _rtB -> itxnx2wsi5 [ 2 ] = bpjo4wesci_idx_3 ; } if (
_rtB -> gfa3kvzw15 [ 6 ] >= _rtP -> P_79 ) { bpjo4wesci_idx_3 = _rtP -> P_78
* _rtB -> at4svfakyj [ 6 ] ; } else { bpjo4wesci_idx_3 = _rtP -> P_77 ; } if
( bpjo4wesci_idx_3 > _rtP -> P_80 ) { _rtB -> itxnx2wsi5 [ 3 ] = _rtP -> P_80
; } else if ( bpjo4wesci_idx_3 < _rtP -> P_81 ) { _rtB -> itxnx2wsi5 [ 3 ] =
_rtP -> P_81 ; } else { _rtB -> itxnx2wsi5 [ 3 ] = bpjo4wesci_idx_3 ; } } if
( ssIsSampleHit ( S , 1 , 0 ) ) { ckqv2ilyre = _rtP -> P_82 ; } if (
ssIsSampleHit ( S , 4 , 0 ) ) { _rtB -> bgmzip30xp = _rtB -> cf1glnoy5m ; } {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> bxxpgfrdrd . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> bxxpgfrdrd . TUbufferPtrs
[ 1 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP
-> P_83 ; lvy0y3atfy = PV_Single_Phase_inverter_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * tBuffer , * uBuffer , _rtDW -> cmbq4gun1g .
CircularBufSize , & _rtDW -> cmbq4gun1g . Last , _rtDW -> cmbq4gun1g . Tail ,
_rtDW -> cmbq4gun1g . Head , _rtP -> P_84 , 1 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } if ( ssIsSampleHit ( S , 4 , 0 ) ) { _rtB -> awrurg1zmy = _rtDW ->
lkjsqizslz ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { fmbjzdfypj =
muDoubleScalarMod ( _rtB -> awrurg1zmy , _rtP -> P_87 ) ; _rtB -> h4fxaiqvyl
= muDoubleScalarSin ( fmbjzdfypj ) ; } _rtB -> euvpfhvqhz = lvy0y3atfy * _rtB
-> h4fxaiqvyl - _rtB -> bgmzip30xp ; if ( ssIsSampleHit ( S , 2 , 0 ) ) {
_rtB -> akw04q0asa = _rtP -> P_88 * _rtB -> euvpfhvqhz ; _rtDW -> jdlga5gylp
= ( _rtB -> akw04q0asa - _rtP -> P_90 [ 1 ] * _rtDW -> dxwkuh3yet ) / _rtP ->
P_90 [ 0 ] ; jp5ua23i4a = _rtP -> P_89 [ 0 ] * _rtDW -> jdlga5gylp + _rtP ->
P_89 [ 1 ] * _rtDW -> dxwkuh3yet ; _rtB -> fmfmahjizi = _rtP -> P_97 * _rtB
-> euvpfhvqhz ; hwjiit4ixv = ( _rtP -> P_96 * _rtB -> euvpfhvqhz + _rtDW ->
b3cy3g4cyx ) + jp5ua23i4a ; if ( hwjiit4ixv > _rtP -> P_98 ) { hwjiit4ixv =
_rtP -> P_98 ; } else { if ( hwjiit4ixv < _rtP -> P_99 ) { hwjiit4ixv = _rtP
-> P_99 ; } } } if ( ssIsSampleHit ( S , 4 , 0 ) ) { _rtB -> kcaisrgvjm =
_rtB -> p11ibpnyrl ; } if ( ssIsSampleHit ( S , 2 , 0 ) ) { jp5ua23i4a = (
_rtB -> kcaisrgvjm + hwjiit4ixv ) * _rtP -> P_100 ; } if ( ssIsSampleHit ( S
, 1 , 0 ) ) { _rtB -> kb3yih1xdi = _rtB -> j0xmonptfz / ckqv2ilyre ; } if (
ssIsSampleHit ( S , 4 , 0 ) ) { _rtB -> kduibceqxq = _rtP -> P_101 * _rtB ->
p11ibpnyrl ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { m41w1amzyn =
muDoubleScalarCos ( fmbjzdfypj ) ; _rtB -> knnrqhsfui = _rtB -> kduibceqxq *
m41w1amzyn ; } if ( ssIsSampleHit ( S , 4 , 0 ) ) { if ( _rtDW -> p0mjtkssxb
!= 0 ) { _rtB -> gsb54uku3l = _rtDW -> fv1uvl41xo ; } else { _rtB ->
gsb54uku3l = _rtP -> P_102 * _rtB -> knnrqhsfui + _rtDW -> fv1uvl41xo ; }
hvhzmisfej = _rtDW -> bgv2ulrz1x ; if ( _rtDW -> bgv2ulrz1x > _rtP -> P_105 )
{ bpjo4wesci_idx_3 = _rtP -> P_105 ; } else if ( _rtDW -> bgv2ulrz1x < _rtP
-> P_106 ) { bpjo4wesci_idx_3 = _rtP -> P_106 ; } else { bpjo4wesci_idx_3 =
_rtDW -> bgv2ulrz1x ; } eo3w5kdmqj = 1.0 / bpjo4wesci_idx_3 / 0.0001 ;
jsxxec1j5w = muDoubleScalarCeil ( eo3w5kdmqj ) ; _rtB -> huwt4a5rbi = _rtP ->
P_107 * jsxxec1j5w ; ssCallAccelRunBlock ( S , 3 , 108 , SS_CALL_MDL_OUTPUTS
) ; _rtB -> lzaxcymkxk = ( _rtB -> gsb54uku3l - _rtB -> fxmrh3cchn ) * _rtDW
-> bgv2ulrz1x ; _rtB -> eexthrg0py = eo3w5kdmqj - jsxxec1j5w ; _rtB ->
heihunb43x = _rtB -> eexthrg0py / eo3w5kdmqj ; _rtB -> olyyvkfew4 = _rtDW ->
mo5i5rvr5i ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { nyuda2vtrc = ( ( _rtB ->
knnrqhsfui - _rtB -> olyyvkfew4 ) * _rtB -> eexthrg0py * _rtP -> P_109 + _rtB
-> knnrqhsfui ) * _rtB -> heihunb43x + _rtB -> lzaxcymkxk ; } if (
ssIsSampleHit ( S , 4 , 0 ) ) { if ( ssGetTaskTime ( S , 4 ) < _rtP -> P_110
) { _rtB -> f10mw4wrfz = _rtP -> P_111 ; } else { _rtB -> f10mw4wrfz = _rtP
-> P_112 ; } _rtB -> onuoiaanvo = _rtP -> P_113 ; } if ( ssIsSampleHit ( S ,
1 , 0 ) ) { if ( _rtB -> f10mw4wrfz >= _rtP -> P_114 ) { fmbjzdfypj =
nyuda2vtrc ; } else { fmbjzdfypj = _rtB -> onuoiaanvo ; } _rtB -> d5qx1wyeik
= _rtP -> P_115 ; if ( ssIsMajorTimeStep ( S ) ) { if ( _rtB -> d5qx1wyeik >
0.0 ) { if ( ! _rtDW -> m1v05clsvv ) { if ( ssGetTaskTime ( S , 1 ) !=
ssGetTStart ( S ) ) { ssSetSolverNeedsReset ( S ) ; } _rtDW -> i5rjtcs5sd =
1U ; _rtDW -> j1qqayjy1b = 1U ; _rtDW -> m1v05clsvv = true ; } } else { if (
_rtDW -> m1v05clsvv ) { ssSetSolverNeedsReset ( S ) ; _rtDW -> m1v05clsvv =
false ; } } } } if ( _rtDW -> m1v05clsvv ) { if ( ssIsSampleHit ( S , 1 , 0 )
) { _rtB -> jkovacda1u [ 0 ] = _rtP -> P_2 * _rtB -> h4fxaiqvyl * _rtB ->
kduibceqxq ; _rtB -> jkovacda1u [ 1 ] = _rtP -> P_2 * m41w1amzyn * _rtB ->
kduibceqxq ; } if ( ssIsSampleHit ( S , 4 , 0 ) ) { if ( _rtDW -> i5rjtcs5sd
!= 0 ) { _rtB -> ptmdu1k3aj = _rtDW -> hbwmzyylnb ; } else { _rtB ->
ptmdu1k3aj = _rtP -> P_3 * _rtB -> jkovacda1u [ 0 ] + _rtDW -> hbwmzyylnb ; }
if ( hvhzmisfej > _rtP -> P_5 ) { bpjo4wesci_idx_3 = _rtP -> P_5 ; } else if
( hvhzmisfej < _rtP -> P_6 ) { bpjo4wesci_idx_3 = _rtP -> P_6 ; } else {
bpjo4wesci_idx_3 = hvhzmisfej ; } eo3w5kdmqj = 1.0 / bpjo4wesci_idx_3 /
0.0001 ; jsxxec1j5w = muDoubleScalarCeil ( eo3w5kdmqj ) ; _rtB -> ojmqkmcuoj
= _rtP -> P_7 * jsxxec1j5w ; ssCallAccelRunBlock ( S , 1 , 7 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> bq3d3y3bcr = ( _rtB -> ptmdu1k3aj - _rtB ->
keckzqo4yo ) * hvhzmisfej ; _rtB -> no5ogczi4y = eo3w5kdmqj - jsxxec1j5w ;
_rtB -> pydroeurl5 = _rtB -> no5ogczi4y / eo3w5kdmqj ; _rtB -> gcnlpfbb30 =
_rtDW -> es20fy2lvr ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { anh5zrqjrh = ( (
_rtB -> jkovacda1u [ 0 ] - _rtB -> gcnlpfbb30 ) * _rtB -> no5ogczi4y * _rtP
-> P_9 + _rtB -> jkovacda1u [ 0 ] ) * _rtB -> pydroeurl5 + _rtB -> bq3d3y3bcr
; } if ( ssIsSampleHit ( S , 4 , 0 ) ) { if ( ssGetTaskTime ( S , 4 ) < _rtP
-> P_10 ) { _rtB -> akkevkcjwq = _rtP -> P_11 ; } else { _rtB -> akkevkcjwq =
_rtP -> P_12 ; } _rtB -> aexzz4isvj = _rtP -> P_13 ; } if ( ssIsSampleHit ( S
, 1 , 0 ) ) { if ( _rtB -> akkevkcjwq >= _rtP -> P_14 ) { cvxn31ydno =
anh5zrqjrh ; } else { cvxn31ydno = _rtB -> aexzz4isvj ; } } if (
ssIsSampleHit ( S , 4 , 0 ) ) { if ( _rtDW -> j1qqayjy1b != 0 ) { _rtB ->
np3w5lg3ur = _rtDW -> ltialakutr ; } else { _rtB -> np3w5lg3ur = _rtP -> P_15
* _rtB -> jkovacda1u [ 1 ] + _rtDW -> ltialakutr ; } if ( hvhzmisfej > _rtP
-> P_17 ) { bpjo4wesci_idx_3 = _rtP -> P_17 ; } else if ( hvhzmisfej < _rtP
-> P_18 ) { bpjo4wesci_idx_3 = _rtP -> P_18 ; } else { bpjo4wesci_idx_3 =
hvhzmisfej ; } jsxxec1j5w = 1.0 / bpjo4wesci_idx_3 / 0.0001 ; eo3w5kdmqj =
muDoubleScalarCeil ( jsxxec1j5w ) ; _rtB -> ogknms4cd3 = _rtP -> P_19 *
eo3w5kdmqj ; ssCallAccelRunBlock ( S , 1 , 27 , SS_CALL_MDL_OUTPUTS ) ; _rtB
-> nfazloyrkv = ( _rtB -> np3w5lg3ur - _rtB -> fu1dru4srm ) * hvhzmisfej ;
_rtB -> jyqcip0a0k = jsxxec1j5w - eo3w5kdmqj ; _rtB -> gxcqbrikiy = _rtB ->
jyqcip0a0k / jsxxec1j5w ; _rtB -> ngki0owl2f = _rtDW -> e3hcogjryj ; } if (
ssIsSampleHit ( S , 1 , 0 ) ) { gqbesqi422 = ( ( _rtB -> jkovacda1u [ 1 ] -
_rtB -> ngki0owl2f ) * _rtB -> jyqcip0a0k * _rtP -> P_21 + _rtB -> jkovacda1u
[ 1 ] ) * _rtB -> gxcqbrikiy + _rtB -> nfazloyrkv ; } if ( ssIsSampleHit ( S
, 4 , 0 ) ) { if ( ssGetTaskTime ( S , 4 ) < _rtP -> P_22 ) { _rtB ->
gl5k23gezs = _rtP -> P_23 ; } else { _rtB -> gl5k23gezs = _rtP -> P_24 ; }
_rtB -> iltfgaylem = _rtP -> P_25 ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { if
( ! ( _rtB -> gl5k23gezs >= _rtP -> P_26 ) ) { gqbesqi422 = _rtB ->
iltfgaylem ; } cvxn31ydno = muDoubleScalarHypot ( cvxn31ydno , gqbesqi422 ) ;
if ( cvxn31ydno > _rtP -> P_27 ) { cvxn31ydno = _rtP -> P_27 ; } else { if (
cvxn31ydno < _rtP -> P_28 ) { cvxn31ydno = _rtP -> P_28 ; } } _rtB ->
ku3yiw1fh4 = 1.0 / cvxn31ydno ; } if ( ssIsMajorTimeStep ( S ) ) { srUpdateBC
( _rtDW -> bzrv5d0tmm ) ; } } if ( ssIsSampleHit ( S , 1 , 0 ) ) { fmbjzdfypj
*= _rtB -> ku3yiw1fh4 ; eintqa3vub = _rtP -> P_116 * fmbjzdfypj ; } if (
ssIsSampleHit ( S , 4 , 0 ) ) { _rtB -> loemvl42qz = _rtDW -> n1uqml3zbu ; }
if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> ow11ibemug = _rtP -> P_121 *
fmbjzdfypj ; } if ( ssIsSampleHit ( S , 4 , 0 ) ) { _rtB -> gwwpugbml0 = _rtB
-> ow11ibemug * _rtP -> P_122 ; _rtB -> ae1nyib4nh = _rtB -> gwwpugbml0 -
_rtDW -> j1gf540g2s ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { bpjo4wesci_idx_3
= ( eintqa3vub + _rtB -> loemvl42qz ) + _rtB -> ae1nyib4nh ; if (
bpjo4wesci_idx_3 > _rtP -> P_124 ) { _rtB -> htjmcyxm4o = _rtP -> P_124 ; }
else if ( bpjo4wesci_idx_3 < _rtP -> P_125 ) { _rtB -> htjmcyxm4o = _rtP ->
P_125 ; } else { _rtB -> htjmcyxm4o = bpjo4wesci_idx_3 ; } daqejettql = _rtP
-> P_126 * _rtB -> htjmcyxm4o ; } if ( ssIsSampleHit ( S , 4 , 0 ) ) { _rtB
-> ilhtrsad2u = _rtDW -> j0jutlrn4j ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) {
bpjo4wesci_idx_3 = daqejettql - _rtB -> ilhtrsad2u ; if ( bpjo4wesci_idx_3 >
_rtP -> P_128 ) { bpjo4wesci_idx_3 = _rtP -> P_128 ; } else { if (
bpjo4wesci_idx_3 < _rtP -> P_129 ) { bpjo4wesci_idx_3 = _rtP -> P_129 ; } }
_rtB -> bzzw2dv1ny = bpjo4wesci_idx_3 + _rtB -> ilhtrsad2u ; } if (
ssIsSampleHit ( S , 4 , 0 ) ) { { _rtB -> mq00slxbar = ( _rtP -> P_132 [ 0 ]
) * _rtDW -> opbvzhddhh [ 0 ] + ( _rtP -> P_132 [ 1 ] ) * _rtDW -> opbvzhddhh
[ 1 ] ; _rtB -> mq00slxbar += _rtP -> P_133 * _rtB -> bzzw2dv1ny ; } } if (
ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> fhncp1qyw1 = _rtP -> P_135 *
fmbjzdfypj ; _rtB -> muxovlbgtv = _rtP -> P_136 ; } lvy0y3atfy = ssGetT ( S )
; if ( ssIsSampleHit ( S , 1 , 0 ) ) { fmbjzdfypj = _rtP -> P_138 ; _rtB ->
eiounl1kd2 = _rtP -> P_139 * _rtP -> P_138 ; _rtB -> howfzjg0z4 = _rtP ->
P_140 ; _rtB -> fqgar3z0da = _rtP -> P_141 ; } if ( ssIsSampleHit ( S , 2 , 0
) ) { if ( jp5ua23i4a > _rtP -> P_142 ) { _rtB -> e3uxf1rx4a = _rtP -> P_142
; } else if ( jp5ua23i4a < _rtP -> P_143 ) { _rtB -> e3uxf1rx4a = _rtP ->
P_143 ; } else { _rtB -> e3uxf1rx4a = jp5ua23i4a ; } } if ( _rtB ->
fqgar3z0da >= _rtP -> P_144 ) { _rtB -> mitttnwtyr = muDoubleScalarSin ( (
_rtP -> P_137 * lvy0y3atfy + _rtB -> eiounl1kd2 ) + _rtB -> howfzjg0z4 ) *
_rtB -> muxovlbgtv ; } else { _rtB -> mitttnwtyr = _rtB -> e3uxf1rx4a ; }
lvy0y3atfy = ssGetT ( S ) ; _rtB -> l0d30ukkiq = ssGetTStart ( S ) ;
lvy0y3atfy -= _rtB -> l0d30ukkiq ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB
-> l1xrrn10pr = _rtP -> P_145 ; } lvy0y3atfy = muDoubleScalarRem ( lvy0y3atfy
, _rtB -> l1xrrn10pr ) ; _rtB -> keefsnuxrd = look1_binlxpw ( lvy0y3atfy ,
_rtP -> P_147 , _rtP -> P_146 , 3U ) ; if ( ssIsSampleHit ( S , 1 , 0 ) ) {
if ( ssIsMajorTimeStep ( S ) ) { _rtDW -> m4eictromn = ( _rtB -> mitttnwtyr
>= _rtB -> keefsnuxrd ) ; } ckeb1s2dlz = _rtDW -> m4eictromn ; hitkasikyj = !
_rtDW -> m4eictromn ; } _rtB -> iqoqu01tut = _rtP -> P_148 * _rtB ->
mitttnwtyr ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { if ( ssIsMajorTimeStep ( S
) ) { _rtDW -> krdkwp0a5c = ( _rtB -> iqoqu01tut >= _rtB -> keefsnuxrd ) ; }
_rtB -> hfimc5myup [ 0 ] = ckeb1s2dlz ; _rtB -> hfimc5myup [ 1 ] = hitkasikyj
; _rtB -> hfimc5myup [ 2 ] = _rtDW -> krdkwp0a5c ; _rtB -> hfimc5myup [ 3 ] =
! _rtDW -> krdkwp0a5c ; } lvy0y3atfy = ssGetT ( S ) ; _rtB -> n0e3jmigef =
ssGetTStart ( S ) ; lvy0y3atfy -= _rtB -> n0e3jmigef ; if ( ssIsSampleHit ( S
, 1 , 0 ) ) { _rtB -> b0rtda24hj = _rtP -> P_149 ; } lvy0y3atfy =
muDoubleScalarRem ( lvy0y3atfy , _rtB -> b0rtda24hj ) ; _rtB -> o3hqxq2oxj =
look1_binlxpw ( lvy0y3atfy , _rtP -> P_151 , _rtP -> P_150 , 2U ) ;
ssCallAccelRunBlock ( S , 3 , 172 , SS_CALL_MDL_OUTPUTS ) ; if (
ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> nwsjnwwyrd = _rtP -> P_152 ;
fmbjzdfypj = _rtP -> P_153 ; } _rtB -> j0tsqlxxlp = 0.0 ; _rtB -> j0tsqlxxlp
+= _rtP -> P_155 * _rtX -> n52bxvtwu4 ; if ( ssIsSampleHit ( S , 4 , 0 ) ) {
_rtB -> gcdisv1ijf = _rtB -> j0tsqlxxlp ; } if ( ssIsSampleHit ( S , 1 , 0 )
) { oossyunrst = muDoubleScalarSign ( _rtB -> gcdisv1ijf - _rtDW ->
b0kcb2fd0b ) ; } _rtB -> ezvob1t52f = 0.0 ; _rtB -> ezvob1t52f += _rtP ->
P_158 * _rtX -> fahgdobdau ; if ( ssIsSampleHit ( S , 4 , 0 ) ) { _rtB ->
jhunvt3eqk = _rtB -> gcdisv1ijf * _rtB -> ezvob1t52f ; } if ( ssIsSampleHit (
S , 1 , 0 ) ) { _rtB -> mo41cxvmi2 = muDoubleScalarSign ( muDoubleScalarSign
( _rtB -> jhunvt3eqk - _rtDW -> osnm2k1bun ) * oossyunrst ) * _rtP -> P_160 +
_rtDW -> kaozvtc11k ; _rtB -> lhjpc5s0cg = fmbjzdfypj + _rtB -> mo41cxvmi2 ;
} if ( ssIsSampleHit ( S , 4 , 0 ) ) { if ( _rtB -> lhjpc5s0cg > _rtP ->
P_162 ) { _rtB -> hcfwego5f4 = _rtP -> P_162 ; } else if ( _rtB -> lhjpc5s0cg
< _rtP -> P_163 ) { _rtB -> hcfwego5f4 = _rtP -> P_163 ; } else { _rtB ->
hcfwego5f4 = _rtB -> lhjpc5s0cg ; } } _rtB -> hvbxnfvt4i = _rtB -> hcfwego5f4
- _rtB -> o3hqxq2oxj ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB ->
e54atfpdeq = _rtP -> P_164 ; } if ( ssIsMajorTimeStep ( S ) ) { _rtDW ->
oqbijwo5xn = ( _rtB -> hvbxnfvt4i >= _rtP -> P_165 ) ; } if ( _rtDW ->
oqbijwo5xn ) { _rtB -> g2o3iq2kqm = _rtB -> nwsjnwwyrd ; } else { _rtB ->
g2o3iq2kqm = _rtB -> e54atfpdeq ; } ssCallAccelRunBlock ( S , 3 , 198 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB ->
ncqiv444nv [ 0 ] = _rtB -> gtvmdf51i0 ; _rtB -> ncqiv444nv [ 1 ] = _rtB ->
ivri3xnnra ; ssCallAccelRunBlock ( S , 3 , 208 , SS_CALL_MDL_OUTPUTS ) ; _rtB
-> o0rsqrdpoy = _rtP -> P_170 * _rtB -> at4svfakyj [ 7 ] ; _rtB -> b3ryv0v1zr
= _rtP -> P_171 * _rtB -> at4svfakyj [ 10 ] ; ssCallAccelRunBlock ( S , 3 ,
211 , SS_CALL_MDL_OUTPUTS ) ; } UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { real_T HoldSine ;
n3qi1whofz * _rtB ; loikxjbxjg * _rtP ; ew10rzwqr2 * _rtDW ; _rtDW = ( (
ew10rzwqr2 * ) ssGetRootDWork ( S ) ) ; _rtP = ( ( loikxjbxjg * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ;
if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtDW -> pd3luz4avz [ 0 ] = _rtB ->
itxnx2wsi5 [ 0 ] ; _rtDW -> pd3luz4avz [ 1 ] = _rtB -> itxnx2wsi5 [ 1 ] ;
_rtDW -> pd3luz4avz [ 2 ] = _rtB -> itxnx2wsi5 [ 2 ] ; _rtDW -> pd3luz4avz [
3 ] = _rtB -> itxnx2wsi5 [ 3 ] ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtDW
-> b2gcy3vb2a [ 0 ] = _rtB -> hfimc5myup [ 0 ] ; _rtDW -> b2gcy3vb2a [ 1 ] =
_rtB -> hfimc5myup [ 1 ] ; _rtDW -> b2gcy3vb2a [ 2 ] = _rtB -> hfimc5myup [ 2
] ; _rtDW -> b2gcy3vb2a [ 3 ] = _rtB -> hfimc5myup [ 3 ] ; } if ( _rtDW ->
byh0btktux && ssIsSampleHit ( S , 2 , 0 ) ) { HoldSine = _rtP -> P_32 * _rtB
-> ecey0q2vme ; _rtDW -> l4dlj3krq0 [ 0 ] += HoldSine ; _rtDW -> l4dlj3krq0 [
1 ] += HoldSine ; _rtDW -> l4dlj3krq0 [ 2 ] += HoldSine ; _rtDW -> l4dlj3krq0
[ 3 ] += HoldSine ; if ( _rtB -> hpvnminlgh [ 0 ] > 0.0 ) { _rtDW ->
b13awnigwz [ 0 ] = 1 ; } else if ( _rtB -> hpvnminlgh [ 0 ] < 0.0 ) { _rtDW
-> b13awnigwz [ 0 ] = - 1 ; } else if ( _rtB -> hpvnminlgh [ 0 ] == 0.0 ) {
_rtDW -> b13awnigwz [ 0 ] = 0 ; } else { _rtDW -> b13awnigwz [ 0 ] = 2 ; } if
( _rtB -> hpvnminlgh [ 1 ] > 0.0 ) { _rtDW -> b13awnigwz [ 1 ] = 1 ; } else
if ( _rtB -> hpvnminlgh [ 1 ] < 0.0 ) { _rtDW -> b13awnigwz [ 1 ] = - 1 ; }
else if ( _rtB -> hpvnminlgh [ 1 ] == 0.0 ) { _rtDW -> b13awnigwz [ 1 ] = 0 ;
} else { _rtDW -> b13awnigwz [ 1 ] = 2 ; } if ( _rtB -> hpvnminlgh [ 2 ] >
0.0 ) { _rtDW -> b13awnigwz [ 2 ] = 1 ; } else if ( _rtB -> hpvnminlgh [ 2 ]
< 0.0 ) { _rtDW -> b13awnigwz [ 2 ] = - 1 ; } else if ( _rtB -> hpvnminlgh [
2 ] == 0.0 ) { _rtDW -> b13awnigwz [ 2 ] = 0 ; } else { _rtDW -> b13awnigwz [
2 ] = 2 ; } if ( _rtB -> hpvnminlgh [ 3 ] > 0.0 ) { _rtDW -> b13awnigwz [ 3 ]
= 1 ; } else if ( _rtB -> hpvnminlgh [ 3 ] < 0.0 ) { _rtDW -> b13awnigwz [ 3
] = - 1 ; } else if ( _rtB -> hpvnminlgh [ 3 ] == 0.0 ) { _rtDW -> b13awnigwz
[ 3 ] = 0 ; } else { _rtDW -> b13awnigwz [ 3 ] = 2 ; } _rtDW -> bxpfaqqn5g [
0 ] = _rtB -> peonb2s13j [ 0 ] ; _rtDW -> bxpfaqqn5g [ 1 ] = _rtB ->
peonb2s13j [ 1 ] ; _rtDW -> bxpfaqqn5g [ 2 ] = _rtB -> peonb2s13j [ 2 ] ;
_rtDW -> bxpfaqqn5g [ 3 ] = _rtB -> peonb2s13j [ 3 ] ; } if ( ssIsSampleHit (
S , 2 , 0 ) ) { HoldSine = _rtDW -> gzppta2yd5 ; _rtDW -> gzppta2yd5 = _rtDW
-> gzppta2yd5 * _rtP -> P_51 + _rtDW -> h4dq2buhxq * _rtP -> P_50 ; _rtDW ->
h4dq2buhxq = _rtDW -> h4dq2buhxq * _rtP -> P_51 - HoldSine * _rtP -> P_50 ;
ssCallAccelRunBlock ( S , 0 , 6 , SS_CALL_MDL_UPDATE ) ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> bxxpgfrdrd . TUbufferPtrs [ 0 ] ; real_T
* * tBuffer = ( real_T * * ) & _rtDW -> bxxpgfrdrd . TUbufferPtrs [ 1 ] ;
real_T simTime = ssGetT ( S ) ; _rtDW -> cmbq4gun1g . Head = ( ( _rtDW ->
cmbq4gun1g . Head < ( _rtDW -> cmbq4gun1g . CircularBufSize - 1 ) ) ? ( _rtDW
-> cmbq4gun1g . Head + 1 ) : 0 ) ; if ( _rtDW -> cmbq4gun1g . Head == _rtDW
-> cmbq4gun1g . Tail ) { if ( !
PV_Single_Phase_inverter_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
cmbq4gun1g . CircularBufSize , & _rtDW -> cmbq4gun1g . Tail , & _rtDW ->
cmbq4gun1g . Head , & _rtDW -> cmbq4gun1g . Last , simTime - _rtP -> P_83 ,
tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW ->
cmbq4gun1g . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
cmbq4gun1g . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> cmbq4gun1g . Head ]
= _rtB -> kb3yih1xdi ; } if ( ssIsSampleHit ( S , 4 , 0 ) ) { _rtDW ->
lkjsqizslz += _rtP -> P_85 * _rtB -> htjmcyxm4o ; } if ( ssIsSampleHit ( S ,
2 , 0 ) ) { _rtDW -> dxwkuh3yet = _rtDW -> jdlga5gylp ; _rtDW -> b3cy3g4cyx
+= _rtP -> P_92 * _rtB -> fmfmahjizi ; if ( _rtDW -> b3cy3g4cyx >= _rtP ->
P_94 ) { _rtDW -> b3cy3g4cyx = _rtP -> P_94 ; } else { if ( _rtDW ->
b3cy3g4cyx <= _rtP -> P_95 ) { _rtDW -> b3cy3g4cyx = _rtP -> P_95 ; } } } if
( ssIsSampleHit ( S , 4 , 0 ) ) { _rtDW -> p0mjtkssxb = 0U ; _rtDW ->
fv1uvl41xo = _rtP -> P_102 * _rtB -> knnrqhsfui + _rtB -> gsb54uku3l ; _rtDW
-> bgv2ulrz1x = _rtB -> mq00slxbar ; ssCallAccelRunBlock ( S , 3 , 108 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> mo5i5rvr5i = _rtB -> knnrqhsfui ; } if (
_rtDW -> m1v05clsvv && ssIsSampleHit ( S , 4 , 0 ) ) { _rtDW -> i5rjtcs5sd =
0U ; _rtDW -> hbwmzyylnb = _rtP -> P_3 * _rtB -> jkovacda1u [ 0 ] + _rtB ->
ptmdu1k3aj ; ssCallAccelRunBlock ( S , 1 , 7 , SS_CALL_MDL_UPDATE ) ; _rtDW
-> es20fy2lvr = _rtB -> jkovacda1u [ 0 ] ; _rtDW -> j1qqayjy1b = 0U ; _rtDW
-> ltialakutr = _rtP -> P_15 * _rtB -> jkovacda1u [ 1 ] + _rtB -> np3w5lg3ur
; ssCallAccelRunBlock ( S , 1 , 27 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
e3hcogjryj = _rtB -> jkovacda1u [ 1 ] ; } if ( ssIsSampleHit ( S , 4 , 0 ) )
{ _rtDW -> n1uqml3zbu += _rtP -> P_117 * _rtB -> fhncp1qyw1 ; if ( _rtDW ->
n1uqml3zbu >= _rtP -> P_119 ) { _rtDW -> n1uqml3zbu = _rtP -> P_119 ; } else
{ if ( _rtDW -> n1uqml3zbu <= _rtP -> P_120 ) { _rtDW -> n1uqml3zbu = _rtP ->
P_120 ; } } _rtDW -> j1gf540g2s = _rtB -> gwwpugbml0 ; _rtDW -> j0jutlrn4j =
_rtB -> bzzw2dv1ny ; { real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_130 [ 0 ]
) * _rtDW -> opbvzhddhh [ 0 ] + ( _rtP -> P_130 [ 1 ] ) * _rtDW -> opbvzhddhh
[ 1 ] ; xnew [ 0 ] += ( _rtP -> P_131 [ 0 ] ) * _rtB -> bzzw2dv1ny ; xnew [ 1
] = ( _rtP -> P_130 [ 2 ] ) * _rtDW -> opbvzhddhh [ 0 ] + ( _rtP -> P_130 [ 3
] ) * _rtDW -> opbvzhddhh [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_131 [ 1 ] ) *
_rtB -> bzzw2dv1ny ; ( void ) memcpy ( & _rtDW -> opbvzhddhh [ 0 ] , xnew ,
sizeof ( real_T ) * 2 ) ; } } if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtDW ->
b0kcb2fd0b = _rtB -> gcdisv1ijf ; _rtDW -> osnm2k1bun = _rtB -> jhunvt3eqk ;
_rtDW -> kaozvtc11k = _rtB -> mo41cxvmi2 ; } if ( ssIsSampleHit ( S , 2 , 0 )
) { ssCallAccelRunBlock ( S , 3 , 208 , SS_CALL_MDL_UPDATE ) ; }
UNUSED_PARAMETER ( tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { n3qi1whofz * _rtB ; loikxjbxjg
* _rtP ; f1xhd02yjc * _rtX ; pqmvzr1kvu * _rtXdot ; _rtXdot = ( ( pqmvzr1kvu
* ) ssGetdX ( S ) ) ; _rtX = ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) ;
_rtP = ( ( loikxjbxjg * ) ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz *
) _ssGetBlockIO ( S ) ) ; _rtXdot -> n52bxvtwu4 = 0.0 ; _rtXdot -> n52bxvtwu4
+= _rtP -> P_154 * _rtX -> n52bxvtwu4 ; _rtXdot -> n52bxvtwu4 += _rtB ->
gtvmdf51i0 ; _rtXdot -> fahgdobdau = 0.0 ; _rtXdot -> fahgdobdau += _rtP ->
P_157 * _rtX -> fahgdobdau ; _rtXdot -> fahgdobdau += _rtB -> ivri3xnnra ;
_rtXdot -> pjbcugwxf0 = 0.0 ; _rtXdot -> pjbcugwxf0 += _rtP -> P_166 * _rtX
-> pjbcugwxf0 ; _rtXdot -> pjbcugwxf0 += _rtB -> gtvmdf51i0 ; _rtXdot ->
nsp3ji42uc = 0.0 ; _rtXdot -> nsp3ji42uc += _rtP -> P_168 * _rtX ->
nsp3ji42uc ; _rtXdot -> nsp3ji42uc += _rtB -> ivri3xnnra ; }
#define MDL_ZERO_CROSSINGS
static void mdlZeroCrossings ( SimStruct * S ) { n3qi1whofz * _rtB ;
loikxjbxjg * _rtP ; d15n1ti4kq * _rtZCSV ; _rtZCSV = ( ( d15n1ti4kq * )
ssGetSolverZcSignalVector ( S ) ) ; _rtP = ( ( loikxjbxjg * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ;
_rtZCSV -> j2wg3uhh42 = _rtB -> mitttnwtyr - _rtB -> keefsnuxrd ; _rtZCSV ->
osxehvr00q = _rtB -> iqoqu01tut - _rtB -> keefsnuxrd ; _rtZCSV -> gxb5rs1fuh
= _rtB -> hvbxnfvt4i - _rtP -> P_165 ; } static void mdlInitializeSizes (
SimStruct * S ) { ssSetChecksumVal ( S , 0 , 1396219958U ) ; ssSetChecksumVal
( S , 1 , 3607492743U ) ; ssSetChecksumVal ( S , 2 , 643093016U ) ;
ssSetChecksumVal ( S , 3 , 1386400694U ) ; { mxArray * slVerStructMat = NULL
; mxArray * slStrMat = mxCreateString ( "simulink" ) ; char slVerChar [ 10 ]
; int status = mexCallMATLAB ( 1 , & slVerStructMat , 1 , & slStrMat , "ver"
) ; if ( status == 0 ) { mxArray * slVerMat = mxGetField ( slVerStructMat , 0
, "Version" ) ; if ( slVerMat == NULL ) { status = 1 ; } else { status =
mxGetString ( slVerMat , slVerChar , 10 ) ; } } mxDestroyArray ( slStrMat ) ;
mxDestroyArray ( slVerStructMat ) ; if ( ( status == 1 ) || ( strcmp (
slVerChar , "8.3" ) != 0 ) ) { return ; } } ssSetOptions ( S ,
SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork ( S ) != sizeof (
ew10rzwqr2 ) ) { ssSetErrorStatus ( S ,
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
loikxjbxjg * ) ssGetDefaultParam ( S ) ) -> P_80 = rtInf ; } static void
mdlInitializeSampleTimes ( SimStruct * S ) { } static void mdlTerminate (
SimStruct * S ) { }
#include "simulink.c"
