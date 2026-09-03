#include "types.h"

#define FieldScene_RunCompanionActorSequence Func_020031b4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200e65c[];
void Func_02009086();
void Func_020090a6();
void Func_020090c2();
void Func_02009110();
void Func_0200911c();
void Func_02009128();
void Func_0200915e();
void Func_02009174();
void Func_02009184();
s32 Func_020091a2();
s32 Func_020091ae();
s32 Func_020091ba();
void Func_02009208();
s32 Func_02009216();
void Func_02009228();
void Func_02009230();
void Func_02009236();
void Func_02009238();
s32 Func_02009242();
void Func_0200925a();
void Func_020092b6();
void Func_020092e4();
void Func_020092e8();
void Func_0200930a();
void Func_0200930e();
void Func_02009314();
void Func_02009338();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

void FieldScene_RunCompanionActorSequence(void)
{
    u32 i;
    s32 record;
    s32 base5_200e65c;

    Func_02009174();
    record = Func_020091a2(12);
    Func_02009110(record, 0);
    record = Func_020091ae(13);
    Func_0200911c(record, 0);
    record = Func_020091ba(14);
    Func_02009128(record, 0);
    Func_02009228(12, 0);
    Func_02009230(13, 0);
    Func_02009238(14, 0);
    Func_02009086(20);
    Call3(Func_0200915e, 0x20000, 0x20000, 0x10000);
    base5_200e65c = (s32)Data_0200e65c;
    Func_02009208(12, base5_200e65c);
    Func_020090a6(10);
    Value2(Func_02009216, 13, base5_200e65c);
    Call3(Func_02009184, -1, -1, 0xe666);
    Func_020090c2(20);
    Value2(Func_02009242, 14, base5_200e65c);
    Call3(Func_0200930e, 11, 0x100, 40);
    Func_020092b6(11, 2);
    Call3(Func_0200930a, 11, 0xd000, 10);
    Call1(Func_020092e8, 0x1c90);
    Func_0200930a(11, 0, 40);
    Func_020092e4(11, 0, 20);
    Func_02009314(11, 0);
    Call3(Func_02009338, 11, 0x8000, 10);
    Call1(Func_02009236, 0x305);
    Func_0200925a();
}
