#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002082();
s32 Func_02003588();
s32 Func_0200358e();
void Func_020035ac();
void Func_020035ae();
s32 Func_020035b6();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b4SequenceC(void)
{
    u32 i;
    s32 record;

    record = Func_020035b6(0);
    if (*(u16 *)(record + 6) == 0xc000) {
        if (Value1(Func_0200358e, 0x9c4) == 0) {
            if (Func_02003588(243) != -1) {
                Call1(Func_020035ae, 0x9c4);
                Call1(Func_02002082, 0x100);
                Func_020035ac(243);
            }
        }
    }
}
