#include "types.h"

extern u8 Value_0000217f;

extern void Func_02007170(void);
extern void Func_02007228(s32 arg0);
extern s32 Func_02007238(s32 arg0, s32 arg1);
extern s32 Func_02007198(s32 arg0, s32 arg1);
extern void Func_0200718a(s32 arg0);
extern void Func_02007248(s32 arg0);
extern void Func_02007260(s32 arg0, s32 arg1);
extern void Func_020071a0(s32 arg0);
extern void Func_0200725e(s32 arg0);
extern void Func_02007276(s32 arg0, s32 arg1);
extern void Func_020071c2(void);

void Func_020034bc(void)
{
    s32 resource;

    Func_02007170();
    resource = (s32)&Value_0000217f;
    Func_02007228(resource);
    Func_02007238(8, 0);
    if (Func_02007198(0, 0) == 0) {
        Func_0200718a(20);
        Func_02007248(resource + 1);
        Func_02007260(8, 0);
    } else {
        Func_020071a0(20);
        Func_0200725e(resource + 2);
        Func_02007276(8, 0);
    }
    Func_020071c2();
}
