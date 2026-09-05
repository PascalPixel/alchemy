#include "types.h"

extern void Func_02005030(void);
extern void Func_02004fe8(s32, s32);
extern void Func_02004ff0(s32, s32);
extern void Func_0200504c(void);

void SceneState_SetFlags929And949(void)
{
    Func_02005030();
    Func_02004fe8(0x929, 1);
    Func_02004ff0(0x949, 1);
    Func_0200504c();
}
