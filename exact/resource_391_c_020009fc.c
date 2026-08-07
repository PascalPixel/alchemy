/*
 * BYTE-EXACT and adopted 2026-08-07 with no compiler flags: the clamp store
 * had to take its zero from a local.  Written as a literal it was materialised
 * from the literal pool, which both changed the store's source register and
 * added a pool word; a `u16 z = 0' gives the reference's `movs r3, #0'.  The
 * owner ends at 0x02000a4a -- the veneer that follows is not its code.
 */
#include "types.h"
 
extern u32 Func_0200359e(void);
extern void Func_020036d2(u8 *object, s32 mode);
extern void Func_020036dc(u8 *object, s32 mode);
s32 Func_020009fc(u8 *object)
{
    u16 *counter = (u16 *)(object + 100);
    *counter = (u16)(*counter + ((Func_0200359e() * 100) >> 16));
    if ((s16)*counter > 1000) Func_020036d2(object, 7);
    else Func_020036dc(object, 10);
    if (*(s16 *)counter > 1200) { u16 z = 0; *counter = z; }
    return 1;
}
