#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "RESOURCE.H"
#include "RESMENU.H"
#include "RAM_BUFFER.H"

extern char ResourceId_CommandIcons;
void Resource_DecodeByteLzInRam(void *source, void *destination);
void Sys_Free(void *);
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);

void Menu_LoadResourceSlot(s32 slot, s32 index)
{
    s32 size = 1024;
    void *buffer = (void *)Runtime_BumpAllocate(size);
    u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

    /* 表内の相対位置から転送元を求める。 */
    Resource_DecodeByteLzInRam((void *)((u32)base + base[index]), buffer);
    VramBlock_LoadCached(slot, size, buffer);
    Sys_Free(buffer);
}

/* ☀️'s, reaching the selection work through ⚓️'s heap slot. */
void Menu_AppendResourceEntry(s32 no)
{
    struct ResourceMenuWork *work;
    struct ResourceMenuEntry *entry;
    s16 index;
    s32 slot;
    s32 flags;

    work = Ram_HeapSlots->menu_select_work;
    /* Without the barrier the copy of no into r10 waits until after the
       count is read; ⚓️ copies it straight after loading the work. */
    /* FAKEMATCH: copy the argument to r10 right after loading the work. */
    asm("");
    index = work->count;
    if (index <= 5) {
        work->count = (u16)work->count + 1;
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

/* Lays the menu's entries out three tiles apart on the given tile row,
   centred with the window for their text, and opens that window. */
