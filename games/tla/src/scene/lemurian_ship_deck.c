/// @file
/// The Lemurian ship's deck, in two scenes: the deck at the tiller, where the
/// crew takes its places as the leader sails (Room1), and the deck where the
/// party sights a ship, reaches Piers' ship, sets the Black Crystal back in it
/// and sets out for Madra (Room2).

#include "battle/battle.h"
#include "flags.h"
#include "item_ids.h"
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
#include "field/field_sequence_event.h"
#include "module/scene/scene.h"
#include "shared/tables.h"
#include "version.h"
#include "macros.h"
#include "field_sprite.h"
#include "system/task.h"
#include "call.h"
#include "objcmd.h"
#include "text/msg_ids.h"

extern s32 gLemurianShipDeck_WaveScroll;
extern s32 gLemurianShipDeck_BobPhase;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gLemurianShipDeck_Placements[];
extern struct ActorPlacement gLemurianShipDeck_Room1Placements[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_LemurianShipDeck1)
    return gLemurianShipDeck_Room1Placements;
  return gLemurianShipDeck_Placements;
}

/// Once flag 0x985 is set, the leader goes through the cabin door, the door
/// opening first unless it already has (flag 0x201).
void LemurianShipDeck_EnterCabin(void) {
  if (Event_GetFlag(0x985)) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;
    u32 i;

    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    i = 8;
  loop:
    {
      struct Actor *object = Actor_Get(i);

      if (object != 0) {
        object->motionFlags = 0;
        object->y = px(32);
      }
    }
    i++;
    if (i <= 63)
      goto loop;
    if (!Event_GetFlag(FLAG_SCENE_TEMP_01)) {
      Event_PlaySound(SE_DOOR_OPEN);
      MapGrid_CopyCellsTo(51, 38, 70, 22, 1, 2);
    }
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
    Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -4);
    FieldEvent_Wait(8);
    Event_PlaySound(SE_MAP_EXIT);
    FieldEvent_SetExitRequest(work->stepTrigger);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Event_End();
  }
}

extern struct Trigger gLemurianShipDeck_Triggers[];
extern struct Trigger gLemurianShipDeck_Room2Triggers[];

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_LemurianShipDeck2)
    return gLemurianShipDeck_Room2Triggers;
  return gLemurianShipDeck_Triggers;
}

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

#include "text/msg_ids.h"

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
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(342, 48, 408, 0);
  System_Wait(1);
  MapGrid_CopyCellsTo(60, 53, 67, 16, 7, 3);
  LemurianShipDeck_PlaceFacing(gMainRoutineWork.currentOwner, px(552), px(376),
                               FACING_EAST);
  LemurianShipDeck_PlaceFacing(9, px(558), px(345), 0xd000);
  LemurianShipDeck_PlaceFacing(7, px(534), px(404), FACING_EAST);
  turn = 0x2000;
  LemurianShipDeck_PlaceFacing(6, px(568), px(408), turn);
  LemurianShipDeck_PlaceFacing(5, px(546), px(440), 0x4000);
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
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(542, 44, 396, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(80);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  FieldEvent_SetMessageId((s32)MsgTakeTheTiller);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  gFieldWork->message++;
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHWEST, 20);
  Event_ShowMessage(7, 0);
  FieldActor_Stop(5);
  FieldActor_Stop(9);
  FieldActor_Stop(6);
  System_Wait(1);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(9, turn);
  Event_ShowMessage(9, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessage(5, 0);
  Event_OpenMessage(7, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    Event_ShowMessage(9, 0);
    gFieldWork->message++;
  } else {
    Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    gFieldWork->message++;
    Event_ShowMessage(9, 0);
  }
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_ShowMessage(7, 0);
  Event_ShowMessage(9, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(9, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessage(9, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(5, 0);
  Actor_FaceDirectionAndWait(9, FACING_SOUTH + FACING_STEP);
  Event_ShowMessage(9, 0);
  Event_ShowMessage(6, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_FaceDirectionAndWait(5, FACING_NORTHEAST);
  Event_ShowMessage(5, 0);
  Actor_SetAttachedEffect(6, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirectionAndWait(6, FACING_SOUTHWEST);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 6, 0);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
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

  Actor_SetSpeed(id, 1.2);
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
  /* FAKEMATCH(dead): a call that never runs, kept for its routine address, the
   * first constant the function puts in its pool: the message id's pool label
   * is then numbered one later, and reload's hard-register CSE. */
  if (0)
    Task_AddFrame(LemurianShipDeck_BobDeck, 0x480);
#endif
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(458, 48, 408, 0);
  FieldEvent_SetMessageId(MsgHoldOnTo);
  Actor_SetPositionAndFacing(4, px(470), px(392), FACING_EAST);
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
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_FaceDirectionAndWait(6, FACING_SOUTH);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  Actor_FaceDirectionAndWait(5, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
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
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirectionAndWait(9, FACING_SOUTHEAST);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 5);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  Actor_FaceEachOther(6, 5, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(5, FACING_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
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
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
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
   * stored back. */
  bits.mode = 1;
  /* FAKEMATCH(dead): an empty once-only block before the copy's store. */
  do {
  } while (0);
  party = &gMainRoutineWork;
  map->bits = bits;
  if (party->scene == (s32)&SceneId_LemurianShipDeck1) {
    LemurianShipDeck_SetUpTillerDeck();
  } else {
    gFieldWork->transition = 0x100;
    /* FAKEMATCH(dead): an empty once-only block. */
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
  if (gMainRoutineWork.scene == (s32)&SceneId_LemurianShipDeck1) {
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

  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) != 0) {
    Event_SetFlag(0x980);
    return;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(4, 0.8);
  Actor_WalkToAndWait(4, 264, 136);
  FieldEvent_SetMessageId(MsgLookItS);
  FieldActor_LaunchFromLeader(5, 16, -16, FACING_SOUTH);
  FieldActor_LaunchFromLeader(9, 32, -16, FACING_SOUTH);
  FieldActor_LaunchFromLeader(6, -16, -16, FACING_SOUTH);
  FieldEvent_Wait(80);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  Actor_SetSpeed(9, 1.6);
  Actor_WalkToAndWait(9, 300, 152);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Event_ShowMessage(9, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(200, CAMERA_KEEP, 344, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(600, CAMERA_KEEP, 344, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  FieldCamera_SetSpeed(6.4);
  FieldCamera_MoveTo(264, CAMERA_KEEP, 128, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimationAndWait(5, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 5, 0);
  Actor_FaceDirectionAndWait(6, FACING_EAST);
  Actor_SetAnimation(6, 4);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTHWEST + FACING_STEP);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
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
    gFieldWork->message++;
  } else {
    Actor_WaitForAnimationChange(owner);
    FieldEvent_SetMessageId(MsgLookItS + 5); // the fifth message after it
    Event_ShowMessage(9, 0);
  }
  Actor_SetAnimation(5, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 5, 0);
  Actor_FaceDirectionAndWait(6, FACING_EAST);
  Actor_SetAnimation(6, 4);
  Event_ShowMessage(6, 0);
  Actor_SetAnimation(9, 4);
  Event_ShowMessage(9, 0);
  Actor_SetSpeed(9, 0.8);
  Actor_Get(9)->actionFlags &= ~1;
  Actor_WalkToAndWait(9, 288, 144);
  FieldEvent_Wait(1);
  Actor_Get(9)->actionFlags |= 1;
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 9, 0);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimationAndWait(9, 3);
  Actor_SetSpeed(9, 1.2);
  Actor_SetSpeed(6, 1.2);
  Actor_SetSpeed(5, 1.2);
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

void LemurianShipDeck_LeaveToFindCrew(void) {
  struct Actor *object;

  if (Event_GetFlag(FLAG_SCENE_TEMP_12))
    return;
  if (Event_GetFlag(0x988))
    return;
  Event_SetFlag(FLAG_SCENE_TEMP_12);
  if (Event_GetFlag(FLAG_SCENE_TEMP_11)) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
    System_Wait(1);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
    Actor_SetSpeed(9, 0.8);
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
  } else if (Event_GetFlag(FLAG_SCENE_TEMP_10)) {
    Event_SetFlag(FLAG_SCENE_TEMP_11);
  } else {
    Event_SetFlag(FLAG_SCENE_TEMP_10);
  }
}

/// Stepping back off the gangway clears flag 0x212, so the crew is sought
/// again.
void LemurianShipDeck_ResetCrewSearch(void) {
  GameFlag_Clear(FLAG_SCENE_TEMP_12);
}

void Actor_SelectAnimation(struct Actor *object, s32 value);
void Actor_StartScript(struct Actor *object, const void *data);

/// A task: a sparkle rises from around the Black Crystal toward the ship's
/// core.
void LemurianShipDeck_EmitSparkle(void) {
  s32 position[3];
  struct Actor *object;

  SET_VECTOR(position, 0x1680000, 0x400000, 0x1a80000);
  FieldVector_AddPolarOffset(px(32), Random16(), position);
  object = FieldActor_Create(0x2a2, position[0], position[1], position[2]);
  if (object != NULL) {
    object->motionFlags = 0;
    *(s32 *)((u8 *)object + 0x30) = 0x20000;
    *(s32 *)((u8 *)object + 0x34) = 0x8000;
    Actor_SetSpriteFlags(object, 0);
    Actor_SelectAnimation(object, 0);
    FieldActor_SetMoveTarget(object, 360, 64, 424);
    Actor_StartScript(object, gLemurianShipDeck_SparkleFades);
  }
}

/// The Black Crystal (item 242's icon) as an actor on the deck.
struct Actor *LemurianShipDeck_CreateBlackCrystal(void) {
  struct Actor *object =
      (struct Actor *)Import_ActorCreate(0x1e9, px(360), px(64), px(424));
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
  Icon_DrawItem(ITEM_BLACK_CRYSTAL);
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
  Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
  System_Wait(1);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_SetSpeed(7, 1.6);
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
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
  Actor_SetScript(gMainRoutineWork.currentOwner,
                  gLemurianShipDeck_ReturnToShipScript);
  Actor_SetSpeed(7, 0.8);
  Actor_WalkToAndWait(7, 360, 424);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  Event_PlaySound(SE_GEM_SET);
  handle = LemurianShipDeck_CreateBlackCrystal();
  FieldEvent_Wait(20);
  Event_PlaySound(SE_SHIMMER);
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
  Event_SetFlag(FLAG_SCENE_TEMP_01);
  FieldTask_Remove(func);
  if (handle != 0)
    Actor_Release(handle);
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
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) == 0)
    return;
  if (Event_GetFlag(0x989) != 0)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgMyShipAt);
  Actor_WalkTo(4, 264, 176);
  FieldActor_WaitForMoveById(4);
  FieldActor_LaunchFromLeader(7, 16, 8, FACING_SOUTH);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(288, CAMERA_KEEP, 208, 1);
  FieldActor_LaunchFromLeader(9, 32, 0, FACING_SOUTH);
  FieldActor_LaunchFromLeader(6, 48, -8, FACING_SOUTH);
  FieldActor_LaunchFromLeader(5, -16, -8, FACING_SOUTH);
  FieldEvent_Wait(40);
  Actor_SetSpeed(7, 2.4);
  Actor_WalkToAndWait(7, 280, 224);
  FieldEvent_Wait(20);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  FieldActor_Jump(7, 4, 10);
  FieldActor_Jump(7, 4, 20);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessage(9, 0);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(288, CAMERA_KEEP, 184, 1);
  Actor_SetSpeed(7, 0.8);
  Actor_WalkByAndWait(7, 0, -16);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(7, 3);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  Actor_FaceDirectionAndWait(6, FACING_SOUTHWEST);
  Actor_SetAnimation(6, 4);
  Event_ShowMessage(6, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Actor_SetAnimation(6, 4);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_SetAnimationAndWait(5, 3);
  Event_ShowMessage(5, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  Actor_SetAttachedEffect(7, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
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
    Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    Event_ShowMessage(5, 0);
    Event_ShowMessage(6, 0);
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 2;
    Actor_FaceDirection(7, FACING_NORTHWEST, 0);
    Actor_SetAnimationAndWait(5, 3);
    Event_ShowMessage(5, 0);
    Actor_FaceDirection(7, FACING_NORTHEAST, 0);
    Actor_SetAnimation(6, 3);
    Event_ShowMessage(6, 0);
  }
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTHEAST);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_BELOW | 9, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Actor_SetAnimation(7, 4);
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirection(6, FACING_WEST, 40);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(6, FACING_SOUTHWEST);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_SetAttachedEffect(9, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Event_ShowMessage(9, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(5, FACING_EAST, 20);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_EAST);
  Actor_FaceDirection(9, FACING_WEST, 40);
  Actor_FaceDirectionAndWait(9, FACING_SOUTH);
  Event_ShowMessage(9, 0);
  Actor_SetAnimationAndWait(7, 3);
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_RunRepeatedMotion(9, 3);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_SetAttachedEffect(5, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  Actor_FaceDirectionAndWait(9, FACING_WEST);
  FieldActor_StartRepeatedMotion(9, 2);
  Actor_SetAnimationAndWait(9, 3);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(9, FACING_SOUTH + FACING_STEP);
  FieldActor_StartRepeatedMotion(9, 2);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_SetAnimationAndWait(9, 3);
  Event_ShowMessage(9, 0);
  Actor_SetAnimation(7, 3);
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  Actor_SetAnimation(7, 3);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Event_ShowMessage(5, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  Actor_SetAnimation(7, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_SetAnimationAndWait(5, 3);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Event_ShowMessage(7, 0);
  Actor_SetAnimationAndWait(9, 3);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
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
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
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
    gFieldWork->message += 1;
  } else {
    Actor_SetAnimationAndWait(9, 3);
    gFieldWork->message += 1;
    Event_ShowMessage(9, 0);
  }
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(9, FACING_SOUTH + FACING_STEP);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessage(9, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(9, FACING_WEST);
  Actor_SetAnimationAndWait(9, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimationAndWait(7, 3);
  Actor_SetSpeed(5, 1.2);
  Actor_SetSpeed(9, 1.2);
  Actor_SetSpeed(6, 1.2);
  Actor_SetSpeed(7, 1.2);
  Actor_SetScript(5, gLemurianShipDeck_ReachPiersShipScript);
  Actor_SetScript(9, gLemurianShipDeck_ReachPiersShipScript);
  Actor_SetScript(6, gLemurianShipDeck_ReachPiersShipScript);
  Actor_RunScriptAndWait(7, gLemurianShipDeck_ReachPiersShipScript);
  FieldEvent_Wait(20);
  Event_SetFlag(0x981);
  Event_End();
}

#include "shared/actor_emit_smoke.h"

void LemurianShipDeck_SetOutForMadra(void) {
  if (Event_GetFlag(7)) {
    struct Actor *object;

    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_SetSpeed(4, 0.8);
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
    SC_MOVE(px(368), 0, px(448)),
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

u32 gLemurianShipDeck_FirstCrewLooksAround[] = {                                                               
      SC_SET(SW_FACING, FACING_SOUTH),     SC_WAIT(30),                        
      SC_SET(SW_FACING, FACING_SOUTHWEST), SC_WAIT(50),                        
      SC_SET(SW_FACING, FACING_SOUTH),     SC_WAIT(10),                        
      SC_SET(SW_FACING, FACING_SOUTHEAST), SC_WAIT(70),                        
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

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
    SCENE_EXITS(LemurianShipDeck2),
    SCENE_EXIT(1, LemurianShip1, 1),
    SCENE_EXIT(1, WorldMap, 68),
    SCENE_EXIT(2, WorldMap, 8),
    SCENE_EXITS(LemurianShipDeck1),
    SCENE_EXIT(1, WorldMap, 42),
    SCENE_EXIT(2, WorldMap, 8),
    SCENE_EXIT(10, WorldMap, 71),
    SCENE_EXITS_END,
};

struct ActorPlacement gLemurianShipDeck_Placements[] = {
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(5, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(6, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(7, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gLemurianShipDeck_Room1Placements[] = {
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(5, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(6, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(7, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gLemurianShipDeck_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gLemurianShipDeck_Room2Triggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(LemurianShipDeck_EnterCabin)},
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
