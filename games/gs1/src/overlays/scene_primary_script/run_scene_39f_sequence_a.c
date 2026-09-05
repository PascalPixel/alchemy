#include "types.h"

#define FieldScene_RunScene39fSequenceA Func_02001d04

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
void Func_02002834();
void Func_02002abc();
void Func_02002b32();
void Func_02002bc4();
void Func_02004ab8();
s32 Func_02004ac8();
void Func_02004ace();
void Func_02004b00();
void Func_02004b1e();
void Func_02004b42();
s32 Func_02004b44();
s32 Func_02004b4e();
void Func_02004b70();
void Func_02004b7a();
void Func_02004ba8();
void Func_02004bba();
void Func_02004c30();
void Func_02004c3c();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene39fSequenceA(void)
{
    s32 rec7;
    s32 big;
    s32 first;
    s32 shown;
    s32 second;
    s32 base3_2000240;

    rec7 = Func_02004ac8(15);
    big = 0x80000;
    Func_02004ab8();
    Func_02002bc4(15, 0);
    Call4(Func_02002abc, 15, 0x1d8, 104, big);
    Func_02004ace(10);
    Func_02002834(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + big), 0, 0, 0, 1, 0);
    Func_02004ba8(15, 1);
    Func_02004b7a(15, 0, 0);
    Func_02004b00(30);
    Func_02004b70(15, 2);
    Call3(Func_02004bba, 15, 0x103, 0);
    Func_02004c30(147);
    Func_02004b1e(60);
    first = Func_02004b44(0);
    shown = *(s16 *)(first + 10);
    second = Func_02004b4e(0);
    Call4(Func_02002b32, 15, shown, *(s16 *)(second + 18), 0x60000);
    ((s64 (*)())Func_02004b44)(10);
    Call1(Func_02004b42, 0x307);
    base3_2000240 = (s32)Data_02000240;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    Func_02004c3c(53, 0);
    Func_02004b70();
}
