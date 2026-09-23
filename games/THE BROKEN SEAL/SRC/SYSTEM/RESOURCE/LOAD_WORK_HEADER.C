#include "DMA.H"

u32 Resource_GetTableEntry(u32 index);
extern u8 Value_00000002;

struct ResourceWork {
    u32 header[3];
    u32 cursor;
};

void Resource_LoadWorkHeader(void)
{
    const void *header;
    struct ResourceWork *work;

    header = (const void *)Resource_GetTableEntry((u32)&Value_00000002);
    work = (struct ResourceWork *)0x03007804;
    Dma_Set(header, work, 0x84000003, (volatile u32 *)0x040000d4);
    work->cursor = 0;
}
