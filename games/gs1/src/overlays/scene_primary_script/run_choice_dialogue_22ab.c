#include "types.h"

extern u8 Value_000022ab;
extern void Func_02004a06(s32);
extern s32 Func_02004a16(s32, s32);
extern s32 Func_02004976(s32, s32);
extern void Func_02004a20(s32);
extern void Func_02004a28(s32);
extern s32 Func_02004a40(s32, s32);

void SceneDialogue_RunChoiceSequence22ab(s32 no)
{
    s32 msg = (s32)&Value_000022ab;

    Func_02004a06(msg);
    Func_02004a16(no, 0);
    if (Func_02004976(0, 0) == 0)
        Func_02004a20(msg + 1);
    else
        Func_02004a28(msg + 2);
    Func_02004a40(no, 0);
}
