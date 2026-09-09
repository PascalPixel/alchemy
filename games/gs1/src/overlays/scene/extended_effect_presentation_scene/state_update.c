#include "types.h"

#define SceneState_InitStateWordsAndSlots Func_02001084
#define SceneState_UpdateRandomTimerLevel Func_0200135c
#define SceneState_ApplyRectsByCondition Func_0200284c
#define SceneState_ApplyRectPairByFlag Func_020028dc
#define FieldScene_RunRandomHalfBranch Func_0200293c
#define FieldScene_RunLateRandomHalfBranch Func_02002970
#define FieldScene_RunVariantStep Func_02002d48
#define FieldScene_RunStepByRuntimeBits Func_02002d94
#define SceneState_ForwardByRuntimeWordBits Func_02002dd0
#define SceneState_SetValue140Mode0 Func_02002ff0
extern s32 Data_0200bb68;
extern s32 Data_0200bb00;
extern s32 Data_0200bb6c;
extern s32 Data_0200bb70;
extern s32 Data_0200bac0[];
extern s32 Data_03001e40;

s32 Func_02004778(void);
s32 Func_0200479e(void);
s32 Func_020047b6(void);
s32 Func_020047ce(void);
s32 Func_020047e6(void);
void Func_02005cd8(s32, s32, s32, s32, s32, s32);
void Func_02005cea(s32, s32, s32, s32, s32, s32);
void Func_02005cfa(s32, s32, s32, s32, s32, s32);
void Func_02005d0e(s32, s32, s32, s32, s32, s32);
void Func_02005d1e(s32, s32, s32, s32, s32, s32);
void Func_02005d30(s32, s32, s32, s32, s32, s32);
void Func_02005d40(s32, s32, s32, s32, s32, s32);
void Func_02005d34(void);
void Func_02005d68(s32, s32, s32, s32, s32, s32);
void Func_02005d7a(s32, s32, s32, s32, s32, s32);
void Func_02005d8e(s32, s32, s32, s32, s32, s32);
void Func_02005da0(s32, s32, s32, s32, s32, s32);
void Func_02005d94(void);
s32 Func_02005d48(void);
u32 Func_02005d2e(s32, s32);
void Func_020051a8(s32);
void Func_020051b0(s32);
s32 Func_02005d7c(void);
u32 Func_02005d62(s32, s32);
void Func_0200526c(s32);
void Func_02005274(s32);
void Func_0200637c(s32);
void Func_0200634c(s32, s32);
void Func_0200638c(s32);
void Func_0200635e(s32, s32);
void Func_0200636c(s32);
void Func_0200624e(s32);
void Func_0200624c(s32, s32);
void Func_02006256(s32, s32);
void Func_02005c74(s32);
s32 Func_020061c4(u32, s32);
void Func_02006294(s32, s32);
void Func_02005cb0(s32);
void Func_0200660c(s32, s32);

void SceneState_InitStateWordsAndSlots(void)
{
    s32 *p;
    u32 i;

    Data_0200bb68 = 63;
    Data_0200bb00 = 0;
    Data_0200bb6c = 0;
    Data_0200bb70 = 120;
    p = Data_0200bac0;
    for (i = 0; i < 16; i++) {
        *p++ = 0;
    }
}

void SceneState_UpdateRandomTimerLevel(void)
{
    u32 v;

    if (Data_0200bb70 != 0) {
        Data_0200bb70--;
        return;
    }
    if (Data_0200bb6c != 0) {
        Data_0200bb6c--;
    } else {
        Data_0200bb6c = (u32)(Func_02004778() << 2) >> 16;
    }
    v = Data_0200bb6c;
    switch (v) {
    case 3:
        Data_0200bb68 = v;
        Data_0200bb70 = ((u32)(Func_0200479e() * 20) >> 16) + 40;
        break;
    case 2:
        Data_0200bb68 = 15;
        Data_0200bb70 = ((u32)(Func_020047b6() * 40) >> 16) + 80;
        break;
    case 1:
        Data_0200bb68 = 63;
        Data_0200bb70 = ((u32)(Func_020047ce() * 80) >> 16) + 160;
        break;
    default:
        Data_0200bb68 = 127;
        Data_0200bb70 = ((u32)(Func_020047e6() * 160) >> 16) + 320;
        break;
    }
}

void SceneState_ApplyRectsByCondition(s32 a)
{
    if (a != 0) {
        s32 x;
        s32 y;
        x = 1;
        Func_02005cd8(8, 47, 64, 7, x, x);
        y = 2;
        Func_02005cea(7, 48, 63, 8, y, x);
        Func_02005cfa(7, 49, 63, 9, y, x);
    } else {
        s32 x;
        x = 1;
        Func_02005d0e(56, 0, 64, 7, x, x);
        Func_02005d1e(56, 0, 63, 8, x, x);
        Func_02005d30(56, 0, 63, 9, 2, x);
        Func_02005d40(58, 25, 64, 8, x, x);
    }
    Func_02005d34();
}

void SceneState_ApplyRectPairByFlag(s32 a)
{
    if (a != 0) {
        s32 n;
        n = 2;
        Func_02005d68(9, 45, 65, 5, n, n);
        Func_02005d7a(11, 46, 67, 6, 1, n);
    } else {
        s32 n;
        n = 2;
        Func_02005d8e(89, 2, 65, 5, n, n);
        Func_02005da0(102, 32, 67, 6, 1, n);
    }
    Func_02005d94();
}

void FieldScene_RunRandomHalfBranch(void)
{
    if ((Data_03001e40 & 1) == 0) {
        if (Func_02005d2e(Func_02005d48(), 100) > 50) {
            Func_020051a8(1);
        } else {
            Func_020051b0(0);
        }
    }
}

void FieldScene_RunLateRandomHalfBranch(void)
{
    if ((Data_03001e40 & 1) == 0) {
        if (Func_02005d62(Func_02005d7c(), 100) > 50) {
            Func_0200526c(1);
        } else {
            Func_02005274(0);
        }
    }
}

void FieldScene_RunVariantStep(s32 a, s32 b, s32 c)
{
    if (a == 1) {
        Func_0200637c(0x134);
        Func_0200634c(0x203a52, 1);
    } else {
        Func_0200638c(0x121);
        Func_0200635e(0x10000, 1);
    }
    Func_0200636c(b);
    if (c != 0) {
        Func_0200624e(c);
    }
}

void FieldScene_RunStepByRuntimeBits(s32 a)
{
    if ((Data_03001e40 & 2) != 0) {
        Func_0200624c(a, 7);
    } else {
        Func_02006256(a, 0);
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02005c74(a);
    }
}

void SceneState_ForwardByRuntimeWordBits(s32 a)
{
    volatile u32 *p = (u32 *)0x03001e40;

    if (*p & 1) {
        Func_02006294(a, Func_020061c4(*p >> 1, 6));
    }
    if ((*p & 15) == 0) {
        Func_02005cb0(a);
    }
}

void SceneState_SetValue140Mode0(void)
{
    Func_0200660c(140, 0);
}
