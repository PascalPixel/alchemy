#include "TYPES.H"

void Engine_ActorSetSpeed();
s32 Engine_TaskAddCallback();
void KareiMachi_AlternateDance(void);
extern s32 KareiMachi_DanceStep;

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Kalay: clear the step counter, speed actors 20 and 21 up and schedule the
 * per-frame task. */
void KareiMachi_SpeedUpActors(s32 a0, s32 a1)
{

    {
        s32 zero = 0;

        KareiMachi_DanceStep = zero;
    }
    Call3(Engine_ActorSetSpeed, 20, 0x19999, 0xcccc);
    Call3(Engine_ActorSetSpeed, 21, 0x19999, 0xcccc);
    Engine_TaskAddCallback((s32)KareiMachi_AlternateDance, 0xc80);
}
