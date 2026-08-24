/*
 * BYTE-EXACT and adopted 2026-08-07 with no compiler flags: the four callees
 * were declared at their ROM-space veneer addresses, so every `bl' went
 * through a veneer; they are now declared at the in-overlay entry points the
 * reference calls directly.  The +9 flag clear then had to be written as a
 * mask local that is initialised and only afterwards ANDed with the loaded
 * byte -- writing it as one expression puts the mask in r2 and the byte in
 * r3, the reverse of the reference.
 */
#include "types.h"

u8 *Func_02003a6a(s32 kind, s32 x, s32 y, s32 z);
void Func_02003ad8(u8 *object, s32 mode);
void Func_02003b98(u8 *object, s32 mode);
void Func_02003af0(u8 *object, s32 mode);

u8 *Func_020009f4(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *result;
    u8 *object = Func_02003a6a(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        u32 masked = ~12;
        masked = masked & owner[9];
        owner[9] = (u8)masked;
        object[85] = 0;
        object[89] = 8;
        Func_02003ad8(object, 0);
        Func_02003b98(object, 14);
        Func_02003af0(object, 1);
        result = object;
    } else {
        result = 0;
    }
    return result;
}
