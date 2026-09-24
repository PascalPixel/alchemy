#include "TYPES.H"

u8 *Engine_ActorGet();
void Engine_MapCopyCellAttributes();
void Engine_GameFlagSet();

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* NONMATCHING: 80 of 80 bytes, 8 halfword edits (2026-09-24). The z tile
 * lives in r4 in the reference (stored to the stack after r1-r3 are set);
 * here it takes r1 and is stored first. */
void Func_02000cf8(void)
{
    u8 *actor = Engine_ActorGet(12);

    if ((*(s32 *)(actor + 8) >> 20) == 30) {
        s32 z = *(s32 *)(actor + 16) >> 20;

        if (z == 20) {
            actor[85] = 2;
            *(s32 *)(actor + 20) = 0;
            actor[35] = 2;
            Engine_MapCopyCellAttributes(30, 20, 1, 1, 32, z);
            Engine_GameFlagSet(0x212);
        }
    }
}
