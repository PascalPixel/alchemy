#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020023ec();
void Func_02002400();
s32 Func_02002430();
s32 Func_0200243c();
void Func_02002444();
void Func_02002456();
void Func_02002464();
void Func_0200246c();
void Func_02002472();
void Func_02002476();
void Func_020024be();
void Func_020024c4();
void Func_020024d6();
void Func_020024ec();
void Func_0200251c();
void Func_0200252a();
void Func_02002530();
void Func_02002536();
void Func_02002540();
void Func_0200254c();
s32 Func_02002550();
void Func_02002564();
void Func_0200256a();
s32 Func_02002570();
void Func_02002572();
void Func_02002574();
void Func_02002584();
void Func_02002586();
void Func_0200258a();
s32 Func_02002590();
void Func_0200259a();
void Func_020025aa();
void Func_020025b8();
void Func_020025ba();
void Func_020025c4();
void Func_020025ca();
void Func_020025ce();
void Func_020025da();
void Func_020025e8();
void Func_020025ea();
void Func_020025fa();
void Func_02002602();
void Func_02002604();
void Func_0200260e();
void Func_02002614();
void Func_02002624();
void Func_0200262c();
void Func_02002634();
void Func_02002636();
void Func_02002640();
void Func_02002724();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene375_02000964(void)
{
    u32 i;
    s32 record;

    Func_020023ec();
    Func_02002456(13, 0, 0);
    Call3(Func_02002464, 1, 0xd80000, 0x1080000);
    Call3(Func_02002472, 5, 0xf80000, 0x1080000);
    record = Func_02002430(1);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(record + 6) = shown;
    }
    record = Func_0200243c(5);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Call3(Func_02002400, 0x200a0ac, 43, 8);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x202;
    Func_02002572();
    Func_02002586();
    Func_02002444(40);
    Call3(Func_02002476, 13, 0xcccc, 0x6666);
    Call3(Func_020024c4, 13, 0xe60000, 0xdc0000);
    Func_020024be(13, 230, 232);
    Func_0200246c(20);
    Func_020024ec(13, 3);
    Call1(Func_0200252a, 0xfcc);
    Func_0200254c(13, 0, 10);
    Func_0200251c(13, 2);
    Call3(Func_02002570, 13, 0x3000, 10);
    Func_0200256a(13, 0, 10);
    Func_02002584(1, 0, 0);
    Call3(Func_02002590, 0, 0x8000, 10);
    Func_02002530(0, 3);
    Func_02002540(1, 3);
    Func_02002540(0, 0);
    Func_020024d6(20);
    Func_020025b8(0, 0, 0);
    Call3(Func_020025c4, 5, 0x8000, 10);
    Func_02002564(5, 3);
    Func_02002574(0, 3);
    Func_02002574(0, 0);
    Call3(Func_02002536, 1, 0xcccc, 0x6666);
    Call3(Func_02002540, 5, 0xcccc, 0x6666);
    Func_020025fa(0, 0xc000, 0);
    Func_0200259a(1, 2);
    record = Value1(Func_02002550, 0);
    if (record != 0) {
        Func_0200258a(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020025ba(5, 2);
    record = Value1(Func_02002570, 0);
    if (record != 0) {
        Func_020025aa(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020025da(13, 2);
    record = Value1(Func_02002590, 0);
    if (record != 0) {
        Func_020025ca(13, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020025e8(1);
    Func_020025fa(1, 0, 0);
    Func_02002604(5, 0, 0);
    Func_02002602(13);
    Func_02002614(13, 0, 0);
    Func_02002624(1, 1);
    Func_0200262c(5, 1);
    Func_02002634(13, 1);
    Func_02002636(14, 0, 0);
    Func_02002640(15, 0, 0);
    Call1(Func_020025ce, 0x801);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Call2(Func_02002724, 0x10000, 0);
    Call1(Func_020025ea, 0x242);
    Func_0200260e();
}
