/*!
 * @file	syscalls.c
 * Implements some base-level for libc for heap management and printing on
 * the debug port.
 */
#include "lwip_raw_conf.h"
#include "hardware_init.h"

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

#define AUTO_RETURN_AFTER_NEWLINE	1

// _write currently adds \n after \r automatically.
// If the At91lib\utility\stdio.c printf implementation is not used anymore:
// remove if there are spurious newlines!

//int _read(int file, void *ptr, size_t len) {
//	(void)len;
//	if(file <= STDERR_FILENO) {
//		*(char *) ptr = DBGU_GetChar();
//#if AUTO_RETURN_AFTER_NEWLINE
//		if((*(char *) ptr != '\n') && (*(char *) ptr != '\r')) {
//			DBGU_PutChar(*(char *) ptr);
//		}
//		else if(*(char *) ptr == '\r') {
//			DBGU_PutChar('\n');
//			DBGU_PutChar('\r');
//		}
//#endif
//		return 1;
//	}
//	else {
//		return -1;
//	}
//
//}

int _write(int file, const void *ptr, size_t len) {
	size_t i;

	if(file <= STDERR_FILENO) {
		for(i=0; i<len; i++) {
#if AUTO_RETURN_AFTER_NEWLINE
			if(((const char *) ptr)[i] == '\n' &&
					((const char *) ptr)[i+1] != '\r') {
				HAL_UART_Transmit(&huart3, (uint8_t*)"\r", 1 ,
						STM32_LWIP_UART_TIMEOUT);
			}
#endif
			uint8_t* character = (uint8_t*) ptr;
			HAL_UART_Transmit(&huart3, (uint8_t*) (character + i),
					1 , STM32_LWIP_UART_TIMEOUT);
		}
		return len;
	} else {
		return -1;
	}

}

int _lseek (int file, int ptr, int dir) {
	(void)file;
	(void)ptr;
	(void)dir;
	return 0;
}

int _close(int file) {
	(void)file;
	return -1;
}

int _open(const char *name, int flags, ...) {
	(void)name;
	(void)flags;
	return -1;
}

int _link(const char *oldpath, const char *newpath) {
	(void)oldpath;
	(void)newpath;
	return -1;
}

int _rename(const char *oldpath, const char *newpath) {
	(void)oldpath;
	(void)newpath;
	return -1;
}

int _unlink(const char *pathname) {
	(void)pathname;
	return -1;
}

int fsync(int fd) {
	(void)fd;
	return -1;
}

int _fstat(int fd, struct stat *st) {
	(void)fd;
	(void)st;
	return -1;
}

int _stat(const char *path, struct stat *st) {
	(void)path;
	(void)st;
	return -1;
}

int _isatty(int fd) {
	if(fd <= STDERR_FILENO) {
		return 1;
	}
	else {
		return 0;
	}
}

void exit(int n) {
	printf("\n\r EXITING WITH CODE: %u \n\r", n);
	//restart();
	while(1);
}

void _exit(int n) {
	printf("\n\r EXITING WITH CODE: %u \n\r", n);
	//restart();
	while(1);
}

int _kill() {
	return -1;
}

int _getpid() {
	return 1;
}

int _gettimeofday() {
	return -1;
}
