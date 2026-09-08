#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. Func_02002b74, Func_02002bf4 and Func_02002cd4 each
 * reach two runtime targets from this owner, so each is declared twice with
 * an _a alias with its own runtime symbol. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_02000ffc();
void Func_02001ef0();
void Func_0200213e();
void Func_02002a42();
void Func_02002b26();
void Func_02002b2c();
s32 Func_02002b58();
void Func_02002b5a();
s32 Func_02002b6a();
s32 Func_02002b74();
s32 Func_02002b74_a();
s32 Func_02002b9a();
s32 Func_02002bac();
s32 Func_02002bca();
s32 Func_02002bd6();
s32 Func_02002bea();
s32 Func_02002bf4();
void Func_02002bf4_a();
void Func_02002c06();
void Func_02002c52();
void Func_02002c5e();
void Func_02002c6a();
void Func_02002c76();
s32 Func_02002ca4();
s32 Func_02002cb0();
s32 Func_02002cbc();
s32 Func_02002cc8();
s32 Func_02002cd4();
s32 Func_02002cd4_a();
s32 Func_02002cde();
void Func_02002ce2();
s32 Func_02002ce8();
void Func_02002cf0();
s32 Func_02002cf2();
s32 Func_02002cfc();
void Func_02002cfe();
s32 Func_02002d06();
s32 Func_02002d0e();
s32 Func_02002d16();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

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

s32 FieldScene_SelectActorPresentationBranch(void)
{
    s32 rec8;
    s32 record;

    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x209;
    Func_02002b26(1);
    Func_02002b2c(2);
    Call1(Func_02002b5a, 0x84b);
    if (Value1(Func_02002b58, 0x109) != 0) {
        Call1(Func_02002b74, 0x200);
    }
    if (Value1(Func_02002b6a, 0x84f) == 0) {
        record = Value1(Func_02002b74_a, 0x845);
        if (record != 0) {
            goto L_02000758;
        }
        if (Data_02000240_t[225][0] == 29) {
            Func_02000ffc();
            goto L_02000888;
        }
        if (Data_02000240_t[225][0] != 9) {
            goto L_02000888;
        }
        if (Value1(Func_02002b9a, 0x321) == 0) {
            goto L_02000888;
        }
        Func_02001ef0();
    } else {
        L_02000758:;
        rec8 = Value1(Func_02002bac, 0x84e);
        if (rec8 != 0) {
        } else {
            if (Data_02000240_t[225][0] == 29) {
                if (Value1(Func_02002bca, 0x85e) != 0) {
                    goto L_02000888;
                }
                record = Value1(Func_02002bd6, 0x845);
                if (record == 0) {
                    goto L_02000888;
                }
                Func_0200213e();
            } else {
                if (Data_02000240_t[225][0] == 28) {
                    if (Value1(Func_02002bea, 0x322) != 0) {
                        if (Value1(Func_02002bf4, 0x109) != 0) {
                            Call6(Func_02002bf4_a, 38, 55, 4, 1, 38, 45);
                            Call6(Func_02002c06, 42, 55, 4, 1, 38, 46);
                            Call3(Func_02002cd4, 21, 0x2680000, 0x2d80000);
                            Call3(Func_02002ce2, 22, 0x2780000, 0x2d80000);
                            Call3(Func_02002cf0, 23, 0x2880000, 0x2d80000);
                            Call3(Func_02002cfe, 24, 0x2980000, 0x2d80000);
                            record = Func_02002ca4(21);
                            Func_02002c52(record, 0);
                            record = Func_02002cb0(22);
                            Func_02002c5e(record, 0);
                            record = Func_02002cbc(23);
                            Func_02002c6a(record, 0);
                            record = Func_02002cc8(24);
                            Func_02002c76(record, 0);
                            *(u8 *)(Func_02002cd4_a(21) + 85) = rec8;
                            *(u8 *)(Func_02002cde(22) + 85) = rec8;
                            *(u8 *)(Func_02002ce8(23) + 85) = rec8;
                            *(u8 *)(Func_02002cf2(24) + 85) = rec8;
                            record = Func_02002cfc(21);
                            *(volatile s32 *)(record + 12) = -0x40000;
                            record = Value1(Func_02002d06, 22);
                            *(volatile s32 *)(record + 12) = -0x40000;
                            record = Value1(Func_02002d0e, 23);
                            *(volatile s32 *)(record + 12) = -0x40000;
                            record = Value1(Func_02002d16, 24);
                            *(volatile s32 *)(record + 12) = -0x40000;
                        } else {
                            Func_02002a42();
                        }
                    }
                }
            }
        }
    }
    L_02000888:;
    return 0;
}
