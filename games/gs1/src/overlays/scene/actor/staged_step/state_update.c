#include "types.h"

#define SceneState_SetSelectorFlagWhenFacingC000 Func_020010b8
#define SceneActor_MarkSlot13AndSetFlag200 Func_02001984
#define SceneState_ApplyFourRectsAndSetActor8Byte85 Func_020019e8
#define SceneState_ApplyTwoRectsAtRow56 Func_02001c6c
#define SceneState_ApplyRectAndClearActor10Byte85 Func_02001ccc
u8 *Func_02003600(s32);
s32 Func_020035de(s32);
s32 Func_020035d8(s32);
void Func_020035fe(s32);
void Func_02002038(s32);
u8 *Func_02003ecc(s32);
void Func_02003e7a(s32, s32, s32, s32, s32, s32);
u8 *Func_02003eec(s32);
void Func_02003ed2(s32);
void Func_02003ed6(s32, s32, s32, s32, s32, s32);
void Func_02003ee8(s32, s32, s32, s32, s32, s32);
void Func_02003ef8(s32, s32, s32, s32, s32, s32);
void Func_02003f0a(s32, s32, s32, s32, s32, s32);
u8 *Func_02003f78(s32);
void Func_0200415a(s32, s32, s32, s32, s32, s32);
void Func_0200416c(s32, s32, s32, s32, s32, s32);
s32 Func_020041ba(s32, s32, s32, s32, s32, s32);
u8 *Func_02004228(s32);

/* Four sites of one import, so four names. */

void SceneState_SetSelectorFlagWhenFacingC000(s32 selector)
{
    u8 *slot = Func_02003600(0);
    s32 flag;

    if (*(u16 *)(slot + 6) != 0xC000) {
        return;
    }
    flag = selector + 2496;
    if (Func_020035de(flag)!= 0) {
        return;
    }
    if (Func_020035d8(244) == -1) {
        return;
    }
    Func_020035fe(flag);
    Func_02002038(0x100 | selector);
    Func_020035fe_a(244);
}

void SceneActor_MarkSlot13AndSetFlag200(void)
{
    u8 *slot = Func_02003ecc(13);
    s32 fifth = 40;
    s32 sixth = 55;

    Func_02003e7a(40, 54, 1, 1, fifth, sixth);
    if (slot != 0) {
        u8 *other = Func_02003eec(13) + 85;
        u8 *flags = slot + 35;

        *other = 0;
        *flags = 2;
    }
    Func_02003ed2(512);
}

void SceneState_ApplyFourRectsAndSetActor8Byte85(void)
{
    Func_02003ed6(57, 42, 1, 1, 40, 42);
    Func_02003ee8(57, 42, 1, 1, 41, 42);
    Func_02003ef8(58, 42, 1, 1, 42, 42);
    Func_02003f0a(62, 37, 3, 1, 37, 42);

    Func_02003f78(8)[85] = 1;
}

void SceneState_ApplyTwoRectsAtRow56(void)
{
    s32 base = 55;

    Func_0200415a(38, 56, 1, 1, 38, base);
    Func_0200416c(42, 56, 1, 1, 42, base);
}

void SceneState_ApplyRectAndClearActor10Byte85(void)
{
    s32 w = 42;
    s32 h = 55;
    u8 *p;

    Func_020041ba(40, 54, 1, 1, w, h);
    p = Func_02004228(10) + 85;
    *p = 0;
}
