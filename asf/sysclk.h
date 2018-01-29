#ifndef SYSCLK_H_INCLUDED
#define SYSCLK_H_INCLUDED

#include "parts.h"
#include "conf_clock.h"

#if SAM3S
# include "sam3s/sysclk.h"
#elif SAM3U
# include "sam3u/sysclk.h"
#elif SAM3N
# include "sam3n/sysclk.h"
#elif SAM3XA
# include "sam3x/sysclk.h"
#elif SAM4S
# include "sam4s/sysclk.h"
#elif (UC3A0 || UC3A1)
# include "uc3a0_a1/sysclk.h"
#elif UC3A3
# include "uc3a3_a4/sysclk.h"
#elif UC3B
# include "uc3b0_b1/sysclk.h"
#elif UC3C
# include "uc3c/sysclk.h"
#elif UC3D
# include "uc3d/sysclk.h"
#elif UC3L
# include "uc3l/sysclk.h"
#elif XMEGA
# include "xmega/sysclk.h"
#else
# error Unsupported chip type
#endif






#endif /* SYSCLK_H_INCLUDED */
