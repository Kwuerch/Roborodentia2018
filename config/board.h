#ifndef BOARD_H_
#define BOARD_H_

#define CPU_HZ 60000000
#define PBA_HZ 60000000
#define PBC_HZ 60000000

#define TC0_A_PORT (AVR32_TC0_A0_0_0_PIN / 32)
#define TC0_A_PIN  (1 << (AVR32_TC0_A0_0_0_PIN % 32))

#define SPI0_PORT (AVR32_SPI0_MISO_PIN / 32)

#define SPI0_MISO_PIN (1 << (AVR32_SPI0_MISO_PIN % 32))
#define SPI0_MOSI_PIN (1 << (AVR32_SPI0_MOSI_PIN % 32))
#define SPI0_SCK_PIN (1 << (AVR32_SPI0_SCK_PIN % 32))
#define SPI0_NPCS_0_PIN (1 << (AVR32_SPI0_NPCS_0_PIN % 32))
#define SPI0_NPCS_1_PIN (1 << (AVR32_SPI0_NPCS_1_PIN % 32))
#define SPI0_NPCS_2_PIN (1 << (AVR32_SPI0_NPCS_2_PIN % 32))
#define SPI0_NPCS_3_PIN (1 << (AVR32_SPI0_NPCS_3_PIN % 32))

#define SPI_CS_PORT 1 // Port B
#define SPI_CS0 4
#define SPI_CS1 5
#define SPI_CS2 6
#define SPI_CS3 7

#define TWI0_PORT (AVR32_TWIMS0_TWD_0_0_PIN / 32)

#define TWID_PIN  (1 << (AVR32_TWIMS0_TWD_0_0_PIN % 32))
#define TWICK_PIN (1 << (AVR32_TWIMS0_TWCK_0_0_PIN % 32))


#define TWI_SPEED 100000 // 100kHz

void init_twi();
void init_spi();

#endif
