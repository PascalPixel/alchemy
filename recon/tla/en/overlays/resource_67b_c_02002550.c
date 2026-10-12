// differ 4: the pool loads of IwramClearWords and of the callback each sit
// one instruction away from where the listing has them among the split
// constants 0x70c and 0x480. The Scheduler_AddOrUpdateCallback_ inline does
// not change it.
#include "../../../../games/tla/src/field/overlay_67b/local.h"

void Func_02002550(void)
{
    struct Overlay67bWork *work = &Data_020034f0;
    void *buffer;

    Iwram_ClearWords(work, 0x70c);
    buffer = MemAlloc_(0x100);
    Func_020028b8(Data_02002c74, buffer);
    work->vram_entry = Resource_FindFreeEntry();
    VramBlock_LoadCached_(work->vram_entry, 0x100, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02002534, 0x480);
}
