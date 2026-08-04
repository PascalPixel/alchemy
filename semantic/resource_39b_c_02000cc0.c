#include "types.h"

extern s32 Func_080000f8(void);
extern void Func_0200013c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                          s32 velocityZ, s32 *descriptor);

/* Emit one alternating spark above the supplied actor. */
s32 Func_02000cc0(u8 *actor)
{
    volatile s32 *frame = (volatile s32 *)0x03001e40;
    s32 descriptor[4];
    s32 random = Func_080000f8();
    s32 speed;

    descriptor[0] = 0;
    descriptor[1] = ((*frame & 1) != 0) ? 7 : 5;
    descriptor[2] = 0xcccc;
    descriptor[3] = 0xcccc;
    speed = -((random >> 13) * 3 * 17 * 257);
    Func_0200013c(*(s32 *)(actor + 8) + ((8 - (*frame & 15)) << 16),
                   *(s32 *)(actor + 12) + 0x001a0000,
                   *(s32 *)(actor + 16), speed, 0, 0x000b0000, descriptor);
    return 0;
}
