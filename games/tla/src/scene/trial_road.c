/// @file
/// Trial Road, the mountain path above the Shaman Village where the party races
/// Moapa to the summit: four stages dealt from eight puzzle rooms, a left and a
/// right path through each, the blue chests that open each stage's door for
/// the weight of the party's equipment, and the battle at the peak; after the
/// race, the summit's cave and pools and Gasp the Djinni.

#include "shared/trial_road.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "module/field/object.h"

void TrialRoad_FightGasp(void) { Field_RequestDjinnBattle(8, 75); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gTrialRoad_Placements[];
extern const u8 gTrialRoad_Room3Placements[];
extern const u8 gTrialRoad_Room4Placements[];
extern const u8 gTrialRoad_Room2Placements[];
extern const u8 *const gTrialRoad_Room1Placements[];

s32 TrialRoad_SettleOnGround(struct Actor *object) {
  s32 height = Map_GetTerrainHeight(0, object->x, object->z) + 0x20000;

  object->terrainHeight = height;
  object->y = height;
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->scaleY = 0x18000;
  return 0;
}

const u8 *Scene_GetPlacements(void) {
  s32 pattern = gTrialRoadState->pattern;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room1)
    return gTrialRoad_Room1Placements[pattern & 7];
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room3)
    return gTrialRoad_Room3Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room4)
    return gTrialRoad_Room4Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room2)
    return gTrialRoad_Room2Placements;
  return gTrialRoad_Placements;
}

s32 TrialRoad_RivalIsAhead(s32 fallback) {
  struct TrialRoadState *state = gTrialRoadState;

  if (state->leaderBest < state->rivalBest)
    return 1;
  if (state->leaderBest > state->rivalBest)
    return 0;
  if (Event_GetFlag(0x284))
    return 0;
  if (state->rivalTimer != 0)
    return 1;
  return fallback;
}

#include "event_inline.h"

/* VIEW(trial_road): routine TrialRoad_OfferToGiveUp as s32
 * TrialRoad_OfferToGiveUp(s32 value); scene/trial_road/s0218 has s32
 * TrialRoad_OfferToGiveUp(void) {. */
s32 TrialRoad_OfferToGiveUp__trial_road(s32 value) __asm__(
    "TrialRoad_OfferToGiveUp");

void TrialRoad_TakeStairs(s32 direction) {
  struct TrialRoadState *state = gTrialRoadState;
  s16 *order = (s16 *)state;

  if ((u32)(direction - 1) <= 1) {
    if (state->stage[0] <= 0) {
      if (Event_GetFlag(2559)) {
        if (TrialRoad_OfferToGiveUp__trial_road(0) == 0)
          Actor_WalkByAndWait(gMainRoutineWork.currentOwner, 0, -16);
        return;
      }
      Import_EventSetExitRequest(direction + 4);
    } else {
      state->stage[0]--;
      Import_EventSetExitRequest(direction);
    }
    Event_PlaySound(123);
  }
  if ((u32)(direction - 3) <= 1) {
    state->stage[0]++;
    if (state->stage[0] > 3) {
      if (Event_GetFlag(2559) && TrialRoad_RivalIsAhead(0) == 0)
        Event_SetFlag(901);
      Event_PlaySound(128);
      Exit_StartStairsUp();
      Import_EventSetExitRequest(direction + 4);
    } else {
      Import_EventSetExitRequest(direction);
      Event_PlaySound(123);
    }
    if (state->stage[0] > state->leaderBest)
      state->leaderBest = state->stage[0];
  }
  {
    /* FAKEMATCH(temporary): the byte offset kept in a local; as order[index], 2
     * of 93 instructions differ in tla-en. */
    s32 offset = (state->stage[0] * 2 + state->mirrored) * 2;

    state->pattern = *(s16 *)((u8 *)order + offset);
  }
}

// ---- scene/trial_road/s0218.c ----

/// Trial Road's stage puzzles: the stairs, chests and doors, and the pillars,
/// torches, logs, spouts and sand the eight rooms are built from.

#include "shared/trial_road.h"
#include "event_inline.h"
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "macros.h"
#include "text/msg_ids.h"
#include "vector3.h"
#include "field.h"
#include "event_work.h"
#include "module/scene/trial_road.h"
#include "module/window/window.h"

#define DEFINE_TRIAL_ROAD_OPEN_DOOR(name, n1, n2, n3, n4, n5, n6)              \
  void name(s32 side, s32 phase) {                                             \
    if (side == 0) {                                                           \
      if (phase == 3)                                                          \
        MapGrid_CopyCellPictureCellsRect(0, 120, 1, 2, n1, phase);             \
      MapGrid_CopyCellsLayered(127, 3, 1, 2, n2, 3);                           \
      MapGrid_CopyCellAttributes(n3, 127, 1, 1, n1, 8);                        \
    } else {                                                                   \
      if (phase == 3)                                                          \
        MapGrid_CopyCellPictureCellsRect(0, 120, 1, 2, n4, phase);             \
      MapGrid_CopyCellsLayered(127, 3, 1, 2, n5, 3);                           \
      MapGrid_CopyCellAttributes(n6, 127, 1, 1, n4, 8);                        \
    }                                                                          \
  }

DEFINE_TRIAL_ROAD_OPEN_DOOR(TrialRoad_OpenLeaderDoor, 23, 57, 0, 9, 43, 1)

DEFINE_TRIAL_ROAD_OPEN_DOOR(TrialRoad_OpenRivalDoor, 28, 62, 1, 4, 38, 0)

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/// A step at a stage's foot asks; giving up takes the party back to the start.
/// ★ Named from the game: "Do you want to give up?" (MsgTrialRoadGiveUpPrompt)
s32 TrialRoad_OfferToGiveUp(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EventWork *work = Ram_HeapSlots->eventWork;
  s32 message;

  if (Event_GetFlag(0x9ff)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner,
                        ((leader->x >> 20) << 4) + 8,
                        ((leader->z >> 20) << 4) + 8);
    Event_PlaySound(229);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    message = (s32)&MsgTrialRoadGiveUpPrompt;
    FieldMessage_Show(message, 13);
    work->unkcb0 = message + 1;
    work->unkcb2 = 4;
    work->unkcb4 = 1;
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 1) ==
        0) {
      Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
      Event_Wait(10);
      Actor_SetAnimation(gMainRoutineWork.currentOwner, 22);
      Event_Wait(10);
      FieldActor_ShowEmote(gMainRoutineWork.currentOwner, 0x109, 0);
      Event_Wait(30);
      InventorySnapshot_Restore();
      Import_EventSetExitRequest(9);
      return 1;
    }
  }
  Event_SetFlag(0x281);
  return 0;
}

void TrialRoad_ClearGiveUpAsked(void) { GameFlag_Clear(0x281); }

/// The leader reaches the door at a stage's end: the blue chests open (flags
/// 645 and 646), the second only when the rival arrived first and twice the
/// weight is due.
void TrialRoad_OpenChests(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct TrialRoadState *state = gTrialRoadState;
  s32 found;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner,
                      ((leader->x >> 20) << 4) + 8,
                      ((leader->z >> 20) << 4) + 8);
  TrialRoad_Idle();
  found = TrialRoad_RivalIsAhead(0);
  if (found != 0)
    Event_SetFlag(state->stage[0] + 897);
  Event_PlaySound(124);
  if (state->mirrored == 0) {
    Actor_SetAnimation(9, 6);
    Event_SetFlag(645);
    if (found != 0) {
      Actor_SetAnimation(10, 6);
      Event_SetFlag(646);
    }
  } else {
    Actor_SetAnimation(11, 6);
    Event_SetFlag(645);
    if (found != 0) {
      Actor_SetAnimation(12, 6);
      Event_SetFlag(646);
    }
  }
  Event_SetFlag(644);
  Event_End();
}

/// Puts an item in the chest: "{protagonist} put {actor}'s {item} in the
/// chest." (MsgTrialRoadStoredItem) with sound 125 while the chest object
/// Field_FindNearbyTalkTarget picks plays motion 5; chests 9 and 11 clear flag
/// 645, the others 646, and once both are clear it calls
/// TrialRoad_OpenLeaderDoor with the stage state's mirrored and stage[0],
/// plays sound 138 and sets flag 642. Then the item leaves the owner's slot.
void TrialRoad_PutItemInChest(s32 item, s32 owner, s32 slot) {
  s32 kind = Field_FindNearbyTalkTarget(Party_GetLeaderActor());
  struct TrialRoadState *state = gTrialRoadState;

  Event_Start();
  Event_ResolvePendingActions(0);
  MessageArgument_Push(owner, 1);
  MessageArgument_Push(item, 2);
  FieldMessage_Show((s32)&MsgTrialRoadStoredItem, 1);
  Event_PlaySound(125);
  Actor_SetAnimationAndWait(kind, 5);
  if (kind == 9 || kind == 11)
    Event_ClearFlag(645);
  else
    Event_ClearFlag(646);
  if (!Event_GetFlag(645) && !Event_GetFlag(646)) {
    TrialRoad_OpenLeaderDoor(state->mirrored, state->stage[0]);
    Event_PlaySound(138);
    Event_SetFlag(642);
  }
  Inventory_ClearSlot(owner, slot);
  Event_End();
}

/* VIEW(s0218): tag struct EventWork15a as struct EventWork15a__s0218;
 * module/tla.h has another definition; tried with the definition the rest of
 * the object has, its members read value as flag, it does not compile in
 * tla-ja: games/tla/src/scene/trial_road/s0218.c:171: dereferencing pointer to
 * incomplete type. */
struct EventWork15a__s0218 {
  u8 unk000[0x15a];
  u16 value;
};

/// ★ Named from the game: "Place this item in the chest?"
/// (MsgTrialRoadPlaceItemPrompt)
void TrialRoad_ExamineChest(s32 id) {
  s32 flag;

  if (id == 9 || id == 11)
    flag = 0x285;
  else
    flag = 0x286;
  if (!Event_GetFlag(flag)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldMessage_Show((s32)MsgTrialRoadChestBlocked, 1);
    Event_End();
  } else {
    FieldMessage_Show((s32)MsgTrialRoadPlaceItemPrompt, 13);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 1) == 0)
      ((struct EventWork15a__s0218 *)Ram_HeapSlots->eventWork)->value = 1;
  }
}

void TrialRoad_StepPastChests(void) {}

void TrialRoad_BlockCellUnder(s32 id) {
  struct TrialRoadMapWork *map =
      (struct TrialRoadMapWork *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    u8 *cells = map->layers[0].cells;
    u8 *cell = cells + (((object->x >> 20) + ((object->z >> 20) << 7)) << 2);
    s32 bit = 128;

    cell[3] = bit | cell[3];
  }
}

void TrialRoad_UnblockCellUnder(s32 id) {
  struct TrialRoadMapWork *map =
      (struct TrialRoadMapWork *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    u8 *cells = map->layers[0].cells;
    u8 *cell = cells + (((object->x >> 20) + ((object->z >> 20) << 7)) << 2);

    cell[3] &= 0x7f;
  }
}

s32 TrialRoad_FollowPillar(struct Actor *object) {
  struct Actor *owner = object->watchedObject;

  SET_POSITION(object, owner->x, owner->y + (OBJECT_BYTE(object, 99) << 16),
               owner->z);
  object->terrainHeight = owner->terrainHeight;
  return 1;
}

extern const u8 gTrialRoad_PillarTopScript[];

void TrialRoad_AddPillarTop(s32 id) {
  struct Actor *owner = Actor_GetById(id);
  struct Actor *object;

  if (owner != NULL) {
    object = Actor_Create(489, owner->x, owner->y, owner->z);
    if (object != NULL) {
      Actor_SetSpriteFlags(object, 0);
      Actor_StartScript(object, gTrialRoad_PillarTopScript);
      object->watchedObject = owner;
      object->priorityFlags |= 0x80;
      object->motionFlags = 0;
      object->collisionFlags = 0;
      OBJECT_BYTE(object, 99) = 16;
      owner->watchedObject = object;
    }
  }
}

void TrialRoad_SetPillarTopLift(s32 id, s32 value) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    object = object->watchedObject;
    if (object != NULL)
      OBJECT_BYTE(object, 99) = value;
  }
}

/// While the party's render mode is 0, clears the map cell flag under the
/// object.
#define DEFINE_TRIAL_ROAD_ON_FOOT(name, call1)                                 \
  void name(s32 a, s32 id) {                                                   \
    struct MainRoutineWork *party = &gMainRoutineWork;                         \
    u8 mode;                                                                   \
                                                                               \
    mode = party->renderMode;                                                  \
                                                                               \
    if (mode == 0)                                                             \
      call1(id);                                                               \
  }

DEFINE_TRIAL_ROAD_ON_FOOT(TrialRoad_UnblockMovedPillar,
                          TrialRoad_UnblockCellUnder)

/// While the party's render mode is 0, sets the map cell flag under the object.
DEFINE_TRIAL_ROAD_ON_FOOT(TrialRoad_BlockMovedPillar, TrialRoad_BlockCellUnder)

void TrialRoad_BlowBushOffLonePillar(void) {
  OBJECT_BYTE(Actor_GetById(17), 98) = 0;
  Event_SetFlag(0x307);
}

void TrialRoad_JumpForward(void) { Field_TryJumpForward(); }

void TrialRoad_OpenSpoutCell(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL)
    MapGrid_CopyCellsLayered(14, 27, 1, 1, object->x >> 20, object->z >> 20);
}

void TrialRoad_TrackLeaderOnPillars(void) {
  s32 y = Actor_GetById(gMainRoutineWork.currentOwner)->y;

  if (y == 0x200000)
    Event_SetFlag(0x208);
  else
    Event_ClearFlag(0x208);
}

void TrialRoad_JumpAcrossGap(void) {
  if (Actor_GetById(16)->z >> 20 == 23)
    Field_TryJumpForward();
  else
    Actor_JumpDownLedge(gMainRoutineWork.currentOwner, 6, 0);
}

void TrialRoad_UnblockTorch(s32 a, s32 id) {
  if ((u32)(id - 15) <= 1)
    TrialRoad_UnblockCellUnder(id);
}

void TrialRoad_MeltIceBesideTorch(s32 a, s32 id) {
  s32 x;

  if ((u32)(id - 15) <= 1) {
    TrialRoad_BlockCellUnder(id);
    return;
  }
  x = (s32)Actor_GetById(id);
  x = ((struct Actor *)x)->x >> 20;
  id = 0;
  if (x == 13)
    id = 17;
  if (x == 15)
    id = 18;
  if (x == 17)
    id = 19;
  if (x == 19)
    id = 20;
  if (id != 0) {
    s32 flag = id + 0x203;

    if (Event_GetFlag(flag) == 0) {
      struct Actor *object = Actor_GetById(id);

      object->update = NULL;
      Actor_SetSpriteColorMode(object, 0);
      Actor_SetAnimation(id, 6);
      TrialRoad_SetPillarTopLift(id, -16);
      Event_SetFlag(flag);
    }
  }
}

void TrialRoad_JumpMeltedPuddle(void) {
  if (Event_GetFlag(Field_FindNearbyTalkTarget(gMainRoutineWork.currentOwner) +
                    0x203))
    Field_TryJumpForward();
}

void TrialRoad_FreezePuddle(s32 a, s32 id) {
  struct Actor *object = Actor_GetById(id);

  TrialRoad_SetPillarTopLift(id, 16);
  Event_ClearFlag(id + 0x203);
  if (object != NULL)
    object->priorityFlags &= 0xfd;
}

void TrialRoad_BlowBushOffPillar(s32 a, s32 id) {
  OBJECT_BYTE(Actor_GetById(id - 5), 98) = 0;
  Event_SetFlag(id + 0x313);
}

void TrialRoad_LowerPillar(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    TrialRoad_SetPillarTopLift(id, -16);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    Actor_SelectAnimation(object, 6);
  }
}

void TrialRoad_RaisePillar(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    TrialRoad_SetPillarTopLift(id, 16);
    object->collisionFlags = 1;
    object->priorityFlags &= 0xfd;
    Actor_SelectAnimation(object, 5);
  }
}

void TrialRoad_BlowBushOffPoundPillar(void) {
  OBJECT_BYTE(Actor_GetById(16), 98) = 0;
  Event_SetFlag(0x339);
}

extern const s8 gTrialRoad_PoundPartners[4];

void TrialRoad_CheckPoundedPillarsCovered(s32 a, s32 b) {
  s32 id;

  id = 18;
  // Goto loops: as for or do-while loops, the table address and the 1 are moved
  // out of them (differ 29).
next:
  {
    struct Actor *object = Actor_GetById(id);
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;
    u8 *covered = &OBJECT_BYTE(
        Actor_GetById(gTrialRoad_PoundPartners[(id - 18) & 3]), 99);
    s32 other;

    other = 15;
    *covered = 0;
    goto test;
  step:
    other++;
  test:
    if (other <= 17) {
      object = Actor_GetById(other);
      if (x != object->x >> 20)
        goto step;
      if (z != object->z >> 20)
        goto step;
      *covered = 1;
    }
  }
  id++;
  if (id <= 21)
    goto next;
}

void TrialRoad_PoundPillar(s32 id, s32 mode, s32 frame) {
  s32 other = gTrialRoad_PoundPartners[(id - 18) & 3];
  struct Actor *object = Actor_GetById(id);
  struct Actor *partner = Actor_GetById(other);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  if (frame <= 10)
    object->scaleY = 0x10000 - (frame << 12);
  else
    object->scaleY = 0x10000;
  if (mode == 1) {
    TrialRoad_LowerPillar(id);
    TrialRoad_RaisePillar(other);
    switch (id) {
    case 18:
      Event_SetFlag(821);
      break;
    case 19:
      Event_ClearFlag(821);
      break;
    case 20:
      Event_SetFlag(823);
      break;
    case 21:
      Event_ClearFlag(823);
      break;
    }
    if (partner->x >> 20 == leader->x >> 20 &&
        partner->z >> 20 == leader->z >> 20) {
      leader->velocityY = 0x80000;
      leader->x = (leader->x & 0xfff00000) + 0x80000;
      leader->z = (leader->z & 0xfff00000) + 0x80000;
    }
  }
  if (frame == 9)
    partner->scaleY = 0x13333;
  else if (frame == 11)
    partner->scaleY = 0x11999;
  else if (frame == 13)
    partner->scaleY = 0x10ccc;
  else
    partner->scaleY = 0x10000;
}

void TrialRoad_PoundPillar18(s32 a, s32 b) { TrialRoad_PoundPillar(18, a, b); }

void TrialRoad_PoundPillar19(s32 a, s32 b) { TrialRoad_PoundPillar(19, a, b); }

void TrialRoad_PoundPillar20(s32 a, s32 b) { TrialRoad_PoundPillar(20, a, b); }

void TrialRoad_PoundPillar21(s32 a, s32 b) { TrialRoad_PoundPillar(21, a, b); }

void TrialRoad_StepOnSeesawPillar(s32 id, s32 other, s32 flag, s32 side) {
  struct Actor *object = Actor_GetById(id);
  struct Actor *partner = Actor_GetById(other);
  s32 t;

  if (OBJECT_BYTE(object, 0x63) != 0)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  partner->scaleY = 0x2000;
  wait(1);
  TrialRoad_RaisePillar(other);
  t = 0x2000;
grow:
  TrialRoad_SetPillarTopLift(id, (0x8000 - t) / 2048);
  object->scaleY = 0x10000 - t;
  partner->scaleY = t;
  wait(1);
  t += 0x2000;
  if (t <= 0x10000)
    goto grow;
  object->scaleY = 0x10000;
  TrialRoad_LowerPillar(id);
  Event_PlaySound(138);
  if (side != 0)
    Event_SetFlag(flag);
  else
    Event_ClearFlag(flag);
  Event_End();
}

void TrialRoad_StepOnPillar18(void) {
  TrialRoad_StepOnSeesawPillar(18, 19, 0x335, 1);
}

void TrialRoad_StepOnPillar19(void) {
  TrialRoad_StepOnSeesawPillar(19, 18, 0x335, 0);
}

void TrialRoad_StepOnPillar20(void) {
  TrialRoad_StepOnSeesawPillar(20, 21, 0x337, 1);
}

void TrialRoad_StepOnPillar21(void) {
  TrialRoad_StepOnSeesawPillar(21, 20, 0x337, 0);
}

void TrialRoad_LandOnPillar(void) {
  s32 y = Actor_GetById(gMainRoutineWork.currentOwner)->y;

  if (y >= 0x100000) {
    Field_TryJumpForward();
  } else if (Leader_CanPushObjectAhead()) {
    Leader_PushObjectAhead();
    TrialRoad_CheckPoundedPillarsCovered(0, 0);
  }
}

void TrialRoad_CheckCrumblingPillars(void) {
  TrialRoad_CheckCrumblingStep(gTrialRoad_CrumblingPillars);
}

void TrialRoad_CrumblePillar15(s32 phase) {
  if (phase == 3)
    TrialRoad_CrumblePillar(gTrialRoad_CrumblingPillars, 15);
}

void TrialRoad_CrumblePillar16(s32 phase) {
  if (phase == 3)
    TrialRoad_CrumblePillar(gTrialRoad_CrumblingPillars, 16);
}

// Blocks the three cells through an object's cell on layers 0 and 2 (collision
// code 255): the column above and below it in mode 0, the row beside it
// otherwise.
#include "map.h"

/// Blocks the three cells through an object's cell on layers 0 and 2 (collision
/// code 255): the column above and below it in mode 0, the row beside it
/// otherwise.
void TrialRoad_BlockLogCells(s32 id, s32 mode) {
  struct TrialRoadMapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);
  s32 index;
  struct MapCell *low;
  struct MapCell *high;

  if (object == 0)
    return;
  index = (object->x >> 20) + ((object->z >> 20) << 7);
  low = (struct MapCell *)map->layers[0].cells + index;
  high = (struct MapCell *)map->layers[2].cells + index;
  if (mode == 0) {
    low[-128].collisionCode = 255;
    low->collisionCode = 255;
    low[128].collisionCode = 255;
    high[-128].collisionCode = 255;
    high->collisionCode = 255;
    high[128].collisionCode = 255;
  } else {
    low[-1].collisionCode = 255;
    low->collisionCode = 255;
    low[1].collisionCode = 255;
    high[-1].collisionCode = 255;
    high->collisionCode = 255;
    high[1].collisionCode = 255;
  }
}

void TrialRoad_BlockLogs(void) {
  MapGrid_CopyCellAttributes(11, 64, 11, 8, 11, 22);
  MapGrid_CopyCellAttributes(79, 64, 11, 8, 79, 22);
  TrialRoad_BlockLogCells(16, 0);
  TrialRoad_BlockLogCells(17, 0);
  TrialRoad_BlockLogCells(18, 1);
  if (Event_GetFlag(0x253))
    MapGrid_CopyCellAttributes(0, 48, 1, 3, 19, 23);
  if (Event_GetFlag(0x254))
    MapGrid_CopyCellAttributes(0, 48, 1, 3, 19, 26);
}

s32 TrialRoad_IsLogBlocked(s32 x, s32 z) {
  struct Actor *object = Actor_GetById(15);
  struct TrialRoadMapWork *map =
      (struct TrialRoadMapWork *)Ram_HeapSlots->mapWork;
  u8 *cells;
  u8 *cell;
  s32 value;

  if (object->x >> 20 == x && object->z >> 20 == z)
    return 1;
  cells = map->layers[0].cells;
  cell = cells + ((x + (z << 7)) << 2);
  value = cell[2];
  return IsZero(value ^ 0xff);
}

void TrialRoad_PushLog(s32 id, s32 dx, s32 dz, s32 axis) {
  struct Actor *object = Actor_GetById(id);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 fromX = object->x >> 20;
  s32 fromZ = object->z >> 20;
  s32 toX = fromX;
  s32 toZ = fromZ;
  s32 n;
  s32 mode = 2;
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;
  // Unused: gives the stack frame its 12 bytes.
  struct Vector3 unused;

  if ((dx != 0 && toX != x + dx) || (dz != 0 && toZ != z + dz)) {
    ((struct EventWork184 *)Ram_HeapSlots->eventWork)->value = 13;
    return;
  }
  for (n = 0; n <= 5;) {
    x = toX + dx;
    z = toZ + dz;
    if (axis == 0) {
      if (TrialRoad_IsLogBlocked(x, z - 1))
        break;
      if (TrialRoad_IsLogBlocked(x, z))
        break;
      if (TrialRoad_IsLogBlocked(x, z + 1))
        break;
    } else {
      if (TrialRoad_IsLogBlocked(x - 1, z))
        break;
      if (TrialRoad_IsLogBlocked(x, z))
        break;
      if (TrialRoad_IsLogBlocked(x + 1, z))
        break;
    }
    toX = x;
    n++;
    toZ = z;
  }
  if (fromX == toX && fromZ == toZ)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  Event_Wait(6);
  object->speedLimit = 0x8000;
  object->acceleration = 0x3333;
  Sound_Play(239);
  if (dx < 0 || dz < 0)
    mode = 3;
  Actor_SelectAnimation(object, mode);
  Actor_SetMoveTarget(object, (toX << 20) + 0x80000, 0,
                      z = (toZ << 20) + 0x80000);
  Event_Wait(6);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x4ccc, 0x3333);
  Actor_SetDestinationOffsetById(gMainRoutineWork.currentOwner, dx << 3,
                                 dz << 3);
  Event_Wait(24);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Actor_WaitForMove(object);
  if (toX <= 17) {
    Actor_SelectAnimation(object, 1);
    Sound_Play(298);
    Sound_Play(213);
  } else {
    Actor_SelectAnimation(object, 3);
    Actor_SetMoveTarget(object, 312 << 16, 0, z);
    Event_Wait(30);
    Actor_SelectAnimation(object, 8);
    Actor_WaitForMove(object);
    {
      /* FAKEMATCH(pin): the 2 held in r2 while the address takes r3; as
       * object->priorityFlags = 2, 8 of 240 instructions differ in tla-en. */
      u8 *flags = &object->priorityFlags;
      register s32 value asm("r2") = 2;

      *flags = value;
    }
    Sound_Play(298);
    Sound_Play(240);
    GameFlag_Set(id + 579);
  }
  Event_Wait(15);
  Event_End();
  TrialRoad_BlockLogs();
}

void TrialRoad_PushLog16West(void) { TrialRoad_PushLog(16, -1, 0, 0); }

void TrialRoad_PushLog16East(void) { TrialRoad_PushLog(16, 1, 0, 0); }

void TrialRoad_PushLog17West(void) { TrialRoad_PushLog(17, -1, 0, 0); }

void TrialRoad_PushLog17East(void) { TrialRoad_PushLog(17, 1, 0, 0); }

void TrialRoad_PushLog18North(void) { TrialRoad_PushLog(18, 0, -1, 1); }

void TrialRoad_PushLog18South(void) { TrialRoad_PushLog(18, 0, 1, 1); }

void TrialRoad_PushLog18SouthFromRow52(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->renderMode == 2)
    Field_ClimbDown();
  else
    TrialRoad_PushLog(18, 0, 1, 1);
}

void TrialRoad_UncoverSpout(s32 id, s32 a, s32 frame) {
  if (frame == 30) {
    struct Actor *object = Actor_GetById(id);
    s32 height;

    object->x = object->timer << 16;
    object->z = (s16)object->unk66 << 16;
    height = Map_GetTerrainHeight(0, object->x, object->z);
    object->terrainHeight = height;
    object->y = height;
    TrialRoad_OpenSpoutCell(id);
    Event_SetFlag(id + 0x353);
  }
}

void TrialRoad_UncoverSpout16(s32 a, s32 b) {
  TrialRoad_UncoverSpout(16, a, b);
}

void TrialRoad_UncoverSpout17(s32 a, s32 b) {
  TrialRoad_UncoverSpout(17, a, b);
}

void TrialRoad_UncoverSpout18(s32 a, s32 b) {
  TrialRoad_UncoverSpout(18, a, b);
}

void TrialRoad_StandOnBlock(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *block = Actor_GetById(15);

  if (block->x >> 20 == leader->x >> 20 && block->z >> 20 == leader->z >> 20) {
    s32 y = leader->y;

    if (y < 0x200000)
      leader->y = 0x200000;
  }
}

void TrialRoad_StartSand(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 2;
}

void TrialRoad_EndSand(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 0;
}

void TrialRoad_RevealVine(void) {
  MapGrid_CopyCellAttributes(12, 21, 1, 1, 19, 21);
  Event_SetFlag(0x374);
}

#include "../field/object/set_sprite_priority.c"
