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

extern s32 *Data_03001e70;
extern s32 Data_0200e1d0[];
extern s32 Data_0200e1e8[];

extern s32 *Func_020068b6(void);
extern void Func_020068d4(s32 x, s32 z, s32 ex, s32 ez, s32 cx, s32 cz);
extern void Func_02000bf0(s32 kind, s32 x, s32 z, s32 ex, s32 ez, s32 alpha);
extern void Func_02000c02(s32 kind, s32 x, s32 z, s32 ex, s32 ez, s32 alpha);

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
    s32 *camera = Data_03001e70;
    struct Work work;
    s32 *obj;
    s32 i;
    s32 slot;
    s32 ex, ez, t, k;

    obj = Func_020068b6();
    i = 0;
    while (*(s16 *)((s32 *)obj[20])[10] != Data_0200e1d0[i]) {
        work.slot = 7;
        i = i + 1;
        if ((u32)i > 5) goto searched;
    }
    work.slot = i;
searched:
    slot = work.slot;
    if ((u32)slot > 6) return 0;

    work.x = obj[2];
    work.y = obj[3];
    work.z = obj[4];

    k = slot * 4;

    ez = Data_0200e1e8[k + 1];
    if (ez < 0) ez = -ez;
    t = Data_0200e1e8[k + 3];
    if (t < 0) t = -t;
    ez = (ez + t) >> 4;

    ex = Data_0200e1e8[k];
    if (ex < 0) ex = -ex;
    t = Data_0200e1e8[k + 2];
    if (t < 0) t = -t;

    work.x = work.x + (Data_0200e1e8[k] << 16);
    work.z = work.z + (Data_0200e1e8[k + 1] << 16);
    work.x = work.x >> 20;
    work.z = work.z >> 20;
    ex = (ex + t) >> 4;

    Func_020068d4(work.x, work.z, ex, ez,
                  (camera[79] >> 20) + work.x, (camera[80] >> 20) + work.z);
    Func_02000bf0(0, work.x, work.z, ex, ez, 255);
    Func_02000c02(2, work.x, work.z, ex, ez, 255);
    return 1;
}
