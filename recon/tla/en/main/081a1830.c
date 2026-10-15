/* Near miss: in the first two loops the native walks two pointers (the entry
 * in r6 and a copy in r2, both stepped at the end of each pass); this walks
 * one. Post-increment and increment-first forms give two pointers but step
 * the copy at the top (differ 70). */
#include "types.h"
#include "dma.h"
#include "mem.h"
#include "resource.h"
#include "callback_scheduler.h"

struct BucketEntry {
    void *next;
    u32 attributes;
    s32 tile;
};

extern struct BucketEntry *gData_0200752c;

void Func_081a1810(void);

void Func_081a1830(void)
{
    s32 slot = Resource_FindFreeEntry();
    struct BucketEntry *entry;
    s32 tile;
    u32 i;

    gData_0200752c = Runtime_BumpAllocateAlternatePool(0x120);
    tile = VramBlock_LoadCached(slot, 0x200, 0);
    Dma_Fill32(0x11111111, (void *)(0x06010000 + tile * 32), 0x200);
    entry = gData_0200752c;
    for (i = 0; i < 8; i++) {
        entry->next = 0;
        entry->attributes = (i << 21) | 0x80004078;
        entry->tile = tile;
        entry++;
    }
    for (i = 0; i < 8; i++) {
        entry->next = 0;
        entry->attributes = (i << 21) | 0x80004088;
        entry->tile = tile | 0x800;
        entry++;
    }
    for (i = 0; i < 8; i++) {
        entry->next = 0;
        entry->attributes = (i << 21) | 0x40004098;
        entry->tile = tile | 0x800;
        entry++;
    }
    Scheduler_AddOrUpdateCallback(Func_081a1810, 0x480);
}
