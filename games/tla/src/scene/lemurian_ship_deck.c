/// @file
/// The Lemurian ship's deck, in two scenes: the deck at the tiller, where the
/// crew takes its places as the leader sails (Room1), and the deck where the
/// party sights a ship, reaches Piers' ship, sets the Black Crystal back in it
/// and sets out for Madra (Room2).

#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "field/event_inline.h"
#include "system/io_reg.h"
#include "map.h"
#include "system/frame.h"
#include "event_work.h"
#include "module/scene/scene.h"
#include "text/msg_ids.h"

extern s32 gLemurianShipDeck_WaveScroll;
extern s32 gLemurianShipDeck_BobPhase;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gLemurianShipDeck_Placements[];
extern struct ActorPlacement gLemurianShipDeck_Room1Placements[];

const u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov657Room1)
    return (const u8 *)gLemurianShipDeck_Room1Placements;
  return (const u8 *)gLemurianShipDeck_Placements;
}

/// Once flag 0x985 is set, the leader goes through the cabin door, the door
/// opening first unless it already has (flag 0x201).
void LemurianShipDeck_EnterCabin(void) {
  if (Event_GetFlag(0x985)) {
    u8 *work = Ram_HeapSlots->eventWork;
    u32 i;

    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    // The loop is written with goto so that it carries no loop notes: the bytes
    // rebuild the zero and the height on every pass, and a for loop hoists both
    // (differ 39).
    i = 8;
  loop:
    {
      struct Actor *object = Actor_Get(i);

      if (object != 0) {
        object->motionFlags = 0;
        object->y = PIXELS(32);
      }
    }
    i++;
    if (i <= 63)
      goto loop;
    if (!Event_GetFlag(0x201)) {
      Event_PlaySound(158);
      MapGrid_CopyCellsTo(51, 38, 70, 22, 1, 2);
    }
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
    Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -4);
    FieldEvent_Wait(8);
    Event_PlaySound(123);
    FieldEvent_SetExitRequest(*(s16 *)(work + 0x154));
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Event_End();
  }
}

extern struct Trigger gLemurianShipDeck_Triggers[];
extern struct Trigger gLemurianShipDeck_Room2Triggers[];

const u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov657Room2)
    return (const u8 *)gLemurianShipDeck_Room2Triggers;
  return (const u8 *)gLemurianShipDeck_Triggers;
}

/// ★ Named from the game: "The door is held closed by a powerful force."
/// (MsgTheDoorIsHeld)
void LemurianShipDeck_TryHeldDoor(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered(MsgTheDoorIsHeld, 1);
  Event_End();
}

#define OBJ_PALETTE ((volatile u16 *)&BG_PLTT_COLOR(11, 0))

#define DEFINE_ROTATE_PALETTE_COLORS(name, n1, n2)                             \
  void name(void) {                                                            \
    if ((gFrameCount & 7) == 0) {                                              \
      s16 last = OBJ_PALETTE[n1];                                              \
      s32 i;                                                                   \
                                                                               \
      /* The loop is written with goto so that it carries no loop notes: the   \
       * bytes */                                                              \
      /* rebuild the palette base on every pass, and a for or do loop hoists   \
       * it */                                                                 \
      /* and steps two pointers (differ 30). */                                \
      i = n1;                                                                  \
    loop:                                                                      \
      OBJ_PALETTE[i] = OBJ_PALETTE[i - 1];                                     \
      i--;                                                                     \
      if (i != n2)                                                             \
        goto loop;                                                             \
      OBJ_PALETTE[n2] = last;                                                  \
    }                                                                          \
  }

DEFINE_ROTATE_PALETTE_COLORS(LemurianShipDeck_RotateColors12To15, 15, 12)

DEFINE_ROTATE_PALETTE_COLORS(LemurianShipDeck_RotateColors4To9, 9, 4)

void Map_ClearLayerEntryFlag(s32 a);

/// Arriving by entrance 99: the screen opens and two of the map's layer entries
/// are cleared in turn.
void LemurianShipDeck_ShowArrival(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  FieldTask_AddFrame(LemurianShipDeck_RotateColors4To9, 0x480);
  Event_WaitTransitionFrames();
  FieldEvent_Wait(60);
  Map_ClearLayerEntryFlag(13);
  FieldEvent_Wait(60);
  FieldEvent_Wait(60);
  Map_ClearLayerEntryFlag(14);
  Event_End();
}

#define LAYER_WORD(layer, offset) (*(s32 *)((u8 *)(layer) + (offset)))

/// A task: scrolls the waves' layer half a pixel a frame.
void LemurianShipDeck_ScrollWaves(void) {
  s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x108);
  s32 phase = gLemurianShipDeck_WaveScroll;

  layer[2] = phase;
  phase += 0x8000;
  gLemurianShipDeck_WaveScroll = phase;
  if (phase == 0x200000)
    gLemurianShipDeck_WaveScroll = 0;
}

/// A task: bobs the camera's actor and the first map layer together on a sine
/// wave, its phase advancing 0x200 a frame.
void LemurianShipDeck_BobDeck(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[0];
  struct Actor *object = EventCamera_GetActor();
  s32 offset = Math_Sin(gLemurianShipDeck_BobPhase) >> 2;

  object->y += offset;
  LAYER_WORD(layer, 12) += offset;
  gLemurianShipDeck_BobPhase += 0x200;
}

#include "shared/effects.h"
#include "shared/tables.h"
#include "text/msg_ids.h"
#include "version.h"
#include "macros.h"
#include "field_sprite.h"
#include "system/task.h"
#include "call.h"

/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;

struct MapMode {
  u8 unk00[23];
  struct MapModeBits {
    u8 low : 2;
    u8 mode : 2;
  } __attribute__((packed)) bits;
};

extern u32 gLemurianShipDeck_FirstCrewLooksAround[];

extern u32 gLemurianShipDeck_SecondCrewLooksAround[];

extern u32 gLemurianShipDeck_ThirdCrewLooksAround[];

void LemurianShipDeck_RotateColors12To15(void);

void LemurianShipDeck_ScrollWaves(void);

void LemurianShipDeck_BobDeck(void);

void LemurianShipDeck_TakeTheTiller(void);

void FieldCamera_WaitForArrival(void);

extern s16 gLemurianShipDeck_Puddles[];

void FlagBlock_PlaceAll(const void *data);

void LemurianShipDeck_ShowArrival(void);

void FlagBlock_ApplyFlags(const void *data);

extern u32 gLemurianShipDeck_SparkleFades[];
extern u32 gLemurianShipDeck_ReturnToShipScript[];

void Actor_Release(struct Actor *object);

extern u32 gLemurianShipDeck_ReachPiersShipScript[];
extern u32 gLemurianShipDeck_SmokePuff[];

void LemurianShipDeck_EnterCabin(void);

void LemurianShipDeck_TryHeldDoor(void);

void LemurianShipDeck_RotateColors4To9(void);
void LemurianShipDeck_HoldOnToTiller(void);

/// Places an actor facing an angle, the angle taken as the u16 an actor's
/// facing is; the helper, our name, Madra's and Loho's placements use too.
static inline void LemurianShipDeck_PlaceFacing(s32 id, s32 x, s32 z,
                                                u16 facing) {
  Actor_SetPositionAndFacing(id, x, z, facing);
}

/// The scene run from entrance 10: the crew takes its places on deck and the
/// leader is asked to take the tiller; it ends by setting flag 0x8de.
void LemurianShipDeck_TakeTheTiller(void) {
  s32 turn;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(342), PIXELS(48), PIXELS(408), 0);
  System_Wait(1);
  MapGrid_CopyCellsTo(60, 53, 67, 16, 7, 3);
  LemurianShipDeck_PlaceFacing(gMainRoutineWork.currentOwner, PIXELS(552),
                               PIXELS(376), FACING_EAST);
  LemurianShipDeck_PlaceFacing(9, PIXELS(558), PIXELS(345), 0xd000);
  LemurianShipDeck_PlaceFacing(7, PIXELS(534), PIXELS(404), FACING_EAST);
  turn = 0x2000;
  LemurianShipDeck_PlaceFacing(6, PIXELS(568), PIXELS(408), turn);
  LemurianShipDeck_PlaceFacing(5, PIXELS(546), PIXELS(440), 0x4000);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  Actor_SetScript(5, gLemurianShipDeck_FirstCrewLooksAround);
  Actor_SetScript(6, gLemurianShipDeck_SecondCrewLooksAround);
  Actor_SetScript(9, gLemurianShipDeck_ThirdCrewLooksAround);
  Event_StartScreenTransition();
  gLemurianShipDeck_WaveScroll = 0;
  Task_AddFrame(LemurianShipDeck_ScrollWaves, 0x480);
  gLemurianShipDeck_BobPhase = 0;
  Task_AddFrame(LemurianShipDeck_BobDeck, 0x480);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(542), PIXELS(44), PIXELS(396), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(80);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  FieldEvent_SetMessageId((s32)MsgTakeTheTiller);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHWEST, 20);
  Event_ShowMessage(7, 0);
  FieldActor_Stop(5);
  FieldActor_Stop(9);
  FieldActor_Stop(6);
  System_Wait(1);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_ShowEmote(6, 0x101, 20);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(9, turn);
  Event_ShowMessage(9, 0);
  Actor_ShowEmote(5, 0x108, 20);
  Event_ShowMessage(5, 0);
  Event_OpenMessage(7, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
    Actor_ShowEmote(9, 0x109, 20);
    Event_ShowMessage(9, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
    Actor_ShowEmote(9, 0x109, 20);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Event_ShowMessage(9, 0);
  }
  Actor_ShowEmote(7, 0x100, 0);
  Event_ShowMessage(7, 0);
  Event_ShowMessage(9, 0);
  Actor_ShowEmote(6, 0x101, 20);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(9, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessage(9, 0);
  Actor_ShowEmote(5, 0x100, 20);
  Event_ShowMessage(5, 0);
  Actor_FaceDirectionAndWait(9, FACING_SOUTH + FACING_STEP);
  Event_ShowMessage(9, 0);
  Event_ShowMessage(6, 0);
  Actor_ShowEmote(5, 0x108, 0);
  Actor_FaceDirectionAndWait(5, FACING_NORTHEAST);
  Event_ShowMessage(5, 0);
  Actor_SetAttachedEffect(6, 0x102);
  Actor_FaceDirectionAndWait(6, FACING_SOUTHWEST);
  Event_ShowMessage(0x4006, 0);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_ShowEmote(7, 0x101, 20);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(9, 0x108, 20);
  Event_ShowMessage(9, 0);
  FieldActor_Jump(6, 4, 10);
  Event_ShowMessage(6, 0);
  FieldActor_Jump(5, 2, 10);
  FieldActor_Jump(5, 4, 10);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(7, 0);
  Actor_SetAnimation(9, 3);
  Event_ShowMessage(9, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  gMainRoutineWork.renderMode = 7;
  gMainRoutineWork.ownerOverride = 1;
  Event_SetFlag(0x8de);
  FieldEvent_SetExitRequest(10);
}

/// Sends actor id to actor 4's spot at a run.
static inline void RunToActor4(s32 id) {
  struct Actor *target;

  Actor_SetSpeed(id, 0x13333, 0x9999);
  Actor_SetAnimation(id, 2);
  target = Actor_GetById(4);
  if (target != NULL)
    Actor_SetDestination(id, ((s16 *)&target->x)[1], ((s16 *)&target->z)[1]);
  Actor_WaitForMoveById(id);
  Actor_SetPosition(id, 0, 0);
}

/// The scene run from entrance 20: the camera frames actors 4 to 9 around the
/// spot, two callbacks start the bobbing, and after the exchange of looks and
/// emotes the four actors run to actor 4.
void LemurianShipDeck_HoldOnToTiller(void) {
#if !TLA_PATCH_2
  /* FAKEMATCH(dead): a call that never runs, kept for its routine address,
   * the first constant the function puts in its pool: the message id's pool
   * label is then numbered one later, and reload's hard-register CSE, whose
   * lookup of the second -16 finds r1's only through that label's hash in
   * this object, builds -16 twice, as Japanese and English do; from patch 2
   * the ROM copies it from r1, as the plain form does here. As nothing, 15 of
   * 459 instructions differ in tla-en. */
  if (0)
    Task_AddFrame(LemurianShipDeck_BobDeck, 0x480);
#endif
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(458), PIXELS(48), PIXELS(408), 0);
  FieldEvent_SetMessageId(MsgHoldOnTo);
  Actor_SetPositionAndFacing(4, PIXELS(470), PIXELS(392), FACING_EAST);
  FieldActor_LaunchFromLeader(6, 0, -24, FACING_SOUTH);
  FieldActor_LaunchFromLeader(5, -16, -16, FACING_SOUTHEAST);
  FieldActor_LaunchFromLeader(9, -24, 8, FACING_NORTHEAST + FACING_STEP);
  FieldActor_LaunchFromLeader(7, -4, 24, FACING_NORTHEAST);
  MapGrid_CopyCellsTo(60, 53, 67, 16, 7, 3);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  gLemurianShipDeck_WaveScroll = 0;
  Task_AddFrame(LemurianShipDeck_ScrollWaves, 0x480);
  gLemurianShipDeck_BobPhase = 0;
  Task_AddFrame(LemurianShipDeck_BobDeck, 0x480);
  Actor_ShowEmote(6, 0x102, 30);
  Actor_FaceDirectionAndWait(6, FACING_SOUTH);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  Actor_FaceDirectionAndWait(5, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_ShowEmote(4, 0x102, 30);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(5, FACING_SOUTHEAST);
  FieldEvent_Wait(30);
  Actor_FaceDirectionAndWait(5, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_FaceDirectionAndWait(6, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(6, 3);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(6, FACING_SOUTH);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  Actor_ShowEmote(7, 0x109, 30);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirectionAndWait(9, FACING_SOUTHEAST);
  Actor_ShowEmote(9, 0x101, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 5);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(7, 0x102, 30);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  Actor_FaceEachOther(6, 5, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(5, FACING_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(6, 0x101, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  FieldEvent_ShowMessageThenWait(9, 0, 5);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  Actor_ShowEmote(9, 0x102, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 5);
  Actor_FaceDirectionAndWait(9, FACING_NORTH);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 5);
  Actor_FaceDirection(4, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 5);
  Actor_FaceDirectionAndWait(9, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(9, 4);
  FieldEvent_ShowMessageThenWait(9, 0, 5);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(10);
  Actor_FaceEachOther(9, 7, 0);
  Actor_FaceEachOther(6, 5, 0);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(9, 3);
  Actor_SetAnimation(7, 3);
  FieldEvent_Wait(40);
  RunToActor4(7);
  RunToActor4(5);
  RunToActor4(6);
  RunToActor4(9);
  FieldEvent_SetExitRequest(1);
}

/// Room1, the deck at the tiller: the colour rotations, and the tiller scenes
/// of entrances 10 and 20.
void LemurianShipDeck_SetUpTillerDeck(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  FieldTask_AddFrame(LemurianShipDeck_RotateColors12To15, 0x480);
  FieldTask_AddFrame(LemurianShipDeck_RotateColors4To9, 0x480);
  if (gMainRoutineWork.entrance == 10)
    LemurianShipDeck_TakeTheTiller();
  if (gMainRoutineWork.entrance == 20)
    LemurianShipDeck_HoldOnToTiller();
}

s32 Scene_Initialize(void) {
  struct MainRoutineWork *party;
  struct MapMode *map = (struct MapMode *)Ram_HeapSlots->mapWork;
  struct MapModeBits bits = map->bits;

  /* FAKEMATCH(temporary): the mode bits changed in a one-byte copy, bits, and
   * stored back; as map->bits.mode = 1 with no empty block, 2 of 62
   * instructions differ in tla-en. */
  bits.mode = 1;
  /* FAKEMATCH(dead): an empty once-only block before the copy's store, so
   * gMainRoutineWork's address loads between the or and the store; as no block,
   * 12 of 62 instructions differ in tla-en. */
  do {
  } while (0);
  party = &gMainRoutineWork;
  map->bits = bits;
  if (party->scene == (s32)&SceneId_Ov657Room1) {
    LemurianShipDeck_SetUpTillerDeck();
  } else {
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
    /* FAKEMATCH(dead): an empty once-only block, which ends the scheduling
     * block after the store; as no block, 4 of 62 instructions differ in
     * tla-en. */
    do {
    } while (0);
    Actor_Get(party->currentOwner)->priorityFlags |= 0x20;
    Actor_SetAnimation(8, 1);
    FlagBlock_PlaceAll(gLemurianShipDeck_Puddles);
    FieldTask_AddFrame(LemurianShipDeck_RotateColors12To15, 0x480);
    if (party->entrance == 99)
      LemurianShipDeck_ShowArrival();
    else
      FieldTask_AddFrame(LemurianShipDeck_RotateColors4To9, 0x480);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov657Room1) {
    MapGrid_CopyCellsTo(2, 2, 10, 24, 18, 10);
    MapGrid_CopyCellsTo(0, 62, 26, 62, 3, 8);
    MapGrid_CopyCellsTo(50, 65, 25, 61, 6, 1);
  }
  return 0;
}

/// Unless flag 0x8ff is set: the crew sights the ship from the deck and the
/// leader is asked about it. Either way flag 0x980 is set.
void LemurianShipDeck_SightMenardisShip(void) {
  s32 answer;
  s32 owner;

  if (Event_GetFlag(0x8ff) != 0) {
    Event_SetFlag(0x980);
    return;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(4, 0xcccc, 0x6666);
  Actor_WalkToAndWait(4, 264, 136);
  FieldEvent_SetMessageId(MsgLookItS);
  FieldActor_LaunchFromLeader(5, 16, -16, FACING_SOUTH);
  FieldActor_LaunchFromLeader(9, 32, -16, FACING_SOUTH);
  FieldActor_LaunchFromLeader(6, -16, -16, FACING_SOUTH);
  FieldEvent_Wait(80);
  Actor_ShowEmote(9, 0x102, 20);
  Actor_SetSpeed(9, 0x19999, 0xcccc);
  Actor_WalkToAndWait(9, 300, 152);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Event_ShowMessage(9, 0);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(PIXELS(200), -1, PIXELS(344), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldCamera_SetSpeed(0x13333, 0x2666);
  FieldCamera_MoveTo(PIXELS(600), -1, PIXELS(344), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  FieldCamera_SetSpeed(0x66666, 0xcccc);
  FieldCamera_MoveTo(PIXELS(264), -1, PIXELS(128), 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimationAndWait(5, 4);
  Event_ShowMessage(0x2005, 0);
  Actor_FaceDirectionAndWait(6, FACING_EAST);
  Actor_SetAnimation(6, 4);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTHWEST + FACING_STEP);
  Actor_ShowEmote(9, 0x101, 0);
  Event_OpenMessage(9, 0);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  answer =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  owner = gMainRoutineWork.currentOwner;
  if (answer == 0) {
    Actor_WaitForAnimationChange(owner);
    Actor_SetAnimation(9, 3);
    Event_ShowMessage(9, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    Actor_WaitForAnimationChange(owner);
    FieldEvent_SetMessageId(MsgLookItS + 5); // the fifth message after it
    Event_ShowMessage(9, 0);
  }
  Actor_SetAnimation(5, 4);
  Event_ShowMessage(0x2005, 0);
  Actor_FaceDirectionAndWait(6, FACING_EAST);
  Actor_SetAnimation(6, 4);
  Event_ShowMessage(6, 0);
  Actor_SetAnimation(9, 4);
  Event_ShowMessage(9, 0);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_Get(9)->actionFlags &= ~1;
  Actor_WalkToAndWait(9, 288, 144);
  FieldEvent_Wait(1);
  Actor_Get(9)->actionFlags |= 1;
  Event_ShowMessage(0x2009, 0);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimationAndWait(9, 3);
  Actor_SetSpeed(9, 0x13333, 0x9999);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  {
    struct Actor *target;

    Actor_SetAnimation(9, 2);
    target = Actor_Get(4);
    if (target != NULL)
      Actor_SetDestination(9, ((s16 *)&target->x)[1], ((s16 *)&target->z)[1]);
    Actor_SetAnimation(5, 2);
    target = Actor_Get(4);
    if (target != NULL)
      Actor_SetDestination(5, ((s16 *)&target->x)[1], ((s16 *)&target->z)[1]);
    Actor_SetAnimation(6, 2);
    target = Actor_Get(4);
    if (target != NULL)
      Actor_SetDestination(6, ((s16 *)&target->x)[1], ((s16 *)&target->z)[1]);
  }
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetPosition(5, 0, 0);
  Actor_SetPosition(9, 0, 0);
  Event_End();
  Event_SetFlag(0x980);
}

/// Frost freezes the deck's puddle (flag 0x200).
void LemurianShipDeck_FreezePuddle(void) {
  FlagBlock_ApplyFlags(gLemurianShipDeck_Puddles);
}

void Actor_MoveToObject(s32 id, s32 target);

/// ★ Named from the game: "Well, there's no point in hanging around here all
/// day." (MsgWellThereS)
void LemurianShipDeck_LeaveToFindCrew(void) {
  struct Actor *object;

  if (Event_GetFlag(0x212))
    return;
  if (Event_GetFlag(0x988))
    return;
  Event_SetFlag(0x212);
  if (Event_GetFlag(0x211)) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
    System_Wait(1);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_WalkToAndWait(9, 416, 428);
    Actor_FaceDirectionAndWait(9, FACING_WEST);
    FieldEvent_SetMessageId(MsgWellThereS);
    Event_ShowMessage(9, 0);
    Actor_SetAnimation(9, 2);
    object = Actor_Get(gMainRoutineWork.currentOwner);
    if (object != 0)
      Actor_SetDestination(9, *(s16 *)((u8 *)object + 10),
                           *(s16 *)((u8 *)object + 18));
    FieldActor_WaitForMoveById(9);
    Actor_SetPosition(9, 0, 0);
    Event_SetFlag(0x988);
    Event_End();
  } else if (Event_GetFlag(0x210)) {
    Event_SetFlag(0x211);
  } else {
    Event_SetFlag(0x210);
  }
}

/// Stepping back off the gangway clears flag 0x212, so the crew is sought
/// again.
void LemurianShipDeck_ResetCrewSearch(void) { GameFlag_Clear(0x212); }

void Actor_SelectAnimation(struct Actor *object, s32 value);
void Actor_StartScript(struct Actor *object, const void *data);

/// A task: a sparkle rises from around the Black Crystal toward the ship's
/// core.
void LemurianShipDeck_EmitSparkle(void) {
  s32 position[3];
  struct Actor *object;

  SET_VECTOR(position, 0x1680000, 0x400000, 0x1a80000);
  FieldVector_AddPolarOffset(0x200000, Random16(), position);
  object = FieldActor_Create(0x2a2, position[0], position[1], position[2]);
  if (object != NULL) {
    object->motionFlags = 0;
    *(s32 *)((u8 *)object + 0x30) = 0x20000;
    *(s32 *)((u8 *)object + 0x34) = 0x8000;
    Actor_SetSpriteFlags(object, 0);
    Actor_SelectAnimation(object, 0);
    FieldActor_SetMoveTarget(object, PIXELS(360), PIXELS(64), PIXELS(424));
    Actor_StartScript(object, gLemurianShipDeck_SparkleFades);
  }
}

/// The Black Crystal (item 242's icon) as an actor on the deck.
/// ★ Named from the game: "Black Crystal" (item 242's name)
struct Actor *LemurianShipDeck_CreateBlackCrystal(void) {
  struct Actor *object =
      (struct Actor *)Import_ActorCreate(0x1e9, 0x1680000, 0x400000, 0x1a80000);
  u8 *buffer = 0;
  struct FieldSprite *sprite;

  if (object == 0)
    return 0;
  sprite = object->sprite;
  sprite->fullColor = 0;
  sprite->palette = 0;
  sprite->priority = 1;
  object->motionFlags = 0;
  object->unk5c[0] = 1;
  sprite->nextFrame = 0;
  sprite->unk1b[0] = 0;
  buffer = HeapSlot_Alloc(68, 0x608);
  Icon_DrawItem(242);
  VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
  HeapSlot_Release(68);
  return object;
}

/// Once flag 7 is set, and only until flag 0x985 is: a scene of actor 7 and the
/// leader, which ends by setting flags 0x201 and 0x985.
void LemurianShipDeck_ReturnToShip(void) {
  struct Actor *handle;
  void (*func)(void);
  struct Actor *object;

  if (Event_GetFlag(0x985) != 0)
    return;
  if (Event_GetFlag(7) == 0)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Call2((void (*)())Actor_MoveToObject, 7, gMainRoutineWork.currentOwner);
  System_Wait(1);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_SetSpeed(7, 0x19999, 0xcccc);
  Actor_WalkToAndWait(7, 412, 424);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  FieldEvent_SetMessageId((s32)MsgAtLastI);
  FieldActor_Jump(7, 4, 10);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Event_ShowMessage(7, 0);
  Actor_WalkToAndWait(7, 386, 424);
  Actor_FaceActor(7, gMainRoutineWork.currentOwner, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Event_ShowMessage(7, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetScript(gMainRoutineWork.currentOwner,
                  gLemurianShipDeck_ReturnToShipScript);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_WalkToAndWait(7, 360, 424);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  Event_PlaySound(182);
  handle = LemurianShipDeck_CreateBlackCrystal();
  FieldEvent_Wait(20);
  Event_PlaySound(220);
  func = LemurianShipDeck_EmitSparkle;
  Task_Add(func, TASK_PASS_VBLANK);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_Interpolate(20);
  FieldEvent_Wait(20);
  PaletteFade_ApplyTarget(0x10006, 1);
  PaletteFade_Interpolate(20);
  FieldEvent_Wait(20);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(20);
  FieldEvent_Wait(20);
  MapGrid_CopyCellsTo(51, 38, 70, 22, 1, 2);
  Event_SetFlag(0x201);
  FieldTask_Remove(func);
  if (handle != 0)
    Call1((void (*)())Actor_Release, handle);
  FieldEvent_Wait(20);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(20);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(7, FACING_SOUTH);
  Event_ShowMessage(7, 0);
  Actor_SetAnimation(7, 3);
  Event_ShowMessage(7, 0);
  Actor_SetAnimation(7, 2);
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object != 0)
    Actor_SetDestination(7, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
  Event_SetFlag(0x985);
  Event_End();
}

/// Once flag 0x8ff is set, and only until flag 0x989 is: the crew talks
/// over the ship, the leader answers two prompts, and everyone leaves by
/// script gLemurianShipDeck_ReachPiersShipScript; it sets flag 0x981.
void LemurianShipDeck_ReachPiersShip(void) {
  if (Event_GetFlag(0x8ff) == 0)
    return;
  if (Event_GetFlag(0x989) != 0)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgMyShipAt);
  Actor_WalkTo(4, 264, 176);
  FieldActor_WaitForMoveById(4);
  FieldActor_LaunchFromLeader(7, 16, 8, FACING_SOUTH);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(288), -1, PIXELS(208), 1);
  FieldActor_LaunchFromLeader(9, 32, 0, FACING_SOUTH);
  FieldActor_LaunchFromLeader(6, 48, -8, FACING_SOUTH);
  FieldActor_LaunchFromLeader(5, -16, -8, FACING_SOUTH);
  FieldEvent_Wait(40);
  Actor_SetSpeed(7, 0x26666, 0x13333);
  Actor_WalkToAndWait(7, 280, 224);
  FieldEvent_Wait(20);
  Actor_ShowEmote(7, 264, 0);
  FieldActor_Jump(7, 4, 10);
  FieldActor_Jump(7, 4, 20);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(9, 265, 20);
  Event_ShowMessage(9, 0);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(288), -1, PIXELS(184), 1);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_WalkByAndWait(7, 0, -16);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(7, 3);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(9, 258, 20);
  Actor_FaceDirectionAndWait(6, FACING_SOUTHWEST);
  Actor_SetAnimation(6, 4);
  Event_ShowMessage(6, 0);
  Actor_ShowEmote(7, 257, 20);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Actor_ShowEmote(5, 261, 20);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  Actor_ShowEmote(7, 261, 20);
  Actor_SetAnimation(6, 4);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_SetAnimationAndWait(5, 3);
  Event_ShowMessage(5, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  Actor_SetAttachedEffect(7, 258);
  FieldEvent_Wait(20);
  Event_ShowMessage(0x2007, 0);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(6, 3);
  Actor_SetAnimation(9, 4);
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Event_ShowMessage(9, 0);
  Actor_FaceDirectionAndWait(9, FACING_WEST);
  Event_OpenMessage(9, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    Actor_ShowEmote(5, 263, 20);
    Event_ShowMessage(5, 0);
    Event_ShowMessage(6, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
    Actor_FaceDirection(7, FACING_NORTHWEST, 0);
    Actor_SetAnimationAndWait(5, 3);
    Event_ShowMessage(5, 0);
    Actor_FaceDirection(7, FACING_NORTHEAST, 0);
    Actor_SetAnimation(6, 3);
    Event_ShowMessage(6, 0);
  }
  Actor_ShowEmote(9, 265, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTHEAST);
  Event_ShowMessage(0x9009, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Actor_SetAnimation(7, 4);
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Actor_ShowEmote(5, 257, 0);
  Actor_ShowEmote(6, 257, 40);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(9, 265, 20);
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirection(6, FACING_WEST, 40);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(6, FACING_SOUTHWEST);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Actor_ShowEmote(7, 257, 20);
  Actor_SetAttachedEffect(9, 258);
  FieldEvent_Wait(20);
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_ShowEmote(7, 264, 20);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(5, 264, 0);
  Actor_ShowEmote(6, 264, 40);
  Actor_ShowEmote(9, 261, 20);
  Event_ShowMessage(9, 0);
  Actor_ShowEmote(5, 0x100, 0);
  Actor_ShowEmote(6, 0x100, 0);
  Actor_FaceDirection(5, FACING_EAST, 20);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_EAST);
  Actor_FaceDirection(9, FACING_WEST, 40);
  Actor_FaceDirectionAndWait(9, FACING_SOUTH);
  Event_ShowMessage(9, 0);
  Actor_SetAnimationAndWait(7, 3);
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_ShowEmote(9, 0x100, 20);
  Actor_RunRepeatedMotion(9, 3);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_SetAttachedEffect(5, 258);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  Actor_FaceDirectionAndWait(9, FACING_WEST);
  FieldActor_StartRepeatedMotion(9, 2);
  Actor_SetAnimationAndWait(9, 3);
  Actor_ShowEmote(7, 265, 0);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(9, FACING_SOUTH + FACING_STEP);
  FieldActor_StartRepeatedMotion(9, 2);
  Actor_ShowEmote(9, 261, 40);
  Actor_SetAnimationAndWait(9, 3);
  Event_ShowMessage(9, 0);
  Actor_SetAnimation(7, 3);
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Actor_ShowEmote(6, 257, 20);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(4, 257, 20);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  Event_ShowMessage(0x2007, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  Actor_SetAnimation(7, 3);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(5, 265, 40);
  Event_ShowMessage(5, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  Actor_SetAnimation(7, 4);
  Event_ShowMessage(0x2007, 0);
  Actor_SetAnimationAndWait(5, 3);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Event_ShowMessage(7, 0);
  Actor_SetAnimationAndWait(9, 3);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  Event_ShowMessage(0x2007, 0);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Actor_ShowEmote(9, 264, 20);
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(6, FACING_WEST);
  Actor_SetAnimation(6, 3);
  Event_ShowMessage(6, 0);
  Actor_SetAnimation(5, 3);
  Event_ShowMessage(5, 0);
  Actor_WalkByAndWait(7, -16, 0);
  FieldEvent_Wait(40);
  Actor_WalkByAndWait(7, -16, 0);
  Actor_ShowEmote(7, 261, 20);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_ShowEmote(5, 257, 40);
  Actor_WalkByAndWait(5, 0, 16);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_ShowMessageThenWait(7, 0, 20);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(9, FACING_WEST);
  Actor_SetAnimation(9, 3);
  Event_OpenMessage(9, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 1) {
    Actor_SetAnimationAndWait(9, 4);
    Event_ShowMessage(9, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    Actor_SetAnimationAndWait(9, 3);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Event_ShowMessage(9, 0);
  }
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(9, FACING_SOUTH + FACING_STEP);
  Actor_ShowEmote(9, 264, 20);
  Event_ShowMessage(9, 0);
  Actor_ShowEmote(7, 264, 0);
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(9, FACING_WEST);
  Actor_SetAnimationAndWait(9, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimationAndWait(7, 3);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetSpeed(9, 0x13333, 0x9999);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetScript(5, gLemurianShipDeck_ReachPiersShipScript);
  Actor_SetScript(9, gLemurianShipDeck_ReachPiersShipScript);
  Actor_SetScript(6, gLemurianShipDeck_ReachPiersShipScript);
  Actor_RunScriptAndWait(7, gLemurianShipDeck_ReachPiersShipScript);
  FieldEvent_Wait(20);
  Event_SetFlag(0x981);
  Event_End();
}

DEFINE_ACTOR_EMIT_SMOKE(Actor_EmitSmoke, 7, Actor_SelectAnimation,
                        gLemurianShipDeck_SmokePuff)

/// ★ Named from the game: "Then let's waste no time! We should set out for
/// Madra at once!" (MsgThenLetS)
void LemurianShipDeck_SetOutForMadra(void) {
  if (Event_GetFlag(7)) {
    struct Actor *object;

    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_SetSpeed(4, 0xcccc, 0x6666);
    Actor_Get(7)->update = (void (*)(void))Actor_EmitSmoke;
    FieldActor_LaunchFromLeader(7, 16, 0, FACING_WEST);
    Actor_SetSpriteFlags(Actor_Get(7), 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
    FieldEvent_SetMessageId(MsgThenLetS);
    FieldActor_WaitForMoveById(7);
    Actor_Get(7)->update = 0;
    Event_ShowMessage(7, 0);
    Actor_SetAnimationAndWait(4, 3);
    Actor_Get(gMainRoutineWork.currentOwner)->update =
        (void (*)(void))Actor_EmitSmoke;
    Actor_WalkByAndWait(4, 0, 16);
    Actor_Get(gMainRoutineWork.currentOwner)->update = 0;
    Actor_Get(7)->update = (void (*)(void))Actor_EmitSmoke;
    Actor_SetAnimation(7, 2);
    object = Actor_Get(4);
    if (object != 0)
      Actor_SetDestination(7, *(s16 *)((u8 *)object + 10),
                           *(s16 *)((u8 *)object + 18));
    FieldActor_WaitForMoveById(7);
    Actor_SetPosition(7, 0, 0);
    Actor_Get(7)->update = 0;
    Actor_SetSpriteFlags(Actor_Get(7), 1);
    Event_End();
  }
}

#include "objcmd.h"

// clang-format off
s16 gLemurianShipDeck_Puddles[] = {
    8, 512, -1, 0,
};

u32 gLemurianShipDeck_ReachPiersShipScript[] = {
    39, 2, 4, 0x1070000, 0, 0xb00000,
    1, 39, 1, 0, 1, 3,
    0, 1, 17,
};

u32 gLemurianShipDeck_ReturnToShipScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(368), 0, PIXELS(448)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_END,
};

DEFINE_G_DATA_0200218C(gLemurianShipDeck_FirstCrewLooksAround, 30, 50, 10, 70)

u32 gLemurianShipDeck_SecondCrewLooksAround[] = {
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(70),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gLemurianShipDeck_ThirdCrewLooksAround[] = {
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_WAIT(70),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(43),
    SCENE_EXIT(1, 40, 1),
    SCENE_EXIT(1, 2, 68),
    SCENE_EXIT(2, 2, 8),
    SCENE_EXITS(42),
    SCENE_EXIT(1, 2, 42),
    SCENE_EXIT(2, 2, 8),
    SCENE_EXIT(10, 2, 71),
    SCENE_EXITS_END,
};

struct ActorPlacement gLemurianShipDeck_Placements[] = {
    {290, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(312), FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gLemurianShipDeck_Room1Placements[] = {
    {290, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gLemurianShipDeck_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gLemurianShipDeck_Room2Triggers[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(LemurianShipDeck_EnterCabin)},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 10, 0x980, EVENT(LemurianShipDeck_SightMenardisShip)},
    {TRIGGER_STEP, 11, 0x981, EVENT(LemurianShipDeck_ReachPiersShip)},
    {TRIGGER_STEP, 12, 0x989, EVENT(LemurianShipDeck_SetOutForMadra)},
    {TRIGGER_STEP, 1, 0x985, EVENT(LemurianShipDeck_ReturnToShip)},
    {TRIGGER_EXAMINE, 14, 0x985, EVENT(LemurianShipDeck_TryHeldDoor)},
    {TRIGGER_STEP, 15, 0x985, EVENT(LemurianShipDeck_LeaveToFindCrew)},
    {TRIGGER_STEP, 16, 0x985, EVENT(LemurianShipDeck_ResetCrewSearch)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, 0x200, EVENT(LemurianShipDeck_FreezePuddle)},
    {TRIGGER_TABLE_END},
};

u32 gLemurianShipDeck_SparkleFades[] = {
    0, 10, 1, 3, 0, 1,
    38,
};

u32 gLemurianShipDeck_SmokePuff[] = {
    0, 15, 3, 0, 1, 38,
};
// clang-format on
