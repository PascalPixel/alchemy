/* resource_389 0x02000608-0x020008bf: staged actor move/redraw family. */
#include "types.h"

extern s32 Data_020095ac[][4];
extern s32 Data_02009554[];

u8 *Func_0808a080(s32 slot);
void Func_0808a090(s32 id, s32 value, s32 callback);
void Func_0808a100(s32 id, s32 mode);
void Func_0808a010(s32 frames);
void Func_0808a0e0(s32 id, s32 x, s32 y);
void Func_0808a0e8(s32 id);
void Func_08009080(u8 *actor, s32 mode);
void Func_080f9010(s32 sound);
void Func_08009150(u8 *actor, s32 x, s32 elevation, s32 y);
void Func_08009158(u8 *actor);
void Func_080091c0(s32 x, s32 y, s32 width, s32 height,
                   s32 localX, s32 localY);
void Func_0808a5e8(void);
s32 Func_02000244(s32 layer, s32 x, s32 y, s32 width, s32 height, s32 value);

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
    u8 *lead = Func_0808a080(0);
    s32 direction = *(u16 *)(lead + 6) >> 12;
    u8 *actor = Func_0808a080(args.actorSlot);
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
    Func_02000244(0, occupiedPosition[0], occupiedPosition[2], width, height, 0);
    Func_0808a090(0, 0x8000, 0x1999);
    Func_0808a100(0, 8);
    Func_0808a010(15);

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
            Func_0808a0e0(0, moveX, moveY);
        }
    }

    lead = Func_0808a080(0);
    *(u32 *)(lead + 108) = 0x020082a9;
    Func_0808a010(4);
    if ((u32)(direction - 6) <= 7)
        Func_08009080(actor, 3);
    else
        Func_08009080(actor, 2);
    Func_080f9010(0xef);
    Func_08009150(actor, args.targetX, args.elevation, args.targetY);
    Func_0808a0e8(0);
    Func_0808a100(0, 2);
    Func_0808a090(0, 0x4ccc, 0x1999);

    {
        s32 packed = Data_02009554[direction];
        Func_0808a0e0(0, (s16)(packed >> 16) / 2, (s16)packed / 2);
    }
    if (args.callback)
        args.callback();
    Func_0808a0e8(0);
    Func_0808a100(0, 1);

    lead = Func_0808a080(0);
    *(u32 *)(lead + 108) = 0;
    Func_08009158(actor);
    Func_080f9010(0x120);
    Func_080f9010(0xd5);
    *(s32 *)(actor + 8) = args.targetX;
    *(s32 *)(actor + 16) = args.targetY;
    *(s32 *)(actor + 36) = 0;
    *(s32 *)(actor + 44) = 0;
    Func_08009080(actor, 1);

    args.targetX += DISP_WORD(0) << 16;
    args.targetY += DISP_WORD(4) << 16;
    args.targetX >>= 20;
    args.targetY >>= 20;
    baseX = *(s32 *)(world + 316) >> 20;
    baseY = *(s32 *)(world + 320) >> 20;
    targetWorldX = baseX + args.targetX;
    targetWorldY = baseY + args.targetY;
    Func_080091c0(args.targetX, args.targetY, width, height,
                  targetWorldX, targetWorldY);
    Func_02000244(0, args.targetX, args.targetY, width, height, 255);
    Func_02000244(2, args.targetX, args.targetY, width, height, 255);

    oldPosition[0] += DISP_WORD(0) << 16;
    oldPosition[2] += DISP_WORD(4) << 16;
    oldPosition[0] >>= 20;
    oldPosition[2] >>= 20;
    Func_080091c0(baseX + oldPosition[0], baseY + oldPosition[2], width, height,
                  oldPosition[0], oldPosition[2]);
    Func_02000244(2, oldPosition[0], oldPosition[2], width, height, 0);
    Func_0808a5e8();
#undef DISP_WORD
}
