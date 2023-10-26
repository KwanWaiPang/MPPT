#include "__cf_PV_Single_Phase_inverter.h"
#ifndef RTW_HEADER_PV_Single_Phase_inverter_acc_h_
#define RTW_HEADER_PV_Single_Phase_inverter_acc_h_
#include <stddef.h>
#include <string.h>
#include <float.h>
#ifndef PV_Single_Phase_inverter_acc_COMMON_INCLUDES_
#define PV_Single_Phase_inverter_acc_COMMON_INCLUDES_
#include <stdlib.h>
#define S_FUNCTION_NAME simulink_only_sfcn 
#define S_FUNCTION_LEVEL 2
#define RTW_GENERATED_S_FUNCTION
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#endif
#include "PV_Single_Phase_inverter_acc_types.h"
#include "multiword_types.h"
#include "mwmathutil.h"
#include "rtGetInf.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
typedef struct { real_T mtebedot43 [ 3 ] ; real_T k0eoach3d0 [ 4 ] ; real_T
hpvnminlgh [ 4 ] ; real_T fh3kyg4a5x ; real_T akemzmcvei ; real_T beonv2fsee
; real_T mqoejox33w ; real_T ipu5qysy5u ; real_T esubiw2vje ; real_T
kc0aufcser ; real_T ptix3hif0l ; real_T p11ibpnyrl ; real_T cf1glnoy5m ;
real_T ivri3xnnra ; real_T j0xmonptfz ; real_T itxnx2wsi5 [ 4 ] ; real_T
bgmzip30xp ; real_T awrurg1zmy ; real_T h4fxaiqvyl ; real_T euvpfhvqhz ;
real_T akw04q0asa ; real_T fmfmahjizi ; real_T kcaisrgvjm ; real_T kb3yih1xdi
; real_T kduibceqxq ; real_T knnrqhsfui ; real_T gsb54uku3l ; real_T
huwt4a5rbi ; real_T fxmrh3cchn ; real_T lzaxcymkxk ; real_T eexthrg0py ;
real_T heihunb43x ; real_T olyyvkfew4 ; real_T f10mw4wrfz ; real_T onuoiaanvo
; real_T d5qx1wyeik ; real_T loemvl42qz ; real_T ow11ibemug ; real_T
gwwpugbml0 ; real_T ae1nyib4nh ; real_T htjmcyxm4o ; real_T ilhtrsad2u ;
real_T bzzw2dv1ny ; real_T mq00slxbar ; real_T fhncp1qyw1 ; real_T muxovlbgtv
; real_T eiounl1kd2 ; real_T howfzjg0z4 ; real_T fqgar3z0da ; real_T
e3uxf1rx4a ; real_T mitttnwtyr ; real_T l0d30ukkiq ; real_T l1xrrn10pr ;
real_T keefsnuxrd ; real_T iqoqu01tut ; real_T hfimc5myup [ 4 ] ; real_T
n0e3jmigef ; real_T b0rtda24hj ; real_T nwsjnwwyrd ; real_T j0tsqlxxlp ;
real_T gcdisv1ijf ; real_T ezvob1t52f ; real_T jhunvt3eqk ; real_T mo41cxvmi2
; real_T lhjpc5s0cg ; real_T hcfwego5f4 ; real_T hvbxnfvt4i ; real_T
e54atfpdeq ; real_T g2o3iq2kqm ; real_T ncqiv444nv [ 2 ] ; real_T cjlzumffng
; real_T ecey0q2vme ; real_T igl1rmdosy [ 4 ] ; real_T hceqogjf5t [ 4 ] ;
real_T adlprvfww0 [ 4 ] ; real_T peonb2s13j [ 4 ] ; real_T auwitbs4pp [ 4 ] ;
real_T jkovacda1u [ 2 ] ; real_T ptmdu1k3aj ; real_T ojmqkmcuoj ; real_T
keckzqo4yo ; real_T bq3d3y3bcr ; real_T no5ogczi4y ; real_T pydroeurl5 ;
real_T gcnlpfbb30 ; real_T akkevkcjwq ; real_T aexzz4isvj ; real_T np3w5lg3ur
; real_T ogknms4cd3 ; real_T fu1dru4srm ; real_T nfazloyrkv ; real_T
jyqcip0a0k ; real_T gxcqbrikiy ; real_T ngki0owl2f ; real_T gl5k23gezs ;
real_T iltfgaylem ; real_T ku3yiw1fh4 ; real_T ffiu1goml5 ; real_T d31qwvxqqq
; real_T ihdf0sp0fl ; real_T b2pdjcuv24 ; real_T mpxre0gqhe ; real_T
lukedum401 ; real_T at4svfakyj [ 13 ] ; real_T gfa3kvzw15 [ 7 ] ; real_T
gtvmdf51i0 ; real_T gt21ysbvs0 ; real_T o3hqxq2oxj ; real_T b3ryv0v1zr ;
real_T o0rsqrdpoy ; boolean_T is1k135npl ; char pad_is1k135npl [ 7 ] ; }
n3qi1whofz ; typedef struct { real_T pd3luz4avz [ 4 ] ; real_T lkjsqizslz ;
real_T dxwkuh3yet ; real_T b3cy3g4cyx ; real_T fv1uvl41xo ; real_T bgv2ulrz1x
; real_T mo5i5rvr5i ; real_T n1uqml3zbu ; real_T j1gf540g2s ; real_T
j0jutlrn4j ; real_T opbvzhddhh [ 2 ] ; real_T dppbo3ehy2 [ 3 ] ; real_T
l4dlj3krq0 [ 4 ] ; real_T bxpfaqqn5g [ 4 ] ; real_T hbwmzyylnb ; real_T
es20fy2lvr ; real_T ltialakutr ; real_T e3hcogjryj ; real_T m21yjcdjmh [ 7 ]
; real_T b2gcy3vb2a [ 4 ] ; real_T gzppta2yd5 ; real_T h4dq2buhxq ; real_T
jdlga5gylp ; real_T b0kcb2fd0b ; real_T osnm2k1bun ; real_T kaozvtc11k ;
struct { real_T modelTStart ; } d1hpfnitxq ; void * k5gof4wkwx ; struct {
void * LoggedData ; } m1f3rggy2e ; struct { void * LoggedData ; } h3xatpgftt
; struct { void * TUbufferPtrs [ 2 ] ; } bxxpgfrdrd ; struct { void *
uBuffers ; } mesli0okvc ; struct { void * LoggedData ; } lwlp0nygmj ; struct
{ void * LoggedData ; } diznhmxmss ; struct { void * LoggedData ; }
mwxmqn3uxu ; struct { void * uBuffers ; } b2odtudvmj ; struct { void *
uBuffers ; } pyupkhl1mt ; struct { void * AS ; void * BS ; void * CS ; void *
DS ; void * DX_COL ; void * BD_COL ; void * TMP1 ; void * TMP2 ; void * XTMP
; void * SWITCH_STATUS ; void * SWITCH_STATUS_INIT ; void * SW_CHG ; void *
CHOPPER ; void * G_STATE ; void * IDX_SW_CHG ; void * Y_SWITCH ; void *
SWITCH_TYPES ; void * IDX_OUT_SW ; } ox01apbhye ; int32_T fueor4l32w ; struct
{ int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } cmbq4gun1g ; struct { int_T indBeg ; int_T indEnd ; int_T
bufSz ; int_T maxDiscrDelay ; } ify2lmx2uw ; struct { int_T indBeg ; int_T
indEnd ; int_T bufSz ; int_T maxDiscrDelay ; } dncqeseerq ; struct { int_T
indBeg ; int_T indEnd ; int_T bufSz ; int_T maxDiscrDelay ; } ovl34hg3tv ;
int_T eiuncsbtre [ 4 ] ; int8_T cvc2xojcfc ; int8_T ccnsfwllyk ; int8_T
bzrv5d0tmm ; int8_T b13awnigwz [ 4 ] ; uint8_T p0mjtkssxb ; uint8_T
i5rjtcs5sd ; uint8_T j1qqayjy1b ; boolean_T m4eictromn ; boolean_T krdkwp0a5c
; boolean_T oqbijwo5xn ; boolean_T byh0btktux ; boolean_T m1v05clsvv ; char
pad_m1v05clsvv [ 5 ] ; } ew10rzwqr2 ; typedef struct { real_T n52bxvtwu4 ;
real_T fahgdobdau ; real_T pjbcugwxf0 ; real_T nsp3ji42uc ; } f1xhd02yjc ;
typedef struct { real_T n52bxvtwu4 ; real_T fahgdobdau ; real_T pjbcugwxf0 ;
real_T nsp3ji42uc ; } pqmvzr1kvu ; typedef struct { boolean_T n52bxvtwu4 ;
boolean_T fahgdobdau ; boolean_T pjbcugwxf0 ; boolean_T nsp3ji42uc ; }
biadadh31q ; typedef struct { real_T n52bxvtwu4 ; real_T fahgdobdau ; real_T
pjbcugwxf0 ; real_T nsp3ji42uc ; } dnp02qqx2g ; typedef struct { real_T
j2wg3uhh42 ; real_T osxehvr00q ; real_T gxb5rs1fuh ; } d15n1ti4kq ; typedef
struct { ZCSigState fjdtztelkf ; ZCSigState hfplf0mnsg ; ZCSigState
dhrrf5wdtl ; } o24blyp2sp ; struct loikxjbxjg_ { real_T P_0 ; real_T P_1 ;
real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ; real_T P_7 ;
real_T P_8 ; real_T P_9 ; real_T P_10 ; real_T P_11 ; real_T P_12 ; real_T
P_13 ; real_T P_14 ; real_T P_15 ; real_T P_16 ; real_T P_17 ; real_T P_18 ;
real_T P_19 ; real_T P_20 ; real_T P_21 ; real_T P_22 ; real_T P_23 ; real_T
P_24 ; real_T P_25 ; real_T P_26 ; real_T P_27 ; real_T P_28 ; real_T P_29 ;
real_T P_30 ; real_T P_31 ; real_T P_32 ; real_T P_33 ; real_T P_34 ; real_T
P_35 ; real_T P_36 ; real_T P_37 ; real_T P_38 ; real_T P_39 ; real_T P_40 ;
real_T P_41 ; real_T P_42 ; real_T P_43 ; real_T P_44 [ 3 ] ; real_T P_45 ;
real_T P_46 ; real_T P_47 ; real_T P_48 ; real_T P_49 ; real_T P_50 ; real_T
P_51 ; real_T P_52 ; real_T P_53 ; real_T P_54 ; real_T P_55 ; real_T P_56 ;
real_T P_57 ; real_T P_58 ; real_T P_59 ; real_T P_60 ; real_T P_61 ; real_T
P_62 ; real_T P_63 ; real_T P_64 ; real_T P_65 ; real_T P_66 ; real_T P_67 ;
real_T P_68 ; real_T P_69 ; real_T P_70 ; real_T P_71 ; real_T P_72 ; real_T
P_73 ; real_T P_74 ; real_T P_75 ; real_T P_76 ; real_T P_77 ; real_T P_78 ;
real_T P_79 ; real_T P_80 ; real_T P_81 ; real_T P_82 ; real_T P_83 ; real_T
P_84 ; real_T P_85 ; real_T P_86 ; real_T P_87 ; real_T P_88 ; real_T P_89 [
2 ] ; real_T P_90 [ 2 ] ; real_T P_91 ; real_T P_92 ; real_T P_93 ; real_T
P_94 ; real_T P_95 ; real_T P_96 ; real_T P_97 ; real_T P_98 ; real_T P_99 ;
real_T P_100 ; real_T P_101 ; real_T P_102 ; real_T P_103 ; real_T P_104 ;
real_T P_105 ; real_T P_106 ; real_T P_107 ; real_T P_108 ; real_T P_109 ;
real_T P_110 ; real_T P_111 ; real_T P_112 ; real_T P_113 ; real_T P_114 ;
real_T P_115 ; real_T P_116 ; real_T P_117 ; real_T P_118 ; real_T P_119 ;
real_T P_120 ; real_T P_121 ; real_T P_122 ; real_T P_123 ; real_T P_124 ;
real_T P_125 ; real_T P_126 ; real_T P_127 ; real_T P_128 ; real_T P_129 ;
real_T P_130 [ 4 ] ; real_T P_131 [ 2 ] ; real_T P_132 [ 2 ] ; real_T P_133 ;
real_T P_134 [ 2 ] ; real_T P_135 ; real_T P_136 ; real_T P_137 ; real_T
P_138 ; real_T P_139 ; real_T P_140 ; real_T P_141 ; real_T P_142 ; real_T
P_143 ; real_T P_144 ; real_T P_145 ; real_T P_146 [ 4 ] ; real_T P_147 [ 4 ]
; real_T P_148 ; real_T P_149 ; real_T P_150 [ 3 ] ; real_T P_151 [ 3 ] ;
real_T P_152 ; real_T P_153 ; real_T P_154 ; real_T P_155 ; real_T P_156 ;
real_T P_157 ; real_T P_158 ; real_T P_159 ; real_T P_160 ; real_T P_161 ;
real_T P_162 ; real_T P_163 ; real_T P_164 ; real_T P_165 ; real_T P_166 ;
real_T P_167 ; real_T P_168 ; real_T P_169 ; real_T P_170 ; real_T P_171 ;
boolean_T P_172 ; char pad_P_172 [ 7 ] ; } ; extern loikxjbxjg o2iu0a2jke ;
#endif
