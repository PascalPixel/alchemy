#include "TYPES.H"
#include "SCENE_IDS.H"
#include "FIELD_EVENT.H"
extern struct EventWork *gEventWork;

void ImiruFuchin_HopBy();
s32 Engine_GameFlagIsSet();

extern u8 Data_02000240[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

void ImiruFuchin_NudgeCameraByStep(void)
{
    s32 step;

    step = *(s16 *)(*(u8 **)&gEventWork + 0x16c);
    if (gGameState.scene == (s32)&SceneId_ImiruFuchin3) {
        if (step == 17)
            ImiruFuchin_HopBy(0, -32);
        else
            ImiruFuchin_HopBy(-32, 0);
    }
    if (gGameState.scene == (s32)&SceneId_ImiruFuchin4 && step == 25 && Value1(Engine_GameFlagIsSet, 0x309) != 0)
        ImiruFuchin_HopBy(0, 32);
}
