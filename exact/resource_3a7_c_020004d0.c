#include "types.h"

extern s32 Func_020015d6(s32);
extern s32 Func_02000936(void);
extern s32 Func_020015e8(s32);
extern void Func_02001642(s32);
extern void Func_02000906(void);
extern void Func_02001604(s32);

void Func_020004d0(void)
{
    if (Func_020015d6(0x9AA) != 0) {
        return;
    }
    if (Func_02000936() == 0) {
        return;
    }
    if (Func_020015e8(0x207) != 0) {
        return;
    }
    Func_02001642(0x50);
    Func_02000906();
    Func_02001604(0x9AA);
}
