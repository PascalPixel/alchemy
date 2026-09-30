#include "TYPES.H"
#include "CALL.H"
extern u8 MsgKorosseoSiteSecondFinals[];

s32 Object_GetById();
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

/* Colosso: line the other competitors up around actor a0, show its message
 * and walk it back to its place while the others gather on actor 0. */
/* Colosso: the competitors are greeted before a trial. The same scene sits
 * in the wall and log-rolling trial overlays. */
void Korosseo_RunGreetScene(s32 a0)
{
    s32 p10;
    s32 p9;
    s32 record;
    s32 v6;

    record = Object_GetById(a0);
    p9 = *(s16 *)(record + 10);
    p10 = *(s16 *)(record + 18);
    Engine_EventBegin();
    Call3(Engine_ActorSetSpeed, a0, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 2, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
    Engine_ActorSetPosition(0, (p9 << 16), ((p10 << 16) - 0x300000));
    Engine_ActorSetPosition(1, (p9 << 16) - 0x100000, (p10 << 16) - 0x280000);
    Engine_ActorSetPosition(2, (p9 << 16) + 0x100000, (p10 << 16) - 0x280000);
    Engine_ActorSetPosition(3, (p9 << 16), ((p10 << 16) - 0x200000));
    Engine_ActorSetPosition(a0, (p9 << 16), ((p10 << 16) - 0x500000));
    /* FAKEMATCH: the facing is built from a parked local, which keeps its
     * constant in the register the reference holds it in for both uses. */
    v6 = 192;
    record = Object_GetById(0);
    *(u16 *)(record + 6) = (v6 << 8);
    Engine_CameraFollowActor(0, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventSetMessage((s32)MsgKorosseoSiteSecondFinals);
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
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorSetAnimation(2, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorSetAnimation(3, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetDestination(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWalkToAndWait(a0, (p9 - 16), (p10 - 64));
    ((void (*)())Engine_ActorSetPosition)(1, 0, 0);
    ((void (*)())Engine_ActorSetPosition)(2, 0, 0);
    ((void (*)())Engine_ActorSetPosition)(3, 0, 0);
    Engine_ActorWalkToAndWait(a0, (p9 - 16), (p10 - 16));
    Engine_ActorWalkToAndWait(a0, p9, p10);
    Engine_ActorFaceDirection(a0, (v6 << 8), 10);
    Engine_EventEnd();
}
