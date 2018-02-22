#include <stdio.h>
#include <stdarg.h>
#include "board.h"
#include "console.h"
#include "usart.h"

#define STR_BUF_SIZE 256
char strBuf[STR_BUF_SIZE];

void console_init(){
    init_usart();
    console_print_str("Hello\r\n");
}

void console_printf(char* format, ...){
    va_list args;

    va_start(args, format);
    vsnprintf(strBuf, STR_BUF_SIZE, format, args);
    va_end(args);

    console_print_str(strBuf);
}

void console_print(char c){
    usart_write_char(USART, c);
}

void console_print_str(char* str){
    usart_write_line(USART, str);
}
