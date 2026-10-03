#include "TYPES.H"
#include "RESOURCE.H"

extern char ResourceId_CommandIcons;
u32 Runtime_BumpAllocate(s32 size);
void Resource_DecodeByteLzInRam(void *source, void *destination);
void VramBlock_LoadCached(s32, s32, void *);
void Sys_Free(void *);

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
