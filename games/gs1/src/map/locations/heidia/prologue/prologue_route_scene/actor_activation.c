#include "types.h"

#define StopBlockedActorMotion Func_020002a8
#define PositionSceneActorPair Func_02001774
#define ActivateSceneActor8 Func_020019b4
#define ActivateSceneActor10 Func_020019c0
#define ActivateSceneActor11 Func_020019cc
#define ActivateSceneActor12 Func_020019d8
#define ActivateSceneActor13 Func_020019e4
#define ActivateSceneActor15 Func_020019f0
#define ActivateSceneActor17 Func_020019fc
#define ActivateSceneActor18 Func_02001a08
#define ActivateSceneActor9 Func_02001a14
#define ActivateSceneActor19 Func_02001a20
#define ActivateSceneActor14 Func_02001a2c
#define ActivateSceneActor16 Func_02001a38
typedef struct { s32 unk0; s32 unk4; s32 unk8; } Desc;

typedef struct { u8 filler0[0x28]; s16 *unk28; } Sub;

typedef struct {
    u8 filler0[6]; u16 unk6; s32 unk8; s32 unkC; s32 unk10;
    u8 filler14[0x10]; s32 unk24; u8 filler28[4]; s32 unk2C;
    u8 filler30[8]; s32 unk38; u8 filler3C[4]; s32 unk40;
    u8 filler44[0xC]; Sub *unk50;
} Ent;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

extern s32 Data_0200b15c[];
extern s32 Data_0200b19c[];
extern s32 Data_02000240[];

Ent *Func_02000342(Desc *, Ent *);
s32 Func_02003352(Ent *, Desc *);
Obj *Func_0200481e(s32);
Obj *Func_02004826(s32);
void Func_02004814(void);
void Func_020047ee(Obj *, s32, s32, s32);
void Func_020047de(Obj *, s32);
void Func_0200481a(Obj *, s32, s32, s32);
void Func_02004816(Obj *, s32);
void Func_02004820(Obj *, s32);
void Func_02004946(s32);
void Func_0200484c(Obj *);
void Func_02004954(s32);
void Func_020048b8(void);
void Func_020032be(int arg0);
void Func_020032ca(int arg0);
void Func_020032d6(int arg0);
void Func_020032e2(int arg0);
void Func_020032ee(int arg0);
void Func_020032fa(int arg0);
void Func_02003306(int arg0);
void Func_02003312(int arg0);
void Func_0200331e(int arg0);
void Func_0200332a(int arg0);
void Func_02003336(int arg0);
void Func_02003342(int arg0);

s32 StopBlockedActorMotion(Ent *actor)
{
    Desc destination;
    u32 direction_index;
    s32 step;
    Ent *blocking_actor;

    direction_index = actor->unk6 >> 12;
    step = Data_0200b15c[direction_index];
    destination.unk0 = actor->unk8 + (step & 0xffff0000);
    destination.unk4 = actor->unkC;
    step = step << 16;
    destination.unk8 = actor->unk10 + step;
    blocking_actor = Func_02000342(&destination, actor);
    if (blocking_actor != 0) {
        u32 kind_index = 0;
        s32 blocking_kind = *blocking_actor->unk50->unk28;
        s32 *allowed_kinds = Data_0200b19c;

        do {
            if (blocking_kind == *allowed_kinds++) {
                goto done;
            }
            kind_index++;
        } while (kind_index <= 5);
        actor->unk24 = 0;
        actor->unk2C = 0;
        actor->unk38 = 0x80000000;
        actor->unk40 = 0x80000000;
    }
    step = Data_0200b15c[direction_index];
    destination.unk0 = actor->unk8 + (step & 0xffff0000);
    destination.unk4 = actor->unkC;
    step = step << 16;
    destination.unk8 = actor->unk10 + step;
    if (Func_02003352(actor, &destination) > 0) {
        actor->unk24 = 0;
        actor->unk2C = 0;
        actor->unk38 = 0x80000000;
        actor->unk40 = 0x80000000;
    }
done:
    return 0;
}

void PositionSceneActorPair(s32 actor_id, s32 x_offset, s32 z_offset)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Func_0200481e(Data_02000240[125]);
    q = Func_02004826(actor_id);
    Func_02004814();
    {
        x = ((p->f08 + (x_offset << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (z_offset << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_020047ee(p, x, p->f0c, y);
    }
    Func_020047de(p, 27);
    {
        x = ((q->f08 + (x_offset << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (z_offset << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_0200481a(q, x, q->f0c, y);
    }
    if (x_offset < 0 || z_offset < 0) {
        Func_02004816(q, 4);
    } else {
        Func_02004820(q, 3);
    }
    Func_02004946(226);
    Func_0200484c(p);
    Func_02004954(288);
    Func_020048b8();
}

void ActivateSceneActor8(void)
{
    Func_020032be(8);
}

void ActivateSceneActor10(void)
{
    Func_020032ca(10);
}

void ActivateSceneActor11(void)
{
    Func_020032d6(11);
}

void ActivateSceneActor12(void)
{
    Func_020032e2(12);
}

void ActivateSceneActor13(void)
{
    Func_020032ee(13);
}

void ActivateSceneActor15(void)
{
    Func_020032fa(15);
}

void ActivateSceneActor17(void)
{
    Func_02003306(17);
}

void ActivateSceneActor18(void)
{
    Func_02003312(18);
}

void ActivateSceneActor9(void)
{
    Func_0200331e(9);
}

void ActivateSceneActor19(void)
{
    Func_0200332a(19);
}

void ActivateSceneActor14(void)
{
    Func_02003336(14);
}

void ActivateSceneActor16(void)
{
    Func_02003342(16);
}
