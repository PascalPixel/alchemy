#include "TYPES.H"

s32 Engine_ActorGet();
void Engine_EventBegin();
void Engine_ActorSetSpeed();
s32 Engine_ActorSetPosition();
void Engine_CameraFollowActor();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Engine_EventSetMessage();
void Engine_ActorSetAnimationAndWait();
void Engine_EventShowMessage();
void Engine_ActorRunRepeatedMotion();
void Engine_ActorSetAnimation();
void Engine_EventWait();
void Engine_ActorSetDestination();
void Engine_ActorWalkToAndWait();
void Engine_ActorFaceDirection();
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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* FAKEMATCH: the message id differs between the three Colosso stages, so
 * the shared source takes it from a link symbol placed at the id. */
extern u8 Korosseo_GreetMessage[];

/* Colosso: line the other competitors up around actor a0, show its message
 * and walk it back to its place while the others gather on actor 0. */
void Korosseo_RunGreetScene(s32 a0)
{
    s32 p10;
    s32 p9;
    s32 record;
    s32 v6;

    record = Engine_ActorGet();
    p9 = *(s16 *)(record + 10);
    p10 = *(s16 *)(record + 18);
    Engine_EventBegin();
    Call3(Engine_ActorSetSpeed, a0, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 2, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
    Call3((void (*)())Engine_ActorSetPosition, 0, ((s32)p9 << 16), ((s32)((s32)p10 << 16) + -0x300000));
    Engine_ActorSetPosition(1, (p9 << 16) - 0x100000, (p10 << 16) - 0x280000);
    Engine_ActorSetPosition(2, (p9 << 16) + 0x100000, (p10 << 16) - 0x280000);
    Value3(Engine_ActorSetPosition, 3, ((s32)p9 << 16), ((s32)((s32)p10 << 16) + -0x200000));
    Engine_ActorSetPosition(a0, ((s32)p9 << 16), ((s32)((s32)p10 << 16) + -0x500000));
    v6 = 192;
    record = Engine_ActorGet(0);
    *(u16 *)(record + 6) = (v6 << 8);
    Engine_CameraFollowActor(0, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Call1(Engine_EventSetMessage, (s32)Korosseo_GreetMessage);
    Engine_ActorSetAnimationAndWait(a0, 3);
    Engine_EventShowMessage(a0, 0);
    Engine_ActorRunRepeatedMotion(a0, 2);
    Engine_EventShowMessage(a0, 0);
    Engine_ActorRunRepeatedMotion(a0, 2);
    Engine_EventShowMessage(a0, 0);
    Engine_ActorRunRepeatedMotion(a0, 2);
    Engine_EventShowMessage(a0, 0);
    Engine_ActorSetAnimation(3, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimation(2, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(1, 2);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorSetAnimation(2, 2);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Engine_ActorSetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorSetAnimation(3, 2);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Engine_ActorSetDestination(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWalkToAndWait(a0, (p9 - 16), (p10 - 64));
    Call3((void (*)())Engine_ActorSetPosition, 1, 0, 0);
    Call3((void (*)())Engine_ActorSetPosition, 2, 0, 0);
    Call3((void (*)())Engine_ActorSetPosition, 3, 0, 0);
    Engine_ActorWalkToAndWait(a0, (p9 - 16), (p10 - 16));
    Engine_ActorWalkToAndWait(a0, p9, p10);
    Engine_ActorFaceDirection(a0, (v6 << 8), 10);
    Engine_EventEnd();
}
