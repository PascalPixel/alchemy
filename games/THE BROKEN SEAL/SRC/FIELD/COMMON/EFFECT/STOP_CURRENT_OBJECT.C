#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
extern u8 Data_03001f30[];

struct EffectObjectWork {
    u8 unknown_00[0x10];
    u8 *object;
};

void Animation_ApplyChildValuesFar(void *, s32);

void EffectRuntime_StopCurrentObject(void)
{
    struct EffectObjectWork *work = *(void **)((u32)&Data_03001f30);
    u8 *object = work->object;

    *(s32 *)(object + 0x6c) = 0;
    Animation_ApplyChildValuesFar(object, 0);
    WaitFrames(1);
}
