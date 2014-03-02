#ifndef PAGE_ALLOCATOR_H
#define PAGE_ALLOCATOR_H
void init_page_allocator();
void pa_set_frame(uint32_t address);
void pa_clear_frame(uint32_t address);
uint32_t pa_test_frame(uint32_t address);
void pa_alloc_frame(page_t *page, int kernel, int rw);
void pa_free_frame(page_t *page);
#endif