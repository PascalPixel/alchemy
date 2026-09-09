#include "types.h"

#define FieldScene_RunScriptedSteps947And29DD Func_02000170
#define FieldScene_RunEarlySequence Func_02000328
#define FieldScene_RunScene38bSequenceC Func_02000404
#define FieldScene_RunScene38b_02000584 Func_02000584
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_020091c0[];

void Func_0200122c(void);
void Func_0200120c(s32, s32);
void Func_02001214(s32, s32);
void Func_02001248(void);
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
void Func_020014da_a();
void Func_0200151a_a();
s32 Func_02001522_a();
s32 Func_0200162a();
s32 Func_02001636();
s32 Func_0200165a();
void Func_02001660();
u8 *Func_0200169a();
void Func_02001720();
void Func_02001720_a();
void Func_0200172e();
void Func_02001738();
void Func_0200173a();
void Func_02001744();
void Func_0200174a();
void Func_02001756();
void Func_02001760();
void Func_02001764();
void Func_02001766();
void Func_02001770();
void Func_02001780();
void Func_02001780_a();
void Func_0200179c();
void Func_020017a4();
void Func_020017a8();
void Func_020017ac();
void Func_020017b0();
void Func_020017bc();
void Func_020017cc();
void Func_020017d0();
void Func_020017d4();
void Func_020017d6();
void Func_020017f0();
void Func_020017fc();
void Func_02001802();
s32 Func_02001804();
void Func_02001806();
void Func_02001808();
void Func_02001812();
void Func_02001816();
void Func_02001818();
void Func_0200181c();
void Func_02001826();
s32 Func_02001828();
void Func_0200182c();
void Func_02001832();
void Func_02001834();
void Func_02001838();
void Func_02001844();
void Func_02001846();
void Func_0200185c();
s32 Func_0200186a();
void Func_02001880();
void Func_0200189c();
s32 Func_020018a2();
void Func_020018a4();
void Func_020018b0();
void Func_020018ba();
void Func_020018ba_a();
void Func_020018ba_b();
u8 *Func_020018c2();
void Func_020018ce();
void Func_020018f6();
void Func_0200190e();
void Func_0200191a();
void Func_02001922();
void Func_02001928();
void Func_0200192a();
void Func_02001932();
void Func_0200193a();
void Func_02001952();
void Func_02001960();
void Func_0200196c();
void Func_02001978();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void FieldScene_RunScriptedSteps947And29DD(void)
{
    Func_0200122c();
    Func_0200120c(0x947, 1);
    Func_02001214(0x29DD, 1);
    Func_02001248();
}

void FieldScene_RunEarlySequence(void)
{
    u32 i;
    u8 *record;
    s32 v5;
    u8 *tbl;
    u8 *tbl2;
    s32 off;
    s32 off2;
    s32 a1;
    s32 a2;
    u8 *p7;

    p7 = *(u8 *volatile *)Data_03001ebc;
    Func_020013e8();
    for (i = 8; i < 66; i++) {
        record = (u8 *)Value1(Func_0200140a, i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    v5 = (s32)((s32)(*(volatile u16 *)(p7 + 0x16c) - 3) << 16) >> 16;
    if (v5 == 6) {
        Func_0200151a(188);
    } else {
        Call1((void (*)())Func_02001522, 158);
    }
    off = v5 << 2;
    tbl = (u8 *)0x02009d0c;
    a1 = *(s16 *)(tbl + off);
    off2 = off + 2;
    a2 = *(s16 *)(tbl + off2);
    tbl2 = (u8 *)0x02009cf0;
    Value3(Func_020013e4, *(s32 *)(tbl2 + off), a1, a2);
    Call3(Func_02001462, 0, 0x8000, 0x4000);
    *(u8 *)(Func_02001460(0) + 85) = 0;
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    if (v5 == 6) {
        Func_020014b8(0, 2);
        Call3(Func_020014b4, 0, 0, -4);
    } else {
        Call3(Func_020014ba, 0, 3, -16);
    }
    if (v5 == 4) {
        Func_0200152e(0, 3);
    } else {
        Func_02001538(0, 2);
    }
    Func_02001496(16);
    Func_02001574(v5 + 3);
    Func_020014b0();
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
        Func_020014da_a();
        Func_02001582(11, 1);
        Func_02001552(0, 2);
        Func_020014e8(20);
        Call3(Func_0200151a_a, 0, 0x3333, 0x1999);
        Call3(Func_02001524, 11, 0x3333, 0x1999);
        *(u8 *)(Func_02001522_a(0) + 90) &= 254;
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

void FieldScene_RunScene38b_02000584(void)
{
    u32 i;
    s32 rec7;
    u8 *record;
    s32 base5_20091c0;

    rec7 = Value1(Func_0200165a, 0);
    if (Value1(Func_0200162a, 0x845) == 0) {
    } else {
        if (Value1(Func_02001636, 0x848) == 0) {
        } else {
            Func_02001660();
            Call2(Func_02001720, 0x26666, 0x4ccc);
            Call4(Func_02001738, 0x1070000, -1, 0xad0000, 1);
            Func_02001744();
            record = Func_0200169a(12);
            if (*(s32 *)((s32)record + 8) > *(volatile s32 *)(rec7 + 8)) {
                Call3(Func_0200172e, 13, 0x5000, 20);
                Call3(Func_0200174a, 13, 0x100, 20);
                Call1(Func_02001720_a, 0x1775);
                Func_0200173a(13, 0, 10);
                Call3(Func_02001766, 12, 0x100, 0);
            } else {
                Call3(Func_02001764, 12, 0x3000, 20);
                Call3(Func_02001780, 12, 0x100, 20);
                Call1(Func_02001756, 0x1775);
                Func_02001770(12, 0, 10);
                Call3(Func_0200179c, 13, 0x100, 0);
            }
            Call3(Func_020017a8, 14, 0x100, 0);
            Call3(Func_020017a4, 14, 0x3000, 0);
            Call3(Func_020017b0, 12, 0x5000, 0);
            Call3(Func_020017bc, 13, 0x3000, 0);
            Call3(Func_02001760, 0, 0x10c, 184);
            Call3(Func_020017d4, 0, 0xc000, 40);
            Func_020017ac(13, 2);
            Func_020017d6(13, 0, 10);
            Func_020017f0(13, 0, 0);
            Call3(Func_020017fc, 14, 0x3000, 20);
            Call3(Func_02001808, 12, 0x8000, 20);
            Func_020017d0(12, 3);
            Call2(Func_02001832, 14, 0x102);
            Func_02001780_a(40);
            Call3(Func_0200182c, 14, 0x3000, 10);
            Call3(Func_02001838, 12, 0x5000, 0);
            Call3(Func_02001844, 13, 0x3000, 10);
            Func_0200181c(14, 1);
            Func_02001846(14, 0, 10);
            Func_02001816(12, 3);
            Func_02001826(13, 3);
            Func_020017cc(20);
            Func_0200185c(14, 0);
            Call3(Func_02001806, 14, 0x9999, 0x4ccc);
            *(u8 *)(Func_02001804(14) + 90) &= 254;
            Call3(Func_02001834, 14, 0x10a, 172);
            Func_02001802(1);
            *(u8 *)(Func_02001828(14) + 90) |= 1;
            Func_02001818(10);
            Func_02001880(14, 3);
            Func_020018ba(14, 0, 10);
            Call2(Func_02001812, 0x177a, 1);
            bump_step(1);
            Func_02001932(194, 3);
            Value2(Func_0200186a, 194, 0);
            Func_020018ba_a(14, 3);
            Func_020018ba_b(0, 1);
            Call3(Func_0200190e, 0, 0xc000, 0);
            Call3(Func_020018a4, 14, 0x10000, 0x8000);
            *(u8 *)(Func_020018a2(14) + 90) &= 254;
            Call3(Func_020018ce, 14, 0x106, 156);
            Func_0200189c(1);
            {
                u8 *record = Func_020018c2(14);
                u8 value = *(volatile u8 *)&record[90];

                record[90] = (u8)(value | 1);
            }
            Func_020018b0(20);
            Func_02001928(12, 2);
            Func_02001952(12, 0, 10);
            Func_02001922(12, 3);
            Func_0200192a(13, 3);
            Func_0200193a(14, 3);
            base5_20091c0 = (s32)Data_020091c0;
            Call3(Func_02001960, 12, 0x10000, base5_20091c0);
            Call3(Func_0200196c, 13, 0x10000, base5_20091c0);
            Call3(Func_02001978, 14, 0x10000, base5_20091c0);
            Call1(Func_020018f6, 0x849);
            Func_0200191a();
        }
    }
}
