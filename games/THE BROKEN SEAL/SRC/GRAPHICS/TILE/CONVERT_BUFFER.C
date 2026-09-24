#include "TYPES.H"
#include "DMA.H"

typedef void (*WordCopyFn)(void *destination, const void *source, s32 size);
typedef void (*ConvertFn)(void *dst, const void *src, const void *saved);

void *Runtime_BumpAllocateAlternatePool(s32 size);
void *Runtime_BumpAllocate(u32 size);
void Runtime_BumpFree(void *allocation);

extern u8 Value_0000009c;

static __inline__ void CopyWords(void *dst, const void *src, s32 size)
{
    ((WordCopyFn)0x03001388)(dst, src, size);
}

/* Converts the map at 0x02010000 into 0x02018000 with the ARM routine at
   0x0800a37c, run from a heap copy, against a saved copy of the source. */
void Tilemap_ConvertBuffer(void)
{
    u8 *saved;
    ConvertFn routine;
    u32 size;

    size = 0x8000;
    /* FAKEMATCH: the two do-while blocks keep each allocation next to the
       transfer that uses it */
    do {
        saved = (u8 *)Runtime_BumpAllocateAlternatePool(size);
        CopyWords(saved, (const void *)0x02010000, size);
    } while (0);
    {
        u32 code_size = (u32)&Value_0000009c;

        do {
            routine = (ConvertFn)Runtime_BumpAllocate(code_size);
            Dma_Set((void *)0x0800a37c, routine, 0x84000000 | (code_size >> 2),
                    (volatile u32 *)0x040000d4);
        } while (0);
    }
    routine((void *)0x02018000, (const void *)0x02010000, saved);
    Runtime_BumpFree(routine);
    Runtime_BumpFree(saved);
}
