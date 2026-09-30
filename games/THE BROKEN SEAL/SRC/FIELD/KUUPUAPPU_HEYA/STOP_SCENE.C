#include "TYPES.H"


extern u16 KuupuappuHeya_StopTimer;
void KuupuappuHeya_UpdateActorStops(void);

/* FAKEMATCH: a one-halfword struct keeps the zero a halfword pool constant. */
struct Half {
    u16 v;
};
void FieldScene_ConfigurePairedActors();
void Scheduler_AddOrUpdateCallback();
void Engine_GameFlagClear();
void Engine_EventBegin();
void Engine_EventEnd();
void Engine_ActorSetSpeed();
void Engine_ActorWalkTo();
void Engine_ActorWalkToAndWait();
void Engine_ActorWaitForMove();
void Engine_ActorSetPosition();
void Engine_ActorSetAnimation();
void Engine_ActorFaceDirection();
void Engine_CameraMoveTo();
void Map_SetWorkFourValues();
void Engine_AudioPlayCue();

/* FAKEMATCH: call sites spelled through these wrappers pass their constants straight
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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void KuupuappuHeya_StartActorStops(void)
{
    u32 i;
    s32 record;
    struct Half zero;

    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, 0xa80000, -1, 0x2900000, 1);
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 1, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 2, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 0, 248, 0x2b8);
    Call3(Engine_ActorSetPosition, 1, 0xf80000, 0x2b80000);
    Call3(Engine_ActorSetPosition, 2, 0xf80000, 0x2b80000);
    Call3(Engine_ActorWalkTo, 0, 200, 0x2b8);
    Call3(Engine_ActorWalkTo, 1, 248, 0x2c8);
    Call3(Engine_ActorWalkToAndWait, 2, 232, 0x2b8);
    Engine_ActorWaitForMove(1);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
    Engine_ActorWaitForMove(0);
    Engine_ActorSetAnimation(1, 12);
    FieldScene_ConfigurePairedActors();
    Call4(Map_SetWorkFourValues, 0x300000, 0x2400000, 0x1200000, 0x2e00000);
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 2, 0xc000, 0x6000);
    Call3(Engine_ActorSetSpeed, 24, 0x10000, 0x13333);
    Call3(Engine_ActorSetSpeed, 25, 0x18000, 0x18000);
    {
        u16 *timer = &KuupuappuHeya_StopTimer;

        zero.v = 0;
        *timer = zero.v;
    }
    Call2(Scheduler_AddOrUpdateCallback, (s32)KuupuappuHeya_UpdateActorStops, 0xc94);
    Call1(Engine_GameFlagClear, 0x1ff);
    Engine_EventEnd();
    Engine_AudioPlayCue(9);
}
