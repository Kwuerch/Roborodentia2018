#include "../peripherals/usart.h"

#define USART ((avr32_usart_t*)AVR32_USART1_ADDRESS)

#define USART1_PORT (AVR32_USART1_RXD_PIN / 32)

#define RX_PIN  (1 << (AVR32_USART1_RXD_PIN % 32))
#define TX_PIN (1 << (AVR32_USART1_TXD_PIN % 32))

#define PBA_HZ 60000000


void init_usart(){
    AVR32_GPIO.port[USART1_PORT].pmr2c = (RX_PIN | TX_PIN);
    AVR32_GPIO.port[USART1_PORT].pmr1c = (RX_PIN | TX_PIN);
    AVR32_GPIO.port[USART1_PORT].pmr0c = (RX_PIN | TX_PIN);
    AVR32_GPIO.port[USART1_PORT].gperc = (RX_PIN | TX_PIN);

    usart_options_t opts;
    opts.baudrate = 38400;
    opts.charlength = 8;
    opts.paritytype = USART_NO_PARITY;
    opts.stopbits = USART_1_STOPBIT;
    opts.channelmode = USART_NORMAL_CHMODE;

    usart_init_rs232(USART, &opts, PBA_HZ); 
}

void console_init(){
    init_usart();
}

void console_print(char c){
    usart_write_char(USART, c);
}

void console_print_str(char* str){
    usart_write_line(USART, str);
}
