#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Engine_EventBegin(void);
void Engine_AudioPlayCue(s32 cue);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02003310(s32 skip);
void Func_020032e8(s32 countdown);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);

static inline s32 FlagChk(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return GameFlag_Test(flag);
}

static inline void FlagSet(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_SetBit(flag);
}

static inline void ActorPlace(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetPositionAndReset(actor, x, z);
}

void Func_02000640(void)
{
    Engine_EventBegin();
    Func_02003310(0);
    Engine_AudioPlayCue(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020032e8(3);
}

void Func_02000664(void)
{
    if (!FlagChk(0x200)) {
        ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0xcccc, 0x6666);
        ActorPlace(gPartyState.current_owner, 152, 756);
        FlagSet(0x200);
    }
}
