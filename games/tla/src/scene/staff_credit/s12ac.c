#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void                  WaitFrames(s32 frames);
s32                   Func_0200209c(void);
void                  Func_020020b4(s32 cue);
void                  Func_02001274(struct ObjectRuntime *object, s32 mode);
s32                   Func_02002004(s32 *position);
s32                   Func_020020a4(s32 mode, s32 value);

void Func_020012ac(void)
{
    struct ObjectRuntime *object = Object_GetById(Func_0200209c());
    s32                   i;

    Func_020020b4(212);
    Func_02001274(object, 1);
    for (i = 0; i < 20; i++)
    {
        s32 hit = Func_02002004(&object->x);

        if (hit != 0 && Func_020020a4(2, hit) != 0)
            break;
        WaitFrames(1);
    }
    Func_02001274(object, 2);
}
