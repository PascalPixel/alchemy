#include "types.h"

/*
 * Point actors 24 and 25 toward actor 0. The saved-register owner begins at
 * 0x02002ba0 and reaches its only return at 0x02002bee; the next prologue is
 * immediate at 0x02002bf0, so the complete owner is 80 bytes with no pool.
 */

extern u8 *Func_0808a080(s32 id);
extern s32 Func_08000100(s32 z_delta, s32 x_delta);

static s32 subtract_wrapped(s32 left, s32 right)
{
    return (s32)((u32)left - (u32)right);
}

void Func_02002ba0(void)
{
    u8 *actor0 = Func_0808a080(0);
    u8 *actor24 = Func_0808a080(24);
    u8 *actor25 = Func_0808a080(25);

    *(u16 *)(actor24 + 6) = (u16)Func_08000100(
        subtract_wrapped(*(s32 *)(actor0 + 16), *(s32 *)(actor24 + 16)),
        subtract_wrapped(*(s32 *)(actor0 + 8), *(s32 *)(actor24 + 8)));
    *(u16 *)(actor25 + 6) = (u16)Func_08000100(
        subtract_wrapped(*(s32 *)(actor0 + 16), *(s32 *)(actor25 + 16)),
        subtract_wrapped(*(s32 *)(actor0 + 8), *(s32 *)(actor25 + 8)));
}
