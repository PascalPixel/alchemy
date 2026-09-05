#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200190c();
void Func_02003af4();
void Func_02003b1e();
s32 Func_02003b7c();
s32 Func_02003b80();
void Func_02003b90();
s32 Func_02003bca();
s32 Func_02003bd6();
void Func_02003bde();
s32 Func_02003be2();
s32 Func_02003bec();
void Func_02003bf6();
s32 Func_02003c0a();
s32 Func_02003c1c();
void Func_02003c4e();
void Func_02003c5c();
void Func_02003c68();
void Func_02003c8a();
s32 Func_02003cc0();
void Func_02003cc6();
s32 Func_02003ccc();
void Func_02003cce();
void Func_02003cf4();
void Func_02003d08();
void Func_02003d3e();
void Func_02003d44();
void Func_02003d72();
void Func_02003dbe();

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

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    u32 i;
    s32 rec4;
    s32 zero;
    s32 t;
    s32 nv;
    u8 *p8;
    u8 *p6;
    u8 slot28[40];
    u8 slot16[12];

    Func_02003b90(a0);
    Call4(Func_02003c8a, -1, -1, -1, 0);
    Func_02003b1e();
    Func_02003af4(1);
    *(volatile s32 *)(Func_02003bca(0) + 12) = 0x820000;
    *(volatile s32 *)(Func_02003bd6(0) + 72) = 0x8000;
    zero = 0;
    *(volatile s32 *)(Func_02003be2(0) + 68) = zero;
    *(u8 *)(Func_02003bec(0) + 85) = zero;
    Func_02003cf4();
    Func_02003d08();
    Func_02003bde(30);
    Func_02003d44(204);
    *(u8 *)(Func_02003c0a(0) + 85) = 3;
    Func_02003bf6(24);
    rec4 = Value1(Func_02003c1c, 0);
    p8 = slot28;
    *(s32 *)(p8 + 4) = 7;
    *(s32 *)(p8 + 36) = 0x2008cf1;
    *(s32 *)(p8 + 8) = 0xcccc;
    *(s32 *)(p8 + 12) = 0xcccc;
    i = 0;
    p6 = slot16;
    for (; i < 17; i++) {
        *(s32 *)(p6) = Func_02003b7c(i << 12);
        *(s32 *)(p6 + 4) = 0;
        t = Func_02003b80(i << 12);
        nv = *(s32 *)(p6);
        nv = nv + nv / 2;
        *(s32 *)(p6 + 8) = t;
        *(s32 *)(p6) = nv;
        Func_0200190c(*(s32 *)(rec4 + 8), *(s32 *)(rec4 + 12),
              *(s32 *)(rec4 + 16), nv,
              *(s32 *)(p6 + 4), t, 0x1090001, (s32)p8);
    }
    Func_02003dbe(188);
    Call2(Func_02003d3e, 0, 0x101);
    Func_02003cce(0, 22);
    Call3(Func_02003c4e, 0x50000, 0x50000, 0x10000);
    Call3(Func_02003c5c, -1, -1, 0xe666);
    Func_02003c68();
    Call2(Func_02003d72, 0, 0x100);
    *(volatile s32 *)(Func_02003cc0(0) + 72) = 0x10000;
    *(volatile s32 *)(Func_02003ccc(0) + 68) = 0x4000;
    Func_02003cc6();
}
