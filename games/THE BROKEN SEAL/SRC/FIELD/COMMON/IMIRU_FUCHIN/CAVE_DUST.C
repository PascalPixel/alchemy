#include "TYPES.H"

void Engine_AudioPlayCue();
u32 Engine_RandomNext();
void Local_02001c18();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

struct DustParams {
    s32 count;
    s32 kind;
    s32 spreadX;
    s32 spreadY;
    s32 growX;
    s32 growY;
};

/* Every fourth frame, blow a puff of dust across the cave mouth. */
void ImiruFuchin_BlowCaveMouthDust(void)
{
    struct DustParams params;
    struct DustParams *p;
    s32 phase;
    s32 dx;
    s32 dy;

    phase = *(s32 *)0x03001e40 & 3;
    if (phase != 0)
        return;
    p = &params;
    p->kind = 10;
    p->spreadX = 0x8000;
    p->spreadY = 0x8000;
    p->growX = 0x1cccc;
    p->growY = 0x1cccc;
    if ((*(s32 *)0x03001e40 & 7) == 0)
        Engine_AudioPlayCue(136);
    dx = -0x10000 - (((Engine_RandomNext() << 1) >> 16) << 16);
    dy = -(s32)(((Engine_RandomNext() * 3) >> 16) * 0x3333);
    Local_02001c18(0x1340000, 0x400000, 0xde0000, dx, dy, phase, 0xd0001, p);
}
