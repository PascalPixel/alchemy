#include "types.h"

#define FieldScene_RunSecondaryGroupSequence Func_020026c0

struct SceneWork {
    u8 unknown_000[0x1c0];
    s32 request;
    u8 unknown_1c4[4];
    s32 setup;
    u8 unknown_1cc[12];
    u16 step;
};

extern struct SceneWork *Data_03001ebc;
extern const u8 Data_0200c21c[];
extern const u8 Data_0200c230[];

void Func_02003bf4();
void Func_02003d14();
void Func_02003b14();
void Func_02003c74();
u8 *Func_02003c1c();
void Func_02003bb4();
void Func_02003b7c();
void Func_02003d2c();
void Func_02003d0c();
void Func_02003d7c();
void Func_02003c2c();
void Func_02003c5c();
void Func_02003c7c();
void Func_02003bec();
void Func_02003c8c();
void Func_02003ce4();
u32 Func_02003b2c();
void Func_02003c34();
void Func_02003c84();
void Func_02003cbc();
void Func_02003cd4();
void Func_02003c54();
void Func_02003cfc();
void Func_02003c44();
void Func_02003c9c();
void Func_02003d04();
void Func_02003c94();
s32 Func_02003cc4();
s32 Func_02003c14();
void Func_02003b9c();
void Func_02003ba4();
u8 *Func_02003d1c();
void Func_02003d34();
void Func_02003d3c();
void Func_02003c4c();
void Func_02003d24();
void Func_02003bfc();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunSecondaryGroupSequence(void)
{
    u8 *fieldActor;
    u8 *object;
    struct SceneWork *work;
    u32 random;
    u32 motionPhase;
    const u8 *motionActions;
    s32 scale;
    const u8 *exitActions;

    Func_02003bf4();
    Call4(Func_02003d14, -1, -1, -1, 0);
    Func_02003b14(1);
    Func_02003c74(0, 0, 0);
    Call3(Func_02003c74, 9, 0x1b80000, 0x20a0000);
    object = Func_02003c1c(9);
    Func_02003bb4(object, 0);
    Call4(Func_02003d14, 0x1b80000, -1, 0x20a0000, 0);
    Func_02003b7c();
    Func_02003b14(1);
    Func_02003d2c();
    Call2(Func_02003d0c, 0x19999, 0x3333);
    Call4(Func_02003d14, 0x1b80000, -1, 0x1900000, 1);
    Func_02003d7c(141);
    Call3(Func_02003c2c, 9, 0x19999, 0xcccc);
    Call3(Func_02003c5c, 9, 0x1b8, 0x190);
    Call2(Func_02003d0c, 0xc000, 0x1800);
    Call4(Func_02003d14, 0x1b80000, -1, 0x12c0000, 1);
    Call3(Func_02003c2c, 9, 0xcccc, 0x6666);
    Call3(Func_02003c5c, 9, 0x1b8, 0x12c);
    Func_02003c7c(9, 0);
    Call1(Func_02003d7c, 0x121);
    Func_02003bec(40);
    Call3(Func_02003c2c, 11, 0xcccc, 0x6666);
    Call3(Func_02003c74, 11, 0x1b70000, 0x1320000);
    Func_02003c8c(11, 4, 0);
    Call3(Func_02003c5c, 11, 0x1b7, 0x138);
    Call3(Func_02003c5c, 11, 0x1a0, 0x138);
    Call3(Func_02003c5c, 11, 0x190, 0x100);
    Call3(Func_02003ce4, 11, 0x3000, 40);
    Call4(Func_02003d14, 0x19a0000, -1, 0x1180000, 1);
    Call3(Func_02003c2c, 10, 0x9999, 0x4ccc);
    Call3(Func_02003c74, 10, 0x1b70000, 0x1320000);
    Func_02003c8c(10, 4, 0);
    Call3(Func_02003c5c, 10, 0x1b7, 0x138);
    Call3(Func_02003c5c, 10, 0x1a0, 0x138);
    Call3(Func_02003c5c, 10, 0x184, 0x10e);
    Call3(Func_02003ce4, 10, 0xd000, 10);
    Call3(Func_02003ce4, 11, 0x5000, 10);
    Call3(Func_02003c2c, 0, 0xcccc, 0x6666);
    Call3(Func_02003c74, 0, 0x1b70000, 0x1320000);
    Func_02003c8c(0, 4, 0);
    Call3(Func_02003c5c, 0, 0x1b7, 0x138);
    Call3(Func_02003c5c, 0, 0x1a0, 0x138);
    Call3(Func_02003c5c, 0, 0x184, 0x12c);
    Call3(Func_02003ce4, 0, 0xc000, 40);
    fieldActor = Func_02003c1c(0);
    random = Func_02003b2c();
    motionPhase = random * 5;
    motionActions = Data_0200c21c;
    fieldActor += 102;
    *(u16 *)fieldActor = motionPhase >> 12;
    Func_02003c34(0, motionActions);
    Func_02003c8c(11, 2, 20);
    Func_02003c84(11, 3);
    Call1(Func_02003cbc, 0x256f);
    Func_02003cd4(11, 0, 10);
    Func_02003c84(10, 3);
    object = Func_02003c1c(0);
    if (object != 0) {
        Func_02003c74(1, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    object = Func_02003c1c(0);
    if (object != 0) {
        Func_02003c74(2, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    object = Func_02003c1c(0);
    if (object != 0) {
        Func_02003c74(3, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    Call3(Func_02003c2c, 1, 0xcccc, 0x6666);
    Call3(Func_02003c2c, 2, 0xcccc, 0x6666);
    Call3(Func_02003c2c, 3, 0xcccc, 0x6666);
    Call3(Func_02003c54, 1, 0x17a, 0x136);
    Call3(Func_02003c54, 2, 0x190, 0x120);
    Call3(Func_02003c5c, 3, 0x19a, 0x134);
    Func_02003c7c(1, 1);
    Func_02003c7c(2, 1);
    Call3(Func_02003ce4, 1, 0xc000, 0);
    Call3(Func_02003ce4, 2, 0xc000, 0);
    Call3(Func_02003ce4, 3, 0xc000, 40);
    fieldActor = Func_02003c1c(1);
    random = Func_02003b2c();
    fieldActor += 102;
    *(u16 *)fieldActor = ((random * 5) >> 12);
    fieldActor = Func_02003c1c(2);
    random = Func_02003b2c();
    fieldActor += 102;
    *(u16 *)fieldActor = ((random * 5) >> 12);
    fieldActor = Func_02003c1c(3);
    random = Func_02003b2c();
    fieldActor += 102;
    *(u16 *)fieldActor = ((random * 5) >> 12);
    Func_02003c34(1, motionActions);
    Func_02003c34(2, motionActions);
    Func_02003c34(3, motionActions);
    Call3(Func_02003cfc, 2, 0x101, 60);
    Call3(Func_02003cd4, 0x2002, 0, 10);
    Call3(Func_02003cfc, 10, 0x100, 40);
    Call3(Func_02003ce4, 10, 0x3000, 20);
    Func_02003cd4(10, 0, 10);
    Func_02003c44(0);
    Func_02003c44(1);
    Func_02003c44(2);
    Func_02003c44(3);
    Func_02003b14(1);
    Call3(Func_02003ce4, 2, 0xc000, 10);
    Func_02003c84(2, 3);
    Call3(Func_02003ce4, 0, 0xc000, 0);
    Call3(Func_02003ce4, 1, 0xc000, 0);
    Call3(Func_02003ce4, 3, 0xc000, 20);
    Func_02003c84(10, 3);
    Func_02003cd4(10, 0, 10);
    Call3(Func_02003cfc, 3, 0x101, 60);
    Call3(Func_02003cd4, 0x2003, 0, 10);
    Func_02003c84(10, 3);
    Func_02003cd4(10, 0, 10);
    Call3(Func_02003cfc, 1, 0x101, 60);
    Func_02003cd4(1, 0, 10);
    Call3(Func_02003ce4, 10, 0x5000, 0);
    Func_02003c9c(11, 2);
    Func_02003cd4(11, 0, 10);
    Call3(Func_02003ce4, 0, 0x6000, 0);
    Call3(Func_02003ce4, 1, 0xe000, 0);
    Call3(Func_02003ce4, 2, 0x2000, 0);
    Call3(Func_02003ce4, 3, 0xa000, 20);
    Func_02003c7c(0, 3);
    Func_02003c7c(1, 3);
    Func_02003c7c(2, 3);
    Func_02003c84(3, 3);
    Func_02003bec(10);
    Call3(Func_02003ce4, 0, 0xc000, 0);
    Call3(Func_02003ce4, 1, 0xc000, 0);
    Call3(Func_02003ce4, 2, 0xc000, 0);
    Call3(Func_02003ce4, 3, 0xc000, 10);
    Call3(Func_02003ce4, 10, 0xd000, 10);
    Func_02003c84(10, 3);
    Func_02003cd4(10, 0, 10);
    Func_02003c84(11, 3);
    Call2(Func_02003d04, 2, 0x102);
    Func_02003bec(40);
    Func_02003c94(2, 2);
    Call3(Func_02003cd4, 0x2002, 0, 10);
    Call3(Func_02003ce4, 11, 0x3000, 10);
    Func_02003c7c(11, 4);
    Func_02003cd4(11, 0, 10);
    Call3(Func_02003cfc, 3, 0x101, 40);
    Call3(Func_02003cd4, 0x2003, 0, 10);
    Call3(Func_02003ce4, 10, 0x3000, 10);
    Call3(Func_02003cfc, 10, 0x108, 20);
    Func_02003cd4(10, 0, 10);
    Func_02003c9c(1, 2);
    Func_02003cd4(1, 0, 10);
    Call3(Func_02003ce4, 11, 0x5000, 0);
    Call3(Func_02003ce4, 10, 0x5000, 10);
    Func_02003c7c(11, 4);
    Func_02003cd4(11, 0, 10);
    Call3(Func_02003ce4, 0, 0x6000, 0);
    Call3(Func_02003ce4, 1, 0xe000, 40);
    Call3(Func_02003ce4, 0, 0xc000, 0);
    Call3(Func_02003ce4, 1, 0xc000, 10);
    Func_02003c84(11, 4);
    Func_02003cd4(11, 0, 10);
    Func_02003c84(10, 4);
    Func_02003cd4(10, 0, 10);
    Call3(Func_02003cfc, 0, 0x105, 0);
    Call3(Func_02003cfc, 1, 0x105, 0);
    Call3(Func_02003cfc, 2, 0x105, 0);
    Call3(Func_02003cfc, 3, 0x105, 60);
    Call3(Func_02003ce4, 10, 0x3000, 10);
    Func_02003c7c(10, 3);
    Value2(Func_02003cc4, 10, 0);
    Call3(Func_02003ce4, 1, 0xe000, 0);
    Call3(Func_02003ce4, 2, 0x6000, 0);
    Call3(Func_02003ce4, 3, 0xa000, 0);
    if (Value2(Func_02003c14, 0, 0) == 0) {
        Func_02003bec(20);
        Data_03001ebc->step += 3;
    } else {
        Func_02003bec(20);
        Func_02003c9c(11, 2);
        Func_02003cd4(11, 0, 40);
        Func_02003cd4(11, 0, 10);
        Call3(Func_02003cfc, 3, 0x106, 40);
        Call3(Func_02003ce4, 3, 0xc000, 20);
        Call3(Func_02003cd4, 0x2003, 0, 10);
    }
    Call3(Func_02003ce4, 1, 0xc000, 0);
    Call3(Func_02003ce4, 2, 0xc000, 0);
    Call3(Func_02003ce4, 3, 0xc000, 10);
    Func_02003c7c(1, 3);
    Func_02003c7c(2, 3);
    Func_02003c84(3, 3);
    Func_02003bec(20);
    Call3(Func_02003ce4, 8, 0x3000, 0);
    Func_02003c84(10, 3);
    Call2(Func_02003d0c, 0x10000, 0x2000);
    Call4(Func_02003d14, 0x1180000, -1, 0xc80000, 1);
    Call3(Func_02003c5c, 10, 0x14d, 222);
    Call3(Func_02003c5c, 10, 0x11c, 198);
    Call3(Func_02003ce4, 10, 0x8000, 10);
    Func_02003c84(10, 3);
    Func_02003ce4(8, 0, 10);
    Func_02003c84(8, 3);
    Call3(Func_02003c2c, 8, 0xcccc, 0x6666);
    Call3(Func_02003c5c, 8, 0x10c, 198);
    Call3(Func_02003ce4, 8, 0xc000, 10);
    Func_02003c9c(8, 2);
    Func_02003d7c(125);
    Call6(Func_02003b9c, 71, 60, 76, 11, 2, 1);
    Call6(Func_02003ba4, 71, 60, 2, 1, 16, 11);
    Func_02003bec(20);
    Func_02003c5c(8, 246, 198);
    Call3(Func_02003ce4, 8, 0, 20);
    *(u8 *)(Func_02003d1c() + 85) = 0;
    Call2(Func_02003d0c, 0x9999, 0x1333);
    Call4(Func_02003d14, 0xf80000, -1, 0xaa0000, 1);
    Call3(Func_02003c5c, 10, 0x10e, 198);
    Call3(Func_02003c5c, 10, 0x10e, 174);
    Func_02003c5c(10, 224, 170);
    Func_02003c5c(10, 210, 158);
    Func_02003c5c(10, 246, 148);
    Func_02003c5c(10, 246, 142);
    Func_02003c74(10, 0, 0);
    Data_03001ebc->request = 514;
    Func_02003d34();
    Func_02003d3c();
    Call3(Func_02003c74, 9, 0x1b80000, 0x1540000);
    object = Func_02003c1c(9);
    scale = 0x4000;
    *(u16 *)(object + 6) = scale;
    Call4(Func_02003d14, 0x17c0000, -1, 0x1180000, 0);
    Func_02003b7c();
    Func_02003b14(10);
    Func_02003d2c();
    Func_02003d3c();
    Call1(Func_02003bec, 40);
    Func_02003c84(11, 3);
    Func_02003bec(20);
    Func_02003c7c(0, 3);
    Func_02003c7c(1, 3);
    Func_02003c7c(2, 3);
    ((void (*)())Func_02003c84)(3, 3);
    Call3(Func_02003c5c, 11, 0x1a4, 0x11a);
    Call3(Func_02003c5c, 11, 0x1a4, 0x138);
    Call3(Func_02003c5c, 11, 0x1b7, 0x138);
    Call3(Func_02003c5c, 11, 0x1b7, 0x132);
    Func_02003c74(11, 0, 0);
    exitActions = Data_0200c230;
    Func_02003c34(1, exitActions);
    Call2(Func_02003c34, 2, (s32)exitActions);
    Call2(Func_02003c4c, 3, (s32)exitActions);
    Call4(Func_02003d14, 0x19a0000, -1, 0x12c0000, 1);
    Call3(Func_02003c5c, 0, 0x1a0, 0x138);
    Call3(Func_02003c5c, 0, 0x1b7, 0x138);
    Call3(Func_02003c5c, 0, 0x1b7, 0x132);
    Func_02003c74(0, 0, 0);
    Func_02003d7c(141);
    Call3(Func_02003c2c, 9, 0xcccc, 0x6666);
    Call2(Func_02003d0c, 0x10000, 0x2000);
    Call4(Func_02003d14, 0x1b80000, -1, 0x1a40000, 1);
    Call3(Func_02003c5c, 9, 0x1b8, 0x1a4);
    Call2(Func_02003d0c, 0x20000, scale);
    Call4(Func_02003d14, 0x1b80000, -1, 0x2580000, 1);
    Call3(Func_02003c2c, 9, 0x19999, 0xcccc);
    Call3(Func_02003c5c, 9, 0x1b8, 0x1f4);
    Call3(Func_02003c54, 9, 0x1b8, 0x258);
    Call1(Func_02003d7c, 0x121);
    work = Data_03001ebc;
    work->setup = 24;
    work->request = 0x100;
    Func_02003d34();
    Func_02003d3c();
    Func_02003d24(10);
    Func_02003bfc();
}
