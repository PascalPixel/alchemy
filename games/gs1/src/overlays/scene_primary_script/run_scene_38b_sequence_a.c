#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001948();
void Func_02001b86();
s32 Func_02001b9c();
void Func_02001bac();
s32 Func_02001bb8();
s32 Func_02001bc0();
s32 Func_02001bc8();
s32 Func_02001bee();
s32 Func_02001bf8();
void Func_02001c14();
void Func_02001c1e();
void Func_02001c28();
void Func_02001c3c();
void Func_02001c48();
void Func_02001c54();
s32 Func_02001c70();
s32 Func_02001c7e();
s32 Func_02001c8a();
void Func_02001c94();
s32 Func_02001c96();
void Func_02001c9e();
void Func_02001ca8();
void Func_02001caa();
s32 Func_02001cb2();
s32 Func_02001cc2();
void Func_02001ccc();
s32 Func_02001cd0();
void Func_02001cd8();
s32 Func_02001cde();
s32 Func_02001cec();
void Func_02001d16();
void Func_02001d30();
void Func_02001d58();
void Func_02001d68();
void Func_02001dc8();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene38bSequenceA(void)
{
    u32 i;
    u8 *rec;
    s32 rec7;
    u8 *rec8;
    s32 record;
    s32 target;

    rec8 = Value1(Func_02001bb8, 10);
    rec = Value1(Func_02001bc0, 11);
    record = Func_02001bc8(8);
    Func_02001b86(record, 0);
    rec7 = Value1(Func_02001b9c, 0x845);
    if (rec7 != 0) {
        Func_02001c14(9, 0, 0);
        Func_02001c1e(10, 0, 0);
        Func_02001c28(11, 0, 0);
        Call6(Func_02001bac, 56, 15, 40, 15, 1, 2);
        Call6(Func_02001bc8, 26, 15, 1, 3, 10, 15);
        if (Value1(Func_02001bee, 0x849) == 0) {
            if (Value1(Func_02001bf8, 0x848) != 0) {
                goto L_02000c92;
            }
            ((void (*)())Func_02001c70)(14, 0, 0);
        }
        Call3(Func_02001ccc, 12, 0xd000, 0);
        Call3(Func_02001cd8, 13, 0xb000, 0);
    } else {
        Func_02001c94(12, 0, 0);
        Func_02001c9e(13, 0, 0);
        Func_02001ca8(14, 0, 0);
        record = Func_02001c7e(9);
        Func_02001c3c(record, 0);
        record = Func_02001c8a(10);
        Func_02001c48(record, 0);
        record = Func_02001c96(11);
        Func_02001c54(record, 0);
        target = (s32)rec8 + 85;
        *(u8 *)target = rec7;
        record = Value1(Func_02001c70, 0x881);
        if (record != 0) {
            *(u8 *)(Func_02001cb2(9) + 89) |= 16;
            *(u8 *)(Func_02001cc2(16) + 89) |= 16;
            *(u8 *)(Func_02001cd0(11) + 89) |= 16;
            Call3(Func_02001d16, 16, 0x8e0000, 0x9c0000);
            record = Func_02001cec(16);
            Func_02001caa(record, 0);
            Call3(Func_02001d30, 10, 0x8e0000, 0x9c0000);
            {
                s32 shown;
            
                target = *(volatile s32 *)((s32)rec8 + 80);
                shown = 0x4000;
                *(volatile u16 *)(target + 30) = shown;
            }
            *(volatile s32 *)((s32)rec8 + 12) += -0x80000;
            if (Value1(Func_02001cde, 0x848) != 0) {
                Call3(Func_02001d58, 11, 0x840000, 0xba0000);
                goto L_02000c92;
            }
            Call3(Func_02001d68, 11, 0x580000, 0xc40000);
            Func_02001dc8(11, 3);
            rec[89] |= 4;
        } else {
            *(volatile s32 *)((s32)rec8 + 12) = 0x200000;
            rec[85] = record;
            *(volatile s32 *)((s32)rec + 12) = 0x300000;
        }
    }
    L_02000c92:;
    Func_02001948();
}
