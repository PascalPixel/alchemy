/*
 * resource_393 @ 0x02000608 (696 bytes through the pool at 0x020008bc).
 *
 * By-value staged actor movement/effect owner. It shares the proved control
 * flow of resources 373/389/391/392, with resource-local actor, camera,
 * redraw, displacement, orientation, and callback-veneer identities.
 */
#include "types.h"

extern s32 Data_02008f68[][4];
extern s32 Data_02008f10[];

u8 *Func_020014b0();
u8 *Func_020014bc();
void Func_02000902();
void Func_02001554();
void Func_0200157c();
void Func_02001542();
void Func_0200159c();
u8 *Func_0200158a();
void Func_0200154c();
void Func_02001556();
void Func_020015fc();
void Func_02001570();
void Func_020015e6();
void Func_020015f6();
void Func_020015e0();
void Func_02001614();
void Func_0200162c();
void Func_0200163c();
u8 *Func_0200161a();
void Func_020015dc();
void Func_02001674();
void Func_0200167a();
void Func_020015f0();
void Func_0200165a();
void Func_02000a6e();
void Func_02000a80();
void Func_020016b6();
void Func_02000acc();
void Func_02001750();

struct Resource393MoveArgs {
    s32 row;
    s32 actorSlot;
    s32 targetX;
    s32 elevation;
    s32 targetY;
    void (*callback)(void);
};

void Func_02000608(struct Resource393MoveArgs args)
{
#define DISP_WORD(offset) (Data_02008f68[args.row][(offset) >> 2])
    u8 *world = *(u8 **)0x03001e70;
    u8 *lead = Func_020014b0(0);
    s32 direction = *(u16 *)(lead + 6) >> 12;
    u8 *actor = Func_020014bc(args.actorSlot);
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
    Func_02001554(0, 0x8000, 0x1999);
    Func_0200157c(0, 8);
    Func_02001542(15);

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
            Func_0200159c(0, moveX, moveY);
        }
    }

    lead = Func_0200158a(0);
    *(u32 *)(lead + 108) = 0x020082a9;
    Func_0200157c(4);
    if ((u32)(direction - 6) <= 7)
        Func_0200154c(actor, 3);
    else
        Func_02001556(actor, 2);
    Func_020015fc(0xef);
    Func_02001570(actor, args.targetX, args.elevation, args.targetY);
    Func_020015e6(0);
    Func_020015f6(0, 2);
    Func_020015e0(0, 0x4ccc, 0x1999);

    {
        s32 packed = Data_02008f10[direction];
        Func_02001614(0, (s16)(packed >> 16) / 2, (s16)packed / 2);
    }
    if (args.callback)
        args.callback();
    Func_0200162c(0);
    Func_0200163c(0, 1);

    lead = Func_0200161a(0);
    *(u32 *)(lead + 108) = 0;
    Func_020015dc(actor);
    Func_02001674(0x120);
    Func_0200167a(0xd5);
    *(s32 *)(actor + 8) = args.targetX;
    *(s32 *)(actor + 16) = args.targetY;
    *(s32 *)(actor + 36) = 0;
    *(s32 *)(actor + 44) = 0;
    Func_020015f0(actor, 1);

    args.targetX += DISP_WORD(0) << 16;
    args.targetY += DISP_WORD(4) << 16;
    args.targetX >>= 20;
    args.targetY >>= 20;
    baseX = *(s32 *)(world + 316) >> 20;
    baseY = *(s32 *)(world + 320) >> 20;
    targetWorldX = baseX + args.targetX;
    targetWorldY = baseY + args.targetY;
    Func_0200165a(args.targetX, args.targetY, width, height,
                  targetWorldX, targetWorldY);
    Func_02000a6e(0, args.targetX, args.targetY, width, height, 255);
    Func_02000a80(2, args.targetX, args.targetY, width, height, 255);

    oldPosition[0] += DISP_WORD(0) << 16;
    oldPosition[2] += DISP_WORD(4) << 16;
    oldPosition[0] >>= 20;
    oldPosition[2] >>= 20;
    Func_020016b6(baseX + oldPosition[0], baseY + oldPosition[2], width, height,
                  oldPosition[0], oldPosition[2]);
    Func_02000acc(2, oldPosition[0], oldPosition[2], width, height, 0);
    Func_02001750();
#undef DISP_WORD
}
