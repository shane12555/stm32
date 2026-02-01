#include <sys/stat.h>
#include <stdint.h>
#include "uart.h" // 必須要有這行，不然找不到 uart_write

extern char _ebss; 

/* 這裡是唯一的 _write，負責轉交給 UART */
int _write(int file, char *ptr, int len) {
    int i;
    for (i = 0; i < len; i++) {
        uart_write(ptr[i]); 
    }
    return len;
}

/* 這裡是 _sbrk，給 malloc 用的 */
void *_sbrk(int incr) {
    static char *heap_end = 0;
    char *prev_heap_end;
    if (heap_end == 0) {
        heap_end = &_ebss;
    }
    prev_heap_end = heap_end;
    heap_end += incr;
    return (void *)prev_heap_end;
}

/* 其他必要的空函式 */
int _close(int file) { return -1; }
int _fstat(int file, struct stat *st) { st->st_mode = S_IFCHR; return 0; }
int _isatty(int file) { return 1; }
int _lseek(int file, int ptr, int dir) { return 0; }
int _read(int file, char *ptr, int len) { return 0; }
void _exit(int status) { while(1); }
int _getpid(void) { return -1; }
int _kill(int pid, int sig) { return -1; }