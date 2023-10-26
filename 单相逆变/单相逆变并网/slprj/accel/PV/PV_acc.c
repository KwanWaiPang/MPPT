#include "__cf_PV.h"
#include <math.h>
#include "PV_acc.h"
#include "PV_acc_private.h"
#include <stdio.h>
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
static void mdlOutputs ( SimStruct * S , int_T tid ) { real_T i2gqfnu31d ;
real_T p35eimwogi ; real_T jp5ua23i4a ; real_T tmp ; real_T mlwhlk4gtf ;
n3qi1whofz * _rtB ; loikxjbxjg * _rtP ; ew10rzwqr2 * _rtDW ; _rtDW = ( (
ew10rzwqr2 * ) ssGetRootDWork ( S ) ) ; _rtP = ( ( loikxjbxjg * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ;
if ( ssIsSampleHit ( S , 1 , 0 ) ) { if ( ssIsMajorTimeStep ( S ) ) { _rtDW
-> ndralnpcm4 = ( ssGetTaskTime ( S , 1 ) >= _rtP -> P_0 ) ; } if ( _rtDW ->
ndralnpcm4 == 1 ) { _rtB -> dblmosbci5 = _rtP -> P_2 ; } else { _rtB ->
dblmosbci5 = _rtP -> P_1 ; } } i2gqfnu31d = ssGetT ( S ) ; if ( ssIsSampleHit
( S , 1 , 0 ) ) { _rtB -> jsikxfnt4k = _rtP -> P_3 ; _rtB -> pf5gqb1qet =
_rtP -> P_4 ; } _rtB -> o4biktnln1 = ( i2gqfnu31d - _rtB -> jsikxfnt4k ) *
_rtB -> dblmosbci5 + _rtB -> pf5gqb1qet ; if ( ssIsSampleHit ( S , 1 , 0 ) )
{ p35eimwogi = _rtP -> P_5 / _rtP -> P_6 ; mlwhlk4gtf = p35eimwogi - _rtP ->
P_7 ; jp5ua23i4a = _rtP -> P_7 - _rtP -> P_8 / _rtP -> P_9 ; tmp = jp5ua23i4a
; if ( ssIsMajorTimeStep ( S ) ) { if ( _rtDW -> ccnsfwllyk != 0 ) {
ssSetSolverNeedsReset ( S ) ; _rtDW -> ccnsfwllyk = 0 ; } } else { if (
jp5ua23i4a < 0.0 ) { tmp = 0.0 ; _rtDW -> ccnsfwllyk = 1 ; } } mlwhlk4gtf /=
muDoubleScalarLog ( tmp ) ; _rtB -> prhmmk1qdh = 1.0 / muDoubleScalarExp (
p35eimwogi / mlwhlk4gtf ) * jp5ua23i4a ; _rtB -> fcmboa5g3l = mlwhlk4gtf *
_rtP -> P_10 ; if ( ssIsMajorTimeStep ( S ) ) { _rtDW -> gvbb0zob1v = (
ssGetTaskTime ( S , 1 ) >= _rtP -> P_14 ) ; } if ( _rtDW -> gvbb0zob1v == 1 )
{ tmp = _rtP -> P_16 ; } else { tmp = _rtP -> P_15 ; } p35eimwogi = tmp /
_rtP -> P_17 ; mlwhlk4gtf = _rtP -> P_20 - _rtP -> P_21 ; _rtB -> hjjkwhgwj2
= _rtP -> P_19 * _rtP -> P_18 * p35eimwogi * mlwhlk4gtf + ( p35eimwogi - _rtP
-> P_13 ) * _rtP -> P_12 ; _rtB -> b3dfwx3z0u = ( 0.0 - _rtP -> P_11 * _rtB
-> hjjkwhgwj2 ) - _rtP -> P_22 * mlwhlk4gtf ; _rtB -> pluqgletco = _rtP ->
P_23 ; _rtB -> eisnplaxw0 = _rtP -> P_24 ; } _rtB -> kqhuhqft5y = ( _rtB ->
pluqgletco - ( muDoubleScalarExp ( 1.0 / _rtB -> fcmboa5g3l * ( _rtB ->
o4biktnln1 - _rtB -> b3dfwx3z0u ) ) - 1.0 ) * _rtB -> prhmmk1qdh ) * _rtB ->
eisnplaxw0 + _rtB -> hjjkwhgwj2 ; _rtB -> m3sqr2yo3x = _rtB -> o4biktnln1 *
_rtB -> kqhuhqft5y ; ssCallAccelRunBlock ( S , 2 , 51 , SS_CALL_MDL_OUTPUTS )
; if ( ssIsSampleHit ( S , 1 , 0 ) ) { ssCallAccelRunBlock ( S , 0 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> lwzjjfcs3e [ 0 ] = _rtB -> o4biktnln1 ; _rtB
-> lwzjjfcs3e [ 1 ] = _rtB -> kqhuhqft5y ; } _rtB -> gyccfblz3n = _rtB ->
o4biktnln1 * _rtB -> kqhuhqft5y ; if ( ssIsSampleHit ( S , 1 , 0 ) ) {
ssCallAccelRunBlock ( S , 1 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> mwcdejhknu
[ 0 ] = _rtB -> o4biktnln1 ; _rtB -> mwcdejhknu [ 1 ] = _rtB -> gyccfblz3n ;
} UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { n3qi1whofz * _rtB ;
_rtB = ( ( n3qi1whofz * ) _ssGetBlockIO ( S ) ) ; if ( ssIsSampleHit ( S , 1
, 0 ) ) { ssCallAccelRunBlock ( S , 0 , 0 , SS_CALL_MDL_UPDATE ) ;
ssCallAccelRunBlock ( S , 1 , 0 , SS_CALL_MDL_UPDATE ) ; } UNUSED_PARAMETER (
tid ) ; }
#define MDL_ZERO_CROSSINGS
static void mdlZeroCrossings ( SimStruct * S ) { loikxjbxjg * _rtP ;
d15n1ti4kq * _rtZCSV ; _rtZCSV = ( ( d15n1ti4kq * ) ssGetSolverZcSignalVector
( S ) ) ; _rtP = ( ( loikxjbxjg * ) ssGetDefaultParam ( S ) ) ; _rtZCSV ->
ges0vmcus1 = ssGetT ( S ) - _rtP -> P_0 ; _rtZCSV -> cn1aifqltt = ssGetT ( S
) - _rtP -> P_14 ; } static void mdlInitializeSizes ( SimStruct * S ) {
ssSetChecksumVal ( S , 0 , 2220404244U ) ; ssSetChecksumVal ( S , 1 ,
3859224264U ) ; ssSetChecksumVal ( S , 2 , 1018851026U ) ; ssSetChecksumVal (
S , 3 , 1273112185U ) ; { mxArray * slVerStructMat = NULL ; mxArray *
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
real_T * ) & o2iu0a2jke ) ; rt_InitInfAndNaN ( sizeof ( real_T ) ) ; } static
void mdlInitializeSampleTimes ( SimStruct * S ) { } static void mdlTerminate
( SimStruct * S ) { }
#include "simulink.c"
