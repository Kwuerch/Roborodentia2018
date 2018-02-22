#ifndef BOARD_H_
#define BOARD_H_

#define CPU_HZ 60000000
#define PBA_HZ 60000000
#define PBB_HZ 60000000
#define PBC_HZ 60000000

/** TC0 **/
#define TC0_A_PORT (AVR32_TC0_A0_0_0_PIN / 32)
#define TC0_A_PIN  (1 << (AVR32_TC0_A0_0_0_PIN % 32))

/** TWIM **/
#define TWIM ((avr32_twim_t*)AVR32_TWIM0_ADDRESS)
#define TWI0_PORT (AVR32_TWIMS0_TWD_0_0_PIN / 32)

#define TWID_PIN  (1 << (AVR32_TWIMS0_TWD_0_0_PIN % 32))
#define TWICK_PIN (1 << (AVR32_TWIMS0_TWCK_0_0_PIN % 32))

#define TWI_SPEED 100000 // 100kHz

/** USART **/
#define USART ((avr32_usart_t*)AVR32_USART1_ADDRESS)

#define USART1_PORT (AVR32_USART1_RXD_PIN / 32)

#define RX_PIN  (1 << (AVR32_USART1_RXD_PIN % 32))
#define TX_PIN (1 << (AVR32_USART1_TXD_PIN % 32))

/** SPI **/
#define SPI ((avr32_spi_t*)AVR32_SPI0_ADDRESS)

#define SPI0_PORT (AVR32_SPI0_MISO_PIN / 32)

#define SPI0_MISO_PIN (1 << (AVR32_SPI0_MISO_PIN % 32))
#define SPI0_MOSI_PIN (1 << (AVR32_SPI0_MOSI_PIN % 32))
#define SPI0_SCK_PIN (1 << (AVR32_SPI0_SCK_PIN % 32))

#define SPI0_NPCS_0_PIN (1 << (AVR32_SPI0_NPCS_0_PIN % 32))
#define SPI0_NPCS_1_PIN (1 << (AVR32_SPI0_NPCS_1_PIN % 32))
#define SPI0_NPCS_2_PIN (1 << (AVR32_SPI0_NPCS_2_PIN % 32))
#define SPI0_NPCS_3_PIN (1 << (AVR32_SPI0_NPCS_3_PIN % 32))

#define SPI_CS_PORT 3
#define SPI_CS0_PIN ((uint32_t)(1 << 3))
#define SPI_CS1_PIN ((uint32_t)(1 << 4))
#define SPI_CS2_PIN ((uint32_t)(1 << 5))
#define SPI_CS3_PIN ((uint32_t)(1 << 6))

/** DRV8711 Motor Driver **/
typedef enum DRV8711_ID{
   DRV8711_FL = 0,
   DRV8711_BR,
   DRV8711_FR,
   DRV8711_BL
}DRV8711_ID;

/** VL53L0X Sensor **/
typedef enum VL53L0X_ID{
   VL53L0X_F = 0,
   VL53L0X_B,
   VL53L0X_L,
   VL53L0X_R
}VL53L0X_ID;

#define VL53L0X_ADDR_F ((uint8_t)0x20)
#define VL53L0X_ADDR_B ((uint8_t)0x21)
#define VL53L0X_ADDR_L ((uint8_t)0x22)
#define VL53L0X_ADDR_R ((uint8_t)0x23)

#define VL53L0X_PORT 2
#define VL53L0X_SD_PIN_F ((uint32_t)(1 << 9))
#define VL53L0X_SD_PIN_B ((uint32_t)(1 << 10))
#define VL53L0X_SD_PIN_L ((uint32_t)(1 << 11))
#define VL53L0X_SD_PIN_R ((uint32_t)(1 << 12))

/** Function Prototypes **/
void init_twi();
void init_spi();
void init_usart();
void init_vl53l0x_sd();

#endif
