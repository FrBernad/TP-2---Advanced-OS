#include <RTCTime.h>
#include <keyboardDriver.h>
#include <lib.h>
#include <memoryManager.h>
#include <semaphores.h>
#include <stringLib.h>
#include <sysCallDispatcher.h>
#include <taskManager.h>
#include <timerTick.h>
#include <videoDriver.h>

#define SYS_GETMEM_ID 0
#define SYS_RTC_TIME_ID 1
#define SYS_TEMP_ID 2
#define SYS_WRITE_ID 3
#define SYS_GETCHAR_ID 4
#define SYS_CLEAR_ID 5
#define SYS_LOAD_APP_ID 6
#define SYS_INFOREG_ID 7
#define SYS_PS_ID 8
#define SYS_SECS_ELAPSED_ID 9
#define SYS_KILL_ID 10
#define SYS_NICE_ID 11
#define SYS_BLOCK_ID 12
#define SYS_GETPID_ID 13
#define SYS_MALLOC_ID 14
#define SYS_FREE_ID 15
#define SYS_YIELD_ID 16
#define SYS_SEM_OPEN_ID 17
#define SYS_SEM_CLOSE_ID 18
#define SYS_SEM_POST_ID 19
#define SYS_SEM_WAIT_ID 20

#define SYSCALLS 21

uint64_t sysCallDispatcher(t_registers *r) {
      if (r->rax >= 0 && r->rax < SYSCALLS) {
            switch (r->rax) {
                  case SYS_GETMEM_ID:
                        sys_getMem(r->rdi, (uint8_t *)r->rsi);
                        break;

                  case SYS_RTC_TIME_ID:
                        return getDecimalTimeInfo((t_timeInfo)(r->rdi));
                        break;

                  case SYS_TEMP_ID:
                        return cpuTemp();
                        break;

                  case SYS_WRITE_ID:
                        sys_write((char *)(r->rdi), (uint8_t)(r->rsi), (t_colour)(r->rdx), (t_colour)(r->r10));
                        break;

                  case SYS_GETCHAR_ID:
                        return getchar();
                        break;

                  case SYS_CLEAR_ID:
                        clearScreen();
                        break;

                  case SYS_LOAD_APP_ID:
                        return addProcess((void (*)(int, char **))r->rdi, (int)r->rsi, (char **)r->rdx, (uint8_t)r->r10);
                        break;

                  case SYS_INFOREG_ID:
                        return (uint64_t)getSnapshot();
                        break;

                  case SYS_PS_ID:
                        listProcesses();
                        break;

                  case SYS_SECS_ELAPSED_ID:
                        return secondsElapsed();
                        break;

                  case SYS_KILL_ID:
                        return killProcess((uint64_t)r->rdi);
                        break;

                  case SYS_NICE_ID:
                        changePriority((uint64_t)r->rdi, (uint64_t)r->rsi);
                        break;

                  case SYS_BLOCK_ID:
                        return blockProcess((uint64_t)r->rdi);
                        break;

                  case SYS_GETPID_ID:
                        return currentProcessPid();
                        break;

                  case SYS_MALLOC_ID:
                        return (uint64_t)mallocBR((uint32_t)r->rdi);
                        break;

                  case SYS_FREE_ID:
                        freeBR((void *)r->rdi);
                        break;

                  case SYS_YIELD_ID:
                        yield();
                        break;

                  case SYS_SEM_OPEN_ID:
                        return sem_open((char *)r->rdi,r->rsi);
                        break;

                  case SYS_SEM_CLOSE_ID:
                        return sem_close((int)r->rdi);
                        break;

                  case SYS_SEM_POST_ID:
                        return sem_post((int)r->rdi);
                        break;

                  case SYS_SEM_WAIT_ID:
                        return sem_wait((int)r->rdi);
                        break;
            }
      }
      return 0;
}