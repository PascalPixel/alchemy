#include "types.h"

extern void Func_02007242(void);
extern s32 Func_02004612(s32, s32, s32, s32);
extern void Func_02007246(s32);
extern void Func_020071ea(s32, s32, s32, s32, s32, s32);
extern void Func_02007276(void);
void SceneState_SetFlag204AndConfigureRegion49_46(void)
{
    s32 p5, p6;
    Func_02007242();
    Func_02004612(0, 13, 10, 1);
    Func_02007246(0x204);
    p5 = 20;
    p6 = 50;
    Func_020071ea(49, 46, 8, 4, p5, p6);
    Func_02007276();
}
