/*
 * BYTE-EXACT and adopted 2026-08-07 with no compiler flags: the three callees
 * are declared at the in-overlay entry points the reference calls directly
 * rather than at their veneer addresses, and both read-modify-writes take
 * their mask from a local that is initialised first and ANDed with the loaded
 * byte afterwards, which is what puts the mask in r3 and the byte in r2.  The
 * +0x23 mask stays byte-wide (0xfe) rather than ~1, which would widen it to a
 * negated 2.
 */
#include "types.h"

#define OverlayObject_PrepareSpawnedObjectMode4 Func_02000a4c

u8 *Func_02003ac2(s32 kind, s32 x, s32 y, s32 z);
void Func_02003b34(u8 *obj, s32 mode);
void Func_02003bf4(u8 *obj, s32 mode);

u8 *OverlayObject_PrepareSpawnedObjectMode4(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Func_02003ac2(kind, x, y, z);

    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        u32 mask = ~12;
        u32 lo;
        mask = mask & rec[9];
        rec[9] = (u8)(mask | 4);
        obj[85] = 0;
        obj[89] = 8;
        Func_02003b34(obj, 0);
        Func_02003bf4(obj, 15);
        lo = 0xfe;
        lo = lo & obj[35];
        obj[35] = (u8)(lo | 2);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}
