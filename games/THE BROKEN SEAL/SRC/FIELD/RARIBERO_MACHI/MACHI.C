#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FIELD_EFFECT.H"
#include "ITEM_IDS.H"
#include "CALL.H"

extern u8 MsgRariberoOhhTheyTookShebaHeaded[];
void BattleFx_RunPageEffectForSlot(s32, s32, s32);

/* The scene's tables, laid out after the code; the actor table has an
 * alternate once flag 0x9a7 is set. */
extern u8 Placement_Scripts[];
extern u8 Placement_Messages[];
extern u8 Placement_Actors[];
extern u8 Placement_Actors9a7[];

/* The lamenting villager's action table. */
extern const u8 RariberoMachi_LamentActions[];

extern u8 MsgRariberoLeavingLalivero[];

extern u8 MsgRariberoBabiToldMeShipAncients[];
extern u8 MsgRariberoMoreTolbisSoldiersLayDefeated[];
extern u8 MsgRariberoThoughtShipWeSawAt[];
extern u8 MsgRariberoWasToldLetInIf[];
extern u8 MsgRariberoWhereGoingArentWeTaking[];
extern u8 MsgRariberoWhereGoingRobinIodemAsked[];
#define NULL ((void *)0)

/* A signed 16-bit field of an actor record returned by one of the record
 * lookups. */
#define REC_S16(rec, off) (*(s16 *)((rec) + (off)))

/* Each door's cell, by the trigger that opens it, and the cell animations
 * that open the doors. */
extern s16 RariberoMachi_DoorCells[][2];
extern u8 RariberoMachi_GateOpenSteps[];
extern u8 RariberoMachi_DoorOpenSteps[];

/* The effect tables, laid out after the code, with an alternate once flag
 * 0x9a7 is set. */
extern u8 Placement_Effects[];
extern u8 Placement_Effects9a7[];
void Motion_LaunchFromFocusedObject(u32, s32, s32, s32);
void FieldScene_RunScene3c6SequenceA(void);

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    void Actor_SetSpeed(s32, s32, s32);

    Actor_SetSpeed(actor, x, y);
}

static __inline__ void SetOffset(s32 actor, s32 offset, s32 zero)
{
    void Actor_FaceDirection(s32, s32, s32);

    Actor_WalkByAndWait(actor, offset, zero);
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

/* Lalivero's scene tables and the first actor setups that precede
   FLAGGED_CUE.C in the overlay. */
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
    if (GameFlag_IsSet(0x9A7) != 0) {
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

/* Asks whether the party is leaving Lalivero, with a line for each answer. */
void RariberoMachi_AskLeaving(s32 obj)
{
    s32 msg = (s32)MsgRariberoLeavingLalivero;
    Engine_EventSetMessage(msg);
    Event_OpenMessage(obj, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(msg + 1);
    } else {
        Engine_EventSetMessage(msg + 2);
    }
    Event_ShowMessage(obj, 0);
}

void SceneActor_SetupActor18Event(void)
{
    void Actor_FaceDirection(s32, s32, s32);

    GameFlag_Set(2491);
    Engine_EventSetMessage((s32)MsgRariberoWasToldLetInIf);
    Event_ShowMessage(18, 0);
    PlaceActor(18, 65536, 32768);
    SetOffset(18, -16, 0);
    Actor_FaceDirection(18, 0, 0);
    Engine_EventWait(10);
}

void Scene_RunTableTransition(void)
{
    s32 no = gEventWork->touched_trigger;
    s32 x = RariberoMachi_DoorCells[no][0];
    s32 y = RariberoMachi_DoorCells[no][1];

    ((u8 *)Object_GetById(0))[85] = 2;
    Audio_PlayCue(158);
    if (no == 6) {
        Call3(Engine_MapAnimateCells, (s32)RariberoMachi_GateOpenSteps, (u16)x, (u16)y);
        Call3(Engine_ActorWalkBy, 0, 0, -16);
    } else {
        Call3(Engine_MapAnimateCells, (s32)RariberoMachi_DoorOpenSteps, (u16)x, (u16)y);
        Call3(Engine_ActorCenterAndWalk, 0, 2, -16);
    }
    Engine_EventWait(10);
    gEventWork->transition_frames = 16;
    Engine_EventRequestExit(no);
}

void SceneState_SetWord1c8To16AndForward16c(void)
{
    u8 *work = (u8 *)gEventWork;
    s16 *p = (s16 *)(work + 0x16C);
    s32 n = *p;

    *(s32 *)(work + 0x1C8) = 16;
    Engine_EventRequestExit(n);
}

/*
 * Runs the fixed call sequence for this scene: a paced series of setup and
 * per-entity calls -- position, pose and property triples keyed by entity id,
 * interleaved with timed single-argument steps -- ending with a record lookup
 * whose s16 fields at +10 and +18 feed the last positioning call.
 */
void Scene_RunPrimarySequence(void)
{
    u32 i;
    u8 *record;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgRariberoMoreTolbisSoldiersLayDefeated);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xf80000, 0x1a80000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Engine_ActorSetAnimation(8, 0);
    Engine_ActorSetAnimation(9, 0);
    Engine_EventOpenScreen(); /* main:0808a360 */
    Engine_EventWaitForScreen(); /* main:0808a370 */
    Engine_EventWait(20);
    Call4(Motion_LaunchFromFocusedObject, 22, 8, -16, 0xc000);
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
    Event_OpenMessage(22, 0); /* main:0808a178 */
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
        /* Pass the record's s16 fields at +10 and +18 through to entity 22. */
        Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(22);
    Actor_SetPosition(22, 0, 0);
    Engine_EventWait(10);
    Engine_EventEnd();
}

/*
 * Drives actors 0 to 3, 22 and 25 through a long timed sequence of pose, move
 * and sprite-flag calls, gated by two condition checks that each pick one of
 * two call sequences and both bump the shared scene step counter.
 */
void FieldScene_RunSecondarySequence(void)
{
    u32 i;
    u8 *record;

    GameFlag_Set(0x9ba);
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgRariberoWhereGoingRobinIodemAsked);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 104, 0x178);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Call4(Motion_LaunchFromFocusedObject, 1, -32, 0, 0);
    Call4(Motion_LaunchFromFocusedObject, 3, -16, 16, 0xe000);
    Motion_LaunchFromFocusedObject(2, 0, 16, 0xc000);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Engine_EventWait(30);
    Event_OpenMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Engine_EventWait(10);
    /* Either path bumps the step counter once, at a different point in its
     * four calls. */
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(ACTOR_MIA, 4);
        Engine_EventWait(20);
        Event_ShowMessage(ACTOR_MIA, 0);
        gEventWork->message += 1;
    } else {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(ACTOR_MIA, 4);
        Engine_EventWait(20);
        gEventWork->message += 1;
        Event_ShowMessage(ACTOR_MIA, 0);
    }
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_IVAN, 0x108, 40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(30);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -16, 0);
    Actor_WalkBy(ACTOR_IVAN, 0, -16);
    Actor_WalkBy(ACTOR_MIA, 0, -8);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
    Engine_ActorSetAnimation(ACTOR_MIA, 1);
    Engine_ActorSetAnimation(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(22, 0x4000, 0);
    Engine_EventWait(80);
    Actor_SetSpeed(22, 0xcccc, 0x6666);
    /* Set the byte at offset 85 of actor 22's record to 2. */
    *(u8 *)((s32)Object_GetById(22) + 85) = 2;
    Engine_ActorSetSpritePriority(22, 2);
    Call6(Engine_MapCopyCellsTo, 34, 0, 1, 2, 4, 18);
    Audio_PlayCue(158);
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
    Audio_PlayCue(159);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(40);
    Actor_WalkByAndWait(22, 16, 0);
    Actor_FaceDirection(22, 0x4000, 0);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(22, 0x102, 40);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Engine_EventWait(40);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Engine_EventWait(20);
    Event_OpenMessage(ACTOR_GERALD, 0);
    /* The same branch-and-bump shape as above. */
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
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_ShowEmote(22, 0x106, 50);
    Event_ShowMessage(22, 0);
    Engine_EventWait(20);
    Actor_SetPosition(25, 0x580000, 0x14c0000);
    Event_ShowMessage(-1, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 40);
    Engine_ActorJump(ACTOR_MIA, 4, 13);
    Engine_ActorJump(ACTOR_MIA, 4, 30);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    /* One extra call, run only when GameFlag_IsSet(0x9bf) is non-zero. */
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
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
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
    /* Clear bit 0 of the actor flag byte, then set it back through a second
     * record accessor. */
    ((struct FieldActor *)Actor_Get(22))->unknown_5a &= ~1;
    Actor_WalkByAndWait(22, 0, -16);
    ((struct FieldActor *)Actor_Get(22))->unknown_5a |= 1;
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
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(50);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Actor_WalkByAndWait(22, -16, 0);
    Actor_FaceDirection(22, 0xc000, 0);
    Engine_EventWait(20);
    Call6(Engine_MapCopyCellsTo, 34, 0, 1, 2, 4, 18);
    Audio_PlayCue(158);
    Engine_EventWait(10);
    Actor_WalkByAndWait(22, 0, -16);
    Actor_SetPosition(22, 0, 0);
    Engine_EventWait(10);
    Engine_MapCopyCellsTo(32, 0, 1, 2, 4, 18);
    Audio_PlayCue(159);
    Engine_EventWait(50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(30);
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_IVAN, 0x13333, 0x9999);
    Engine_ActorSetAnimation(ACTOR_GERALD, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, REC_S16(record, 10), REC_S16(record, 18));
    }
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Engine_ActorSetAnimation(ACTOR_MIA, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_MIA, REC_S16(record, 10), REC_S16(record, 18));
    }
    Engine_ActorWaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Engine_ActorSetAnimation(ACTOR_IVAN, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_IVAN, REC_S16(record, 10), REC_S16(record, 18));
    }
    Engine_ActorWaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Engine_EventWait(10);
    Engine_EventEnd();
}

/* A fixed, unbranching sequence of overlay calls with constant arguments: no
 * loop, no stored result, no use of the scene work record. */
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
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 55);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(22, 4);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    Engine_EventWait(65);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
}

void FieldScene_RunSequenceB(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgRariberoWhereGoingArentWeTaking);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
    Engine_EventEnd();
}

s32 SceneData_SelectSecondaryTableByFlag9a7(void)
{
    if (GameFlag_IsSet(0x9A7) != 0) {
        return (s32)Placement_Effects9a7;
    }
    return (s32)Placement_Effects;
}

/*
 * Lalivero's scene setup. Arriving from the aerie by entrance 90 or 91 sets
 * flag 0x9a7, and entrance 90 also sets flag 0x9bf. Actors 19 to 21 take
 * animation 3. Actor 25 is placed 10 pixels up and shows the Black Orb's icon.
 * While flag 0x9a7 is set, actor 24's sprite flags and collision are cleared,
 * three map cells are copied and flag 0x9bb moves actor 18. Otherwise actors 8
 * and 9 face north and south, five cells are copied, and arriving by entrance
 * 20 or 21 plays the primary sequence once, recorded by flag 0x9b8.
 */
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
            object->actor.unknown_5b = 1;
            object = (union FieldObject *)Actor_Get(17);
            object->actor.unknown_5b = 1;
            Scene_RunPrimarySequence();
            object = (union FieldObject *)Actor_Get(11);
            object->actor.unknown_5b = 0;
            object = (union FieldObject *)Actor_Get(17);
            object->actor.unknown_5b = 0;
        }
    }
    return 0;
}
