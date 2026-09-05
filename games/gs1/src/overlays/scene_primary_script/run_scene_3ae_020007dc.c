#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000006b[];
extern u8 Data_00000070[];
extern u8 Data_02000240[];
void Func_02001d26();
void Func_02001d42();
void Func_02001d60();
void Func_02001d9c();
void Func_02001dc0();
void Func_02001dda();
void Func_02001ddc();
void Func_02001df8();
void Func_02001e44();
void Func_02001e96();
void Func_02001ebe();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunScene3ae_020007dc(void)
{
    u8 *work;

    Func_02001d60();
    Func_02001e96(158);
    Call3(Func_02001d9c, 0, 0x8000, 0x4000);
    Func_02001e44(0, 3);
    work = Data_02000240;
    if (*(s16 *)(work + 0x1c0) == (s32)Data_0000006b) {
        Call3(Func_02001ddc, 0, 0x130, 0x570);
        Call3(Func_02001d26, 0x20096b8, 78, 86);
    } else {
        if (*(s16 *)(work + 0x1c0) == (s32)Data_00000070) {
            Func_02001df8(0, 248, 192);
            Call3(Func_02001d42, 0x20096ce, 74, 9);
        }
    }
    Func_02001dc0(16);
    Func_02001ebe(3);
    Func_02001dda();
}
