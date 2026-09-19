#include "FIELD_EFFECT.H"
#include "STAGED_ACTOR.H"
#include "STAGED_ACTOR_PAIR_SCENE.H"

union SpinningActor {
    union FieldObject object;
    struct StagedActor staged;
};

void Map_WaitWorkValuesBelow256(void);

void StagedActorPairScene_RunSpinningLeap(s32 id)
{
    union SpinningActor *work;
    u32 cnt;
    s32 velocity[3];
    s32 angle;

    work = (union SpinningActor *)Engine_ActorGet(id);
    work->object.actor.motion_flags = 0;
    for (cnt = 0; cnt < 9; cnt++) {
        Engine_TaskWait(1);
        work->object.actor.sprite->rotation -= 0x100;
        work->object.actor.x.fixed -= Engine_MathCos(work->object.actor.sprite->rotation) / 2;
        work->staged.unknown_38 = 0x80000000;
    }
    work->object.actor.update = (void (*)(union FieldObject *))StagedActorPairScene_RotateActorPart;
    Engine_AudioPlayCue(136);
    Actor_SetSpeed(id, 0x20000, 0x10000);
    Actor_SetDestination(id, 472, 288);
    work->object.effect.velocity_y = 0xcccc;
    work->object.actor.motion_flags = 3;
    work->object.actor.unknown_22 = 0;
    Engine_ActorWaitForMove(id);
    StagedActorPairScene_WaitForHeight(work, 0x200000);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    for (cnt = 0; cnt < 17; cnt++) {
        angle = cnt << 12;
        velocity[0] = Engine_MathCos(angle);
        velocity[1] = 0;
        velocity[2] = Engine_MathSin(angle);
        velocity[0] -= velocity[0] / 4;
        velocity[2] -= velocity[2] / 2;
        Effect_Spawn(work->object.actor.x.fixed, work->object.actor.y.fixed,
                     work->object.actor.z.fixed, velocity[0], velocity[1], velocity[2], 0, 0);
    }
    Actor_SetDestination(id, 440, 308);
    Engine_ActorWaitForMove(id);
    StagedActorPairScene_WaitForHeight(work, 0x200000);
    work->object.actor.update = 0;
    work->object.actor.sprite->rotation = 0x1000;
    Engine_AudioPlayCue(154);
    Engine_ActorSetAnimation(id, 3);
    Map_WaitWorkValuesBelow256();
    Engine_EventWait(10);
    Engine_ActorSetAnimation(id, 2);
}
