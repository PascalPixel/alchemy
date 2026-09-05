#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200aa7a();
void Func_0200ab8a();
void Func_0200ab9a();
void Func_0200aba0();
void Func_0200abba();
void Func_0200abbc();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunScene39c_02004f44(void)
{
    u8 *p5;
    u8 *addr;
    u8 v;

    p5 = *(volatile s32 *)0x03001f30;
    Call3(Func_0200aa7a, 11, 0x3480000, 0x2580000);
    Func_0200ab8a(93, 1);
    Func_0200ab9a(3, 11);
    addr = p5 + 0x71c;
    v = *addr | 8;
    *addr = v;
    Func_0200abba();
    Func_0200aba0(1);
    Func_0200abbc();
}
