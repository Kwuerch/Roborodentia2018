/*****************************************************************************
 *
 * \file
 *
 * \brief AT32UC3C UC3C_EK board header file.
 *
 * This file contains definitions and services related to the features of the
 * UC3C_EK board.
 *
 * To use this board, define BOARD=UC3C_EK.
 *
 * Copyright (C) 2009 - 2011 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 ******************************************************************************/


#ifndef _UC3C_EK_H_
#define _UC3C_EK_H_

#include "compiler.h"

#ifdef __AVR32_ABI_COMPILER__ // Automatically defined when compiling for AVR32, not when assembling.
#  include "led.h"
#endif  // __AVR32_ABI_COMPILER__


#ifdef AVR32_SCIF_101_H_INCLUDED
#define AVR32_SCIF_OSCCTRL0_STARTUP_2048_RCOSC  0x00000003
#define AVR32_SCIF_OSCCTRL0_STARTUP_16384_RCOSC 0x00000006
#define AVR32_SCIF_OSCCTRL32_STARTUP_8192_RCOSC 0x00000002
#endif




#define FOSC32          AVR32_SCIF_OSC32_FREQUENCY              //!< Osc32 frequency: Hz.
#define OSC32_STARTUP   AVR32_SCIF_OSCCTRL32_STARTUP_8192_RCOSC //!< Osc32 startup time: RCOsc periods.

// Osc0 crystal is not mounted by default. Set the following definitions to the
// appropriate values if a custom Osc0 crystal is mounted on your STK600
#define FOSC0           16000000                                //!< Osc0 frequency: Hz.
#define OSC0_STARTUP    AVR32_SCIF_OSCCTRL0_STARTUP_2048_RCOSC  //!< Osc0 startup time: RCOsc periods.

// Osc1 crystal is not mounted by default. Set the following definitions to the
// appropriate values if a custom Osc1 crystal is mounted on your board.
// #define FOSC1           12000000                              //!< Osc1 frequency: Hz.
// #define OSC1_STARTUP    AVR32_SCIF_OSCCTRL1_STARTUP_2048_RCOSC  //!< Osc1 startup time: RCOsc periods.


#define BOARD_OSC0_HZ           16000000
#define BOARD_OSC0_STARTUP_US   2000
#define BOARD_OSC0_IS_XTAL      true
#define BOARD_OSC32_HZ          32768
#define BOARD_OSC32_STARTUP_US  71000
#define BOARD_OSC32_IS_XTAL     true



#define SDRAM_PART_HDR  "mt48lc16m16a2tg7e/mt48lc16m16a2tg7e.h"

#define SDRAM_DBW       16







#define USB_ID                             AVR32_USBC_ID_

#define USB_VBOF                           AVR32_USBC_VBOF

# define USB_VBOF_ACTIVE_LEVEL       LOW

# define USB_OVERCURRENT_DETECT_PIN  AVR32_PIN_PB7



#define LED_COUNT   4


#  define LED0_GPIO   AVR32_PIN_PA08
#  define LED1_GPIO   AVR32_PIN_PD23
#  define LED2_GPIO   AVR32_PIN_PC13
#  define LED3_GPIO   AVR32_PIN_PD22



#define LED0_PWM      (-1)
#define LED1_PWM      (-1)
#define LED2_PWM      (-1)
#define LED3_PWM      (-1)



/* TODO: Implement PWM functionality */
#define LED0_PWM_FUNCTION   (-1)//AVR32_PWM_0_FUNCTION
#define LED1_PWM_FUNCTION   (-1)//AVR32_PWM_1_FUNCTION
#define LED2_PWM_FUNCTION   (-1)
#define LED3_PWM_FUNCTION   (-1)



#define LED_MONO0_GREEN   LED0
#define LED_MONO1_GREEN   LED1
#define LED_MONO2_GREEN   LED2
#define LED_MONO3_GREEN   LED3



#define GPIO_PUSH_BUTTON_0            AVR32_PIN_PA14
#define GPIO_PUSH_BUTTON_0_PRESSED    0
#define GPIO_PUSH_BUTTON_1            AVR32_PIN_PA29
#define GPIO_PUSH_BUTTON_1_PRESSED    0



#  define UH   AVR32_PIN_PB09
#  define UL   AVR32_PIN_PB08
#  define VH   AVR32_PIN_PB11
#  define VL   AVR32_PIN_PB10
#  define WH   AVR32_PIN_PB13
#  define WL   AVR32_PIN_PB12
#  define XH   AVR32_PIN_PB15
#  define XL   AVR32_PIN_PB14
#  define ShV  AVR32_PIN_PA21



#define AT45DBX_SPI                 (&AVR32_SPI1)
#define AT45DBX_SPI_NPCS            1
#define AT45DBX_SPI_SCK_PIN         AVR32_SPI1_SCK_0_1_PIN
#define AT45DBX_SPI_SCK_FUNCTION    AVR32_SPI1_SCK_0_1_FUNCTION
#define AT45DBX_SPI_MISO_PIN        AVR32_SPI1_MISO_0_1_PIN
#define AT45DBX_SPI_MISO_FUNCTION   AVR32_SPI1_MISO_0_1_FUNCTION
#define AT45DBX_SPI_MOSI_PIN        AVR32_SPI1_MOSI_0_1_PIN
#define AT45DBX_SPI_MOSI_FUNCTION   AVR32_SPI1_MOSI_0_1_FUNCTION
#define AT45DBX_SPI_NPCS0_PIN       AVR32_SPI1_NPCS_1_2_PIN
#define AT45DBX_SPI_NPCS0_FUNCTION  AVR32_SPI1_NPCS_1_2_FUNCTION



#define SD_MMC_CARD_DETECT_PIN      AVR32_PIN_PA28
#define SD_MMC_WRITE_PROTECT_PIN    AVR32_PIN_PD30
#define SD_MMC_SPI                  (&AVR32_SPI1)
#define SD_MMC_SPI_NPCS             3
#define SD_MMC_SPI_SCK_PIN          AVR32_SPI1_SCK_0_1_PIN
#define SD_MMC_SPI_SCK_FUNCTION     AVR32_SPI1_SCK_0_1_FUNCTION
#define SD_MMC_SPI_MISO_PIN         AVR32_SPI1_MISO_0_1_PIN
#define SD_MMC_SPI_MISO_FUNCTION    AVR32_SPI1_MISO_0_1_FUNCTION
#define SD_MMC_SPI_MOSI_PIN         AVR32_SPI1_MOSI_0_1_PIN
#define SD_MMC_SPI_MOSI_FUNCTION    AVR32_SPI1_MOSI_0_1_FUNCTION
#define SD_MMC_SPI_NPCS_PIN         AVR32_SPI1_NPCS_3_2_PIN
#define SD_MMC_SPI_NPCS_FUNCTION    AVR32_SPI1_NPCS_3_2_FUNCTION






#define ET024006DHU_TE_PIN              AVR32_PIN_PD19
#define ET024006DHU_RESET_PIN           AVR32_PIN_PD16
#define ET024006DHU_BL_PIN              AVR32_TC0_B0_0_2_PIN
#define ET024006DHU_BL_FUNCTION         AVR32_TC0_B0_0_2_FUNCTION
#define ET024006DHU_DNC_PIN             AVR32_EBI_ADDR_22_PIN
#define ET024006DHU_DNC_FUNCTION        AVR32_EBI_ADDR_22_FUNCTION
#define ET024006DHU_EBI_NCS_PIN         AVR32_EBI_NCS_PIN
#define ET024006DHU_EBI_NCS_FUNCTION    AVR32_EBI_NCS_FUNCTION




#define ET024006DHU_SPI                  (&AVR32_SPI1)
#define ET024006DHU_SPI_NPCS             1
#define ET024006DHU_SPI_SCK_PIN          AVR32_SPI1_SCK_0_1_PIN
#define ET024006DHU_SPI_SCK_FUNCTION     AVR32_SPI1_SCK_0_1_FUNCTION
#define ET024006DHU_SPI_MISO_PIN         AVR32_SPI1_MISO_0_1_PIN
#define ET024006DHU_SPI_MISO_FUNCTION    AVR32_SPI1_MISO_0_1_FUNCTION
#define ET024006DHU_SPI_MOSI_PIN         AVR32_SPI1_MOSI_0_1_PIN
#define ET024006DHU_SPI_MOSI_FUNCTION    AVR32_SPI1_MOSI_0_1_FUNCTION
#define ET024006DHU_SPI_NPCS_PIN         AVR32_SPI1_NPCS_2_2_PIN
#define ET024006DHU_SPI_NPCS_FUNCTION    AVR32_SPI1_NPCS_2_2_FUNCTION





#define TPA6130_TWI                  (&AVR32_TWIM0_ADDRESS)
#define TPA6130_TWI_SCL_PIN          AVR32_TWIMS0_TWCK_0_1_PIN
#define TPA6130_TWI_SCL_FUNCTION     AVR32_TWIMS0_TWCK_0_1_FUNCTION
#define TPA6130_TWI_SDA_PIN          AVR32_TWIMS0_TWD_0_0_PIN
#define TPA6130_TWI_SDA_FUNCTION     AVR32_TWIMS0_TWD_0_0_FUNCTION


#define USART                        (&AVR32_USART2)
#define USART_RXD_PIN                AVR32_USART2_RXD_0_1_PIN
#define USART_RXD_FUNCTION           AVR32_USART2_RXD_0_1_FUNCTION
#define USART_TXD_PIN                AVR32_USART2_TXD_0_1_PIN
#define USART_TXD_FUNCTION           AVR32_USART2_TXD_0_1_FUNCTION
#define USART_IRQ                    AVR32_USART2_IRQ
#define USART_IRQ_GROUP              AVR32_USART2_IRQ_GROUP
#define USART_SYSCLK                 SYSCLK_USART2


#define ADC_VEXT_PIN         AVR32_PKGANA_ADCIN5_0_0_PIN
#define ADC_VEXT_FUNCTION    AVR32_PKGANA_ADCIN5_0_0_FUNCTION


#define ET024006DHU_SMC_USE_NCS           0
#define ET024006DHU_SMC_COMPONENT_CS      "smc_et024006dhu.h"

#define ET024006DHU_EBI_DATA_0    AVR32_EBI_DATA_0
#define ET024006DHU_EBI_DATA_1    AVR32_EBI_DATA_1
#define ET024006DHU_EBI_DATA_2    AVR32_EBI_DATA_2
#define ET024006DHU_EBI_DATA_3    AVR32_EBI_DATA_3
#define ET024006DHU_EBI_DATA_4    AVR32_EBI_DATA_4
#define ET024006DHU_EBI_DATA_5    AVR32_EBI_DATA_5
#define ET024006DHU_EBI_DATA_6    AVR32_EBI_DATA_6
#define ET024006DHU_EBI_DATA_7    AVR32_EBI_DATA_7
#define ET024006DHU_EBI_DATA_8    AVR32_EBI_DATA_8
#define ET024006DHU_EBI_DATA_9    AVR32_EBI_DATA_9
#define ET024006DHU_EBI_DATA_10   AVR32_EBI_DATA_10
#define ET024006DHU_EBI_DATA_11   AVR32_EBI_DATA_11
#define ET024006DHU_EBI_DATA_12   AVR32_EBI_DATA_12
#define ET024006DHU_EBI_DATA_13   AVR32_EBI_DATA_13
#define ET024006DHU_EBI_DATA_14   AVR32_EBI_DATA_14
#define ET024006DHU_EBI_DATA_15   AVR32_EBI_DATA_15

#define ET024006DHU_EBI_ADDR_21   AVR32_EBI_ADDR_22

#define ET024006DHU_EBI_NWE       AVR32_EBI_NWE0
#define ET024006DHU_EBI_NRD       AVR32_EBI_NRD
#define ET024006DHU_EBI_NCS       AVR32_EBI_NCS_0


#endif  // _UC3C_EK_H_
