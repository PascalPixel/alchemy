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
#include "field_service.h"
#include "field_effect.h"
#include "item_ids.h"
#include "text/msg_ids.h"

void BattleFx_RunPageEffectForSlot(s32, s32, s32);

extern u8 Placement_Scripts[];
extern u8 Placement_Messages[];
extern u8 Placement_Actors[];



#define NULL ((void *)0)

#define REC_S16(rec, off) (*(s16 *)((rec) + (off)))


extern u8 Placement_Effects[];
void Motion_LaunchFromFocusedObject(u32, s32, s32, s32);

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    void Engine_ActorSetSpeed(s32, s32, s32);

    Engine_ActorSetSpeed(actor, x, y);
}

static __inline__ void SetOffset(s32 actor, s32 offset, s32 zero)
{
    void Engine_ActorFaceDirection(s32, s32, s32);

    Engine_ActorWalkByAndWait(actor, offset, zero);
}

enum {
    ENTRANCE_PRIMARY_SEQUENCE = 20,
    ENTRANCE_PRIMARY_SEQUENCE_ALTERNATE = 21,
    ENTRANCE_FROM_AERIE = 90,
    ENTRANCE_FROM_AERIE_ALTERNATE = 91
};

enum {
    FLAG_AERIE_EVENTS_DONE = 0x9a7,
    FLAG_PRIMARY_SEQUENCE_SEEN = 0x9b8,
    FLAG_ACTOR_18_MOVED = 0x9bb,
    FLAG_ARRIVED_FROM_AERIE = 0x9bf
};

enum {
    ACTOR_BLACK_ORB = 25
};

enum {
    HEAP_ITEM_ICON = 17,
    ITEM_ICON_BUFFER_SIZE = 0x608,
    ITEM_ICON_TILES = 0x400,
    ITEM_ICON_TILE_BYTES = 128
};

void SceneActor_SetActor23Params2And6(void)
{
    BattleFx_RunPageEffectForSlot(0x17, 2, 6);
}

u8 *SceneData_GetScriptTable(void)
{
    return Placement_Scripts;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}

s32 SceneData_SelectTableByFlag9a7(void)
{
    if (Engine_GameFlagIsSet(0x9A7) != 0) {
        return (s32)Placement_Actors9a7;
    }
    return (s32)Placement_Actors;
}

void SceneActor_StartLament(s32 actor)
{
    void Actor_FaceDirection();

    struct FieldActor *object;

    object = (struct FieldActor *)Actor_Get(actor);
    object->scale_x = 0x10000;
    object = (struct FieldActor *)Actor_Get(actor);
    object->scale_y = 0x10000;
    Engine_EventSetMessage((s32)MsgRariberoOhhTheyTookShebaHeaded);
    Event_ShowMessage(actor, 0);
    Actor_FaceDirection(actor, 0xc000, 0);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(actor, RariberoMachi_LamentActions);
}

void RariberoMachi_AskLeaving(s32 obj)
{
    s32 msg = (s32)MsgRariberoLeavingLalivero;
    Engine_EventSetMessage(msg);
    Engine_EventOpenMessage(obj, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(msg + 1);
    } else {
        Engine_EventSetMessage(msg + 2);
    }
    Engine_EventShowMessage(obj, 0);
}

void SceneActor_SetupActor18Event(void)
{
    void Engine_ActorFaceDirection(s32, s32, s32);

    Engine_GameFlagSet(2491);
    Engine_EventSetMessage((s32)MsgRariberoWasToldLetInIf);
    Engine_EventShowMessage(18, 0);
    PlaceActor(18, 65536, 32768);
    SetOffset(18, -16, 0);
    Engine_ActorFaceDirection(18, 0, 0);
    Engine_EventWait(10);
}

void Scene_RunTableTransition(void)
{
    s32 no = gEventWork->touched_trigger;
    s32 x = RariberoMachi_DoorCells[no][0];
    s32 y = RariberoMachi_DoorCells[no][1];

    Object_GetById(0)->motion_flags = 2;
    Engine_AudioPlayCue(158);
    if (no == 6) {
        Map_AnimateCells((s32)RariberoMachi_GateOpenSteps, (u16)x, (u16)y);
        Actor_WalkBy(0, 0, -16);
    } else {
        Map_AnimateCells((s32)RariberoMachi_DoorOpenSteps, (u16)x, (u16)y);
        Actor_CenterAndWalk(0, 2, -16);
    }
    Engine_EventWait(10);
    gEventWork->transition_frames = 16;
    Engine_EventRequestExit(no);
}

void RariberoMachi_RequestTouchedExit(void)
{
    struct EventWork *work = gEventWork;
    s16 *p = (s16 *)&work->touched_trigger;
    s32 n = *p;

    work->transition_frames = 16;
    Engine_EventRequestExit(n);
}

void Scene_RunPrimarySequence(void)
{
    u32 i;
    struct FieldActor *record;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgRariberoMoreTolbisSoldiersLayDefeated);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xf80000, 0x1a80000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Engine_ActorSetAnimation(8, 0);
    Engine_ActorSetAnimation(9, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    FieldMotion_LaunchFromFocusedObject(22, 8, -16, 0xc000);
    Engine_ActorWaitForMove(22);
    Engine_EventWait(20);
    Actor_ShowEmote(22, 0x102, 60);
    Actor_SetSpeed(22, 0x10000, 0x8000);
    Actor_WalkByAndWait(22, 0, -16);
    Engine_EventWait(10);
    Actor_FaceActor(22, 8, 40);
    Actor_FaceActor(22, 9, 40);
    Actor_FaceActor(22, 8, 40);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(22, 0x4000, 0);
    Engine_EventWait(30);
    Event_OpenMessage(22, 0);
    Engine_EventChooseYesNo(0, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Event_ShowMessage(8, 0);
    Engine_EventWait(10);
    Actor_FaceActor(22, 8, 30);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_ActorSetAnimation(8, 1);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(30);
    Event_ShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(20);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Actor_FaceActor(22, 9, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 30);
    Engine_ActorSetAnimation(9, 1);
    Engine_EventWait(10);
    Actor_ShowEmote(22, 0x108, 40);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Actor_FaceDirection(22, 0x4000, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(30);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(22, 0x102, 50);
    Actor_FaceActor(22, 9, 20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Event_ShowMessage(8, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(22, 0x4000, 0);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Engine_EventWait(20);
    Actor_ShowEmote(22, 0x102, 50);
    Actor_SetSpeed(22, 0x1cccc, 0xe666);
    Actor_WalkToAndWait(22, 0x100, 0x168);
    Engine_EventWait(10);
    Actor_FaceDirection(22, 0, 0);
    Engine_EventWait(30);
    Actor_FaceDirection(22, 0x8000, 0);
    Engine_EventWait(30);
    Actor_FaceDirection(22, 0xc000, 0);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(20);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(22, 0x100, 40);
    Actor_FaceDirection(22, 0x2000, 0);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(9, 3);
    Engine_EventWait(30);
    Event_ShowMessage(9, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Actor_SetSpeed(22, 0x19999, 0xcccc);
    Actor_WalkToAndWait(22, 0x100, 0x180);
    Actor_FaceDirection(22, 0, 0);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Event_ShowMessage(8, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(22, 0x8000, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_SetSpeed(22, 0x13333, 0x9999);
    Actor_WalkByAndWait(22, 0, 16);
    Engine_EventWait(10);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 80);
    Actor_ShowEmote(22, 0x101, 80);
    Actor_FaceActor(22, 8, 40);
    Actor_FaceActor(22, 9, 40);
    Actor_FaceDirection(22, 0x4000, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(20);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(22, 0, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Event_ShowMessage(8, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(22, 0x8000, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 70);
    Actor_FaceDirection(22, 0x4000, 0);
    Engine_EventWait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Actor_SetSpeed(22, 0x13333, 0x9999);
    Engine_ActorSetAnimation(22, 2);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(22, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(22);
    Actor_SetPosition(22, 0, 0);
    Engine_EventWait(10);
    Engine_EventEnd();
}

void FieldScene_RunSecondarySequence(void)
{
    u32 i;
    u8 *record;

    GameFlag_Set(0x9ba);
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgRariberoWhereGoingRobinIodemAsked);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 104, 0x178);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    FieldMotion_LaunchFromFocusedObject(1, -32, 0, 0);
    FieldMotion_LaunchFromFocusedObject(3, -16, 16, 0xe000);
    Motion_LaunchFromFocusedObject(2, 0, 16, 0xc000);
    Engine_ActorWaitForMove(GERALD);
    Engine_EventWait(30);
    Event_OpenMessage(GERALD, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Engine_EventWait(10);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(MEARI, 4);
        Engine_EventWait(20);
        Event_ShowMessage(MEARI, 0);
        gEventWork->message += 1;
    } else {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(MEARI, 4);
        Engine_EventWait(20);
        gEventWork->message += 1;
        Event_ShowMessage(MEARI, 0);
    }
    Engine_EventWait(10);
    Actor_ShowEmote(IWAN, 0x108, 40);
    Event_ShowMessage(IWAN, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(30);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -16, 0);
    Actor_WalkBy(IWAN, 0, -16);
    Actor_WalkBy(MEARI, 0, -8);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
    Engine_ActorSetAnimation(MEARI, 1);
    Engine_ActorSetAnimation(IWAN, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(GERALD, 0xc000, 0);
    Actor_FaceDirection(MEARI, 0xc000, 0);
    Actor_FaceDirection(IWAN, 0xc000, 0);
    Actor_FaceDirection(22, 0x4000, 0);
    Engine_EventWait(80);
    Actor_SetSpeed(22, 0xcccc, 0x6666);
    Object_GetById(22)->motion_flags = 2;
    Engine_ActorSetSpritePriority(22, 2);
    Map_CopyCellsTo(34, 0, 1, 2, 4, 18);
    Event_PlayCue(158);
    Engine_EventWait(20);
    Actor_SetPosition(22, 0x480000, 0x1380000);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    Object_GetById(22)->target_z = ACTOR_NO_TARGET;
    Actor_SetPosition(22, 0x480000, 0x1380000);
    Engine_EventWait(20);
    Actor_WalkToAndWait(22, 72, 328);
#else
    Engine_EventWait(20);
    Actor_WalkByAndWait(22, 0, 16);
#endif
    Engine_MapCopyCellsTo(32, 0, 1, 2, 4, 18);
    Event_PlayCue(159);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(40);
    Actor_WalkByAndWait(22, 16, 0);
    Actor_FaceDirection(22, 0x4000, 0);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(30);
    Event_ShowMessage(MEARI, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Engine_EventWait(20);
    Event_ShowMessage(GERALD, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(22, 0x102, 40);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(IWAN, 0x100, 40);
    Event_ShowMessage(IWAN, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceEachOther(MEARI, IWAN, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(GERALD, 0xc000, 0);
    Actor_FaceDirection(IWAN, 0xc000, 0);
    Actor_FaceDirection(MEARI, 0xc000, 0);
    Engine_EventWait(40);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(MEARI, 0x102, 40);
    Event_ShowMessage(MEARI, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(GERALD, 0xe000, 0);
    Engine_EventWait(20);
    Event_OpenMessage(GERALD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(22, 4);
        Engine_EventWait(20);
        Event_ShowMessage(22, 0);
        gEventWork->message += 1;
    } else {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(22, 4);
        Engine_EventWait(20);
        gEventWork->message += 1;
        Event_ShowMessage(22, 0);
    }
    Engine_EventWait(10);
    Actor_ShowEmote(IWAN, 0x102, 40);
    Event_ShowMessage(IWAN, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(GERALD, 0xc000, 0);
    Actor_ShowEmote(22, 0x106, 50);
    Event_ShowMessage(22, 0);
    Engine_EventWait(20);
    Actor_SetPosition(25, 0x580000, 0x14c0000);
    Event_ShowMessage(-1, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(MEARI, 0x100, 40);
    Engine_ActorJump(MEARI, 4, 13);
    Engine_ActorJump(MEARI, 4, 30);
    Event_ShowMessage(MEARI, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    if (GameFlag_IsSet(0x9bf) != 0) {
        FieldScene_RunScene3c6SequenceA();
    }
    Engine_EventSetMessage((s32)MsgRariberoBabiToldMeShipAncients);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(GERALD, 0x100, 40);
    Event_ShowMessage(GERALD, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(30);
    Event_ShowMessage(IWAN, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Actor_SetSpeed(25, 0xcccc, 0x6666);
    Actor_SetDestinationOffset(25, 0, 16);
    Actor_WalkByAndWait(22, 0, 16);
    Engine_EventWait(30);
    Actor_SetPosition(25, 0, 0);
    gEventWork->message += 1;
    Engine_PartyGiveItem(242, 0);
    Engine_EventWait(10);
    ((struct FieldActor *)Actor_Get(22))->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_WalkByAndWait(22, 0, -16);
    ((struct FieldActor *)Actor_Get(22))->steering_flags |= ACTOR_FACES_MOTION;
    Actor_FaceDirection(22, 0x4100, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Actor_ShowEmote(22, 0x100, 40);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(50);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Actor_WalkByAndWait(22, -16, 0);
    Actor_FaceDirection(22, 0xc000, 0);
    Engine_EventWait(20);
    Map_CopyCellsTo(34, 0, 1, 2, 4, 18);
    Event_PlayCue(158);
    Engine_EventWait(10);
    Actor_WalkByAndWait(22, 0, -16);
    Actor_SetPosition(22, 0, 0);
    Engine_EventWait(10);
    Engine_MapCopyCellsTo(32, 0, 1, 2, 4, 18);
    Event_PlayCue(159);
    Engine_EventWait(50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceActor(GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(IWAN, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Event_ShowMessage(GERALD, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(MEARI, 2);
    Engine_EventWait(20);
    Event_ShowMessage(MEARI, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(30);
    Event_ShowMessage(IWAN, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(30);
    Actor_SetSpeed(GERALD, 0x13333, 0x9999);
    Actor_SetSpeed(MEARI, 0x13333, 0x9999);
    Actor_SetSpeed(IWAN, 0x13333, 0x9999);
    Engine_ActorSetAnimation(GERALD, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(GERALD, REC_S16(record, 10), REC_S16(record, 18));
    }
    Engine_ActorWaitForMove(GERALD);
    Actor_SetPosition(GERALD, 0, 0);
    Engine_ActorSetAnimation(MEARI, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(MEARI, REC_S16(record, 10), REC_S16(record, 18));
    }
    Engine_ActorWaitForMove(MEARI);
    Actor_SetPosition(MEARI, 0, 0);
    Engine_ActorSetAnimation(IWAN, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(IWAN, REC_S16(record, 10), REC_S16(record, 18));
    }
    Engine_ActorWaitForMove(IWAN);
    Actor_SetPosition(IWAN, 0, 0);
    Engine_EventWait(10);
    Engine_EventEnd();
}

void FieldScene_RunScene3c6SequenceA(void)
{
    u32 i;
    u8 *record;

    Engine_EventSetMessage((s32)MsgRariberoThoughtShipWeSawAt);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 4);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(GERALD, 0x100, 0);
    Actor_ShowEmote(MEARI, 0x100, 0);
    Actor_ShowEmote(IWAN, 0x100, 55);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Engine_EventWait(20);
    Event_ShowMessage(GERALD, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 4);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(MEARI, 0x102, 40);
    Event_ShowMessage(MEARI, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(30);
    Event_ShowMessage(GERALD, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(IWAN, 0x8000, 0);
    Engine_EventWait(65);
    Actor_FaceDirection(IWAN, 0xc000, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(30);
    Event_ShowMessage(IWAN, 0);
}

void FieldScene_RunSequenceB(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgRariberoWhereGoingArentWeTaking);
    Event_ShowMessage(GERALD, 0);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
    Engine_EventEnd();
}

s32 SceneData_SelectSecondaryTableByFlag9a7(void)
{
    if (Engine_GameFlagIsSet(0x9A7) != 0) {
        return (s32)Placement_Effects9a7;
    }
    return (s32)Placement_Effects;
}

s32 Scene_Initialize(void)
{
    union FieldObject *object;
    struct FieldSprite *sprite;
    u8 *icon;

    if (gGameState.entrance == ENTRANCE_FROM_AERIE) {
        GameFlag_Set(FLAG_AERIE_EVENTS_DONE);
        GameFlag_Set(FLAG_ARRIVED_FROM_AERIE);
    }
    if (gGameState.entrance == ENTRANCE_FROM_AERIE_ALTERNATE) {
        GameFlag_Set(FLAG_AERIE_EVENTS_DONE);
    }
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 24;

    Engine_ActorSetAnimation(19, 3);
    Actor_Get(19)->collision_flags = 0;
    Engine_ActorSetSpriteFlags(Actor_Get(19), 0);
    Engine_ActorSetAnimation(20, 3);
    Actor_Get(20)->collision_flags = 0;
    Engine_ActorSetSpriteFlags(Actor_Get(20), 0);
    Engine_ActorSetAnimation(21, 3);
    Actor_Get(21)->collision_flags = 0;
    Engine_ActorSetSpriteFlags(Actor_Get(21), 0);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_BLACK_ORB), 0);

    object = (union FieldObject *)Actor_Get(ACTOR_BLACK_ORB);
    object->actor.unknown_5c = 1;
    object->actor.motion_flags = 0;
    sprite = object->actor.sprite;
    object->actor.y.fixed = PIXELS(10);
    sprite->part_count = 0;
    sprite->full_color = 0;
    sprite->palette = 0;
    icon = Engine_HeapAllocate(HEAP_ITEM_ICON, ITEM_ICON_BUFFER_SIZE);
    Engine_ItemLoadIcon(ITEM_BLACK_ORB);
    Engine_VramLoad(sprite->vram_block, ITEM_ICON_TILE_BYTES, &icon[ITEM_ICON_TILES]);
    Engine_HeapRelease(HEAP_ITEM_ICON);

    if (GameFlag_IsSet(FLAG_AERIE_EVENTS_DONE)) {
        Engine_ActorSetSpriteFlags(Actor_Get(24), 0);
        Actor_Get(24)->collision_flags = 0;
        Map_CopyCellAttributes(20, 23, 1, 1, 14, 4);
        Map_CopyCellAttributes(20, 23, 1, 1, 15, 4);
        Map_CopyCellAttributes(20, 23, 1, 1, 16, 4);
        if (GameFlag_IsSet(FLAG_ACTOR_18_MOVED)) {
            Actor_SetPosition(18, PIXELS(56), PIXELS(184));
        }
    } else {
        object = (union FieldObject *)Actor_Get(8);
        object->actor.collision_flags = 0;
        object->actor.priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
        object->actor.sprite->flags = 0;
        object->actor.sprite->rotation = FACING_NORTH;
        object = (union FieldObject *)Actor_Get(9);
        object->actor.collision_flags = 0;
        object->actor.priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
        object->actor.sprite->flags = 0;
        object->actor.sprite->rotation = FACING_SOUTH;
        Map_CopyCellAttributes(20, 23, 1, 1, 13, 23);
        Map_CopyCellAttributes(20, 23, 1, 1, 14, 23);
        Map_CopyCellAttributes(20, 23, 1, 1, 78, 23);
        Map_CopyCellAttributes(20, 23, 1, 1, 17, 23);
        Map_CopyCellAttributes(20, 23, 1, 1, 18, 23);
        if ((gGameState.entrance == ENTRANCE_PRIMARY_SEQUENCE
                || gGameState.entrance == ENTRANCE_PRIMARY_SEQUENCE_ALTERNATE)
            && !GameFlag_IsSet(FLAG_PRIMARY_SEQUENCE_SEEN)) {
            GameFlag_Set(FLAG_PRIMARY_SEQUENCE_SEEN);
            object = (union FieldObject *)Actor_Get(11);
            object->actor.paused = 1;
            object = (union FieldObject *)Actor_Get(17);
            object->actor.paused = 1;
            Scene_RunPrimarySequence();
            object = (union FieldObject *)Actor_Get(11);
            object->actor.paused = 0;
            object = (union FieldObject *)Actor_Get(17);
            object->actor.paused = 0;
        }
    }
    return 0;
}
