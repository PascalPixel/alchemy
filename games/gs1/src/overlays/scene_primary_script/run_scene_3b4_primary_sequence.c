#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
void Func_02003b2a();
void Func_02003b54();
void Func_02003b5e();
s32 Func_02003b62();
void Func_02003b6a();
void Func_02003b84();
void Func_02003b9a();
void Func_02003baa();
void Func_02003bac();
void Func_02003bae();
void Func_02003bbe();
void Func_02003bc2();
void Func_02003bd0();
void Func_02003be4();
void Func_02003c1c();
void Func_02003c50();

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

void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    s32 record;
    s32 base3_2000240;

    base3_2000240 = (s32)Data_02000240;
    if (*(s16 *)((base3_2000240 + 0x24a)) != 10) {
        Func_02003b2a();
        Call3(Func_02003b54, 0, 0x1b333, 0xd999);
        Call3(Func_02003b5e, 10, 0x1b333, 0xd999);
        Func_02003c1c(188);
        record = Value1(Func_02003b62, 0);
        if (record != 0) {
            Func_02003b84(10, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_02003baa(10);
        Func_02003bac(0, 0, 24);
        Func_02003b6a(4);
        Func_02003c50(188);
        Func_02003bc2(10, 0, 16);
        Func_02003bd0(0);
        Call3(Func_02003bbe, 10, 0x108, 0x168);
        Func_02003be4(10);
        Func_02003b9a(10);
        Func_02003bae();
    }
}
