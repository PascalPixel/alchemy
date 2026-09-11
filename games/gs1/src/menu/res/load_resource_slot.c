#include "types.h"
#include "scene.h"
#include "abi/menu/res/load_resource_slot.h"
#include "resource.h"

extern u8 gVal[];

u32 Runtime_BumpAllocate(s32 size);

void Resource_CopyData(s32, s32, void *);

void Menu_LoadResourceSlot(s32 slot, s32 index)
{
    s32 size = 1024;
    void *buffer = (void *)Runtime_BumpAllocate(size);
    u16 *base = GetResource((s32)gVal);

    /* 表内の相対位置から転送元を求める。 */
    Menu_Apply((void *)((u32)base + base[index]), buffer);
    Resource_CopyData(slot, size, buffer);
    Menu_Do(buffer);
}
