#include "types.h"

#define FieldScene_ShowDialogue17DF Func_0200254c

extern void Func_0200692c(void);
extern void Func_0200694e(void);
extern void Func_020069cc(s32 id, s32 arg1);
extern void Func_020069fa(s32 dialogue_id);
extern void Func_02006a12(s32 id, s32 arg1);

void FieldScene_ShowDialogue17DF(void)
{
    Func_0200692c();
    Func_020069cc(8, 2);
    Func_020069fa(0x17df);
    Func_02006a12(8, 0);
    Func_0200694e();
}
