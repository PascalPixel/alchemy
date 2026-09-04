#include "types.h"

#define SceneState_SetWord1c0To209AndRun Func_020045cc

extern void Func_0200947a(void);
extern void Func_0200948e(void);
extern void Func_02009324(s32);

void SceneState_SetWord1c0To209AndRun(void)
{
    u8 *state;

    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1c0) = 0x209;
    Func_0200947a();
    Func_0200948e();
    Func_02009324(1);
}
