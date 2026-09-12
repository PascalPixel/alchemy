#include "types.h"
#include "global_cells.h"

struct EffectObjectWork {
    u8 unknown_00[0x10];
    u8 *object;
};

void WaitFrames(s32);
void Func_08009240(void *, s32);

void EffectRuntime_StopCurrentObject(void)
{
    struct EffectObjectWork *work = *(void **)ADDR_03001F30;
    u8 *object = work->object;

    *(s32 *)(object + 0x6c) = 0;
    Func_08009240(object, 0);
    WaitFrames(1);
}
