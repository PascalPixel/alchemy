#include "types.h"

#define SceneDialogue_ShowMessage22a8Branch Func_02000524
#define SceneDialogue_ShowMessage22a3Branch Func_02003fc4
extern u8 Value_000022a8;
extern u8 Value_000022a3;

void Func_020049c6(s32);
s32 Func_020049d6(s32, s32);
s32 Func_02004936(s32, s32);
void Func_020049e0(s32);
void Func_020049e8(s32);
s32 Func_02004a00(s32, s32);
void Func_02008466(s32);
s32 Func_02008476(s32, s32);
s32 Func_020083d6(s32, s32);
void Func_02008480(s32);
void Func_02008498(s32, s32);
void Func_02008490(s32);
void Func_020084a8(s32, s32);

void SceneDialogue_ShowMessage22a8Branch(s32 a)
{
    s32 k = (s32)&Value_000022a8;

    Func_020049c6(k);
    Func_020049d6(a, 0);
    if (Func_02004936(0, 0) == 0)
        Func_020049e0(k + 1);
    else
        Func_020049e8(k + 2);
    Func_02004a00(a, 0);
}

void SceneDialogue_ShowMessage22a3Branch(s32 a)
{
    s32 k = (s32)&Value_000022a3;

    Func_02008466(k);
    Func_02008476(a, 0);
    if (Func_020083d6(0, 0) == 0) {
        Func_02008480(k + 1);
        Func_02008498(a, 0);
    } else {
        Func_02008490(k + 2);
        Func_020084a8(a, 0);
    }
}
