#include "types.h"

#define FieldScene_RunScene38d_020005f4 Func_020005f4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02002a4a();
void Func_02002a72();
void Func_02002aac();
void Func_02002ab6();
void Func_02002abc();
void Func_02002af6();
void Func_02002afa();
void Func_02002afc();
void Func_02002b16();
void Func_02002b20();
void Func_02002b34();
void Func_02002b3e();
void Func_02002b46();
void Func_02002b58();
void Func_02002b64();
void Func_02002b76();
void Func_02002b82();
void Func_02002b8e();
void Func_02002b9a();
void Func_02002bbc();

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene38d_020005f4(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02002a4a, 0x84e) != 0) {
        Func_02002a72();
        Func_02002b34(0, 19, 0);
        Call3(Func_02002ab6, 19, 0x9999, 0x4ccc);
        Call3(Func_02002afa, 19, 0x26e, 0x2fc);
        Call3(Func_02002b8e, 19, 0xf000, 20);
        Func_02002b3e(19, 3);
        Func_02002b46(17, 3);
        Func_02002aac(20);
        Func_02002b76(19, 0, 0);
        Func_02002abc(20);
        Func_02002b64(19, 3);
        Call1(Func_02002b9a, 0x1749);
        Func_02002bbc(19, 0, 10);
        Call3(Func_02002b16, 19, 0xcccc, 0x6666);
        Call3(Func_02002b58, 19, 0x23a, 0x2f6);
        Func_02002b82(19, 0, 0);
        *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x209;
        Call1(Func_02002af6, 0x85e);
        Call1(Func_02002afc, 0x333);
        Func_02002b20();
    }
}
