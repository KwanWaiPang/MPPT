#include "__cf_PV_MPPT_BOOST1.h"
#include <math.h>
#include "PV_MPPT_BOOST1_acc.h"
#include "PV_MPPT_BOOST1_acc_private.h"
#include <stdio.h>
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
real_T look1_binlxpw ( real_T u0 , const real_T bp0 [ ] , const real_T table
[ ] , uint32_T maxIndex ) { real_T frac ; uint32_T iRght ; uint32_T iLeft ;
uint32_T bpIdx ; if ( u0 <= bp0 [ 0U ] ) { iLeft = 0U ; frac = ( u0 - bp0 [
0U ] ) / ( bp0 [ 1U ] - bp0 [ 0U ] ) ; } else if ( u0 < bp0 [ maxIndex ] ) {
bpIdx = maxIndex >> 1U ; iLeft = 0U ; iRght = maxIndex ; while ( iRght -
iLeft > 1U ) { if ( u0 < bp0 [ bpIdx ] ) { iRght = bpIdx ; } else { iLeft =
bpIdx ; } bpIdx = ( iRght + iLeft ) >> 1U ; } frac = ( u0 - bp0 [ iLeft ] ) /
( bp0 [ iLeft + 1U ] - bp0 [ iLeft ] ) ; } else { iLeft = maxIndex - 1U ;
frac = ( u0 - bp0 [ maxIndex - 1U ] ) / ( bp0 [ maxIndex ] - bp0 [ maxIndex -
1U ] ) ; } return ( table [ iLeft + 1U ] - table [ iLeft ] ) * frac + table [
iLeft ] ; } static void mdlOutputs ( SimStruct * S , int_T tid ) { real_T
bc1scdvt2g ; real_T p35eimwogi ; real_T jp5ua23i4a ; real_T tmp ; real_T
mlwhlk4gtf ; real_T oossyunrst ; n3qi1whofz * _rtB ; loikxjbxjg * _rtP ;
f1xhd02yjc * _rtX ; ew10rzwqr2 * _rtDW ; _rtDW = ( ( ew10rzwqr2 * )
ssGetRootDWork ( S ) ) ; _rtX = ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) ;
_rtP = ( ( loikxjbxjg * ) ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz *
) _ssGetBlockIO ( S ) ) ; if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB ->
lshmzlu0no [ 0 ] = _rtP -> P_1 [ 0 ] ; _rtB -> lshmzlu0no [ 1 ] = _rtP -> P_1
[ 1 ] ; _rtB -> lshmzlu0no [ 2 ] = _rtP -> P_1 [ 2 ] ; } if ( ssIsSampleHit (
S , 1 , 0 ) ) { p35eimwogi = _rtP -> P_2 / _rtP -> P_3 ; mlwhlk4gtf =
p35eimwogi - _rtP -> P_4 ; jp5ua23i4a = _rtP -> P_4 - _rtP -> P_5 / _rtP ->
P_6 ; tmp = jp5ua23i4a ; if ( ssIsMajorTimeStep ( S ) ) { if ( _rtDW ->
ccnsfwllyk != 0 ) { ssSetSolverNeedsReset ( S ) ; _rtDW -> ccnsfwllyk = 0 ; }
} else { if ( jp5ua23i4a < 0.0 ) { tmp = 0.0 ; _rtDW -> ccnsfwllyk = 1 ; } }
mlwhlk4gtf /= muDoubleScalarLog ( tmp ) ; _rtB -> ipnv2na000 = 1.0 /
muDoubleScalarExp ( p35eimwogi / mlwhlk4gtf ) * jp5ua23i4a ; _rtB ->
d14ivuctoj = mlwhlk4gtf * _rtP -> P_7 ; if ( ssIsMajorTimeStep ( S ) ) {
_rtDW -> gvbb0zob1v = ( ssGetTaskTime ( S , 1 ) >= _rtP -> P_11 ) ; } if (
_rtDW -> gvbb0zob1v == 1 ) { tmp = _rtP -> P_13 ; } else { tmp = _rtP -> P_12
; } p35eimwogi = tmp / _rtP -> P_14 ; mlwhlk4gtf = _rtP -> P_17 - _rtP ->
P_18 ; _rtB -> bzlcadf5qp = _rtP -> P_16 * _rtP -> P_15 * p35eimwogi *
mlwhlk4gtf + ( p35eimwogi - _rtP -> P_10 ) * _rtP -> P_9 ; _rtB -> piktxrmr03
= ( 0.0 - _rtP -> P_8 * _rtB -> bzlcadf5qp ) - _rtP -> P_19 * mlwhlk4gtf ;
_rtB -> k32qyfpllv = _rtP -> P_20 ; _rtB -> kvhj52yzy4 = _rtP -> P_21 ; } if
( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> ob4fv1wza3 = _rtP -> P_22 ; }
ssCallAccelRunBlock ( S , 1 , 41 , SS_CALL_MDL_OUTPUTS ) ; if ( ssIsSampleHit
( S , 2 , 0 ) ) { _rtB -> h4dfm5wstm = _rtP -> P_23 * _rtB -> at4svfakyj [ 6
] ; _rtB -> cz2pn2ngus = _rtB -> gtvmdf51i0 * _rtB -> h4dfm5wstm ;
ssCallAccelRunBlock ( S , 1 , 44 , SS_CALL_MDL_OUTPUTS ) ; } bc1scdvt2g =
ssGetT ( S ) ; _rtB -> ibmkekxjle = ssGetTStart ( S ) ; bc1scdvt2g -= _rtB ->
ibmkekxjle ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> h5yyuz5ige = _rtP
-> P_24 ; } bc1scdvt2g = muDoubleScalarRem ( bc1scdvt2g , _rtB -> h5yyuz5ige
) ; _rtB -> bvpnmyfvpp = look1_binlxpw ( bc1scdvt2g , _rtP -> P_26 , _rtP ->
P_25 , 2U ) ; ssCallAccelRunBlock ( S , 1 , 63 , SS_CALL_MDL_OUTPUTS ) ; if (
ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> pbwjgc0ulp = _rtP -> P_27 ;
mlwhlk4gtf = _rtP -> P_28 ; } _rtB -> mu4kvtivgw = 0.0 ; _rtB -> mu4kvtivgw
+= _rtP -> P_30 * _rtX -> n52bxvtwu4 ; if ( ssIsSampleHit ( S , 2 , 0 ) ) {
_rtB -> mhtekpekgq = _rtB -> mu4kvtivgw ; } if ( ssIsSampleHit ( S , 1 , 0 )
) { oossyunrst = muDoubleScalarSign ( _rtB -> mhtekpekgq - _rtDW ->
b0kcb2fd0b ) ; } _rtB -> bsilm1fdxc = 0.0 ; _rtB -> bsilm1fdxc += _rtP ->
P_33 * _rtX -> fahgdobdau ; if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB ->
flnqt1ilvz = _rtB -> mhtekpekgq * _rtB -> bsilm1fdxc ; } if ( ssIsSampleHit (
S , 1 , 0 ) ) { _rtB -> mfsgc05quk = muDoubleScalarSign ( muDoubleScalarSign
( _rtB -> flnqt1ilvz - _rtDW -> osnm2k1bun ) * oossyunrst ) * _rtP -> P_35 +
_rtDW -> kaozvtc11k ; _rtB -> czdsl2g3hy = mlwhlk4gtf + _rtB -> mfsgc05quk ;
} if ( ssIsSampleHit ( S , 2 , 0 ) ) { if ( _rtB -> czdsl2g3hy > _rtP -> P_37
) { _rtB -> afshkegvfp = _rtP -> P_37 ; } else if ( _rtB -> czdsl2g3hy < _rtP
-> P_38 ) { _rtB -> afshkegvfp = _rtP -> P_38 ; } else { _rtB -> afshkegvfp =
_rtB -> czdsl2g3hy ; } } _rtB -> auwfqnt3aq = _rtB -> afshkegvfp - _rtB ->
bvpnmyfvpp ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> mcoyusqmbx = _rtP
-> P_39 ; } if ( ssIsMajorTimeStep ( S ) ) { _rtDW -> oqbijwo5xn = ( _rtB ->
auwfqnt3aq >= _rtP -> P_40 ) ; } if ( _rtDW -> oqbijwo5xn ) { _rtB ->
akdpawdy4t = _rtB -> pbwjgc0ulp ; } else { _rtB -> akdpawdy4t = _rtB ->
mcoyusqmbx ; } ssCallAccelRunBlock ( S , 1 , 89 , SS_CALL_MDL_OUTPUTS ) ; if
( ssIsSampleHit ( S , 1 , 0 ) ) { ssCallAccelRunBlock ( S , 1 , 90 ,
SS_CALL_MDL_OUTPUTS ) ; } if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB ->
cz2pn2ngus = _rtP -> P_41 * _rtB -> at4svfakyj [ 3 ] ; _rtB -> p4yu0kkyux =
_rtP -> P_42 * _rtB -> at4svfakyj [ 5 ] ; ssCallAccelRunBlock ( S , 1 , 96 ,
SS_CALL_MDL_OUTPUTS ) ; } UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { n3qi1whofz * _rtB ;
ew10rzwqr2 * _rtDW ; _rtDW = ( ( ew10rzwqr2 * ) ssGetRootDWork ( S ) ) ; _rtB
= ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ; if ( ssIsSampleHit ( S , 2 , 0 )
) { ssCallAccelRunBlock ( S , 0 , 6 , SS_CALL_MDL_UPDATE ) ; } if (
ssIsSampleHit ( S , 1 , 0 ) ) { _rtDW -> b0kcb2fd0b = _rtB -> mhtekpekgq ;
_rtDW -> osnm2k1bun = _rtB -> flnqt1ilvz ; _rtDW -> kaozvtc11k = _rtB ->
mfsgc05quk ; } UNUSED_PARAMETER ( tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { n3qi1whofz * _rtB ; loikxjbxjg
* _rtP ; f1xhd02yjc * _rtX ; pqmvzr1kvu * _rtXdot ; _rtXdot = ( ( pqmvzr1kvu
* ) ssGetdX ( S ) ) ; _rtX = ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) ;
_rtP = ( ( loikxjbxjg * ) ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz *
) _ssGetBlockIO ( S ) ) ; _rtXdot -> n52bxvtwu4 = 0.0 ; _rtXdot -> n52bxvtwu4
+= _rtP -> P_29 * _rtX -> n52bxvtwu4 ; _rtXdot -> n52bxvtwu4 += _rtB ->
gtvmdf51i0 ; _rtXdot -> fahgdobdau = 0.0 ; _rtXdot -> fahgdobdau += _rtP ->
P_32 * _rtX -> fahgdobdau ; _rtXdot -> fahgdobdau += _rtB -> h4dfm5wstm ; }
#define MDL_ZERO_CROSSINGS
static void mdlZeroCrossings ( SimStruct * S ) { n3qi1whofz * _rtB ;
loikxjbxjg * _rtP ; d15n1ti4kq * _rtZCSV ; _rtZCSV = ( ( d15n1ti4kq * )
ssGetSolverZcSignalVector ( S ) ) ; _rtP = ( ( loikxjbxjg * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ;
_rtZCSV -> cn1aifqltt = ssGetT ( S ) - _rtP -> P_11 ; _rtZCSV -> gxb5rs1fuh =
_rtB -> auwfqnt3aq - _rtP -> P_40 ; } static void mdlInitializeSizes (
SimStruct * S ) { ssSetChecksumVal ( S , 0 , 2958731831U ) ; ssSetChecksumVal
( S , 1 , 592297482U ) ; ssSetChecksumVal ( S , 2 , 1576211060U ) ;
ssSetChecksumVal ( S , 3 , 2217609345U ) ; { mxArray * slVerStructMat = NULL
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
real_T * ) & o2iu0a2jke ) ; rt_InitInfAndNaN ( sizeof ( real_T ) ) ; } static
void mdlInitializeSampleTimes ( SimStruct * S ) { } static void mdlTerminate
( SimStruct * S ) { }
#include "simulink.c"
