#include "TYPES.H"


void Func_020020b8();
void Func_020020ba();
s32 Func_020020ca();
void Func_020020da();
s32 Func_020020e8();
void Func_020020ee();
s32 Func_020020f2();
void Func_02002126();
void Func_02002132();
void Func_02002140();
void Func_0200215e();
void Func_0200219e();
s32 Func_020021b6();
void Func_0200223e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* NONMATCHING: 220 of 220 bytes, 2 halfword edits (2026-09-24). The angle ldrh is
 * scheduled before the 0x2000 constant; the reference builds 0x2000 first.
 * The position pointer must be taken inside the branch to live in r6. */
void Func_02000d0c(void)
{
    u32 i;
    s32 p8;
    u8 *rec7;
    s32 record;
    s32 pos[3];
    s32 *p;

    rec7 = Value1(Func_020020e8, 0);
    p8 = rec7[85];
    if (Value1(Func_020020ca, 0x200) != 0) {
        p = pos;
        p[0] = ((*(s32 *)((s32)rec7 + 8) & -0x100000) + 0x80000);
        p[1] = *(s32 *)((s32)rec7 + 12);
        p[2] = ((*(s32 *)((s32)rec7 + 16) & -0x100000) + 0x80000);
        Call3(Func_020020ba, 0x200000, ((*(u16 *)((s32)rec7 + 6) + 0x2000) & 0xc000), (s32)p);
        if (Value2(Func_020020f2, (s32)rec7, (s32)p) == 0) {
            Func_02002132();
            Func_020020da((s32)rec7, 6);
            Func_020020b8(6);
            Func_0200223e(152);
            Func_020020ee((s32)rec7, 7);
            *(s32 *)((s32)rec7 + 48) = 0x30000;
            *(s32 *)((s32)rec7 + 52) = 0x20000;
            *(s32 *)((s32)rec7 + 40) = 0x40000;
            rec7[85] &= 126;
            Func_02002140((s32)rec7, 0);
            Value3(Func_020021b6, 0, ((s16 *)p)[1], ((s16 *)p)[5]);
            Func_02002126((s32)rec7, 6);
            Func_0200215e((s32)rec7, 1);
            rec7[85] = p8;
            Func_0200219e();
        }
    }
}
