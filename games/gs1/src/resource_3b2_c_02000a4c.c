/* Spawn and configure the mode-15 object variant. */
#include "types.h"

u8 *Func_02003a66(s32 kind, s32 x, s32 y, s32 z);
void Func_02003ac0(u8 *object, s32 mode);
void Func_02003b58(u8 *object, s32 mode);

u8 *Func_02000a4c(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *object = Func_02003a66(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        s32 mask = ~12;
        mask &= owner[9];
        mask |= 4;
        owner[9] = mask;
        object[85] = 0;
        object[89] = 8;
        Func_02003ac0(object, 0);
        Func_02003b58(object, 15);
        {
            s32 mask2 = 254;
            mask2 &= object[35];
            mask2 |= 2;
            object[35] = mask2;
        }
        return object;
    }
    return 0;
}
