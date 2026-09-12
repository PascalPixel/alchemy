#include "types.h"

#define Scene_RunOpeningAuxiliarySequence Func_02000a1c

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 26 calls, 1 loop(s), and 0 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02003c48();
void Func_02003c6a();
s32 Func_02003d70();
void Func_0200495c();
void Func_020049a4();
void Func_020049ae();
void Func_020049bc();
void Func_020049c8();
void Func_020049d2();
void Func_020049d2_a();
void Func_020049f4();
void Func_02004a0c();
void Func_02004a10();
void Func_02004a14();
void Func_02004a1c();
void Func_02004a1e();
void Func_02004a2c();
void Func_02004a3c();
void Func_02004a42();
void Func_02004a46();
void Func_02004a56();
void Func_02004a76();
void Func_02004a7e();
void Func_02004b30();
void Func_02004b4c();
void Func_02004b5c();

extern u16 Data_02000240[];
extern u8 Value_0000022b;
extern u8 Value_00000090;


/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Func_02000a1c(void)
{
    s32 i1;
    s32 v;
    s32 slot;

    Func_02003c48();
    Func_0200495c();
    v = Func_02003d70(77, 89);
    Func_02003c6a();
    for (i1 = 9; i1 >= 0; i1--) {
        Func_020049a4(8);
    }
    Call3(Func_020049ae, 8, 65536, 32768);
    Call3(Func_020049d2, 8, 88, 256);
    Call3(Func_020049c8, 0, 65536, 32768);
    Call3(Func_020049f4, 0, 120, 256);
    Func_02004a14(8, 1);
    Func_02004a46(0, 8, 0);
    Func_020049bc(10);
    Func_02004a2c(8, 3);
    Func_02004a3c(0, 3);
    Func_020049d2_a(20);
    Call3(Func_02004a10, 0, 131072, 65536);
    Call3(Func_02004a1e, 8, 131072, 65536);
    Call3(Func_02004a42, 0, 112, 256);
    Call3(Func_02004a56, 8, 96, 256);
    Func_02004a76(0, 16);
    Func_02004a7e(8, 9);
    Func_02004a1c(10);
    /* The reference builds this argument as 2 - v and then adds 1 in a
     * second instruction. Written as 3 - v, or as a fresh temporary that is
     * then incremented, combine folds the pair. Reassigning the variable to
     * a value that reads itself keeps the two instructions apart: combine
     * refuses to reuse a destination that its merged pattern still reads. */
    v = 2 - v;
    v = v + 1;
    Func_02004b30(72, v);
    ((u8 *)Data_02000240)[(s32)&Value_0000022b] = 3;
    slot = (s32)&Value_00000090;
    Func_02004b4c(slot, 4);
    Func_02004b5c(slot, 5);
    Call1(Func_02004a0c, 282);
}
