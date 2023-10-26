#include "__cf_PV_MPPT_BOOST1.h"
#ifndef RTW_HEADER_PV_MPPT_BOOST1_acc_private_h_
#define RTW_HEADER_PV_MPPT_BOOST1_acc_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#if !defined(ss_VALIDATE_MEMORY)
#define ss_VALIDATE_MEMORY(S, ptr)   if(!(ptr)) {\
  ssSetErrorStatus(S, RT_MEMORY_ALLOCATION_ERROR);\
  }
#endif
#if !defined(rt_FREE)
#if !defined(_WIN32)
#define rt_FREE(ptr)   if((ptr) != (NULL)) {\
  free((ptr));\
  (ptr) = (NULL);\
  }
#else
#define rt_FREE(ptr)   if((ptr) != (NULL)) {\
  free((void *)(ptr));\
  (ptr) = (NULL);\
  }
#endif
#endif
#ifndef __RTWTYPES_H__
#error This file requires rtwtypes.h to be included
#endif
extern real_T look1_binlxpw ( real_T u0 , const real_T bp0 [ ] , const real_T
table [ ] , uint32_T maxIndex ) ;
#endif
