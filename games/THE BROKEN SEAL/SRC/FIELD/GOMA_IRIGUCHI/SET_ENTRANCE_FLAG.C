#include "TYPES.H"

#define GomaIriguchi_SetEntranceFlag Func_02000d68

void Func_02001f70();
void Func_02001f80();
void Func_02001f86();
void Func_02001fa0();
void Func_02001fa2();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

void GomaIriguchi_SetEntranceFlag(void)
{

    u8 *p5;

    p5 = *(s32 *)0x03001f30;
    Func_02001f70(78, 1);
    Func_02001f80(2, 15);
    {
        u8 *f = (u8 *)((s32)p5 + 0x71c);
        s32 t = 8;

        t |= *f;
        *f = t;
    }
    Func_02001fa0();
    Func_02001f86(1);
    Func_02001fa2();
}
