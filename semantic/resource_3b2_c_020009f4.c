/* Spawn and configure the mode-14 object variant. */
#include "types.h"

u8 *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
void Func_080091e0(u8 *object, s32 mode);
void Func_0808a160(u8 *object, s32 mode);
void Func_080091e8(u8 *object, s32 mode);

u8 *Func_020009f4(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *object = Func_080090c8(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        object[85] = 0;
        owner[9] &= ~12;
        object[89] = 8;
        Func_080091e0(object, 0);
        Func_0808a160(object, 14);
        Func_080091e8(object, 1);
    }
    return object;
}
