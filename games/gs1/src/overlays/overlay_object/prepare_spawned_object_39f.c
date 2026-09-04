/*
 * BYTE-EXACT and adopted 2026-08-07 with no compiler flags: same two facts as
 * the sibling owner in resource_3c4 -- the four callees are declared at the
 * in-overlay entry points the reference calls directly rather than at their
 * veneer addresses, and the +9 flag clear is written as a mask local that is
 * initialised first and ANDed with the loaded byte afterwards, which is what
 * puts the mask in r3 and the byte in r2.
 */
#include "types.h"

#define OverlayObject_PrepareSpawnedObject Func_020009f4

u8 *Func_0200373a(s32 kind, s32 x, s32 y, s32 z);
void Func_02003788(u8 *obj, s32 mode);
void Func_02003860(u8 *obj, s32 mode);
void Func_020037a0(u8 *obj, s32 mode);

u8 *OverlayObject_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Func_0200373a(kind, x, y, z);

    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        u32 mask = ~12;
        mask = mask & rec[9];
        rec[9] = (u8)mask;
        obj[85] = 0;
        obj[89] = 8;
        Func_02003788(obj, 0);
        Func_02003860(obj, 14);
        Func_020037a0(obj, 1);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}
