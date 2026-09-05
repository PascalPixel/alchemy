#include "types.h"

#define FieldScene_RunScene37fSequenceA Func_020017c0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_0200333c();
void Func_0200334e();
void Func_02003360();
void Func_02003372();
void Func_02003384();
void Func_02003396();
void Func_020033b0();
s32 Func_020033c4();
void Func_020033e0();
s32 Func_020033f4();
void Func_02003410();
s32 Func_02003420();
void Func_02003442();
s32 Func_02003452();
void Func_0200345a();
void Func_02003472();
s32 Func_02003482();
void Func_0200348a();
void Func_020034a6();
s32 Func_020034b6();
void Func_020034ba();
void Func_020034d6();
s32 Func_020034e6();
void Func_020034ec();
void Func_02003508();
s32 Func_02003518();
void Func_0200351c();
void Func_02003538();
s32 Func_02003548();
void Func_02003550();
void Func_0200356c();
s32 Func_0200357c();
void Func_02003580();
void Func_0200359c();
s32 Func_020035ac();
void Func_020035b2();
void Func_020035ce();
s32 Func_020035de();
void Func_020035e2();
void Func_02003616();
void Func_02003646();
void Func_02003678();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene37fSequenceA(s32 a0)
{
    u32 i;
    s32 record;

    Value6(Func_0200333c, 122, 20, 1, 1, 100, 32);
    Call6(Func_0200334e, 122, 20, 1, 1, 104, 32);
    Call6(Func_02003360, 122, 20, 1, 1, 108, 32);
    Call6(Func_02003372, 122, 20, 1, 1, 112, 32);
    Call6(Func_02003384, 122, 20, 1, 1, 116, 32);
    Call6(Func_02003396, 122, 20, 1, 1, 120, 32);
    if (Value1(Func_020033c4, 0x311) != 0) {
        Call6(Func_020033b0, 121, 20, 1, 1, 100, 32);
        if (a0 != 0) {
            Call3(Func_0200345a, 9, 0x6380000, 0x2080000);
        }
    } else {
        if (Value1(Func_020033f4, 0x310) != 0) {
            Call6(Func_020033e0, 121, 20, 1, 1, 100, 32);
            if (a0 != 0) {
                Call3(Func_0200348a, 9, 0x6580000, 0x2080000);
            }
        }
    }
    if (Value1(Func_02003420, 0x313) != 0) {
        Call6(Func_02003410, 121, 20, 1, 1, 104, 32);
        if (a0 != 0) {
            Call3(Func_020034ba, 10, 0x6780000, 0x2080000);
        }
    } else {
        if (Value1(Func_02003452, 0x312) != 0) {
            Call6(Func_02003442, 121, 20, 1, 1, 104, 32);
            if (a0 != 0) {
                Call3(Func_020034ec, 10, 0x6980000, 0x2080000);
            }
        }
    }
    if (Value1(Func_02003482, 0x315) != 0) {
        Call6(Func_02003472, 121, 20, 1, 1, 108, 32);
        if (a0 != 0) {
            Call3(Func_0200351c, 11, 0x6b80000, 0x2080000);
        }
    } else {
        if (Value1(Func_020034b6, 0x314) != 0) {
            Call6(Func_020034a6, 121, 20, 1, 1, 108, 32);
            if (a0 != 0) {
                Call3(Func_02003550, 11, 0x6d80000, 0x2080000);
            }
        }
    }
    if (Value1(Func_020034e6, 0x317) != 0) {
        Call6(Func_020034d6, 121, 20, 1, 1, 112, 32);
        if (a0 != 0) {
            Call3(Func_02003580, 12, 0x6f80000, 0x2080000);
        }
    } else {
        if (Value1(Func_02003518, 0x316) != 0) {
            Call6(Func_02003508, 121, 20, 1, 1, 112, 32);
            if (a0 != 0) {
                Call3(Func_020035b2, 12, 0x7180000, 0x2080000);
            }
        }
    }
    if (Value1(Func_02003548, 0x319) != 0) {
        Call6(Func_02003538, 121, 20, 1, 1, 116, 32);
        if (a0 != 0) {
            Call3(Func_020035e2, 13, 0x7380000, 0x2080000);
        }
    } else {
        if (Value1(Func_0200357c, 0x318) != 0) {
            Call6(Func_0200356c, 121, 20, 1, 1, 116, 32);
            if (a0 != 0) {
                Call3(Func_02003616, 13, 0x7580000, 0x2080000);
            }
        }
    }
    if (Value1(Func_020035ac, 0x31b) != 0) {
        Call6(Func_0200359c, 121, 20, 1, 1, 120, 32);
        if (a0 != 0) {
            Call3(Func_02003646, 14, 0x7780000, 0x2080000);
        }
    } else {
        if (Value1(Func_020035de, 0x31a) != 0) {
            Call6(Func_020035ce, 121, 20, 1, 1, 120, 32);
            if (a0 != 0) {
                Call3(Func_02003678, 14, 0x7980000, 0x2080000);
            }
        }
    }
}
