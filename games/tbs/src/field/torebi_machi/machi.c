#include "machi.h"
#include "iwram_call.h"
#include "call.h"
#include "field_event.h"
#include "field_service.h"
#include "text/msg_ids.h"







void SceneState_SetValues31_2_4(void)
{
    BattleFx_RunPageEffectForSlot(0x1F, 2, 4);
}

s32 SceneActor_GetPositionDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dxsq = dx *dx;
    s32 dysq = dy *dy;
    s32 dzsq = dz *dz;

    return Iwram_Sqrt(dxsq + dysq + dzsq);
}

s32 SceneActor_UpdatePlayerProximity(struct SceneActor *actor,
                                    struct SceneActor *target,
                                    s32 range, s32 force)
{
    s32 result = 0;
    s32 *targetPos = &target->x;
    s32 *actorPos = &actor->x;

    if (SceneActor_GetPositionDistance(targetPos, actorPos) < range || force != 0) {
        u32 angle = (u16)ArcTan2(target->z - actor->z,
                                            *targetPos - *actorPos);
        u32 farLeft = (angle - 0x2000) & 0xf000;
        u32 farRight = (angle + 0x2000) & 0xf000;
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 forward = angle & 0xf000;
        u32 facing = actor->facing & 0xf000;

        if (forward == facing || right == facing || left == facing || force != 0) {
            actor->active = 1;
            Object_SetMode(actor, 1);
            result = 1;
        }
        if ((u8 *)target == Object_GetById(0) && (farRight == facing || farLeft == facing)) {
            actor->active = 1;
            Object_SetMode(actor, 1);
            result = 1;
        }
    } else {
        actor->active = 0;
        Object_SetMode(actor, 2);
    }
    return result;
}

s32 SceneActor_UpdatePartnerProximity(u8 *self)
{
    u8 **globals = (u8 **)gWindowWork;
    struct TorebiSceneWork *scene = (struct TorebiSceneWork *)globals[0];
    u8 *work = globals[12];
    u16 *flags = (u16 *)(self + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *partner;
    u8 *player;

    if ((*flags & 1) != 0) {
        partner = Object_GetById(17);
    } else {
        partner = Object_GetById(16);
    }
    if (SceneActor_UpdatePlayerProximity(self, partner, 32, 0) != 0) {
        return 0;
    }

    player = Object_GetById(0);

    if (*(s16 *)(work + 376) != 0 || scene->widen_proximity != 0) {
        range = 26;
        if ((*flags & 2) != 0) {
            force = 1;
        }
    }

    SceneActor_UpdatePlayerProximity(self, player, range, force);
    return 0;
}

u8 *TorebiMachi_GetEntrances(void)
{
    return gTorebiMachiEntrances;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *TorebiMachi_GetExits(void)
{
    return gTorebiMachiExits;
}

u8 *TorebiMachi_GetPlacements(void)
{
    return gTorebiMachiPlacements;
}

void TorebiMachi_OpenActorEightPassage(void)
{

    u32 i;
    struct FieldActor *record;

    record = Object_GetById(8);
    if ((s32)record != 0) {
        record->collision_flags = 0;
    }
    record = Object_GetById(8);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    Field_SetMapCellCollision(0, 0x2200000, 0x1200000, 253);
    Engine_GameFlagSet(0x200);
}

void ConfigureAndPlaceActorOneHundredTwo(void)
{
    s32 a = 3, b = 26;
    Engine_MapCopyCellAttributes(3, 32, 1, 1, a, b);
    MapObject_SetPosition(102, 0x00380000, 0x01a80000);
}

void HideActorOneHundredTwo(void)
{
    s32 a = 3, b = 26;
    Engine_MapCopyCellAttributes(2, 25, 1, 1, a, b);
    MapObject_SetPosition(102, -1, -1);
}

void SceneDialogue_RunMessage0e36(void)
{
    Engine_EventSetMessage((s32)MsgTorebiFaceAwayTolbi);
    Engine_EventShowMessage(-1, 0);
}

void SceneDialogue_RunMessage0e37(void)
{
    Engine_EventSetMessage((s32)MsgTorebiTossLuckyMedal);
    Engine_EventShowMessage(-1, 0);
}

void FieldScene_RunSupplementalSequenceTwo(void)
{

    struct EventWork *work;
    struct SceneActor *actor;
    s16 facing;
    s32 msg;

    /* FAKEMATCH: the event work is read into a local before the actor lookup, where the reference loads it. */
    work = gEventWork;
    actor = Object_GetById(16);
    facing = actor->facing;
    Engine_EventBegin();
    actor->proximity_flags |= 2;
    if (work->psynergy_request == 0) {
        if (Engine_GameFlagIsSet(0x950) != 0) {
            msg = (s32)MsgTorebiYayEasyRun;
        } else if (Engine_GameFlagIsSet(0x962) != 0) {
            msg = (s32)MsgTorebiHoorayFinalsSeven;
        } else {
            msg = (s32)MsgTorebiWheeFestivalColosso;
        }
    } else {
        if (Engine_GameFlagIsSet(0x950) != 0) {
            msg = (s32)MsgTorebiMainStreetTolbi;
        } else if (Engine_GameFlagIsSet(0x962) != 0) {
            msg = (s32)MsgTorebiWinBigUsed;
        } else {
            msg = (s32)MsgTorebiOldestShouldntShare;
        }
    }
    Engine_EventSetMessage(msg);
    Engine_ActorSetAnimation(16, 0);
    Engine_ActorFaceEachOther(16, 0, 2);
    Engine_EventShowMessageAndWait(16, 0, 10);
    actor->facing = facing;
    Engine_TaskWait(1);
    actor->proximity_flags &= 1;
    Engine_EventEnd();
}

void FieldScene_RunSiblingsTalk(void)
{

    struct EventWork *work;
    struct SceneActor *actor;
    s16 facing;
    s32 msg;

    /* FAKEMATCH: the event work is read into a local before the actor lookup, where the reference loads it. */
    work = gEventWork;
    actor = Object_GetById(17);
    facing = actor->facing;
    Engine_EventBegin();
    actor->proximity_flags |= 2;
    if (work->psynergy_request == 0) {
        if (Engine_GameFlagIsSet(0x950) != 0) {
            msg = (s32)MsgTorebiHaHaHa;
        } else if (Engine_GameFlagIsSet(0x962) != 0) {
            msg = (s32)MsgTorebiWaahBigBrother;
        } else {
            msg = (s32)MsgTorebiWaahSaidMoney;
        }
    } else {
        if (Engine_GameFlagIsSet(0x950) != 0) {
            msg = (s32)MsgTorebiBrotherMeanDuring;
        } else if (Engine_GameFlagIsSet(0x962) != 0) {
            msg = (s32)MsgTorebiMamaWhyListen;
        } else {
            msg = (s32)MsgTorebiMamaToldShare;
        }
    }
    Engine_EventSetMessage(msg);
    Engine_ActorSetAnimation(17, 0);
    Engine_ActorFaceEachOther(17, 0, 2);
    Engine_EventShowMessageAndWait(17, 0, 10);
    actor->facing = facing;
    Engine_TaskWait(1);
    actor->proximity_flags &= 1;
    Engine_EventEnd();
}

u8 *TorebiMachi_SelectEvents(void)
{
    if (Engine_GameFlagIsSet(0x950) != 0) {
        return gTorebiMachiEvents3;
    }
    if (Engine_GameFlagIsSet(0x962) != 0) {
        return gTorebiMachiEvents2;
    }
    return gTorebiMachiEvents;
}

void SceneDialogue_RunActor15Message1f92(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgTorebiSomeoneLiveMore);
    Engine_EventAskYesNo(15, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor24Message1f9d(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgTorebiChildrenLoveSouvenirs);
    Engine_EventAskYesNo(24, 0);
    Engine_EventEnd();
}

void FieldScene_RunPatientTalk(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgTorebiPatientLittleGuy);
    Actor_FaceDirection(25, 0xc000, 0);
    Engine_EventShowMessage(25, 0);
    Actor_FaceDirection(25, 0x8000, 0);
    Engine_EventShowMessage(25, 0);
    Engine_EventEnd();
}

void TorebiMachi_AskForSweets(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_FaceDirection(26, 0x4000, 0);
    Engine_ActorStartRepeatedMotion(26, 2);
    Engine_EventSetMessage((s32)MsgTorebiWaahBuySweets);
    Engine_EventShowMessage(26, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor27Message1fa3(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgTorebiInnsFullStaying);
    Engine_EventShowMessage(0x1B, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor24Message235f(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgTorebiFinalsWerent);
    Engine_EventAskYesNo(24, 0);
    Engine_EventEnd();
}

void TorebiMachi_ReceiveLeftovers(void)
{

    u32 i;
    s32 record;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x8bf) == 0) {
        Engine_GameFlagSet(0x8bf);
        Engine_EventSetMessage((s32)MsgTorebiLeftovers);
        Engine_EventShowMessage(19, 0);
        Engine_ItemShowFound(233, 3);
        Engine_EventShowMessage(19, 0);
        Engine_ActorSetAnimation(0, 1);
        Engine_PartyGiveItem(233, 0);
    } else {
        Engine_EventSetMessage((s32)MsgTorebiFestivalLongerUsual);
        Engine_EventShowMessage(19, 0);
    }
    Engine_EventEnd();
}

void SceneScript_SetupActors(void)
{

    struct EventWork *work = gEventWork;
    u32 no;
    s32 index;

    Engine_EventBegin();
    for (no = 8; no <= 65; no++) {
        u8 *actor = Object_GetById(no);
        if (actor != NULL) {
            actor[85] = 0;
        }
    }
    index = work->touched_trigger - 1;
    Engine_AudioPlayCue(158);
    Map_AnimateCells((s32)gTorebiMachiCellSteps[index].commands, gTorebiMachiCellSteps[index].first, gTorebiMachiCellSteps[index].second);
    Actor_SetSpeed(0, 0x8000, 0x4000);
    Object_GetById(0)->motion_flags = 0;
    Engine_ActorSetAnimation(0, 2);
    if (index != 6) {
        Actor_CenterAndWalk(0, 2, -8);
        Engine_EventWait(10);
    }
    Engine_EventRequestExit(work->touched_trigger);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}

void SceneActor_CopyPlayerModeToActor(union FieldObject *object)
{
    s32 priority;

    if (object != NULL) {
        priority = Object_GetById(0)->sprite->priority;
        object->actor.priority_flags = 0;
        object->actor.sprite->priority = priority;
        ((struct SceneSprite *)object->actor.sprite)->priority_15 = priority;
    }
}

s32 TorebiMachi_ApplyEntryState(s32 a0)
{
    u32 i;
    s32 record;
    s32 handler;
    s32 hidden;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Actor_SetPosition(16, 0x1600000, 0x1600000);
    Engine_ActorEnableActionCallback(16, gTorebiMachiActor16Action);
    record = Object_GetById(16);
    handler = (s32)SceneActor_UpdatePartnerProximity;
    ((struct SceneActor *)record)->proximity_flags = 1;
    *(s32 *)(record + 108) = handler;
    hidden = 0;
    Engine_ActorSetPosition(17, 0x1700000, 0x1400000);
    Engine_ActorEnableActionCallback(17, gTorebiMachiActor17Action);
    record = Object_GetById(17);
    ((struct SceneActor *)record)->proximity_flags = hidden;
    *(s32 *)(record + 108) = handler;
    record = Object_GetById(14);
    *(s32 *)(record + 108) = (s32)SceneActor_CopyPlayerModeToActor;
    if (Engine_GameFlagIsSet(0x8c1) != 0) {
        Actor_SetPosition(28, 0x13c0000, 0x1480000);
    }
    if (Engine_GameFlagIsSet(0x201) != 0) {
        FieldScene_ResetActor9AndDrawTiles();
    }
    if (Engine_GameFlagIsSet(0x200) != 0) {
        TorebiMachi_OpenActorEightPassage();
        Engine_ActorSetAnimation(8, 4);
    }
    if (Engine_GameFlagIsSet(0x950) != 0) {
        Actor_SetPosition(20, 0x2080000, 0x2300000);
        Actor_SetPosition(21, 0x2080000, 0x2300000);
        Actor_SetPosition(22, 0x2080000, 0x2300000);
        Actor_SetPosition(24, 0x2080000, 0x2300000);
        Actor_SetPosition(25, 0x2080000, 0x2300000);
        Actor_SetPosition(26, 0x2080000, 0x2300000);
        Actor_SetPosition(27, 0x2080000, 0x2300000);
    } else {
        if (Engine_GameFlagIsSet(0x962) != 0) {
            Actor_SetPosition(27, 0x1180000, 0x500000);
            Actor_FaceDirection(27, 0x2000, 0);
            Engine_ActorSetAnimation(27, 1);
        }
    }
    return 0;
}

void FieldScene_RunScene3b5SequenceA(void)
{

    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_WalkToAndWait(0, 0x130, 0x138);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Engine_ActorFaceDirection(28, 0x4000, 0);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgTorebiWantTestLuck);
    Event_OpenMessage(28, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        gEventWork->message++;
        Engine_EventShowMessage(28, 0);
        Actor_SetSpeed(28, 0x10000, 0x8000);
        Actor_WalkToAndWait(28, 0x140, 0x130);
        Actor_WalkToAndWait(28, 0x13c, 0x148);
        Engine_ActorFaceDirection(28, 0xa000, 0);
        Engine_GameFlagSet(0x8c1);
    } else {
        Engine_EventShowMessage(28, 0);
    }
    Engine_EventEnd();
}

void SceneState_SetValue30ThenCall(void)
{
    Engine_EventRequestExit(30);
    Engine_EventEnd();
}

void TorebiMachi_RequestTouchedExit(void)
{

    s16 *cnt = &gEventWork->touched_trigger;

    Engine_EventRequestExit(*cnt);
}

void FieldScene_RunPrimarySequence(void)
{
    s32 base;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Actor_SetSpeed(29, 0x10000, 0x8000);
    Engine_ActorSetSpeed(30, 0x10000, 0x8000);
    base = (s32)MsgTorebiSeenAnyoneWho;
    Engine_EventSetMessage(base);
    Actor_SetPosition(29, 0x480000, 0xd00000);
    Actor_SetPosition(30, 0x380000, 0xd00000);
    Engine_ActorSetChildValue(32, 15);
    Engine_ActorSetSpriteFlags(Object_GetById(32), 0);
    Actor_SetPosition(32, 0x5f0000, 0x280000);
    Engine_ActorWalkTo(29, 72, 248);
    Engine_ActorWalkTo(30, 56, 248);
    Actor_WalkToAndWait(0, 64, 0x108);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Engine_ActorWaitForMove(29);
    Engine_ActorSetAnimation(29, 1);
    Engine_ActorSetAnimation(30, 1);
    Engine_ActorSetAnimation(0, 1);
    Engine_ActorFaceActor(29, 0, 0);
    Engine_ActorFaceActor(30, 0, 0);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(29, 0x102);
    Actor_SetAttachedEffect(30, 0x102);
    Engine_ActorStartRepeatedMotion(29, 2);
    Engine_ActorRunRepeatedMotion(30, 2);
    Engine_EventWait(20);
    Event_OpenMessage(29, 0);
    Engine_EventWait(25);
    UiWindow_CreateWithSideObject(52, 0, 12, 7);
    UiText_OpenMessageWindow((base + 3), 11, 12, 2);
    SCENE_OBJECT_ID = 32;
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(30, 2);
        Engine_EventWait(30);
        Engine_ActorFaceDirection(30, 0, 0);
        Engine_EventWait(30);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(29, 3);
        Engine_EventWait(20);
        Engine_ActorFaceDirection(29, 0, 0);
        Engine_EventWait(30);
        Engine_EventShowMessage(29, 0);
        Engine_EventWait(20);
        Actor_FaceDirection(29, 0x4000, 0);
        Engine_ActorFaceDirection(30, 0x4000, 0);
        Engine_EventWait(30);
        Engine_ActorSetAnimation(29, 3);
        Engine_ActorSetAnimationAndWait(30, 3);
        Engine_EventWait(20);
        Actor_SetSpeed(29, 0x1cccc, 0xe666);
        Actor_SetSpeed(30, 0x1cccc, 0xe666);
        Actor_WalkTo(29, 232, 248);
        Engine_EventWait(2);
        Actor_WalkTo(30, 232, 248);
        Engine_ActorWaitForMove(29);
        Engine_ActorWalkTo(29, 248, 248);
        Engine_ActorWalkToAndWait(30, 248, 248);
    } else {
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(30, 2);
        Engine_EventWait(30);
        Engine_ActorFaceDirection(30, 0, 0);
        Engine_EventWait(30);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(29, 4);
        Engine_EventWait(20);
        Engine_ActorFaceDirection(29, 0, 0);
        Engine_EventWait(30);
        gEventWork->message += 1;
        Engine_EventShowMessage(29, 0);
        Engine_EventWait(20);
        Actor_FaceDirection(29, 0x4000, 0);
        Actor_FaceDirection(30, 0x4000, 0);
        Engine_EventWait(30);
        Engine_ActorSetAnimation(29, 3);
        Engine_ActorSetAnimationAndWait(30, 3);
        Engine_EventWait(20);
        Actor_SetSpeed(29, 0x19999, 0xcccc);
        Actor_SetSpeed(30, 0x19999, 0xcccc);
        Engine_ActorWalkTo(29, 72, 184);
        Engine_ActorWalkToAndWait(30, 56, 184);
    }
    Engine_ActorSetPosition(29, 0, 0);
    Engine_ActorSetPosition(30, 0, 0);
    Engine_ActorSetPosition(32, 0, 0);
    Engine_GameFlagSet(0x8c0);
    Engine_EventEnd();
}

void FieldScene_ResetActor9AndDrawTiles(void)
{
    struct Actor *actor = Object_GetById(9);
    if (actor != 0) {
        Engine_ActorSetSpriteFlags(actor, 0);
        actor->field23 = 2;
        actor->field55 = 0;
    }
    Engine_ActorSetAnimation(9, 5);
    {
        s32 v5 = 34;
        s32 v6 = 16;
        Engine_MapCopyCellAttributes(36, 16, 1, 1, v5, v6);
    }
    Engine_GameFlagSet(0x201);
}
