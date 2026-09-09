#include "types.h"

/*
 * resource_3bb: six steps, each calling its own near-identical handler stub
 * with a fixed id -- 896 through 936, eight apart -- and a second argument of
 * zero.  The owner at 0x02001fb8 is 66 bytes: its code plus one trailing
 * alignment halfword.  It reads no incoming argument, so it takes none.  The
 * six callees are identified by call shape only.
 */

extern void Func_02005eca(s32 arg0, s32 arg1);
extern void Func_02005ed4(s32 arg0, s32 arg1);
extern void Func_02005ede(s32 arg0, s32 arg1);
extern void Func_02005ee8(s32 arg0, s32 arg1);
extern void Func_02005ef2(s32 arg0, s32 arg1);
extern void Func_02005efc(s32 arg0, s32 arg1);

void FieldScene_RunSixSteps896To936(void)
{
    Func_02005eca(896, 0);
    Func_02005ed4(904, 0);
    Func_02005ede(912, 0);
    Func_02005ee8(920, 0);
    Func_02005ef2(928, 0);
    Func_02005efc(936, 0);
}
