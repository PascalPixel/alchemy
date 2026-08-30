#include "types.h"

#define FieldScene_FinishSequence Func_02004128

extern void Func_02008584(s32 id, s32 arg1);
extern void Func_02008672(s32 arg0);
extern void Func_0200868e(void);

void FieldScene_FinishSequence(void)
{
    Func_02008584(0, 1);
    Func_02008672(2);
    Func_0200868e();
}
