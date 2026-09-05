#include "types.h"

#define Battle_PlaceActorsByFormationKind Func_08012e28

struct BattleCells {
    u8 *actors;   /* 56-byte actor records */
    u8 *unk_04;
    u8 *unk_08;
    u8 *unk_0c;
    u8 *unk_10;
    u8 *work;     /* 16-byte position records */
};

struct Vec2 {
    s32 x;
    s32 y;
};

extern struct BattleCells Data_03001e60;
extern struct Vec2 Data_08013584;

void Func_0800b168(void *object, s32 *position, s32 *scale, u32 mode);

void Battle_PlaceActorsByFormationKind(void)
{
    u8 *actor = Data_03001e60.actors;
    u32 kind = (*(u8 **)(actor + 40))[4];
    struct Vec2 scale;
    u8 *tbl;
    u16 angle;
    u16 step;
    u16 odd = 0;
    u32 cnt;
    u32 i;

    scale = Data_08013584;
    tbl = Data_03001e60.work;

    switch (kind) {
    case 3:
        angle = 0;
        step = 0x2aaa;
        cnt = 6;
        break;
    case 5:
    case 8:
    case 44:
    case 88:
        angle = 0;
        step = 0x2000;
        cnt = 8;
        break;
    case 4:
    case 6:
        angle = 0;
        step = 0x1999;
        cnt = 10;
        break;
    case 20:
        angle = 0;
        step = 0;
        odd = 0x8000;
        cnt = 4;
        break;
    default:
        angle = 0x2000;
        step = 0x4000;
        cnt = 4;
        break;
    }

    for (i = 0; i < cnt; i++) {
        Func_0800b168(actor, (s32 *)(tbl + i * 16), (s32 *)&scale, angle);
        actor += 56;
        angle += step;
        if (i & 1)
            angle += odd;
    }
}
