#include "types.h"

/*
 * resource_387 actor-9 tile/flag owner at 0x02000348, complete 136-byte span
 * through its three-word pool. The actor's fixed-point X tile chooses one of
 * two story flags and one of two adjacent six-argument window rectangles.
 */






static s32 Fixed20_02000348(s32 value)
{
    if (value < 0) {
        value += 0x0fffff;
    }
    return value >> 20;
}

extern u8 * Func_02001494(s32 actor);
extern void Func_02001476(s32 flagId);
extern void Func_0200147c(s32 flagId);
extern void Func_0200143c(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);
extern void Func_02001454(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);
extern void Func_020014aa(s32 flagId);
extern void Func_02001470(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);
extern void Func_020014c6(s32 flagId);
void Func_02000348(void)
{
    s32 tileX = Fixed20_02000348(*(s32 *)(Func_02001494(9) + 8));

    Func_02001476(0x861);
    Func_0200147c(0x862);

    if (tileX == 15) {
        Func_0200143c(47, 18, 1, 2, 16, 18);
    } else if (tileX == 16) {
        Func_02001454(48, 18, 1, 2, 16, 18);
        Func_020014aa(0x861);
    } else {
        Func_02001470(47, 18, 1, 2, 16, 18);
        Func_020014c6(0x862);
    }
}
