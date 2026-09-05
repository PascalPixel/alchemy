#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003dc6();
s32 Func_02003dce();
void Func_02003df6();
s32 Func_02003dfe();
void Func_02003e00();
s32 Func_02003e16();
void Func_02003e30();
void Func_02003e32();
void Func_02003e3e();
void Func_02003e50();
void Func_02003e64();
void Func_02003e7c();
void Func_02003edc();

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

void FieldScene_RunScene37bSequenceA(void)
{
    u32 i;
    s32 record;

    record = Value1(Func_02003dce, 17);
    if (record != 0) {
        if ((*(volatile s32 *)(record + 16) >> 20) == 8) {
            Func_02003dc6();
            Func_02003edc(185);
            Call3(Func_02003df6, 17, 0x3333, 0x1999);
            Call3(Func_02003e00, 0, 0x3333, 0x1999);
            *(u8 *)(Func_02003dfe(17) + 90) &= 254;
            Func_02003e50(0, 8);
            record = Func_02003e16(0);
            Func_02003e32(0, *(s16 *)(record + 10), 136);
            Call3(Func_02003e3e, 17, 0x120, 120);
            Func_02003e64(17);
            Func_02003e7c(0, 1);
            Func_02003e30();
        }
    }
}
