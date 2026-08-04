#include "types.h"

extern u8 *Func_02003b62(s32, s32, s32, s32);
extern void Func_02003bec(s32, s32);
extern void Func_02003b82(s32, s32, s32);
extern void Func_02003c78(s32);
/* Two sites of one import, so two names. */
extern void Func_02003b36(u8 *, s32);
extern void Func_02003b5e(u8 *, s32);
extern void Func_02003bb8(s32, s32, s32);
extern void Func_02003bee(s32, s32, s32);

void Func_02000d90(s32 subject, s32 x, s32 z, s32 detail)
{
    u8 *record;

    /* r0-r3 are untouched by the prologue, so all four incoming arguments
     * reach this import unchanged. */
    record = Func_02003b62(subject, x, z, detail);

    Func_02003bec(subject, 1);

    /* 192 << 10 and 192 << 9, built with movs/lsls. */
    Func_02003b82(subject, 0x30000, 0x18000);

    Func_02003c78(152);

    *(s32 *)(record + 40) = detail;
    *(s32 *)(record + 72) = 0x8000;
    *(s32 *)(record + 68) = 0;

    Func_02003b36(record, 0);

    /* First with the whole-unit coordinates ... */
    Func_02003bb8(subject, x, z);
    /* ... then with the same pair promoted to 16.16 by `lsls #16`.  The high
     * registers holding x and z are overwritten with the shifted values, so
     * the shift is not undone afterwards. */
    Func_02003bee(subject, x << 16, z << 16);

    Func_02003b5e(record, 1);

    *(s32 *)(record + 72) = 0x10000;
}
