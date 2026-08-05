/* Spawn and configure the mode-15 object variant. */
#include "types.h"

u8 *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
void Func_080091e0(u8 *object, s32 mode);
void Func_0808a160(u8 *object, s32 mode);

u8 *Func_02000a4c(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *object = Func_080090c8(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        object[85] = 0;
        owner[9] = (owner[9] & ~12) | 4;
        object[89] = 8;
        Func_080091e0(object, 0);
        Func_0808a160(object, 15);
        object[35] = (object[35] & ~1) | 2;
    }
    return object;
}
