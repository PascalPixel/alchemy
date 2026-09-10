#include "types.h"

#define SceneState_ApplyPair12And21 Func_02000cc0
#define SceneState_ApplyWork16cMinus50A Func_02001060
#define SceneState_ApplyWork16cMinus50 Func_02001080
#define SceneState_ApplyWork16cMinus50B Func_020010a0
#define SceneState_RunWhenActor8AtTile10x23 Func_02001bf0
#define SceneState_ApplyRectWhenActor20AtColumn28 Func_02002600
extern u8 *Data_03001ebc;
extern u8 Data_0200e064[];

void Func_0200689c(s32, s32);
void Func_02001ff8(s32);
void Func_02002018(s32);
void Func_02002038(s32);
s32 *Func_020076cc(s32);
s32 *Func_020076de(s32);
void Func_020076ce(void);
s32 *Func_020076fc(s32);
s32 *Func_0200770a(s32);
s32 *Func_02007712(s32);
void Func_02007658(s32, s32, s32, s32, s32, s32);
void Func_020078be(s32);
void Func_02007660(s32, s32, s32);
void Func_02007692(s32, s32, s32, s32, s32, s32);
void Func_02007708(s32);
void Func_0200773c(void);
void Func_020080ba(void);
s32 *Func_020080e0(s32);
void Func_020080b0(s32);
void Func_02008054(s32, s32, s32, s32, s32, s32);
void Func_020080f8(void);

/*
 * resource_39c owner at 0x02000cc0, 16 bytes: invoke one fixed two-value
 * setup call with 12 and 21.
 *
 * The executable owner runs from `push {lr}` at 0x02000cc0 through the
 * void interworking return `pop {r0} / bx r0` at 0x02000cca-0x02000ccc.
 * m2c_guard independently measures those 14 code bytes and finds no
 * seeding hazard.  The zero halfword at 0x02000cce is alignment before
 * the next owner's prologue at 0x02000cd0, making the whole span 16 bytes.
 * Its sole call resolves through the overlay veneer to Func_0808a258.
 * The constants' game-level meanings are not established by this owner.
 */

void SceneState_ApplyPair12And21(void)
{
    Func_0200689c(12, 21);
}

void SceneState_ApplyWork16cMinus50A(void)
{
    Func_02001ff8(*(s16 *)(Data_03001ebc + 0x16c) - 50);
}

void SceneState_ApplyWork16cMinus50(void)
{
    Func_02002018(*(s16 *)(Data_03001ebc + 0x16c) - 50);
}

void SceneState_ApplyWork16cMinus50B(void)
{
    Func_02002038(*(s16 *)(Data_03001ebc + 0x16c) - 50);
}

void SceneState_RunWhenActor8AtTile10x23(void)
{
    s32 x = Func_020076cc(8)[2] / 0x100000;
    s32 y = Func_020076de(8)[4] / 0x100000;

    Func_020076ce();
    if (x == 10 && y == 23) {
        s32 *p;
        Func_020076fc(8)[3] += 0xfffe0000;
        p = Func_0200770a(8);
        p[15] = Func_02007712(8)[3];
        Func_02007658(6, 29, 10, 23, 1, 1);
        Func_020078be(0xd9);
        Func_02007660((s32)Data_0200e064, 10, 18);
        Func_02007692(10, 16, 1, 1, x, 19);
        Func_02007708(0x878);
    }
    Func_0200773c();
}

void SceneState_ApplyRectWhenActor20AtColumn28(void)
{
    s32 col;

    Func_020080ba();
    col = Func_020080e0(20)[2] / 0x100000;
    if (col == 28) {
        Func_020080b0(840);
        {
            s32 a = 31;
            s32 b = 20;

            Func_02008054(29, 20, 1, 1, a, b);
        }
    }
    Func_020080f8();
}
