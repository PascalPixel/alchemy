#include "TYPES.H"

s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void Engine_ActorSetSpeed();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
s32 Engine_ActorGet();
void Engine_ActorSetPosition();
void Engine_ActorWalkTo();
void Engine_ActorWalkToAndWait();
void Engine_ActorSetAnimation();
void Engine_ActorFaceDirection();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_ActorRunRepeatedMotion();
void Engine_EventWait();
void Engine_EventShowMessageAndWait();
void Engine_ActorJump();
void Engine_ActorShowEmote();
void Engine_ActorSetAnimationAndWait();
void Engine_CameraFollowActor();
void Engine_ActorSetDestination();
void Engine_ActorWaitForMove();
void Engine_GameFlagSet();
void Engine_EventEnd();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void HaidiaSukureta_RunArrivalScene(void)
{
    u32 i;
    s32 record;

    if (Value1(Engine_GameFlagIsSet, 0x800) != 0) {
    } else {
        Engine_EventBegin();
        Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
        Call1(Engine_EventSetMessage, 0xf9f);
        Engine_EventShowMessage(13, 0);
        record = Engine_ActorGet(0);
        {
            s32 x = *(s16 *)(record + 10);
            s32 y = *(s16 *)(record + 18);

            Engine_ActorSetPosition(5, x << 16, y << 16);
            Engine_ActorSetPosition(1, x << 16, y << 16);
        }
        Call3(Engine_ActorSetSpeed, 5, 0xcccc, 0x6666);
        Call3(Engine_ActorSetSpeed, 1, 0xcccc, 0x6666);
        Call3(Engine_ActorWalkTo, 5, 0x128, 0x148);
        Call3(Engine_ActorWalkToAndWait, 1, 0x118, 0x148);
        Engine_ActorSetAnimation(0, 0);
        Engine_ActorSetAnimation(5, 0);
        Engine_ActorSetAnimation(1, 0);
        Call3(Engine_ActorFaceDirection, 5, 0xb000, 0);
        Call3(Engine_ActorFaceDirection, 1, 0xb000, 0);
        Call3(Engine_ActorFaceDirection, 0, 0xb000, 0);
        Call4(Engine_CameraMoveTo, 0xe80000, -1, 0xf00000, 1);
        Engine_CameraWaitForMove();
        Call3(Engine_ActorFaceDirection, 13, 0x8000, 20);
        Engine_ActorRunRepeatedMotion(13, 2);
        Engine_EventShowMessage(13, 0);
        Call3(Engine_ActorSetSpeed, 13, 0x3333, 0x1999);
        Engine_ActorWalkToAndWait(13, 216, 232);
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(13, 1);
        Engine_EventWait(10);
        Engine_EventShowMessageAndWait(13, 0, 40);
        Engine_ActorJump(13, 2, 10);
        Engine_ActorFaceDirection(13, 0, 10);
        Engine_EventShowMessageAndWait(13, 0, 10);
        Engine_ActorWalkToAndWait(13, 248, 232);
        Engine_EventWait(40);
        Engine_ActorRunRepeatedMotion(13, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(13, 0);
        Call3(Engine_ActorShowEmote, 13, 0x101, 60);
        Engine_ActorRunRepeatedMotion(13, 1);
        Call3(Engine_ActorFaceDirection, 13, 0x8000, 20);
        Engine_EventShowMessageAndWait(13, 0, 2);
        Engine_ActorWalkToAndWait(13, 232, 232);
        Engine_EventWait(2);
        Call3(Engine_ActorFaceDirection, 13, 0x4000, 4);
        Engine_ActorRunRepeatedMotion(13, 2);
        Engine_EventWait(4);
        Engine_EventShowMessageAndWait(13, 0, 2);
        Engine_ActorSetAnimationAndWait(13, 4);
        Engine_EventWait(20);
        Engine_CameraFollowActor(0, 1);
        Engine_CameraWaitForMove();
        Engine_ActorSetAnimation(1, 2);
        record = Value1(Engine_ActorGet, 0);
        if (record != 0) {
            Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(1);
        Engine_ActorSetPosition(1, 0, 0);
        Engine_ActorSetAnimation(5, 2);
        record = Value1(Engine_ActorGet, 0);
        if (record != 0) {
            Engine_ActorSetDestination(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(5);
        Engine_ActorSetPosition(5, 0, 0);
        Call1(Engine_GameFlagSet, 0x800);
        Engine_EventEnd();
    }
}
