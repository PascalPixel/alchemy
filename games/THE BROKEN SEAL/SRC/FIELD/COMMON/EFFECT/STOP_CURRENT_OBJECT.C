#include "TYPES.H"
#include "GLOBAL_CELLS.H"

struct EffectObjectWork {
    u8 unknown_00[0x10];
    u8 *object;
};

void WaitFrames(s32);
void Animation_ApplyChildValuesFar(void *, s32);

void EffectRuntime_StopCurrentObject(void)
{
    struct EffectObjectWork *work = *(void **)ADDR_03001F30;
    u8 *object = work->object;

    *(s32 *)(object + 0x6c) = 0;
    Animation_ApplyChildValuesFar(object, 0);
    WaitFrames(1);
}
