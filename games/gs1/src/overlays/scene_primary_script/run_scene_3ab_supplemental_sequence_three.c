#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
extern u8 Data_0200a50c[];
s32 Func_02002532();
void Func_02002554();
s32 Func_0200257e();
void Func_020025c2();
s32 Func_020025c6();
void Func_020025c8();
void Func_020025e6();
void Func_0200260c();
void Func_0200261c();
void Func_020026aa();
void Func_020026c4();
void Func_020026c6();
void Func_020026d2();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void FieldScene_RunSupplementalSequenceThree(void)
{
    u8 *work;
    s16 *slot;
    u32 i;
    u8 *record;
    s32 kind;
    s32 off;
    s32 off4;
    u8 *table;

    work = *(u8 **)Data_03001ebc;
    Func_02002554();
    for (i = 8; i <= 65; i++) {
        record = (u8 *)Value1(Func_0200257e, i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    Func_020026c4(158);
    slot = (s16 *)(work + 0x16c);
    kind = *slot - 4;
    off = kind << 3;
    table = Data_0200a50c;
    off4 = off + 4;
    Value3(Func_02002532, *(s32 *)(table + off), *(u16 *)(table + off4),
           *(u16 *)(table + off4 + 2));
    Call3(Func_020025c8, 0, 0x8000, 0x4000);
    *(u8 *)(Func_020025c6(0) + 85) = 0;
    Func_0200261c(0, 2);
    if (kind != 6) {
        Call3(Func_0200260c, 0, 2, -8);
        Func_020025c2(10);
    }
    Func_020026aa(*slot);
    Func_020026c6();
    Func_020026d2();
    Func_020025e6();
}
