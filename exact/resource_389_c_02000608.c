/* resource_389 0x02000608-0x020008bf: staged actor move/redraw family. */
#include "types.h"

extern s32 Data_020095ac[][4];
extern s32 Data_02009554[];

u8 *Func_02001a7c();
u8 *Func_02001a88();
void Func_02000902();
void Func_02001b20();
void Func_02001b60();
void Func_02001b0e();
void Func_02001b78();
u8 *Func_02001b56();
void Func_02001b48();
void Func_02001ae8();
void Func_02001af2();
void Func_02001c40();
void Func_02001b1c();
void Func_02001bc2();
void Func_02001bda();
void Func_02001bac();
void Func_02001bf0();
void Func_02001c08();
void Func_02001c20();
u8 *Func_02001be6();
void Func_02001b88();
void Func_02001cb8();
void Func_02001cbe();
void Func_02001b8c();
void Func_02001c06();
void Func_02000a6e();
void Func_02000a80();
void Func_02001c62();
void Func_02000acc();
void Func_02001d94();

struct Resource389MoveArgs {
    s32 row;
    s32 actorSlot;
    s32 targetX;
    s32 elevation;
    s32 targetY;
    void (*callback)(void);
};

void Func_02000608(struct Resource389MoveArgs args)
{
#define DISP_WORD(offset) (Data_020095ac[args.row][(offset) >> 2])
    u8 *world = *(u8 **)0x03001e70;
    u8 *lead = Func_02001a7c(0);
    s32 direction = *(u16 *)(lead + 6) >> 12;
    u8 *actor = Func_02001a88(args.actorSlot);
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
    occupiedPosition[0] = *(s32 *)(actor + 8) + (DISP_WORD(0) << 16);
    occupiedPosition[2] = *(s32 *)(actor + 16) + (DISP_WORD(4) << 16);
    occupiedPosition[0] >>= 20;
    occupiedPosition[2] >>= 20;
    Func_02000902(0, occupiedPosition[0], occupiedPosition[2], width, height, 0);
    Func_02001b20(0, 0x8000, 0x1999);
    Func_02001b60(0, 8);
    Func_02001b0e(15);

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
            Func_02001b78(0, moveX, moveY);
        }
    }

    lead = Func_02001b56(0);
    *(u32 *)(lead + 108) = 0x020082a9;
    Func_02001b48(4);
    if ((u32)(direction - 6) <= 7)
        Func_02001ae8(actor, 3);
    else
        Func_02001af2(actor, 2);
    Func_02001c40(0xef);
    Func_02001b1c(actor, args.targetX, args.elevation, args.targetY);
    Func_02001bc2(0);
    Func_02001bda(0, 2);
    Func_02001bac(0, 0x4ccc, 0x1999);

    {
        s32 packed = Data_02009554[direction];
        Func_02001bf0(0, (s16)(packed >> 16) / 2, (s16)packed / 2);
    }
    if (args.callback)
        args.callback();
    Func_02001c08(0);
    Func_02001c20(0, 1);

    lead = Func_02001be6(0);
    *(u32 *)(lead + 108) = 0;
    Func_02001b88(actor);
    Func_02001cb8(0x120);
    Func_02001cbe(0xd5);
    *(s32 *)(actor + 8) = args.targetX;
    *(s32 *)(actor + 16) = args.targetY;
    *(s32 *)(actor + 36) = 0;
    *(s32 *)(actor + 44) = 0;
    Func_02001b8c(actor, 1);

    args.targetX += DISP_WORD(0) << 16;
    args.targetY += DISP_WORD(4) << 16;
    args.targetX >>= 20;
    args.targetY >>= 20;
    baseX = *(s32 *)(world + 316) >> 20;
    baseY = *(s32 *)(world + 320) >> 20;
    targetWorldX = baseX + args.targetX;
    targetWorldY = baseY + args.targetY;
    Func_02001c06(args.targetX, args.targetY, width, height,
                  targetWorldX, targetWorldY);
    Func_02000a6e(0, args.targetX, args.targetY, width, height, 255);
    Func_02000a80(2, args.targetX, args.targetY, width, height, 255);

    oldPosition[0] += DISP_WORD(0) << 16;
    oldPosition[2] += DISP_WORD(4) << 16;
    oldPosition[0] >>= 20;
    oldPosition[2] >>= 20;
    Func_02001c62(baseX + oldPosition[0], baseY + oldPosition[2], width, height,
                  oldPosition[0], oldPosition[2]);
    Func_02000acc(2, oldPosition[0], oldPosition[2], width, height, 0);
    Func_02001d94();
#undef DISP_WORD
}
