#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "arutin.h"
#include "call.h"
#include "text/msg_ids.h"

#define FieldScene_PrepareActors ((void (*)(void*))FieldScene_PrepareActors)


extern u8 Placement_Messages[];



void FieldScene_RunMiddleSequence(void);

const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_ArutinMura1) {
        return gArutinMuraEntrances1;
    }
    if (v == (s32)&SceneId_ArutinMura2) {
        return gArutinMuraEntrances2;
    }
    return gArutinMuraEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    s32 id = gGameState.scene;
    if (id == (s32)&SceneId_ArutinMura1) {
        if (Engine_GameFlagIsSet(0x909)) {
            gArutinMuraPlacements1[142] = 0;
            gArutinMuraPlacements1[166] = 0;
        }
        return (const struct ScenePlacement *)gArutinMuraPlacements1;
    }
    if (id == (s32)&SceneId_ArutinMura2) {
        if (Engine_GameFlagIsSet(0x8fd))
            gArutinMuraPlacements2[46] = 1;
        if (Engine_GameFlagIsSet(0x8fe) || Engine_GameFlagIsSet(0x907))
            gArutinMuraPlacements2[94] = 1;
        FieldScene_PrepareActors(gArutinMuraPlacements2);
        return (const struct ScenePlacement *)gArutinMuraPlacements2;
    }
    return (const struct ScenePlacement *)gArutinMuraPlacementsOther;
}

const struct SceneEvent *Scene_GetEvents(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_ArutinMura1) {
        return gArutinMuraEvents1;
    }
    if (v == (s32)&SceneId_ArutinMura2) {
        return gArutinMuraEvents2;
    }
    return gArutinMuraEventsOther;
}

void FieldScene_RunActorEightPromptDialogue(void)
{
    struct EventWork *work;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinYourFirstTimeVisitAltin);

    Engine_EventOpenMessage(8, 0);

    if (Engine_EventChooseYesNo(0, 0) == 1) {
        Engine_EventShowMessage(8, 0);
    } else {
        work = gEventWork;
        work->message = (u16)(work->message + 1);
        Engine_EventAskYesNo(8, 0);
    }

    Engine_EventEnd();
}

void SceneDialogue_ShowLine1918(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinDidSeeWaterGushingOut);
    Engine_EventAskYesNo(9, 0);
    Engine_EventEnd();
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{

    struct FieldActor *rec18;
    u8 *ready_flag;
    struct FieldActor *record;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x909) != 0) {
        Engine_EventSetMessage((s32)MsgArutinOnesWhoDefeatedWaterBeasts);
        Event_AskYesNo(14, 0);
    } else {
        Engine_ActorSetAnimation(14, 4);
        Engine_EventSetMessage((s32)MsgArutinWeCantDrinkWaterMonsters);
        Event_ShowMessageAndWait(14, 0, 10);
        ready_flag = GameFlag_IsSet(0x8ff);
        if (ready_flag == 0) {
            rec18 = Object_GetById(18);

            ((struct FieldActor *)Battle_GetWorkObject1e0())->motion_flags = ready_flag;
            Camera_SetSpeed(0x10000, 0x2000);
            Camera_MoveTo(rec18->x.fixed, rec18->y.fixed, rec18->z.fixed, 1);
            Actor_FaceActor(ACTOR_PARTY_LEADER, 0x4000, 0);
            Actor_FaceDirection(14, 0x3000, 0);
            Engine_CameraWaitForMove();
            Engine_EventWait(120);
            record = Object_GetById(0);
            Camera_MoveTo(record->x.fixed, record->y.fixed, record->z.fixed, 1);
            Engine_CameraWaitForMove();
        }
        Engine_ActorSetAnimationAndWait(14, 4);
    }
    Engine_EventEnd();
}

void SceneDialogue_RunActor17Message1924(void)
{
    void Engine_EventEnd(void);

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinTrueFoundAncientRuinsIn);
    Engine_EventAskYesNo(17, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor9Message1932(void)
{

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinGirlFromXianWasAsking);
    Engine_EventAskYesNo(9, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor10Message18d9(void)
{
    void Engine_EventBegin(void);
    void Engine_EventEnd(void);

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinTryingFindYourWayWest);
    Engine_EventAskYesNo(10, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor14Message18e1(void)
{

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinDefeatedThoseMonstersDidnt);
    Engine_EventAskYesNo(14, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor21Message194a(void)
{

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinThereFewBeastsInMine);
    Engine_EventAskYesNo(21, 0);
    Engine_EventEnd();
}

s32 SceneActor_IsSlotZeroAngleInRange(void)
{
    struct Slot02000338 *slot = (struct Slot02000338 *)Object_GetById(0);

    if ((u32)((slot->angle + 0x5FFF) << 16) <= 0x3FFE0000) {
        return 1;
    }
    return 0;
}

void FieldScene_RunActorFifteenFlagBranch(void)
{
    void Engine_EventSetMessage();
    void Engine_ShopOpen();

    if (Engine_GameFlagIsSet(0x242) == 0) {
        Engine_EventBegin();
        Engine_EventSetMessage((s32)MsgArutinDoWantWeapons);

        Engine_EventAskYesNo(15, 0);
        Engine_EventEnd();
        return;
    }

    if (SceneActor_IsSlotZeroAngleInRange() != 0) {
        Engine_ShopOpen(19, 15);
        return;
    }

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinThankGoodnessWaterHasReceded);
    if (Engine_GameFlagIsSet(0x909) != 0) {
        Engine_EventSetMessage((s32)MsgArutinYoullHaveFindPassageIn);
    }
    Engine_EventShowMessage(15, 0);
    Engine_EventEnd();
}

void FieldScene_RunActorTwentyFlagBranch(void)
{
    void Engine_EventEnd(void);
    void Engine_EventEnd(void);
    void Engine_EventSetMessage(s32);

    if (Engine_GameFlagIsSet(0x241) == 0) {
        Engine_EventBegin();
        Engine_EventSetMessage((s32)MsgArutinMyStoreSubmergedWantSell);
        Engine_EventShowMessage(20, 0);
        Engine_EventEnd();
        return;
    }

    if (SceneActor_IsSlotZeroAngleInRange() != 0) {
        Engine_ShopOpen(20, 17);
        return;
    }

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinItsGreatCanSellArmor);
    if (Engine_GameFlagIsSet(0x909) != 0) {
        Engine_EventSetMessage((s32)MsgArutinHowAboutArentImpressedBy);
    }
    Engine_EventShowMessage(17, 0);
    Engine_EventEnd();
}

void FieldScene_RunActorTwentyOneFlagBranch(void)
{
    if (Engine_GameFlagIsSet(0x240) == 0) {
        Engine_EventBegin();
        Engine_EventSetMessage((s32)MsgArutinWillDoIfMyMerchandise);
        Engine_EventShowMessage(21, 0);
        Engine_EventEnd();
        return;
    }

    if (SceneActor_IsSlotZeroAngleInRange() != 0) {
        Engine_ShopOpen(21, 16);
        return;
    }

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinNoneMyGoodsWereDamaged);
    if (Engine_GameFlagIsSet(0x909) != 0) {
        Engine_EventSetMessage((s32)MsgArutinGirlFromXianBoughtLot);
    }
    Engine_EventShowMessage(16, 0);
    Engine_EventEnd();
}

void FieldScene_RunFacingGatedDialogue18(void)
{
    struct FieldActor *rec;

    rec = Object_GetById(0);

    if ((u32)((rec->facing + 0x5fff) << 16) <= 0x3ffe0000) {
        Engine_InnOpen(6, 18);
        return;
    }

    Engine_EventBegin();

    if (Engine_GameFlagIsSet(0x909) != 0) {
        Engine_EventSetMessage((s32)MsgArutinThereSmallTempleWestAltin);
        Engine_EventShowMessage(18, 0);
    } else {
        Engine_EventSetMessage((s32)MsgArutinWeGotLittleDampBut);
        Engine_EventAskYesNo(18, 0);
    }

    Engine_EventEnd();
}

void FieldScene_RunEarlySequence(void)
{
    u32 i;
    struct EventWork *work;
    struct FieldActor *record;
    s32 idx;
    u8 *tbl;
    s32 off;
    s32 o4;
    s32 a;
    s32 b;
    s32 c;

    work = gEventWork;
    Engine_EventBegin();
    for (i = 8; i <= 65; i++) {
        record = Object_GetById(i);
        if (record != 0) {
            record->motion_flags = 0;
        }
    }
    tbl = ArutinMura_TriggerTable;
    idx = (s32)((*(u16 *)&work->touched_trigger - 2) << 16) >> 16;
    off = idx << 3;
    o4 = off + 4;
    a = *(s16 *)(tbl + o4);
    b = *(s16 *)(tbl + o4 + 2);
    if (idx == 1) {
        Event_PlayCue(188);
        Map_CopyCellsTo(42, 33, a, b, 2, 2);
        c = a + 2;
        Map_CopyCellsTo(42, 35, c, b, 2, 2);
        Engine_EventWait(4);
        Map_CopyCellsTo(40, 33, a, b, 2, 2);
        Map_CopyCellsTo(40, 35, c, b, 2, 2);
        Engine_EventWait(4);
    } else {
        Event_PlayCue(158);
        if (idx == 3) {
            Map_CopyCellsTo(33, 42, 8, 17, 1, 2);
        }
        Map_AnimateCells(*(s32 *)(tbl + off), a, b);
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Object_GetById(0)->motion_flags = 0;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    if (idx == 6) {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, 0);
    } else {
        if (idx != 1) {
            Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -4);
        } else {
            Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
            Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
        }
    }
    Engine_EventWait(10);
    Engine_EventRequestExit(work->touched_trigger);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}

void FieldScene_RunScene3a3SequenceB(void)
{
    struct EventWork *work;

    work = gEventWork;
    Engine_EventBegin();
    Object_GetById(0)->motion_flags = 0;
    Event_PlayCue(123);
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
    Engine_EventRequestExit(work->touched_trigger);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}

void SceneMotion_UpdateTimedActor(struct SceneMotion *work)
{
    if (work->delay != 0) {
        if (--work->delay == 1)
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    }
    if (work->velocity == 0) {
        Object_SetMode(work, 1);
        work->y += -0x18000;
        if (work->y < work->ground) {
            if (work->active != 0) {
                Event_PlayCue(229);
                work->active = 0;
                work->delay = 4;
                Work_SetValuesIfNonNegative(0, 0x10000, 0x10000);
            }
            work->y = work->ground;
        }
        work->state = 1;
    } else {
        work->state = 0;
    }
    if (work->timer == 0) {
        Event_PlayCue(152);
        work->active = 1;
        Object_SetMode(work, 2);
        work->velocity = 0x30000;
    }
    if (++work->timer == 60)
        work->timer = 0;
}

void FieldScene_RunScene3a3SequenceC(void)
{
    struct SceneMotion *motion;

    motion = (struct SceneMotion *)Object_GetById(18);
    motion->timer = 0;
    motion->delay = 0;
    *(s32 *)((u8 *)motion + 72) = 0x6666;
    motion->callback = SceneMotion_UpdateTimedActor;
    Actor_SetSpeed(18, 0x13333, 0x9999);
    Actor_MoveToAndWait(18, 28, 0x1cc);
    Actor_MoveToAndWait(18, 24, 0x1c0);
    Event_PlayCue(229);
    Engine_ActorDestroy(18);
    Work_SetValuesIfNonNegative(0, 0x10000, 0x10000);
    Engine_EventWait(4);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(18, 1);
}

void SceneState_SetFlag906ByActorNineteenX(void)
{
    struct Actor *p = (struct Actor *)Object_GetById(19);

    if ((p->f08 >> 20) == 22) {
        Engine_GameFlagSet(0x906);
    } else {
        Engine_GameFlagClear(0x906);
    }
}

s32 Scene_Initialize(void)
{
    s16 scene;

    if (Engine_GameFlagIsSet(0x8fd) != 0) {
        Engine_GameFlagSet(0x240);
    }

    if (Engine_GameFlagIsSet(0x8fe) != 0 || Engine_GameFlagIsSet(0x907) != 0) {
        Engine_GameFlagSet(0x241);
    }

    if (Engine_GameFlagIsSet(0x8fe) != 0 && Engine_GameFlagIsSet(0x907) != 0) {
        Engine_GameFlagSet(0x242);
    }

    scene = gGameState.scene;
    if (scene == (s32)&SceneId_ArutinMura1) {
        FieldScene_RunMiddleSequence();
    } else if (scene == (s32)&SceneId_ArutinMura2) {
        FieldScene_RunScene3a3SequenceD();
    }

    return 0;
}

void FieldScene_RunMiddleSequence(void)
{
    struct FieldActor *leader;
    s32 kind;

    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (GameFlag_IsSet(0x242) != 0) {
        Map_CopyCellsTo(64, 32, 0, 32, 32, 32);
        Map_CopyCellAttributes(64, 32, 32, 32, 0, 0);
        kind = 20;
    } else {
        if (GameFlag_IsSet(0x241) != 0) {
            Map_CopyCellsTo(64, 0, 0, 32, 32, 32);
            /* FAKEMATCH: the void result is discarded; Call6 changes argument allocation. */
            Value6(Engine_MapCopyCellAttributes, 64, 0, 32, 32, 0, 0);
            Engine_ActorDestroy(17);
            kind = 20;
        } else {
            if (GameFlag_IsSet(0x240) == 0) {
                goto unchanged;
            }
            Map_CopyCellsTo(0, 64, 0, 32, 32, 32);
            /* FAKEMATCH: the void result is discarded; Call6 changes argument allocation. */
            Value6(Engine_MapCopyCellAttributes, 0, 64, 32, 32, 0, 0);
            Engine_ActorDestroy(16);
            kind = 17;
        }
    }
    Engine_ActorDestroy(kind);
    Engine_ActorDestroy(21);
    goto placed;
unchanged:
    Map_CopyCellAttributes(0, 32, 32, 32, 0, 0);
    Engine_ActorDestroy(15);
    Engine_ActorDestroy(16);
    Engine_ActorDestroy(17);
placed:
    if (GameFlag_IsSet(0x8ff) != 0) {
        Engine_ActorDestroy(18);
    } else {
        BattleFx_SetQueuedSoundAndPlay(170);
        Actor_SetChildValue(18, 2);
        Engine_ActorSetAnimation(18, 3);
        Engine_TaskAddCallback(SceneEffect_SpawnDriftingParticle, 0xc80);
    }
    if (gGameState.entrance == 3) {
        GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    }
    Map_CopyCellAttributes(0, 33, 4, 3, 20, 41);
    if (GameFlag_IsSet(0x906) != 0) {
        Actor_SetPosition(19, 0x1680000, 0xa80000);
    }
    Engine_ActorSetSpriteFlags(Object_GetById(19), 0);
    Actor_SetChildValue(22, 15);
    Engine_ActorSetChildValue(23, 15);
    Actor_SetChildValue(24, 15);
    Object_GetById(22)->collision_flags |= 8;
    Object_GetById(23)->collision_flags |= 8;
    Object_GetById(24)->collision_flags |= 8;
    Object_GetById(22)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    Object_GetById(23)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    Object_GetById(24)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    Engine_ActorSetSpritePriority(22, 1);
    Engine_ActorSetSpritePriority(23, 1);
    Engine_ActorSetSpritePriority(24, 1);
    Engine_TaskWait(1);
    Engine_EventBegin();
    Camera_MoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 0);
    Engine_MapRedraw();
    Engine_EventEnd();
    Engine_TaskWait(1);
}

void FieldScene_RunScene3a3SequenceD(void)
{
    struct FieldActor *actor;
    s32 facing;

    if (GameFlag_IsSet(0x240) == 0) {
        Actor_SetPosition(8, 0x3280000, 0x2d70000);
        actor = Object_GetById(8);
        facing = 0x3000;
        actor->facing = facing;
        Actor_SetPosition(9, 0x31a0000, 0x3390000);
    }
    if (GameFlag_IsSet(0x241) == 0) {
        Actor_SetPosition(10, 0x2300000, 0x2c60000);
        actor = Object_GetById(10);
        facing = 0x1000;
        actor->facing = facing;
        Actor_SetPosition(11, 0x2400000, 0x2c60000);
    }
    if (GameFlag_IsSet(0x242) == 0) {
        Actor_SetPosition(15, 0x1270000, 0x2e80000);
        actor = Object_GetById(15);
        facing = 0xb000;
        actor->facing = facing;
    } else {
        u8 flags;

        actor = Object_GetById(15);
        flags = 4;
        flags |= actor->collision_flags;
        actor->collision_flags = flags;
    }
    actor = Object_GetById(17);
    if (actor != 0) {
        actor->collision_flags = 4 | actor->collision_flags;
    }
    actor = Object_GetById(16);
    if (actor != 0) {
        actor->collision_flags = 4 | actor->collision_flags;
    }
}

void SceneActor_ResetStateAndSpan(struct Actor02000c0c *actor)
{

    u8 *state = &actor->state;
    s32 clear = 0;
    u8 *attached;

    *state = (u8)clear;
    attached = actor->attached;
    clear -= 13;
    attached[9] = (clear & attached[9]) | 4;
    ObjectGroup_SetChildValue(actor, 3);
    Engine_ActorSetSpriteFlags((struct FieldActor *)actor, 0);
    actor->span = 0x4CCC;
    actor->reach = 0x4CCC;
}

void SceneEffect_UpdateDriftingParticle(struct SceneMotion *work)
{
    work->x += (work->timer << 12) +
        ((s16)((s32)((((u32 (*)(void))Engine_RandomNext)() * 2) >> 16) - 1) << 15);
    if (work->timer <= 3) {
        work->z += -((((u32 (*)(void))Engine_RandomNext)() * 0x8000) >> 16) - 0x10000;
        work->scale_x += 0x2666;
        work->scale_y += -0xa3d;
    } else {
        work->z += 0x20000;
        work->scale_x += 0x7ae;
        work->scale_y += 0x7ae;
    }
    if ((((u32 (*)(void))Engine_RandomNext)() * work->timer) >> 16 == 0)
        ObjectGroup_SetChildValue(work, 7);
    if (work->timer != 0)
        work->timer--;
    else
        work->timer = ((((u32 (*)(void))Engine_RandomNext)() * 5) >> 16) * 2 + 2;
    if (--work->active == 0) {
        work->callback = 0;
        Engine_ObjectDispatchRelease(work);
    }
}

void SceneEffect_SpawnDriftingParticle(void)
{
    struct SceneMotion *work;
    if ((gFrameCount & 3) == 0) {
        work = ((struct SceneMotion *)Object_Create(222, 0x400000, 0, 0x1900000));
        if (work != 0) {
            work->timer = 20;
            work->delay = 0;
            work->active = 20;
            SceneActor_ResetStateAndSpan(work);
            work->callback = SceneEffect_UpdateDriftingParticle;
            Object_SetMode(work, 1);
        }
    }
}

void ArutinMura_RunDriftEndScene(void)
{
    Engine_EventBegin();
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x3f0000, -1, 0x1c20000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(30);
    Engine_ActorSetAnimation(18, 1);
    BattleFx_SetQueuedSoundAndPlay(-1);
    Engine_TaskRemoveCallback(SceneEffect_SpawnDriftingParticle);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(18, 0, 20);
    Actor_FaceDirection(18, 0xd000, 40);
    Event_PlayCue(147);
    Engine_ActorRunRepeatedMotion(18, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(18, 0xb000, 40);
    FieldScene_RunScene3a3SequenceC();
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
    Engine_CameraWaitForMove();
    Engine_ActorSetAnimationAndWait(14, 4);
    GameFlag_Set(0x8ff);
    Engine_EventEnd();
}
