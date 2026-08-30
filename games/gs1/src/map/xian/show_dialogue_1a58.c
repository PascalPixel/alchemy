#include "types.h"

#define FieldScene_ShowDialogue1A58 Func_02002464

extern void Func_02006844(void);
extern void Func_0200685e(void);
extern void Func_0200690a(s32 dialogue_id);
extern s32 Func_02006932(s32 id, s32 arg1);

void FieldScene_ShowDialogue1A58(void)
{
    Func_02006844();
    Func_0200690a(0x1a58);
    Func_02006932(11, 0);
    Func_0200685e();
}
