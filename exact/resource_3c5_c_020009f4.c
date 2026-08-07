/*
 * BYTE-EXACT and adopted 2026-08-07 with no compiler flags: same two facts as
 * the sibling owner in resource_3c4 -- the four callees are declared at the
 * in-overlay entry points the reference calls directly rather than at their
 * veneer addresses, and the +9 flag clear is written as a mask local that is
 * initialised first and ANDed with the loaded byte afterwards, which is what
 * puts the mask in r3 and the byte in r2.
 */
#include "types.h"

u8 *Func_020037ae(s32 kind, s32 x, s32 y, s32 z);
void Func_0200381c(u8 *object, s32 mode);
void Func_02003904(u8 *object, s32 mode);
void Func_02003834(u8 *object, s32 mode);

u8 *Func_020009f4(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *result;
    u8 *object = Func_020037ae(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        u32 masked = ~12;
        masked = masked & owner[9];
        owner[9] = (u8)masked;
        object[85] = 0;
        object[89] = 8;
        Func_0200381c(object, 0);
        Func_02003904(object, 14);
        Func_02003834(object, 1);
        result = object;
    } else {
        result = 0;
    }
    return result;
}
