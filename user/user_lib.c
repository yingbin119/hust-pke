/*
 * The supporting library for applications.
 * Actually, supporting routines for applications are catalogued as the user 
 * library. we don't do that in PKE to make the relationship between application 
 * and user library more straightforward.
 */

#include "user_lib.h"
#include "util/types.h"
#include "util/snprintf.h"
#include "kernel/syscall.h"
#include "util/snscanf.h"

uint64 do_user_call(uint64 sysnum, uint64 a1, uint64 a2, uint64 a3, uint64 a4, uint64 a5, uint64 a6,
                 uint64 a7) {
  int ret;

  // before invoking the syscall, arguments of do_user_call are already loaded into the argument
  // registers (a0-a7) of our (emulated) risc-v machine.
  asm volatile(
      "ecall\n"
      "sw a0, %0"  // returns a 32-bit value
      : "=m"(ret)
      :
      : "memory");

  return ret;
}

//
// printu() supports user/lab1_1_helloworld.c
//
int printu(const char* s, ...) {
  va_list vl;
  va_start(vl, s);

  char out[256];  // fixed buffer size.
  int res = vsnprintf(out, sizeof(out), s, vl);
  va_end(vl);
  const char* buf = out;
  size_t n = res < sizeof(out) ? res : sizeof(out);

  // make a syscall to implement the required functionality.
  return do_user_call(SYS_user_print, (uint64)buf, n, 0, 0, 0, 0, 0);
}

//
// applications need to call exit to quit execution.
//
int exit(int code) {
  return do_user_call(SYS_user_exit, code, 0, 0, 0, 0, 0, 0); 
}


//
// lib call to naive_fork
int fork() {
  return do_user_call(SYS_user_fork, 0, 0, 0, 0, 0, 0, 0);
}

//
// lib call to yield
//
void yield() {
  do_user_call(SYS_user_yield, 0, 0, 0, 0, 0, 0, 0);
}

//
// lib call to open
//
int open(const char *pathname, int flags) {
  return do_user_call(SYS_user_open, (uint64)pathname, flags, 0, 0, 0, 0, 0);
}

//
// lib call to read
//
int read_u(int fd, void * buf, uint64 count){
  return do_user_call(SYS_user_read, fd, (uint64)buf, count, 0, 0, 0, 0);
}

//
// lib call to write
//
int write_u(int fd, void *buf, uint64 count) {
  return do_user_call(SYS_user_write, fd, (uint64)buf, count, 0, 0, 0, 0);
}

//
// lib call to seek
// 
int lseek_u(int fd, int offset, int whence) {
  return do_user_call(SYS_user_lseek, fd, offset, whence, 0, 0, 0, 0);
}

//
// lib call to read file information
//
int stat_u(int fd, struct istat *istat) {
  return do_user_call(SYS_user_stat, fd, (uint64)istat, 0, 0, 0, 0, 0);
}

//
// lib call to read file information from disk
//
int disk_stat_u(int fd, struct istat *istat) {
  return do_user_call(SYS_user_disk_stat, fd, (uint64)istat, 0, 0, 0, 0, 0);
}

//
// lib call to open dir
//
int opendir_u(const char *dirname) {
  return do_user_call(SYS_user_opendir, (uint64)dirname, 0, 0, 0, 0, 0, 0);
}

//
// lib call to read dir
//
int readdir_u(int fd, struct dir *dir) {
  return do_user_call(SYS_user_readdir, fd, (uint64)dir, 0, 0, 0, 0, 0);
}

//
// lib call to make dir
//
int mkdir_u(const char *pathname) {
  return do_user_call(SYS_user_mkdir, (uint64)pathname, 0, 0, 0, 0, 0, 0);
}

//
// lib call to close dir
//
int closedir_u(int fd) {
  return do_user_call(SYS_user_closedir, fd, 0, 0, 0, 0, 0, 0);
} 

//
// lib call to link
//
int link_u(const char *fn1, const char *fn2){
  return do_user_call(SYS_user_link, (uint64)fn1, (uint64)fn2, 0, 0, 0, 0, 0);
}

//
// lib call to unlink
//
int unlink_u(const char *fn){
  return do_user_call(SYS_user_unlink, (uint64)fn, 0, 0, 0, 0, 0, 0);
}

//
// lib call to close
//
int close(int fd) {
  return do_user_call(SYS_user_close, fd, 0, 0, 0, 0, 0, 0);
}


//lib call to wait
int wait(int pid){
  return do_user_call(SYS_user_wait, pid, 0, 0, 0, 0, 0, 0);
}

//lib call to exec
int exec(char *command,char * para){
  return do_user_call(SYS_user_exec, (uint64)command, (uint64)para, 0, 0, 0, 0, 0);
}

//lib call to scan
int scanu(const char* s, ...) {
  va_list vl;
  va_start(vl, s);

  char in[256];  // fixed buffer size.
  const char* buf = in;
  do_user_call(SYS_user_scanf, (uint64)buf, 0, 0, 0, 0, 0, 0);
  int res = vsscanf(in, s, vl);

  va_end(vl);
  return res;
}
int read_cwd(char *path) {
  return do_user_call(SYS_user_read_cwd, (uint64)path, 0, 0, 0, 0, 0, 0);
}

int change_cwd(const char *path) {
  return do_user_call(SYS_user_change_cwd, (uint64)path, 0, 0, 0, 0, 0, 0);
}

void pwd(char *path){
    read_cwd(path);
}
void cd(const char *path) {
  if (change_cwd(path) != 0)
    printu("cd failed\n");
}

//
// lib call to sem_new
//
int sem_new(int value){
    return do_user_call(SYS_user_sem_new, value, 0, 0, 0, 0, 0, 0);
}

//
// lib call to sem_P
//
int sem_P(int sem_id){
    return do_user_call(SYS_user_sem_p, sem_id, 0, 0, 0, 0, 0, 0);
}
//
// lib call to sem_V
//
int sem_V(int sem_id){
    return do_user_call(SYS_user_sem_v, sem_id, 0, 0, 0, 0, 0, 0);
}
int print_backtrace(int n) {
  return do_user_call(SYS_user_print_backtrace, n, 0, 0, 0, 0, 0, 0);
}
//
// lib call to better_malloc
//
void* malloc(uint64 n) {
  return (void*)do_user_call(SYS_user_allocate_page, n, 0, 0, 0, 0, 0, 0);
}

//
// lib call to better_free
//
void free(void* va) {
  do_user_call(SYS_user_free_page, (uint64)va, 0, 0, 0, 0, 0, 0);
}

//
// lib call to printpa
//
void printpa(int *va)
{
  do_user_call(SYS_user_printpa, (uint64)va, 0, 0, 0, 0, 0, 0);
}