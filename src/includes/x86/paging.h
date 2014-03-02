#ifndef PAGING_H
#define PAGING_H
#include <types.h>
#include <x86/isr.h>

typedef struct page
{
	uint32_t present    : 1;   // Page present in memory
	uint32_t rw         : 1;   // Read-only if clear, readwrite if set
	uint32_t user       : 1;   // Supervisor level only if clear
	uint32_t accessed   : 1;   // Has the page been accessed since last refresh?
	uint32_t dirty      : 1;   // Has the page been written to since last refresh?
	uint32_t unused     : 7;   // Amalgamation of unused and reserved bits
	uint32_t frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table
{
	page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
	page_table_t *tables[1024];
	uint32_t tablesPhysical[1024];
	uint32_t physicalAddr;
} page_directory_t;

void paging_init();
void paging_fault(struct regs *regs);
void paging_identity_map(uint32_t start_addr, uint32_t end_addr);
page_t * paging_get_page(uint32_t address, int make, page_directory_t *dir);
#endif