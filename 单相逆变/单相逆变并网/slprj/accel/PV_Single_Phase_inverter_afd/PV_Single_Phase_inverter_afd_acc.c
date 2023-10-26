#include "__cf_PV_Single_Phase_inverter_afd.h"
#include <math.h>
#include "PV_Single_Phase_inverter_afd_acc.h"
#include "PV_Single_Phase_inverter_afd_acc_private.h"
#include <stdio.h>
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T PV_Single_Phase_inverter_afd_acc_rt_TDelayUpdateTailOrGrowBuf (
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
PV_Single_Phase_inverter_afd_acc_rt_TDelayInterpolate ( real_T tMinusDelay ,
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
SimStruct * S , int_T tid ) { real_T ib4jdeu3b2 ; real_T jj21tnwlcr ; real_T
fygrsvxcxj ; real_T jp5ua23i4a ; real_T fmbjzdfypj ; boolean_T hitkasikyj ;
real_T ckqv2ilyre ; boolean_T ckeb1s2dlz ; real_T aoer0ike4l ; real_T
hwjiit4ixv ; real_T hvhzmisfej ; real_T oossyunrst ; real_T eo3w5kdmqj ;
real_T jsxxec1j5w ; real_T bpjo4wesci_idx_0 ; real_T bpjo4wesci_idx_1 ;
real_T bpjo4wesci_idx_2 ; real_T bpjo4wesci_idx_3 ; n3qi1whofz * _rtB ;
loikxjbxjg * _rtP ; f1xhd02yjc * _rtX ; ew10rzwqr2 * _rtDW ; _rtDW = ( (
ew10rzwqr2 * ) ssGetRootDWork ( S ) ) ; _rtX = ( ( f1xhd02yjc * )
ssGetContStates ( S ) ) ; _rtP = ( ( loikxjbxjg * ) ssGetDefaultParam ( S ) )
; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ; if ( ssIsSampleHit ( S ,
2 , 0 ) ) { fygrsvxcxj = _rtP -> P_44 ; _rtB -> jrewwolk22 [ 0 ] = _rtP ->
P_45 [ 0 ] ; _rtB -> jrewwolk22 [ 1 ] = _rtP -> P_45 [ 1 ] ; _rtB ->
jrewwolk22 [ 2 ] = _rtP -> P_45 [ 2 ] ; _rtB -> jrewwolk22 [ 3 ] = _rtP ->
P_45 [ 3 ] ; _rtB -> lbxyutubre [ 0 ] = _rtDW -> pd3luz4avz [ 0 ] ; _rtB ->
lbxyutubre [ 1 ] = _rtDW -> pd3luz4avz [ 1 ] ; _rtB -> lbxyutubre [ 2 ] =
_rtDW -> pd3luz4avz [ 2 ] ; _rtB -> lbxyutubre [ 3 ] = _rtDW -> pd3luz4avz [
3 ] ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> e3hpbcnjjc [ 0 ] = _rtDW
-> b2gcy3vb2a [ 0 ] ; _rtB -> e3hpbcnjjc [ 1 ] = _rtDW -> b2gcy3vb2a [ 1 ] ;
_rtB -> e3hpbcnjjc [ 2 ] = _rtDW -> b2gcy3vb2a [ 2 ] ; _rtB -> e3hpbcnjjc [ 3
] = _rtDW -> b2gcy3vb2a [ 3 ] ; _rtB -> nrgiikxpzm = _rtP -> P_188 ; if (
ssIsMajorTimeStep ( S ) ) { if ( _rtB -> nrgiikxpzm ) { if ( ! _rtDW ->
byh0btktux ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetSolverNeedsReset ( S ) ; } _rtDW -> l4dlj3krq0 [ 0 ] = _rtP -> P_33 ;
_rtDW -> l4dlj3krq0 [ 1 ] = _rtP -> P_33 ; _rtDW -> l4dlj3krq0 [ 2 ] = _rtP
-> P_33 ; _rtDW -> l4dlj3krq0 [ 3 ] = _rtP -> P_33 ; _rtDW -> b13awnigwz [ 0
] = 2 ; _rtDW -> b13awnigwz [ 1 ] = 2 ; _rtDW -> b13awnigwz [ 2 ] = 2 ; _rtDW
-> b13awnigwz [ 3 ] = 2 ; _rtDW -> bxpfaqqn5g [ 0 ] = _rtP -> P_42 ; _rtDW ->
bxpfaqqn5g [ 1 ] = _rtP -> P_42 ; _rtDW -> bxpfaqqn5g [ 2 ] = _rtP -> P_42 ;
_rtDW -> bxpfaqqn5g [ 3 ] = _rtP -> P_42 ; _rtDW -> byh0btktux = true ; } }
else { if ( _rtDW -> byh0btktux ) { ssSetSolverNeedsReset ( S ) ; _rtDW ->
byh0btktux = false ; } } } } if ( _rtDW -> byh0btktux ) { if ( ssIsSampleHit
( S , 2 , 0 ) ) { _rtB -> ecey0q2vme = _rtP -> P_30 ; } if ( ssIsSampleHit (
S , 1 , 0 ) ) { eo3w5kdmqj = _rtP -> P_31 ; } if ( ssIsSampleHit ( S , 2 , 0
) ) { if ( ( _rtB -> e3hpbcnjjc [ 0 ] <= 0.0 ) && ( _rtDW -> b13awnigwz [ 0 ]
== 1 ) ) { _rtDW -> l4dlj3krq0 [ 0 ] = _rtP -> P_33 ; } if ( ( _rtB ->
e3hpbcnjjc [ 1 ] <= 0.0 ) && ( _rtDW -> b13awnigwz [ 1 ] == 1 ) ) { _rtDW ->
l4dlj3krq0 [ 1 ] = _rtP -> P_33 ; } if ( ( _rtB -> e3hpbcnjjc [ 2 ] <= 0.0 )
&& ( _rtDW -> b13awnigwz [ 2 ] == 1 ) ) { _rtDW -> l4dlj3krq0 [ 2 ] = _rtP ->
P_33 ; } if ( ( _rtB -> e3hpbcnjjc [ 3 ] <= 0.0 ) && ( _rtDW -> b13awnigwz [
3 ] == 1 ) ) { _rtDW -> l4dlj3krq0 [ 3 ] = _rtP -> P_33 ; } _rtB ->
igl1rmdosy [ 0 ] = _rtDW -> l4dlj3krq0 [ 0 ] ; _rtB -> igl1rmdosy [ 1 ] =
_rtDW -> l4dlj3krq0 [ 1 ] ; _rtB -> igl1rmdosy [ 2 ] = _rtDW -> l4dlj3krq0 [
2 ] ; _rtB -> igl1rmdosy [ 3 ] = _rtDW -> l4dlj3krq0 [ 3 ] ; } if (
ssIsSampleHit ( S , 1 , 0 ) ) { jsxxec1j5w = _rtP -> P_34 ; } if (
ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> hceqogjf5t [ 0 ] = _rtP -> P_35 *
_rtB -> igl1rmdosy [ 0 ] ; _rtB -> hceqogjf5t [ 1 ] = _rtP -> P_35 * _rtB ->
igl1rmdosy [ 1 ] ; _rtB -> hceqogjf5t [ 2 ] = _rtP -> P_35 * _rtB ->
igl1rmdosy [ 2 ] ; _rtB -> hceqogjf5t [ 3 ] = _rtP -> P_35 * _rtB ->
igl1rmdosy [ 3 ] ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { bpjo4wesci_idx_3 =
jsxxec1j5w + _rtB -> hceqogjf5t [ 0 ] ; jp5ua23i4a = ( _rtP -> P_38 - _rtB ->
igl1rmdosy [ 0 ] ) * _rtP -> P_39 ; if ( bpjo4wesci_idx_3 > _rtP -> P_36 ) {
bpjo4wesci_idx_3 = _rtP -> P_36 ; } else { if ( bpjo4wesci_idx_3 < _rtP ->
P_37 ) { bpjo4wesci_idx_3 = _rtP -> P_37 ; } } if ( jp5ua23i4a > _rtP -> P_40
) { jp5ua23i4a = _rtP -> P_40 ; } else { if ( jp5ua23i4a < _rtP -> P_41 ) {
jp5ua23i4a = _rtP -> P_41 ; } } bpjo4wesci_idx_0 = bpjo4wesci_idx_3 +
jp5ua23i4a ; bpjo4wesci_idx_3 = jsxxec1j5w + _rtB -> hceqogjf5t [ 1 ] ;
jp5ua23i4a = ( _rtP -> P_38 - _rtB -> igl1rmdosy [ 1 ] ) * _rtP -> P_39 ; if
( bpjo4wesci_idx_3 > _rtP -> P_36 ) { bpjo4wesci_idx_3 = _rtP -> P_36 ; }
else { if ( bpjo4wesci_idx_3 < _rtP -> P_37 ) { bpjo4wesci_idx_3 = _rtP ->
P_37 ; } } if ( jp5ua23i4a > _rtP -> P_40 ) { jp5ua23i4a = _rtP -> P_40 ; }
else { if ( jp5ua23i4a < _rtP -> P_41 ) { jp5ua23i4a = _rtP -> P_41 ; } }
bpjo4wesci_idx_1 = bpjo4wesci_idx_3 + jp5ua23i4a ; bpjo4wesci_idx_3 =
jsxxec1j5w + _rtB -> hceqogjf5t [ 2 ] ; jp5ua23i4a = ( _rtP -> P_38 - _rtB ->
igl1rmdosy [ 2 ] ) * _rtP -> P_39 ; if ( bpjo4wesci_idx_3 > _rtP -> P_36 ) {
bpjo4wesci_idx_3 = _rtP -> P_36 ; } else { if ( bpjo4wesci_idx_3 < _rtP ->
P_37 ) { bpjo4wesci_idx_3 = _rtP -> P_37 ; } } if ( jp5ua23i4a > _rtP -> P_40
) { jp5ua23i4a = _rtP -> P_40 ; } else { if ( jp5ua23i4a < _rtP -> P_41 ) {
jp5ua23i4a = _rtP -> P_41 ; } } bpjo4wesci_idx_2 = bpjo4wesci_idx_3 +
jp5ua23i4a ; bpjo4wesci_idx_3 = jsxxec1j5w + _rtB -> hceqogjf5t [ 3 ] ;
jp5ua23i4a = ( _rtP -> P_38 - _rtB -> igl1rmdosy [ 3 ] ) * _rtP -> P_39 ; if
( bpjo4wesci_idx_3 > _rtP -> P_36 ) { bpjo4wesci_idx_3 = _rtP -> P_36 ; }
else { if ( bpjo4wesci_idx_3 < _rtP -> P_37 ) { bpjo4wesci_idx_3 = _rtP ->
P_37 ; } } if ( jp5ua23i4a > _rtP -> P_40 ) { jp5ua23i4a = _rtP -> P_40 ; }
else { if ( jp5ua23i4a < _rtP -> P_41 ) { jp5ua23i4a = _rtP -> P_41 ; } }
bpjo4wesci_idx_3 += jp5ua23i4a ; } if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB
-> adlprvfww0 [ 0 ] = _rtDW -> bxpfaqqn5g [ 0 ] ; _rtB -> adlprvfww0 [ 1 ] =
_rtDW -> bxpfaqqn5g [ 1 ] ; _rtB -> adlprvfww0 [ 2 ] = _rtDW -> bxpfaqqn5g [
2 ] ; _rtB -> adlprvfww0 [ 3 ] = _rtDW -> bxpfaqqn5g [ 3 ] ; } if (
ssIsSampleHit ( S , 1 , 0 ) ) { if ( _rtB -> e3hpbcnjjc [ 0 ] >= _rtP -> P_43
) { _rtB -> peonb2s13j [ 0 ] = _rtB -> lbxyutubre [ 0 ] ; } else { _rtB ->
peonb2s13j [ 0 ] = _rtB -> adlprvfww0 [ 0 ] ; } if ( _rtB -> e3hpbcnjjc [ 1 ]
>= _rtP -> P_43 ) { _rtB -> peonb2s13j [ 1 ] = _rtB -> lbxyutubre [ 1 ] ; }
else { _rtB -> peonb2s13j [ 1 ] = _rtB -> adlprvfww0 [ 1 ] ; } if ( _rtB ->
e3hpbcnjjc [ 2 ] >= _rtP -> P_43 ) { _rtB -> peonb2s13j [ 2 ] = _rtB ->
lbxyutubre [ 2 ] ; } else { _rtB -> peonb2s13j [ 2 ] = _rtB -> adlprvfww0 [ 2
] ; } if ( _rtB -> e3hpbcnjjc [ 3 ] >= _rtP -> P_43 ) { _rtB -> peonb2s13j [
3 ] = _rtB -> lbxyutubre [ 3 ] ; } else { _rtB -> peonb2s13j [ 3 ] = _rtB ->
adlprvfww0 [ 3 ] ; } _rtB -> auwitbs4pp [ 0 ] = bpjo4wesci_idx_0 * _rtB ->
peonb2s13j [ 0 ] * eo3w5kdmqj ; _rtB -> auwitbs4pp [ 1 ] = bpjo4wesci_idx_1 *
_rtB -> peonb2s13j [ 1 ] * eo3w5kdmqj ; _rtB -> auwitbs4pp [ 2 ] =
bpjo4wesci_idx_2 * _rtB -> peonb2s13j [ 2 ] * eo3w5kdmqj ; _rtB -> auwitbs4pp
[ 3 ] = bpjo4wesci_idx_3 * _rtB -> peonb2s13j [ 3 ] * eo3w5kdmqj ; } if (
ssIsMajorTimeStep ( S ) ) { srUpdateBC ( _rtDW -> cvc2xojcfc ) ; } } if (
ssIsSampleHit ( S , 2 , 0 ) ) { if ( _rtDW -> fueor4l32w != 0 ) { _rtDW ->
gzppta2yd5 = muDoubleScalarSin ( _rtP -> P_50 * ssGetTaskTime ( S , 2 ) ) ;
_rtDW -> h4dq2buhxq = muDoubleScalarCos ( _rtP -> P_50 * ssGetTaskTime ( S ,
2 ) ) ; _rtDW -> fueor4l32w = 0 ; } _rtB -> dfyjajvsrf = ( ( _rtDW ->
gzppta2yd5 * _rtP -> P_54 + _rtDW -> h4dq2buhxq * _rtP -> P_53 ) * _rtP ->
P_52 + ( _rtDW -> h4dq2buhxq * _rtP -> P_54 - _rtDW -> gzppta2yd5 * _rtP ->
P_53 ) * _rtP -> P_51 ) * _rtP -> P_48 + _rtP -> P_49 ; bpjo4wesci_idx_0 =
_rtP -> P_55 / _rtP -> P_56 ; eo3w5kdmqj = bpjo4wesci_idx_0 - _rtP -> P_57 ;
jp5ua23i4a = _rtP -> P_57 - _rtP -> P_58 / _rtP -> P_59 ; jsxxec1j5w =
jp5ua23i4a ; if ( ssIsMajorTimeStep ( S ) ) { if ( _rtDW -> ccnsfwllyk != 0 )
{ ssSetSolverNeedsReset ( S ) ; _rtDW -> ccnsfwllyk = 0 ; } } else { if (
jp5ua23i4a < 0.0 ) { jsxxec1j5w = 0.0 ; _rtDW -> ccnsfwllyk = 1 ; } }
eo3w5kdmqj /= muDoubleScalarLog ( jsxxec1j5w ) ; _rtB -> merlowzogr = 1.0 /
muDoubleScalarExp ( bpjo4wesci_idx_0 / eo3w5kdmqj ) * jp5ua23i4a ; _rtB ->
plq5ubw3vt = eo3w5kdmqj * _rtP -> P_60 ; jj21tnwlcr = _rtP -> P_65 ;
bpjo4wesci_idx_0 = _rtP -> P_64 / jj21tnwlcr ; jj21tnwlcr = _rtP -> P_66 ;
jp5ua23i4a = _rtP -> P_67 * jj21tnwlcr ; jj21tnwlcr = _rtP -> P_68 ;
jj21tnwlcr -= _rtP -> P_69 ; _rtB -> pxraeqag4v = ( bpjo4wesci_idx_0 - _rtP
-> P_63 ) * _rtP -> P_62 + bpjo4wesci_idx_0 * jp5ua23i4a * jj21tnwlcr ; _rtB
-> b5r3n2glcn = ( 0.0 - _rtP -> P_61 * _rtB -> pxraeqag4v ) - _rtP -> P_70 *
jj21tnwlcr ; _rtB -> l4ddgizmio = _rtP -> P_71 ; _rtB -> a5uuaan0fj = _rtP ->
P_72 ; _rtB -> jisyjbxemd = _rtP -> P_73 ; ssCallAccelRunBlock ( S , 3 , 44 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> p4uajk50sn = _rtP -> P_74 * _rtB ->
at4svfakyj [ 1 ] - _rtDW -> k5u0blc5by ; _rtB -> fjqqgv5ing = ( _rtB ->
p4uajk50sn * _rtDW -> kn5xrzh2lm > fygrsvxcxj ) ; } if ( ssIsSampleHit ( S ,
1 , 0 ) ) { if ( ssIsMajorTimeStep ( S ) ) { _rtDW -> epz3e0mhiv = (
ssGetTaskTime ( S , 1 ) >= _rtP -> P_77 ) ; } if ( _rtDW -> epz3e0mhiv == 1 )
{ ckqv2ilyre = _rtP -> P_79 ; } else { ckqv2ilyre = _rtP -> P_78 ; } } if (
ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> oq3yz5z5co = _rtP -> P_80 ;
jj21tnwlcr = ssGetTaskTime ( S , 2 ) ; _rtB -> n3ykvmsgw4 = rt_Lookup ( _rtP
-> P_81 , 6 , jj21tnwlcr , _rtP -> P_82 ) ; } if ( ssIsSampleHit ( S , 1 , 0
) ) { if ( ! ( _rtB -> oq3yz5z5co >= _rtP -> P_83 ) ) { ckqv2ilyre = _rtB ->
n3ykvmsgw4 ; } fmbjzdfypj = _rtB -> fjqqgv5ing + ckqv2ilyre ; if ( fmbjzdfypj
>= _rtP -> P_84 ) { _rtDW -> pqxnc1gnc0 = true ; } else { if ( fmbjzdfypj <=
_rtP -> P_85 ) { _rtDW -> pqxnc1gnc0 = false ; } } ckeb1s2dlz = ( ckqv2ilyre
!= 0.0 ) ; if ( _rtDW -> pqxnc1gnc0 ) { bpjo4wesci_idx_3 = _rtP -> P_86 ; }
else { bpjo4wesci_idx_3 = _rtP -> P_87 ; } hitkasikyj = ( ( bpjo4wesci_idx_3
!= 0.0 ) || ( ckqv2ilyre != 0.0 ) ) ; fmbjzdfypj = hitkasikyj ; if ( _rtDW ->
pa4mtppeis ) { _rtDW -> pa4mtppeis = false ; _rtB -> kry5hz2pth = _rtP ->
P_88 ; } else { _rtB -> kry5hz2pth = hitkasikyj ; } } if ( ssIsSampleHit ( S
, 2 , 0 ) ) { _rtB -> bwx0anqw5s = _rtP -> P_89 * _rtB -> at4svfakyj [ 10 ] ;
_rtB -> eiywenztid = _rtP -> P_90 * _rtB -> at4svfakyj [ 13 ] ; _rtB ->
ikx4yqfnno = _rtP -> P_91 * _rtB -> eiywenztid ; ssCallAccelRunBlock ( S , 3
, 73 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> lwiqst51ht = _rtP -> P_92 * _rtB ->
at4svfakyj [ 12 ] ; _rtB -> p1ntvjnf2a = _rtB -> gtvmdf51i0 * _rtB ->
lwiqst51ht ; ssCallAccelRunBlock ( S , 3 , 76 , SS_CALL_MDL_OUTPUTS ) ;
jj21tnwlcr = _rtP -> P_93 ; if ( _rtB -> gfa3kvzw15 [ 4 ] >= _rtP -> P_95 ) {
bpjo4wesci_idx_3 = _rtP -> P_94 * _rtB -> at4svfakyj [ 4 ] ; } else {
bpjo4wesci_idx_3 = jj21tnwlcr ; } if ( bpjo4wesci_idx_3 > _rtP -> P_96 ) {
_rtB -> k2gt5xfkuo [ 0 ] = _rtP -> P_96 ; } else if ( bpjo4wesci_idx_3 < _rtP
-> P_97 ) { _rtB -> k2gt5xfkuo [ 0 ] = _rtP -> P_97 ; } else { _rtB ->
k2gt5xfkuo [ 0 ] = bpjo4wesci_idx_3 ; } if ( _rtB -> gfa3kvzw15 [ 5 ] >= _rtP
-> P_95 ) { bpjo4wesci_idx_3 = _rtP -> P_94 * _rtB -> at4svfakyj [ 5 ] ; }
else { bpjo4wesci_idx_3 = jj21tnwlcr ; } if ( bpjo4wesci_idx_3 > _rtP -> P_96
) { _rtB -> k2gt5xfkuo [ 1 ] = _rtP -> P_96 ; } else if ( bpjo4wesci_idx_3 <
_rtP -> P_97 ) { _rtB -> k2gt5xfkuo [ 1 ] = _rtP -> P_97 ; } else { _rtB ->
k2gt5xfkuo [ 1 ] = bpjo4wesci_idx_3 ; } if ( _rtB -> gfa3kvzw15 [ 6 ] >= _rtP
-> P_95 ) { bpjo4wesci_idx_3 = _rtP -> P_94 * _rtB -> at4svfakyj [ 6 ] ; }
else { bpjo4wesci_idx_3 = jj21tnwlcr ; } if ( bpjo4wesci_idx_3 > _rtP -> P_96
) { _rtB -> k2gt5xfkuo [ 2 ] = _rtP -> P_96 ; } else if ( bpjo4wesci_idx_3 <
_rtP -> P_97 ) { _rtB -> k2gt5xfkuo [ 2 ] = _rtP -> P_97 ; } else { _rtB ->
k2gt5xfkuo [ 2 ] = bpjo4wesci_idx_3 ; } if ( _rtB -> gfa3kvzw15 [ 7 ] >= _rtP
-> P_95 ) { bpjo4wesci_idx_3 = _rtP -> P_94 * _rtB -> at4svfakyj [ 7 ] ; }
else { bpjo4wesci_idx_3 = jj21tnwlcr ; } if ( bpjo4wesci_idx_3 > _rtP -> P_96
) { _rtB -> k2gt5xfkuo [ 3 ] = _rtP -> P_96 ; } else if ( bpjo4wesci_idx_3 <
_rtP -> P_97 ) { _rtB -> k2gt5xfkuo [ 3 ] = _rtP -> P_97 ; } else { _rtB ->
k2gt5xfkuo [ 3 ] = bpjo4wesci_idx_3 ; } } if ( ssIsSampleHit ( S , 1 , 0 ) )
{ fmbjzdfypj = _rtP -> P_98 ; } if ( ssIsSampleHit ( S , 4 , 0 ) ) { _rtB ->
pesub22waq = _rtB -> eiywenztid ; } { real_T * * uBuffer = ( real_T * * ) &
_rtDW -> bxxpgfrdrd . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * *
) & _rtDW -> bxxpgfrdrd . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S )
; real_T tMinusDelay = simTime - _rtP -> P_99 ; ib4jdeu3b2 =
PV_Single_Phase_inverter_afd_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , _rtDW -> cmbq4gun1g . CircularBufSize , & _rtDW ->
cmbq4gun1g . Last , _rtDW -> cmbq4gun1g . Tail , _rtDW -> cmbq4gun1g . Head ,
_rtP -> P_100 , 1 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && (
ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) ) ) ; } if ( ssIsSampleHit ( S
, 4 , 0 ) ) { hvhzmisfej = _rtDW -> bgv2ulrz1x ; aoer0ike4l =
muDoubleScalarMod ( _rtDW -> lkjsqizslz , _rtP -> P_104 ) ; _rtB ->
egzqinya3d [ 0 ] = _rtDW -> bgv2ulrz1x ; _rtB -> egzqinya3d [ 1 ] =
aoer0ike4l ; ssCallAccelRunBlock ( S , 3 , 100 , SS_CALL_MDL_OUTPUTS ) ; }
_rtB -> fbhe5aje54 = ib4jdeu3b2 * _rtB -> czop1dz2xx [ 0 ] - _rtB ->
pesub22waq ; if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> atxp5ufhq0 = _rtP
-> P_105 * _rtB -> fbhe5aje54 ; _rtDW -> jdlga5gylp = ( _rtB -> atxp5ufhq0 -
_rtP -> P_107 [ 1 ] * _rtDW -> dxwkuh3yet ) / _rtP -> P_107 [ 0 ] ;
jj21tnwlcr = _rtP -> P_106 [ 0 ] * _rtDW -> jdlga5gylp + _rtP -> P_106 [ 1 ]
* _rtDW -> dxwkuh3yet ; _rtB -> dpstypxipj = _rtP -> P_114 * _rtB ->
fbhe5aje54 ; hwjiit4ixv = ( _rtP -> P_113 * _rtB -> fbhe5aje54 + _rtDW ->
b3cy3g4cyx ) + jj21tnwlcr ; if ( hwjiit4ixv > _rtP -> P_115 ) { hwjiit4ixv =
_rtP -> P_115 ; } else { if ( hwjiit4ixv < _rtP -> P_116 ) { hwjiit4ixv =
_rtP -> P_116 ; } } } if ( ssIsSampleHit ( S , 4 , 0 ) ) { _rtB -> f3auxgllop
= _rtB -> bwx0anqw5s ; } if ( ssIsSampleHit ( S , 2 , 0 ) ) { jj21tnwlcr = (
_rtB -> f3auxgllop + hwjiit4ixv ) * _rtP -> P_117 ; } if ( ssIsSampleHit ( S
, 1 , 0 ) ) { _rtB -> apdiuhihpb = _rtB -> p1ntvjnf2a / fmbjzdfypj ; } if (
ssIsSampleHit ( S , 4 , 0 ) ) { ssCallAccelRunBlock ( S , 3 , 123 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 3 , 124 ,
SS_CALL_MDL_OUTPUTS ) ; eo3w5kdmqj = _rtP -> P_118 * _rtB -> bwx0anqw5s ;
jsxxec1j5w = muDoubleScalarCos ( aoer0ike4l ) ; _rtB -> ddlkzdtlxo =
eo3w5kdmqj * jsxxec1j5w ; if ( _rtDW -> p0mjtkssxb != 0 ) { _rtB ->
i0uvbrdpcl = _rtDW -> fv1uvl41xo ; } else { _rtB -> i0uvbrdpcl = _rtP ->
P_119 * _rtB -> ddlkzdtlxo + _rtDW -> fv1uvl41xo ; } if ( hvhzmisfej > _rtP
-> P_121 ) { bpjo4wesci_idx_3 = _rtP -> P_121 ; } else if ( hvhzmisfej < _rtP
-> P_122 ) { bpjo4wesci_idx_3 = _rtP -> P_122 ; } else { bpjo4wesci_idx_3 =
hvhzmisfej ; } jp5ua23i4a = 1.0 / bpjo4wesci_idx_3 / 0.0001 ;
bpjo4wesci_idx_0 = muDoubleScalarCeil ( jp5ua23i4a ) ; _rtB -> kxbdjf44uc =
_rtP -> P_123 * bpjo4wesci_idx_0 ; ssCallAccelRunBlock ( S , 3 , 134 ,
SS_CALL_MDL_OUTPUTS ) ; bpjo4wesci_idx_0 = jp5ua23i4a - bpjo4wesci_idx_0 ; if
( ssGetTaskTime ( S , 4 ) < _rtP -> P_126 ) { bpjo4wesci_idx_3 = _rtP ->
P_127 ; } else { bpjo4wesci_idx_3 = _rtP -> P_128 ; } if ( bpjo4wesci_idx_3
>= _rtP -> P_130 ) { bpjo4wesci_idx_0 = ( ( _rtB -> ddlkzdtlxo - _rtDW ->
mo5i5rvr5i ) * bpjo4wesci_idx_0 * _rtP -> P_125 + _rtB -> ddlkzdtlxo ) * (
bpjo4wesci_idx_0 / jp5ua23i4a ) + ( _rtB -> i0uvbrdpcl - _rtB -> kgfp1gk3m2 )
* hvhzmisfej ; } else { bpjo4wesci_idx_0 = _rtP -> P_129 ; } _rtB ->
f250f4qnzj = _rtP -> P_131 ; if ( ssIsMajorTimeStep ( S ) ) { if ( _rtB ->
f250f4qnzj > 0.0 ) { if ( ! _rtDW -> m1v05clsvv ) { if ( ssGetTaskTime ( S ,
4 ) != ssGetTStart ( S ) ) { ssSetSolverNeedsReset ( S ) ; } _rtDW ->
i5rjtcs5sd = 1U ; _rtDW -> j1qqayjy1b = 1U ; _rtDW -> m1v05clsvv = true ; } }
else { if ( _rtDW -> m1v05clsvv ) { ssSetSolverNeedsReset ( S ) ; _rtDW ->
m1v05clsvv = false ; } } } if ( _rtDW -> m1v05clsvv ) { _rtB -> jkovacda1u [
0 ] = _rtP -> P_2 * muDoubleScalarSin ( aoer0ike4l ) * eo3w5kdmqj ; _rtB ->
jkovacda1u [ 1 ] = _rtP -> P_2 * jsxxec1j5w * eo3w5kdmqj ; if ( _rtDW ->
i5rjtcs5sd != 0 ) { _rtB -> ptmdu1k3aj = _rtDW -> hbwmzyylnb ; } else { _rtB
-> ptmdu1k3aj = _rtP -> P_3 * _rtB -> jkovacda1u [ 0 ] + _rtDW -> hbwmzyylnb
; } if ( hvhzmisfej > _rtP -> P_5 ) { bpjo4wesci_idx_3 = _rtP -> P_5 ; } else
if ( hvhzmisfej < _rtP -> P_6 ) { bpjo4wesci_idx_3 = _rtP -> P_6 ; } else {
bpjo4wesci_idx_3 = hvhzmisfej ; } eo3w5kdmqj = 1.0 / bpjo4wesci_idx_3 /
0.0001 ; jsxxec1j5w = muDoubleScalarCeil ( eo3w5kdmqj ) ; _rtB -> ojmqkmcuoj
= _rtP -> P_7 * jsxxec1j5w ; ssCallAccelRunBlock ( S , 1 , 7 ,
SS_CALL_MDL_OUTPUTS ) ; jsxxec1j5w = eo3w5kdmqj - jsxxec1j5w ; if (
ssGetTaskTime ( S , 4 ) < _rtP -> P_10 ) { bpjo4wesci_idx_3 = _rtP -> P_11 ;
} else { bpjo4wesci_idx_3 = _rtP -> P_12 ; } if ( bpjo4wesci_idx_3 >= _rtP ->
P_14 ) { jsxxec1j5w = ( ( _rtB -> jkovacda1u [ 0 ] - _rtDW -> es20fy2lvr ) *
jsxxec1j5w * _rtP -> P_9 + _rtB -> jkovacda1u [ 0 ] ) * ( jsxxec1j5w /
eo3w5kdmqj ) + ( _rtB -> ptmdu1k3aj - _rtB -> keckzqo4yo ) * hvhzmisfej ; }
else { jsxxec1j5w = _rtP -> P_13 ; } if ( _rtDW -> j1qqayjy1b != 0 ) { _rtB
-> np3w5lg3ur = _rtDW -> ltialakutr ; } else { _rtB -> np3w5lg3ur = _rtP ->
P_15 * _rtB -> jkovacda1u [ 1 ] + _rtDW -> ltialakutr ; } if ( hvhzmisfej >
_rtP -> P_17 ) { bpjo4wesci_idx_3 = _rtP -> P_17 ; } else if ( hvhzmisfej <
_rtP -> P_18 ) { bpjo4wesci_idx_3 = _rtP -> P_18 ; } else { bpjo4wesci_idx_3
= hvhzmisfej ; } eo3w5kdmqj = 1.0 / bpjo4wesci_idx_3 / 0.0001 ; jp5ua23i4a =
muDoubleScalarCeil ( eo3w5kdmqj ) ; _rtB -> ogknms4cd3 = _rtP -> P_19 *
jp5ua23i4a ; ssCallAccelRunBlock ( S , 1 , 27 , SS_CALL_MDL_OUTPUTS ) ;
jp5ua23i4a = eo3w5kdmqj - jp5ua23i4a ; if ( ssGetTaskTime ( S , 4 ) < _rtP ->
P_22 ) { bpjo4wesci_idx_3 = _rtP -> P_23 ; } else { bpjo4wesci_idx_3 = _rtP
-> P_24 ; } if ( bpjo4wesci_idx_3 >= _rtP -> P_26 ) { jp5ua23i4a = ( ( _rtB
-> jkovacda1u [ 1 ] - _rtDW -> e3hcogjryj ) * jp5ua23i4a * _rtP -> P_21 +
_rtB -> jkovacda1u [ 1 ] ) * ( jp5ua23i4a / eo3w5kdmqj ) + ( _rtB ->
np3w5lg3ur - _rtB -> fu1dru4srm ) * hvhzmisfej ; } else { jp5ua23i4a = _rtP
-> P_25 ; } jsxxec1j5w = muDoubleScalarHypot ( jsxxec1j5w , jp5ua23i4a ) ; if
( jsxxec1j5w > _rtP -> P_27 ) { jsxxec1j5w = _rtP -> P_27 ; } else { if (
jsxxec1j5w < _rtP -> P_28 ) { jsxxec1j5w = _rtP -> P_28 ; } } _rtB ->
ku3yiw1fh4 = 1.0 / jsxxec1j5w ; if ( ssIsMajorTimeStep ( S ) ) { srUpdateBC (
_rtDW -> bzrv5d0tmm ) ; } } bpjo4wesci_idx_0 *= _rtB -> ku3yiw1fh4 ; _rtB ->
o4nzysnrhm = _rtP -> P_137 * bpjo4wesci_idx_0 * _rtP -> P_138 ;
bpjo4wesci_idx_3 = ( _rtP -> P_132 * bpjo4wesci_idx_0 + _rtDW -> n1uqml3zbu )
+ ( _rtB -> o4nzysnrhm - _rtDW -> j1gf540g2s ) ; if ( bpjo4wesci_idx_3 > _rtP
-> P_140 ) { _rtB -> m53khf4jhi = _rtP -> P_140 ; } else if (
bpjo4wesci_idx_3 < _rtP -> P_141 ) { _rtB -> m53khf4jhi = _rtP -> P_141 ; }
else { _rtB -> m53khf4jhi = bpjo4wesci_idx_3 ; } bpjo4wesci_idx_3 = _rtP ->
P_142 * _rtB -> m53khf4jhi - _rtDW -> j0jutlrn4j ; if ( bpjo4wesci_idx_3 >
_rtP -> P_144 ) { bpjo4wesci_idx_3 = _rtP -> P_144 ; } else { if (
bpjo4wesci_idx_3 < _rtP -> P_145 ) { bpjo4wesci_idx_3 = _rtP -> P_145 ; } }
_rtB -> okjfmmt5gr = bpjo4wesci_idx_3 + _rtDW -> j0jutlrn4j ; { _rtB ->
fxmnjmycj4 = ( _rtP -> P_148 [ 0 ] ) * _rtDW -> opbvzhddhh [ 0 ] + ( _rtP ->
P_148 [ 1 ] ) * _rtDW -> opbvzhddhh [ 1 ] ; _rtB -> fxmnjmycj4 += _rtP ->
P_149 * _rtB -> okjfmmt5gr ; } _rtB -> byppjewfsv = _rtP -> P_151 *
bpjo4wesci_idx_0 ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> gql1xexo0i
= _rtP -> P_152 ; } ib4jdeu3b2 = ssGetT ( S ) ; if ( ssIsSampleHit ( S , 1 ,
0 ) ) { fmbjzdfypj = _rtP -> P_154 ; _rtB -> de4wnophms = _rtP -> P_155 *
_rtP -> P_154 ; _rtB -> awekxtjz5k = _rtP -> P_156 ; _rtB -> poycpjgrw4 =
_rtP -> P_157 ; } if ( ssIsSampleHit ( S , 2 , 0 ) ) { if ( jj21tnwlcr > _rtP
-> P_158 ) { _rtB -> age53lx4i0 = _rtP -> P_158 ; } else if ( jj21tnwlcr <
_rtP -> P_159 ) { _rtB -> age53lx4i0 = _rtP -> P_159 ; } else { _rtB ->
age53lx4i0 = jj21tnwlcr ; } } if ( _rtB -> poycpjgrw4 >= _rtP -> P_160 ) {
_rtB -> pqlw2k1viz = muDoubleScalarSin ( ( _rtP -> P_153 * ib4jdeu3b2 + _rtB
-> de4wnophms ) + _rtB -> awekxtjz5k ) * _rtB -> gql1xexo0i ; } else { _rtB
-> pqlw2k1viz = _rtB -> age53lx4i0 ; } ib4jdeu3b2 = ssGetT ( S ) ; _rtB ->
ls1o22eqaq = ssGetTStart ( S ) ; ib4jdeu3b2 -= _rtB -> ls1o22eqaq ; if (
ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> lkydcnkkai = _rtP -> P_161 ; }
ib4jdeu3b2 = muDoubleScalarRem ( ib4jdeu3b2 , _rtB -> lkydcnkkai ) ; _rtB ->
ew3slzrgoh = look1_binlxpw ( ib4jdeu3b2 , _rtP -> P_163 , _rtP -> P_162 , 3U
) ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { if ( ssIsMajorTimeStep ( S ) ) {
_rtDW -> m4eictromn = ( _rtB -> pqlw2k1viz >= _rtB -> ew3slzrgoh ) ; }
ckeb1s2dlz = _rtDW -> m4eictromn ; hitkasikyj = ! _rtDW -> m4eictromn ; }
_rtB -> pxly0twee5 = _rtP -> P_164 * _rtB -> pqlw2k1viz ; if ( ssIsSampleHit
( S , 1 , 0 ) ) { if ( ssIsMajorTimeStep ( S ) ) { _rtDW -> krdkwp0a5c = (
_rtB -> pxly0twee5 >= _rtB -> ew3slzrgoh ) ; } _rtB -> jcmkitb2fr [ 0 ] =
ckeb1s2dlz ; _rtB -> jcmkitb2fr [ 1 ] = hitkasikyj ; _rtB -> jcmkitb2fr [ 2 ]
= _rtDW -> krdkwp0a5c ; _rtB -> jcmkitb2fr [ 3 ] = ! _rtDW -> krdkwp0a5c ; }
ib4jdeu3b2 = ssGetT ( S ) ; _rtB -> lpopvuj5ic = ssGetTStart ( S ) ;
ib4jdeu3b2 -= _rtB -> lpopvuj5ic ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB
-> j2i0q2wswi = _rtP -> P_165 ; } ib4jdeu3b2 = muDoubleScalarRem ( ib4jdeu3b2
, _rtB -> j2i0q2wswi ) ; _rtB -> dlhtdcdygr = look1_binlxpw ( ib4jdeu3b2 ,
_rtP -> P_167 , _rtP -> P_166 , 2U ) ; ssCallAccelRunBlock ( S , 3 , 198 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB ->
a44b4vzxio = _rtP -> P_168 ; fmbjzdfypj = _rtP -> P_169 ; } _rtB ->
eac1vv1tjj = 0.0 ; _rtB -> eac1vv1tjj += _rtP -> P_171 * _rtX -> n52bxvtwu4 ;
if ( ssIsSampleHit ( S , 4 , 0 ) ) { _rtB -> bnupqq2zu2 = _rtB -> eac1vv1tjj
; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { oossyunrst = muDoubleScalarSign (
_rtB -> bnupqq2zu2 - _rtDW -> b0kcb2fd0b ) ; } _rtB -> k0q4iswykt = 0.0 ;
_rtB -> k0q4iswykt += _rtP -> P_174 * _rtX -> fahgdobdau ; if ( ssIsSampleHit
( S , 4 , 0 ) ) { _rtB -> c1i1vyc0f5 = _rtB -> bnupqq2zu2 * _rtB ->
k0q4iswykt ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> bguocwjkex =
muDoubleScalarSign ( muDoubleScalarSign ( _rtB -> c1i1vyc0f5 - _rtDW ->
osnm2k1bun ) * oossyunrst ) * _rtP -> P_176 + _rtDW -> kaozvtc11k ; _rtB ->
o3s4d0bhno = fmbjzdfypj + _rtB -> bguocwjkex ; } if ( ssIsSampleHit ( S , 4 ,
0 ) ) { if ( _rtB -> o3s4d0bhno > _rtP -> P_178 ) { _rtB -> dbldqrj4a3 = _rtP
-> P_178 ; } else if ( _rtB -> o3s4d0bhno < _rtP -> P_179 ) { _rtB ->
dbldqrj4a3 = _rtP -> P_179 ; } else { _rtB -> dbldqrj4a3 = _rtB -> o3s4d0bhno
; } } _rtB -> j3cvophzir = _rtB -> dbldqrj4a3 - _rtB -> dlhtdcdygr ; if (
ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> aygt4axy1l = _rtP -> P_180 ; } if (
ssIsMajorTimeStep ( S ) ) { _rtDW -> oqbijwo5xn = ( _rtB -> j3cvophzir >=
_rtP -> P_181 ) ; } if ( _rtDW -> oqbijwo5xn ) { _rtB -> dlhtdcdygr = _rtB ->
a44b4vzxio ; } else { _rtB -> dlhtdcdygr = _rtB -> aygt4axy1l ; }
ssCallAccelRunBlock ( S , 3 , 224 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
lftg4azy0s = _rtB -> dlhtdcdygr * _rtB -> czop1dz2xx [ 2 ] ; if (
ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> nmolartuek [ 0 ] = _rtB -> gtvmdf51i0
; _rtB -> nmolartuek [ 1 ] = _rtB -> lwiqst51ht ; ssCallAccelRunBlock ( S , 3
, 235 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> ikx4yqfnno = _rtP -> P_186 * _rtB ->
at4svfakyj [ 8 ] ; _rtB -> ip3yrber15 = _rtP -> P_187 * _rtB -> at4svfakyj [
11 ] ; ssCallAccelRunBlock ( S , 3 , 238 , SS_CALL_MDL_OUTPUTS ) ; }
UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { real_T HoldSine ;
n3qi1whofz * _rtB ; loikxjbxjg * _rtP ; ew10rzwqr2 * _rtDW ; _rtDW = ( (
ew10rzwqr2 * ) ssGetRootDWork ( S ) ) ; _rtP = ( ( loikxjbxjg * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ;
if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtDW -> pd3luz4avz [ 0 ] = _rtB ->
k2gt5xfkuo [ 0 ] ; _rtDW -> pd3luz4avz [ 1 ] = _rtB -> k2gt5xfkuo [ 1 ] ;
_rtDW -> pd3luz4avz [ 2 ] = _rtB -> k2gt5xfkuo [ 2 ] ; _rtDW -> pd3luz4avz [
3 ] = _rtB -> k2gt5xfkuo [ 3 ] ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtDW
-> b2gcy3vb2a [ 0 ] = _rtB -> jcmkitb2fr [ 0 ] ; _rtDW -> b2gcy3vb2a [ 1 ] =
_rtB -> jcmkitb2fr [ 1 ] ; _rtDW -> b2gcy3vb2a [ 2 ] = _rtB -> jcmkitb2fr [ 2
] ; _rtDW -> b2gcy3vb2a [ 3 ] = _rtB -> jcmkitb2fr [ 3 ] ; } if ( _rtDW ->
byh0btktux && ssIsSampleHit ( S , 2 , 0 ) ) { HoldSine = _rtP -> P_32 * _rtB
-> ecey0q2vme ; _rtDW -> l4dlj3krq0 [ 0 ] += HoldSine ; _rtDW -> l4dlj3krq0 [
1 ] += HoldSine ; _rtDW -> l4dlj3krq0 [ 2 ] += HoldSine ; _rtDW -> l4dlj3krq0
[ 3 ] += HoldSine ; if ( _rtB -> e3hpbcnjjc [ 0 ] > 0.0 ) { _rtDW ->
b13awnigwz [ 0 ] = 1 ; } else if ( _rtB -> e3hpbcnjjc [ 0 ] < 0.0 ) { _rtDW
-> b13awnigwz [ 0 ] = - 1 ; } else if ( _rtB -> e3hpbcnjjc [ 0 ] == 0.0 ) {
_rtDW -> b13awnigwz [ 0 ] = 0 ; } else { _rtDW -> b13awnigwz [ 0 ] = 2 ; } if
( _rtB -> e3hpbcnjjc [ 1 ] > 0.0 ) { _rtDW -> b13awnigwz [ 1 ] = 1 ; } else
if ( _rtB -> e3hpbcnjjc [ 1 ] < 0.0 ) { _rtDW -> b13awnigwz [ 1 ] = - 1 ; }
else if ( _rtB -> e3hpbcnjjc [ 1 ] == 0.0 ) { _rtDW -> b13awnigwz [ 1 ] = 0 ;
} else { _rtDW -> b13awnigwz [ 1 ] = 2 ; } if ( _rtB -> e3hpbcnjjc [ 2 ] >
0.0 ) { _rtDW -> b13awnigwz [ 2 ] = 1 ; } else if ( _rtB -> e3hpbcnjjc [ 2 ]
< 0.0 ) { _rtDW -> b13awnigwz [ 2 ] = - 1 ; } else if ( _rtB -> e3hpbcnjjc [
2 ] == 0.0 ) { _rtDW -> b13awnigwz [ 2 ] = 0 ; } else { _rtDW -> b13awnigwz [
2 ] = 2 ; } if ( _rtB -> e3hpbcnjjc [ 3 ] > 0.0 ) { _rtDW -> b13awnigwz [ 3 ]
= 1 ; } else if ( _rtB -> e3hpbcnjjc [ 3 ] < 0.0 ) { _rtDW -> b13awnigwz [ 3
] = - 1 ; } else if ( _rtB -> e3hpbcnjjc [ 3 ] == 0.0 ) { _rtDW -> b13awnigwz
[ 3 ] = 0 ; } else { _rtDW -> b13awnigwz [ 3 ] = 2 ; } _rtDW -> bxpfaqqn5g [
0 ] = _rtB -> peonb2s13j [ 0 ] ; _rtDW -> bxpfaqqn5g [ 1 ] = _rtB ->
peonb2s13j [ 1 ] ; _rtDW -> bxpfaqqn5g [ 2 ] = _rtB -> peonb2s13j [ 2 ] ;
_rtDW -> bxpfaqqn5g [ 3 ] = _rtB -> peonb2s13j [ 3 ] ; } if ( ssIsSampleHit (
S , 2 , 0 ) ) { HoldSine = _rtDW -> gzppta2yd5 ; _rtDW -> gzppta2yd5 = _rtDW
-> gzppta2yd5 * _rtP -> P_52 + _rtDW -> h4dq2buhxq * _rtP -> P_51 ; _rtDW ->
h4dq2buhxq = _rtDW -> h4dq2buhxq * _rtP -> P_52 - HoldSine * _rtP -> P_51 ;
ssCallAccelRunBlock ( S , 0 , 6 , SS_CALL_MDL_UPDATE ) ; _rtDW -> k5u0blc5by
= _rtB -> at4svfakyj [ 1 ] ; _rtDW -> kn5xrzh2lm = _rtB -> p4uajk50sn ; } {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> bxxpgfrdrd . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> bxxpgfrdrd . TUbufferPtrs
[ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW -> cmbq4gun1g . Head = ( (
_rtDW -> cmbq4gun1g . Head < ( _rtDW -> cmbq4gun1g . CircularBufSize - 1 ) )
? ( _rtDW -> cmbq4gun1g . Head + 1 ) : 0 ) ; if ( _rtDW -> cmbq4gun1g . Head
== _rtDW -> cmbq4gun1g . Tail ) { if ( !
PV_Single_Phase_inverter_afd_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
cmbq4gun1g . CircularBufSize , & _rtDW -> cmbq4gun1g . Tail , & _rtDW ->
cmbq4gun1g . Head , & _rtDW -> cmbq4gun1g . Last , simTime - _rtP -> P_99 ,
tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW ->
cmbq4gun1g . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
cmbq4gun1g . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> cmbq4gun1g . Head ]
= _rtB -> apdiuhihpb ; } if ( ssIsSampleHit ( S , 4 , 0 ) ) { _rtDW ->
bgv2ulrz1x = _rtB -> fxmnjmycj4 ; _rtDW -> lkjsqizslz += _rtP -> P_102 * _rtB
-> m53khf4jhi ; ssCallAccelRunBlock ( S , 3 , 100 , SS_CALL_MDL_UPDATE ) ; }
if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtDW -> dxwkuh3yet = _rtDW ->
jdlga5gylp ; _rtDW -> b3cy3g4cyx += _rtP -> P_109 * _rtB -> dpstypxipj ; if (
_rtDW -> b3cy3g4cyx >= _rtP -> P_111 ) { _rtDW -> b3cy3g4cyx = _rtP -> P_111
; } else { if ( _rtDW -> b3cy3g4cyx <= _rtP -> P_112 ) { _rtDW -> b3cy3g4cyx
= _rtP -> P_112 ; } } } if ( ssIsSampleHit ( S , 4 , 0 ) ) { _rtDW ->
p0mjtkssxb = 0U ; _rtDW -> fv1uvl41xo = _rtP -> P_119 * _rtB -> ddlkzdtlxo +
_rtB -> i0uvbrdpcl ; ssCallAccelRunBlock ( S , 3 , 134 , SS_CALL_MDL_UPDATE )
; _rtDW -> mo5i5rvr5i = _rtB -> ddlkzdtlxo ; if ( _rtDW -> m1v05clsvv ) {
_rtDW -> i5rjtcs5sd = 0U ; _rtDW -> hbwmzyylnb = _rtP -> P_3 * _rtB ->
jkovacda1u [ 0 ] + _rtB -> ptmdu1k3aj ; ssCallAccelRunBlock ( S , 1 , 7 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> es20fy2lvr = _rtB -> jkovacda1u [ 0 ] ; _rtDW
-> j1qqayjy1b = 0U ; _rtDW -> ltialakutr = _rtP -> P_15 * _rtB -> jkovacda1u
[ 1 ] + _rtB -> np3w5lg3ur ; ssCallAccelRunBlock ( S , 1 , 27 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> e3hcogjryj = _rtB -> jkovacda1u [ 1 ] ; }
_rtDW -> n1uqml3zbu += _rtP -> P_133 * _rtB -> byppjewfsv ; if ( _rtDW ->
n1uqml3zbu >= _rtP -> P_135 ) { _rtDW -> n1uqml3zbu = _rtP -> P_135 ; } else
{ if ( _rtDW -> n1uqml3zbu <= _rtP -> P_136 ) { _rtDW -> n1uqml3zbu = _rtP ->
P_136 ; } } _rtDW -> j1gf540g2s = _rtB -> o4nzysnrhm ; _rtDW -> j0jutlrn4j =
_rtB -> okjfmmt5gr ; { real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_146 [ 0 ]
) * _rtDW -> opbvzhddhh [ 0 ] + ( _rtP -> P_146 [ 1 ] ) * _rtDW -> opbvzhddhh
[ 1 ] ; xnew [ 0 ] += ( _rtP -> P_147 [ 0 ] ) * _rtB -> okjfmmt5gr ; xnew [ 1
] = ( _rtP -> P_146 [ 2 ] ) * _rtDW -> opbvzhddhh [ 0 ] + ( _rtP -> P_146 [ 3
] ) * _rtDW -> opbvzhddhh [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_147 [ 1 ] ) *
_rtB -> okjfmmt5gr ; ( void ) memcpy ( & _rtDW -> opbvzhddhh [ 0 ] , xnew ,
sizeof ( real_T ) * 2 ) ; } } if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtDW ->
b0kcb2fd0b = _rtB -> bnupqq2zu2 ; _rtDW -> osnm2k1bun = _rtB -> c1i1vyc0f5 ;
_rtDW -> kaozvtc11k = _rtB -> bguocwjkex ; } if ( ssIsSampleHit ( S , 2 , 0 )
) { ssCallAccelRunBlock ( S , 3 , 235 , SS_CALL_MDL_UPDATE ) ; }
UNUSED_PARAMETER ( tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { n3qi1whofz * _rtB ; loikxjbxjg
* _rtP ; f1xhd02yjc * _rtX ; pqmvzr1kvu * _rtXdot ; _rtXdot = ( ( pqmvzr1kvu
* ) ssGetdX ( S ) ) ; _rtX = ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) ;
_rtP = ( ( loikxjbxjg * ) ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz *
) _ssGetBlockIO ( S ) ) ; _rtXdot -> n52bxvtwu4 = 0.0 ; _rtXdot -> n52bxvtwu4
+= _rtP -> P_170 * _rtX -> n52bxvtwu4 ; _rtXdot -> n52bxvtwu4 += _rtB ->
gtvmdf51i0 ; _rtXdot -> fahgdobdau = 0.0 ; _rtXdot -> fahgdobdau += _rtP ->
P_173 * _rtX -> fahgdobdau ; _rtXdot -> fahgdobdau += _rtB -> lwiqst51ht ;
_rtXdot -> pjbcugwxf0 = 0.0 ; _rtXdot -> pjbcugwxf0 += _rtP -> P_182 * _rtX
-> pjbcugwxf0 ; _rtXdot -> pjbcugwxf0 += _rtB -> gtvmdf51i0 ; _rtXdot ->
nsp3ji42uc = 0.0 ; _rtXdot -> nsp3ji42uc += _rtP -> P_184 * _rtX ->
nsp3ji42uc ; _rtXdot -> nsp3ji42uc += _rtB -> lwiqst51ht ; }
#define MDL_ZERO_CROSSINGS
static void mdlZeroCrossings ( SimStruct * S ) { n3qi1whofz * _rtB ;
loikxjbxjg * _rtP ; d15n1ti4kq * _rtZCSV ; _rtZCSV = ( ( d15n1ti4kq * )
ssGetSolverZcSignalVector ( S ) ) ; _rtP = ( ( loikxjbxjg * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ;
_rtZCSV -> bznktecnfh = ssGetT ( S ) - _rtP -> P_77 ; _rtZCSV -> j2wg3uhh42 =
_rtB -> pqlw2k1viz - _rtB -> ew3slzrgoh ; _rtZCSV -> osxehvr00q = _rtB ->
pxly0twee5 - _rtB -> ew3slzrgoh ; _rtZCSV -> gxb5rs1fuh = _rtB -> j3cvophzir
- _rtP -> P_181 ; } static void mdlInitializeSizes ( SimStruct * S ) {
ssSetChecksumVal ( S , 0 , 2667993726U ) ; ssSetChecksumVal ( S , 1 ,
145389658U ) ; ssSetChecksumVal ( S , 2 , 3254011176U ) ; ssSetChecksumVal (
S , 3 , 2800692331U ) ; { mxArray * slVerStructMat = NULL ; mxArray *
slStrMat = mxCreateString ( "simulink" ) ; char slVerChar [ 10 ] ; int status
= mexCallMATLAB ( 1 , & slVerStructMat , 1 , & slStrMat , "ver" ) ; if (
status == 0 ) { mxArray * slVerMat = mxGetField ( slVerStructMat , 0 ,
"Version" ) ; if ( slVerMat == NULL ) { status = 1 ; } else { status =
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
loikxjbxjg * ) ssGetDefaultParam ( S ) ) -> P_96 = rtInf ; } static void
mdlInitializeSampleTimes ( SimStruct * S ) { } static void mdlTerminate (
SimStruct * S ) { }
#include "simulink.c"
