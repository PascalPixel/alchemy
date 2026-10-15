#include "types.h"
#include "call.h"

void Func_02000e44(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02000148(void)
{
    Call6((void (*)())Func_02000e44, 7, 90, 1, 1, 8, 91);
}
