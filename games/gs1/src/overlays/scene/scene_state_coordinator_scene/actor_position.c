#include "types.h"

#define SceneActor_ShiftActorSeventeenByLeaderRow Func_0200042c
#define SceneActor_ShiftActorEighteenByInputAndLeaderColumn Func_02000490
#define SceneActor_MarkObjectAtTiles94To95 Func_02000a10
#define SceneActor_PlaceSlots1To3FromWork Func_02001898
#define SceneActor_PlaceLinkedActorAbove Func_02003058
typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
} Obj;

typedef struct Obj_02000a10 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[14];
    u8 f22;
} Obj_02000a10;

typedef struct Obj_02003058 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[65];
    u8 f55;
    u8 f56[14];
    s16 f64;
} Obj_02003058;

extern s32 Data_02000240[];
extern volatile u32 Data_03001ae8;
extern u8 Data_0200c008[];

Obj *Func_020040f4(s32);
void Func_02004054(s32, s32, s32, s32, s32, s32);
void Func_020007a2(s32, s32, s32);
Obj *Func_02004124(s32);
void Func_02004078(s32, s32, s32, s32, s32, s32);
Obj *Func_02004158(s32);
Obj *Func_0200417e(s32);
void Func_02004100(s32, s32, s32, s32, s32, s32);
void Func_0200084e(s32, s32, s32);
Obj *Func_020041d0(s32);
void Func_02004124_a(s32, s32, s32, s32, s32, s32);
Obj *Func_020046cc(void);
s32 Func_02005508(s32);
s32 Func_02005512(s32);
void Func_020055b8(s32, s32, s32);
s32 Func_02005530(s32);
s32 Func_0200553a(s32);
void Func_020055dc(s32, s32, s32);
s32 Func_02005554(s32);
s32 Func_0200555e(s32);
void Func_02005600(s32, s32, s32);
Obj *Func_02006d24(s16);
void Func_02006c56(Obj *, s32, s32, s32);
void Func_02006c3e(Obj *, s32);
void Func_02006e8c(s32);

void SceneActor_ShiftActorSeventeenByLeaderRow(void)
{
    Obj *o;
    s32 v;
    s32 t;

    o = Func_020040f4(Data_02000240[125]);
    v = o->f10 >> 20;
    t = -48;
    if (v <= 8) {
        t = 48;
    }
    Func_02004054(67, 8, 3, 1, 64, v);
    Func_020007a2(17, 0, t);
    o = Func_02004124(17);
    v = o->f10 >> 20;
    Func_02004078(64, 24, 3, 1, 64, v);
}

void SceneActor_ShiftActorEighteenByInputAndLeaderColumn(void)
{
    Obj *o;
    s32 a;
    s32 b;
    s32 s;

    o = Func_02004158(Data_02000240[125]);
    a = o->f08 >> 20;
    if ((Data_03001ae8 & 32) != 0) {
        s = -1;
    }
    if ((Data_03001ae8 & 16) != 0) {
        s = 1;
    }
    o = Func_0200417e(17);
    b = o->f10 >> 20;
    if (a == 63) {
        if (b == 11) {
            return;
        }
        b = 160;
    } else if (a == 67) {
        if (b == 11 && s == -1) {
            return;
        }
        b = 96;
    } else {
        if (b == 11) {
            b = 96;
        } else {
            b = 160;
        }
        b = -b;
    }
    Func_02004100(72, 9, 1, 3, a, 9);
    Func_0200084e(18, b, 0);
    o = Func_020041d0(18);
    a = o->f08 >> 20;
    Func_02004124_a(63, 25, 1, 3, a, 9);
}

void SceneActor_MarkObjectAtTiles94To95(void)
{
    Obj_02000a10 *o;
    s32 x;
    s32 y;

    o = Func_020046cc();
    if (o != 0) {
        x = o->f08 >> 19;
        y = o->f10 >> 19;
        if (x >= 94 && x <= 95 && y > 23 && y <= 26) {
            o->f22 = 1;
        }
    }
}

void SceneActor_PlaceSlots1To3FromWork(void)
{
    {
        s32 x = Func_02005508(896);
        s32 y = Func_02005512(904);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_020055b8(1, x, y);
    }
    {
        s32 x = Func_02005530(912);
        s32 y = Func_0200553a(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_020055dc(2, x, y);
    }
    {
        s32 x = Func_02005554(928);
        s32 y = Func_0200555e(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_02005600(3, x, y);
    }
}

s32 SceneActor_PlaceLinkedActorAbove(Obj_02003058 *a)
{
    Obj_02003058 *o = Func_02006d24(a->f64);

    Func_02006c56(o, a->f08, a->f0c + 0x240000, a->f10);
    o->f55 = 0;
    Func_02006c3e(o, (s32)Data_0200c008);
    Func_02006e8c(83);
    a->f64 = 0;
    return 0;
}
