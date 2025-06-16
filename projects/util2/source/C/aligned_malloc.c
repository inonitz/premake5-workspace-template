#include "util2/C/aligned_malloc.h"
#include "util2/C/macro.h"
#include <stdlib.h>


/*
    This article Helped: https://medium.com/howsofcoding/memory-management-aligned-malloc-and-free-9273336bd4c6
*/
void* util2_aligned_malloc(u64 bytes, u16 alignment)
{
    /* 
        Header(u16) - used to retrieve the offset for free(); 
        contained between the initial memory allocation address, 
        and the aligned memory address 
        More Precisely, given the aligned address 'aligned_addr',
        the value 'offset' is at address (aligned_addr - 2bytes)
    */
    u64 initial_addr, aligned_addr;
    const u32 hdr_size = sizeof(u16) + alignment - 1;


    initial_addr = __rcast(u64, malloc(bytes + hdr_size));
    const u16 offset = ( alignment - (initial_addr & ( alignment - 1)) );
    aligned_addr = (initial_addr + offset) * (initial_addr != 0); /* if(initial_addr == nulltr) => aligned_addr = nullptr */

    if(unlikely(initial_addr != 0)) /* nullptr allocation from malloc */
        *__rcast(u16*, aligned_addr - 2) = offset;

    return __rcast(void*, aligned_addr);
}


void util2_aligned_free(void* ptr)
{
    /* Self Explanatory */
    u64 offset_to_original_alloc = *(__rcast(u16*, ptr) - 1);
    
    /* Now we re-use 'offset_to_original_alloc' to get the original address in integer form */
    offset_to_original_alloc = __rcast(u64, ptr) - offset_to_original_alloc;
    /* Free the original address */
    free(__rcast(void*, offset_to_original_alloc));


    return;
}

