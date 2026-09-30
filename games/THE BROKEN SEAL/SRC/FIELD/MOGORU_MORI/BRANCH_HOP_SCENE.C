#include "TYPES.H"

s32 Engine_ActorGet();
void Engine_EventBegin();
void FieldScene_RunSixCallSetupSequence();
void FieldScene_RunScene39f_02000d90();
void Battle_WaitMode0();
void Effect_Spawn();
void Engine_CameraFollowActor();
void Engine_ActorFaceEachOther();
void Engine_ActorStartRepeatedMotion();
void Engine_ActorSetAttachedEffect();
void Engine_ActorFaceActor();
void Engine_ActorSetPosition();
void Engine_GameFlagSet();
void Engine_EventEnd();

/* Mogoru Forest: actor 12 hops up on the branch in four steps, with the
 * leader watching each one, then leaves; flag 0x303 records it. */
void MogoruMori_RunBranchHopScene(void)
{
    s32 rec7;

    rec7 = Engine_ActorGet(12);
    Engine_EventBegin();
    FieldScene_RunSixCallSetupSequence(12, 1);
    FieldScene_RunScene39f_02000d90(12, 0x188, 104, 0x70000);
    Battle_WaitMode0(10);
    Effect_Spawn(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + 0x40000), 0, 0, 0, 1, 0);
    Engine_CameraFollowActor(12, 1);
    Engine_ActorFaceEachOther(12, 0, 0);
    Battle_WaitMode0(20);
    Engine_ActorStartRepeatedMotion(12, 2);
    Engine_ActorSetAttachedEffect(12, 0x102);
    Battle_WaitMode0(60);
    FieldScene_RunScene39f_02000d90(12, 0x1a8, 120, 0x30000);
    Engine_ActorFaceActor(0, 12, 0);
    Battle_WaitMode0(6);
    FieldScene_RunScene39f_02000d90(12, 0x1a8, 168, 0x30000);
    Engine_ActorFaceActor(0, 12, 0);
    Battle_WaitMode0(6);
    FieldScene_RunScene39f_02000d90(12, 0x1a8, 208, 0x30000);
    Engine_ActorFaceActor(0, 12, 0);
    Battle_WaitMode0(6);
    FieldScene_RunScene39f_02000d90(12, 0x1a8, 232, 0x30000);
    Engine_ActorFaceActor(0, 12, 0);
    Battle_WaitMode0(6);
    Engine_ActorSetPosition(12, 0, 0);
    Engine_GameFlagSet(0x303);
    Engine_ActorSetPosition(15, 0, 0);
    Engine_EventEnd();
}
