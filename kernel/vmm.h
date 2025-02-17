#ifndef _VMM_H_
#define _VMM_H_

#include "riscv.h"
#include "process.h"

/* --- utility functions for virtual address mapping --- */
int map_pages(pagetable_t pagetable, uint64 va, uint64 size, uint64 pa, int perm);

/* --- Memory management structure --- */
typedef struct mem_control_block_t{
    int is_available; // 是否可用，1表示可用，0表示不可用
    uint64 offset; // 起始地址
    uint64 size; // 大小
    struct mem_control_block_t *next; // 勾链指针
}mem_control_block;

// permission codes.
enum VMPermision {
  PROT_NONE = 0,
  PROT_READ = 1,
  PROT_WRITE = 2,
  PROT_EXEC = 4,
};

uint64 prot_to_type(int prot, int user);
pte_t *page_walk(pagetable_t pagetable, uint64 va, int alloc);
uint64 lookup_pa(pagetable_t pagetable, uint64 va);

/* --- kernel page table --- */
// pointer to kernel page directory
extern pagetable_t g_kernel_pagetable;

void kern_vm_map(pagetable_t page_dir, uint64 va, uint64 pa, uint64 sz, int perm);

// Initialize the kernel pagetable
void kern_vm_init(void);

/* --- user page table --- */
void *user_va_to_pa(pagetable_t page_dir, void *va);
void user_vm_map(pagetable_t page_dir, uint64 va, uint64 size, uint64 pa, int perm);
void user_vm_unmap(pagetable_t page_dir, uint64 va, uint64 size, int free);
void print_proc_vmspace(process* proc);

//added @lab2_ch2
uint64 user_heap_grow(pagetable_t pagetable,uint64 old_size,uint64 new_size);
void user_better_malloc(uint64 n);
void mcb_init();
uint64 malloc(int n);
void free(void *  va);

#endif
