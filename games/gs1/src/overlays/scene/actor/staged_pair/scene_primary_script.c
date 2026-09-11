#include "types.h"

#define FieldScene_RunScene3beSequenceA Func_02000d88
#define FieldScene_RunScene3beSequenceB Func_02000df0
#define FieldScene_RunScene3be_02000fd0 Func_02000fd0
#define FieldScene_RunScene3be_02001080 Func_02001080
#define FieldScene_RunScene3be_02001394 Func_02001394
extern u8 Data_00000088[];
extern u8 Data_00000098[];
extern u8 Data_0000009d[];
extern u8 Data_0000009e[];
extern u8 Data_02000240[];
extern u8 Data_0200995c[];
extern u8 Data_02009974[];
extern u8 Data_020099d4[];
extern u8 Data_02009a4c[];
extern u8 Data_02009aac[];
extern u8 Data_02009b3c[];
extern u8 Data_02009b48[];
extern u8 Data_02009bcc[];
extern u8 Data_02009c80[];
extern u8 Data_02009ce0[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
extern u8 Data_000023cc[];

s32 Func_02002234();
s32 Func_0200229c();
s32 Func_020022f2();
s32 Func_02002300();
void Func_0200232a();
s32 Func_02002356();
void Func_02002358();
void Func_02002368();
void Func_02002390();
void Func_02002394();
void Func_0200239e();
s32 Func_020023a0();
void Func_020023d2();
void Func_020023d6();
void Func_020023f0();
void Func_020023f2();
void Func_020023f4();
s32 Func_02002400();
void Func_0200240c();
s32 Func_02002412();
void Func_0200241a();
void Func_0200241c();
void Func_02002434();
void Func_02002444();
void Func_0200244e();
void Func_02002452();
void Func_02002464();
void Func_0200246c();
void Func_0200247a();
void Func_02002492();
void Func_0200239e_a();
void Func_0200246c_a();
s32 Func_020024f0();
s32 Func_020024fa();
s32 Func_02002514();
void Func_02002564();
s32 Func_02002574();
void Func_02002596();
void Func_020025ae();
void Func_020025c8();
s32 Func_0200258c();
void Func_020025a4();
void Func_020025a6();
void Func_020025ac();
void Func_020025b2();
void Func_020025b6();
void Func_020025b8();
void Func_020025bc();
void Func_020025f6();
void Func_02002672();
void Func_02002686();
void Func_02002692();
s32 Func_020028b4();
s32 Func_020028b4_a();
void Func_020028c4();
void Func_020028f8();
s32 Func_020028fa();
void Func_0200292a();
s32 Func_0200292e();
void Func_0200293e();
void Func_02002974();
void Func_02002984();
s32 Func_02002990();
void Func_020029a4();
void Func_020029b2();
void Func_020029c2();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02000fd0(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 Func_02000d20(void)
{
    if (Data_02000240_t[224][0] == (s32)Data_00000098) {
        if (Value1(Func_02002234, 0x96f) != 0) {
            return (s32)Data_020099d4;
        }
        return (s32)Data_02009974;
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_0000009d) {
            return (s32)Data_02009a4c;
        } else {
            if (Data_02000240_t[224][0] == (s32)Data_0000009e) {
                return (s32)Data_02009aac;
            } else {
            }
        }
    }
    return (s32)Data_0200995c;
}

s32 FieldScene_RunScene3beSequenceA(void)
{
    if (Data_02000240_t[224][0] == (s32)Data_00000098) {
        if (Value1(Func_0200229c, 0x96f) != 0) {
            return (s32)Data_02009bcc;
        }
        return (s32)Data_02009b48;
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_0000009d) {
            return (s32)Data_02009c80;
        } else {
            if (Data_02000240_t[224][0] == (s32)Data_0000009e) {
                return (s32)Data_02009ce0;
            } else {
            }
        }
    }
    return (s32)Data_02009b3c;
}

void FieldScene_RunScene3beSequenceB(void)
{
    s32 record;

    if (Value1(Func_020022f2, 0x98a) != 0) {
    } else {
        if (Value1(Func_02002300, 0x9a0) == 0) {
        } else {
            Func_0200232a();
            Call3(Func_02002358, 11, 0x10000, 0x8000);
            record = Value1(Func_02002356, 0);
            if (record != 0) {
                Func_02002394(11, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
            }
            Call3(Func_02002390, 11, -8, 16);
            Func_0200239e(11);
            Call3(Func_020023f2, 11, 0xd000, 0);
            Func_02002368(10);
            Func_020023d2(0, 11, 0);
            Call1(Func_020023f0, 0x23da);
            Value2(Func_02002400, 11, 0);
            if (Value2(Func_020023a0, 0, 0) == 0) {
                Func_0200241c(11, 0);
                Func_020023d6(11, 152, 232);
                Call1(Func_0200239e_a, 0x9a0);
                Func_020023f4(11);
                Func_0200240c(11, 1);
                Data_02000240_t[226][0] = (s32)Data_00000088;
                Data_02000240_t[227][0] = 30;
            } else {
                bump_step(1);
                Func_0200246c(11, 0);
                Func_02002444(11, 2);
                record = Value1(Func_02002412, 0);
                if (record != 0) {
                    Func_02002434(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
                }
                Func_02002452(11);
                Func_02002464(11, 0, 0);
                Func_0200241a(30);
                Func_0200247a(0, 2);
                Func_0200246c_a(0, 0, 16);
                Func_0200247a_a(0);
                Func_02002492(0, 1);
            }
            Func_0200244e();
        }
    }
}

void FieldScene_RunScene3be_02000fd0(void)
{
    u32 i;
    s32 record;
    s32 base5_23cc;

    base5_23cc = (s32)Data_000023cc;
    Func_02002564(base5_23cc);
    Value2(Func_02002574, 8, 0);
    if (Value2(Func_02002514, 0, 0) == 0) {
        if (Value1_02000fd0(Func_020024f0, 0x950) != 0) {
            if (Value1_02000fd0(Func_020024fa, 0x96f) == 0) {
                Func_02002596((base5_23cc + 8));
            }
        }
        Func_020025ae(8, 0);
    } else {
        bump_step(1);
        Func_020025c8(8, 0);
    }
}

void FieldScene_RunScene3be_02001080(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    Func_020025a4();
    if (Value1(Func_0200258c, 0x204) != 0) {
        Call1(Func_020025a6, 0x9a3);
        Call1(Func_020025ac, 0x9a5);
        Call1(Func_020025b2, 0x9a4);
        Call1(Func_020025b8, 0x9a6);
        Call1(Func_020025b6, 0x9a5);
        Call1(Func_020025bc, 0x9a4);
    }
    Func_02002672(*(s16 *)(((s32)p5 + 0x16c)));
    Func_02002686();
    Func_02002692();
    Func_020025f6();
}

s32 FieldScene_RunScene3be_02001394(void)
{
    u32 i;
    s32 record;

    if (Data_02000240_t[224][0] == (s32)Data_00000098) {
        Call1(Func_020028b4, 0x144);
        if (Value1(Func_020028b4_a, 0x9a0) != 0) {
            Func_0200292a(11, 0, 0);
        }
    }
    if (Data_02000240_t[224][0] == (s32)Data_0000009e) {
        if (Data_02000240_t[225][0] == 1) {
            Call6(Func_020028c4, 108, 17, 1, 1, 107, 17);
        }
        if (Value1(Func_020028fa, 0x9a2) != 0) {
            Call3(Func_02002974, 8, 0x1b80000, 0x1340000);
            Func_02002984(8, 2);
            Call6(Func_020028f8, 29, 19, 1, 1, 27, 19);
        }
        if (Value1(Func_0200292e, 0x9a5) != 0) {
            Func_020029a4(9, 0, 0);
            Call3(Func_020029b2, 10, 0x2b80000, 0x1200000);
            Func_020029c2(10, 2);
        }
        record = Func_02002990(12);
        Func_0200293e(record, 0);
    }
    return 0;
}
