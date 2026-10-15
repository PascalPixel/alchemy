#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void                  WaitFrames(s32 frames);
void                  Func_02003bb8(struct ObjectRuntime *object, s32 mode);
s32                   Func_02004b90(s32 *position);
s32                   Func_02004e10(void);
s32                   Func_02004e20(s32 a, s32 b);
void                  Func_02004e30(s32 value);

void Func_02003bf0(void)
{
    struct ObjectRuntime *object = Object_GetById(Func_02004e10());
    s32                   i;

    Func_02004e30(212);
    Func_02003bb8(object, 1);
    for (i = 0; i < 20; i++)
    {
        s32 hit = Func_02004b90(&object->x);

        if (hit != 0 && Func_02004e20(2, hit) != 0)
            break;
        WaitFrames(1);
    }
    Func_02003bb8(object, 2);
}
