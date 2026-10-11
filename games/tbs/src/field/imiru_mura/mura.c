#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "types.h"
#include "text/msg_ids.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_effect.h"
#include "facing_object.h"
#include "battle_effect_runtime.h"
#include "object_lookup.h"
#include "field_service.h"
#include "call.h"
#include "iwram_call.h"
#include "frame.h"

void SceneActor_TurnTowardTableAngle();

#define ObjectTable_Get ((struct Subject_02001fa4 * (*)())ObjectTable_Get)
extern u8 gEffectWork[];

#define NULL ((void *)0)
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define ACTOR_FLAGS_OFFSET 89
#define ACTOR_FLAGS_OFFSET_020019e8 90
#define ACTOR_DONE_OFFSET 100

struct Object {
    u8 filler00[24];
    s32 x;
    s32 z;
    u8 filler20[68];
    s16 counter;
    s16 mode;
};

struct SceneService_02001990 {
    u16 unknown00[3];
    u16 value06;
};

typedef struct {
    u8 filler0[6];
    u16 unk6;
    u8 filler8[82];
    u8 unk5A;
    u8 filler5B[9];
    u16 unk64;
} T;

struct MuraSceneRecord {
    u8 pad[100];
    u16 field_0x64;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[80];
    u16 unk64;
    s16 unk66;
} T_0200154c;

struct Rec_399 {
    u8 pad00[9];
    u8 lo : 2;
    u8 mode : 2;
    u8 hi : 4;
};

struct Work_399 {
    u8 pad00[24];
    s32 f24;
    s32 f28;
    u8 pad20[3];
    u8 f35;
    u8 pad24[44];
    struct Rec_399 *f80;
    u8 pad54[1];
    u8 f85;
    u8 pad56[14];
    u16 f100;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
} T_020018c4;

typedef struct {
    u8 filler0[23];
    u8 unk17;
} State;

struct SharedData_02000240 {
    u8 pad_000[500];
    s32 selected_subject;
};

struct Subject_02001fa4 {
    u8 pad_000[6];
    u16 heading;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_020[14];
    u8 id;
    u8 pad_035[13];
    s32 state_048;
    s32 state_052;
    u8 pad_056[34];
    u8 flags_090;
    u8 pad_091[9];
    u16 state_100;
    u8 pad_102[6];
    void *callback;
};

extern s16 ImiruMura_KeyHeadings[];
extern u8 ImiruMura_CellStepsB[];
extern u8 ImiruMura_PrimaryScript[];
extern u8 ImiruMura_PrimaryScript2[];
extern s32 ImiruMura_ArcOrigin[];
extern u8 ImiruMura_SceneTable[];
extern u8 ImiruMura_CellStepsA[];
extern u8 ImiruMura_ActorScriptA[];
extern u8 ImiruMura_ActorScriptB[];
extern u8 ImiruMura_ActorScriptC[];
extern u8 ImiruMura_MiaScriptA[];
extern u8 ImiruMura_MiaScriptB[];
extern s16 ImiruMura_SwayHeadings[];
s32 ArcTan2(s32, s32);
void ObjectDispatch_ApplyValueToChildren(T *, s32);
void Object_SetActionCallbackAndRefreshById();
s32 Battle_GetWorkObject1e0();
void Object_RefreshSelectorById();
void BattleEffect_CleanupSceneObjects(void);
void SceneActor_TurnTowardTableAngle();
s32 GetMapCellCollision();
s32 Map_GetTerrainHeight();
void ObjectDispatch_ApplyValueToChildren();

s32 OverlayObject_UpdateWobbleByCounter(struct Object *obj);
s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj);
s32 SceneData_ReturnZero(void);
u8 *ImiruMura_GetExits(void);
void ImiruMura_RunHeadingProbe(void);
void SceneDialogue_RunActorEightFlagGatedDialogue(void);
void SceneDialogue_ShowLine1571Or152F(void);
void SceneDialogue_RunActor9Line(void);
void SceneDialogue_RunActorTenFlag881Dialogue(void);
void FieldScene_RunSupplementalSequenceOne(void);
void SceneDialogue_RunActor12Line(void);
void SceneDialogue_RunActor18Line(void);
void SceneDialogue_RunActor20BranchScene(void);
void SceneDialogue_RunActor20FlaggedLine(void);
void ImiruMura_TalkAboutLighthouse(void);
void SceneDialogue_RunActorEightBranchedDialogue(void);
void FieldScene_RunSingleStep(void);
void SceneDialogue_ShowLine156E(void);
void SceneDialogue_ShowLine1573Or155A(void);
void ImiruMura_OpenInn(void);
void ImiruMura_OpenSanctum(void);
void FieldScene_RunPrimaryScriptChoreography(void);
void SceneActor_UpdateCountdownArcPosition(T_0200154c *o);
void ImiruMura_ShowActorNineTurnMessage(void);
void ImiruMura_CastPsynergyAtActorNine(void);
void SceneActor_ResetActorAndCenterOffsets(struct Work_399 *work);
void FieldScene_RunSupplementalSequenceTwo(union FieldObject *object);
void FieldScene_RunScene399SequenceA(void);
void SceneState_UpdateZoneFlagsFromActorZero(void);
void SceneState_UpdateActor11WithFlag203(void);
void SceneActor_SetActorZeroFacingC000AndRun(void);
void RunEventScript02(void);
void FieldScene_RunThreeActorChoreography(void);
void SceneActor_TurnTowardTableAngle(s32 z);
void StagedActor_RunHeadingProbeStep(void);

#define FieldScene_PrepareActors ((void (*)(u8*))FieldScene_PrepareActors)

s32 OverlayObject_UpdateWobbleByCounter(struct Object *obj)
{
    switch (obj->counter) {
    case 6:
        obj->x += 0xffffc000;
        obj->z += 0x2000;
        break;
    case 4:
        obj->x += 0x2000;
        obj->z -= 0x1000;
        break;
    case 2:
        obj->x += 0x1000;
        obj->z += 0xfffff800;
        break;
    case 0:
        obj->x += 0x1000;
        obj->z += 0xfffff800;
        if (obj->mode != 0) {
            obj->counter = (u32)Engine_RandomNext() % 40 + 40;
        } else {
            obj->counter = (u32)Engine_RandomNext() % 20 + 20;
        }
        break;
    }
    obj->counter--;
    return 1;
}

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = obj->facing_target;
    if (target != NULL) {
        obj->facing_flags &= ~1;
        angle = (u16)ArcTan2(target->position_z - obj->position_z, target->position_x - obj->position_x);
        old = obj->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000)
                delta = 0x1000;
            if (delta < -0x1000)
                delta = -0x1000;
            obj->facing = old + delta;
        }
    }
    return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
    if (gGameState.scene == (s32)&SceneId_ImiruMura2) {
        return gImiruMuraEntrances2;
    }
    return gImiruMuraEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *ImiruMura_GetExits(void)
{
    return (u8 *)ImiruMura_SceneTable;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    u8 *script;

    if (gGameState.scene == ((s32)&SceneId_ImiruMura2)) {
        script = gImiruMuraPlacements2;
        FieldScene_PrepareActors(script);
        if (Engine_GameFlagIsSet(0x881) != 0) {
            script[262] = 0;
            *(s32 *)(script + 0x50) = 182 << 16;
            *(s32 *)(script + 0x58) = 564 << 16;
            *(s32 *)(script + 0x4c) = 2;
        }
        return (const struct ScenePlacement *)script;
    }

    if (Engine_GameFlagIsSet(0x881) != 0) {
        return gImiruMuraPlacementsFlag881;
    }
    return gImiruMuraPlacements;
}

void ImiruMura_RunHeadingProbe(void)
{
    StagedActor_RunHeadingProbeStep();
}

const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_ImiruMura2) {
        return gImiruMuraEvents2;
    }
    return gImiruMuraEventsOther;
}

void SceneDialogue_RunActorEightFlagGatedDialogue(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(3) != 0) {
        Engine_EventSetMessage(MSGNO(MsgImiruEvenIfMaryHealsUs));
    } else {
        Engine_EventSetMessage(MSGNO(MsgImiruBrrrrrChooImFreezingMary));
    }
    {
        s32 val = 0;
        s32 mode = 8;
        Engine_EventShowMessage(mode, val);
    }
    Engine_EventEnd();
}

void SceneDialogue_ShowLine1571Or152F(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(3) != 0) {
        Engine_EventSetMessage(MSGNO(MsgImiruCantUnderstandWhyAnyOne));
    } else {
        Engine_EventSetMessage(MSGNO(MsgImiruMaryShouldHereByNow));
    }
    Engine_EventShowMessage(8, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor9Line(void)
{
    Engine_EventBegin();
    Engine_ActorFaceActor(9, ACTOR_PARTY_LEADER, 10);
    Engine_EventSetMessage(MSGNO(MsgImiruHiNewInImil));
    Engine_EventAskYesNo(9, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActorTenFlag881Dialogue(void)
{
    Engine_EventBegin();

    if (GameFlag_IsSet(0x881) != 0) {
        Engine_EventSetMessage(MSGNO(MsgImiruCountingAfterDeparture));
        Event_ShowMessage(10, 0);
        Actor_SetAttachedEffect(10, 258);
        Engine_EventWait(40);
        Engine_ActorSetAnimation(10, 1);
        Engine_EventWait(20);
        Actor_FaceActor(10, ACTOR_PARTY_LEADER, 20);
        Event_AskYesNo(10, 0);
        Actor_FaceDirection(10, 0x3000, 10);
        Engine_ActorSetAnimation(10, 9);
    } else {
        Engine_EventSetMessage(MSGNO(MsgImiruOneTwoThreeFour));
        Event_ShowMessage(10, 0);
        Actor_SetAttachedEffect(10, 258);
        Engine_EventWait(40);
        Engine_ActorSetAnimation(10, 1);
        Engine_EventWait(20);
        Actor_FaceActor(10, ACTOR_PARTY_LEADER, 20);
        Event_ShowMessage(10, 0);
        Actor_FaceDirection(10, 0x3000, 10);
        Engine_ActorSetAnimation(10, 9);
    }

    Engine_EventEnd();
}

void FieldScene_RunSupplementalSequenceOne(void)
{

    void *actor9_record;
    struct FieldActor *actor8_record;

    if (GameFlag_IsSet(2177) != 0) {
        Engine_EventBegin();
        Actor_FaceActor(9, 0, 0);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgMakyuriReallySayDie);
        Event_AskYesNo(9, 0);
        Engine_EventEnd();
    } else {
        if (GameFlag_IsSet(2091) != 0) {
            Engine_EventBegin();
            Engine_ActorSetAnimation(9, 7);
            Engine_MapAnimateCells((s32)ImiruMura_CellStepsA, 10, 69);
            Engine_EventSetMessage((s32)MsgMakyuriFeelMuchBetter);
            Event_ShowMessage(9, 0);
            Engine_ActorSetAnimation(9, 8);
            Map_AnimateCells((s32)ImiruMura_CellStepsB, 10, 69);
            Engine_EventEnd();
        } else {
            Engine_EventBegin();
            actor9_record = Object_GetById(9);
            ((struct MuraSceneRecord *)actor9_record)->field_0x64 = 10;
            Engine_ActorEnableActionCallback(9, ImiruMura_ActorScriptA);
            Engine_EventSetMessage((s32)MsgMakyuriOoohHelpMe);
            Engine_EventShowMessage(9, 0);
            Engine_ActorStop(8);
            Actor_ShowEmote(8, 256, 40);
            Actor_FaceDirection(8, 53248, 10);
            Engine_ActorStartRepeatedMotion(8, 2);
            Event_ShowMessageAndWait(8, 0, 20);
            Engine_ActorEnableActionCallback(0, ImiruMura_ActorScriptC);
            Actor_SetSpeed(8, 104857, 52428);
            Object_SetActionCallbackAndRefreshById(8, (s32)ImiruMura_ActorScriptB);
            Engine_EventWait(40);
            Engine_ActorJump(8, 2, 0);
            Engine_ActorStartRepeatedMotion(8, 2);
            Engine_ActorSetAttachedEffect(8, 258);
            Engine_EventWait(60);
            Event_ShowMessageAndWait(8, 0, 10);
            Actor_FaceDirection(8, 12288, 20);
            Engine_ActorStartRepeatedMotion(8, 2);
            Engine_EventShowMessage(8, 0);
            actor8_record = Object_GetById(8);
            *(u8 *)((u8 *)(actor8_record) + ACTOR_FLAGS_OFFSET) ^= 0x2;
            GameFlag_Set(0x82c);
            Engine_EventEnd();
        }
    }
}

void SceneDialogue_RunActor12Line(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage(MSGNO(MsgImiruDoWantWeapons));
    Engine_EventAskYesNo(12, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor18Line(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage(MSGNO(MsgImiruDoWantSeeRestaurantMenu));
    Engine_EventAskYesNo(18, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor20BranchScene(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(3) != 0) {
        Engine_EventSetMessage(MSGNO(MsgImiruEveryoneCountsOnMaryThats));
        Engine_EventShowMessage(20, 0);
    } else {
        Engine_EventSetMessage((s32)MsgImiruHaveVisitedOldCoupleWho);
        Engine_EventAskYesNo(20, 0);
        Engine_GameFlagSet(0x82a);
        Engine_GameFlagSet(0x82c);
    }
    Engine_EventEnd();
}

void SceneDialogue_RunActor20FlaggedLine(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(3) != 0) {
        Engine_EventSetMessage(MSGNO(MsgImiruWeHaveDoWhateverWe));
    } else {
        Engine_EventSetMessage(MSGNO(MsgImiruMaryWasSayingSheHas));
    }
    Engine_EventShowMessage(20, 0);
    Engine_EventEnd();
}

void ImiruMura_TalkAboutLighthouse(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventSetMessage(MSGNO(MsgImiruHappenedInLighthouseNortheast));
    Event_ShowMessage(8, 0);
    Actor_FaceDirection(8, 0x3000, 10);
    Engine_EventEnd();
}

void SceneDialogue_RunActorEightBranchedDialogue(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x82b) != 0) {
        Engine_EventSetMessage(MSGNO(MsgImiruMaryClanOnceLivedHere));
    } else if (Engine_GameFlagIsSet(0x82c) != 0) {
        Engine_EventSetMessage(MSGNO(MsgImiruHesAlwaysExaggeratingThingsBut));
    } else {
        Engine_EventSetMessage(MSGNO(MsgImiruMaryRunningAroundTownCaring));
    }
    Engine_EventShowMessage(8, 0);
    Engine_EventEnd();
}

void FieldScene_RunSingleStep(void)
{
    FieldScene_RunSupplementalSequenceOne();
}

void SceneDialogue_ShowLine156E(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage(MSGNO(MsgImiruMuraMaryIsAGoodGirlI));
    Engine_EventShowMessage(10, 0);
    Engine_EventEnd();
}

void SceneDialogue_ShowLine1573Or155A(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(3) != 0) {
        Engine_EventSetMessage(MSGNO(MsgImiruFeelLikeGrownUpWhen));
    } else {
        Engine_EventSetMessage(MSGNO(MsgImiruTheseFolkOkayTheyDont));
    }
    Engine_EventShowMessage(19, 0);
    Engine_EventEnd();
}

void ImiruMura_RunExitDoor(void)
{
    struct EventWork *event;
    struct FieldActor *actor;
    u32 i;
    s32 exit;

    event = gEventWork;
    Engine_EventBegin();
    for (i = 8; i <= 65; i++) {
        actor = Object_GetById(i);
        if (actor != NULL) {
            actor->motion_flags = 0;
        }
    }
    exit = (s16)(event->touched_trigger - 50);
    if (exit == 6) {
        Engine_AudioPlayCue(188);
    } else {
        Engine_AudioPlayCue(158);
    }
    {
        s32 x = ImiruMura_ExitCellPoints[exit - 1][0];
        s32 y = ImiruMura_ExitCellPoints[exit - 1][1];

        Engine_MapAnimateCells(ImiruMura_ExitCellSteps[exit - 1], x, y);
    }
    Actor_SetSpeed(0, 0x8000, 0x4000);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    if (exit == 6) {
        Actor_SetSpeed(0, 0x3333, 0x1999);
        Engine_ActorSetAnimation(0, 2);
        Engine_ActorSetSpritePriority(0, 3);
        Actor_SetDestinationOffset(0, 0, -8);
    } else {
        Object_GetById(0)->motion_flags = 0;
        Actor_CenterAndWalk(0, 3, -16);
    }
    Engine_EventWait(16);
    Engine_EventRequestExit(exit);
    Engine_EventEnd();
}

void ImiruMura_RunWeaponShop(void)
{
    s32 dir;

    dir = *(s16 *)(((s32 (*)())Object_GetById)(0) + 6);
    if (Engine_GameFlagIsSet(0x881) != 0) {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_ShopOpen(10, 12);
            return;
        }
        Engine_EventBegin();
        Engine_ActorFaceActor(12, 0, 0);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgMakyuriCatchUpLostOpportunity);
        Engine_EventShowMessage(12, 0);
        Actor_FaceDirection(12, 0x4000, 10);
        Engine_EventEnd();
    } else {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_EventBegin();
            Engine_CameraSetSpeed(0x60000, 0xc000);
            Camera_MoveTo(0x1aa0000, -1, 0x1ec0000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(20);
            Object_SetActionCallbackAndRefreshById(12, (s32)ImiruMura_TurnScript);
            Engine_EventSetMessage((s32)MsgMakyuriStoreClosedUntilWell);
            Engine_EventShowMessage(12, 0);
            Engine_CameraMoveTo(0x1aa0000, -1, 0x2680000, 1);
            Engine_CameraWaitForMove();
            Engine_EventEnd();
        }
    }
}

void ImiruMura_RunArmorShop(void)
{
    s32 dir;

    dir = *(s16 *)(((s32 (*)())Object_GetById)(0) + 6);
    if (Engine_GameFlagIsSet(0x881) != 0) {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_ShopOpen(11, 13);
            return;
        }
        Engine_EventBegin();
        Engine_ActorFaceActor(13, 0, 0);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgMakyuriShameStoreClosed);
        Engine_EventShowMessage(13, 0);
        Actor_FaceDirection(13, 0x4000, 10);
        Engine_EventEnd();
    } else {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_EventBegin();
            Engine_CameraSetSpeed(0x60000, 0xc000);
            Camera_MoveTo(0x1aa0000, -1, 0x1ec0000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(20);
            Object_SetActionCallbackAndRefreshById(13, (s32)ImiruMura_TurnScript);
            Engine_EventSetMessage((s32)MsgMakyuriTooSickBusiness);
            Engine_EventShowMessage(13, 0);
            Engine_CameraMoveTo(0x1aa0000, -1, 0x2680000, 1);
            Engine_CameraWaitForMove();
            Engine_EventEnd();
        }
    }
}

void ImiruMura_RunItemShop(void)
{
    s32 dir;

    dir = *(s16 *)(((s32 (*)())Object_GetById)(0) + 6);
    if (Engine_GameFlagIsSet(0x881) != 0) {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_ShopOpen(12, 15);
            return;
        }
        Engine_EventBegin();
        Engine_ActorFaceActor(15, 0, 0);
        Engine_EventSetMessage((s32)MsgMakyuriRecoveredThanksFountain);
        Engine_EventShowMessage(15, 0);
        Actor_FaceDirection(15, 0x4000, 0);
        Engine_EventEnd();
    } else {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_EventBegin();
            Engine_EventSetMessage((s32)MsgMakyuriMomsSickGetWhatever);
            Engine_EventShowMessage(14, 0);
            ((s32 (*)())Engine_ShopOpen)(12, 14);
            Engine_EventEnd();
        } else {
            Engine_ActorFaceActor(14, 0, 10);
            Engine_EventSetMessage((s32)MsgMakyuriDadLumberjack);
            Engine_EventShowMessage(14, 0);
            Actor_FaceDirection(14, 0x5000, 10);
        }
    }
}

void ImiruMura_OpenInn(void)
{
    struct FieldActor *leader;

    leader = Object_GetById(0);
    if ((u16)(leader->facing + 0x5fff) <= 0x3ffe) {
        Engine_InnOpen(4, 16);
    } else {
        Engine_EventBegin();
        Actor_FaceActor(16, ACTOR_PARTY_LEADER, 10);
        if (GameFlag_IsSet(0x881) != 0) {
            Engine_EventSetMessage(MSGNO(MsgImiruItsAlmostTimeForLeave));
            Event_AskYesNo(16, 0);
        } else {
            Engine_EventSetMessage(MSGNO(MsgImiruWhyHaveTwoGroupsTravelers));
            Event_ShowMessage(16, 0);
        }
        Actor_FaceDirection(16, 0x3000, 10);
        Engine_EventEnd();
    }
}

void ImiruMura_OpenSanctum(void)
{
    struct FieldActor *leader;

    leader = Object_GetById(0);
    if ((u16)(leader->facing + 0x5fff) <= 0x3ffe) {
        Engine_EventBegin();
        if (GameFlag_IsSet(0x82d) == 0) {
            Engine_EventSetMessage(MSGNO(MsgImiruMayOnlyStudentButCan));
            Event_ShowMessage(19, 0);
            GameFlag_Set(0x82d);
        }
        Engine_EventEnd();
        Engine_SanctumOpen(19);
    } else {
        Engine_EventBegin();
        if (GameFlag_IsSet(0x881) != 0) {
            Engine_EventSetMessage(MSGNO(MsgImiruMaryGoingOnJourneyWith));
            Event_ShowMessage(19, 0);
        } else if (GameFlag_IsSet(3) != 0) {
            Engine_EventSetMessage(MSGNO(MsgImiruAmHealerWhileMaryOut));
            Event_ShowMessage(19, 0);
        } else {
            Engine_EventSetMessage(MSGNO(MsgImiruLookingForMary));
            (void)Event_AskYesNo(19, 0);
            Actor_FaceDirection(19, 0x3000, 10);
        }
        Engine_EventEnd();
    }
}

s32 ImiruMura_ApplyEntryState(void)
{
    struct FieldActor *leader;
    s32 entrance;

    if (gGameState.scene == (s32)&SceneId_ImiruMura1) {
        leader = Object_GetById(0);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        Engine_ActorSetAnimation(10, 9);
        if (GameFlag_IsSet(0x109)) {
            Engine_GameFlagClear(0x200);
            Engine_GameFlagClear(0x201);
        }
        leader->timer = 0;
        leader->work = 0;
        Task_AddCallback(FieldScene_RunScene399SequenceA, 0xc80);
        Engine_TaskAddCallback(SceneState_UpdateZoneFlagsFromActorZero, 0xc80);
        Engine_ActorSetSpritePriority(11, 1);
        if (Engine_GameFlagIsSet(0x203)) {
            SceneState_UpdateActor11WithFlag203();
        }
        if (!Engine_GameFlagIsSet(0x109) && gGameState.entrance == 9) {
            RunEventScript02();
        }
    } else if (gGameState.scene == (s32)&SceneId_ImiruMura2) {
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
        entrance = gGameState.entrance;
        if (entrance == 1) {
            Engine_ActorSetChildValue(21, 15);
            Object_GetById(21)->collision_flags |= 8;
            Engine_ActorSetSpritePriority(21, 1);
            if (Engine_GameFlagIsSet(0x881)) {
                Map_CopyCellAttributes(10, 7, 1, 1, 10, 8);
                Engine_MapCopyCellsTo(3, 125, 9, 69, 3, 3);
                Engine_MapRedraw();
                Engine_TaskWait(1);
                Engine_ActorEnableActionCallback(8, (const u8 *)2);
                Engine_ActorSetPosition(10, 0, 0);
            } else if (Engine_GameFlagIsSet(0x82c) && Engine_GameFlagIsSet(0x82a)) {
                Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
                Engine_ActorSetPosition(9, 0xae0000, 0xa40000);
                Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
                Engine_ActorSetAnimation(9, 5);
                Actor_SetPosition(8, 0xa80000, 0x980000);
                Object_GetById(8)->facing = 0x3000;
                if (!Engine_GameFlagIsSet(0x82b)) {
                    FieldScene_RunPrimaryScriptChoreography();
                }
            } else {
                Map_CopyCellAttributes(10, 7, 1, 1, 10, 8);
                Engine_MapCopyCellsTo(3, 125, 9, 69, 3, 3);
                Engine_MapRedraw();
                Engine_TaskWait(1);
                if (Engine_GameFlagIsSet(0x82c)) {
                    Engine_ActorSetPosition(8, 0x950000, 0x740000);
                    Object_GetById(8)->facing = 0;
                    Object_GetById(9)->work = 0;
                    Engine_ActorEnableActionCallback(9, ImiruMura_ActorScriptA);
                } else {
                    Engine_ActorEnableActionCallback(8, (const u8 *)2);
                }
            }
        } else if (entrance == 2) {
            if (!Engine_GameFlagIsSet(0x881)) {
                Object_GetById(11)->work = 1;
                Engine_ActorEnableActionCallback(11, ImiruMura_ActorScriptA);
            }
        } else if (entrance == 4) {
            if (Engine_GameFlagIsSet(0x881)) {
                Actor_SetPosition(12, 0x16c0000, 0x2420000);
                Engine_ActorSetSpritePriority(12, 2);
                Object_GetById(12)->collision_flags |= 4;
                Engine_MapCopyCellsTo(6, 125, 22, 88, 3, 3);
                Actor_SetPosition(13, 0x1ec0000, 0x2420000);
                Engine_ActorSetSpritePriority(13, 2);
                Object_GetById(13)->collision_flags |= 4;
                Engine_MapCopyCellsTo(9, 125, 28, 88, 3, 3);
            } else {
                Object_GetById(12)->scale_x = -0x10000;
                Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
                Engine_ActorSetAnimation(12, 5);
                Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
                Engine_ActorSetAnimation(13, 5);
            }
        } else if (entrance == 3) {
            if (Engine_GameFlagIsSet(0x881)) {
                Actor_SetPosition(15, 0x1cc0000, 0x1020000);
                Engine_ActorSetSpritePriority(15, 2);
                Object_GetById(15)->collision_flags |= 4;
                Engine_ActorSetPosition(14, 0x1980000, 0x1080000);
                Object_GetById(14)->facing = 0x1000;
                Engine_MapCopyCellsTo(12, 125, 26, 70, 3, 3);
            } else {
                Actor_SetPosition(14, 0x1cc0000, 0x1020000);
                Engine_ActorSetSpritePriority(14, 2);
                Object_GetById(14)->collision_flags |= 4;
                Object_GetById(15)->scale_x = -0x10000;
                Engine_ActorSetSpriteFlags(Object_GetById(15), 0);
                Engine_ActorSetAnimation(15, 5);
            }
        } else if (entrance == 7) {
            if (Engine_GameFlagIsSet(0x881)) {
                Object_GetById(20)->facing = 0x3000;
                if (!Engine_GameFlagIsSet(0x82e)) {
                    Actor_SetPosition(20, 0x28a0000, 0xa10000);
                    FieldScene_RunThreeActorChoreography();
                } else {
                    Actor_SetPosition(20, 0x2840000, 0xa60000);
                }
            }
        }
    }
    return 0;
}

void FieldScene_RunPrimaryScriptChoreography(void)
{
    struct FieldActor *leader;
    s32 tbl;
    struct EventWork *work;

    Engine_EventBegin();
    Actor_SetPosition(MEARI, 0xb60000, 0x960000);
    Camera_MoveTo(0x8d0000, -1, 0xdd0000, 0);
    Engine_TaskWait(1);
    Camera_SetSpeed(0x4ccc, 0x999);
    Camera_MoveTo(0x8c0000, -1, 0xa40000, 1);
    work = gEventWork;
    work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transition_frames = 40;
    Engine_EventOpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Actor_SetSpeed(GERALD, 0x6666, 0x3333);
    Actor_SetSpeed(IWAN, 0x6666, 0x3333);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 142, 221);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    leader = Object_GetById(0);
    if (leader != NULL) {
        Actor_SetPosition(GERALD, leader->x.fixed, leader->z.fixed);
    }
    leader = Object_GetById(0);
    if (leader != NULL) {
        Actor_SetPosition(IWAN, leader->x.fixed, leader->z.fixed);
    }
    Actor_WalkTo(GERALD, 150, 234);
    Actor_WalkToAndWait(IWAN, 134, 234);
    Engine_ActorSetAnimation(GERALD, 1);
    FieldObject_SetTargetAndCallback(0, 0x10003, ImiruMura_PrimaryScript);
    FieldObject_SetTargetAndCallback(1, 0x10003, ImiruMura_PrimaryScript);
    FieldObject_SetTargetAndCallback(2, 0x10003, ImiruMura_PrimaryScript);
    Engine_CameraWaitForMove();
    tbl = (s32)ImiruMura_TurnScript;
    Engine_ActorEnableActionCallback(9, tbl);
    Engine_EventWait(40);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    Engine_EventSetMessage(MSGNO(MsgImiruHowFeeling));
    Event_ShowMessageAndWait(MEARI, 0, 20);
    Engine_ActorEnableActionCallback(9, tbl);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(MEARI, 0x8000, 20);
    Actor_FaceDirection(8, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 40);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(10);
    Actor_FaceDirection(MEARI, 0x4000, 0);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(MEARI, 0, 10);
    Engine_ActorEnableActionCallback(9, tbl);

    ImiruMura_CastPsynergyAtActorNine();
    Actor_ShowEmote(MEARI, 0x101, 60);
    Event_ShowMessageAndWait(MEARI, 0, 40);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Engine_ActorSetAnimation(9, 7);
    Map_AnimateCells((s32)ImiruMura_CellStepsA, 10, 69);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(MEARI, 2);
    Engine_ActorSetAnimationAndWait(MEARI, 4);
    Event_ShowMessageAndWait(MEARI, 0, 20);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(9, 8);
    Map_AnimateCells((s32)ImiruMura_CellStepsB, 10, 69);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(20);
    Actor_FaceDirection(8, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(MEARI, 0x101, 30);
    Actor_FaceDirection(MEARI, 0x8000, 10);
    Engine_ActorSetAnimation(MEARI, 4);
    Event_ShowMessageAndWait(MEARI, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(40);
    Actor_SetSpeed(MEARI, 0x10000, 0x8000);
    ((struct Work_399 *)((s32)Object_GetById(3)))->f100 = 0;
    Engine_ActorEnableActionCallback(MEARI, ImiruMura_MiaScriptA);
    while (*(s16 *)(((s32)Object_GetById(3)) + ACTOR_DONE_OFFSET) == 0) {
        Engine_TaskWait(1);
    }
    Camera_MoveTo(0x8c0000, -1, 0xc60000, 1);
    Object_RefreshSelectorById(3);
    Actor_ShowEmote(MEARI, 0x101, 80);
    Event_ShowMessageAndWait(MEARI, 0, 40);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    Engine_EventWait(10);
    Event_ShowMessage(MEARI, 0);
    Event_PlayCue(131);
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x207e9f, 0);
    Engine_ColorBufferInterpolate(10);
    Engine_TaskWait(1);
    Event_PlayCue(220);
    Engine_TaskWait(40);
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(60);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Engine_EventWait(20);
    Actor_FaceDirection(MEARI, 0, 10);
    Actor_SetSpeed(MEARI, 0x20000, 0x10000);
    Actor_WalkToAndWait(MEARI, 202, 198);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(MEARI, 2);
    Event_ShowMessage(MEARI, 0);
    Engine_ActorSetAnimationAndWait(MEARI, 4);
    Event_ShowMessageAndWait(MEARI, 0, 20);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(MEARI, 0, 40);
    Actor_ShowEmote(MEARI, 0x100, 40);
    Event_ShowMessage(MEARI, 0);
    Engine_ActorStop(ACTOR_PARTY_LEADER);
    Engine_ActorStop(GERALD);
    Engine_ActorStop(IWAN);
    Actor_SetSpeed(MEARI, 0x30000, 0x18000);
    ((struct Work_399 *)((s32)Object_GetById(3)))->f100 = 0;
    Engine_ActorEnableActionCallback(MEARI, ImiruMura_MiaScriptB);
    while (*(s16 *)(((s32)Object_GetById(3)) + ACTOR_DONE_OFFSET) == 0) {
        Engine_TaskWait(1);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(GERALD, 0x4000, 0);
    Actor_FaceDirection(IWAN, 0x4000, 10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x40000, 0x20000);
    Actor_SetSpeed(GERALD, 0x40000, 0x20000);
    Actor_SetSpeed(IWAN, 0x40000, 0x20000);
    Event_PlayCue(152);
    (Object_GetById(0))->steering_flags &= ~ACTOR_FACES_MOTION;
    (Object_GetById(1))->steering_flags &= ~ACTOR_FACES_MOTION;
    (Object_GetById(2))->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_SetDestination(ACTOR_PARTY_LEADER, 132, 206);
    Actor_SetDestination(GERALD, 136, 221);
    Actor_SetDestination(IWAN, 122, 238);
    Object_RefreshSelectorById(3);
    Engine_EventWait(80);
    (Object_GetById(0))->steering_flags |= ACTOR_FACES_MOTION;
    (Object_GetById(1))->steering_flags |= ACTOR_FACES_MOTION;
    (Object_GetById(2))->steering_flags |= ACTOR_FACES_MOTION;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
    tbl = (s32)ImiruMura_PrimaryScript2;
    Engine_ActorEnableActionCallback(GERALD, tbl);
    Object_SetActionCallbackAndRefreshById(2, tbl);
    Engine_EventWait(20);
    work = gEventWork;
    work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    work->transition_frames = 24;
    Engine_GameFlagSet(0x82b);
    Engine_EventEnd();
}

void SceneActor_UpdateCountdownArcPosition(T_0200154c *o)
{
    s32 buf[3];
    s32 *b;
    s32 n;
    s32 t;

    if (o != 0) {
        n = o->unk64 - 1;
        o->unk64 = n;
        t = (s16)n;
        if (t != 0) {
            b = buf;
            b[0] = ImiruMura_ArcOrigin[0];
            b[1] = ImiruMura_ArcOrigin[1] + 0x80000;
            b[2] = ImiruMura_ArcOrigin[2];
            Vector_AddPolarOffset(t << 16, (t << 11) + o->unk66, b);
            o->unk8 = b[0];
            o->unkC = b[1];
            o->unk10 = b[2];
        } else {
            Engine_ObjectDispatchRelease(o);
        }
    }
}

void ImiruMura_SwayAndSpark(struct FieldActor *actor)
{
    s32 pos[3];
    struct FieldActor *spark;
    s32 radius;

    actor->x.fixed = ImiruMura_ArcOrigin[0] + Iwram_MulQ16(0x60000, Engine_MathSin(actor->timer << 10));
    actor->timer++;
    actor->timer = (actor->timer + 64) % 64;
    if (gFrameCount % 3 == 0) {
        pos[0] = actor->x.fixed;
        pos[1] = actor->y.fixed + 0x20000;
        pos[2] = actor->z.fixed;
        radius = Engine_RandomNext();
        Vector_AddPolarOffset(radius * 6, Engine_RandomNext(), pos);
        spark = Engine_ObjectCreate(0x11d, pos[0], pos[1], pos[2]);
        if (spark != NULL) {
            spark->sprite->priority = 0;
            Engine_ActorSetSpriteFlags(spark, 0);
            Object_SetMode(spark, 1);
            spark->scale_x = 0x9999;
            spark->scale_y = 0x9999;
            spark->priority_flags = 2;
            spark->motion_flags = 0;
            ObjectGroup_SetChildValue(spark, 9);
            Engine_ObjectSetScript(spark, ImiruMura_SparkScript);
        }
    }
}

s32 __umodsi3();

void ImiruMura_ShowActorNineTurnMessage(void)
{
    extern s32 ImiruMura_TurnScript[];

    Engine_ActorEnableActionCallback(9, (s32)ImiruMura_TurnScript);
    Engine_EventShowMessage(9, 0);
}

void ImiruMura_CastPsynergyAtActorNine(void)
{
    u8 *work;

    Engine_PsynergyBegin(93, 1);
    work = *(u8 **)gEffectWork;
    Engine_PsynergySetTarget(3, 9);
    *(s32 *)(work + 36) = (s32)ImiruMura_ShowActorNineTurnMessage;
    Engine_PsynergyRaiseHands();
    Engine_PsynergyPlayEffect(1);
    Engine_PsynergyLowerHands();
    BattleEffect_CleanupSceneObjects();
}

void SceneActor_ResetActorAndCenterOffsets(struct Work_399 *work)
{
    struct Rec_399 *rec;

    work->f85 = 0;
    work->f100 = 0;
    work->f35 &= ~1;

    rec = work->f80;
    rec->mode = 1;

    ObjectGroup_SetChildValue(work, 9);
    Engine_ActorSetSpriteFlags(work, 0);

    work->f24 = 0x8000;
    work->f28 = 0x8000;
}

void FieldScene_RunSupplementalSequenceTwo(union FieldObject *object)
{
    object->effect.x += (s16)object->effect.spin << 8;
    object->effect.y += 0x8000;
    object->effect.scale_x += 0x7ae;
    object->effect.scale_y += 0x7ae;
    object->effect.spin += 2;
    if (--object->effect.countdown == 0) {
        Engine_ObjectDispatchRelease(object);
    }
}

void FieldScene_RunScene399SequenceA(void)
{
    volatile u32 *frame;
    union FieldObject *object;

    frame = &gFrameCount;
    if (__umodsi3(*frame, 60) == 0) {
        object = (union FieldObject *)Object_Create(222, 0x1cf0000, 0, 0x1240000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetMode(object, 5);
        }
    }
    if (__umodsi3(*frame + 30, 60) == 0) {
        object = (union FieldObject *)Object_Create(222, 0x1400000, 0x200000, 0x1640000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetMode(object, 5);
        }
    }
    if (__umodsi3(*frame + 10, 60) == 0) {
        object = (union FieldObject *)Object_Create(222, 0x760000, 0, 0x460000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetMode(object, 5);
        }
    }
    if (__umodsi3(*frame + 50, 60) == 0) {
        object = (union FieldObject *)Object_Create(222, 0x1560000, 0, 0x7c0000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetMode(object, 5);
        }
    }
    if (__umodsi3(*frame + 80, 60) == 0) {
        object = (union FieldObject *)Object_Create(222, 0x1af0000, 0, 0xab0000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetMode(object, 5);
        }
    }
}

void SceneState_UpdateZoneFlagsFromActorZero(void)
{
    T_020018c4 *obj;
    s32 x;
    s32 cx;
    s32 y;
    s32 r;
    s32 g;
    s32 h;
    State *st;

    obj = ((T *)Object_GetById(0));
    x = obj->unk8;
    cx = x >> 19;
    g = 0x200;
    h = 0x201;
    if ((u32)(cx - 24) > 7) {
        y = obj->unk10;
        if ((u32)((y >> 19) - 36) > 9 || (u32)(cx - 22) > 9)
            goto rest;
    }
    r = Engine_GameFlagIsSet(g);
    if (r != 0)
        return;
    ((State *)gMapWork[0])->unk17 = r;
    ((void (*)(s32))Engine_GameFlagSet)(g);
    Engine_GameFlagClear(h);
    return;

rest:
    if (x > 0xE80000 && obj->unkC > 0x1E0000 && y > 0xD40000) {
        st = gMapWork[0];
        st->unk17 = 0;
        ((void (*)(s32))Engine_GameFlagSet)(g);
        Engine_GameFlagClear(h);
        return;
    }
    r = Engine_GameFlagIsSet(h);
    if (r != 0)
        return;
    st = gMapWork[0];
    st->unk17 = 1;
    ((void (*)(s32))Engine_GameFlagSet)(h);
    Engine_GameFlagClear(g);
    return;
}

void SceneState_UpdateActor11WithFlag203(void)
{
    s32 a;
    s32 b;

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    struct FieldActor *actor = Object_GetById(11);

    a = actor->x.fixed >> 20;
    if (a != 15)
        return;
    b = actor->z.fixed >> 20;
    if (b != 7)
        return;
    Engine_GameFlagSet(0x203);
    Engine_ActorSetSpritePriority(11, 3);
#else
    Engine_GameFlagSet(0x203);
    Engine_ActorSetSpritePriority(11, 3);
    a = 15;
    b = 7;
#endif
    Engine_MapCopyCellAttributes(15, 6, 1, 1, a, b);
}

void SceneActor_SetActorZeroFacingC000AndRun(void)
{
    struct SceneService_02001990 *work;

    Engine_EventBegin();
    work = ((struct SceneService_02001990 *)Object_GetById(0));
    work->value06 = 0xc000;
    Engine_AudioPlayCue(123);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(8);
}

void RunEventScript02(void)
{
    u32 i;
    u8 *record;

    Engine_EventBegin();
    Engine_EventOpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x1999);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 232, 204);
    Engine_EventEnd();
}

void FieldScene_RunThreeActorChoreography(void)
{

    struct FieldActor *leader;
    struct EventWork *work;

    Engine_EventBegin();
    Actor_FaceDirection(MEARI, 0xa000, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Engine_ActorWalkTo(0, 0x2b2, 200);

    ((struct FieldActor *)Battle_GetWorkObject1e0())->motion_flags = 0;
    Engine_CameraSetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x2b20000, 0, 0xa40000, 1);
    work = gEventWork;
    work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transition_frames = 48;
    Engine_EventOpenScreen();
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_SetSpeed(MEARI, 0x9999, 0x4ccc);
    leader = Object_GetById(0);
    if (leader != NULL) {
        Actor_SetPosition(MEARI, leader->x.fixed, leader->z.fixed);
    }
    Actor_WalkToAndWait(MEARI, 0x2a1, 183);
    Actor_FaceDirection(MEARI, 0xc000, 0);
    Engine_ActorStartRepeatedMotion(19, 2);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(40);
    Engine_EventSetMessage(MSGNO(MsgImiruMary));
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(MEARI, 0xe000, 40);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Actor_SetAttachedEffect(20, 0x102);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x4014, 0, 10);
    Actor_FaceDirection(MEARI, 0xa000, 40);
    Engine_ActorSetAnimationAndWait(MEARI, 4);
    Event_ShowMessageAndWait(0x2003, 0, 10);
    Engine_ActorSetAnimationAndWait(20, 3);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(19, 0x102);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_FaceDirection(MEARI, 0xf000, 10);
    Actor_FaceDirection(MEARI, 0x2000, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(MEARI, 0xc000, 40);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x4014, 0, 20);
    Actor_FaceDirection(MEARI, 0xa000, 20);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(60);
    Actor_ShowEmote(MEARI, 0x105, 60);
    Actor_ShowEmote(19, 0x101, 0);
    Actor_ShowEmote(20, 0x101, 60);
    Engine_ActorRunRepeatedMotion(19, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(MEARI, 0xe000, 40);
    Actor_FaceDirection(MEARI, 0xa000, 40);
    Actor_FaceDirection(MEARI, 0xe000, 20);
    Actor_FaceDirection(MEARI, 0x6000, 80);
    Event_ShowMessageAndWait(0x2003, 0, 20);
    Actor_FaceDirection(20, 0xf000, 0);
    Actor_FaceDirection(19, 0x7000, 40);
    Actor_FaceDirection(19, 0x5000, 0);
    Actor_FaceDirection(20, 0x3000, 20);
    Actor_SetSpeed(20, 0x10000, 0x8000);
    Object_GetById(20)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(20, 0x290, 166);
    Engine_EventWait(1);
    Object_GetById(20)->steering_flags |= ACTOR_FACES_MOTION;
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x4014, 0, 10);
    Engine_ActorRunRepeatedMotion(MEARI, 2);
    Engine_EventWait(40);
    Actor_FaceDirection(MEARI, 0xa000, 10);
    Event_ShowMessageAndWait(0x2003, 0, 40);
    Actor_FaceDirection(MEARI, 0x2000, 20);
    Event_ShowMessageAndWait(0x4003, 0, 10);
    Actor_SetAttachedEffect(19, 0x102);
    Actor_SetAttachedEffect(20, 0x102);
    Engine_EventWait(40);
    Actor_FaceDirection(MEARI, 0xc000, 20);
    Engine_ActorSetAnimation(MEARI, 4);
    Event_ShowMessageAndWait(0x2003, 0, 20);
    Engine_ActorRunRepeatedMotion(19, 1);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(MEARI, 0x2000, 40);
    Actor_FaceDirection(MEARI, 0xc000, 20);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(20, 1);
    Event_ShowMessageAndWait(0x4014, 0, 20);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    Engine_EventWait(20);
    Actor_FaceDirection(MEARI, 0xa000, 20);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Event_ShowMessageAndWait(0x2003, 0, 80);
    Actor_ShowEmote(19, 0x105, 0);
    Actor_ShowEmote(20, 0x105, 60);
    Engine_ActorSetAnimationAndWait(19, 4);
    Event_ShowMessageAndWait(19, 0, 10);
    Engine_ActorSetAnimation(20, 4);
    Event_ShowMessageAndWait(0x4014, 0, 20);
    Actor_ShowEmote(MEARI, 0x102, 60);
    Event_ShowMessageAndWait(0x2003, 0, 40);
    Engine_ActorSetAnimationAndWait(19, 3);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(MEARI, 0xe000, 20);
    Engine_ActorSetAnimationAndWait(20, 3);
    Event_ShowMessageAndWait(0x4014, 0, 10);
    Actor_FaceDirection(MEARI, 0xa000, 60);
    Actor_FaceDirection(MEARI, 0xe000, 20);
    Actor_FaceDirection(MEARI, 0xa000, 20);
    Actor_FaceDirection(MEARI, 0xc000, 40);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Event_ShowMessageAndWait(0x2003, 0, 10);
    Engine_ActorSetAnimation(19, 3);
    Engine_ActorSetAnimationAndWait(20, 3);
    Engine_EventWait(40);
    Actor_FaceDirection(MEARI, 0x2000, 20);
    Event_ShowMessageAndWait(0x4003, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Actor_WalkToAndWait(MEARI, 0x2b0, 200);
    Actor_SetPosition(MEARI, 0, 0);
    Object_GetById(20)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(20, 0x284, 166);
    Engine_EventWait(1);
    *(u8 *)(((s32)Object_GetById(20)) + ACTOR_FLAGS_OFFSET_020019e8) |= 1;
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    GameFlag_Set(0x82e);
    GameFlag_Clear(0x82d);
    Engine_EventEnd();
}

void SceneActor_TurnTowardTableAngle(s32 z)
{
    T *o;
    s32 t;
    s32 d;
    u16 prev;
    s32 n;

    o = (T *)z;
    n = o->unk64;
    z = 0;
    t = ((s16 *)&o->unk64)[z];
    if (t != 0) {
        o->unk64 = n - 1;
        return;
    }
    o->unk5A = t;
    z = 1;
    d = ImiruMura_KeyHeadings[(gKeysHeld >> 4) & 0xF];
    z = -z;
    if (d == z) {
        Object_SetMode(o, 9);
        return;
    }
    prev = o->unk6;
    d = (s16)(d - prev);
    if (d > 0x1000)
        d = 0x1000;
    if (d < -0x1000)
        d = -0x1000;
    o->unk6 = prev + d;
    Object_SetMode(o, 2);
    ObjectDispatch_ApplyValueToChildren(o, 0x30);
}

void StagedActor_RunHeadingProbeStep(void)
{

    struct Subject_02001fa4 *subject;
    struct FieldPosition probe;
    s32 heading;
    s32 goal;
    s32 marker;
    s32 z;
    s32 x;
    u8 *p;

    subject = Engine_ActorLookup(((struct SharedData_02000240 *)&gGameState)->selected_subject);

    for (;;) {
        heading = ImiruMura_SwayHeadings[(gKeysHeld >> 4) & 15];
        if ((heading << 16) == -0x10000) {
            return;
        }

        Engine_EventBegin();

        probe.x = (subject->x & 0xfff00000) + 0x80000;
        probe.y = subject->y;
        probe.z = (subject->z & 0xfff00000) + 0x80000;
        z = probe.z;
        x = probe.x;
        p = (u8 *)subject;
        p += 34;
        goal = GetMapCellCollision((s32)*p, x, z);
        Vector_AddPolarOffset(0x100000, heading, &probe);

        marker = GetMapCellCollision((s32)*p, probe.x, probe.z);
        if (marker == 255
                || Map_GetTerrainHeight((s32)*p, probe.x, probe.z)
                    - subject->y > 0x80000) {
            subject->heading = (u16)heading;
            goto tail;
        }

        probe.x = x;
        probe.z = z;
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        subject->state_100 = 0;
        Engine_ObjectSetPosition(subject, x, subject->y, z);
        Object_SetMode(subject, 2);
        ObjectDispatch_ApplyValueToChildren(subject, 48);
        Engine_ObjectCommitPosition(subject);
        subject->callback = (void *)SceneActor_TurnTowardTableAngle;

        goto advanceProbe;
continueProbe:
        if (Map_GetTerrainHeight((s32)*p, probe.x, probe.z)
                - subject->y > 0x80000) {
            goto finishProbe;
        }
        x = probe.x;
        z = probe.z;
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        Engine_ObjectSetPosition(subject, probe.x, probe.y, probe.z);
        Engine_ObjectCommitPosition(subject);
        if (marker != goal) {
            goto blocked;
        }

advanceProbe:
        FieldVector_AddPolarOffset(0x100000, heading, &probe);
        marker = GetMapCellCollision((s32)*p, probe.x, probe.z);
        if (marker != 255) {
            goto continueProbe;
        }

finishProbe:
        subject->state_048 = 0x20000;
        subject->state_052 = 0x10000;
        Engine_ObjectSetPosition(subject, x, subject->y, z);
        Engine_ObjectCommitPosition(subject);
        Engine_TaskWait(2);
    }

blocked:
    subject->callback = NULL;
    subject->flags_090 |= 1;

    subject->state_052 = 0x4000;

tail:
    Engine_TaskWait(10);
    Engine_EventEnd();
}
