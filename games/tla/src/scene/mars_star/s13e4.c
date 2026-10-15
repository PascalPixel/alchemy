#include "types.h"
#include "object_runtime.h"

struct RiseTimer {
    u8 unk00[0x68];
    s32 frames;
};

void Func_020041ec(struct ObjectRuntime *object);

void Func_020013e4(struct ObjectRuntime *object)
{
    struct RiseTimer *timer = (struct RiseTimer *)object;

    s32 growth;

    /* FAKEMATCH: a one-pass block and one growth value for both scales keep the y step ahead of the scale loads (alchemy permute). */
    do {
        object->y += 0x8000;
    } while (0);
    growth = 0x51e;
    object->scaleX += growth;
    object->scaleY += growth;
    timer->frames--;
    if (timer->frames == 1)
        Object_SetPositionAndResetMotion(object, 0, 0, 0);
    else if (timer->frames == 0)
        Func_020041ec(object);
}
