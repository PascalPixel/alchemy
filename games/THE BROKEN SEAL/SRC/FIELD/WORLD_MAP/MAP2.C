#include "EDITION.H"
#include "STORY.H"
#include "TYPES.H"
#include "FIELD_SERVICE.H"
#include "SCENE_IDS.H"
#include "CALL.H"
#include "TBS_EDITION.H"

extern u8 MsgWorldMapLook[];
extern u8 MsgWorldMapNowUseOnShip[];
void WorldMap_ActivateSite138(s32 actor);
void Battle_SetObjectFlag5bWhenMode3(void);
void Battle_ClearObjectFlag5bWhenMode3(void);
void BattleFx_ScheduleRatioTransition(s32 speed, s32 frames);
extern s32 gWorldMapTriggerActor;
extern const u8 gBlackOrbLeaderScript[];

void FieldScene_RunScene371_02002858(void);

extern u8 MsgWorldMapRobinWhereGoingSaidUse[];
extern u8 MsgWorldMapWreckageShipScuttledOffCoast[];

/* The actor that speaks for the world map's triggers; the scene entry and the
 * Black Orb scene set it. */
s32 gWorldMapTriggerActor;

void FieldScene_RunScene371_02002274(void)
{
    struct FieldActor *actor;

    actor = (struct FieldActor *)Object_GetById(10);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Engine_TaskWait(1);
    actor->scale_x = 0x18000;
    actor->scale_y = 0x18000;
    actor->facing = 0x4000;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Actor_SetPosition(10, 0x15680000, 0x8380000);
    Engine_TaskWait(1);
    Audio_PlayCue(141);
    Actor_SetSpeed(10, 0x19999, 0x6666);
    Engine_ActorSetAnimation(10, 2);
    Actor_MoveToAndWait(10, 0x156d, 0x858);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x15b80000, -1, 0x8580000, 1);
    Actor_MoveToAndWait(10, 0x159e, 0x858);
    Actor_MoveToAndWait(10, 0x15a8, 0x86e);
    Actor_MoveToAndWait(10, 0x15e8, 0x878);
    Engine_ActorSetAnimation(10, 1);
    Audio_PlayCue(0x121);
    Engine_EventWait(20);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x15d80000, 0x8780000);
    Engine_TaskWait(1);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x15c8, 0x878);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 40);
    Audio_PlayCue(141);
    Engine_ActorSetAnimation(10, 2);
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
    Engine_EventWait(40);
    Camera_MoveTo(0x15d80000, -1, 0x8580000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x15d8, 0x858);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(20);
    Engine_EventEnd();
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

/* Switch every exit event of trigger 138 to a touch event with the Robin dialogue as its value, then move placement 57 to (0x1794, 0xd48) facing 0x3000. The caller passes an actor it does not use. */
void WorldMap_ActivateSite138(s32 actor)
{
    s32 i;

    for (i = 0;; i++) {
        if (gWorldMapEvents[i].control == EVENT_EXIT && gWorldMapEvents[i].trigger == 138) {
            gWorldMapEvents[i].control = EVENT_TOUCH;
            gWorldMapEvents[i].value = (u32)FieldScene_RunScene371_02002858;
        }
        /* FAKEMATCH: a goto past the loop, not a break, keeps the ROM's strength-reduced offsets. */
        if (gWorldMapEvents[i].control == SCENE_EVENTS_END)
            goto marks;
    }
marks:
    for (i = 0;; i++) {
        if (gWorldMapPlacements[i].sprite == 57) {
            gWorldMapPlacements[i].x = 0x17940000;
            gWorldMapPlacements[i].z = 0x0d480000;
            gWorldMapPlacements[i].facing = 0x3000;
            return;
        }
    }
}

void WorldMap_RestoreExitTrigger(void)
{
    s32 i;

    i = 0;
    while (1) {
        if (gWorldMapEvents[i].control == EVENT_TOUCH && gWorldMapEvents[i].trigger == 138) {
            gWorldMapEvents[i].control = EVENT_EXIT;
            gWorldMapEvents[i].value = 33;
            break;
        }
        if (gWorldMapEvents[i].control == SCENE_EVENTS_END)
            break;
        i++;
    }
}

void FieldScene_RunScene371_0200281c(void)
{
    Engine_EventBegin();
    Actor_FaceActor(55, ACTOR_PARTY_LEADER, 0);
    Engine_EventSetMessage((s32)MsgWorldMapNowUseOnShip);
    Engine_EventShowMessage(gWorldMapTriggerActor, 0);
    Actor_FaceDirection(55, 0x3000, 0);
    Engine_EventEnd();
}

void FieldScene_RunScene371_02002858(void)
{
    Engine_EventBegin();
    Battle_SetObjectFlag5bWhenMode3();
    Engine_EventSetMessage((s32)MsgWorldMapRobinWhereGoingSaidUse);
    Engine_EventShowMessage(gWorldMapTriggerActor, 0);
    Battle_ClearObjectFlag5bWhenMode3();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1778, 0xd48);
    Engine_EventEnd();
}

/* Runs dialogue 0x264c and publishes the story result when flag 0x234 is
 * set. */
void StoryScene_ShowRewardDialogue(void)
{

    Engine_EventBegin();
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#else
    Battle_SetObjectFlag5bWhenMode3();
#endif
    Engine_MessageShowCentered((s32)MsgWorldMapWreckageShipScuttledOffCoast, 1);
    if (Engine_GameFlagIsSet(0x234) != 0) {
        ((struct StoryDialogueWork *)gEventWork)->story_result = 1;
    }
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#else
    Battle_ClearObjectFlag5bWhenMode3();
#endif
    Engine_EventEnd();
}

extern u8 MsgWorldMapMatter[];
void ObjectDispatch_InitFromTable6(struct FieldActor *object);
void Engine_ObjectCommitPosition(struct FieldActor *object);
void PartyInventory_Discard(s32 item);
void StoryActor_ConfigureSpawnedObject(u8 *actor);

#define SITE 55
#define ITEM_BLACK_ORB 242

/* The world map's Black Orb used at the wreck: the leader steps up and holds
 * the orb out, the guide explains it, the site's figure walks out and back,
 * and the orb is used up before the party carries on. The step up from the
 * west reads its height through the site pointer before that is set. */
void WorldMap_UseBlackOrb(void)
{
    struct FieldActor *site;
    struct FieldActor *orb;
    struct FieldActor *leader;
    struct FieldActor *actor;
    s32 message;
    struct FieldSprite *sprite;
    u8 *buffer;

    leader = Actor_Get(ACTOR_PARTY_LEADER);
    Event_Begin();
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_ScheduleRatioTransition(0x16666, 6);
    Camera_SetSpeed(0x30000, 0x6000);
    Camera_MoveTo(0x17880000, -1, 0xd680000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    site = NULL;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    leader->unknown_5b = 0;
    ObjectDispatch_InitFromTable6(leader);
    if (leader->z.fixed > 0xd680000) {
        if (leader->x.fixed > 0x176e0000) {
            FieldObject_SetPosition(leader, 0x176e0000, leader->y.fixed, 0xd7d0000);
            Engine_ObjectCommitPosition(leader);
        }
    } else if (leader->x.fixed > 0x177a0000) {
        FieldObject_SetPosition(leader, 0x177a0000, site->y.fixed, 0xd480000);
        Engine_ObjectCommitPosition(leader);
    }
    FieldObject_SetPosition(leader, 0x17690000, 0, 0xd680000);
    Engine_ObjectCommitPosition(leader);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 40);
    Battle_SetObjectFlag5bWhenMode3();
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 28);
    orb = Object_Create(22, leader->x.fixed + 0x20000, 0x260000,
                        leader->z.fixed);
    if (orb != NULL) {
        /* FAKEMATCH: the game clears these bytes from r1 and reaches the first through the created object still in r0; unpinned, the zero takes r3 and the address a copy in r2. */
        register s32 zero asm("r1") = 0;
        register u8 *motion asm("r0") = &orb->motion_flags; /* FAKEMATCH: the first address stays in r0, see above. */
        u8 *attr;

        *motion = zero;
        sprite = orb->sprite;
        attr = &sprite->flags;
        *attr = zero;
        attr++;
        *attr = zero;
        sprite->full_color = 0;
        sprite->palette = 0;
        buffer = Heap_Allocate(17, 0x608);
        Item_LoadIcon(ITEM_BLACK_ORB);
        Vram_Load(sprite->vram_block, 128, buffer + 0x400);
        Heap_Release(17);
        Event_Wait(20);
        orb->update = (void (*)(union FieldObject *))StoryActor_ConfigureSpawnedObject;
        Event_Wait(80);
    }
    Object_GetById(gWorldMapTriggerActor)->facing = 0x3000;
    Actor_ShowEmote(gWorldMapTriggerActor, EMOTE_IN_FRONT, 0);
    Actor_RunRepeatedMotion(gWorldMapTriggerActor, 2);
    message = (s32)MsgWorldMapMatter;
    Event_SetMessage(message);
    Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 80);
    if (orb != NULL) {
        Engine_ObjectDispatchRelease(orb);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(40);
    Actor_Jump(gWorldMapTriggerActor, 6, 40);
    Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_FaceDirection(gWorldMapTriggerActor, 0xd000, 20);
    Event_ShowMessageAndWait(gWorldMapTriggerActor | 0x9000, 0, 40);
    Actor_SetAnimationAndWait(gWorldMapTriggerActor, 4);
    Event_ShowMessageAndWait(gWorldMapTriggerActor | 0x9000, 0, 20);
    Actor_FaceDirection(gWorldMapTriggerActor, 0x3000, 20);
    Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
    Actor_SetSpeed(gWorldMapTriggerActor, 0xcccc, 0x6666);
    Actor_SetAnimation(gWorldMapTriggerActor, 2);
    site = Actor_Get(SITE);
    FieldObject_SetPosition(site, 0x177a0000, site->y.fixed, 0xd480000);
    Engine_ObjectCommitPosition(site);
    FieldObject_SetPosition(site, 0x17710000, 0, 0xd580000);
    Engine_ObjectCommitPosition(site);
    Actor_SetAnimation(SITE, 1);
    Actor_FaceDirection(gWorldMapTriggerActor, 0x5000, 10);
    Actor_RunRepeatedMotion(gWorldMapTriggerActor, 1);
    Event_ShowMessageAndWait(gWorldMapTriggerActor | 0x1000, 0, 20);
    Actor_SetSpeed(gWorldMapTriggerActor, 0x10000, 0x8000);
    Object_GetById(SITE)->unknown_5a &= ~1;
    Actor_SetAnimation(SITE, 2);
    FieldObject_SetPosition(site, 0x176d0000, 0, 0xd600000);
    Engine_ObjectCommitPosition(site);
    Actor_SetAnimation(SITE, 1);
    Event_Wait(10);
    Actor_SetAnimation(SITE, 2);
    FieldObject_SetPosition(site, 0x17710000, 0, 0xd580000);
    Engine_ObjectCommitPosition(site);
    Actor_SetAnimation(SITE, 1);
    Message_ShowCentered(message + 6, 1);
    gEventWork->message++;
    PartyInventory_Discard(ITEM_BLACK_ORB);
    Event_Wait(20);
    Actor_SetAnimation(gWorldMapTriggerActor, 4);
    Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(gWorldMapTriggerActor, 3);
    Actor_SetAnimation(gWorldMapTriggerActor, 2);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
        Actor_SetDestination(gWorldMapTriggerActor, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_WaitForMove(gWorldMapTriggerActor);
    Actor_SetPosition(gWorldMapTriggerActor, 0, 0);
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_ScheduleRatioTransition(0x10000, 6);
    Event_Wait(20);
    WorldMap_RestoreExitTrigger();
    Engine_ActorDestroy(gWorldMapTriggerActor);
    GameFlag_Clear(0x234);
    GameFlag_Set(0x85d);
    Event_End();
}
