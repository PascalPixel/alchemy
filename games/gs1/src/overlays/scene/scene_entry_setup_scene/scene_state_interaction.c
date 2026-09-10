#include "types.h"

#define SceneState_ApplySixRectsAfterFlag161 Func_020010cc
#define SceneState_RunActor13AtColumn42Setup Func_02001644
#define SceneState_ApplyRectAt19_44AndRunThree Func_0200196c
typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    u8 filler10[4];
    s32 unk14;
    u8 filler18[61];
    u8 unk55;
} Struct_1644;

void Func_02005f50(s32);
void Func_02005f1c(s32, s32, s32, s32, s32, s32);
void Func_02005f18(s32, s32, s32, s32, s32, s32);
void Func_02005f28(s32, s32, s32, s32, s32, s32);
void Func_02005f54(s32, s32, s32, s32, s32, s32);
void Func_02005f4c(s32, s32, s32, s32, s32, s32);
void Func_02005f5c(s32, s32, s32, s32, s32, s32);
Struct_1644 *Func_020064f8();
Struct_1644 *Func_020064f8_a();
void Func_020064e6(void);
void Func_020064ec(s32);
void Func_02006662(s32);
void Func_020064ac(s32, s32, s32, s32, s32, s32);
void Func_02006530(void);
void Func_02006806(void);
void Func_020067ba(s32, s32, s32, s32, s32, s32);
void Func_02001cfe(void);
void Func_02003286(void);
void Func_0200682e(void);

void SceneState_ApplySixRectsAfterFlag161(void)
{
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    Func_02005f50(0x161);
    x = 23;
    y = 8;
    Func_02005f1c(36, 8, 1, 3, x, y);
    a = 3;
    b = 1;
    Func_02005f18(36, 8, 23, 8, b, a);
    Func_02005f28(100, 8, 87, 8, b, a);
    x = 46;
    y = 55;
    Func_02005f54(53, 55, 3, 3, x, y);
    Func_02005f4c(53, 55, 46, 55, a, a);
    Func_02005f5c(117, 55, 110, 55, a, a);
}

void SceneState_RunActor13AtColumn42Setup(void)
{
    Struct_1644 *obj;
    s32 val;
    s32 a;
    s32 b;

    obj = Func_020064f8(13);
    Func_020064e6();
    if (obj->unk8 >> 20 == 42) {
        Func_020064ec(30);
        Func_02006662(188);
        obj->unk55 = 0;
        val = 0xfffe0000;
        obj->unk14 = val;
        obj->unkC = val;
        Func_020064f8_a(0x200);
        a = 3;
        b = 5;
        Func_020064ac(44, 117, 41, 117, a, b);
    }
    Func_02006530();
}

void SceneState_ApplyRectAt19_44AndRunThree(void)
{
    s32 x;
    s32 y;

    Func_02006806();
    x = 12;
    y = 44;
    Func_020067ba(19, 44, 4, 1, x, y);
    Func_02001cfe();
    Func_02003286();
    Func_0200682e();
}
