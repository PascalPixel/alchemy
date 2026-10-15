/* Near miss, differ 2: the listing loads the table address before it scales the index (ldr r3; lsls r5, r0, #1);
 * a one-pass block after a table pointer changes the registers instead. */
#include "types.h"
#include "mem.h"
#include "resource.h"
#include "dma.h"
#include "io_reg.h"

struct MapPaletteEntry {
    u16 unk0;
    u16 resource;
    u8 unk4[8];
};

extern const struct MapPaletteEntry Data_0802f380[];
extern char ResourceId_Map000;

void Func_0802a650(s32 index, void *destination)
{
    const struct MapPaletteEntry *entry = &Data_0802f380[index];
    u16 *buffer = MemAlloc(0x200);
    s16 first = *(s16 *)0x05000000;

    Resource_DecodeType01(Resource_GetTableEntry(entry->resource + (s32)&ResourceId_Map000), buffer);
    buffer[0] = first;
    Dma_Set(buffer, destination, 0x84000070, REG_DMA3);
    MemFree(buffer);
}
