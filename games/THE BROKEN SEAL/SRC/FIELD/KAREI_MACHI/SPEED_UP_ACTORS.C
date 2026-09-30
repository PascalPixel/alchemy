#include "TYPES.H"
#include "CALL.H"

void Engine_ActorSetSpeed();
s32 Engine_TaskAddCallback();
void KareiMachi_AlternateDance(void);
extern s32 KareiMachi_DanceStep;

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
