/**
*****************************************************************************
**
**  File        : syscalls.c
**
**  Abstract    : Atollic TrueSTUDIO Minimal System calls file
**
** 		          For more information about which c-functions
**                need which of these lowlevel functions
**                please consult the Newlib libc-manual
**
**  Environment : Atollic TrueSTUDIO
**
**  Distribution: The file is distributed �as is,� without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. Distribution of this file (unmodified or modified) is not
**  permitted. Atollic AB permit registered Atollic TrueSTUDIO(R) users the
**  rights to distribute the assembled, compiled & linked contents of this
**  file as part of an application binary file, provided that it is built
**  using the Atollic TrueSTUDIO(R) Pro toolchain.
**
*****************************************************************************
*/

/* Includes */
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include "../bsp/bsp.h"

/* Variables */
#undef errno
extern int errno;
extern int __io_putchar(int ch) __attribute__((weak));
extern int __io_getchar(void) __attribute__((weak));

register char * stack_ptr asm("sp");

char *__env[1] = { 0 };
char **environ = __env;


/* Functions */
void initialise_monitor_handles()
{
}

int _getpid(void)
{
	return 1;
}

int _kill(__attribute__((unused)) int pid, __attribute__((unused)) int sig)
{
	// errno = EINVAL;
	return -1;
}

void _exit (int status)
{
	_kill(status, -1);
	while (1) {}		/* Make sure we hang here */
}

int _read (__attribute__((unused)) int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
	  *ptr++ = __io_getchar();
	}

	return len;
}

int _write(__attribute__((unused)) int file, char *ptr, int len)
{
    int i= 0;
    while(i < len) {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
        USART_SendData(USART2, *ptr++);
        i++;
    }
    
    return len;
}

caddr_t _sbrk(int incr)
{
	extern char end asm("end");
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0)
		heap_end = &end;

	prev_heap_end = heap_end;
	if (heap_end + incr > stack_ptr)
	{
//		write(1, "Heap and stack collision\n", 25);
//		abort();
		// errno = ENOMEM;
		return (caddr_t) -1;
	}

	heap_end += incr;

	return (caddr_t) prev_heap_end;
}

int _close(__attribute__((unused)) int file)
{
	return -1;
}


int _fstat(__attribute__((unused)) int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(__attribute__((unused)) int file)
{
	return 1;
}

int _lseek(__attribute__((unused)) int file, __attribute__((unused)) int ptr, __attribute__((unused)) int dir)
{
	return 0;
}

int _open(__attribute__((unused)) char *path, __attribute__((unused)) int flags, ...)
{
	/* Pretend like we always fail */
	return -1;
}

int _wait(__attribute__((unused)) int *status)
{
	// errno = ECHILD;
	return -1;
}

int _unlink(__attribute__((unused)) char *name)
{
	// errno = ENOENT;
	return -1;
}

int _times(__attribute__((unused)) struct tms *buf)
{
	return -1;
}

int _stat(__attribute__((unused)) char *file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _link(__attribute__((unused)) char *old, __attribute__((unused)) char *new)
{
	// errno = EMLINK;
	return -1;
}

int _fork(void)
{
	// errno = EAGAIN;
	return -1;
}

int _execve(__attribute__((unused)) char *name, __attribute__((unused)) char **argv, __attribute__((unused)) char **env)
{
	// errno = ENOMEM;
	return -1;
}
