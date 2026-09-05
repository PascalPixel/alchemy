#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
s32 Func_02005520();
s32 Func_02005530();
void Func_02005532();
void Func_0200554e();
s32 Func_02005568();
void Func_02005572();
void Func_02005576();
s32 Func_02005580();
s32 Func_02005582();
void Func_02005588();
void Func_020055b4();
void Func_020055c0();
void Func_020055d2();
void Func_020055e8();
s32 Func_020055f0();
void Func_020055fa();
u8 *Func_02005606();
void Func_0200560a();
u8 *Func_02005616();
void Func_0200561a();
u8 *Func_02005622();
void Func_02005626();
void Func_0200563a();
u8 *Func_0200564e();
void Func_02005654();
s32 Func_0200565c();
void Func_0200565e();
void Func_02005672();
void Func_02005682();
void Func_02005692();
void Func_020056a0();
void Func_020056a2();
void Func_020056b2();
void Func_020056c2();
void Func_020056d2();
void Func_020056e2();
void Func_020056f2();
void Func_02005702();
void Func_02005710();
void Func_02005712();
void Func_02005722();
void Func_02005732();
void Func_02005742();
void Func_02005752();
void Func_02005762();
void Func_02005772();
void Func_02005782();
void Func_02005792();
void Func_020057a2();
void Func_020057b2();
void Func_020057c2();
void Func_020057d2();
void Func_020057e6();
void Func_020057f8();
void Func_02005814();
void Func_02005826();
void Func_0200588a();
s32 Func_0200589a();
void Func_0200589e();
void Func_020058ae();
void Func_020058be();
void Func_020058ce();
void Func_020058de();
void Func_020058ee();
void Func_020058fe();
void Func_0200590e();
void Func_0200591e();
void Func_0200592e();
void Func_0200593e();
void Func_0200594e();
void Func_0200595e();
void Func_0200596e();
void Func_0200597e();
void Func_0200598e();
void Func_0200599e();
void Func_020059b2();
void Func_020059c4();
void Func_020059e0();
void Func_02005a24();
void Func_02005bbc();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunMiddleSequence(void)
{
    u32 i;
    s32 rec7;
    u8 *record;
    s32 r0;
    s32 v5;
    u8 *p5;
    u8 *volatile *base = (u8 *volatile *)Data_03001ebc;

    *(s32 *)(base[0] + 0x1c0) = 0x204;
    Func_02005692(0);
    rec7 = Value1(Func_02005520, 0x109);
    if (rec7 != 0) {
        p5 = base[9];
        r0 = Value1(Func_02005530, 0x200);
        if (r0 != 0) {
            r0 = Value1(Func_02005582, 0);
        }
        *(volatile s32 *)(p5 + 24) = r0;
    } else {
        Call1(Func_0200554e, 0x200);
        if (Data_02000240_t[225][0] == 4) {
            *(volatile s32 *)(base[9] + 24) = rec7;
            Call1(Func_02005572, 0x200);
        }
    }
    if (Value1(Func_02005568, 0x302) != 0) {
        Call3(Func_0200561a, 11, 0x960000, 0x2d80000);
        if (Value1(Func_02005580, 0x201) != 0) {
            Func_020055d2(11);
            v5 = 9;
            Func_0200563a(11, 5);
            Call6(Func_02005576, 0, 0, 1, 1, v5, 14);
            Call6(Func_02005588, 0, 0, 1, 1, v5, 45);
            {
                u8 *record = Func_02005606(11);
                u8 value = *(volatile u8 *)&record[35];
            
                record[35] = (u8)(value | 2);
            }
        }
    }
    record = Func_02005616(8);
    Func_020055b4((s32)record, 0);
    record = Func_02005622(9);
    Func_020055c0((s32)record, 0);
    Call2(Func_02005532, 0x200b769, 0xc80);
    if (Value1(Func_020055f0, 0x915) != 0) {
        Call3(Func_020056a0, 10, 0x1aa0000, 0x2da0000);
        record = Func_0200564e(10);
        {
            s32 shown = 0x5000;
        
            *(volatile u16 *)((s32)record + 6) = shown;
        }
        Call6(Func_020055e8, 88, 48, 88, 45, 2, 3);
        Call6(Func_020055fa, 24, 49, 24, 48, 2, 1);
        Call6(Func_0200560a, 25, 42, 25, 47, 1, 1);
        Call6(Func_02005626, 22, 50, 2, 1, 24, 49);
    }
    if (Value1(Func_0200565c, 0x302) == 0) {
    } else {
        Call3(Func_02005710, 8, 0xe80000, 0x2dc0000);
        Call6(Func_02005654, 7, 44, 1, 1, 0, 1);
        Call6(Func_0200565e, 74, 58, 78, 41, 1, 5);
        Call6(Func_02005672, 16, 109, 13, 109, 3, 2);
        Call6(Func_02005682, 67, 64, 71, 44, 1, 2);
        Call6(Func_02005692, 67, 64, 72, 44, 1, 2);
        Call6(Func_020056a2, 67, 68, 73, 43, 1, 2);
        Call6(Func_020056b2, 67, 68, 74, 43, 1, 2);
        Call6(Func_020056c2, 67, 64, 75, 44, 1, 2);
        Call6(Func_020056d2, 67, 66, 76, 44, 1, 2);
        Call6(Func_020056e2, 67, 64, 77, 44, 1, 2);
        Call6(Func_020056f2, 67, 64, 78, 44, 1, 2);
        Call6(Func_02005702, 67, 64, 79, 44, 1, 2);
        Call6(Func_02005712, 67, 66, 80, 44, 1, 2);
        Call6(Func_02005722, 2, 0, 9, 42, 2, 2);
        Call6(Func_02005732, 68, 64, 71, 44, 1, 2);
        Call6(Func_02005742, 68, 64, 72, 44, 1, 2);
        Call6(Func_02005752, 68, 68, 73, 43, 1, 2);
        Call6(Func_02005762, 68, 68, 74, 43, 1, 2);
        Call6(Func_02005772, 68, 64, 75, 44, 1, 2);
        Call6(Func_02005782, 68, 66, 76, 44, 1, 2);
        Call6(Func_02005792, 68, 64, 77, 44, 1, 2);
        Call6(Func_020057a2, 68, 64, 78, 44, 1, 2);
        Call6(Func_020057b2, 68, 64, 79, 44, 1, 2);
        Call6(Func_020057c2, 68, 66, 80, 44, 1, 2);
        Call6(Func_020057d2, 4, 0, 9, 42, 2, 2);
        Call6(Func_020057e6, 7, 11, 7, 42, 10, 8);
        Call6(Func_020057f8, 71, 12, 71, 43, 10, 13);
        Call6(Func_02005814, 6, 13, 12, 12, 6, 44);
        Call6(Func_02005826, 0, 1, 1, 1, 7, 44);
        goto L_02001cc2;
    }
    switch (Data_02000240_t[225][0]) {
    case 1:
    case 2:
        Func_02005a24(170);
        break;
    }
    L_02001cc2:;
    if (Value1(Func_0200589a, 0x303) == 0) {
    } else {
        Call3(Func_0200594e, 9, 0x2b80000, 0x2dc0000);
        Call6(Func_0200588a, 74, 58, 107, 41, 1, 5);
        Call6(Func_0200589e, 45, 109, 42, 109, 3, 2);
        Call6(Func_020058ae, 67, 64, 102, 44, 1, 2);
        Call6(Func_020058be, 67, 64, 103, 44, 1, 2);
        Call6(Func_020058ce, 67, 64, 104, 44, 1, 2);
        Call6(Func_020058de, 67, 66, 105, 44, 1, 2);
        Call6(Func_020058ee, 67, 64, 106, 44, 1, 2);
        Call6(Func_020058fe, 67, 64, 107, 44, 1, 2);
        Call6(Func_0200590e, 67, 64, 108, 44, 1, 2);
        Call6(Func_0200591e, 67, 66, 109, 44, 1, 2);
        Call6(Func_0200592e, 68, 64, 102, 44, 1, 2);
        Call6(Func_0200593e, 68, 64, 103, 44, 1, 2);
        Call6(Func_0200594e, 68, 64, 104, 44, 1, 2);
        Call6(Func_0200595e, 68, 66, 105, 44, 1, 2);
        Call6(Func_0200596e, 68, 64, 106, 44, 1, 2);
        Call6(Func_0200597e, 68, 64, 107, 44, 1, 2);
        Call6(Func_0200598e, 68, 64, 108, 44, 1, 2);
        Call6(Func_0200599e, 68, 66, 109, 44, 1, 2);
        Call6(Func_020059b2, 38, 14, 38, 44, 8, 4);
        Call6(Func_020059c4, 102, 14, 102, 44, 8, 12);
        Call6(Func_020059e0, 37, 13, 10, 12, 37, 43);
        goto L_02001e5a;
    }
    switch (Data_02000240_t[225][0]) {
    case 3:
    case 4:
        Func_02005bbc(170);
        break;
    }
    L_02001e5a:;
}
