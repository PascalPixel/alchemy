#include "TYPES.H"

s32 Engine_ActorGet();
void Main_08009278();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void SoruSekizo_CheckTileTrigger0166C(void)
{
    s32 x = *(s32 *)(Engine_ActorGet(0) + 8) >> 20;
    s32 y = *(s32 *)(Engine_ActorGet(0) + 16) >> 20;

    if (y == 7 && (u32)(x - 13) <= 1) {
        Call4(Main_08009278, 2, 0xd00000, 0x700000, 255);
    }
}
