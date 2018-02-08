/*****************************************************************************
 *
 * \file
 *
 * \brief TWIM Configuration File for AVR32 UC3.
 *
 * This file defines a useful set of functions for TWIM on AVR32 devices.
 *
 *****************************************************************************/

#ifndef _CONF_TWIM_H
#define _CONF_TWIM_H
#include "board.h"

#if BOARD==UC3C_EK
// These defines are missing from or wrong in the toolchain header file ip_xxx.h or part.h
#   if UC3C
#       if !defined(AVR32_TWIM0_GROUP)
#           define AVR32_TWIM0_GROUP         25
#       else
#           warning "Duplicate define(s) to remove from the ASF"
#       endif
#       if !defined(AVR32_TWIM1_GROUP)
#           define AVR32_TWIM1_GROUP         26
#       else
#           warning "Duplicate define(s) to remove from the ASF"
#       endif
#       if !defined(AVR32_TWIM2_GROUP)
#           define AVR32_TWIM2_GROUP         45
#       else
#           warning "Duplicate define(s) to remove from the ASF"
#       endif
#   endif
#   define CONF_TWIM_IRQ_LINE          AVR32_TWIM0_IRQ
#   define CONF_TWIM_IRQ_GROUP         AVR32_TWIM0_GROUP
#   define CONF_TWIM_IRQ_LEVEL         1
#elif BOARD==EVK1104
#   if UC3A3
#       if !defined(AVR32_TWIM0_GROUP)
#           define AVR32_TWIM0_GROUP         11
#       else
#           warning "Duplicate define(s) to remove from the ASF"
#       endif
#       if !defined(AVR32_TWIM1_GROUP)
#           define AVR32_TWIM1_GROUP         12
#       else
#           warning "Duplicate define(s) to remove from the ASF"
#       endif
#   endif
#   define CONF_TWIM_IRQ_LINE          AVR32_TWIM0_IRQ
#   define CONF_TWIM_IRQ_GROUP         AVR32_TWIM0_GROUP
#   define CONF_TWIM_IRQ_LEVEL         1
#elif BOARD==UC3L_EK
#   if UC3L
#       if !defined(AVR32_TWIM0_GROUP)
#           define AVR32_TWIM0_GROUP         20
#       else
#           warning "Duplicate define(s) to remove from the ASF"
#       endif
#       if !defined(AVR32_TWIM1_GROUP)
#           define AVR32_TWIM1_GROUP         21
#       else
#           warning "Duplicate define(s) to remove from the ASF"
#       endif
#   endif
#   define CONF_TWIM_IRQ_LINE          AVR32_TWIM0_IRQ
#   define CONF_TWIM_IRQ_GROUP         AVR32_TWIM0_GROUP
#   define CONF_TWIM_IRQ_LEVEL         1
#elif BOARD==STK600_RCUC3D
#   if UC3D
#       if !defined(AVR32_TWIM_GROUP)
#           define AVR32_TWIM_GROUP         10
#       else
#           warning "Duplicate define(s) to remove from the ASF"
#       endif
#   endif
#   define CONF_TWIM_IRQ_LINE          AVR32_TWIM_IRQ
#   define CONF_TWIM_IRQ_GROUP         AVR32_TWIM_GROUP
#   define CONF_TWIM_IRQ_LEVEL         1
#else
#   error Board Not Supported
#endif

#endif // _CONF_TWIM_H
