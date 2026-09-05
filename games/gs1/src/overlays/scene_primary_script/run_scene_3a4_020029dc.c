#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern s16 Data_02000240[];
void Func_02006518();
s32 Func_0200657a();
void Func_020065ee();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunScene3a4_020029dc(void)
{
    s32 record;

    record = Func_0200657a(9);
    Func_02006518(record, 0);
    if (Data_02000240[225] == 2) {
        Call3(Func_020065ee, 9, 0xb80000, 0x1480000);
    }
}
