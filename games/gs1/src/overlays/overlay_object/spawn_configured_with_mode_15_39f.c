/*
 * Spawn an object, clear the low bits of its owner record, and leave it in
 * mode 15.  The three callees are declared at their in-overlay entry points,
 * not at veneers.  Each read-modify-write takes its mask from a local that is
 * initialised first and ANDed with the loaded byte afterwards, which is what
 * puts the mask in r3 and the byte in r2.  The +0x23 mask stays byte-wide as
 * 0xfe rather than ~1, which would widen it to a negated 2.
 */
#include "types.h"

u8 *Func_02003792(s32 kind, s32 x, s32 y, s32 z);
void Func_020037e4(u8 *obj, s32 mode);
void Func_020038bc(u8 *obj, s32 mode);

u8 *OverlayObject_SpawnConfiguredWithMode15(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Func_02003792(kind, x, y, z);

    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        u32 mask = ~12;
        u32 m;
        mask = mask & rec[9];
        rec[9] = (u8)(mask | 4);
        obj[85] = 0;
        obj[89] = 8;
        Func_020037e4(obj, 0);
        Func_020038bc(obj, 15);
        m = 0xfe;
        m = m & obj[35];
        obj[35] = (u8)(m | 2);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}
