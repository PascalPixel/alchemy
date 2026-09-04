#include "types.h"

#define SceneDialogue_AskMessage2557 Func_02001320

extern void Func_02002eca(s32);
extern s32 Func_02002eda(s32, s32);
extern s32 Func_02002e6a(s32, s32);

s32 SceneDialogue_AskMessage2557(void)
{
    Func_02002eca(0x2557);
    Func_02002eda(12, 0);
    return Func_02002e6a(0, 0) == 0;
}
