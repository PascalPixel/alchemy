#include "types.h"

extern s32 Func_020043b8(void);
extern void Func_02004708(void);
extern void Func_02000b9e(void);

void Func_02000954(void)
{
    if (Func_020043b8() == 0) {
        Func_02004708();
    } else {
        Func_02000b9e();
    }
}
