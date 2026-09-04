#include "types.h"

#define FieldScene_RunScene3b1SequenceB Func_02000cc8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001e72();
s32 Func_02001fb8();
void Func_0200555a();
void Func_020055be();
void Func_0200564a();
s32 Func_02007174();
void Func_02007194();
s32 Func_020071e4();
void Func_02007226();
s32 Func_02007228();
s32 Func_02007230();
void Func_02007232();
s32 Func_02007242();
void Func_02007246();
s32 Func_02007248();
s32 Func_02007254();
void Func_02007266();
void Func_0200726e();
void Func_0200728a();
void Func_02007290();
void Func_020072aa();
void Func_020072b2();
void Func_020072b8();
void Func_020072ca();
void Func_020072ce();
s32 Func_020072f0();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b1SequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Func_02007194();
    if (Value1(Func_02007174, 0x300) != 0) {
        rec7 = Value0(Func_02001fb8);
        Func_02001e72();
        Call1(Func_02007290, 0x1ea2);
        Func_0200555a(9);
        Func_02007246(rec7, 2);
        record = Value1(Func_020071e4, 0);
        if (record != 0) {
            Func_02007226(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ((void (*)())Func_02007254)(rec7);
        Func_02007266(rec7, 0, 0);
    } else {
        Call1(Func_020072ce, 0x1e84);
        ((void (*)())Func_020072f0)(9, 0, 60);
        Func_020072b8(9, 1);
        Value2(Func_020072f0, 9, 0);
        if (Value2(Func_02007228, 0, 0) == 0) {
            Func_020055be(9);
            Func_020072aa(9, 2);
            record = Value1(Func_02007248, 0);
            if (record != 0) {
                Func_0200728a(9, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Func_020072b8(9);
            Func_020072ca(9, 0, 0);
            Call1(Func_02007232, 0x300);
            if (Value1(Func_02007230, 0x92b) != 0) {
                Call1(Func_02007242, 0x991);
                goto L_02000de0;
            }
            if (Value1(Func_02007242, 0x92a) != 0) {
                Call1(Func_02007254, 0x918);
                goto L_02000de0;
            }
            if (Value1(Func_02007254, 0x929) != 0) {
                Call1(Func_02007266, 0x936);
                goto L_02000de0;
            }
            Call1(Func_0200726e, 0x92d);
        } else {
            bump_step(1);
            Func_0200564a(9);
        }
    }
    L_02000de0:;
    Func_020072b2();
}
