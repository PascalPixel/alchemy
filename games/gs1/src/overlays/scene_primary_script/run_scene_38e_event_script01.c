#include "types.h"

#define RunEventScript01 Func_0200050c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02000b92();
void Func_02000ea2();
s32 Func_02000fc6();
s32 Func_02000fd8();
void Func_02000fe2();
s32 Func_02000fe8();
s32 Func_0200100a();
s32 Func_02001014();
s32 Func_02001034();
s32 Func_02001062();
void Func_02001068();
void Func_02001074();
void Func_02001092();
void Func_020010b6();
void Func_020010c6();
void Func_0200111a();
void Func_02001124();
extern u8 Data_02008bf4[];

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

void RunEventScript01(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02000fc6, 0x109) != 0) {
        Call1(Func_02000fe2, 0x200);
    }
    if (Value1(Func_02000fd8, 0xfd2) == 0) {
        Func_02000ea2(13);
    }
    if (Value1(Func_02000fe8, 0x84a) != 0) {
        Call3(Func_02001068, 11, 0x1340000, 0x1070000);
        Call3(Func_02001074, 12, 0x15a0000, 0x1070000);
        if (Value1(Func_0200100a, 0x84f) == 0) {
            if (Value1(Func_02001014, 0x845) == 0) {
                Func_02001092(11, 0, 0);
                Call3(Func_020010c6, 12, 0x10000, (s32)Data_02008bf4);
            }
        }
    }
    if (Value1(Func_02001034, 0x845) != 0) {
        Call3(Func_020010b6, 10, 0xe00000, 0x1240000);
        Call3(Func_0200111a, 10, 0x4000, 0);
        Func_02001124(8, 0, 0);
        if (Value1(Func_02001062, 0x85e) == 0) {
            Func_02000b92();
        }
    }
}
