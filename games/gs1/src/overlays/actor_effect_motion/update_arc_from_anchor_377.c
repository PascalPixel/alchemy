#include "types.h"

/* The anchor pointer is read before the frame counter is stored: the reference
 * hoists `ldr r6,[r5,#104]` above the `strh`, and only that source order
 * reproduces it. */
struct Actor_02004004 {
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
    struct Actor_02004004 *anchor;  /* 0x68 */
};

extern void Func_02002eb8(struct Actor_02004004 *);
extern s32 Func_02002e80(s32);

void OverlayObject_UpdateArcFromAnchor(struct Actor_02004004 *obj)
{
    struct Actor_02004004 *anchor;
    s32 frame;
    s32 amp;

    anchor = obj->anchor;
    obj->frame = (u16)(obj->frame + 1);
    frame = (s16)obj->frame;

    if (frame > 31) {
        Func_02002eb8(obj);
        return;
    }

    amp = Func_02002e80(frame << 10);
    obj->amplitude_x = amp;
    obj->amplitude_y = -amp;
    obj->x = anchor->x;
    obj->y += 0x10000;
    obj->z = anchor->z - (0x10000 - amp) * 5 + 0x100000;
}
