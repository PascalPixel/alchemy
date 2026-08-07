#include "types.h"

/*
 * resource_387 actor-9 tile/flag owner at 0x02000348, complete 136-byte span
 * through its three-word pool. The actor's fixed-point X tile chooses one of
 * two story flags and one of two adjacent six-argument window rectangles.
 */

extern void Func_080091c0(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);
extern void Func_080770c8(s32 flagId);
extern void Func_080770d0(s32 flagId);
extern u8 *Func_0808a080(s32 actor);

static s32 Fixed20_02000348(s32 value)
{
    if (value < 0) {
        value += 0x0fffff;
    }
    return value >> 20;
}

void Func_02000348(void)
{
    s32 tileX = Fixed20_02000348(*(s32 *)(Func_0808a080(9) + 8));

    Func_080770d0(0x861);
    Func_080770d0(0x862);

    if (tileX == 15) {
        Func_080091c0(47, 18, 1, 2, 16, 18);
    } else if (tileX == 16) {
        Func_080091c0(48, 18, 1, 2, 16, 18);
        Func_080770c8(0x861);
    } else {
        Func_080091c0(47, 18, 1, 2, 16, 18);
        Func_080770c8(0x862);
    }
}
