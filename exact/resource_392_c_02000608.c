/*
 * resource_392 @ 0x02000608 (696 bytes through the pool at 0x020008bc).
 *
 * Six-argument staged actor movement/effect owner.  It derives a footprint
 * from the selected four-word displacement row, temporarily moves actor and
 * camera state, redraws the old and new occupied rectangles, and restores the
 * normal callbacks.  The sibling banks in resources 373/389/391/393 share the
 * control flow but use different displacement/orientation tables, so those
 * addresses are intentionally resolved per overlay.  The sixth argument is
 * invoked through this overlay's generic call-through veneer.
 */
#include "types.h"

extern s32 Data_02008f20[][4];
extern s32 Data_02008ec8[];

u8 *Func_02001460();
u8 *Func_0200146c();
void Func_02000902();
void Func_02001504();
void Func_0200152c();
void Func_020014ea();
void Func_02001544();
u8 *Func_0200153a();
void Func_02001524();
void Func_020014ec();
void Func_020014f6();
void Func_020015b4();
void Func_02001510();
void Func_0200158e();
void Func_020015a6();
void Func_02001590();
void Func_020015bc();
void Func_020015d4();
void Func_020015ec();
u8 *Func_020015ca();
void Func_0200157c();
void Func_0200162c();
void Func_02001632();
void Func_020015fa();
void Func_02000a6e();
void Func_02000a80();
void Func_02001656();
void Func_02000acc();
void Func_02001708();

struct Resource392MoveArgs {
    s32 row;
    s32 actorSlot;
    s32 targetX;
    s32 elevation;
    s32 targetY;
    void (*callback)(void);
};

void Func_02000608(struct Resource392MoveArgs args)
{
#define DISP_WORD(offset) (Data_02008f20[args.row][(offset) >> 2])
    u8 *world = *(u8 **)0x03001e70;
    u8 *lead = Func_02001460(0);
    s32 direction = *(u16 *)(lead + 6) >> 12;
    u8 *actor = Func_0200146c(args.actorSlot);
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
    Func_02001504(0, 0x8000, 0x1999);
    Func_0200152c(0, 8);
    Func_020014ea(15);

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
            Func_02001544(0, moveX, moveY);
        }
    }

    lead = Func_0200153a(0);
    *(u32 *)(lead + 108) = 0x020082a9;
    Func_02001524(4);
    if ((u32)(direction - 6) <= 7)
        Func_020014ec(actor, 3);
    else
        Func_020014f6(actor, 2);
    Func_020015b4(0xef);
    Func_02001510(actor, args.targetX, args.elevation, args.targetY);
    Func_0200158e(0);
    Func_020015a6(0, 2);
    Func_02001590(0, 0x4ccc, 0x1999);

    {
        s32 packed = Data_02008ec8[direction];
        Func_020015bc(0, (s16)(packed >> 16) / 2, (s16)packed / 2);
    }
    if (args.callback)
        args.callback();
    Func_020015d4(0);
    Func_020015ec(0, 1);

    lead = Func_020015ca(0);
    *(u32 *)(lead + 108) = 0;
    Func_0200157c(actor);
    Func_0200162c(0x120);
    Func_02001632(0xd5);
    *(s32 *)(actor + 8) = args.targetX;
    *(s32 *)(actor + 16) = args.targetY;
    *(s32 *)(actor + 36) = 0;
    *(s32 *)(actor + 44) = 0;
    Func_02001590(actor, 1);

    args.targetX += DISP_WORD(0) << 16;
    args.targetY += DISP_WORD(4) << 16;
    args.targetX >>= 20;
    args.targetY >>= 20;
    baseX = *(s32 *)(world + 316) >> 20;
    baseY = *(s32 *)(world + 320) >> 20;
    targetWorldX = baseX + args.targetX;
    targetWorldY = baseY + args.targetY;
    Func_020015fa(args.targetX, args.targetY, width, height,
                  targetWorldX, targetWorldY);
    Func_02000a6e(0, args.targetX, args.targetY, width, height, 255);
    Func_02000a80(2, args.targetX, args.targetY, width, height, 255);

    oldPosition[0] += DISP_WORD(0) << 16;
    oldPosition[2] += DISP_WORD(4) << 16;
    oldPosition[0] >>= 20;
    oldPosition[2] >>= 20;
    Func_02001656(baseX + oldPosition[0], baseY + oldPosition[2], width, height,
                  oldPosition[0], oldPosition[2]);
    Func_02000acc(2, oldPosition[0], oldPosition[2], width, height, 0);
    Func_02001708();
#undef DISP_WORD
}
