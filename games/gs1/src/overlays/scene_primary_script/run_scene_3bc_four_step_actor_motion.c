#include "types.h"

#define FieldScene_RunFourStepActorMotion Func_020020f0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_0200499e();
s32 Func_02004bb4();
void Func_02004dd0();
s32 Func_02004e4c();
s32 Func_020057b2();
void Func_02005a8c();
void Func_02006b02();
void Func_02006b10();
void Func_02006b18();
void Func_02006b24();
void Func_02006b32();
void Func_02006b44();
void Func_02006b4e();
void Func_02006b52();
void Func_02006b64();
void Func_02006b72();
void Func_02006ba8();
s32 Func_02006bda();
void Func_02006bdc();
void Func_02006bde();
void Func_02006be4();
void Func_02006c00();
void Func_02006c1e();
void Func_02006c26();
void Func_02006c32();
void Func_02006c4c();
void Func_02006c50();
void Func_02006c58();
s32 Func_02006c68();
void Func_02006c70();
void Func_02006c92();
void Func_02006cac();
void Func_02006cae();
void Func_02006cb2();
void Func_02006cd2();
void Func_02006cd4();
void Func_02006d2a();
void Func_02006d3e();
void Func_02006d56();
void Func_02006d68();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunFourStepActorMotion(s32 a0)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p11;
    s32 p8;
    s32 p9;
    s32 rec5;
    s32 rec7;
    s32 record;
    s32 r10;
    s32 v6;
    u8 *p6;

    if (Data_02000240_t[225][0] == 2) {
        Func_0200499e();
        v6 = r10;
    } else {
        Func_02006b18();
        rec5 = Value2(Func_02004bb4, a0, 4);
        if (rec5 != 0) {
            v6 = r10;
        } else {
            Call1(Func_02006bde, 0x20bf);
            Call2(Func_02006c32, 0x30000, 0x6000);
            Call4(Func_02006c4c, 0x3580000, -1, 0xa80000, 1);
            Func_02006c58();
            Func_02006b4e(30);
            Func_02006c1e(a0, 0);
            Func_02006c26(a0, 0);
            Value3(Func_020057b2, 0, 0x330, 200);
            Call3(Func_02006ba8, 0, 0x10000, 0x8000);
            Call3(Func_02006bdc, 0, 0x348, 200);
            Value3(Func_02006c68, 0, 0xc000, 20);
            Func_02006cd4();
            Call4(Func_02006cae, -1, -1, -1, 0);
            Call3(Func_02006be4, 0, 0x8000, 0x4000);
            rec7 = Value1(Func_02006bda, 0);
            p8 = *(volatile s32 *)(rec7 + 12);
            p6 = *(volatile s32 *)(rec7 + 8);
            Call3(Func_02006c00, 0, 0x8000, 0x4000);
            Func_02006c50(0, 10);
            p9 = (0x60000 + p8);
            Func_02006b02(rec7, (s32)p6, p9, *(volatile s32 *)(rec7 + 16));
            Func_02006b10(rec7);
            Func_02006c70(0, 14);
            p10 = (0x400000 + (s32)p6);
            Func_02006b24(rec7, p10, p9, *(volatile s32 *)(rec7 + 16));
            Func_02006b32(rec7);
            Func_02006c92(0, 10);
            Func_02006b44(rec7, p10, (p8 + 0x360000), *(volatile s32 *)(rec7 + 16));
            Func_02006b52(rec7);
            Func_02006cb2(0, 15);
            v6 = ((s32)p6 + 0x300000);
            v6 = ((s32)p6 + 0x300000);
            Func_02006b64(rec7, ((s32)p6 + 0x300000), (p8 + 0x360000), *(volatile s32 *)(rec7 + 16));
            Func_02006b72(rec7);
            Func_02006cd2(0, 12);
            Func_02006d2a(a0, 0);
            Func_02005a8c(0);
            Func_02006d68(0, 0);
            Func_02004dd0(a0, 4);
            goto L_02002298;
        }
        if (rec5 == 1) {
            Call1(Func_02006d3e, 0x20be);
            Func_02006d56(a0, 0);
        }
        L_02002298:;
        Value3(Func_02004e4c, rec5, a0, 4);
        Func_02006cac();
    }
    p10b = v6;
    p11 = a0;
}
