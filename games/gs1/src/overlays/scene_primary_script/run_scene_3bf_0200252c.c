#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02007b82();
s32 Func_02007b96();
s32 Func_02007baa();
void Func_02007bd4();
void Func_02007be0();
void Func_02007bea();
void Func_02007bf4();
void Func_02007c00();
void Func_02007c08();
void Func_02007c12();
void Func_02007c1e();
void Func_02007c34();
void Func_02007c52();
void Func_02007c62();
void Func_02007c68();
void Func_02007c7a();
void Func_02007c94();
void Func_02007caa();
void Func_02007cb4();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3bf_0200252c(void)
{
    u32 i;
    s32 record;

    record = Value1(Func_02007b82, 0);
    if (record != 0) {
        Func_02007be0(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Value1(Func_02007b96, 0);
    if (record != 0) {
        Func_02007bf4(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Value1(Func_02007baa, 0);
    if (record != 0) {
        Func_02007c08(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Func_02007c7a(0, 0, 0);
    Call3(Func_02007bd4, 2, 0xb333, 0x5999);
    Call3(Func_02007c08, 2, 0x1c8, 192);
    Call3(Func_02007bea, 3, 0xb333, 0x5999);
    Call3(Func_02007c1e, 3, 0x1b8, 184);
    Call3(Func_02007c00, 1, 0xb333, 0x5999);
    Call3(Func_02007c34, 1, 0x1c0, 240);
    Func_02007c52(2);
    Func_02007c94(2, 12, 0);
    Func_02007c62(1);
    Func_02007c68(3);
    Func_02007caa(1, 12, 0);
    Func_02007cb4(3, 12, 0);
    Func_02007c12(15);
}
