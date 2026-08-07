#include "types.h"

/*
 * resource_373 owner at 0x020008c0: the "spawn the two effect layers over the
 * current stage's bounding box" helper.  Looks up the running stage id in a
 * seven-entry table, gives up when it is not one of the six known ids, then
 * derives a centre and a half-extent from the matching 16-byte box record and
 * hands them to three presentation calls.
 *
 * This owner is duplicated (same shape, different literals) across twelve
 * overlays: 373, 3c4, 389, 39f, 3c5, 3bf, 391, 3b2, 3b4, 392, 3bd, 393.
 */

extern s32 *Func_020068b6(void);
extern void Func_020068d4(s32 x, s32 z, s32 ex, s32 ez, s32 cx, s32 cz);
extern void Func_02000bf0(s32 kind, s32 x, s32 z, s32 ex, s32 ez, s32 alpha);
extern void Func_02000c02(s32 kind, s32 x, s32 z, s32 ex, s32 ez, s32 alpha);

struct Box {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
};

struct Work {
    s32 slot;
    s32 unused;
    s32 x;
    s32 y;
    s32 z;
    s32 w;
};

s32 Func_020008c0(void)
{
    s32 *camera = *(s32 **)0x03001e70;
    const s32 *ids = (const s32 *)0x0200e1d0;
    const struct Box *boxes = (const struct Box *)0x0200e1e8;
    struct Work work;
    struct Work *w = &work;
    s32 *obj;
    s32 i;
    s32 slot;
    s32 ex, ez, t;

    obj = Func_020068b6();
    i = 0;
    if (*(s16 *)((s32 *)obj[20])[10] == ids[0]) {
        w->slot = i;
    } else {
        for (;;) {
            w->slot = 7;
            i = i + 1;
            if (i > 5) break;
            if (*(s16 *)((s32 *)obj[20])[10] == ids[i]) {
                w->slot = i;
                break;
            }
        }
    }
    slot = w->slot;
    if ((u32)slot > 6) return 0;

    w->x = obj[2];
    w->y = obj[3];
    w->z = obj[4];

    ez = boxes[slot].b;
    if (ez < 0) ez = -ez;
    t = boxes[slot].d;
    if (t < 0) t = -t;
    ez = (ez + t) >> 4;

    ex = boxes[slot].a;
    if (ex < 0) ex = -ex;
    t = boxes[slot].c;
    if (t < 0) t = -t;

    w->x = w->x + (boxes[slot].a << 16);
    w->z = w->z + (boxes[slot].b << 16);
    w->x = w->x >> 20;
    w->z = w->z >> 20;
    ex = (ex + t) >> 4;

    Func_020068d4(w->x, w->z, ex, ez,
                  (camera[79] >> 20) + w->x, (camera[80] >> 20) + w->z);
    Func_02000bf0(0, w->x, w->z, ex, ez, 255);
    Func_02000c02(2, w->x, w->z, ex, ez, 255);
    return 1;
}
