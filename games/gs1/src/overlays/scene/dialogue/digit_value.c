#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/digit_value.h"

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
    ret = Talk_Run2();
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

    Talk_Run3(4);
    Talk_Run4(512);
    Talk_Run5(0x203);
    Talk_Run6();

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

    Talk_Run7(0x2927);
    Talk_Run8(8, 0);
    Talk_Run9(0x205);
    return Talk_Check();
}

s32 SceneData_CopyUpToThreeEntries(u16 *dest)
{
    s32 cnt = Talk_Check2();
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
    Talk_Do2(arg0);
    return Talk_Check3();
}

/* Deliberate no-op callback. */
void State_NoOp(void) {}

u8 *SceneData_GetTable9e14(void) { return (u8 *)0x02009e14; }

s32 Scene_ShowDialoguePair292a(void)
{
    s32 handle;
    Talk_Run10(85);
    handle = Talk_Check4(0x292a, 5, 4, 1);
    while (Talk_Check5() == 0)
        Talk_Run11(1);
    Talk_Run12();
    Talk_Run13(handle, 1);
    Talk_Run14(1);
    handle = Talk_Check6(0x292b, 5, 4, 1);
    while (Talk_Check7() == 0)
        Talk_Run15(1);
    return Talk_Check8(handle, 1);
}

s32 Scene_ShowDialoguePair292c(void)
{
    s32 handle;
    Talk_Run16(85);
    handle = Talk_Check9(0x292c, 5, 4, 1);
    while (Talk_Check10() == 0)
        Talk_Run17(1);
    Talk_Run18();
    Talk_Run19(handle, 1);
    Talk_Run20(1);
    handle = Talk_Check11(0x292d, 5, 4, 1);
    while (Talk_Check12() == 0)
        Talk_Run21(1);
    return Talk_Check13(handle, 1);
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

    return Talk_Check14();
}
