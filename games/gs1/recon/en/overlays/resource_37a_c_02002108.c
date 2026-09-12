#include "types.h"

#define FieldScene_RunScene37aSequenceG Func_02002108

extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];
void Func_0200354a();
void Func_020036f4();
void Func_020039a6();
s32 Func_0200481e();
void Func_02004a80();
s32 Func_02004ade();
s32 Func_02004ae8();
s32 Func_02004af2();
void Func_02004af6();
void Func_02004b06();
s32 Func_02004b0e();
s32 Func_02004b18();
s32 Func_02004b26();
void Func_02004b40();
s32 Func_02004b4e();
void Func_02004b50();
s32 Func_02004b76();
void Func_02004b8c();
s32 Func_02004b90();
void Func_02004ba6();
void Func_02004bb4();
void Func_02004bba();
void Func_02004bc0();
s32 Func_02004bc2();
void Func_02004bc6();
void Func_02004bc8();
void Func_02004bd0();
s32 Func_02004bd8();
s32 Func_02004bd8_a();
s32 Func_02004be8();
void Func_02004bea();
s32 Func_02004bf8();
void Func_02004bfa();
s32 Func_02004c08();
void Func_02004c0a();
void Func_02004c18();
void Func_02004c1a();
void Func_02004c1c();
void Func_02004c32();
void Func_02004c3a();
void Func_02004c44();
void Func_02004c50();
void Func_02004c56();
void Func_02004c66();
void Func_02004c76();
s32 Func_02004cac();
s32 Func_02004cac_a();
s32 Func_02004cb6();
void Func_02004cbe();
void Func_02004cd0();
void Func_02004ce2();
void Func_02004cf2();
void Func_02004d06();
void Func_02004d14();
void Func_02004d1a();
void Func_02004d96();
s32 Func_02004d9e();
void Func_02004da4();
void Func_02004da6();
void Func_02004dc6();
s32 Func_02004dce();
void Func_02004dd6();
void Func_02004df6();
s32 Func_02004dfe();
void Func_02004e06();
void Func_02004e26();
s32 Func_02004e2e();
void Func_02004e36();
void Func_02004e5a();
s32 Func_02004e5e();
void Func_02004e6c();
void Func_02004e7c();
void Func_02004e8e();
void Func_02004eb2();
s32 Func_02004ec8();
void Func_02004eea();
void Func_02004eee();
s32 Func_02004f02();
void Func_02004f14();
void Func_02004f24();
void Func_02005020();
void Func_02005044();

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 Func_02002108(void)
{
    u32 i;
    s32 record;
    s32 v5;

    Func_02004a80(1);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x204;
    if (Value1(Func_02004ade, 0x809) != 0) {
        if (Value1(Func_02004ae8, 0x814) == 0) {
            if (Value1(Func_02004af2, 0x819) == 0) {
                Call1(Func_02004b06, 0x144);
            }
        }
    }
    Call2(Func_02004c18, 0x10000, 0);
    if (Value1(Func_02004b0e, 0x109) != 0) {
        if (Value1(Func_02004b18, 0x201) != 0) {
        } else {
            record = Value1(Func_02004b26, 0x202);
            if (record == 0) {
                goto L_020021ae;
            }
        }
        Call2(Func_02004c3a, 0x2051cc, 1);
        Func_02004c50(1);
        Func_02004af6(1);
    } else {
        Call1(Func_02004b50, 0x200);
        if (Value1(Func_02004b4e, 0x80a) != 0) {
            Call3(Func_02004bc8, 16, 0x2400000, 0x780000);
        }
    }
    L_020021ae:;
    if (Data_02000240_t[225][0] == 4) {
        record = Value1(Func_02004b76, 0x813);
        if (record != 0) {
            v5 = 0;
            goto L_02002220;
        }
        Func_0200354a();
        Call1(Func_02004b8c, 0x813);
    } else {
        if (Data_02000240_t[225][0] == 5) {
            record = Value1(Func_02004b90, 0x812);
            if (record != 0) {
                v5 = 0;
                goto L_02002220;
            }
            Func_020036f4();
            Call1(Func_02004ba6, 0x812);
            Call1(Func_02004bb4, 0x80b);
            Call1(Func_02004bba, 0x80c);
            Call1(Func_02004bc0, 0x80d);
            Call1(Func_02004bc6, 0x80e);
        } else {
            if (Data_02000240_t[225][0] != 6) {
                v5 = 0;
                goto L_02002220;
            }
            if (Value1(Func_02004bc2, 0x812) != 0) {
                Func_020039a6();
                Call1(Func_02004bd8, 0x822);
            }
        }
    }
    v5 = 1;
    L_02002220:;
    if (Value1(Func_02004bd8_a, 0x80b) != 0) {
        Call1(Func_02004bea, 0x826);
    }
    if (Value1(Func_02004be8, 0x80c) != 0) {
        Call1(Func_02004bfa, 0x827);
    }
    if (Value1(Func_02004bf8, 0x80d) != 0) {
        Call1(Func_02004c0a, 0x828);
    }
    if (Value1(Func_02004c08, 0x80e) != 0) {
        Call1(Func_02004c1a, 0x829);
    }
    Func_02004bd0(4);
    if (v5 != 0) {
    } else {
        if (Value0(Func_0200481e) == 0) {
        } else {
            Call6(Func_02004c1c, 30, 44, 30, 38, 12, 5);
            Call6(Func_02004c32, 30, 44, 34, 37, 4, 1);
            Call6(Func_02004c44, 14, 41, 32, 41, 8, 4);
            Call6(Func_02004c56, 45, 28, 34, 10, 4, 2);
            Call6(Func_02004c66, 45, 30, 16, 10, 4, 2);
            Call6(Func_02004c76, 14, 45, 14, 41, 8, 4);
            if (Data_02000240_t[225][0] != 8) {
                if (Value1(Func_02004cac, 0x814) == 0) {
                    if (Value1(Func_02004cb6, 0x819) != 0) {
                        Call6(Func_02004cac_a, 0, 32, 17, 39, 2, 1);
                        Call6(Func_02004cbe, 42, 62, 17, 36, 2, 3);
                        Call6(Func_02004cd0, 0, 32, 17, 40, 2, 1);
                        Call6(Func_02004ce2, 0, 32, 17, 41, 2, 1);
                        Call6(Func_02004cf2, 0, 32, 17, 42, 2, 3);
                    } else {
                        Call6(Func_02004d06, 44, 59, 17, 37, 2, 6);
                    }
                    Func_02004d14(9);
                    Func_02004d1a(10);
                }
            }
            Func_02004da4(16, 0, 0);
            goto L_02002510;
        }
        v5 = 0;
        if (Value1(Func_02004d9e, 0x80b) != 0) {
            Call6(Func_02004d96, 45, 28, 34, 10, 2, 1);
            Call6(Func_02004da6, 45, 30, 16, 10, 2, 1);
            v5 = 1;
        }
        if (Value1(Func_02004dce, 0x80c) != 0) {
            Call6(Func_02004dc6, 47, 28, 36, 10, 2, 1);
            Call6(Func_02004dd6, 47, 30, 18, 10, 2, 1);
            v5 = 1;
        }
        if (Value1(Func_02004dfe, 0x80d) != 0) {
            Call6(Func_02004df6, 45, 29, 34, 11, 2, 1);
            Call6(Func_02004e06, 45, 31, 16, 11, 2, 1);
            v5 = 1;
        }
        if (Value1(Func_02004e2e, 0x80e) != 0) {
            Call6(Func_02004e26, 47, 29, 36, 11, 2, 1);
            Call6(Func_02004e36, 47, 31, 18, 11, 2, 1);
            v5 = 1;
        }
        if (Value1(Func_02004e5e, 0x812) == 0) {
            if (v5 == 0) {
                goto L_020024fc;
            }
        }
        Call6(Func_02004e5a, 30, 43, 32, 40, 8, 3);
        Call6(Func_02004e6c, 30, 43, 33, 39, 8, 1);
        Call6(Func_02004e7c, 30, 43, 36, 38, 3, 3);
        Call6(Func_02004e8e, 36, 58, 32, 41, 8, 4);
        L_020024fc:;
        Call6(Func_02004eb2, 15, 6, 2, 1, 17, 6);
    }
    L_02002510:;
    if (Value1(Func_02004ec8, 0x309) == 0) {
        if (Data_02000240_t[225][0] != 8) {
            goto L_0200254a;
        }
        Func_02004b40();
        Call1(Func_02004eee, 0x309);
        Call6(Func_02004eea, 15, 6, 2, 1, 17, 6);
    } else {
        L_0200254a:;
        if (Value1(Func_02004f02, 0x814) != 0) {
            Func_02005044(141);
            Call3(Func_02004f14, 0x10000, 0x10000, 0x10000);
            Func_02005020();
            Call6(Func_02004f24, 15, 6, 2, 1, 17, 6);
        }
    }
    return 0;
}
