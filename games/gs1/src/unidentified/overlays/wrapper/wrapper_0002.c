#include "types.h"

extern void Func_02007eb8(void);
extern u8 *Func_02007ca6(s32);
extern void Func_02007ce6(s32, s32, s32, s32);
extern void Func_02007ee4(s32);
extern void Func_02007cd2(s32);
extern s32 Func_02007cd8(s32);

s32 Func_02002f98(void)
{
    u8 *o;

    Func_02007eb8();
    o = Func_02007ca6(2);
    o += 0xf8;
    if (*(s32 *)o & 1) {
        Func_02007ce6(2, 0, 0, 0);
        Func_02007ee4(126);
        Func_02007cd2(0);
        return Func_02007cd8(2);
    }
}
