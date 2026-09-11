#include "types.h"
#include "scene.h"

extern s16 gCell[];
extern u8 gOv[];
extern u8 gOv2[];

/*
 * Declared old-style: two of these names can be the same import, and the
 * arity varies per call site, so the interfaces are left open.
 */

/*
 * Returns the in-image table address 0x02009948, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr. Published from the overlay header, not
 * reached by any call.
 */
u8 *SceneData_GetTable9948(void)
{
    return (u8 *)0x02009948;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u32 State_RunQueryWithInterruptMasterSaved(void)
{
    volatile u16 *ime = (volatile u16 *)0x04000208;
    u32 saved = *ime;
    u32 ret;

    *ime = (u16)(u32)ime;
    Talk_Run();
    ret = Talk_unk2_4();
    *ime = saved;
    return ret;
}

u8 *SceneData_GetTable99f0(void) { return (u8 *)0x020099f0; }

s32 SceneData_SelectTable9c04ByState(void)
{
    s16 v = gCell[225];

    if (v == 11 || v == 9) {
        return (s32)gOv;
    }
    return (s32)gOv2;
}

/*
 * Scene teardown: reset one workspace field, clear three flags, play cue
 * 0x2927 and return the last call's result -- the epilogue pops the return
 * address into r1, so r0 survives and is the result. The 88-byte owner
 * includes its alignment bytes and four pool words, one of which is
 * 0x03001ebc -- the IWRAM workspace-pointer cell, not an in-image address.
 */
s32 Scene_ClearFlagsAndPlayCue2927(void)
{
    u16 *work = *(u16 **)0x03001ebc;

    Talk_unk3_4(4);
    Talk_unk4_4(512);
    Talk_unk5_4(0x203);
    Talk_unk6_4();

    {
        /*
         * The halfword store goes through a pointer local and then a value
         * local, in that order. Storing the literal straight into the
         * halfword builds the constant in HImode and fetches it from the
         * literal pool, costing a pool word the reference does not have;
         * splitting the address out first also fixes which register holds
         * the address.
         */
        u16 *p = (u16 *)((u32)work + 386);
        s32 val = 0;
        *p = (u16)val;
    }

    Talk_unk7_4(0x2927);
    Talk_unk8_4(8, 0);
    Talk_unk9_4(0x205);
    return Talk_Check();
}

s32 SceneData_CopyUpToThreeEntries(u16 *dest)
{
    s32 cnt = Talk_unk2();
    if (cnt > 3) cnt = 3;
    if (cnt > 0) {
        s16 *p = gCell;
        const u8 *src;
        s32 n;
        p += 252;
        src = (const u8 *)p;
        n = cnt;
        do {
            u8 c = *src++;
            if (dest != 0) { *dest = (u16)c; dest++; }
            n--;
        } while (n != 0);
    }
    if (dest != 0) *dest = 0x00ff;
    return cnt;
}

s32 State_ApplyValueAndGetResult(s32 arg0)
{
    Talk_Do(arg0);
    Talk_unk2_2(arg0);
    return Talk_unk3();
}

/* Deliberate no-op callback. */
void State_NoOp(void) {}

u8 *SceneData_GetTable9e14(void) { return (u8 *)0x02009e14; }

s32 Scene_ShowDialoguePair292a(void)
{
    s32 handle;
    Talk_unk10_4(85);
    handle = Talk_unk4(0x292a, 5, 4, 1);
    while (Talk_unk5() == 0)
        Talk_unk11_4(1);
    Talk_unk12_4();
    Talk_unk13_4(handle, 1);
    Talk_unk14_4(1);
    handle = Talk_unk6(0x292b, 5, 4, 1);
    while (Talk_unk7() == 0)
        Talk_unk15_4(1);
    return Talk_unk8(handle, 1);
}

s32 Scene_ShowDialoguePair292c(void)
{
    s32 handle;
    Talk_unk16_4(85);
    handle = Talk_unk9(0x292c, 5, 4, 1);
    while (Talk_unk10() == 0)
        Talk_unk17_4(1);
    Talk_unk18_4();
    Talk_unk19_4(handle, 1);
    Talk_unk20_4(1);
    handle = Talk_unk11(0x292d, 5, 4, 1);
    while (Talk_unk12() == 0)
        Talk_unk21_4(1);
    return Talk_unk13(handle, 1);
}

s32 Scene_DrawThreeDigitValue(s32 value)
{
    s32 col;

    if (value > 999) {
        value = 999;
    }

    for (col = 0; col <= 2; col++) {
        s32 digit = Talk_Apply(value, 10);

        Talk_SetRect(27, digit, 16 - col, 8, 1, 1);
        value = Talk_Apply2(value, 10);
    }

    return Talk_unk14();
}
