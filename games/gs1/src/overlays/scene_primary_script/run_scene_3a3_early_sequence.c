#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
extern u8 Data_02009e70[];
void Func_020013e0();
void Func_020013f8();
s32 Func_0200140a();
void Func_0200140c();
void Func_02001422();
void Func_02001432();
void Func_0200144a();
void Func_0200145a();
void Func_0200145c();
void Func_02001470();
void Func_020014e2();
s32 Func_020014e8();
void Func_0200150e();
void Func_02001526();
void Func_0200152e();
void Func_02001538();
void Func_0200153a();
void Func_0200153e();
void Func_02001558();
void Func_02001598();
void Func_020015bc();
void Func_0200160c();
void Func_02001618();
void Func_02001624();

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

void FieldScene_RunEarlySequence(void)
{
    u32 i;
    u8 *work;
    u8 *record;
    s32 idx;
    u8 *tbl;
    s32 off;
    s32 o4;
    s32 a;
    s32 b;
    s32 c;

    work = *(u8 *volatile *)Data_03001ebc;
    Func_020013e0();
    for (i = 8; i <= 65; i++) {
        record = (u8 *)Value1(Func_0200140a, i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    tbl = Data_02009e70;
    idx = ((s32)((s32)(*(u16 *)(work + 0x16c) - 2) << 16) >> 16);
    off = idx << 3;
    o4 = off + 4;
    a = *(s16 *)(tbl + o4);
    b = *(s16 *)(tbl + o4 + 2);
    if (idx == 1) {
        Func_0200153e(188);
        Call6(Func_020013f8, 42, 33, a, b, 2, 2);
        c = a + 2;
        Call6(Func_0200140c, 42, 35, c, b, 2, 2);
        Func_0200144a(4);
        Call6(Func_02001422, 40, 33, a, b, 2, 2);
        Call6(Func_02001432, 40, 35, c, b, 2, 2);
        Func_02001470(4);
    } else {
        Func_02001598(158);
        if (idx == 3) {
            Call6(Func_0200145a, 33, 42, 8, 17, 1, 2);
        }
        Func_0200145c(*(s32 *)(tbl + off), a, b);
    }
    Call3(Func_020014e2, 0, 0x8000, 0x4000);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    *(u8 *)(Func_020014e8(0) + 85) = 0;
    Func_0200152e(0, 2);
    if (idx == 6) {
        Func_02001526(0, 2, 0);
    } else {
        if (idx != 1) {
            Call3(Func_0200153a, 0, 2, -4);
        } else {
            Func_020015bc(0, 2);
            Call3(Func_02001558, 0, 0, -4);
        }
    }
    Func_0200150e(10);
    Func_0200160c(*(s16 *)(work + 0x16c));
    Func_02001618();
    Func_02001624();
    Func_02001538();
}
