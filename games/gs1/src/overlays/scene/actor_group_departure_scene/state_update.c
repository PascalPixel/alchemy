#include "types.h"

#define SceneState_SetFlag210AndConfigureRegion40_89 Func_02000134
#define SceneState_SetWork1c0AndRunObject Func_0200015c
#define SceneState_SetValue123Mode1 Func_020001d0
#define SceneState_ApplyValues123And3 Func_020001e4
#define SceneState_SetValue123Mode4 Func_020001f8
#define SceneState_ApplyFourRects Func_020012f0
#define SceneState_SetWords1c0And1c8AndRun Func_02004234
#define SceneState_SetWorkWordsAndFlag87f Func_02004278
#define SceneState_SetValue140Mode0 Func_02004550
#define SceneState_SetValue19ThenCall Func_020045a8
#define SceneState_SetValueEe4 Func_020045fc
extern u8 Value_00000834;

void Func_0200482a(s32);
void Func_020047fe(s32, s32, s32, s32, s32, s32);
s32 Func_02004840(s32);
void Func_020049c8(void);
void Func_020049c4(u8 *);
void Func_02004a8a(s32);
void Func_02000338(s32);
void Func_02004a9e(s32);
void Func_0200034c(s32);
void Func_02004ab2(s32);
void Func_02000360(s32);
void Func_020059b2(s32, s32, s32, s32, s32, s32);
void Func_020059c4(s32, s32, s32, s32, s32, s32);
void Func_020059d6(s32, s32, s32, s32, s32, s32);
void Func_020059e8(s32, s32, s32, s32, s32, s32);
void Func_02008934(void);
void Func_02008938(s32);
s32 Func_02008aa0(s32, s32);
void Func_02008948(s32);
void Func_0200896c(void);
void Func_02008978(void);
void Func_0200897c(s32);
s32 Func_02008ae4(s32, s32);
void Func_0200898c(s32);
void Func_020089b0(void);
void Func_02008dec(s32, s32);
void Func_02008cda(s32);
void Func_020088fe(void);
void Func_02008cfc(void);
void Func_02008cdc(s32, s32);
void Func_02008d10(void);

void SceneState_SetFlag210AndConfigureRegion40_89(void)
{
    s32 a;
    s32 b;

    Func_0200482a(0x210);
    a = 10;
    b = 84;
    Func_020047fe(40, 89, 7, 4, a, b);
}

void SceneState_SetWork1c0AndRunObject(u8 *o)
{
    u8 *state;

    if (Func_02004840((s32)&Value_00000834) != 0) {
        Func_020049c8();
    }
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x100;
    *(s32 *)(state + 0x1C8) = 16;
    Func_020049c4(o);
}

void SceneState_SetValue123Mode1(void)
{
    Func_02004a8a(0x7B);
    Func_02000338(1);
}

void SceneState_ApplyValues123And3(void)
{
    Func_02004a9e(0x7B);
    Func_0200034c(3);
}

void SceneState_SetValue123Mode4(void)
{
    Func_02004ab2(0x7B);
    Func_02000360(4);
}

void SceneState_ApplyFourRects(void)
{
    s32 a = 0x2a;
    s32 b;

    Func_020059b2(29, 22, 1, 1, 3, a);
    b = 2;
    Func_020059c4(29, 21, 1, 1, b, a);
    Func_020059d6(29, 21, 1, 1, 4, a);
    Func_020059e8(23, 20, 3, 1, b, 0x2b);
}

void SceneState_SetWords1c0And1c8AndRun(void)
{
    u8 *state;

    Func_02008934();
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x200;
    *(s32 *)(state + 0x1C8) = 64;
    Func_02008938(0x87c);
    Func_02008aa0(12, 2);
    Func_02008948(0x900);
    Func_0200896c();
}

void SceneState_SetWorkWordsAndFlag87f(void)
{
    u8 *state;

    Func_02008978();
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x200;
    *(s32 *)(state + 0x1C8) = 64;
    Func_0200897c(0x87f);
    Func_02008ae4(12, 3);
    Func_0200898c(0x900);
    Func_020089b0();
}

void SceneState_SetValue140Mode0(void)
{
    Func_02008dec(140, 0);
}

void SceneState_SetValue19ThenCall(void)
{
    Func_02008cda(19);
    Func_020088fe();
}

void SceneState_SetValueEe4(void)
{
    Func_02008cfc();
    Func_02008cdc(0xee4, 1);
    Func_02008d10();
}
