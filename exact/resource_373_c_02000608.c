/*
 * resource_373 @ 0x02000608 (696 bytes through the pool at 0x020008bc).
 *
 * Six-argument staged actor movement/effect owner.  It derives a footprint
 * from the selected four-word displacement row, temporarily moves actor and
 * camera state, redraws the old and new occupied rectangles, and restores the
 * normal callbacks.  The sibling banks in resources 389/391/392/393 share the
 * control flow but use different displacement/orientation tables, so those
 * addresses are intentionally resolved per overlay.  The sixth argument is
 * invoked through this overlay's generic call-through veneer.
 */
#include "types.h"

extern s32 Data_0200e1e8[][4];
extern s32 Data_0200e190[];

u8 *Func_02006610(s32 slot);
u8 *Func_0200661c(s32 slot);
void Func_020066b4(s32 id, s32 value, s32 callback);
void Func_02006740(s32 id, s32 value, s32 callback);
void Func_02006714(s32 id, s32 mode);
void Func_0200678e(s32 id, s32 mode);
void Func_020067d4(s32 id, s32 mode);
void Func_0200668a(s32 frames);
void Func_020066c4();
void Func_0200672c(s32 id, s32 x, s32 y);
void Func_020067a4(s32 id, s32 x, s32 y);
void Func_02006776(s32 id);
void Func_020067bc(s32 id);
u8 *Func_020066ea(s32 slot);
u8 *Func_0200677a(s32 slot);
void Func_02006614(u8 *actor, s32 mode);
void Func_0200661e(u8 *actor, s32 mode);
void Func_020066b8(u8 *actor, s32 mode);
void Func_0200687c(s32 sound);
void Func_020068f4(s32 sound);
void Func_020068fa(s32 sound);
void Func_02006658(u8 *actor, s32 x, s32 elevation, s32 y);
void Func_02006752(s32 x, s32 y, s32 width, s32 height,
                   s32 localX, s32 localY);
void Func_020067ae(s32 x, s32 y, s32 width, s32 height,
                   s32 localX, s32 localY);
void Func_020069d0(void);
s32 Func_02000902(s32 layer, s32 x, s32 y, s32 width, s32 height, s32 value);
s32 Func_02000a6e(s32 layer, s32 x, s32 y, s32 width, s32 height, s32 value);
s32 Func_02000a80(s32 layer, s32 x, s32 y, s32 width, s32 height, s32 value);
s32 Func_02000acc(s32 layer, s32 x, s32 y, s32 width, s32 height, s32 value);

struct Resource373MoveArgs {
    s32 row;
    s32 actorSlot;
    s32 targetX;
    s32 elevation;
    s32 targetY;
    void (*callback)(void);
};

void Func_02000608(struct Resource373MoveArgs args)
{
#define DISP_WORD(offset) (Data_0200e1e8[args.row][(offset) >> 2])
    u8 *world = *(u8 **)0x03001e70;
    u8 *lead = Func_02006610(0);
    s32 direction = *(u16 *)(lead + 6) >> 12;
    u8 *actor = Func_0200661c(args.actorSlot);
    s32 dy0 = DISP_WORD(4) < 0 ? -DISP_WORD(4) : DISP_WORD(4);
    s32 dy1 = DISP_WORD(12) < 0 ? -DISP_WORD(12) : DISP_WORD(12);
    s32 height = (dy0 + dy1) >> 4;
    s32 dx0 = DISP_WORD(0) < 0 ? -DISP_WORD(0) : DISP_WORD(0);
    s32 dx1 = DISP_WORD(8) < 0 ? -DISP_WORD(8) : DISP_WORD(8);
    s32 width = (dx0 + dx1) >> 4;
    s32 oldPosition[3];
    s32 occupiedPosition[3];
    s32 baseX;
    s32 baseY;
    s32 targetWorldX;
    s32 targetWorldY;

    *(s32 *)(actor + 48) = 0x8000;
    *(s32 *)(actor + 52) = 0x1999;
    oldPosition[0] = *(s32 *)(actor + 8);
    oldPosition[2] = *(s32 *)(actor + 16);
    occupiedPosition[0] = *(s32 *)(actor + 8)
        + (DISP_WORD(0) << 16);
    occupiedPosition[2] = *(s32 *)(actor + 16)
        + (DISP_WORD(4) << 16);
    occupiedPosition[0] >>= 20;
    occupiedPosition[2] >>= 20;
    Func_02000902(0, occupiedPosition[0], occupiedPosition[2], width, height, 0);
    Func_020066b4(0, 0x8000, 0x1999);
    Func_02006714(0, 8);
    Func_0200668a(15);

    {
        s32 moveX = args.targetX - oldPosition[0];
        if (moveX < 0)
            moveX += 0x1ffff;
        moveX >>= 17;
        {
            s32 moveY = args.targetY - oldPosition[2];
            if (moveY < 0)
                moveY += 0x1ffff;
            moveY >>= 17;
            Func_0200672c(0, moveX, moveY);
        }
    }

    lead = Func_020066ea(0);
    *(u32 *)(lead + 108) = 0x020082a9;
    Func_020066c4(4);
    if ((u32)(direction - 6) <= 7)
        Func_02006614(actor, 3);
    else
        Func_0200661e(actor, 2);
    Func_0200687c(0xef);
    Func_02006658(actor, args.targetX, args.elevation, args.targetY);
    Func_02006776(0);
    Func_0200678e(0, 2);
    Func_02006740(0, 0x4ccc, 0x1999);

    {
        s32 packed = Data_0200e190[direction];
        Func_020067a4(0, (s16)(packed >> 16) / 2, (s16)packed / 2);
    }
    if (args.callback)
        args.callback();
    Func_020067bc(0);
    Func_020067d4(0, 1);

    lead = Func_0200677a(0);
    *(u32 *)(lead + 108) = 0;
    Func_020066c4(actor);
    Func_020068f4(0x120);
    Func_020068fa(0xd5);
    *(s32 *)(actor + 8) = args.targetX;
    *(s32 *)(actor + 16) = args.targetY;
    *(s32 *)(actor + 36) = 0;
    *(s32 *)(actor + 44) = 0;
    Func_020066b8(actor, 1);

    args.targetX += DISP_WORD(0) << 16;
    args.targetY += DISP_WORD(4) << 16;
    args.targetX >>= 20;
    args.targetY >>= 20;
    baseX = *(s32 *)(world + 316) >> 20;
    baseY = *(s32 *)(world + 320) >> 20;
    targetWorldX = baseX + args.targetX;
    targetWorldY = baseY + args.targetY;
    Func_02006752(args.targetX, args.targetY, width, height,
                  targetWorldX, targetWorldY);
    Func_02000a6e(0, args.targetX, args.targetY, width, height, 255);
    Func_02000a80(2, args.targetX, args.targetY, width, height, 255);

    oldPosition[0] += DISP_WORD(0) << 16;
    oldPosition[2] += DISP_WORD(4) << 16;
    oldPosition[0] >>= 20;
    oldPosition[2] >>= 20;
    Func_020067ae(baseX + oldPosition[0], baseY + oldPosition[2], width, height,
                  oldPosition[0], oldPosition[2]);
    Func_02000acc(2, oldPosition[0], oldPosition[2], width, height, 0);
    Func_020069d0();
#undef DISP_WORD
}
