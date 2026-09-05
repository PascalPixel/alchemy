#include "types.h"

extern void Func_02004fb4(void);
extern void Func_02004fce(void);
extern void Func_0200507a(s32 dialogueId);
extern s32 Func_020050a2(s32 slot, s32 arg1);

void FieldScene_ShowDialogue182D(void)
{
    Func_02004fb4();
    Func_0200507a(0x182d);
    Func_020050a2(15, 0);
    Func_02004fce();
}
