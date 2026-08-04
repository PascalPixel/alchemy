#include "types.h"

/* Per-site raw names: the first resolves to Func_0808a080 and the four
 * renderer sites resolve to Func_080091c0. */
extern u8 *Func_02002078();
extern void Func_02002040(s32, s32, s32, s32, s32, s32);
extern void Func_02002056(s32, s32, s32, s32, s32, s32);
extern void Func_02002070(s32, s32, s32, s32, s32, s32);
extern void Func_02002086(s32, s32, s32, s32, s32, s32);

void Func_02000fa0(void)
{
    u8 *actor;
    actor = Func_02002078(8);
    if (actor == 0)
        return;

    {
        s32 tile_z = *(s32 *)(actor + 0x10) >> 20;
        if (tile_z == 6)
            Func_02002040(2, 0, 1, 1, 14, 6);
        else {
            s32 x = 14;
            s32 z = 6;
            Func_02002056(0, 0, 1, 1, x, z);
        }
    }

    {
        s32 tile_z = *(s32 *)(actor + 0x10) >> 20;
        if (tile_z == 9)
            Func_02002070(2, 0, 1, 1, 14, 9);
        else {
            s32 x = 14;
            s32 z = 9;
            Func_02002086(1, 0, 1, 1, x, z);
        }
    }
}
