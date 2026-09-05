#include "types.h"

#define FieldScene_RunScene38bSequenceC Func_02000404

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020013e4();
void Func_020013e8();
s32 Func_0200140a();
s32 Func_02001460();
void Func_02001462();
void Func_02001496();
void Func_020014b0();
void Func_020014b4();
void Func_020014b8();
void Func_020014ba();
s32 Func_020014da();
s32 Func_020014e2();
void Func_020014e8();
void Func_0200151a();
s32 Func_02001522();
void Func_02001524();
void Func_0200152e();
void Func_02001538();
void Func_02001548();
void Func_0200154a();
void Func_02001552();
void Func_02001560();
void Func_02001568();
void Func_02001574();
void Func_0200157a();
void Func_0200157c();
void Func_02001582();
void Func_02001584();
void Func_02001598();
void Func_020015ac();
void Func_020015b6();
u8 *Func_020015c0();
void Func_020015c8();
void Func_020015da();
void Func_020015dc();
void Func_020015e6();
void Func_020015ee();
void Func_020015f6();
void Func_0200160e();
void Func_02001612();
void Func_0200163e();
void Func_02001666();
void Func_0200166a();
void Func_02001676();
void Func_0200167a();
void Func_020016ca();
void Func_020016ea();

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}


void FieldScene_RunScene38bSequenceC(void)
{
    u32 i;
    s32 rec;
    u8 *rec7;
    u8 *record;

    rec = Value1(Func_020014da, 0);
    rec7 = Value1(Func_020014e2, 11);
    if ((*(volatile s32 *)((s32)rec7 + 8) >> 20) != 6) {
    } else {
        ((void (*)())Func_020014da)();
        Func_02001582(11, 1);
        Func_02001552(0, 2);
        Func_020014e8(20);
        Call3(Func_0200151a, 0, 0x3333, 0x1999);
        Call3(Func_02001524, 11, 0x3333, 0x1999);
        *(u8 *)(Func_02001522(0) + 90) &= 254;
        {
            u8 zero = 0;
            u8 *dst = rec7 + 85;

            *dst = zero;
        }
        *(volatile s32 *)(rec + 24) = -0x10000;
        Call2(Func_020015da, 0, 0x102);
        Func_02001584(0, 16);
        Func_02001568(11, 111, 196);
        *(volatile s32 *)(rec + 24) = 0x10000;
        Func_0200157c(0, 128, 185);
        Func_0200154a(20);
        *(volatile s32 *)(rec + 24) = -0x10000;
        Call2(Func_0200160e, 0, 0x102);
        Func_020015b6(0, 16);
        Func_02001598(11, 121, 190);
        *(volatile s32 *)(rec + 24) = 0x10000;
        Func_020015ac(0, 141, 189);
        Func_0200157a(20);
        *(volatile s32 *)(rec + 24) = -0x10000;
        Call2(Func_0200163e, 0, 0x102);
        Func_020015e6(0, 16);
        Func_020015c8(11, 132, 186);
        *(volatile s32 *)(rec + 24) = 0x10000;
        {
            u8 *record = Func_020015c0(0);
            u8 value = *(volatile u8 *)&record[90];
        
            record[90] = (u8)(value | 1);
        }
        Call3(Func_020015dc, 0, 0x9999, 0x4ccc);
        Func_020015f6(0, 166, 185);
        Call3(Func_0200166a, 0, 0x8000, 20);
        Func_0200167a(11, 2);
        Func_020016ca(0, 11);
        Func_02001548(10);
        Call1(Func_02001666, 0x1774);
        Func_02001676(11, 0);
        Func_020016ea();
        Func_02001560(10);
        Call1(Func_020015ee, 0x848);
        Func_02001612();
    }
}
