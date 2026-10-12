#include "local.h"

void Func_02002550(void)
{
    struct Overlay67bWork *work = &Data_020034f0;
    void *buffer;

    Iwram_ClearWords(work, 0x70c);
    buffer = MemAlloc_(0x100);
    Func_020028b8(Data_02002c74, buffer);
    work->vram_entry = Resource_FindFreeEntry();
    /* FAKEMATCH: keeps the entry store before the size is built (differ 2 without) */
    __asm__("");
    VramBlock_LoadCached_(work->vram_entry, 0x100, buffer);
    MemFree(buffer);
    // 戻り値を持つ呼び出しとして書く: void の宣言だと引数の順が変わる (differ 2)
    ((s32 (*)(void *, s32))Scheduler_AddOrUpdateCallback)(Func_02002534, 0x480);
}
