#include "types.h"








/*
 * resource_3bc owner at 0x02003cf4, 148 bytes: the resource_3bb clone owner
 * with this overlay's independently resolved callback pool word.
 */
extern u32 Func_02008568(void);
extern u32 Func_02008584(void);
extern u32 Func_0200858a(void);
extern void Func_020085a6();
extern u8 * Func_0200863a();
extern void Func_020086b6();
extern void Func_0200864e();
extern void Func_02008656();
void Func_02003cf4(u8 *object)
{
    s32 position[3];
    u32 random_x;
    u32 random_z;
    u8 *clone;

    if ((u32)(*(s32 *)(object + 40) + 255) <= 510)
        object[0x55] = 0;

    if (((Func_02008568() * 100) >> 16) > 9)
        return;

    position[0] = *(s32 *)(object + 8);
    position[1] = *(s32 *)(object + 12);
    position[2] = *(s32 *)(object + 16);
    random_x = Func_02008584();
    random_z = Func_0200858a();
    Func_020085a6(random_x << 4, random_z, position);

    clone = Func_0200863a(0x11d, position[0], position[1], position[2]);
    if (clone != 0) {
        clone[0x55] = 0;
        Func_020086b6(clone, 0);
        Func_0200864e(clone, (void *)0x0200ce50);
        Func_0200864e(clone, 1);
        Func_02008656(clone, 0);
    }
}
