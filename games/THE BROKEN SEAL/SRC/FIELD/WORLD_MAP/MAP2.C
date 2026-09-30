#include "STORY.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
#include "CALL.H"

extern u8 MsgWorldMapLook[];
extern u8 MsgWorldMapNowUseOnShip[];
void WorldMap_ActivateSite138(s32 actor);
void Battle_SetObjectFlag5bWhenMode3(void);
void Battle_ClearObjectFlag5bWhenMode3(void);
void BattleFx_ScheduleRatioTransition(s32 speed, s32 frames);
extern s32 gWorldMapTriggerActor;
extern const u8 gBlackOrbLeaderScript[];

void FieldScene_RunScene371_02002274(void)
{
    struct FieldActor *actor;

    actor = (struct FieldActor *)Object_GetById(10);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Task_Wait(1);
    actor->scale_x = 0x18000;
    actor->scale_y = 0x18000;
    actor->facing = 0x4000;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetPosition(10, 0x15680000, 0x8380000);
    Task_Wait(1);
    Audio_PlayCue(141);
    Actor_SetSpeed(10, 0x19999, 0x6666);
    Actor_SetAnimation(10, 2);
    Actor_MoveToAndWait(10, 0x156d, 0x858);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x15b80000, -1, 0x8580000, 1);
    Actor_MoveToAndWait(10, 0x159e, 0x858);
    Actor_MoveToAndWait(10, 0x15a8, 0x86e);
    Actor_MoveToAndWait(10, 0x15e8, 0x878);
    Actor_SetAnimation(10, 1);
    Audio_PlayCue(0x121);
    Event_Wait(20);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x15d80000, 0x8780000);
    Task_Wait(1);
    Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x15c8, 0x878);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 40);
    Audio_PlayCue(141);
    Actor_SetAnimation(10, 2);
    Actor_MoveToAndWait(10, 0x15f8, 0x878);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_MoveToAndWait(10, 0x15f8, 0x838);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_MoveToAndWait(10, 0x15bd, 0x838);
    Actor_MoveToAndWait(10, 0x15b8, 0x853);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_MoveToAndWait(10, 0x1572, 0x858);
    Actor_MoveToAndWait(10, 0x1568, 0x838);
    Actor_SetPosition(10, 0, 0);
    Audio_PlayCue(0x121);
    Event_Wait(40);
    Camera_MoveTo(0x15d80000, -1, 0x8580000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x15d8, 0x858);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(20);
    Event_End();
}

/* World-map Black Orb scene: the trigger actor walks up, the camera shows the site, and the party receives the Black Orb before the map is sent to the world map's entrance 78. */
void WorldMap_RunBlackOrbScene(void)
{
    struct FieldActor *leader;

    Engine_EventBegin();
    gWorldMapTriggerActor = 55;
    WorldMap_ActivateSite138(55);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Battle_SetObjectFlag5bWhenMode3();
    leader = Object_GetById(0);
    if (leader != NULL) {
        Engine_ActorSetPosition(gWorldMapTriggerActor, leader->x.fixed, leader->z.fixed);
    }
    Call3(Engine_ActorSetSpeed, gWorldMapTriggerActor, 0x19999, 0xcccc);
    Call3(Engine_ActorWalkToAndWait, gWorldMapTriggerActor, 0x1768, 0xd78);
    Engine_ActorFaceDirection(gWorldMapTriggerActor, 0, 20);
    Engine_ActorShowEmote(gWorldMapTriggerActor, 0x100, 60);
    Engine_ActorStartRepeatedMotion(gWorldMapTriggerActor, 2);
    Engine_EventSetMessage((s32)MsgWorldMapLook);
    Engine_EventShowMessageAndWait(gWorldMapTriggerActor | 0x1000, 0, 10);
    Engine_ActorFaceDirection(0, 0, 0);
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_ScheduleRatioTransition(0x16666, 10);
    Engine_CameraSetSpeed(0x80000, 0x10000);
    Engine_CameraMoveTo(0x17880000, -1, 0xd680000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Battle_SetObjectFlag5bWhenMode3();
    Engine_ActorSetAttachedEffect(gWorldMapTriggerActor, 0x102);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(gWorldMapTriggerActor, 1);
    Engine_EventShowMessageAndWait(gWorldMapTriggerActor | 0x3000, 0, 40);
    Call3(Engine_ActorWalkToAndWait, gWorldMapTriggerActor, 0x1768, 0xd48);
    Engine_ActorWalkToAndWait(gWorldMapTriggerActor, 0x1794, 0xd48);
    Engine_ActorFaceDirection(gWorldMapTriggerActor, 0x3000, 20);
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Engine_ActorEnableActionCallback(0, gBlackOrbLeaderScript);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(gWorldMapTriggerActor, 1);
    Engine_EventShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
    while ((s16)Object_GetById(0)->unknown_64 == 0) {
        Engine_TaskWait(1);
    }
    Call3(Engine_ActorFaceDirection, 0, 0x4000, 20);
    Engine_ActorShowEmote(gWorldMapTriggerActor, 0x106, 0);
    Engine_ActorRunRepeatedMotion(gWorldMapTriggerActor, 2);
    Engine_EventShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
    Engine_ActorFaceDirection(gWorldMapTriggerActor, 0x8000, 10);
    Engine_EventShowMessage(gWorldMapTriggerActor, 0);
    Engine_ActorFaceDirection(0, 0, 20);
    Object_GetById(gWorldMapTriggerActor)->unknown_5a &= ~1;
    Call3(Engine_ActorWalkToAndWait, gWorldMapTriggerActor, 0x178c, 0xd48);
    Engine_EventWait(1);
    Object_GetById(gWorldMapTriggerActor)->unknown_5a |= 1;
    Engine_EventWait(20);
    Object_GetById(gWorldMapTriggerActor)->unknown_5a &= ~1;
    Engine_ActorWalkToAndWait(gWorldMapTriggerActor, 0x1794, 0xd48);
    Engine_EventWait(1);
    Object_GetById(gWorldMapTriggerActor)->unknown_5a |= 1;
    Engine_ItemShowFound(242, 3);
    Engine_PartyGiveItem(242, 0);
    Engine_ActorRunRepeatedMotion(gWorldMapTriggerActor, 1);
    Engine_EventSetMessage((s32)MsgWorldMapNowUseOnShip);
    Engine_EventShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
    Engine_ActorFaceDirection(gWorldMapTriggerActor, 0x3000, 0);
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_ScheduleRatioTransition(0x10000, 10);
    Engine_EventWait(20);
    Engine_GameFlagSet(0x234);
    Engine_GameFlagSet(0x9bf);
    gGameState.saved_scene = (s32)&SceneId_WorldMap;
    gGameState.saved_entrance = 78;
    Engine_EventEnd();
}
