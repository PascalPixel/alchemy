#include "PROTO.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "RESOURCE.H"
#include "RESMENU.H"
#include "RAM_BUFFER.H"

#define Resource_DecodeByteLzInRam ((void (*)(void*,void*))Resource_DecodeByteLzInRam)

extern char ResourceId_CommandIcons;
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);

void Menu_LoadResourceSlot(s32 slot, s32 index)
{
    s32 size = 1024;
    void *buffer = (void *)Runtime_BumpAllocate(size);
    u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

    Resource_DecodeByteLzInRam((void *)((u32)base + base[index]), buffer);
    VramBlock_LoadCached(slot, size, buffer);
    Sys_Free(buffer);
}

void Menu_AppendResourceEntry(s32 no)
{
    struct ResourceMenuWork *work;
    struct ResourceMenuEntry *entry;
    s16 index;
    s32 slot;
    s32 flags;

    work = Ram_HeapSlots->menu_select_work;
    index = work->count;
    if (index <= 5) {
        work->count++;
        entry = &work->entries[index];
        slot = Resource_FindFreeEntry();
        Menu_LoadResourceSlot(slot, no);
        entry->x = index * 24 + 32;
        flags = 136;
        entry->y = flags;
        entry->slot = slot;
        work->resource_ids[index] = no;
    }
}

