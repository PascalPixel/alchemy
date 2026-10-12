#include "types.h"
#include "frame.h"
#include "object_dispatch.h"
#include "ram_buffer.h"

struct EffectSceneWork {
    u8 reserved_00[16];
    struct DispatchObject *main_object;
};

void BattleFx_SetObjectTickMode(struct DispatchObject *object, s32 mode);

void EffectRuntime_StopCurrentObject(void)
{
    struct EffectSceneWork *work = Ram_HeapSlots->effect_work;

    BattleFx_SetObjectTickMode(work->main_object, 0);
    WaitFrames(1);
}
