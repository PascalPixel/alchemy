#include "DMA.H"

struct RuntimeCells {
    u8 unknown_000[196];
    void (*decode)(s32, s32, void *, void *);
};

extern struct RuntimeCells gWorkSlot;
extern u8 Value_0000027c[];
extern u8 Data_0201c000[];

void *Runtime_AllocateHeapBlock(s32 kind, s32 size);
void Runtime_ReleaseHeapBlock(s32 kind);

/* Copies the ARM decoder at 0x08009e7c (Value_0000027c bytes) to a heap
   block and runs the decoder installed at 0x03001f14 on a and b. */
void Resource_RunCopiedDecoder(s32 a, s32 b)
{
    u8 *base;
    u32 size;
    void *code;

    base = Data_0201c000;
    /* FAKEMATCH: the do-whiles order the size load and the call. */
    do { size = (u32)Value_0000027c; } while (0);
    code = Runtime_AllocateHeapBlock(49, size);
    Dma_Set((const void *)0x08009e7c, code, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    do { gWorkSlot.decode(a, b, (void *)0x0203c000, base + 0x1000); } while (0);
    Runtime_ReleaseHeapBlock(49);
}
