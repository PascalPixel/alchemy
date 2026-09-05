#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][2];
void Func_02002bb0();
s32 Func_02002c4a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3abSequenceG(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(s32 *)Data_03001ebc;
    record = Func_02002c4a(0);
    if (Data_02000240_t[147][0] == 0) {
        if ((u32)(*(s32 *)(record + 8) + -0x900000) <= 0x200000) {
            if (*(s32 *)(record + 16) >= 0xa80000) {
                if (*(s32 *)(record + 16) < 0xb00000) {
                    Call1(Func_02002bb0, 0x2009241);
                    {
                        u16 *target = (u16 *)(((s32)p5 + 0x182));
                        s32 shown = 91;

                        *target = shown;
                    }
                }
            }
        }
    }
}
