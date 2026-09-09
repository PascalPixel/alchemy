#include "types.h"

extern s16 Data_02000240[];
extern u8 Data_02009c04[];
extern u8 Data_020099f4[];

u32 Func_02001796(void);
void Func_0200179a(void);
/*
 * Declared old-style: two of these names can be the same import, and the
 * arity varies per call site, so the interfaces are left open.
 */
void Func_02000472();
void Func_02001b6e();
void Func_02001b74();
void Func_02001ba0();
void Func_02001bf0();
void Func_02001c00();
void Func_02001b96();
s32 Func_02001bca();
s32 Func_02001d76(void);
void Func_02002762(s32);
void Func_020027f0(s32);
s32 Func_02002774(void);
void Func_02002aa6();
s32 Func_02002992();
void Func_020028ec();
s32 Func_020029a8();
void Func_020029d8();
s32 Func_020029d8_a();
void Func_020029a0();
void Func_02002906();
s32 Func_020029c2();
void Func_0200291c();
s32 Func_020029cc();
void Func_02002b12();
s32 Func_020029fe();
void Func_02002958();
s32 Func_02002a14();
void Func_02002a44();
s32 Func_02002a44_a();
void Func_02002a0c();
void Func_02002972();
s32 Func_02002a2e();
void Func_02002988();
s32 Func_02002a38();
s32 Func_020029b8(s32, s32);
void Func_02002a4a(s32, s32, s32, s32, s32, s32);
s32 Func_020029ca(s32, s32);
s32 Func_02002a56(void);

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

u32 SceneState_RunQueryWithInterruptMasterSaved(void)
{
    volatile u16 *ime = (volatile u16 *)0x04000208;
    u32 saved = *ime;
    u32 ret;

    *ime = (u16)(u32)ime;
    Func_0200179a();
    ret = Func_02001796();
    *ime = saved;
    return ret;
}

u8 *SceneData_GetTable99f0(void) { return (u8 *)0x020099f0; }

s32 SceneData_SelectTable9c04ByState(void)
{
    s16 v = Data_02000240[225];

    if (v == 11 || v == 9) {
        return (s32)Data_02009c04;
    }
    return (s32)Data_020099f4;
}

/*
 * Scene teardown: reset one workspace field, clear three flags, play cue
 * 0x2927 and return the last call's result -- the epilogue pops the return
 * address into r1, so r0 survives and is the result. The 88-byte owner
 * includes its alignment bytes and four pool words, one of which is
 * 0x03001ebc -- the IWRAM workspace-pointer cell, not an in-image address.
 */
s32 FieldScene_ClearFlagsAndPlayCue2927(void)
{
    u16 *work = *(u16 **)0x03001ebc;

    Func_02000472(4);
    Func_02001b6e(512);
    Func_02001b74(0x203);
    Func_02001ba0();

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

    Func_02001bf0(0x2927);
    Func_02001c00(8, 0);
    Func_02001b96(0x205);
    return Func_02001bca();
}

s32 SceneData_CopyUpToThreeEntries(u16 *dest)
{
    s32 cnt = Func_02001d76();
    if (cnt > 3) cnt = 3;
    if (cnt > 0) {
        s16 *p = Data_02000240;
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

s32 SceneState_ApplyValueAndGetResult(s32 arg0)
{
    Func_02002762(arg0);
    Func_020027f0(arg0);
    return Func_02002774();
}

/* Deliberate no-op callback. */
void SceneState_NoOp(void) {}

u8 *SceneData_GetTable9e14(void) { return (u8 *)0x02009e14; }

s32 FieldScene_ShowDialoguePair292a(void)
{
    s32 handle;
    Func_02002aa6(85);
    handle = Func_02002992(0x292a, 5, 4, 1);
    while (Func_020029a8() == 0)
        Func_020028ec(1);
    Func_020029d8();
    Func_020029a0(handle, 1);
    Func_02002906(1);
    handle = Func_020029c2(0x292b, 5, 4, 1);
    while (Func_020029d8_a() == 0)
        Func_0200291c(1);
    return Func_020029cc(handle, 1);
}

s32 FieldScene_ShowDialoguePair292c(void)
{
    s32 handle;
    Func_02002b12(85);
    handle = Func_020029fe(0x292c, 5, 4, 1);
    while (Func_02002a14() == 0)
        Func_02002958(1);
    Func_02002a44();
    Func_02002a0c(handle, 1);
    Func_02002972(1);
    handle = Func_02002a2e(0x292d, 5, 4, 1);
    while (Func_02002a44_a() == 0)
        Func_02002988(1);
    return Func_02002a38(handle, 1);
}

s32 FieldScene_DrawThreeDigitValue(s32 value)
{
    s32 col;

    if (value > 999) {
        value = 999;
    }

    for (col = 0; col <= 2; col++) {
        s32 digit = Func_020029b8(value, 10);

        Func_02002a4a(27, digit, 16 - col, 8, 1, 1);
        value = Func_020029ca(value, 10);
    }

    return Func_02002a56();
}
