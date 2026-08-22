/* Spawn and configure the mode-14 object variant. */
#include "types.h"

u8 *Func_02003a0e(s32 kind, s32 x, s32 y, s32 z);
void Func_02003a64(u8 *object, s32 mode);
void Func_02003afc(u8 *object, s32 mode);
void Func_02003a7c(u8 *object, s32 mode);

u8 *Func_020009f4(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *object = Func_02003a0e(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        s32 mask = ~12;
        mask &= owner[9];
        owner[9] = mask;
        object[85] = 0;
        object[89] = 8;
        Func_02003a64(object, 0);
        Func_02003afc(object, 14);
        Func_02003a7c(object, 1);
        return object;
    }
    return 0;
}
