/* resource_391 0x02000608-0x020008bf: staged actor move/redraw family. */
#include "types.h"

extern s32 Data_0200adc0[][4];
extern s32 Data_0200ad68[];

u8 *Func_02003250();
u8 *Func_0200325c();
void Func_02000902();
void Func_020032f4();
void Func_02003354();
void Func_020032da();
void Func_0200336c();
u8 *Func_0200332a();
void Func_02003314();
void Func_020032c4();
void Func_020032ce();
void Func_02003454();
void Func_02003308();
void Func_020033b6();
void Func_020033ce();
void Func_02003380();
void Func_020033e4();
void Func_020033fc();
void Func_02003414();
u8 *Func_020033ba();
void Func_02003374();
void Func_020034cc();
void Func_020034d2();
void Func_02003368();
void Func_020033f2();
void Func_02000a6e();
void Func_02000a80();
void Func_0200344e();
void Func_02000acc();
void Func_020035a8();

struct Resource391MoveArgs {
    s32 row;
    s32 actorSlot;
    s32 targetX;
    s32 elevation;
    s32 targetY;
    void (*callback)(void);
};

void Func_02000608(struct Resource391MoveArgs args)
{
#define DISP_WORD(offset) (Data_0200adc0[args.row][(offset) >> 2])
    u8 *world = *(u8 **)0x03001e70;
    u8 *lead = Func_02003250(0);
    s32 direction = *(u16 *)(lead + 6) >> 12;
    u8 *actor = Func_0200325c(args.actorSlot);
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
    Func_020032f4(0, 0x8000, 0x1999);
    Func_02003354(0, 8);
    Func_020032da(15);

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
            Func_0200336c(0, moveX, moveY);
        }
    }

    lead = Func_0200332a(0);
    *(u32 *)(lead + 108) = 0x020082a9;
    Func_02003314(4);
    if ((u32)(direction - 6) <= 7)
        Func_020032c4(actor, 3);
    else
        Func_020032ce(actor, 2);
    Func_02003454(0xef);
    Func_02003308(actor, args.targetX, args.elevation, args.targetY);
    Func_020033b6(0);
    Func_020033ce(0, 2);
    Func_02003380(0, 0x4ccc, 0x1999);

    {
        s32 packed = Data_0200ad68[direction];
        Func_020033e4(0, (s16)(packed >> 16) / 2, (s16)packed / 2);
    }
    if (args.callback)
        args.callback();
    Func_020033fc(0);
    Func_02003414(0, 1);

    lead = Func_020033ba(0);
    *(u32 *)(lead + 108) = 0;
    Func_02003374(actor);
    Func_020034cc(0x120);
    Func_020034d2(0xd5);
    *(s32 *)(actor + 8) = args.targetX;
    *(s32 *)(actor + 16) = args.targetY;
    *(s32 *)(actor + 36) = 0;
    *(s32 *)(actor + 44) = 0;
    Func_02003368(actor, 1);

    args.targetX += DISP_WORD(0) << 16;
    args.targetY += DISP_WORD(4) << 16;
    args.targetX >>= 20;
    args.targetY >>= 20;
    baseX = *(s32 *)(world + 316) >> 20;
    baseY = *(s32 *)(world + 320) >> 20;
    targetWorldX = baseX + args.targetX;
    targetWorldY = baseY + args.targetY;
    Func_020033f2(args.targetX, args.targetY, width, height,
                  targetWorldX, targetWorldY);
    Func_02000a6e(0, args.targetX, args.targetY, width, height, 255);
    Func_02000a80(2, args.targetX, args.targetY, width, height, 255);

    oldPosition[0] += DISP_WORD(0) << 16;
    oldPosition[2] += DISP_WORD(4) << 16;
    oldPosition[0] >>= 20;
    oldPosition[2] >>= 20;
    Func_0200344e(baseX + oldPosition[0], baseY + oldPosition[2], width, height,
                  oldPosition[0], oldPosition[2]);
    Func_02000acc(2, oldPosition[0], oldPosition[2], width, height, 0);
    Func_020035a8();
#undef DISP_WORD
}
