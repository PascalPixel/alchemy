/*
 * Spawn an object, clear the low bits of its owner record, and issue command
 * 15.  The three callees are declared at their in-overlay entry points, not at
 * veneers.  Each read-modify-write takes its mask from a local that is
 * initialised first and ANDed with the loaded byte afterwards, which is what
 * puts the mask in r3 and the byte in r2.  The +0x23 mask stays byte-wide as
 * 0xfe rather than ~1, which would widen it to a negated 2.
 */
#include "types.h"

u8 *Func_02003806(s32 kind, s32 x, s32 y, s32 z);
void Func_02003878(u8 *object, s32 mode);
void Func_02003960(u8 *object, s32 mode);

u8 *OverlayObject_PrepareObjectWithCommand15(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *result;
    u8 *object = Func_02003806(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        u32 mask = ~12;
        u32 low;
        mask = mask & owner[9];
        owner[9] = (u8)(mask | 4);
        object[85] = 0;
        object[89] = 8;
        Func_02003878(object, 0);
        Func_02003960(object, 15);
        low = 0xfe;
        low = low & object[35];
        object[35] = (u8)(low | 2);
        result = object;
    } else {
        result = 0;
    }
    return result;
}
