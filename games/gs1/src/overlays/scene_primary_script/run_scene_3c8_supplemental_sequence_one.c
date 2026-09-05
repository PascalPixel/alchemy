#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000b9a();
void Func_0200579c();
void Func_020057e6();
s32 Func_02005812();
s32 Func_02005816();
void Func_02005870();
s32 Func_02005882();
void Func_020058a8();
void Func_020058b8();
void Func_02005906();
void Func_02005914();
void Func_02005920();
void Func_02005972();
void Func_0200597e();
void Func_0200599e();
void Func_020059d6();
void Func_020059ea();
void Func_02005a0e();
void Func_02005a1e();
void Func_02005a42();
void Func_02005a8e();
void Func_02005ac6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    s32 rec;
    s32 v8;
    s32 sh;
    s32 record;
    s32 half;
    s32 w0;
    s32 w1;
    s32 w2;
    s32 w4;
    u8 *p10;
    u8 *p16;
    u8 slot28[40];
    u8 slot16[12];

    rec = Value1(Func_02005882, 0);
    Func_02005870();
    Call4(Func_02005972, -1, -1, -1, 0);
    Func_020057e6();
    Func_0200579c(1);
    *(volatile s32 *)(rec + 12) = 0x820000;
    *(volatile s32 *)(rec + 72) = 0x8000;
    *(volatile s32 *)(rec + 68) = 0;
    *(volatile u8 *)(rec + 85) = 0;
    Func_020059d6();
    Func_020059ea();
    Func_020058a8(30);
    Func_02005a1e(204);
    *(volatile u8 *)(rec + 85) = 3;
    Func_020058b8(24);
    p10 = slot28;
    *(s32 *)(p10 + 4) = 7;
    *(s32 *)(p10 + 36) = 0x200896d;
    *(s32 *)(p10 + 8) = 0xcccc;
    *(s32 *)(p10 + 12) = 0xcccc;
    v8 = 0;
    p16 = slot16;
    do {
        sh = v8 << 12;
        *(s32 *)(p16) = Func_02005812(sh);
        *(s32 *)(p16 + 4) = 0;
        record = Func_02005816(sh);
        *(s32 *)(p16 + 8) = record;
        half = *(s32 *)p16;
        *(s32 *)(p16) = half + half / 2;
        w0 = *(volatile s32 *)(rec + 8);
        w2 = *(volatile s32 *)(rec + 16);
        w1 = *(volatile s32 *)(rec + 12);
        w4 = *(volatile s32 *)(p16 + 4);
        Func_02000b9a(w0, w1, w2, (*(s32 *)p16), w4, record, 0x1090001, (s32)p10);
        v8 = (v8 + 1);
    } while ((u32)v8 <= 16);
    Func_02005a8e(188);
    Call2(Func_02005a0e, 0, 0x101);
    Func_0200599e(0, 22);
    Call3(Func_02005906, 0x50000, 0x50000, 0x10000);
    Call3(Func_02005914, -1, -1, 0xe666);
    Func_02005920();
    Call2(Func_02005a42, 0, 0x100);
    Func_02005ac6();
    *(volatile s32 *)(rec + 72) = 0x10000;
    *(volatile s32 *)(rec + 68) = 0x4000;
    Func_0200597e();
}
