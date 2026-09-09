#include "types.h"

#define SceneState_Send210AndApplyRectAt40x84 Func_02000d70
#define SceneState_Send210AndApplyRect Func_02000d98
#define SceneState_ApplyFlag801Branch Func_02000df0
#define SceneState_SetValue123Mode3 Func_02000e18
#define SceneState_SetValue123Mode4 Func_02000e2c
#define SceneState_ApplyValues123And2 Func_02000e40
#define SceneState_RunFlag204Step Func_02001244
#define SceneState_RunTablePairWhenActor22State1 Func_02001454
#define SceneState_RunTablePairWhenActor22State2 Func_02001554
#define SceneState_SetValue1ThenCall Func_020055b0
#define SceneState_SetValue0ThenCall Func_020055c0
#define SceneState_SetValue24ThenCall Func_020055f0
#define SceneState_SetValue25ThenCall Func_02005600
#define SceneState_ApplyRectAndRunTwo Func_02005928
extern s32 Data_0200f63c[];
extern s32 Data_0200f6cc[];
extern s32 Data_0200f748[];
extern s32 Data_0200f7c4[];

void Func_02006d1e(s32);
void Func_02006cca(s32, s32, s32, s32, s32, s32);
void Func_02006d4e(s32);
void Func_02006cf2(s32, s32, s32, s32, s32, s32);
s32 Func_02006d92(s32);
void Func_02001dea(void);
void Func_02006f52(s32);
void Func_02006f08(s32);
void Func_02006f6a(s32);
void Func_02006f20(s32);
void Func_02006f7e(s32);
void Func_02006f34(s32);
void Func_02006f92(s32);
void Func_02006f48(s32);
void Func_02007206(void);
void Func_0200719a(s32, s32, s32, s32, s32, s32);
s32 Func_0200451a(s32, s32, s32, s32);
void Func_02007216(s32);
void Func_0200723a(void);
u8 *Func_0200743e(s32);
s32 Func_020073fe(s32);
void Func_0200290a(s32, s32);
u8 *Func_0200753e(s32);
s32 Func_020074fe(s32);
void Func_02002a0a(s32, s32);
void Func_0200b59a(s32);
void Func_0200b216(void);
void Func_0200b5aa(s32);
void Func_0200b226(void);
void Func_0200b5da(s32);
void Func_0200b292(void);
void Func_0200b5ea(s32);
void Func_0200b2a2(void);
void Func_0200b87a(s32, s32, s32, s32, s32, s32);
void Func_02005a06(void);
void Func_0200b296(void);

void SceneState_Send210AndApplyRectAt40x84(void)
{
    s32 m, n;
    Func_02006d1e(0x210);
    m = 10;
    n = 84;
    Func_02006cca(40, 84, 7, 4, m, n);
}

void SceneState_Send210AndApplyRect(void)
{
    s32 m, n;
    Func_02006d4e(0x210);
    m = 10;
    n = 84;
    Func_02006cf2(40, 89, 7, 4, m, n);
}

void SceneState_ApplyFlag801Branch(void)
{
    if (Func_02006d92(0x801) == 0) {
        Func_02001dea();
    } else {
        Func_02006f52(123);
        Func_02006f08(1);
    }
}

void SceneState_SetValue123Mode3(void)
{
    Func_02006f6a(123);
    Func_02006f20(3);
}

void SceneState_SetValue123Mode4(void)
{
    Func_02006f7e(123);
    Func_02006f34(4);
}

void SceneState_ApplyValues123And2(void)
{
    Func_02006f92(123);
    Func_02006f48(2);
}

void SceneState_RunFlag204Step(void)
{
    s32 m, n;
    Func_02007206();
    m = 20;
    n = 50;
    Func_0200719a(49, 53, 8, 4, m, n);
    Func_0200451a(0, 10, 11, 1);
    Func_02007216(0x204);
    Func_0200723a();
}

void SceneState_RunTablePairWhenActor22State1(void)
{
    u8 *p = Func_0200743e(22);
    if (Func_020073fe(0x823) != 0) {
        u8 *q = p;
        q += 100;
        if (*(s16 *)q == 1) {
            Func_0200290a((s32)Data_0200f63c, (s32)Data_0200f6cc);
        }
    }
}

void SceneState_RunTablePairWhenActor22State2(void)
{
    u8 *p = Func_0200753e(22);
    if (Func_020074fe(0x823) != 0) {
        u8 *q = p;
        q += 100;
        if (*(s16 *)q == 2) {
            Func_02002a0a((s32)Data_0200f748, (s32)Data_0200f7c4);
        }
    }
}

void SceneState_SetValue1ThenCall(void)
{
    Func_0200b59a(1);
    Func_0200b216();
}

void SceneState_SetValue0ThenCall(void)
{
    Func_0200b5aa(0);
    Func_0200b226();
}

void SceneState_SetValue24ThenCall(void)
{
    Func_0200b5da(0x18);
    Func_0200b292();
}

void SceneState_SetValue25ThenCall(void)
{
    Func_0200b5ea(0x19);
    Func_0200b2a2();
}

void SceneState_ApplyRectAndRunTwo(void)
{
    s32 e = 22;
    s32 f = 36;
    Func_0200b87a(17, 0, 3, 1, e, f);
    Func_02005a06();
    Func_0200b296();
}
