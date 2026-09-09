#include "types.h"

#define ActorPresentation_SetEightSceneCells Func_020010a4
#define SceneActor_RunActor22PlacementSequence Func_02001a64
#define ActorPresentation_SetFourActorsModeByBit Func_020020fc
#define ActorPresentation_SelectActorTwentySevenState Func_02003e18
#define SceneActor_SetModeByFrameBit1 Func_020042bc
typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Thing1;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} Thing2;

struct Actor {
    u8 reserved_00[80];
    u8 *presentation;
};

extern s32 Data_0200d7fc;
extern u32 Data_03001e40;

void Func_0200576e(s32, s32, s32, s32, s32, s32);
void Func_02005780(s32, s32, s32, s32, s32, s32);
void Func_02005794(s32, s32, s32, s32, s32, s32);
void Func_020057a6(s32, s32, s32, s32, s32, s32);
void Func_020057ba(s32, s32, s32, s32, s32, s32);
void Func_020057cc(s32, s32, s32, s32, s32, s32);
void Func_020057dc(s32, s32, s32, s32, s32, s32);
void Func_020057f0(s32, s32, s32, s32, s32, s32);
Thing1 *Func_0200619a(s32);
void Func_02006208(s32, s32, s32);
void Func_020061c6(s32, s32, s32);
void Func_02006210(s32, s32, s32);
void Func_02006262(s32, s32, s32);
void Func_02006198(s32);
void Func_020062c2(s32, s32);
void Func_020061a8(s32);
void Func_0200628e(s32);
void Func_020062a6(s32, s32);
void Func_0200627e(s32, s32);
void Func_020062b6(s32, s32);
void Func_02006276(s32, s32);
void Func_02006276_a(s32, s32);
Thing2 *Func_02006214(s32);
void Func_0200625e(s32, s32, s32);
void Func_02006284(s32);
void Func_02006296(s32, s32, s32);
s32 Func_02006840(s32);
void Func_020041b6(s32, s32);
s32 Func_0200684c(s32);
void Func_020041c2(s32, s32);
s32 Func_02006858(s32);
void Func_020041ce(s32, s32);
s32 Func_02006864(s32);
void Func_020041da(s32, s32);
s32 Func_02006872(s32);
void Func_020041e8(s32, s32);
s32 Func_0200687e(s32);
void Func_020041f4(s32, s32);
s32 Func_0200688a(s32);
void Func_02004200(s32, s32);
s32 Func_02006896(s32);
void Func_0200420c(s32, s32);
struct Actor *Func_0200854a(s32);
void Func_0200899c(s32, s32);
void Func_020089a6(s32, s32);
s32 Func_0200891e(s32, s32);
void Func_02008710(s32);

void ActorPresentation_SetEightSceneCells(void)
{
    s32 a = 15;
    s32 d = 0x35;
    s32 e;
    s32 b;
    s32 c;
    s32 f;

    Func_0200576e(29, 23, 1, 1, a, d);
    b = 14;
    Func_02005780(29, 23, 1, 1, b, d);
    c = 13;
    Func_02005794(29, 23, 1, 1, c, d);
    Func_020057a6(26, 20, 2, 1, b, 0x34);
    e = 0x36;
    Func_020057ba(25, 21, 1, 1, c, e);
    Func_020057cc(25, 21, 1, 1, a, e);
    Func_020057dc(14, 0x35, 1, 1, b, e);
    f = 0x37;
    Func_020057f0(13, 0x37, 1, 1, a, f);
}

void SceneActor_RunActor22PlacementSequence(s32 x, s32 y)
{
    Thing1 *a;
    s32 w = 0x10000;
    s32 h = 0x8000;
    Thing2 *b;

    a = Func_0200619a(0);
    if (a != 0) {
        Func_02006208(22, a->unk8, a->unk10);
    }
    Func_020061c6(22, w, h);
    Func_02006210(22, x, y);
    Func_02006262(0, 22, 0);
    Func_02006198(20);
    Func_020062c2(0, 0x102);
    Func_020061a8(40);
    Func_0200628e(0xe7d);
    Func_020062a6(22, 0);
    Func_0200627e(22, 2);
    Func_020062b6(22, 0);
    Func_02006276(0, 3);
    Func_02006276_a(22, 2);
    b = Func_02006214(0);
    if (b != 0) {
        Func_0200625e(22, b->unkA, b->unk12);
    }
    Func_02006284(22);
    Func_02006296(22, 0, 0);
}

void ActorPresentation_SetFourActorsModeByBit(void)
{
    if (((*(u32 *)0x03001E40 >> Data_0200d7fc) & 3) != 0) {
        Func_020041b6(Func_02006840(32), 1);
        Func_020041c2(Func_0200684c(33), 1);
        Func_020041ce(Func_02006858(30), 1);
        Func_020041da(Func_02006864(29), 1);
    } else {
        Func_020041e8(Func_02006872(32), 8);
        Func_020041f4(Func_0200687e(33), 8);
        Func_02004200(Func_0200688a(30), 8);
        Func_0200420c(Func_02006896(29), 8);
    }
}

void ActorPresentation_SelectActorTwentySevenState(void)
{
    struct Actor *actor = Func_0200854a(27);
    u32 flags = Data_03001e40;
    u8 *presentation = actor->presentation;

    if (flags & 1) {
        u8 *state = presentation + 35;
        *state = 2;
    } else {
        u8 *state = presentation + 35;
        *state = 64;
    }
}

void SceneActor_SetModeByFrameBit1(s32 o)
{
    s32 v;

    if ((*(volatile s32 *)0x03001e40 & 2) != 0) {
        Func_0200899c(o, 7);
    } else {
        Func_020089a6(o, 0);
    }
    {
        volatile s32 *q = (volatile s32 *)0x03001e40;
        v = (*(s32 *)0x0200d7fc << 3) + 16;
        if (Func_0200891e(*q, v) == 0) {
            Func_02008710(o);
        }
    }

}
