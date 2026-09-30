#include "TYPES.H"
#include "CALL.H"
#include "TASK.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 MsgKorosseoMainThingStage[];
extern u8 MsgKorosseoStageDubbedMini[];
void Korosseo_FinishSoloRound();
void Engine_EventBegin();
s32 KorosseoKabe_RunStateInteraction();
void Engine_EventSetMessage();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_EventShowMessage();
s32 Korosseo_FadeInCompetitor();
void Engine_ActorSetSpeed();
void Engine_EventWait();
void Engine_ActorSetAnimation();
void Engine_ActorSetAttachedEffect();
void Korosseo_RestoreCompetitor();
void Engine_CameraFollowActor();
void KorosseoKabe_ShowFollowUpPrompt();
void Engine_EventEnd();
extern s16 gCell[];

extern u8 MsgKorosseoHereYourObjectiveRideLogs[];
extern u8 MsgKorosseoLogRollingStage[];
extern u8 MsgKorosseoOperatorLiftsWillCheerFor[];
extern u8 MsgKorosseoShiftingFloorStage[];

extern u8 MsgKorosseoFansJustCallWall[];
extern u8 MsgKorosseoScalingWallQuickly[];
void Engine_ObjectSetPosition();
void ObjectMotion_CommitCurrentPositionAndActivate();
void Engine_ActorFaceDirection();
void BattleFx_RunRisingObjectSequence();
void battle_owner_69();

void Object_CommitPosition(struct FieldActor *object);

extern u8 MsgKorosseoFocusRollingLogs[];
extern u8 MsgKorosseoPlaceCalledBoard[];
void Korosseo_FinishSoloRound(void);
s32 KorosseoKabe_RunStateInteraction(s32 speaker, s32 base);
s32 Korosseo_FadeInCompetitor(s32 actor, s32 x, s32 z);
void KorosseoKabe_PushBlockToCell(s32 id, s32 column, s32 row);
void Korosseo_RestoreCompetitor(s32 actor);
void KorosseoKabe_ShowFollowUpPrompt(s32 speaker, s32 base);

/* Colosso wall stage: unless the stage is already cleared, walk the player
 * along the wall, show the introduction and hand over to the stage. */
void KorosseoKabe_RunStageIntro(s32 a0)
{
    s32 rec;
    s32 record;

    if (gCell[225] == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        rec = KorosseoKabe_RunStateInteraction(a0, 1);
        if (rec == 0) {
            Engine_EventSetMessage((s32)MsgKorosseoStageDubbedMini);
            Engine_CameraSetSpeed(0x30000, 0x6000);
            Engine_CameraMoveTo(0x4c80000, -1, 0xb80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventShowMessage(a0, 0);
            Korosseo_FadeInCompetitor(0, 0x4f8, 168);
            Call3(Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
            ((s32 (*)())SceneActor_PlaceWithScale14000)(0, 0x508, 184);
            ((s32 (*)())SceneActor_PlaceWithScale14000)(0, 0x508, 216);
            ((s32 (*)())SceneActor_PlaceWithScale14000)(0, 0x4c8, 216);
            Engine_EventShowMessage(a0, 0);
            ((s32 (*)())SceneActor_PlaceWithScale14000)(0, 0x4c8, 248);
            SceneActor_PlaceWithScale14000(0, 0x4a8, 248);
            Engine_EventWait(3);
            record = ((s32 (*)())Object_GetById)(0);
            *(s32 *)(record + 40) = 0x40000;
            Engine_ActorSetAnimation(0, 28);
            Engine_ActorSetAttachedEffect(0, 0x102);
            Engine_EventWait(30);
            Engine_EventShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(0, 0);
            KorosseoKabe_ShowFollowUpPrompt(a0, 1);
        } else {
            if (rec == 1) {
                Engine_EventSetMessage((s32)MsgKorosseoMainThingStage);
                Engine_EventShowMessage(a0, 0);
            }
        }
        ((s32 (*)())FieldScene_RunMiddleSequence)(rec, a0, 1);
        Engine_EventEnd();
    }
}

void FieldScene_RunSecondActorInteraction(s32 a0)
{
    u32 i;
    s32 rec;
    s32 record;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Event_Begin();
        rec = KorosseoKabe_RunStateInteraction(a0, 2);
        if (rec == 0) {
            Event_SetMessage((s32)MsgKorosseoShiftingFloorStage);
            FieldScene_PlaceSpectatorRow();
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x3d80000, -1, 0xe80000, 1);
            Camera_WaitForMove();
            Value2(Engine_EventShowMessage, a0, 0);
            SceneState_ApplyTable8715AndValue104();
            Event_ShowMessage(a0, 0);
            Korosseo_FadeInCompetitor(0, 0x438, 0x108);
            Event_Wait(15);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
            Value3(SceneActor_PlaceWithScale14000, 0, 0x438, 216);
            SceneActor_PlaceWithScale14000(0, 0x428, 216);
            SceneState_WaitForStatusWords();
            Leader_CheckAhead();
            Camera_MoveTo(-1, -1, -1, 0);
            Event_ShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
            KorosseoKabe_ShowFollowUpPrompt(a0, 2);
        } else {
            if (rec == 1) {
                Event_SetMessage((s32)MsgKorosseoOperatorLiftsWillCheerFor);
                Event_ShowMessage(a0, 0);
            }
        }
        Value3(FieldScene_RunMiddleSequence, rec, a0, 2);
        Engine_EventEnd();
    }
}

void FieldScene_RunSceneThreeCoordinator(s32 a0)
{
    u32 i;
    s32 rec2;
    s32 record;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Event_Begin();
        rec2 = KorosseoKabe_RunStateInteraction(a0, 3);
        if (rec2 != 0) {
        } else {
            Event_SetMessage((s32)MsgKorosseoLogRollingStage);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x2f00000, -1, 0xc00000, 1);
            Camera_WaitForMove();
            Event_Wait(60);
            Camera_SetSpeed(0x10000, 0x2000);
            Camera_MoveTo(0x2f00000, -1, 0xe00000, 1);
            Camera_WaitForMove();
            Event_ShowMessage(a0, 0);
            Korosseo_FadeInCompetitor(0, 0x358, 0x108);
            Event_Wait(10);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
            Value3(SceneActor_PlaceWithScale14000, 0, 0x358, 0x108);
            Value3(SceneActor_PlaceWithScale14000, 0, 0x358, 232);
            Event_ShowMessage(a0, 0);
            SceneActor_PlaceWithScale14000(0, 0x348, 232);
            Event_Wait(10);
            Value3(SceneActor_MovePairByTileOffset, 33, -64, 0);
            Camera_MoveTo(0x2f00000, -1, 0xd80000, 1);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
            Event_Wait(10);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x2f8, 232);
            Event_Wait(10);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 30);
            Event_ShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
            Actor_SetPosition(33, 0x3480000, 0xe80000);
            KorosseoKabe_ShowFollowUpPrompt(a0, 3);
            goto L_020016b0;
        }
        if (rec2 == 1) {
            Event_SetMessage((s32)MsgKorosseoHereYourObjectiveRideLogs);
            Event_ShowMessage(a0, 0);
        }
        L_020016b0:;
        Value3(FieldScene_RunMiddleSequence, rec2, a0, 3);
        Event_End();
    }
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
        Event_SetMessage((s32)MsgKorosseoFansJustCallWall);
        Engine_CameraSetSpeed(196608, 24576);
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
        actor = Object_GetById(0);
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
        Engine_CameraMoveTo(35127296, -1, 10485760, 1);
        Engine_EventShowMessage(scene, 0);
        Korosseo_RestoreCompetitor(0);
        Engine_CameraFollowActor(0, 0);
        KorosseoKabe_ShowFollowUpPrompt(scene, 4);
    } else if (path == 1) {
        Engine_EventSetMessage((s32)MsgKorosseoScalingWallQuickly);
        Engine_EventShowMessage(scene, 0);
    }
    Value3(FieldScene_RunMiddleSequence, path, scene, 4);
    Engine_EventEnd();
}

/* The leader pushes wall block id to cell (column / 2, row): the block slides
 * there and the leader follows half the distance behind it. */
void KorosseoKabe_PushBlockToCell(s32 id, s32 column, s32 row)
{
    s32 pusher = gGameState.selected_actor;
    struct FieldActor *leader = Object_GetById(pusher);
    struct FieldActor *block = Object_GetById(id);
    s32 along_x = (block->x.fixed >> 20) != column / 2;
    s32 dx;
    s32 dz;

    column <<= 16;
    row <<= 16;
    if (along_x) {
        dx = (column - block->x.fixed) / 2;
        dz = 0;
    } else {
        dx = 0;
        dz = (row - block->z.fixed) / 2;
    }
    Engine_ActorSetAnimation(pusher, 8);
    Engine_EventWait(6);
    block->speed = 0x8000;
    block->acceleration = 0x3333;
    Engine_AudioPlayCue(239);
    Object_SetMode(block, 3);
    Engine_ObjectSetPosition(block, column, 0, row);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(pusher, 2);
    Engine_ActorSetSpeed(pusher, 0x8000, 0x3333);
    Object_SetMode(leader, 2);
    Engine_ObjectSetPosition(leader, leader->x.fixed + dx, 0, leader->z.fixed + dz);
    Object_CommitPosition(leader);
    Object_SetMode(leader, 1);
    Object_CommitPosition(block);
    Object_SetMode(block, 1);
    Engine_AudioPlayCue(288);
    Engine_AudioPlayCue(213);
    Engine_EventWait(15);
}

/* The Colosso guide at the Board Walk: on the first visit pans the camera
 * over the course and walks the competitor through pushing a block while
 * explaining it; afterwards reminds that the logs are rolled into a path. */
void KorosseoKabe_RunGuideTalk(s32 speaker)
{
    s32 result;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
        return;
    }
    Engine_EventBegin();
    result = KorosseoKabe_RunStateInteraction(speaker, 5);
    if (result == 0) {
        s32 x;
        s32 z;

        Engine_EventSetMessage((s32)MsgKorosseoPlaceCalledBoard);
        Engine_CameraSetSpeed(0x20000, 0x4000);
        Call4((void (*)())Engine_CameraMoveTo, 0x1480000, -1, 0x1080000, 1);
        Engine_CameraWaitForMove();
        Engine_EventWait(30);
        Call2((void (*)())Engine_CameraSetSpeed, 0x18000, 0x3000);
        x = 408;
        z = 264;
        Engine_CameraMoveTo(0x1380000, -1, 0xb00000, 1);
        Engine_CameraWaitForMove();
        Engine_EventShowMessage(speaker, 0);
        Korosseo_FadeInCompetitor(0, x, z);
        Call3((void (*)())Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
        ((s32 (*)(s32, s32, s32))SceneActor_PlaceWithScale14000)(0, x, 216);
        Call3((void (*)())Engine_ActorFaceDirection, 0, 0x8000, 10);
        Engine_EventShowMessage(speaker, 0);
        KorosseoKabe_PushBlockToCell(16, 360, 208);
        Engine_ActorShowEmote(0, z, 45);
        Call3((void (*)())Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
        x -= 32;
        ((s32 (*)(s32, s32, s32))SceneActor_PlaceWithScale14000)(0, x, 216);
        ((s32 (*)(s32, s32, s32))SceneActor_PlaceWithScale14000)(0, x, 248);
        SceneActor_PlaceWithScale14000(0, 312, 248);
        Engine_EventShowMessage(speaker, 0);
        Korosseo_RestoreCompetitor(0);
        Engine_CameraFollowActor(0, 0);
        Call3((void (*)())Engine_ActorSetPosition, 16, 0x1880000, 0xd00000);
        KorosseoKabe_ShowFollowUpPrompt(speaker, 5);
    } else if (result == 1) {
        Engine_EventSetMessage((s32)MsgKorosseoFocusRollingLogs);
        Engine_EventShowMessage(speaker, 0);
    }
    ((s32 (*)(s32, s32, s32))FieldScene_RunMiddleSequence)(result, speaker, 5);
    Engine_EventEnd();
}

void SceneActor_PlacePartyAtSavedTiles(void)
{
    {
        s32 x = GameFlag_GetByte(896);
        s32 y = GameFlag_GetByte(904);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_GERALD, x, y);
    }
    {
        s32 x = GameFlag_GetByte(912);
        s32 y = GameFlag_GetByte(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_IVAN, x, y);
    }
    {
        s32 x = GameFlag_GetByte(928);
        s32 y = GameFlag_GetByte(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_MIA, x, y);
    }
}
