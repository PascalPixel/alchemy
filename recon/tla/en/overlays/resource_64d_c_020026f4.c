#include "TYPES.H"

/*
 * Measured draft for the reviewed 188-byte owner. The 2026-09-19 baseline
 * emitted 180 bytes with 167 differing byte positions. Keeping the loop
 * count live before setup gives the reference's two saved high registers and
 * emits 184 bytes with 108 differing positions. This proves no exact bytes;
 * the remaining type and allocation residual must be resolved before adoption.
 */

#define Func_020026f4 Func_020026f4

void Func_0200a7ec();
s32 Func_0200a814();
void Func_0200a824();
s32 Func_0200a834();
s32 Func_0200a83c();
s32 Func_0200a844();
void Func_0200aa0c();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void Func_020026f4(void)
{
    u32 i;
    s32 p10;
    s32 rec4;
    s32 rec7;
    s32 rec8;
    s32 record;
    u8 *v5;
    s32 v7;
    s32 base8_5f;

    base8_5f = 95;
    rec8 = Value2(Func_0200a814, 220, 0x2188);
    record = Value1(Func_0200a844, 0x1e1);
    Func_0200a824(record, rec8);
    rec7 = Func_0200a83c();
    rec4 = Value3(Func_0200a834, rec7, 0x800, rec8);
    *(u16 *)((rec8 + 0x2182)) = rec4;
    *(u16 *)((rec8 + 0x2184)) = rec7;
    v5 = (rec8 + 0x1280);
    v7 = (rec8 + 0x800);
    do {
        Func_0200aa0c((s32)v5, 8, 8, 0x40000000, rec4);
        v5[5] |= 32;
        v5[9] = ((15 & v5[9]) & -13);
        {
            s32 shown = 240;

            *(u16 *)((s32)v5 + 30) = shown;
        }
        base8_5f = (base8_5f + -1);
        *(s32 *)(v7 + 24) = -1;
        v5 = ((s32)v5 + 40);
        v7 = (v7 + 28);
    } while (base8_5f >= 0);
    {
        s32 shown = 0;

        *(u16 *)((rec8 + 0x2180)) = shown;
    }
    Call2(Func_0200a7ec, 0x200a58d, 0x480);
    p10 = (s32)v5;
}
