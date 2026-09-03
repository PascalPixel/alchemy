#include "types.h"

#define FieldScene_RunScene3c6SequenceA Func_02001090

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_020025c4();
void Func_020025d2();
void Func_020025e0();
void Func_020025ee();
void Func_020025fc();
void Func_02002632();
void Func_02002640();
void Func_0200264e();
void Func_02002654();
void Func_0200265c();
void Func_0200265e();
void Func_02002660();
void Func_0200266a();
void Func_02002684();
void Func_0200268a();
void Func_02002692();
void Func_020026a0();
void Func_020026a6();
void Func_020026b2();
void Func_020026c2();
void Func_020026c4();
void Func_020026ce();
void Func_020026d0();
void Func_020026d2();
void Func_020026dc();
void Func_020026e8();
void Func_020026f4();
void Func_020026f8();
void Func_02002704();
void Func_02002714();
void Func_0200272e();
void Func_0200273e();
void Func_02002744();
void Func_0200274a();
void Func_02002764();
void Func_02002776();
void Func_0200278a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3c6SequenceA(void)
{
    u32 i;
    u8 *record;

    Call1(Func_0200265e, 0x28b0);
    Func_020025c4(20);
    Func_02002654(22, 2);
    Func_020025d2(20);
    Func_0200268a(22, 0);
    Func_020025e0(10);
    Func_02002660(22, 4);
    Func_020025ee(20);
    Func_020026a6(22, 0);
    Func_020025fc(10);
    Call3(Func_020026d0, 0, 0x100, 0);
    Call3(Func_020026dc, 1, 0x100, 0);
    Call3(Func_020026e8, 3, 0x100, 0);
    Call3(Func_020026f4, 2, 0x100, 55);
    Func_02002632(10);
    Func_020026c2(1, 2);
    Func_02002640(20);
    Func_020026f8(1, 0);
    Func_0200264e(10);
    Func_020026ce(22, 4);
    Func_0200265c(20);
    Func_02002714(22, 0);
    Func_0200266a(10);
    Call3(Func_0200273e, 3, 0x102, 40);
    Func_0200272e(3, 0);
    Func_02002684(10);
    Func_02002704(1, 3);
    Func_02002692(30);
    Func_0200274a(1, 0);
    Func_020026a0(10);
    Call3(Func_02002764, 2, 0x8000, 0);
    Func_020026b2(65);
    Call3(Func_02002776, 2, 0xc000, 0);
    Func_020026c4(40);
    Func_02002744(2, 3);
    Func_020026d2(30);
    Func_0200278a(2, 0);
}
