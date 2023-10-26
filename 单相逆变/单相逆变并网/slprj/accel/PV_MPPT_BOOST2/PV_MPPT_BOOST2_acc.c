#include "__cf_PV_MPPT_BOOST2.h"
#include <math.h>
#include "PV_MPPT_BOOST2_acc.h"
#include "PV_MPPT_BOOST2_acc_private.h"
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
nigs5gezeu ; real_T p35eimwogi ; real_T jp5ua23i4a ; real_T tmp ; real_T
mlwhlk4gtf ; real_T j31e2f5pzo ; n3qi1whofz * _rtB ; loikxjbxjg * _rtP ;
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
] ; _rtB -> m3k5xyatin = _rtB -> gtvmdf51i0 * _rtB -> h4dfm5wstm ;
ssCallAccelRunBlock ( S , 1 , 44 , SS_CALL_MDL_OUTPUTS ) ; } _rtB ->
g1xtvdamdi = 0.0 ; _rtB -> g1xtvdamdi += _rtP -> P_25 * _rtX -> pjbcugwxf0 ;
if ( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> hwdjgkeswh = _rtB -> g1xtvdamdi
; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { j31e2f5pzo = muDoubleScalarSign (
_rtB -> hwdjgkeswh - _rtDW -> iylv2tf2cf ) ; } _rtB -> gbd3aq54nq = 0.0 ;
_rtB -> gbd3aq54nq += _rtP -> P_28 * _rtX -> nsp3ji42uc ; if ( ssIsSampleHit
( S , 2 , 0 ) ) { _rtB -> iluabgtpdk = _rtB -> hwdjgkeswh * _rtB ->
gbd3aq54nq ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> esvcnc5tbk =
muDoubleScalarSign ( muDoubleScalarSign ( _rtB -> iluabgtpdk - _rtDW ->
a5ky0k5op2 ) * j31e2f5pzo ) * _rtP -> P_30 + _rtDW -> pvuuzqlx3o ; } if (
ssIsSampleHit ( S , 2 , 0 ) ) { if ( _rtB -> esvcnc5tbk > _rtP -> P_32 ) {
_rtB -> aaq5l2f5mv = _rtP -> P_32 ; } else if ( _rtB -> esvcnc5tbk < _rtP ->
P_33 ) { _rtB -> aaq5l2f5mv = _rtP -> P_33 ; } else { _rtB -> aaq5l2f5mv =
_rtB -> esvcnc5tbk ; } ssCallAccelRunBlock ( S , 1 , 76 , SS_CALL_MDL_OUTPUTS
) ; } nigs5gezeu = ssGetT ( S ) ; _rtB -> cmgi1f4ini = ssGetTStart ( S ) ;
nigs5gezeu -= _rtB -> cmgi1f4ini ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB
-> jhgcpoyrp2 = _rtP -> P_34 ; } nigs5gezeu = muDoubleScalarRem ( nigs5gezeu
, _rtB -> jhgcpoyrp2 ) ; _rtB -> flo4usste2 = look1_binlxpw ( nigs5gezeu ,
_rtP -> P_36 , _rtP -> P_35 , 2U ) ; ssCallAccelRunBlock ( S , 1 , 83 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB ->
fjlkweyy0d = _rtP -> P_37 ; } _rtB -> m3npnac2ps = _rtB -> aaq5l2f5mv - _rtB
-> flo4usste2 ; if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> mevynvvqt1 =
_rtP -> P_38 ; } if ( ssIsMajorTimeStep ( S ) ) { _rtDW -> oqbijwo5xn = (
_rtB -> m3npnac2ps >= _rtP -> P_39 ) ; } if ( _rtDW -> oqbijwo5xn ) { _rtB ->
j5niy43j10 = _rtB -> fjlkweyy0d ; } else { _rtB -> j5niy43j10 = _rtB ->
mevynvvqt1 ; } ssCallAccelRunBlock ( S , 1 , 88 , SS_CALL_MDL_OUTPUTS ) ; if
( ssIsSampleHit ( S , 2 , 0 ) ) { _rtB -> m3k5xyatin = _rtP -> P_40 * _rtB ->
at4svfakyj [ 3 ] ; _rtB -> mlfkisfqcy = _rtP -> P_41 * _rtB -> at4svfakyj [ 5
] ; _rtB -> hp4s4stu0l = _rtB -> m3k5xyatin * _rtB -> mlfkisfqcy ;
ssCallAccelRunBlock ( S , 1 , 95 , SS_CALL_MDL_OUTPUTS ) ; } UNUSED_PARAMETER
( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { n3qi1whofz * _rtB ;
ew10rzwqr2 * _rtDW ; _rtDW = ( ( ew10rzwqr2 * ) ssGetRootDWork ( S ) ) ; _rtB
= ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ; if ( ssIsSampleHit ( S , 2 , 0 )
) { ssCallAccelRunBlock ( S , 0 , 6 , SS_CALL_MDL_UPDATE ) ; } if (
ssIsSampleHit ( S , 1 , 0 ) ) { _rtDW -> iylv2tf2cf = _rtB -> hwdjgkeswh ;
_rtDW -> a5ky0k5op2 = _rtB -> iluabgtpdk ; _rtDW -> pvuuzqlx3o = _rtB ->
esvcnc5tbk ; } UNUSED_PARAMETER ( tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { n3qi1whofz * _rtB ; loikxjbxjg
* _rtP ; f1xhd02yjc * _rtX ; pqmvzr1kvu * _rtXdot ; _rtXdot = ( ( pqmvzr1kvu
* ) ssGetdX ( S ) ) ; _rtX = ( ( f1xhd02yjc * ) ssGetContStates ( S ) ) ;
_rtP = ( ( loikxjbxjg * ) ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz *
) _ssGetBlockIO ( S ) ) ; _rtXdot -> pjbcugwxf0 = 0.0 ; _rtXdot -> pjbcugwxf0
+= _rtP -> P_24 * _rtX -> pjbcugwxf0 ; _rtXdot -> pjbcugwxf0 += _rtB ->
gtvmdf51i0 ; _rtXdot -> nsp3ji42uc = 0.0 ; _rtXdot -> nsp3ji42uc += _rtP ->
P_27 * _rtX -> nsp3ji42uc ; _rtXdot -> nsp3ji42uc += _rtB -> h4dfm5wstm ; }
#define MDL_ZERO_CROSSINGS
static void mdlZeroCrossings ( SimStruct * S ) { n3qi1whofz * _rtB ;
loikxjbxjg * _rtP ; d15n1ti4kq * _rtZCSV ; _rtZCSV = ( ( d15n1ti4kq * )
ssGetSolverZcSignalVector ( S ) ) ; _rtP = ( ( loikxjbxjg * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ;
_rtZCSV -> cn1aifqltt = ssGetT ( S ) - _rtP -> P_11 ; _rtZCSV -> gxb5rs1fuh =
_rtB -> m3npnac2ps - _rtP -> P_39 ; } static void mdlInitializeSizes (
SimStruct * S ) { ssSetChecksumVal ( S , 0 , 2134959864U ) ; ssSetChecksumVal
( S , 1 , 3446592568U ) ; ssSetChecksumVal ( S , 2 , 2870353630U ) ;
ssSetChecksumVal ( S , 3 , 703703787U ) ; { mxArray * slVerStructMat = NULL ;
mxArray * slStrMat = mxCreateString ( "simulink" ) ; char slVerChar [ 10 ] ;
int status = mexCallMATLAB ( 1 , & slVerStructMat , 1 , & slStrMat , "ver" )
; if ( status == 0 ) { mxArray * slVerMat = mxGetField ( slVerStructMat , 0 ,
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
real_T * ) & o2iu0a2jke ) ; rt_InitInfAndNaN ( sizeof ( real_T ) ) ; } static
void mdlInitializeSampleTimes ( SimStruct * S ) { } static void mdlTerminate
( SimStruct * S ) { }
#include "simulink.c"
