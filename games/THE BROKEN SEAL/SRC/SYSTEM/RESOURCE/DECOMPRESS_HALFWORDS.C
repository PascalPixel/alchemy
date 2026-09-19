#include "DMA.H"

/* Linker sizes determine the stack allocation for the copied ARM entries. */
extern u8 Resource_DecompressHalfwordsCodeSize[];

void Resource_DecompressHalfwords(const void *source, void *destination)
{
    u32 words = (u32)Resource_DecompressHalfwordsCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)0x8002298, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(const void *, void *))routine)(source, destination);
}
