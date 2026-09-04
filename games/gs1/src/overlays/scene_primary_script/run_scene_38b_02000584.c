#include "types.h"

#define FieldScene_RunScene38b_02000584 Func_02000584

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_020091c0[];
extern u8 Data_03001ebc[];
s32 Func_0200162a();
s32 Func_02001636();
s32 Func_0200165a();
void Func_02001660();
u8 *Func_0200169a();
void Func_02001720();
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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
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
                Call1(Func_02001720, 0x1775);
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
            Func_02001780(40);
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
            Func_020018ba(14, 3);
            Func_020018ba(0, 1);
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
