#include "types.h"

#define FieldScene_RunScene398SequenceA Func_02000214

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000b7a();
void Func_02000b82();
void Func_02000b90();
void Func_02000ba0();
void Func_02000ba4();
s32 Func_02000bb6();
u8 *Func_02000bc2();
void Func_02000bc8();
void Func_02000be4();
void Func_02000bfc();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene398SequenceA(void)
{
    u32 i;
    u8 *record;

    Func_02000b82();
    Func_02000ba4(8, 0, 0);
    Call1(Func_02000b7a, 0x883);
    Func_02000b90(40);
    Func_02000bc8(15, 2);
    *(u8 *)(Func_02000bb6(15) + 85) = 0;
    {
        u8 *record = Func_02000bc2(15);
        u8 value = *(volatile u8 *)&record[35];
    
        record[35] = (u8)(value | 2);
    }
    Func_02000bfc(15, 2);
    Call6(Func_02000ba0, 0, 0, 1, 1, 18, 14);
    Func_02000be4();
}
