#include "types.h"

extern void Func_02006130(void);
extern void Func_02006148();
extern s32 Func_0200615e(s32 arg0, s32 arg1);
extern void Func_0200616c(void);
extern void Func_0200618c(void);
extern void Func_020061f6(s32 dialogueId);

extern s32 Func_02006206(s32 slot, s32 arg1);
extern void Func_02006232(s32 slot, s32 arg1, s32 arg2);
extern void Func_02006258(s32 slot, s32 arg1, s32 arg2);

extern u8 *Data_03001ebc;

void FieldScene_RunSkippableStoryBeat(void)
{
    u8 *workspace;

    Func_02006130();
    Func_020061f6(0x186e);
    Func_02006206(18, 0);

    if (Func_0200615e(0, 0) == 0) {
        Func_02006148(20);
        Func_02006232(18, 0, 20);
        Func_02006148(0x898);
        Func_0200616c();
    } else {
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) += 1;
        Func_02006258(18, 0, 20);
        Func_0200618c();
    }
}
