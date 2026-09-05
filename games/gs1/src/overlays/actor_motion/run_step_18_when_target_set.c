#include "types.h"

#define SceneActor_RunStep18WhenTargetSet Func_0200572c

extern s32 Func_0200b718(s32);
extern void Func_0200ad9e(s32, s32, s32, s32);

s32 SceneActor_RunStep18WhenTargetSet(s32 *p)
{
    s32 t = Func_0200b718(0);
    if (p[14] == (s32)0x80000000 && p[16] == (s32)0x80000000)
        return 0;
    Func_0200ad9e((s32)p, t, 18, 0);
    return 0;
}
