#include <stdio.h>
#include <stdarg.h>
#include "board.h"
#include "console.h"
#include "usart.h"
#include "flash.h"

#define STR_BUF_SIZE 256
char strBuf[STR_BUF_SIZE];

void console_printf(char* format, ...){
    va_list args;

    va_start(args, format);
    vsnprintf(strBuf, STR_BUF_SIZE, format, args);
    va_end(args);

    console_print_str(strBuf);
}

void console_printf_flash(char* format, ...){
    va_list args;
    int size;

    va_start(args, format);
    size = vsnprintf(strBuf, STR_BUF_SIZE, format, args);
    va_end(args);

    flash_write((uint8_t*)strBuf, size);
}

void console_print(char c){
    usart_write_char(USART, c);
}

void console_print_str(char* str){
    usart_write_line(USART, str);
}
