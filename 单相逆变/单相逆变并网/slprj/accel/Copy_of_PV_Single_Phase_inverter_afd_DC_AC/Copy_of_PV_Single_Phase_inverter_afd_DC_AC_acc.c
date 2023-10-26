#include "__cf_Copy_of_PV_Single_Phase_inverter_afd_DC_AC.h"
#include <math.h>
#include "Copy_of_PV_Single_Phase_inverter_afd_DC_AC_acc.h"
#include "Copy_of_PV_Single_Phase_inverter_afd_DC_AC_acc_private.h"
#include <stdio.h>
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T
Copy_of_PV_Single_Phase_inverter_afd_DC_AC_acc_rt_TDelayUpdateTailOrGrowBuf (
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
Copy_of_PV_Single_Phase_inverter_afd_DC_AC_acc_rt_TDelayInterpolate ( real_T
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
mdlOutputs ( SimStruct * S , int_T tid ) { real_T fg0atln1ju ; real_T
fwujhzb4et ; real_T htkct3cv1q ; boolean_T hitkasikyj ; real_T lpaxahqxhq ;
boolean_T ckeb1s2dlz ; real_T aoer0ike4l ; real_T pw1ypam5j4 ; real_T
hwjiit4ixv ; real_T lnlspp1hj1 ; real_T hvhzmisfej ; real_T eo3w5kdmqj ;
real_T jsxxec1j5w ; real_T bpjo4wesci_idx_0 ; real_T bpjo4wesci_idx_1 ;
real_T bpjo4wesci_idx_2 ; real_T bpjo4wesci_idx_3 ; n3qi1whofz * _rtB ;
loikxjbxjg * _rtP ; ew10rzwqr2 * _rtDW ; _rtDW = ( ( ew10rzwqr2 * )
ssGetRootDWork ( S ) ) ; _rtP = ( ( loikxjbxjg * ) ssGetDefaultParam ( S ) )
; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ; if ( ssIsSampleHit ( S ,
2 , 0 ) ) { fwujhzb4et = _rtP -> P_43 ; _rtB -> jrewwolk22 = _rtP -> P_44 ;
_rtB -> lbxyutubre [ 0 ] = _rtDW -> pd3luz4avz [ 0 ] ; _rtB -> lbxyutubre [ 1
] = _rtDW -> pd3luz4avz [ 1 ] ; _rtB -> lbxyutubre [ 2 ] = _rtDW ->
pd3luz4avz [ 2 ] ; _rtB -> lbxyutubre [ 3 ] = _rtDW -> pd3luz4avz [ 3 ] ; }
if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> e3hpbcnjjc [ 0 ] = _rtDW ->
b2gcy3vb2a [ 0 ] ; _rtB -> e3hpbcnjjc [ 1 ] = _rtDW -> b2gcy3vb2a [ 1 ] ;
_rtB -> e3hpbcnjjc [ 2 ] = _rtDW -> b2gcy3vb2a [ 2 ] ; _rtB -> e3hpbcnjjc [ 3
] = _rtDW -> b2gcy3vb2a [ 3 ] ; _rtB -> nrgiikxpzm = _rtP -> P_169 ; if (
ssIsMajorTimeStep ( S ) ) { if ( _rtB -> nrgiikxpzm ) { if ( ! _rtDW ->
byh0btktux ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetSolverNeedsReset ( S ) ; } _rtDW -> l4dlj3krq0 [ 0 ] = _rtP -> P_32 ;
_rtDW -> l4dlj3krq0 [ 1 ] = _rtP -> P_32 ; _rtDW -> l4dlj3krq0 [ 2 ] = _rtP
-> P_32 ; _rtDW -> l4dlj3krq0 [ 3 ] = _rtP -> P_32 ; _rtDW -> b13awnigwz [ 0
] = 2 ; _rtDW -> b13awnigwz [ 1 ] = 2 ; _rtDW -> b13awnigwz [ 2 ] = 2 ; _rtDW
-> b13awnigwz [ 3 ] = 2 ; _rtDW -> bxpfaqqn5g [ 0 ] = _rtP -> P_41 ; _rtDW ->
bxpfaqqn5g [ 1 ] = _rtP -> P_41 ; _rtDW -> bxpfaqqn5g [ 2 ] = _rtP -> P_41 ;
_rtDW -> bxpfaqqn5g [ 3 ] = _rtP -> P_41 ; _rtDW -> byh0btktux = true ; } }
else { if ( _rtDW -> byh0btktux ) { ssSetSolverNeedsReset ( S ) ; _rtDW ->
byh0btktux = false ; } } } } if ( _rtDW -> byh0btktux ) { if ( ssIsSampleHit
( S , 2 , 0 ) ) { _rtB -> ecey0q2vme = _rtP -> P_29 ; } if ( ssIsSampleHit (
S , 1 , 0 ) ) { eo3w5kdmqj = _rtP -> P_30 ; } if ( ssIsSampleHit ( S , 2 , 0
) ) { if ( ( _rtB -> e3hpbcnjjc [ 0 ] <= 0.0 ) && ( _rtDW -> b13awnigwz [ 0 ]
== 1 ) ) { _rtDW -> l4dlj3krq0 [ 0 ] = _rtP -> P_32 ; } if ( ( _rtB ->
e3hpbcnjjc [ 1 ] <= 0.0 ) && ( _rtDW -> b13awnigwz [ 1 ] == 1 ) ) { _rtDW ->
l4dlj3krq0 [ 1 ] = _rtP -> P_32 ; } if ( ( _rtB -> e3hpbcnjjc [ 2 ] <= 0.0 )
&& ( _rtDW -> b13awnigwz [ 2 ] == 1 ) ) { _rtDW -> l4dlj3krq0 [ 2 ] = _rtP ->
P_32 ; } if ( ( _rtB -> e3hpbcnjjc [ 3 ] <= 0.0 ) && ( _rtDW -> b13awnigwz [
3 ] == 1 ) ) { _rtDW -> l4dlj3krq0 [ 3 ] = _rtP -> P_32 ; } _rtB ->
igl1rmdosy [ 0 ] = _rtDW -> l4dlj3krq0 [ 0 ] ; _rtB -> igl1rmdosy [ 1 ] =
_rtDW -> l4dlj3krq0 [ 1 ] ; _rtB -> igl1rmdosy [ 2 ] = _rtDW -> l4dlj3krq0 [
2 ] ; _rtB -> igl1rmdosy [ 3 ] = _rtDW -> l4dlj3krq0 [ 3 ] ; } if (
ssIsSampleHit ( S , 1 , 0 ) ) { jsxxec1j5w = _rtP -> P_33 ; } if (
ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> hceqogjf5t [ 0 ] = _rtP -> P_34 *
_rtB -> igl1rmdosy [ 0 ] ; _rtB -> hceqogjf5t [ 1 ] = _rtP -> P_34 * _rtB ->
igl1rmdosy [ 1 ] ; _rtB -> hceqogjf5t [ 2 ] = _rtP -> P_34 * _rtB ->
igl1rmdosy [ 2 ] ; _rtB -> hceqogjf5t [ 3 ] = _rtP -> P_34 * _rtB ->
igl1rmdosy [ 3 ] ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { bpjo4wesci_idx_3 =
jsxxec1j5w + _rtB -> hceqogjf5t [ 0 ] ; lnlspp1hj1 = ( _rtP -> P_37 - _rtB ->
igl1rmdosy [ 0 ] ) * _rtP -> P_38 ; if ( bpjo4wesci_idx_3 > _rtP -> P_35 ) {
bpjo4wesci_idx_3 = _rtP -> P_35 ; } else { if ( bpjo4wesci_idx_3 < _rtP ->
P_36 ) { bpjo4wesci_idx_3 = _rtP -> P_36 ; } } if ( lnlspp1hj1 > _rtP -> P_39
) { lnlspp1hj1 = _rtP -> P_39 ; } else { if ( lnlspp1hj1 < _rtP -> P_40 ) {
lnlspp1hj1 = _rtP -> P_40 ; } } bpjo4wesci_idx_0 = bpjo4wesci_idx_3 +
lnlspp1hj1 ; bpjo4wesci_idx_3 = jsxxec1j5w + _rtB -> hceqogjf5t [ 1 ] ;
lnlspp1hj1 = ( _rtP -> P_37 - _rtB -> igl1rmdosy [ 1 ] ) * _rtP -> P_38 ; if
( bpjo4wesci_idx_3 > _rtP -> P_35 ) { bpjo4wesci_idx_3 = _rtP -> P_35 ; }
else { if ( bpjo4wesci_idx_3 < _rtP -> P_36 ) { bpjo4wesci_idx_3 = _rtP ->
P_36 ; } } if ( lnlspp1hj1 > _rtP -> P_39 ) { lnlspp1hj1 = _rtP -> P_39 ; }
else { if ( lnlspp1hj1 < _rtP -> P_40 ) { lnlspp1hj1 = _rtP -> P_40 ; } }
bpjo4wesci_idx_1 = bpjo4wesci_idx_3 + lnlspp1hj1 ; bpjo4wesci_idx_3 =
jsxxec1j5w + _rtB -> hceqogjf5t [ 2 ] ; lnlspp1hj1 = ( _rtP -> P_37 - _rtB ->
igl1rmdosy [ 2 ] ) * _rtP -> P_38 ; if ( bpjo4wesci_idx_3 > _rtP -> P_35 ) {
bpjo4wesci_idx_3 = _rtP -> P_35 ; } else { if ( bpjo4wesci_idx_3 < _rtP ->
P_36 ) { bpjo4wesci_idx_3 = _rtP -> P_36 ; } } if ( lnlspp1hj1 > _rtP -> P_39
) { lnlspp1hj1 = _rtP -> P_39 ; } else { if ( lnlspp1hj1 < _rtP -> P_40 ) {
lnlspp1hj1 = _rtP -> P_40 ; } } bpjo4wesci_idx_2 = bpjo4wesci_idx_3 +
lnlspp1hj1 ; bpjo4wesci_idx_3 = jsxxec1j5w + _rtB -> hceqogjf5t [ 3 ] ;
lnlspp1hj1 = ( _rtP -> P_37 - _rtB -> igl1rmdosy [ 3 ] ) * _rtP -> P_38 ; if
( bpjo4wesci_idx_3 > _rtP -> P_35 ) { bpjo4wesci_idx_3 = _rtP -> P_35 ; }
else { if ( bpjo4wesci_idx_3 < _rtP -> P_36 ) { bpjo4wesci_idx_3 = _rtP ->
P_36 ; } } if ( lnlspp1hj1 > _rtP -> P_39 ) { lnlspp1hj1 = _rtP -> P_39 ; }
else { if ( lnlspp1hj1 < _rtP -> P_40 ) { lnlspp1hj1 = _rtP -> P_40 ; } }
bpjo4wesci_idx_3 += lnlspp1hj1 ; } if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB
-> adlprvfww0 [ 0 ] = _rtDW -> bxpfaqqn5g [ 0 ] ; _rtB -> adlprvfww0 [ 1 ] =
_rtDW -> bxpfaqqn5g [ 1 ] ; _rtB -> adlprvfww0 [ 2 ] = _rtDW -> bxpfaqqn5g [
2 ] ; _rtB -> adlprvfww0 [ 3 ] = _rtDW -> bxpfaqqn5g [ 3 ] ; } if (
ssIsSampleHit ( S , 1 , 0 ) ) { if ( _rtB -> e3hpbcnjjc [ 0 ] >= _rtP -> P_42
) { _rtB -> peonb2s13j [ 0 ] = _rtB -> lbxyutubre [ 0 ] ; } else { _rtB ->
peonb2s13j [ 0 ] = _rtB -> adlprvfww0 [ 0 ] ; } if ( _rtB -> e3hpbcnjjc [ 1 ]
>= _rtP -> P_42 ) { _rtB -> peonb2s13j [ 1 ] = _rtB -> lbxyutubre [ 1 ] ; }
else { _rtB -> peonb2s13j [ 1 ] = _rtB -> adlprvfww0 [ 1 ] ; } if ( _rtB ->
e3hpbcnjjc [ 2 ] >= _rtP -> P_42 ) { _rtB -> peonb2s13j [ 2 ] = _rtB ->
lbxyutubre [ 2 ] ; } else { _rtB -> peonb2s13j [ 2 ] = _rtB -> adlprvfww0 [ 2
] ; } if ( _rtB -> e3hpbcnjjc [ 3 ] >= _rtP -> P_42 ) { _rtB -> peonb2s13j [
3 ] = _rtB -> lbxyutubre [ 3 ] ; } else { _rtB -> peonb2s13j [ 3 ] = _rtB ->
adlprvfww0 [ 3 ] ; } _rtB -> auwitbs4pp [ 0 ] = bpjo4wesci_idx_0 * _rtB ->
peonb2s13j [ 0 ] * eo3w5kdmqj ; _rtB -> auwitbs4pp [ 1 ] = bpjo4wesci_idx_1 *
_rtB -> peonb2s13j [ 1 ] * eo3w5kdmqj ; _rtB -> auwitbs4pp [ 2 ] =
bpjo4wesci_idx_2 * _rtB -> peonb2s13j [ 2 ] * eo3w5kdmqj ; _rtB -> auwitbs4pp
[ 3 ] = bpjo4wesci_idx_3 * _rtB -> peonb2s13j [ 3 ] * eo3w5kdmqj ; } if (
ssIsMajorTimeStep ( S ) ) { srUpdateBC ( _rtDW -> cvc2xojcfc ) ; } } if (
ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> iadvgyjfea = _rtP -> P_47 ; } _rtB ->
aizxicgt5d = muDoubleScalarSin ( _rtP -> P_50 * ssGetTaskTime ( S , 0 ) +
_rtP -> P_51 ) * _rtP -> P_48 + _rtP -> P_49 ; if ( ssIsSampleHit ( S , 2 , 0
) ) { ssCallAccelRunBlock ( S , 3 , 8 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
pp4nplutdu = _rtP -> P_52 * _rtB -> ks2dmtukc2 [ 0 ] - _rtDW -> k5u0blc5by ;
_rtB -> gfynberdap = ( _rtB -> pp4nplutdu * _rtDW -> kn5xrzh2lm > fwujhzb4et
) ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { if ( ssIsMajorTimeStep ( S ) ) {
_rtDW -> epz3e0mhiv = ( ssGetTaskTime ( S , 1 ) >= _rtP -> P_55 ) ; } if (
_rtDW -> epz3e0mhiv == 1 ) { lpaxahqxhq = _rtP -> P_57 ; } else { lpaxahqxhq
= _rtP -> P_56 ; } } if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> pqqsptsglu
= _rtP -> P_58 ; fwujhzb4et = ssGetTaskTime ( S , 2 ) ; _rtB -> af31t3fqbj =
rt_Lookup ( _rtP -> P_59 , 6 , fwujhzb4et , _rtP -> P_60 ) ; } if (
ssIsSampleHit ( S , 1 , 0 ) ) { if ( ! ( _rtB -> pqqsptsglu >= _rtP -> P_61 )
) { lpaxahqxhq = _rtB -> af31t3fqbj ; } htkct3cv1q = _rtB -> gfynberdap +
lpaxahqxhq ; if ( htkct3cv1q >= _rtP -> P_62 ) { _rtDW -> pqxnc1gnc0 = true ;
} else { if ( htkct3cv1q <= _rtP -> P_63 ) { _rtDW -> pqxnc1gnc0 = false ; }
} ckeb1s2dlz = ( lpaxahqxhq != 0.0 ) ; if ( _rtDW -> pqxnc1gnc0 ) {
bpjo4wesci_idx_3 = _rtP -> P_64 ; } else { bpjo4wesci_idx_3 = _rtP -> P_65 ;
} hitkasikyj = ( ( bpjo4wesci_idx_3 != 0.0 ) || ( lpaxahqxhq != 0.0 ) ) ;
htkct3cv1q = hitkasikyj ; if ( _rtDW -> pa4mtppeis ) { _rtDW -> pa4mtppeis =
false ; _rtB -> pkxgd5gaap = _rtP -> P_66 ; } else { _rtB -> pkxgd5gaap =
hitkasikyj ; } } if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> fg54u50ndu =
_rtP -> P_67 * _rtB -> ks2dmtukc2 [ 6 ] ; _rtB -> ehk43lsd1i = _rtP -> P_68 *
_rtB -> ks2dmtukc2 [ 8 ] ; _rtB -> es5uk3zgm2 = _rtP -> P_69 * _rtB ->
ehk43lsd1i ; ssCallAccelRunBlock ( S , 3 , 34 , SS_CALL_MDL_OUTPUTS ) ; } if
( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> kawhu05wjr = _rtP -> P_70 ; } {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> bxxpgfrdrd . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> bxxpgfrdrd . TUbufferPtrs
[ 1 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP
-> P_71 ; _rtB -> kcunjjcjza =
Copy_of_PV_Single_Phase_inverter_afd_DC_AC_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * tBuffer , * uBuffer , _rtDW -> cmbq4gun1g .
CircularBufSize , & _rtDW -> cmbq4gun1g . Last , _rtDW -> cmbq4gun1g . Tail ,
_rtDW -> cmbq4gun1g . Head , _rtP -> P_72 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } ssCallAccelRunBlock ( S , 3 , 37 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 3 , 38 , SS_CALL_MDL_OUTPUTS ) ; if ( ssIsSampleHit
( S , 2 , 0 ) ) { fwujhzb4et = _rtP -> P_73 ; if ( _rtB -> epextpowq4 [ 1 ]
>= _rtP -> P_75 ) { bpjo4wesci_idx_3 = _rtP -> P_74 * _rtB -> ks2dmtukc2 [ 1
] ; } else { bpjo4wesci_idx_3 = fwujhzb4et ; } if ( bpjo4wesci_idx_3 > _rtP
-> P_76 ) { _rtB -> pwethyghkk [ 0 ] = _rtP -> P_76 ; } else if (
bpjo4wesci_idx_3 < _rtP -> P_77 ) { _rtB -> pwethyghkk [ 0 ] = _rtP -> P_77 ;
} else { _rtB -> pwethyghkk [ 0 ] = bpjo4wesci_idx_3 ; } if ( _rtB ->
epextpowq4 [ 2 ] >= _rtP -> P_75 ) { bpjo4wesci_idx_3 = _rtP -> P_74 * _rtB
-> ks2dmtukc2 [ 2 ] ; } else { bpjo4wesci_idx_3 = fwujhzb4et ; } if (
bpjo4wesci_idx_3 > _rtP -> P_76 ) { _rtB -> pwethyghkk [ 1 ] = _rtP -> P_76 ;
} else if ( bpjo4wesci_idx_3 < _rtP -> P_77 ) { _rtB -> pwethyghkk [ 1 ] =
_rtP -> P_77 ; } else { _rtB -> pwethyghkk [ 1 ] = bpjo4wesci_idx_3 ; } if (
_rtB -> epextpowq4 [ 3 ] >= _rtP -> P_75 ) { bpjo4wesci_idx_3 = _rtP -> P_74
* _rtB -> ks2dmtukc2 [ 3 ] ; } else { bpjo4wesci_idx_3 = fwujhzb4et ; } if (
bpjo4wesci_idx_3 > _rtP -> P_76 ) { _rtB -> pwethyghkk [ 2 ] = _rtP -> P_76 ;
} else if ( bpjo4wesci_idx_3 < _rtP -> P_77 ) { _rtB -> pwethyghkk [ 2 ] =
_rtP -> P_77 ; } else { _rtB -> pwethyghkk [ 2 ] = bpjo4wesci_idx_3 ; } if (
_rtB -> epextpowq4 [ 4 ] >= _rtP -> P_75 ) { bpjo4wesci_idx_3 = _rtP -> P_74
* _rtB -> ks2dmtukc2 [ 4 ] ; } else { bpjo4wesci_idx_3 = fwujhzb4et ; } if (
bpjo4wesci_idx_3 > _rtP -> P_76 ) { _rtB -> pwethyghkk [ 3 ] = _rtP -> P_76 ;
} else if ( bpjo4wesci_idx_3 < _rtP -> P_77 ) { _rtB -> pwethyghkk [ 3 ] =
_rtP -> P_77 ; } else { _rtB -> pwethyghkk [ 3 ] = bpjo4wesci_idx_3 ; } } if
( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> b5giuegzdb = _rtP -> P_78 ;
htkct3cv1q = _rtP -> P_79 ; lpaxahqxhq = _rtP -> P_80 ; } if ( ssIsSampleHit
( S , 3 , 0 ) ) { _rtB -> luxnpctskm = _rtB -> ehk43lsd1i ; hvhzmisfej =
_rtDW -> bgv2ulrz1x ; aoer0ike4l = muDoubleScalarMod ( _rtDW -> lkjsqizslz ,
_rtP -> P_84 ) ; _rtB -> j3ixfwrps5 [ 0 ] = _rtDW -> bgv2ulrz1x ; _rtB ->
j3ixfwrps5 [ 1 ] = aoer0ike4l ; ssCallAccelRunBlock ( S , 3 , 60 ,
SS_CALL_MDL_OUTPUTS ) ; } _rtB -> dcpw4f1ntp = _rtB -> kcunjjcjza * _rtB ->
ad5xetbkg1 [ 0 ] - _rtB -> luxnpctskm ; if ( ssIsSampleHit ( S , 3 , 0 ) ) {
_rtB -> nmeifexnd0 = _rtP -> P_85 * _rtB -> dcpw4f1ntp ; _rtDW -> jdlga5gylp
= ( _rtB -> nmeifexnd0 - _rtP -> P_87 [ 1 ] * _rtDW -> dxwkuh3yet ) / _rtP ->
P_87 [ 0 ] ; _rtB -> lrtpaprp4h = _rtP -> P_94 * _rtB -> dcpw4f1ntp ;
pw1ypam5j4 = ( _rtP -> P_93 * _rtB -> dcpw4f1ntp + _rtDW -> b3cy3g4cyx ) + (
_rtP -> P_86 [ 0 ] * _rtDW -> jdlga5gylp + _rtP -> P_86 [ 1 ] * _rtDW ->
dxwkuh3yet ) ; if ( pw1ypam5j4 > _rtP -> P_95 ) { hwjiit4ixv = _rtP -> P_95 ;
} else if ( pw1ypam5j4 < _rtP -> P_96 ) { hwjiit4ixv = _rtP -> P_96 ; } else
{ hwjiit4ixv = pw1ypam5j4 ; } } if ( ssIsSampleHit ( S , 1 , 0 ) ) {
bpjo4wesci_idx_1 = _rtP -> P_97 / _rtP -> P_98 ; eo3w5kdmqj =
bpjo4wesci_idx_1 - _rtP -> P_99 ; lnlspp1hj1 = _rtP -> P_99 - _rtP -> P_100 /
_rtP -> P_101 ; jsxxec1j5w = lnlspp1hj1 ; if ( ssIsMajorTimeStep ( S ) ) { if
( _rtDW -> fo1r0adewj != 0 ) { ssSetSolverNeedsReset ( S ) ; _rtDW ->
fo1r0adewj = 0 ; } } else { if ( lnlspp1hj1 < 0.0 ) { jsxxec1j5w = 0.0 ;
_rtDW -> fo1r0adewj = 1 ; } } bpjo4wesci_idx_0 = eo3w5kdmqj /
muDoubleScalarLog ( jsxxec1j5w ) ; _rtB -> g2rgyhlwkg = 1.0 /
muDoubleScalarExp ( bpjo4wesci_idx_1 / bpjo4wesci_idx_0 ) * lnlspp1hj1 ; _rtB
-> fr3pu03tlh = bpjo4wesci_idx_0 * _rtP -> P_102 ; lpaxahqxhq /= _rtP ->
P_106 ; htkct3cv1q -= _rtP -> P_109 ; _rtB -> m3fg5r2ngi = _rtP -> P_108 *
_rtP -> P_107 * lpaxahqxhq * htkct3cv1q + ( lpaxahqxhq - _rtP -> P_105 ) *
_rtP -> P_104 ; _rtB -> h3chnofq0n = ( 0.0 - _rtP -> P_103 * _rtB ->
m3fg5r2ngi ) - _rtP -> P_110 * htkct3cv1q ; _rtB -> ntaf01vsjc = _rtP ->
P_111 ; _rtB -> h1wbn4k2p2 = _rtP -> P_112 ; } ssCallAccelRunBlock ( S , 3 ,
104 , SS_CALL_MDL_OUTPUTS ) ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB ->
mrjjpfbkto = _rtB -> kpype5dk1g * _rtB -> p2yo5dlznn ; } if ( ssIsSampleHit (
S , 3 , 0 ) ) { pw1ypam5j4 = ( _rtB -> fg54u50ndu + hwjiit4ixv ) * _rtP ->
P_113 ; } _rtB -> mjoea4ltbl = ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) ->
lsixylg4gz ; { real_T * * uBuffer = ( real_T * * ) & _rtDW -> dkh3g02dnk .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
dkh3g02dnk . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; real_T
tMinusDelay = simTime - _rtP -> P_115 ; fg0atln1ju =
Copy_of_PV_Single_Phase_inverter_afd_DC_AC_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * tBuffer , * uBuffer , _rtDW -> op30r11knm .
CircularBufSize , & _rtDW -> op30r11knm . Last , _rtDW -> op30r11knm . Tail ,
_rtDW -> op30r11knm . Head , _rtP -> P_116 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } _rtB -> f5ki4ipbq3 = _rtB -> mjoea4ltbl - fg0atln1ju ; if (
ssIsMajorTimeStep ( S ) ) { _rtDW -> o3npefk13c = _rtB -> f5ki4ipbq3 >= 0.0 ?
1 : 0 ; } fg0atln1ju = _rtDW -> o3npefk13c > 0 ? _rtB -> f5ki4ipbq3 : - _rtB
-> f5ki4ipbq3 ; if ( ssIsMajorTimeStep ( S ) ) { if ( _rtDW -> emyis2l45g !=
0 ) { ssSetSolverNeedsReset ( S ) ; _rtDW -> emyis2l45g = 0 ; } _rtB ->
kcunjjcjza = muDoubleScalarSqrt ( fg0atln1ju ) ; } else { if ( fg0atln1ju <
0.0 ) { _rtB -> kcunjjcjza = - muDoubleScalarSqrt ( muDoubleScalarAbs (
fg0atln1ju ) ) ; } else { _rtB -> kcunjjcjza = muDoubleScalarSqrt (
fg0atln1ju ) ; } _rtDW -> emyis2l45g = 1 ; } ssCallAccelRunBlock ( S , 3 ,
114 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> gzqt1cqjpl = _rtB -> mrjjpfbkto / _rtB
-> kcunjjcjza ; if ( ssIsSampleHit ( S , 3 , 0 ) ) { ssCallAccelRunBlock ( S
, 3 , 116 , SS_CALL_MDL_OUTPUTS ) ; jsxxec1j5w = _rtP -> P_117 * _rtB ->
fg54u50ndu ; bpjo4wesci_idx_0 = muDoubleScalarCos ( aoer0ike4l ) ; _rtB ->
hxpqflevf0 = jsxxec1j5w * bpjo4wesci_idx_0 ; if ( _rtDW -> p0mjtkssxb != 0 )
{ _rtB -> kne0amghdf = _rtDW -> fv1uvl41xo ; } else { _rtB -> kne0amghdf =
_rtP -> P_118 * _rtB -> hxpqflevf0 + _rtDW -> fv1uvl41xo ; } if ( hvhzmisfej
> _rtP -> P_120 ) { lnlspp1hj1 = _rtP -> P_120 ; } else if ( hvhzmisfej <
_rtP -> P_121 ) { lnlspp1hj1 = _rtP -> P_121 ; } else { lnlspp1hj1 =
hvhzmisfej ; } lnlspp1hj1 = 1.0 / lnlspp1hj1 / 0.0001 ; bpjo4wesci_idx_1 =
muDoubleScalarCeil ( lnlspp1hj1 ) ; _rtB -> laxihhja4k = _rtP -> P_122 *
bpjo4wesci_idx_1 ; ssCallAccelRunBlock ( S , 3 , 126 , SS_CALL_MDL_OUTPUTS )
; bpjo4wesci_idx_1 = lnlspp1hj1 - bpjo4wesci_idx_1 ; if ( ssGetTaskTime ( S ,
3 ) < _rtP -> P_125 ) { bpjo4wesci_idx_3 = _rtP -> P_126 ; } else {
bpjo4wesci_idx_3 = _rtP -> P_127 ; } if ( bpjo4wesci_idx_3 >= _rtP -> P_129 )
{ bpjo4wesci_idx_1 = ( ( _rtB -> hxpqflevf0 - _rtDW -> mo5i5rvr5i ) *
bpjo4wesci_idx_1 * _rtP -> P_124 + _rtB -> hxpqflevf0 ) * ( bpjo4wesci_idx_1
/ lnlspp1hj1 ) + ( _rtB -> kne0amghdf - _rtB -> km1nkilru1 ) * hvhzmisfej ; }
else { bpjo4wesci_idx_1 = _rtP -> P_128 ; } _rtB -> czndxx30wk = _rtP ->
P_130 ; if ( ssIsMajorTimeStep ( S ) ) { if ( _rtB -> czndxx30wk > 0.0 ) { if
( ! _rtDW -> m1v05clsvv ) { if ( ssGetTaskTime ( S , 3 ) != ssGetTStart ( S )
) { ssSetSolverNeedsReset ( S ) ; } _rtDW -> i5rjtcs5sd = 1U ; _rtDW ->
j1qqayjy1b = 1U ; _rtDW -> m1v05clsvv = true ; } } else { if ( _rtDW ->
m1v05clsvv ) { ssSetSolverNeedsReset ( S ) ; _rtDW -> m1v05clsvv = false ; }
} } if ( _rtDW -> m1v05clsvv ) { _rtB -> jkovacda1u [ 0 ] = _rtP -> P_1 *
muDoubleScalarSin ( aoer0ike4l ) * jsxxec1j5w ; _rtB -> jkovacda1u [ 1 ] =
_rtP -> P_1 * bpjo4wesci_idx_0 * jsxxec1j5w ; if ( _rtDW -> i5rjtcs5sd != 0 )
{ _rtB -> ptmdu1k3aj = _rtDW -> hbwmzyylnb ; } else { _rtB -> ptmdu1k3aj =
_rtP -> P_2 * _rtB -> jkovacda1u [ 0 ] + _rtDW -> hbwmzyylnb ; } if (
hvhzmisfej > _rtP -> P_4 ) { lnlspp1hj1 = _rtP -> P_4 ; } else if (
hvhzmisfej < _rtP -> P_5 ) { lnlspp1hj1 = _rtP -> P_5 ; } else { lnlspp1hj1 =
hvhzmisfej ; } jsxxec1j5w = 1.0 / lnlspp1hj1 / 0.0001 ; bpjo4wesci_idx_0 =
muDoubleScalarCeil ( jsxxec1j5w ) ; _rtB -> ojmqkmcuoj = _rtP -> P_6 *
bpjo4wesci_idx_0 ; ssCallAccelRunBlock ( S , 1 , 7 , SS_CALL_MDL_OUTPUTS ) ;
bpjo4wesci_idx_0 = jsxxec1j5w - bpjo4wesci_idx_0 ; if ( ssGetTaskTime ( S , 3
) < _rtP -> P_9 ) { bpjo4wesci_idx_3 = _rtP -> P_10 ; } else {
bpjo4wesci_idx_3 = _rtP -> P_11 ; } if ( bpjo4wesci_idx_3 >= _rtP -> P_13 ) {
bpjo4wesci_idx_0 = ( ( _rtB -> jkovacda1u [ 0 ] - _rtDW -> es20fy2lvr ) *
bpjo4wesci_idx_0 * _rtP -> P_8 + _rtB -> jkovacda1u [ 0 ] ) * (
bpjo4wesci_idx_0 / jsxxec1j5w ) + ( _rtB -> ptmdu1k3aj - _rtB -> keckzqo4yo )
* hvhzmisfej ; } else { bpjo4wesci_idx_0 = _rtP -> P_12 ; } if ( _rtDW ->
j1qqayjy1b != 0 ) { _rtB -> np3w5lg3ur = _rtDW -> ltialakutr ; } else { _rtB
-> np3w5lg3ur = _rtP -> P_14 * _rtB -> jkovacda1u [ 1 ] + _rtDW -> ltialakutr
; } if ( hvhzmisfej > _rtP -> P_16 ) { lnlspp1hj1 = _rtP -> P_16 ; } else if
( hvhzmisfej < _rtP -> P_17 ) { lnlspp1hj1 = _rtP -> P_17 ; } else {
lnlspp1hj1 = hvhzmisfej ; } jsxxec1j5w = 1.0 / lnlspp1hj1 / 0.0001 ;
eo3w5kdmqj = muDoubleScalarCeil ( jsxxec1j5w ) ; _rtB -> ogknms4cd3 = _rtP ->
P_18 * eo3w5kdmqj ; ssCallAccelRunBlock ( S , 1 , 27 , SS_CALL_MDL_OUTPUTS )
; eo3w5kdmqj = jsxxec1j5w - eo3w5kdmqj ; if ( ssGetTaskTime ( S , 3 ) < _rtP
-> P_21 ) { bpjo4wesci_idx_3 = _rtP -> P_22 ; } else { bpjo4wesci_idx_3 =
_rtP -> P_23 ; } if ( bpjo4wesci_idx_3 >= _rtP -> P_25 ) { eo3w5kdmqj = ( (
_rtB -> jkovacda1u [ 1 ] - _rtDW -> e3hcogjryj ) * eo3w5kdmqj * _rtP -> P_20
+ _rtB -> jkovacda1u [ 1 ] ) * ( eo3w5kdmqj / jsxxec1j5w ) + ( _rtB ->
np3w5lg3ur - _rtB -> fu1dru4srm ) * hvhzmisfej ; } else { eo3w5kdmqj = _rtP
-> P_24 ; } bpjo4wesci_idx_0 = muDoubleScalarHypot ( bpjo4wesci_idx_0 ,
eo3w5kdmqj ) ; if ( bpjo4wesci_idx_0 > _rtP -> P_26 ) { bpjo4wesci_idx_0 =
_rtP -> P_26 ; } else { if ( bpjo4wesci_idx_0 < _rtP -> P_27 ) {
bpjo4wesci_idx_0 = _rtP -> P_27 ; } } _rtB -> ku3yiw1fh4 = 1.0 /
bpjo4wesci_idx_0 ; if ( ssIsMajorTimeStep ( S ) ) { srUpdateBC ( _rtDW ->
bzrv5d0tmm ) ; } } bpjo4wesci_idx_1 *= _rtB -> ku3yiw1fh4 ; _rtB ->
g04pdgf3r4 = _rtP -> P_136 * bpjo4wesci_idx_1 * _rtP -> P_137 ;
bpjo4wesci_idx_3 = ( _rtP -> P_131 * bpjo4wesci_idx_1 + _rtDW -> n1uqml3zbu )
+ ( _rtB -> g04pdgf3r4 - _rtDW -> j1gf540g2s ) ; if ( bpjo4wesci_idx_3 > _rtP
-> P_139 ) { _rtB -> nieuypljq0 = _rtP -> P_139 ; } else if (
bpjo4wesci_idx_3 < _rtP -> P_140 ) { _rtB -> nieuypljq0 = _rtP -> P_140 ; }
else { _rtB -> nieuypljq0 = bpjo4wesci_idx_3 ; } bpjo4wesci_idx_3 = _rtP ->
P_141 * _rtB -> nieuypljq0 - _rtDW -> j0jutlrn4j ; if ( bpjo4wesci_idx_3 >
_rtP -> P_143 ) { bpjo4wesci_idx_3 = _rtP -> P_143 ; } else { if (
bpjo4wesci_idx_3 < _rtP -> P_144 ) { bpjo4wesci_idx_3 = _rtP -> P_144 ; } }
_rtB -> berndsqizm = bpjo4wesci_idx_3 + _rtDW -> j0jutlrn4j ; { _rtB ->
hyrobc4xws = ( _rtP -> P_147 [ 0 ] ) * _rtDW -> opbvzhddhh [ 0 ] + ( _rtP ->
P_147 [ 1 ] ) * _rtDW -> opbvzhddhh [ 1 ] ; _rtB -> hyrobc4xws += _rtP ->
P_148 * _rtB -> berndsqizm ; } _rtB -> lk3g0ck0ns = _rtP -> P_150 *
bpjo4wesci_idx_1 ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> ikgdahbaul
= _rtP -> P_151 ; } fg0atln1ju = ssGetT ( S ) ; if ( ssIsSampleHit ( S , 1 ,
0 ) ) { _rtB -> ojsh13lpzn = _rtP -> P_154 * _rtP -> P_153 ; _rtB ->
nccuag3lvh = _rtP -> P_155 ; _rtB -> kvaftg12b5 = _rtP -> P_156 ; } if (
ssIsSampleHit ( S , 3 , 0 ) ) { if ( pw1ypam5j4 > _rtP -> P_157 ) { _rtB ->
eaobwl1gaw = _rtP -> P_157 ; } else if ( pw1ypam5j4 < _rtP -> P_158 ) { _rtB
-> eaobwl1gaw = _rtP -> P_158 ; } else { _rtB -> eaobwl1gaw = pw1ypam5j4 ; }
} if ( _rtB -> kvaftg12b5 >= _rtP -> P_159 ) { _rtB -> o3hzmfm2kj =
muDoubleScalarSin ( ( _rtP -> P_152 * fg0atln1ju + _rtB -> ojsh13lpzn ) +
_rtB -> nccuag3lvh ) * _rtB -> ikgdahbaul ; } else { _rtB -> o3hzmfm2kj =
_rtB -> eaobwl1gaw ; } fg0atln1ju = ssGetT ( S ) ; _rtB -> jhla3p2j4z =
ssGetTStart ( S ) ; fg0atln1ju -= _rtB -> jhla3p2j4z ; if ( ssIsSampleHit ( S
, 1 , 0 ) ) { _rtB -> a54o4cvnvs = _rtP -> P_160 ; } fg0atln1ju =
muDoubleScalarRem ( fg0atln1ju , _rtB -> a54o4cvnvs ) ; _rtB -> e1ra0yak3k =
look1_binlxpw ( fg0atln1ju , _rtP -> P_162 , _rtP -> P_161 , 3U ) ; if (
ssIsSampleHit ( S , 1 , 0 ) ) { if ( ssIsMajorTimeStep ( S ) ) { _rtDW ->
m4eictromn = ( _rtB -> o3hzmfm2kj >= _rtB -> e1ra0yak3k ) ; } ckeb1s2dlz =
_rtDW -> m4eictromn ; hitkasikyj = ! _rtDW -> m4eictromn ; } _rtB ->
hfwmzlziga = _rtP -> P_163 * _rtB -> o3hzmfm2kj ; if ( ssIsSampleHit ( S , 1
, 0 ) ) { if ( ssIsMajorTimeStep ( S ) ) { _rtDW -> krdkwp0a5c = ( _rtB ->
hfwmzlziga >= _rtB -> e1ra0yak3k ) ; } _rtB -> ewhr0u4xmx [ 0 ] = ckeb1s2dlz
; _rtB -> ewhr0u4xmx [ 1 ] = hitkasikyj ; _rtB -> ewhr0u4xmx [ 2 ] = _rtDW ->
krdkwp0a5c ; _rtB -> ewhr0u4xmx [ 3 ] = ! _rtDW -> krdkwp0a5c ; } if (
ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> gg1mrk2tlz = _rtB -> fg54u50ndu *
_rtB -> fg54u50ndu * _rtP -> P_164 ; _rtB -> es5uk3zgm2 = _rtP -> P_165 *
_rtB -> ks2dmtukc2 [ 5 ] ; _rtB -> mr53unt245 = _rtP -> P_166 * _rtB ->
ks2dmtukc2 [ 7 ] ; ssCallAccelRunBlock ( S , 3 , 191 , SS_CALL_MDL_OUTPUTS )
; } UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { real_T tmp ; n3qi1whofz
* _rtB ; loikxjbxjg * _rtP ; ew10rzwqr2 * _rtDW ; _rtDW = ( ( ew10rzwqr2 * )
ssGetRootDWork ( S ) ) ; _rtP = ( ( loikxjbxjg * ) ssGetDefaultParam ( S ) )
; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ; if ( ssIsSampleHit ( S ,
2 , 0 ) ) { _rtDW -> pd3luz4avz [ 0 ] = _rtB -> pwethyghkk [ 0 ] ; _rtDW ->
pd3luz4avz [ 1 ] = _rtB -> pwethyghkk [ 1 ] ; _rtDW -> pd3luz4avz [ 2 ] =
_rtB -> pwethyghkk [ 2 ] ; _rtDW -> pd3luz4avz [ 3 ] = _rtB -> pwethyghkk [ 3
] ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtDW -> b2gcy3vb2a [ 0 ] = _rtB
-> ewhr0u4xmx [ 0 ] ; _rtDW -> b2gcy3vb2a [ 1 ] = _rtB -> ewhr0u4xmx [ 1 ] ;
_rtDW -> b2gcy3vb2a [ 2 ] = _rtB -> ewhr0u4xmx [ 2 ] ; _rtDW -> b2gcy3vb2a [
3 ] = _rtB -> ewhr0u4xmx [ 3 ] ; } if ( _rtDW -> byh0btktux && ssIsSampleHit
( S , 2 , 0 ) ) { tmp = _rtP -> P_31 * _rtB -> ecey0q2vme ; _rtDW ->
l4dlj3krq0 [ 0 ] += tmp ; _rtDW -> l4dlj3krq0 [ 1 ] += tmp ; _rtDW ->
l4dlj3krq0 [ 2 ] += tmp ; _rtDW -> l4dlj3krq0 [ 3 ] += tmp ; if ( _rtB ->
e3hpbcnjjc [ 0 ] > 0.0 ) { _rtDW -> b13awnigwz [ 0 ] = 1 ; } else if ( _rtB
-> e3hpbcnjjc [ 0 ] < 0.0 ) { _rtDW -> b13awnigwz [ 0 ] = - 1 ; } else if (
_rtB -> e3hpbcnjjc [ 0 ] == 0.0 ) { _rtDW -> b13awnigwz [ 0 ] = 0 ; } else {
_rtDW -> b13awnigwz [ 0 ] = 2 ; } if ( _rtB -> e3hpbcnjjc [ 1 ] > 0.0 ) {
_rtDW -> b13awnigwz [ 1 ] = 1 ; } else if ( _rtB -> e3hpbcnjjc [ 1 ] < 0.0 )
{ _rtDW -> b13awnigwz [ 1 ] = - 1 ; } else if ( _rtB -> e3hpbcnjjc [ 1 ] ==
0.0 ) { _rtDW -> b13awnigwz [ 1 ] = 0 ; } else { _rtDW -> b13awnigwz [ 1 ] =
2 ; } if ( _rtB -> e3hpbcnjjc [ 2 ] > 0.0 ) { _rtDW -> b13awnigwz [ 2 ] = 1 ;
} else if ( _rtB -> e3hpbcnjjc [ 2 ] < 0.0 ) { _rtDW -> b13awnigwz [ 2 ] = -
1 ; } else if ( _rtB -> e3hpbcnjjc [ 2 ] == 0.0 ) { _rtDW -> b13awnigwz [ 2 ]
= 0 ; } else { _rtDW -> b13awnigwz [ 2 ] = 2 ; } if ( _rtB -> e3hpbcnjjc [ 3
] > 0.0 ) { _rtDW -> b13awnigwz [ 3 ] = 1 ; } else if ( _rtB -> e3hpbcnjjc [
3 ] < 0.0 ) { _rtDW -> b13awnigwz [ 3 ] = - 1 ; } else if ( _rtB ->
e3hpbcnjjc [ 3 ] == 0.0 ) { _rtDW -> b13awnigwz [ 3 ] = 0 ; } else { _rtDW ->
b13awnigwz [ 3 ] = 2 ; } _rtDW -> bxpfaqqn5g [ 0 ] = _rtB -> peonb2s13j [ 0 ]
; _rtDW -> bxpfaqqn5g [ 1 ] = _rtB -> peonb2s13j [ 1 ] ; _rtDW -> bxpfaqqn5g
[ 2 ] = _rtB -> peonb2s13j [ 2 ] ; _rtDW -> bxpfaqqn5g [ 3 ] = _rtB ->
peonb2s13j [ 3 ] ; } if ( ssIsSampleHit ( S , 2 , 0 ) ) { ssCallAccelRunBlock
( S , 3 , 8 , SS_CALL_MDL_UPDATE ) ; _rtDW -> k5u0blc5by = _rtB -> ks2dmtukc2
[ 0 ] ; _rtDW -> kn5xrzh2lm = _rtB -> pp4nplutdu ; } { real_T * * uBuffer = (
real_T * * ) & _rtDW -> bxxpgfrdrd . TUbufferPtrs [ 0 ] ; real_T * * tBuffer
= ( real_T * * ) & _rtDW -> bxxpgfrdrd . TUbufferPtrs [ 1 ] ; real_T simTime
= ssGetT ( S ) ; _rtDW -> cmbq4gun1g . Head = ( ( _rtDW -> cmbq4gun1g . Head
< ( _rtDW -> cmbq4gun1g . CircularBufSize - 1 ) ) ? ( _rtDW -> cmbq4gun1g .
Head + 1 ) : 0 ) ; if ( _rtDW -> cmbq4gun1g . Head == _rtDW -> cmbq4gun1g .
Tail ) { if ( !
Copy_of_PV_Single_Phase_inverter_afd_DC_AC_acc_rt_TDelayUpdateTailOrGrowBuf (
& _rtDW -> cmbq4gun1g . CircularBufSize , & _rtDW -> cmbq4gun1g . Tail , &
_rtDW -> cmbq4gun1g . Head , & _rtDW -> cmbq4gun1g . Last , simTime - _rtP ->
P_71 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW ->
cmbq4gun1g . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
cmbq4gun1g . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> cmbq4gun1g . Head ]
= _rtB -> gzqt1cqjpl ; } if ( ssIsSampleHit ( S , 3 , 0 ) ) { _rtDW ->
bgv2ulrz1x = _rtB -> hyrobc4xws ; _rtDW -> lkjsqizslz += _rtP -> P_82 * _rtB
-> nieuypljq0 ; ssCallAccelRunBlock ( S , 3 , 60 , SS_CALL_MDL_UPDATE ) ;
_rtDW -> dxwkuh3yet = _rtDW -> jdlga5gylp ; _rtDW -> b3cy3g4cyx += _rtP ->
P_89 * _rtB -> lrtpaprp4h ; if ( _rtDW -> b3cy3g4cyx >= _rtP -> P_91 ) {
_rtDW -> b3cy3g4cyx = _rtP -> P_91 ; } else { if ( _rtDW -> b3cy3g4cyx <=
_rtP -> P_92 ) { _rtDW -> b3cy3g4cyx = _rtP -> P_92 ; } } } if (
ssIsSampleHit ( S , 2 , 0 ) ) { ssCallAccelRunBlock ( S , 0 , 0 ,
SS_CALL_MDL_UPDATE ) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
dkh3g02dnk . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW
-> dkh3g02dnk . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
op30r11knm . Head = ( ( _rtDW -> op30r11knm . Head < ( _rtDW -> op30r11knm .
CircularBufSize - 1 ) ) ? ( _rtDW -> op30r11knm . Head + 1 ) : 0 ) ; if (
_rtDW -> op30r11knm . Head == _rtDW -> op30r11knm . Tail ) { if ( !
Copy_of_PV_Single_Phase_inverter_afd_DC_AC_acc_rt_TDelayUpdateTailOrGrowBuf (
& _rtDW -> op30r11knm . CircularBufSize , & _rtDW -> op30r11knm . Tail , &
_rtDW -> op30r11knm . Head , & _rtDW -> op30r11knm . Last , simTime - _rtP ->
P_115 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW ->
op30r11knm . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
op30r11knm . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> op30r11knm . Head ]
= _rtB -> mjoea4ltbl ; } if ( ssIsSampleHit ( S , 3 , 0 ) ) { _rtDW ->
p0mjtkssxb = 0U ; _rtDW -> fv1uvl41xo = _rtP -> P_118 * _rtB -> hxpqflevf0 +
_rtB -> kne0amghdf ; ssCallAccelRunBlock ( S , 3 , 126 , SS_CALL_MDL_UPDATE )
; _rtDW -> mo5i5rvr5i = _rtB -> hxpqflevf0 ; if ( _rtDW -> m1v05clsvv ) {
_rtDW -> i5rjtcs5sd = 0U ; _rtDW -> hbwmzyylnb = _rtP -> P_2 * _rtB ->
jkovacda1u [ 0 ] + _rtB -> ptmdu1k3aj ; ssCallAccelRunBlock ( S , 1 , 7 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> es20fy2lvr = _rtB -> jkovacda1u [ 0 ] ; _rtDW
-> j1qqayjy1b = 0U ; _rtDW -> ltialakutr = _rtP -> P_14 * _rtB -> jkovacda1u
[ 1 ] + _rtB -> np3w5lg3ur ; ssCallAccelRunBlock ( S , 1 , 27 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> e3hcogjryj = _rtB -> jkovacda1u [ 1 ] ; }
_rtDW -> n1uqml3zbu += _rtP -> P_132 * _rtB -> lk3g0ck0ns ; if ( _rtDW ->
n1uqml3zbu >= _rtP -> P_134 ) { _rtDW -> n1uqml3zbu = _rtP -> P_134 ; } else
{ if ( _rtDW -> n1uqml3zbu <= _rtP -> P_135 ) { _rtDW -> n1uqml3zbu = _rtP ->
P_135 ; } } _rtDW -> j1gf540g2s = _rtB -> g04pdgf3r4 ; _rtDW -> j0jutlrn4j =
_rtB -> berndsqizm ; { real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_145 [ 0 ]
) * _rtDW -> opbvzhddhh [ 0 ] + ( _rtP -> P_145 [ 1 ] ) * _rtDW -> opbvzhddhh
[ 1 ] ; xnew [ 0 ] += ( _rtP -> P_146 [ 0 ] ) * _rtB -> berndsqizm ; xnew [ 1
] = ( _rtP -> P_145 [ 2 ] ) * _rtDW -> opbvzhddhh [ 0 ] + ( _rtP -> P_145 [ 3
] ) * _rtDW -> opbvzhddhh [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_146 [ 1 ] ) *
_rtB -> berndsqizm ; ( void ) memcpy ( & _rtDW -> opbvzhddhh [ 0 ] , xnew ,
sizeof ( real_T ) * 2 ) ; } } UNUSED_PARAMETER ( tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { n3qi1whofz * _rtB ; loikxjbxjg
* _rtP ; _rtP = ( ( loikxjbxjg * ) ssGetDefaultParam ( S ) ) ; _rtB = ( (
n3qi1whofz * ) _ssGetBlockIO ( S ) ) ; { ( ( pqmvzr1kvu * ) ssGetdX ( S ) )
-> lsixylg4gz = _rtB -> gg1mrk2tlz ; } }
#define MDL_ZERO_CROSSINGS
static void mdlZeroCrossings ( SimStruct * S ) { n3qi1whofz * _rtB ;
loikxjbxjg * _rtP ; d15n1ti4kq * _rtZCSV ; _rtZCSV = ( ( d15n1ti4kq * )
ssGetSolverZcSignalVector ( S ) ) ; _rtP = ( ( loikxjbxjg * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ;
_rtZCSV -> bznktecnfh = ssGetT ( S ) - _rtP -> P_55 ; _rtZCSV -> c0mbjypztm =
_rtB -> f5ki4ipbq3 ; _rtZCSV -> j2wg3uhh42 = _rtB -> o3hzmfm2kj - _rtB ->
e1ra0yak3k ; _rtZCSV -> osxehvr00q = _rtB -> hfwmzlziga - _rtB -> e1ra0yak3k
; } static void mdlInitializeSizes ( SimStruct * S ) { ssSetChecksumVal ( S ,
0 , 1177223669U ) ; ssSetChecksumVal ( S , 1 , 919433994U ) ;
ssSetChecksumVal ( S , 2 , 2098599657U ) ; ssSetChecksumVal ( S , 3 ,
415193601U ) ; { mxArray * slVerStructMat = NULL ; mxArray * slStrMat =
mxCreateString ( "simulink" ) ; char slVerChar [ 10 ] ; int status =
mexCallMATLAB ( 1 , & slVerStructMat , 1 , & slStrMat , "ver" ) ; if ( status
== 0 ) { mxArray * slVerMat = mxGetField ( slVerStructMat , 0 , "Version" ) ;
if ( slVerMat == NULL ) { status = 1 ; } else { status = mxGetString (
slVerMat , slVerChar , 10 ) ; } } mxDestroyArray ( slStrMat ) ;
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
loikxjbxjg * ) ssGetDefaultParam ( S ) ) -> P_76 = rtInf ; } static void
mdlInitializeSampleTimes ( SimStruct * S ) { } static void mdlTerminate (
SimStruct * S ) { }
#include "simulink.c"
