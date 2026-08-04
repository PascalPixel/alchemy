#include "types.h"

extern void Func_02003e10(void);
extern void Func_02003e3e(s32, s32, s32);
extern void Func_02003e58(s32, s32, s32);
/* 0x02003e7a serves two imports: the three-argument setter and the
 * two-argument select that follows it. */
extern void Func_02003e7a(s32, s32, s32);
extern void Func_02003e7a_b(s32, s32);
extern void Func_02003e78(s32);
extern void Func_02003e88(s32, s32);
extern void Func_02003e54(void);

void Func_02001ad0(s32 x, s32 y)
{
    Func_02003e10();
    /* movs r1,#0xa0 / lsls r1,#10 and movs r2,#0xa0 / lsls r2,#9. */
    Func_02003e3e(0, (s32) 0x28000, (s32) 0x14000);
    Func_02003e58(0, x, y);
    Func_02003e7a(0, 4, 0);
    Func_02003e7a_b(0, 7);
    Func_02003e78(0);
    Func_02003e88(0, 6);
    Func_02003e54();
}
