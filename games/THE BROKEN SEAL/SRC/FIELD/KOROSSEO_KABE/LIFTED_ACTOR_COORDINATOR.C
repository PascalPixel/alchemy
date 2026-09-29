#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum LiftedActorCoordinatorMessage {
    MSG_FANS_JUST_CALL_WALL = 0x20aa
};


void Korosseo_FinishSoloRound();
s32 KorosseoKabe_RunStateInteraction();
void KorosseoKabe_ShowFollowUpPrompt();
void FieldScene_RunMiddleSequence();
s32 Korosseo_FadeInCompetitor();
void Korosseo_RestoreCompetitor();
void SceneActor_PlaceWithScale14000();
void Engine_EventBegin();
void Engine_ObjectSetPosition();
void Engine_EventWait();
void Engine_ActorSetSpeed();
void Engine_CameraSetSpeed();
void Engine_EventShowMessage();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_ActorSetAnimation();
void ObjectMotion_CommitCurrentPositionAndActivate();
void Engine_ActorFaceDirection();
void Engine_EventEnd();
void BattleFx_RunRisingObjectSequence();
void battle_owner_69();
void Engine_EventSetMessage();
void Engine_CameraFollowActor();


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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* In transition phase 2 only the fast path runs. Otherwise route 0 plays the
 * full presentation, passing actor 0's position on with Y raised by 0x400000,
 * route 1 plays the short revisit, and every such path ends in the common
 * coordinator tail. */
void FieldScene_RunLiftedActorCoordinator(s32 scene)
{
    void *actor;
    s32 path;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
        return;
    }
    Engine_EventBegin();
    path = KorosseoKabe_RunStateInteraction(scene, 4);
    if (path == 0) {
        Event_SetMessage(MSG_FANS_JUST_CALL_WALL);
        Call2(Engine_CameraSetSpeed, 196608, 24576);
        Call4(Engine_CameraMoveTo, 35127296, -1, 15728640, 1);
        Engine_CameraWaitForMove();
        Engine_EventWait(45);
        Call2(Engine_CameraSetSpeed, 65536, 8192);
        Call4(Engine_CameraMoveTo, 35127296, -1, 12582912, 1);
        Engine_CameraWaitForMove();
        Engine_EventShowMessage(scene, 0);
        Korosseo_FadeInCompetitor(0, 632, 264);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 65536, 32768);
        Call3(Engine_ActorWalkToAndWait, 0, 616, 264);
        Value3(Engine_ActorFaceDirection, 0, 49152, 20);
        battle_owner_69();
        Call2(Engine_CameraSetSpeed, 16384, 2048);
        Call4(Engine_CameraMoveTo, 35127296, -1, 10485760, 1);
        Call3(Engine_ActorSetSpeed, 0, 32768, 16384);
        Engine_ActorSetAnimation(0, 10);
        actor = Engine_ActorGet(0);
        Engine_ObjectSetPosition(actor, *(s32 *)((u8 *)actor + 8),
            *(s32 *)((u8 *)actor + 12) + 4194304,
            *(s32 *)((u8 *)actor + 16));
        ObjectMotion_CommitCurrentPositionAndActivate(0);
        battle_owner_69();
        Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
        Event_ShowMessage(scene, 0);
        Call3(Engine_ActorSetSpeed, 0, 98304, 49152);
        Value3(SceneActor_PlaceWithScale14000, 0, 488, 248);
        Call3(Engine_ActorFaceDirection, 0, 16384, 20);
        BattleFx_RunRisingObjectSequence(0, 6, 0);
        Call4(Engine_CameraMoveTo, 35127296, -1, 10485760, 1);
        Engine_EventShowMessage(scene, 0);
        Korosseo_RestoreCompetitor(0);
        Engine_CameraFollowActor(0, 0);
        KorosseoKabe_ShowFollowUpPrompt(scene, 4);
    } else if (path == 1) {
        Call1(Engine_EventSetMessage, 8361);
        Engine_EventShowMessage(scene, 0);
    }
    Value3(FieldScene_RunMiddleSequence, path, scene, 4);
    Engine_EventEnd();
}
