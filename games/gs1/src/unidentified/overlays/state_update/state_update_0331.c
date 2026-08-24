#include "types.h"

u8 *Func_02002ff0();
s32 Func_02002f42();

/*
 * Complete 40-byte heading update: face the supplied entity towards entity 0,
 * store the resulting angle in its +6 halfword and report zero.
 */
s32 Func_02000aa8(u8 *entity)
{
    u8 *leader = Func_02002ff0(0);
    *(u16 *)(entity + 6) = (u16)Func_02002f42(
        *(s32 *)(leader + 16) - *(s32 *)(entity + 16),
        *(s32 *)(leader + 8) - *(s32 *)(entity + 8));
    return 0;
}
