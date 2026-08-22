#include "types.h"

/* resource_381 owner at 0x02002e5c (82 bytes), the twin of resource_371:4004.
 * Ported from that byte-exact owner: same field layout, same two calls. */

struct Actor_02002e5c {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct Actor_02002e5c *anchor;  /* 0x68 */
};

void Func_020062d4();
s32 Func_02006284();

void Func_02002e5c(struct Actor_02002e5c *self)
{
    struct Actor_02002e5c *anchor;
    s32 frame;
    s32 amplitude;

    anchor = self->anchor;
    self->frame = (u16)(self->frame + 1);
    frame = (s16)self->frame;

    if (frame > 31) {
        Func_020062d4(self);
        return;
    }

    amplitude = Func_02006284(frame << 10);
    self->amplitude_x = amplitude;
    self->amplitude_y = -amplitude;
    self->x = anchor->x;
    self->y += 0x10000;
    self->z = anchor->z - (0x10000 - amplitude) * 5 + 0x100000;
}
