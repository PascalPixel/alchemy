#include "types.h"

#define FieldScene_RunScene3aa_02000184 Func_02000184

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001c00();
void Func_02001c06();
void Func_02001c0e();
void Func_02001c10();
void Func_02001c3c();
void Func_02001c6c();
void Func_02001c72();
void Func_02001c7a();
void Func_02001c80();
void Func_02001cae();
void Func_02001cc2();
void Func_02001ccc();
void Func_02001cd2();
void Func_02001d32();
void Func_02001d3a();
void Func_02001d6e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene3aa_02000184(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    Func_02001c10();
    Func_02001c0e(10);
    if (*(s16 *)(((s32)p5 + 0x16c)) == 4) {
        Func_02001d32(188);
    } else {
        Func_02001d3a(158);
    }
    Func_02001c00(1);
    Func_02001c06(2);
    Func_02001c3c(10);
    Call3(Func_02001c7a, 0, 0x8000, 0x4000);
    Func_02001cc2(0, 2);
    if (*(s16 *)(((s32)p5 + 0x16c)) == 4) {
        Call3(Func_02001ccc, 0, 0, -16);
    } else {
        Call3(Func_02001cd2, 0, 3, -16);
    }
    Func_02001c80(16);
    Func_02001d6e(*(s16 *)(((s32)p5 + 0x16c)));
    Func_02001c6c(1);
    Func_02001c72(2);
    Func_02001cae();
}
