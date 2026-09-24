#include "TYPES.H"

void Engine_ActorFaceActor();
void Engine_EventWait();
void Engine_ActorRunRepeatedMotion();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
void Engine_ActorShowEmote();
void Engine_ActorSetAnimationAndWait();
void Engine_ActorFaceEachOther();
void Engine_ActorSetSpeed();
void Local_02000f80();
void Engine_ActorSetPosition();
s32 Engine_ActorGet();
void Engine_AudioPlayCue();
void Engine_ActorWalkToAndWait();
void Engine_ActorFaceDirection();
void Engine_GameFlagSet();



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

/* Xian: everyone turns to actor 19 and talks it through (message 0x187a
 * onward), actor 15 steps forward, actor 19 is lifted into place with cue
 * 124, and flag 0x301 is set. */
void ShianJiin_RunGatheringScene(void)
{
    u32 i;
    s32 record;

    Engine_ActorFaceActor(13, 19, 0);
    Engine_ActorFaceActor(14, 19, 0);
    Engine_ActorFaceActor(15, 19, 0);
    Engine_ActorFaceActor(16, 19, 0);
    Engine_ActorFaceActor(18, 19, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(15, 2);
    Engine_EventWait(20);
    Call1(Engine_EventSetMessage, 0x187a);
    Engine_EventShowMessageAndWait(15, 0, 20);
    Call3((void (*)())Engine_EventShowMessageAndWait, 16, 0, 20);
    Call3(Engine_ActorShowEmote, 18, 0x105, 60);
    Call3(Engine_ActorShowEmote, 16, 0x101, 60);
    Engine_EventShowMessageAndWait(16, 0, 20);
    Engine_ActorSetAnimationAndWait(18, 4);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(18, 0, 20);
    Call3(Engine_ActorShowEmote, 16, 0x102, 60);
    Engine_EventShowMessageAndWait(16, 0, 20);
    Engine_ActorFaceEachOther(15, 18, 0);
    Engine_EventWait(20);
    Call2((void (*)())Engine_ActorSetAnimationAndWait, 18, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(15, 3);
    Engine_EventWait(20);
    Call3(Engine_ActorSetSpeed, 15, 0xcccc, 0x6666);
    Local_02000f80();
    Engine_ActorRunRepeatedMotion(15, 3);
    Call3(Engine_ActorSetPosition, 19, 0xe80000, 0xa80000);
    Call3(Engine_ActorSetPosition, 20, 0xe80000, 0xa80000);
    record = Engine_ActorGet(19);
    *(s32 *)(record + 12) = 0xc0000;
    record = Engine_ActorGet(19);
    *(s32 *)(record + 60) = -0x80000000;
    record = Engine_ActorGet(19);
    *(s32 *)(record + 24) = 0xcccc;
    record = Engine_ActorGet(19);
    {
        s32 target = *(s32 *)(record + 80);
        s32 shown = 0x8000;
    
        *(u16 *)(target + 30) = shown;
    }
    Engine_AudioPlayCue(124);
    Engine_EventWait(40);
    Engine_ActorWalkToAndWait(15, 216, 152);
    Call3(Engine_ActorFaceDirection, 15, 0x4000, 30);
    Call1(Engine_GameFlagSet, 0x301);
}
