/// @file
/// Trial Road, the mountain path above the Shaman Village where the party races
/// Moapa to the summit: four stages dealt from eight puzzle rooms, a left and a
/// right path through each, the blue chests that open each stage's door for
/// the weight of the party's equipment, and the battle at the peak; after the
/// race, the summit's cave and pools and Gasp the Djinni.

#include "shared/trial_road.h"
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
#include "module/field/object.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "macros.h"
#include "system/vector3.h"
#include "event_work.h"
#include "module/window/window.h"
#include "map.h"
#include "system/field_effect.h"
#include "actor.h"
#include "system/frame.h"
#include "shared/map_cells.h"
#include "shared/effects.h"
#include "system/heap_release.h"
#include "item_icon.h"
#include "module/scene/scene.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "graphics/resource.h"
#include "system/serial.h"
#include "system/task.h"
#include "system/heap_slots.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field_sprite.h"
#include "field_event.h"
#include "graphics/vram_tab.h"

void TrialRoad_FightGasp(void) { Field_RequestDjinnBattle(8, 75); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gTrialRoad_Placements[];
extern struct ActorPlacement gTrialRoad_Room3Placements[];
extern struct ActorPlacement gTrialRoad_Room4Placements[];
extern struct ActorPlacement gTrialRoad_Room2Placements[];
extern s32 gTrialRoad_Room1Placements[];

s32 TrialRoad_SettleOnGround(struct Actor *object) {
  s32 height = Map_GetTerrainHeight(0, object->x, object->z) + 0x20000;

  object->terrainHeight = height;
  object->y = height;
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->scaleY = 0x18000;
  return 0;
}

const struct ActorPlacement *Scene_GetPlacements(void) {
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

void TrialRoad_TakeStairs(s32 direction) {
  struct TrialRoadState *state = gTrialRoadState;
  s16 *order = (s16 *)state;

  if ((u32)(direction - 1) <= 1) {
    if (state->stage[0] <= 0) {
      if (Event_GetFlag(2559)) {
        if (TrialRoad_OfferToGiveUp(0) == 0)
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

/// Trial Road's stage puzzles: the stairs, chests and doors, and the pillars,
/// torches, logs, spouts and sand the eight rooms are built from.

#include "text/msg_ids.h"

struct Actor;
struct FieldSprite;
union FieldObject;

struct EventWork184 {
  u8 unk000[388];
  s16 value;
};

/// A view of the map work's byte 23.
struct MapWorkByte23 {
  u8 unk00[0x17];
  u8 bits0 : 2;
  // XXX: what bits 2 and 3 select; the map code that reads them would settle
  // it.
  u8 bits2 : 2;
  u8 bits4 : 4;
};

/// The sprite's priority bits, read as the byte that holds them.
struct EffectSpriteBits {
  u8 unk0 : 2;
  u8 priority : 2;
} __attribute__((packed));

/// A sprite's attribute 2, as the byte bits view reaches it.
struct EffectSpriteAttributes {
  u8 unk00[9];
  struct EffectSpriteBits bits;
};

struct TrialRoadReplay {
  s16 mode;
  s16 mirrored;
  s16 id;
  s16 point;
  s16 still;
  s16 resource;
};

struct TrialRoadLinkWork {
  u8 unk00[0xe4];
  u16 rival;
  u16 unke6;
  u16 linkAngle;
  s16 resource;
  s32 linkX;
  s32 linkZ;
  s16 path[0x1c20];
};

/// One queued sprite: the three words Oam_AddSpriteAtDepth adds to OAM.
struct TrialRoadSprite {
  u32 words[3];
};

struct TrialRoadLink {
  u8 unk00[0xd8];
  s16 vramEntry;
  u16 unkda;
  u8 unkdc[4];
  u16 unke0;
  u16 a;
  u16 b;
  u8 unke6[6];
  s32 height;
  s32 unkf0;
};

struct TrialRoadControl {
  s16 state;
  s16 field02;
  s16 field04;
  s16 field06;
  s16 field08;
  s16 field0a;
};

s32 TrialRoad_IsMirrored(void);

void TrialRoad_BlockCellUnder(s32 id);
void TrialRoad_UnblockCellUnder(s32 id);
void TrialRoad_AddPillarTop(s32 id);
void TrialRoad_SetPillarTopLift(s32 id, s32 value);
void TrialRoad_OpenSpoutCell(s32 id);
void TrialRoad_TrackLeaderOnPillars(void);
void TrialRoad_CheckPoundedPillarsCovered(s32 a, s32 b);
void TrialRoad_PoundPillar(s32 id, s32 a, s32 b);
void TrialRoad_StepOnSeesawPillar(s32 id, s32 other, s32 flag, s32 side);
void TrialRoad_PushLog(s32 id, s32 dx, s32 dz, s32 axis);
void TrialRoad_UncoverSpout(s32 id, s32 a, s32 frame);
void TrialRoad_CheckCrumblingStep(const u16 *table);
void TrialRoad_CrumblePillar(const void *data, s32 a);

void TrialRoad_ClearGiveUpAsked(void);

void TrialRoad_BlowBushOffLonePillar(void);

void TrialRoad_JumpAcrossGap(void);

void TrialRoad_JumpMeltedPuddle(void);

void TrialRoad_StepOnPillar18(void);

void TrialRoad_StepOnPillar19(void);

void TrialRoad_StepOnPillar20(void);

void TrialRoad_StepOnPillar21(void);

void TrialRoad_LandOnPillar(void);

void TrialRoad_CheckCrumblingPillars(void);

void TrialRoad_PushLog16West(void);

void TrialRoad_PushLog16East(void);

void TrialRoad_PushLog17West(void);

void TrialRoad_PushLog17East(void);

void TrialRoad_PushLog18North(void);

void TrialRoad_PushLog18South(void);

void TrialRoad_PushLog18SouthFromRow52(void);

void TrialRoad_StartSand(void);

void TrialRoad_EndSand(void);

void TrialRoad_RevealVine(void);

void TrialRoad_DriftWithStream(void);

void TrialRoad_LiftCaveRock(void);

void TrialRoad_CutBushes(void);

void TrialRoad_CutMadPlantBushes(void);

void TrialRoad_RevealPoolTiles(void);

void TrialRoad_HidePoolTiles(void);

void TrialRoad_ChallengeMoapa(void);

extern struct ActorPlacement gTrialRoad_PillarStagePlacements[];
extern struct ActorPlacement gTrialRoad_TorchStagePlacements[];
extern struct ActorPlacement gTrialRoad_BushPillarStagePlacements[];
extern struct ActorPlacement gTrialRoad_PoundStagePlacements[];
extern struct ActorPlacement gTrialRoad_CrumblingStagePlacements[];
extern struct ActorPlacement gTrialRoad_LogStagePlacements[];
extern struct ActorPlacement gTrialRoad_FountainStagePlacements[];
extern struct ActorPlacement gTrialRoad_SandStagePlacements[];
extern struct Trigger gTrialRoad_PillarStageTriggers[];
extern struct Trigger gTrialRoad_TorchStageTriggers[];
extern struct Trigger gTrialRoad_BushPillarStageTriggers[];
extern struct Trigger gTrialRoad_PoundStageTriggers[];
extern struct Trigger gTrialRoad_CrumblingStageTriggers[];
extern struct Trigger gTrialRoad_LogStageTriggers[];
extern struct Trigger gTrialRoad_FountainStageTriggers[];
extern struct Trigger gTrialRoad_SandStageTriggers[];

void InventorySnapshot_Restore();

void TrialRoad_Idle(void);

s32 TrialRoad_RivalIsAhead(s32 fallback);

/// Inventory_ClearSlot through its far call: empties an inventory slot and
/// returns 2.
s32 Inventory_ClearSlot(s32 owner, s32 slot);

extern s16 gTrialRoad_CrumblingPillars[];

void TrialRoad_ReachPeakFirst(void);
void TrialRoad_ReachPeakSecond(void);

void TrialRoad_LowerPillar(s32 id);

void TrialRoad_RaisePillar(s32 id);

void TrialRoad_BlockLogs(void);

void TrialRoad_StartFountains(s32 a, s32 b);

void Tightrope_Initialize(void);
void Tightrope_Place(s32 a, s32 b, s32 c);

extern u32 gTrialRoad_GaspHopScript[];

extern u16 gTrialRoad_PaletteCycleColors[];

void PaletteCycle_StepAt(void);
void TrialRoad_OpenSummitGate();
void TrialRoad_PlaceMoapaAtPeak(void);
void TrialRoad_BeatMoapa(void);
void TrialRoad_LoseToMoapa(void);
void TrialRoad_ShowTraps(void);

void TrialRoad_SetUpCave(void);

void TrialRoad_SetUpPoolRoom(void);

extern u32 gTrialRoad_RivalRecordings[];
s32 TrialRoad_RivalIsWaiting(void);
void TrialRoad_PlayRivalRecording(s32 a);
void TrialRoad_DealStageOrder(u32 seed);

void Map_CopyRawCellsRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void TrialRoad_ShowProgress(s32 a, s32 b);

void TrialRoad_OpenLeaderDoor(s32 side, s32 phase);

void FieldCamera_WaitForArrival(void);

extern u32 gTrialRoad_LeftKnightEntersScript[];

extern u32 gTrialRoad_RightKnightEntersScript[];

extern u32 gTrialRoad_LeftKnightToChestScript[];
extern u32 gTrialRoad_RightKnightToChestsScript[];

extern u32 gTrialRoad_RightKnightLeavesScript[];

s32 Actor_GetCellIndex(struct Actor *object);
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
void Actor_DisableById(s32 id);
s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);
struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

void TrialRoad_SpawnRubble(s32 value);
void TrialRoad_SetLeaderCellShape(s32 offset, s32 layer);
void TrialRoad_SpawnDustRing(void);
void TrialRoad_SetActorCellShape(s32 a, s32 b, s32 c);

u32 Sprite_ShareVramCacheEntry(struct FieldSprite *sprite, u32 frame);

void TrialRoad_DriftRubble(union FieldObject *object);

void Actor_StartIdle(struct Actor *object);

extern const u16 gTrialRoad_ProgressPalette[];

void TrialRoad_MoveRival(void);
void TrialRoad_SetUpPillarStage(void);
void TrialRoad_SetUpTorchStage(void);
void TrialRoad_SetUpBushPillarStage(void);
void TrialRoad_SetUpPoundStage(void);
void TrialRoad_SetUpCrumblingStage(void);
void TrialRoad_SetUpLogStage(void);
void TrialRoad_SetUpFountainStage(void);

void TrialRoad_SetUpSandStage(void);

void TrialRoad_UpdateFountains(void);

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
 * the object has, it does not compile in tla-ja:
 * games/tla/src/scene/trial_road.c:559: structure has no member named `value'.
 */
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

extern s32 gTrialRoad_PillarTopScript[];

void TrialRoad_AddPillarTop(s32 id) {
  struct Actor *owner = Actor_GetById(id);
  struct Actor *object;

  if (owner != NULL) {
    object = FieldActor_Create(489, owner->x, owner->y, owner->z);
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

/* VIEW(s0218): routine gTrialRoad_PoundPartners as this part declared it; the
 * joined file has another; tried as the definition's extern u32
 * gTrialRoad_PoundPartners[], it does not compile in tla-ja:
 * games/tla/src/scene/trial_road.c:783: `gTrialRoad_PoundPartners' undeclared
 * (first use in this function). */
extern const s8
    gTrialRoad_PoundPartners__s0218[4] __asm__("gTrialRoad_PoundPartners");
#define gTrialRoad_PoundPartners gTrialRoad_PoundPartners__s0218

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

#include "../field/field_effect/move.c"

#undef DEFINE_TRIAL_ROAD_OPEN_DOOR
#undef FIELD_AT
#undef DEFINE_TRIAL_ROAD_ON_FOOT
#undef gTrialRoad_PoundPartners
// @file
// Trial Road's effect spawner and the sand stage's stream.

extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SelectAnimation(struct Actor *object, s32 a);
void Actor_SetSpriteFlags(struct Actor *object, u32 value);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect = ((struct FieldEffect *)Import_ActorCreate(285, x, y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - 0x10000) / frames;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#include "../field/field_effect/advance_motion_priority.c"

void TrialRoad_DriftWithStream(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  s32 velocity[3];

  if (gMainRoutineWork.renderMode == 4)
    return;
  leader->z += 0x10000;
  if ((gFrameCount & 7) != 0)
    return;
  if (leader->velocityZ == 0 && leader->velocityX == 0)
    return;
  options.update = Effect_AdvanceMotionAtLeaderPriority;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  velocity[1] = 0;
  velocity[2] = 0x10000;
  velocity[0] = 0;
  velocity[0] += ((Random16() << 17) >> 16) - 0x10000;
  velocity[2] += ((Random16() << 16) >> 16) - 0x8000;
  FieldEffect_Spawn(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                    velocity[2], 0x1000001, &options);
}

// Trial Road's rooms set up, the rival Moapa's race, the peak, the
// fountains and crumbling pillars, the race's progress display and the tables.

void TrialRoad_LiftCaveRock(void) { GameFlag_Set(0x289); }

void TrialRoad_CutBushes(void) { Func_02005a50_(68, 6, 1, 21, 13, 64, 64); }

void TrialRoad_CutMadPlantBushes(void) {
  Func_02005a50_(68, 6, 1, 21, 13, 64, 64);
  if (!Event_GetFlag(0x9b9)) {
    Func_02005a60(249, 72, 152);
    ((struct MainRoutineWork254 *)&gMainRoutineWork)->value = 0x19b9;
    gMainRoutineWork.battleRule24b = 2;
    Field_RequestBattleFromEncounterTable(106, 4);
  }
}

s32 TrialRoad_ReturnTrue(void) { return 1; }

#define DEFINE_TRIAL_ROAD_SET_POOL_TILES(name, n1, n2)                         \
  void name(void) {                                                            \
    MapGrid_CopyCells(n1, 30, 1, 1, 10, 11);                                   \
    Map_CopyCellsRect(n1, 30, 1, 1, 18, 7);                                    \
    Map_CopyCellsRect(n1, 30, 1, 1, 18, 15);                                   \
    Map_CopyCellsRect(n2, 30, 1, 1, 74, 11);                                   \
    Map_CopyCellsRect(n2, 30, 1, 1, 82, 7);                                    \
    Map_CopyCellsRect(n2, 30, 1, 1, 82, 15);                                   \
  }

DEFINE_TRIAL_ROAD_SET_POOL_TILES(TrialRoad_RevealPoolTiles, 33, 35)

DEFINE_TRIAL_ROAD_SET_POOL_TILES(TrialRoad_HidePoolTiles, 34, 36)

void TrialRoad_OpenSummitGate(s32 sound) {
  if (Event_GetFlag(0x9ff)) {
    if (gTrialRoadState->mirrored == 0) {
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 13, 79);
      MapGrid_CopyCellAttributes(78, 18, 1, 1, 77, 18);
    } else {
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 18, 79);
      MapGrid_CopyCellAttributes(81, 18, 1, 1, 82, 18);
    }
    if (sound)
      Event_PlaySound(138);
  }
  Event_SetFlag(0x283);
}

void TrialRoad_ChallengeMoapa(void) {
  struct MainRoutineWork *party;
  s32 count = Event_GetFlag(0x381);

  count += Event_GetFlag(0x382);
  count += Event_GetFlag(0x383);
  count += Event_GetFlag(0x384);
  if (Event_GetFlag(0x385))
    TrialRoad_ReachPeakFirst();
  else
    TrialRoad_ReachPeakSecond();
  party = &gMainRoutineWork;
  party->battleRule24b = 2;
  do {
  } while (0);
  Event_SetAfterBattleDestination((s32)&SceneId_Ov697Room2, 4);
  Party_SetFields1f2And1f4((s32)&SceneId_Ov697Room2, 5);
  Field_RequestBattleFromEncounterTable(13, 4 - count);
}

// When the object stands in column 8, sets flag 0x9fc, stops its motion and
// lifts it one pixel.
void TrialRoad_SetSummitPillar(s32 a, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object->x >> 20 == 8) {
    Event_SetFlag(0x9fc);
    object->motionFlags = 0;
    object->y -= 0x10000;
  }
}

extern struct Trigger gTrialRoad_Triggers[];
extern struct Trigger gTrialRoad_Room3Triggers[];
extern struct Trigger gTrialRoad_Room4Triggers[];
extern struct Trigger gTrialRoad_Room2Triggers[];
extern s32 gTrialRoad_Room1Triggers[];

const struct Trigger *Scene_GetEvents(void) {
  s32 pattern = gTrialRoadState->pattern;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room1)
    return gTrialRoad_Room1Triggers[pattern & 7];
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room3)
    return gTrialRoad_Room3Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room4)
    return gTrialRoad_Room4Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room2)
    return gTrialRoad_Room2Triggers;
  return gTrialRoad_Triggers;
}

void TrialRoad_SetUpPillarStage(void) {
  if (!Event_GetFlag(0x307))
    OBJECT_BYTE(Actor_GetById(17), 98) = 1;
  else
    Actor_SetPosition(20, 0, 0);
  TrialRoad_AddPillarTop(15);
  TrialRoad_AddPillarTop(16);
  TrialRoad_AddPillarTop(17);
  TrialRoad_AddPillarTop(18);
  TrialRoad_AddPillarTop(19);
  TrialRoad_BlockCellUnder(16);
  TrialRoad_BlockCellUnder(17);
  TrialRoad_BlockCellUnder(18);
  TrialRoad_BlockCellUnder(19);
  Task_Add(TrialRoad_TrackLeaderOnPillars, 0x480);
}

void TrialRoad_SetUpTorchStage(void) {
  TrialRoad_AddPillarTop(15);
  TrialRoad_AddPillarTop(16);
  TrialRoad_AddPillarTop(17);
  TrialRoad_AddPillarTop(18);
  TrialRoad_AddPillarTop(19);
  TrialRoad_AddPillarTop(20);
  TrialRoad_BlockCellUnder(15);
  TrialRoad_BlockCellUnder(16);
  TrialRoad_BlockCellUnder(17);
  TrialRoad_BlockCellUnder(18);
  TrialRoad_BlockCellUnder(19);
  TrialRoad_BlockCellUnder(20);
  OBJECT_BYTE(Actor_GetById(17), 98) = 1;
  OBJECT_BYTE(Actor_GetById(18), 98) = 1;
  OBJECT_BYTE(Actor_GetById(19), 98) = 1;
  OBJECT_BYTE(Actor_GetById(20), 98) = 1;
  if (!Event_GetFlag(0x214))
    Actor_SetAnimation(17, 5);
  else
    TrialRoad_SetPillarTopLift(17, -16);
  if (!Event_GetFlag(0x215))
    Actor_SetAnimation(18, 5);
  else
    TrialRoad_SetPillarTopLift(18, -16);
  if (!Event_GetFlag(0x216))
    Actor_SetAnimation(19, 5);
  else
    TrialRoad_SetPillarTopLift(19, -16);
  if (!Event_GetFlag(0x217))
    Actor_SetAnimation(20, 5);
  else
    TrialRoad_SetPillarTopLift(20, -16);
}

void TrialRoad_SetUpBushPillarStage(void) {
  if (!Event_GetFlag(0x327))
    OBJECT_BYTE(Actor_GetById(15), 98) = 1;
  else
    Actor_SetPosition(20, 0, 0);
  if (!Event_GetFlag(0x328))
    OBJECT_BYTE(Actor_GetById(16), 98) = 1;
  else
    Actor_SetPosition(21, 0, 0);
  TrialRoad_AddPillarTop(15);
  TrialRoad_AddPillarTop(16);
  TrialRoad_AddPillarTop(17);
  TrialRoad_AddPillarTop(18);
  TrialRoad_AddPillarTop(19);
  TrialRoad_BlockCellUnder(15);
  TrialRoad_BlockCellUnder(16);
  TrialRoad_BlockCellUnder(17);
  TrialRoad_BlockCellUnder(18);
  TrialRoad_BlockCellUnder(19);
}

void TrialRoad_SetUpPoundStage(void) {
  OBJECT_BYTE(Actor_GetById(18), 98) = 1;
  OBJECT_BYTE(Actor_GetById(19), 98) = 1;
  OBJECT_BYTE(Actor_GetById(20), 98) = 1;
  OBJECT_BYTE(Actor_GetById(21), 98) = 1;
  if (!Event_GetFlag(0x339))
    OBJECT_BYTE(Actor_GetById(16), 98) = 1;
  else
    Actor_SetPosition(22, 0, 0);
  TrialRoad_AddPillarTop(15);
  TrialRoad_AddPillarTop(16);
  TrialRoad_AddPillarTop(17);
  TrialRoad_AddPillarTop(18);
  TrialRoad_AddPillarTop(19);
  TrialRoad_AddPillarTop(20);
  TrialRoad_AddPillarTop(21);
  TrialRoad_BlockCellUnder(15);
  TrialRoad_BlockCellUnder(16);
  TrialRoad_BlockCellUnder(17);
  if (!Event_GetFlag(0x335)) {
    TrialRoad_RaisePillar(18);
    TrialRoad_LowerPillar(19);
  } else {
    TrialRoad_RaisePillar(19);
    TrialRoad_LowerPillar(18);
  }
  if (!Event_GetFlag(0x337)) {
    TrialRoad_RaisePillar(20);
    TrialRoad_LowerPillar(21);
  } else {
    TrialRoad_RaisePillar(21);
    TrialRoad_LowerPillar(20);
  }
}

void TrialRoad_SetUpCrumblingStage(void) {
  TrialRoad_RestoreCrumblingPillars(gTrialRoad_CrumblingPillars);
}

// Copies two blocks of map cells, sets byte 98 of objects 16 to 18 and clears
// their collision flags; then runs TrialRoad_BlockLogs, and
// TrialRoad_AddPillarTop and TrialRoad_BlockCellUnder for object 15.
void TrialRoad_SetUpLogStage(void) {
  MapGrid_CopyCellAttributes(11, 22, 11, 8, 11, 64);
  MapGrid_CopyCellAttributes(79, 22, 11, 8, 79, 64);
  OBJECT_BYTE(Actor_GetById(16), 98) = 1;
  OBJECT_BYTE(Actor_GetById(17), 98) = 1;
  OBJECT_BYTE(Actor_GetById(18), 98) = 1;
  {
    /* FAKEMATCH(pin): the zero pinned to r6; as a plain local, which reuses r5
     * where the 1 was held, 12 of 52 instructions differ in tla-en. */
    register s32 clear asm("r6") = 0;

    Actor_GetById(16)->collisionFlags = clear;
    Actor_GetById(17)->collisionFlags = clear;
    Actor_GetById(18)->collisionFlags = clear;
  }
  TrialRoad_BlockLogs();
  TrialRoad_AddPillarTop(15);
  TrialRoad_BlockCellUnder(15);
}

// One of the fountain stage's fountains and the spout drawn over it.
struct TrialRoadFountain {
  s16 id;     ///< its actor; 0 for an empty slot
  s16 phase;  ///< frames into its period
  s16 height; ///< how far it has risen, in pixels
  s16 period; ///< frames in one rise and fall
  s16 riseAt; ///< the phase it starts to rise at
  s16 fallAt; ///< the phase it starts to fall at
  s16 high;
  s16 low;
  s16 unk10;
  s16 speed; ///< pixels a frame, 4 rising and -8 falling
  s16 unk14;
  s16 bob;             ///< the spout's sine phase
  struct Actor *spout; ///< drawn over it
};

// The fountain stage's work, kept in the inventory snapshot's spare room.
struct TrialRoadFountains {
  s16 vramEntry;
  s16 shape; ///< the first of the shape grid's values for the fountains
  struct TrialRoadFountain fountains[16];
  u32 *sprites;
};

struct TrialRoadFountain *TrialRoad_AddFountain(s32 slot, s32 id, s32 timer,
                                                s32 low, s32 high);

// Sets up the Trial Road room with five movers: objects 16 to 20 in slots 0
// to 4, the first three over a range of 64 and the last two over 32, each
// with 240, 120 and 150; objects 16 to 18 are put away
// (TrialRoad_OpenSpoutCell) once flags 0x363 to 0x365 are set and placed
// otherwise; then object 15.
void TrialRoad_SetUpFountainStage(void) {
  struct TrialRoadFountain *fountain;

  TrialRoad_StartFountains(0, 240);
  fountain = TrialRoad_AddFountain(0, 16, 0, 0, 64);
  fountain->period = 240;
  fountain->riseAt = 120;
  fountain->fallAt = 150;
  fountain = TrialRoad_AddFountain(1, 17, 0, 0, 64);
  fountain->period = 240;
  fountain->riseAt = 120;
  fountain->fallAt = 150;
  fountain = TrialRoad_AddFountain(2, 18, 0, 0, 64);
  fountain->period = 240;
  fountain->riseAt = 120;
  fountain->fallAt = 150;
  fountain = TrialRoad_AddFountain(3, 19, 0, 0, 32);
  fountain->period = 240;
  fountain->riseAt = 120;
  fountain->fallAt = 150;
  fountain = TrialRoad_AddFountain(4, 20, 0, 0, 32);
  fountain->period = 240;
  fountain->riseAt = 120;
  fountain->fallAt = 150;
  if (!Event_GetFlag(0x363))
    Actor_SetPosition(16, 0, 0);
  else
    TrialRoad_OpenSpoutCell(16);
  if (!Event_GetFlag(0x364))
    Actor_SetPosition(17, 0, 0);
  else
    TrialRoad_OpenSpoutCell(17);
  if (!Event_GetFlag(0x365))
    Actor_SetPosition(18, 0, 0);
  else
    TrialRoad_OpenSpoutCell(18);
  ((struct MapWorkByte23 *)Ram_HeapSlots->mapWork)->bits2 = 1;
  TrialRoad_AddPillarTop(15);
  TrialRoad_BlockCellUnder(15);
}

void TrialRoad_SetUpSandStage(void) {
  Tightrope_Initialize();
  Tightrope_Place(0, 15, 16);
  if (!Event_GetFlag(0x374))
    MapGrid_CopyCellAttributes(18, 21, 1, 1, 19, 21);
  else
    Actor_SetPosition(17, 0, 0);
}

void TrialRoad_SetUpCave(void) {}

void TrialRoad_GaspFlees(void) {
  struct Actor *partner = Actor_GetById(8);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 dx;
  s32 dz;
  s32 near;
  s32 far;
  s32 y;

  if (Event_GetFlag(648))
    return;
  dx = (leader->x - partner->x) >> 16;
  dz = (leader->z - partner->z) >> 16;
  near = dx * dx + dz * dz;
  dx = (leader->x >> 16) - 296;
  dz = (leader->z >> 16) - 248;
  far = dx * dx + dz * dz;
  if (far <= 6400 || near <= 2304)
    Actor_SetMoveTarget(partner, (296 - dx) << 16, partner->y,
                        (248 - dz) << 16);
  if (far > 15)
    return;
  Event_PlaySound(146);
  Event_PlaySound(122);
  partner->speedLimit = 0x20000;
  partner->acceleration = 0x20000;
  partner->motionFlags = 2;
  partner->terrainHeight = 0;
  partner->collisionFlags = 1;
  partner->velocityY = 0xa0000;
  Actor_SetScript(8, gTrialRoad_GaspHopScript);
  y = partner->y;
  Actor_SetMoveTarget(partner, 344 << 16, y, 264 << 16);
  Event_SetFlag(648);
}

/* VIEW(s15a8): routine gTrialRoad_PaletteCycleRunning as this part declared it;
 * the joined file has another; tried as the definition's extern LAST_DATA u32
 * gTrialRoad_PaletteCycleRunning[], it does not compile in tla-ja:
 * games/tla/src/scene/trial_road.c:1651: `gTrialRoad_PaletteCycleRunning'
 * undeclared (first use in this function). */
extern s32 gTrialRoad_PaletteCycleRunning__s15a8 __asm__(
    "gTrialRoad_PaletteCycleRunning");
#define gTrialRoad_PaletteCycleRunning gTrialRoad_PaletteCycleRunning__s15a8

// Unless flag 123 is set, readies object 8: before flag 0x288 it raises it
// 0x88000 with motion 5, after it grounds it with motion 3; then schedules
// TrialRoad_GaspFlees.
void TrialRoad_SetUpPoolRoom(void) {
  if (!Event_GetFlag(123)) {
    struct Actor *object = Actor_GetById(8);
    s32 started = Event_GetFlag(0x288);
    u8 *flags = &object->motionFlags;

    if (!started) {
      *flags = 5;
      object->y += 0x88000;
    } else {
      *flags = 3;
      object->y = 0;
    }
    Task_Add(TrialRoad_GaspFlees, 0x480);
  }
}

void TrialRoad_SetPaletteCycleRunning(s32 value) {
  gTrialRoad_PaletteCycleRunning = value;
}

/* VIEW(s15a8): routine gTrialRoad_PaletteCycleFrame as this part declared it;
 * the joined file has another; tried as the definition's extern LAST_DATA s32
 * gTrialRoad_PaletteCycleFrame[], it does not compile in tla-ja:
 * games/tla/src/scene/trial_road.c:1661: `gTrialRoad_PaletteCycleFrame'
 * undeclared (first use in this function). */
extern s32
    gTrialRoad_PaletteCycleFrame__s15a8 __asm__("gTrialRoad_PaletteCycleFrame");
#define gTrialRoad_PaletteCycleFrame gTrialRoad_PaletteCycleFrame__s15a8

DEFINE_PALETTE_CYCLE_STEP_AT(PaletteCycle_StepAt, gTrialRoad_PaletteCycleFrame,
                             gTrialRoad_PaletteCycleRunning,
                             gTrialRoad_PaletteCycleColors, 0x050000a4)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void TrialRoad_StartPaletteCycle(s32 mode) {
  gTrialRoad_PaletteCycleRunning = 1;
  if (mode != 2)
    Iwram_CopyWords(gTrialRoad_PaletteCycleColors, (void *)&BG_PLTT_COLOR(5, 0),
                    32);
  if (mode == 1) {
    s32 zero = 0;

    gTrialRoad_PaletteCycleFrame = zero;
    Task_Add(PaletteCycle_StepAt, 0x480);
  }
}

#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void TrialRoad_LeaveMoapaWaiting(void);

// The mountain's peak: after the battle, the victory or the defeat; on
// arrival, Moapa and his knights wait.
void TrialRoad_SetUpPeak(void) {
  s32 entrance = gMainRoutineWork.entrance;

  if (entrance == 4) {
    InventorySnapshot_Restore();
    TrialRoad_BeatMoapa();
    Import_EventSetExitRequest(4);
  } else if (entrance == 5) {
    InventorySnapshot_Restore();
    TrialRoad_LoseToMoapa();
    Import_EventSetExitRequest(5);
  } else if (Event_GetFlag(0x9ff)) {
    if (Event_GetFlag(0x283))
      TrialRoad_OpenSummitGate(0);
    if (Event_GetFlag(0x385))
      TrialRoad_LeaveMoapaWaiting();
    else
      TrialRoad_PlaceMoapaAtPeak();
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
  TrialRoad_StartPaletteCycle(1);
  TrialRoad_AddPillarTop(10);
  if (Event_GetFlag(0x9fc))
    Actor_SetPosition(10, PIXELS(136), PIXELS(328));
}

/* VIEW(s1fc4): variable gTrialRoad_StageSetUps as extern s32 (*const
 * gTrialRoad_StageSetUps[])(void); the definition has void
 * (*gTrialRoad_StageSetUps[])(void); called through the void type, the call's
 * result register moves 2 of 84 instructions of Scene_Initialize in tla-en;
 * tried as the rest of the object declares it, it does not compile in tla-ja:
 * games/tla/src/scene/trial_road.c:1734: `gTrialRoad_StageSetUps' undeclared
 * (first use in this function). */
extern s32 (*const gTrialRoad_StageSetUps__s1fc4[])(void) __asm__(
    "gTrialRoad_StageSetUps");

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  s32 pattern = gTrialRoadState->pattern;

  if (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 344) != 0)
    return 0;
  if (scene == (s32)&SceneId_Ov697Room1) {
    if (entrance <= 7) {
      Event_SetFlag(324);
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
      gTrialRoad_StageSetUps__s1fc4[pattern & 7]();
    } else if (entrance == 55) {
      TrialRoad_ShowTraps();
    }
  }
  if (scene == (s32)&SceneId_Ov697Room3)
    TrialRoad_SetUpCave();
  if (scene == (s32)&SceneId_Ov697Room4)
    TrialRoad_SetUpPoolRoom();
  if (scene == (s32)&SceneId_Ov697Room2) {
    Event_SetFlag(324);
    TrialRoad_SetUpPeak();
  }
  return 0;
}

void TrialRoad_CopyCellsBackward(s32 srcX, s32 srcY, s32 width, s32 height,
                                 s32 dstX, s32 dstY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  s32 dst = (dstY << 7) + dstX;
  u8 *dstShapes = gMapShapeGrid + dst;
  s32 src = (srcY << 7) + srcX;
  u8 *srcShapes = gMapShapeGrid + src;
  u32 *dstCells = cells + dst;
  s16 row;
  s16 col;

  cells += src;
  for (row = height - 1; row >= 0; row--) {
    u32 *dstCell = dstCells + ((row << 7) + width) - 1;
    u32 *srcCell = cells + ((row << 7) + width) - 1;
    u8 *dstShape = dstShapes + (row << 7) + width - 1;
    u8 *srcShape = srcShapes + (row << 7) + width - 1;

    for (col = width - 1; col >= 0; col--) {
      *dstCell-- = *srcCell--;
      *dstShape = *srcShape;
      dstShape--;
      srcShape--;
    }
  }
}

void TrialRoad_OpenRivalDoor(s32 mirrored, s32 stage);

void TrialRoad_UpdateRival(void) {
  struct TrialRoadState *state = gTrialRoadState;
  struct TrialRoadState *road = gTrialRoadState;
  s32 side;

  if (state->stage[1] > 3)
    return;
  if (TrialRoad_RivalIsWaiting() != 0) {
    if (state->rivalTimer == 0) {
      side = TrialRoad_RivalIsAhead(1) ^ 1;
      TrialRoad_PlayRivalRecording(gTrialRoad_RivalRecordings[side + 8]);
      state->rivalTimer = side * 5 * 120 + 1;
    } else {
      if (state->stage[1] >= state->rivalBest)
        state->rivalBest = state->stage[1] + 1;
      if (++road->stage[1] <= 3) {
        s32 pattern =
            state->order[state->stage[1] * 2 + (s16)(state->mirrored ^ 1)];

        TrialRoad_PlayRivalRecording(gTrialRoad_RivalRecordings[pattern & 7]);
        Actor_GetById(8)->terrainHeight = 0;
        Actor_GetById(8)->y = 0;
      } else {
        Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
      }
      state->rivalTimer = 0;
    }
  }
  {
    u8 *kind = &Actor_GetById(8)->animationKind;

    if (state->stage[0] == state->stage[1])
      *kind = 1;
    else
      *kind = 0;
  }
  if (state->rivalTimer == 0)
    return;
  if (GameFlag_Get(260))
    return;
  if (state->stage[0] == state->stage[1]) {
    switch (state->rivalTimer) {
    case 1:
    case 601:
      Sound_Play(124);
      if (state->mirrored != 0) {
        Actor_SetModeById(9, 6);
        if (state->rivalTimer > 600)
          Actor_SetModeById(10, 6);
      } else {
        Actor_SetModeById(11, 6);
        if (state->rivalTimer > 600)
          Actor_SetModeById(12, 6);
      }
      break;
    case 655:
      Sound_Play(125);
      if (state->mirrored != 0)
        Actor_SetModeById(10, 5);
      else
        Actor_SetModeById(12, 5);
      break;
    case 653:
    case 708:
      Actor_GetById(8)->facing = 0xc000;
      break;
    case 50:
    case 710:
      Sound_Play(125);
      if (state->mirrored != 0)
        Actor_SetModeById(9, 5);
      else
        Actor_SetModeById(11, 5);
      break;
    case 70:
    case 730:
      TrialRoad_OpenRivalDoor(state->mirrored, state->stage[1]);
      Sound_Play(138);
      break;
    }
  }
  road->rivalTimer++;
}

extern s16 gTrialRoad_StageMapCells[][2];
extern s8 gTrialRoad_DemoSceneryStages[];
extern s8 gTrialRoad_SceneryStages[];

void *Unit_Get(s32 owner);
void TrialRoad_StartRival(s32 a);

// Places `layer` `column` cells into the map: its offset (column << 20), its
// cells and terrain from that column on, and its position from the view. The
// offset takes the literal, the cells and terrain the column's variable:
// with the variable in the offset too, CSE keeps a shift of the register;
// with the literal in the cells, the buffer and offset fold into one literal;
// through gMapCellBuffer's name, the address stays in a register across calls.
#define SET_LAYER(column, x)                                                   \
  words = (s32 *)layer;                                                        \
  words[2] = (column) << 20;                                                   \
  words[3] = row;                                                              \
  x = (column);                                                                \
  layer->cells = (struct MapCell *)Ram_MapCellBuffer + x;                      \
  layer->terrainIds = gMapShapeGrid + x;                                       \
  words[0] = Iwram_MulQ16(*viewX, words[4]) + words[2];                        \
  words[1] = Iwram_MulQ16(*viewZ, words[5]) + words[3]

s32 Scene_PrepareMap(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;

  if (scene == (s32)&SceneId_Ov697Room1) {
    struct MapWork *map = Ram_HeapSlots->mapWork;
    struct TrialRoadState *state = gTrialRoadState;
    struct Actor *leader;
    u8 *unit;
    s32 value;
    s32 n;
    s32 px;
    u32 i;

    if ((u32)(entrance - 31) <= 1) {
      if (!GameFlag_Get(896)) {
        unit = Unit_Get(gMainRoutineWork.currentOwner);
        Iwram_ClearWordsEntry(state, 32);
        value = 0;
        for (n = 0; n <= 13; n++)
          value += unit[n];
        TrialRoad_DealStageOrder(value);
        GameFlag_Set(896);
      }
      state->mirrored = (entrance - 1) & 1;
      state->stage[0] = 0;
      state->stage[1] = 0;
      state->leaderBest = 0;
      state->rivalBest = 0;
      value = state->order[state->mirrored ^ 1];
      TrialRoad_PlayRivalRecording((s32)gTrialRoad_RivalRecordings[value & 7]);
      state->pattern = state->order[state->stage[0] * 2 + state->mirrored];
      n = entrance % 10;
      Event_SetDestination((s32)&SceneId_Ov697Room1, n);
    } else if ((u32)(entrance - 33) <= 1) {
      if (!GameFlag_Get(896)) {
        unit = Unit_Get(gMainRoutineWork.currentOwner);
        Iwram_ClearWordsEntry(state, 32);
        value = 0;
        for (n = 0; n <= 13; n++)
          value += unit[n];
        TrialRoad_DealStageOrder(value);
        GameFlag_Set(896);
      }
      {
        /* FAKEMATCH(temporary): side, for both the store and its flip; as
         * state->mirrored, 2 of 879 instructions differ in tla-en. */
        s16 side = (entrance - 1) & 1;

        state->mirrored = side;
        state->stage[0] = 3;
        state->stage[1] = 3;
        state->leaderBest = 4;
        state->rivalBest = 4;
        side ^= 1;
        {
          /* FAKEMATCH(temporary): index; as state->order[side + 6], 2 of 879
           * instructions differ in tla-en. */
          s32 index = side + 6;

          value = state->order[index];
        }
      }
      TrialRoad_PlayRivalRecording((s32)gTrialRoad_RivalRecordings[value & 7]);
      state->pattern = state->order[state->stage[0] * 2 + state->mirrored];
      n = entrance % 10;
      Event_SetDestination((s32)&SceneId_Ov697Room1, n);
    } else if (entrance == 50) {
      unit = Unit_Get(gMainRoutineWork.currentOwner);
      Iwram_ClearWordsEntry(state, 32);
      value = 0;
      for (n = 0; n <= 13; n++)
        value += unit[n];
      TrialRoad_DealStageOrder(value);
      state->mirrored = 0;
      state->stage[0] = 0;
      state->stage[1] = 0;
      state->leaderBest = 0;
      state->rivalBest = 0;
      state->pattern = state->order[0];
      Event_SetDestination((s32)&SceneId_Ov697Room1, 55);
    } else {
      s32 *viewX = &map->viewX;
      s32 *viewZ = &map->viewZ;
      struct MapLayer *layer;
      s32 *words;
      s32 row;

      if (entrance == 55) {
        s32 x1;
        s32 x2;

        row = 0;
        layer = &map->layers[1];
        SET_LAYER(34, x1);
        layer = &map->layers[2];
        SET_LAYER(68, x2);
        Actor_SetSpriteColor(Actor_GetById(9), 3);
        Actor_SetSpriteColor(Actor_GetById(10), 3);
        Actor_SetSpriteColor(Actor_GetById(11), 3);
        Actor_SetSpriteColor(Actor_GetById(12), 3);
        Map_CopyRawCellsRect(0, 12, 1, 21, 0, 13);
        Map_CopyRawCellsRect(x1, 12, 1, 21, x1, 13);
        Map_CopyRawCellsRect(x2, 12, 1, 21, x2, 13);
        Map_CopyRawCellsRect(0, 13, 33, 21, 1, 13);
        Map_CopyRawCellsRect(x1, 13, 33, 21, x1 + 1, 13);
        Map_CopyRawCellsRect(x2, 13, 33, 21, x2 + 1, 13);
        value = state->pattern;
        px = gTrialRoad_StageMapCells[value][0];
        row = gTrialRoad_StageMapCells[value][1];
        Map_CopyRawCellsRect(px, row, 13, 21, 1, 13);
        Map_CopyRawCellsRect(px + 42, row, 13, 21, x1 + 1, 13);
        Map_CopyRawCellsRect(px + 84, row, 13, 21, x2 + 1, 13);
        // The native neither hoists the table's address nor strength-reduces
        // the index; as a for loop, 10 more instructions differ (376).
        i = 0;
      hideUnused55:
        if (gTrialRoad_DemoSceneryStages[i] != value)
          Actor_SetHorizontalPositionWithTerrain(i + 64, 0, 0);
        i++;
        if (i <= 9)
          goto hideUnused55;
        Map_CenterOnFollowedPoint();
      } else {
        s32 x1;
        s32 x2;
        s32 destinationY;

        row = 0;
        layer = &map->layers[1];
        SET_LAYER(34, x1);
        layer = &map->layers[2];
        SET_LAYER(68, x2);
        value = 1;
        if (entrance & value) {
          TrialRoad_CopyCellsBackward(0, 0, 102, 13, 9, 0);
          Map_CopyRawCellsRect(0, 0, 10, 1, 1, 0);
          Map_CopyRawCellsRect(0, 0, 10, 12, 0, 1);
          Map_CopyRawCellsRect(x1, 0, 10, 12, x1, 1);
          Map_CopyRawCellsRect(x2, 0, 10, 12, x2, 1);
          TrialRoad_CopyCellsBackward(0, 34, 102, 10, 9, 34);
          Map_CopyRawCellsRect(0, 33, 10, 10, 0, 34);
          Map_CopyRawCellsRect(x1, 33, 10, 10, x1, 34);
          Map_CopyRawCellsRect(x2, 33, 10, 10, x2, 34);
        } else {
          Map_CopyRawCellsRect(10, 0, 92, 13, 0, 0);
          Map_CopyRawCellsRect(23, 0, 10, 1, 24, 0);
          Map_CopyRawCellsRect(23, 0, 10, 12, 23, 1);
          Map_CopyRawCellsRect(57, 0, 10, 12, 57, 1);
          Map_CopyRawCellsRect(91, 0, 10, 12, 91, 1);
          Map_CopyRawCellsRect(10, 34, 92, 10, 0, 34);
          Map_CopyRawCellsRect(24, 33, 10, 10, 24, 34);
          Map_CopyRawCellsRect(58, 33, 10, 10, 58, 34);
          Map_CopyRawCellsRect(92, 33, 10, 10, 92, 34);
          Actor_SetPosition(9, 160 << 14, 152 << 16);
          Actor_SetPosition(10, 192 << 13, 152 << 16);
          Actor_SetPosition(11, 184 << 16, 152 << 16);
          Actor_SetPosition(12, 200 << 16, 152 << 16);
        }
        Actor_SetSpriteColor(Actor_GetById(9), 3);
        Actor_SetSpriteColor(Actor_GetById(10), 3);
        Actor_SetSpriteColor(Actor_GetById(11), 3);
        Actor_SetSpriteColor(Actor_GetById(12), 3);
        destinationY = 13;
        value = state->pattern;
        px = gTrialRoad_StageMapCells[value][0];
        row = gTrialRoad_StageMapCells[value][1];
        Map_CopyRawCellsRect(px, row, 13, 21, 10, destinationY);
        Map_CopyRawCellsRect(px + 42, row, 13, 21, 44, destinationY);
        Map_CopyRawCellsRect(px + 84, row, 13, 21, 78, destinationY);
        // A goto loop for the same reason as the one above.
        i = 0;
      hideUnused:
        if (gTrialRoad_SceneryStages[i] != value)
          Actor_SetHorizontalPositionWithTerrain(i + 64, 0, 0);
        i++;
        if (i <= 9)
          goto hideUnused;
        Map_CenterOnFollowedPoint();
        leader = Actor_GetById(gMainRoutineWork.currentOwner);
        if (!GameFlag_Get(265))
          leader->y = leader->terrainHeight =
              Map_GetTerrainHeight(0, leader->x, leader->z);
        if (GameFlag_Get(2559)) {
          struct TrialRoadState *road;

          GameFlag_Set(324);
          // A pointer of its own keeps the state's address in r5 across the
          // calls; through gTrialRoadState each test reloads it (5 more).
          road = gTrialRoadState;
          // The call's argument reads through the other struct: through the
          // same one GCSE moves the load into both predecessors (11 more of
          // gap).
          if (road->stage[0] < road->leaderBest || GameFlag_Get(642)) {
            TrialRoad_OpenLeaderDoor(road->mirrored, road->stage[0]);
            GameFlag_Set(642);
            GameFlag_Set(644);
          }
          if (road->stage[0] < road->rivalBest)
            TrialRoad_OpenRivalDoor(road->mirrored, road->stage[0]);
          TrialRoad_ShowProgress(gMainRoutineWork.currentOwner, 8);
          TrialRoad_StartRival((s16)(state->mirrored ^ 1));
          Task_Add(TrialRoad_UpdateRival, 0x480);
        } else {
          TrialRoad_OpenLeaderDoor(state->mirrored, state->stage[0]);
          GameFlag_Set(642);
          GameFlag_Set(644);
          TrialRoad_OpenRivalDoor(state->mirrored, state->stage[0]);
        }
      }
    }
  } else if (scene == (s32)&SceneId_Ov697Room3) {
  } else if (scene == (s32)&SceneId_Ov697Room4) {
  } else if (scene == (s32)&SceneId_Ov697Room2 && GameFlag_Get(2559) &&
             !GameFlag_Get(901)) {
    if (gTrialRoadState->mirrored != 0)
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 13, 79);
    else
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 18, 79);
  }
  return 0;
}

// Deals the Trial Road's stage order from a seed: shuffles 0 to 7 into the
// eight slots at the start of the stage state, again until both the even and
// the odd slots hold at least one of the four values in gTrialRoad_KeyStages;
// the random state is restored after.

extern s8 gTrialRoad_KeyStages[];

// Deals the Trial Road's stage order from a seed: shuffles 0 to 7 into the
// eight slots at the start of the stage state, again until both the even and
// the odd slots hold at least one of the four values in gTrialRoad_KeyStages;
// the random state is restored after.
void TrialRoad_DealStageOrder(u32 seed) {
  s16 *order = (s16 *)gTrialRoadState;
  u32 saved = gRandomState;
  s32 counts[2];
  s32 i;
  s32 k;
  s32 offset;
  s8 value;

  gRandomState = seed;
  do {
    for (i = 0; i <= 7; i++)
      order[i] = -1;
    for (i = 0; i <= 7; i++) {
      do {
        offset = ((Random16() << 3) >> 16) * 2;
      } while (*(s16 *)((u8 *)order + offset) != -1);
      *(s16 *)((u8 *)order + offset) = i;
    }
    counts[1] = 0;
    counts[0] = 0;
    for (i = 0; i <= 7; i++) {
      for (k = 0; k <= 3; k++) {
        value = gTrialRoad_KeyStages[k];
        if (order[i] == value)
          counts[i & 1]++;
      }
    }
  } while (counts[0] == 0 || counts[1] == 0);
  gRandomState = saved;
}

s32 TrialRoad_GetStage(s32 side) {
  struct TrialRoadState *state;

  state = gTrialRoadState;
  return state->stage[side & 1];
}

s32 TrialRoad_IsMirrored(void) { return gTrialRoadState->mirrored; }

// A pixel x on the mirrored road, the right path's, when the leader took it.
s32 TrialRoad_MirrorX(s32 x) {
  if (TrialRoad_IsMirrored())
    return 512 - x;
  return x;
}

// A cell x, in pixels, on the mirrored road when the leader took it.
s32 TrialRoad_MirrorCellX(s32 x) {
  if (TrialRoad_IsMirrored())
    return (64 - x) << 3;
  return x << 3;
}

s32 TrialRoad_MirrorOffset(s32 x) {
  if (TrialRoad_IsMirrored())
    return -x << 3;
  return x << 3;
}

s32 TrialRoad_MirrorAngle(s32 angle) {
  if (TrialRoad_IsMirrored())
    return (0x8000 - angle) & 0xffff;
  return angle & 0xffff;
}

// Places an actor at a pixel position and facing, mirrored when the leader
// took the right path.
void TrialRoad_PlaceActor(s32 id, s32 x, s32 z, s32 angle) {
  Actor_SetPositionAndFacing(id, TrialRoad_MirrorX(x) << 16, z << 16,
                             (u16)TrialRoad_MirrorAngle(angle));
}

void TrialRoad_LeaveMoapaWaiting(void) {}

void TrialRoad_PlaceMoapaAtPeak(void) {
  TrialRoad_PlaceActor(8, 288, 264, 0x8000);
  TrialRoad_PlaceActor(13, 296, 248, 0x8000);
  TrialRoad_PlaceActor(14, 296, 280, 0x8000);
}

void Actor_MoveToObject(s32 actor, s32 value);

// ★ Named from the game: "You were quite swift! You may prove a challenge
// yet!" (MsgTrialRoadSwiftChallenge)
void TrialRoad_ReachPeakFirst(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(0x1000000, -1, 0x1080000, 1);
  Actor_MoveToObject(5, 4);
  Actor_MoveToObject(6, 4);
  Actor_MoveToObject(7, 4);
  Actor_SetSpeed(4, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_WalkTo(4, TrialRoad_MirrorCellX(28), 272);
  Actor_WalkTo(7, TrialRoad_MirrorCellX(28), 256);
  Actor_WalkTo(5, TrialRoad_MirrorCellX(27), 248);
  Actor_WalkTo(6, TrialRoad_MirrorCellX(27), 280);
  Actor_WaitForMoveById(4);
  Actor_FaceDirection(4, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(7);
  Actor_FaceDirection(7, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(5);
  Actor_FaceDirection(5, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(6);
  Actor_FaceDirection(6, TrialRoad_MirrorAngle(0), 0);
  FieldCamera_WaitForArrival();
  wait(20);
  Event_PlaySound(138);
  wait(20);
  TrialRoad_PlaceActor(8, 344, 296, 0xc000);
  object = Actor_GetById(8);
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->motionFlags = 0;
  Actor_SetMoveTarget(object, object->x, object->y + 0x580000, object->z);
  Actor_SelectAnimation(object, 2);
  FieldActor_WaitForMove(object);
  Actor_SelectAnimation(object, 1);
  object->motionFlags = 2;
  object->terrainHeight = 0x800000;
  object->velocityY = 0x40000;
  object->speedLimit = 0x30000;
  object->acceleration = 0x20000;
  Actor_SetMoveTarget(object, object->x, object->y, object->z - 0x100000);
  FieldActor_WaitForMove(object);
  wait(12);
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  FieldActor_SetMoveTarget(object, TrialRoad_MirrorCellX(41) << 16, object->y,
                           264 << 16);
  FieldActor_WaitForMove(object);
  wait(6);
  Event_PlaySound(152);
  object->velocityY = 0x60000;
  object->speedLimit = 0x30000;
  object->acceleration = 0x20000;
  Actor_SetMoveTarget(object, TrialRoad_MirrorCellX(37) << 16, object->y,
                      264 << 16);
  FieldActor_WaitForMove(object);
  wait(6);
  Actor_MoveToObject(13, 8);
  Actor_MoveToObject(14, 8);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  Actor_SetSpeed(13, 0x10000, 0x8000);
  Actor_SetSpeed(14, 0x10000, 0x8000);
  Actor_WalkTo(8, TrialRoad_MirrorCellX(36), 264);
  Actor_WalkTo(13, TrialRoad_MirrorCellX(37), 248);
  Actor_WalkTo(14, TrialRoad_MirrorCellX(37), 280);
  Actor_WaitForMoveById(8);
  Actor_FaceDirection(8, TrialRoad_MirrorAngle(0x8000), 0);
  Actor_WaitForMoveById(13);
  Actor_FaceDirection(13, TrialRoad_MirrorAngle(0x8000), 0);
  Actor_WaitForMoveById(14);
  Actor_FaceDirection(14, TrialRoad_MirrorAngle(0x8000), 0);
  Event_SetMessageId((s32)MsgTrialRoadSwiftChallenge);
  FieldActor_ShowEmote(8, 256, 10);
  Event_ShowMessage(8, 0);
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(8, 0);
  Actor_RunRepeatedMotion(8, 2);
  Event_ShowMessage(8, 0);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  Event_Wait(15);
  Actor_SetSpeed(4, 0x20000, 0x10000);
  Actor_SetSpeed(5, 0x1e666, 0xf333);
  Actor_SetSpeed(6, 0x1b333, 0xd999);
  Actor_SetSpeed(7, 0x1cccc, 0xe666);
  Actor_SetSpeed(8, 0x20000, 0x10000);
  Actor_SetSpeed(13, 0x1e666, 0xf333);
  Actor_SetSpeed(14, 0x1b333, 0xd999);
  Actor_WalkBy(4, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(8, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(5, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(13, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(7, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(14, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(6, TrialRoad_MirrorOffset(3), 0);
  wait(1);
  Event_Wait(6);
}

// ★ Named from the game: "You may have been slow, but you made it through
// Trial Road to the mountain's peak!" (MsgTrialRoadSlowArrival)
void TrialRoad_ReachPeakSecond(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(PIXELS(256), -1, PIXELS(264), 1);
  Actor_MoveToObject(5, 4);
  Actor_MoveToObject(6, 4);
  Actor_MoveToObject(7, 4);
  Actor_SetSpeed(4, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_WalkTo(4, TrialRoad_MirrorCellX(28), 272);
  Actor_WalkTo(7, TrialRoad_MirrorCellX(28), 256);
  Actor_WalkTo(5, TrialRoad_MirrorCellX(27), 248);
  Actor_WalkTo(6, TrialRoad_MirrorCellX(27), 280);
  Actor_WaitForMoveById(4);
  Actor_FaceDirection(4, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(7);
  Actor_FaceDirection(7, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(5);
  Actor_FaceDirection(5, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(6);
  Actor_FaceDirection(6, TrialRoad_MirrorAngle(0), 0);
  FieldCamera_WaitForArrival();
  Event_SetMessageId((s32)MsgTrialRoadSlowArrival);
  FieldActor_ShowEmote(8, 256, 10);
  Event_ShowMessage(8, 0);
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(8, 0);
  Actor_RunRepeatedMotion(8, 2);
  Event_ShowMessage(8, 0);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  Event_Wait(15);
  Actor_SetSpeed(4, 0x20000, 0x10000);
  Actor_SetSpeed(5, 0x1e666, 0xf333);
  Actor_SetSpeed(6, 0x1b333, 0xd999);
  Actor_SetSpeed(7, 0x1cccc, 0xe666);
  Actor_SetSpeed(8, 0x20000, 0x10000);
  Actor_SetSpeed(13, 0x1e666, 0xf333);
  Actor_SetSpeed(14, 0x1b333, 0xd999);
  Actor_WalkBy(4, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(8, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(5, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(13, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(7, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(14, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(6, TrialRoad_MirrorOffset(3), 0);
  wait(1);
  Event_Wait(6);
}

void PartyInventory_Discard(s32 cue);

// ★ Named from the game: "All right, all right. You've proven your strength,
// and you are heroes indeed." (MsgTrialRoadHeroesProven)
void TrialRoad_BeatMoapa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  TrialRoad_PlaceActor(4, 248, 272, 0);
  TrialRoad_PlaceActor(7, 224, 248, 0);
  TrialRoad_PlaceActor(5, 232, 264, 0);
  TrialRoad_PlaceActor(6, 224, 280, 0);
  TrialRoad_PlaceActor(9, 248, 256, 0);
  TrialRoad_PlaceActor(8, 272, 264, 0x4000);
  TrialRoad_PlaceActor(13, 288, 248, 0x4000);
  TrialRoad_PlaceActor(14, 288, 280, 0x4000);
  Actor_SetAnimation(8, 6);
  Actor_SetAnimation(13, 6);
  Actor_SetAnimation(14, 6);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Event_SetMessageId((s32)MsgTrialRoadHeroesProven);
  Actor_StartRepeatedMotion(13, 2);
  wait(4);
  Actor_StartRepeatedMotion(14, 2);
  wait(10);
  Actor_RunRepeatedMotion(8, 2);
  Event_ShowMessage(8, 0);
  Actor_SetAnimation(8, 7);
  Event_Wait(15);
  Actor_SetAnimation(13, 7);
  Event_Wait(10);
  Actor_SetAnimation(14, 7);
  Event_Wait(5);
  Actor_SetAnimation(8, 1);
  Event_Wait(15);
  Actor_FaceDirection(8, TrialRoad_MirrorAngle(0x8000), 0);
  Actor_SetAnimation(13, 1);
  Event_Wait(10);
  Actor_FaceDirection(13, TrialRoad_MirrorAngle(0x8000), 0);
  Actor_SetAnimation(14, 1);
  Event_Wait(5);
  Actor_FaceDirection(14, TrialRoad_MirrorAngle(0x8000), 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(8, 0);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  Actor_WalkTo(8, TrialRoad_MirrorCellX(32), 264);
  Actor_FaceEachOther(9, 7, 0);
  Actor_FaceEachOther(5, 6, 0);
  Actor_WaitForMoveById(8);
  Actor_FaceActor(9, 4, 0);
  Actor_FaceActor(7, 4, 0);
  Actor_FaceActor(6, 4, 0);
  Actor_FaceActor(5, 4, 0);
  Event_Wait(20);
  PartyInventory_Discard(65);
  Party_GiveItem(215, 0);
  Actor_FaceActor(4, 8, 0);
  OBJECT_BYTE(Actor_GetById(8), 0x5a) &= ~1;
  Actor_WalkToAndWait(8, TrialRoad_MirrorCellX(35), 264);
  Event_Wait(1);
  /* FAKEMATCH(signature): the byte typed as a whole-byte bit-field, as in
   * overlay 685; as a plain byte, which stores in the order of a store that
   * aliases what follows, 4 of 304 instructions differ in tla-en. */
  ((struct ActorSolid *)Actor_GetById(8))->solid |= 1;
  Event_Wait(20);
  FieldActor_ShowEmote(8, 261, 10);
  Event_ShowMessage(8, 0);
  Actor_FaceDirectionAndWait(8, TrialRoad_MirrorAngle(0));
  Actor_FaceActor(13, 8, 0);
  Actor_FaceActor(14, 8, 0);
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(8, 0);
  Actor_SetAnimation(13, 3);
  wait(3);
  Actor_SetAnimationAndWait(14, 3);
  Actor_FaceActor(4, 9, 0);
  Actor_FaceActor(7, 9, 0);
  Actor_FaceActor(5, 9, 0);
  Actor_FaceActor(6, 9, 0);
  Event_ShowMessage(9, 0);
  Actor_SetAnimation(4, 3);
  wait(2);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  wait(1);
  Actor_SetAnimation(5, 3);
  wait(30);
}

// ★ Named from the game: "You've lost. Now leave Shaman village."
// (MsgTrialRoadLeaveVillage)
void TrialRoad_LoseToMoapa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  TrialRoad_PlaceActor(8, 264, 264, 0x8000);
  TrialRoad_PlaceActor(13, 280, 248, 0x8000);
  TrialRoad_PlaceActor(14, 280, 280, 0x8000);
  TrialRoad_PlaceActor(4, 240, 276, 0);
  TrialRoad_PlaceActor(7, 216, 254, 0);
  TrialRoad_PlaceActor(5, 232, 244, 0);
  TrialRoad_PlaceActor(6, 224, 296, 0);
  Actor_SetAnimation(4, 38);
  Actor_SetAnimation(5, 19);
  Actor_SetAnimation(6, 19);
  Actor_SetAnimation(7, 19);
  FieldActor_SetSpriteRotationById(7, 0x4000);
  Actor_SetSpriteFlags(Actor_GetById(4), 0);
  Actor_SetSpriteFlags(Actor_GetById(5), 0);
  Actor_SetSpriteFlags(Actor_GetById(6), 0);
  Actor_SetSpriteFlags(Actor_GetById(7), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Event_SetMessageId((s32)MsgTrialRoadLeaveVillage);
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(8, 0);
  Actor_StartRepeatedMotion(4, 2);
  wait(1);
  Actor_StartRepeatedMotion(6, 2);
  wait(2);
  Actor_StartRepeatedMotion(5, 2);
  Actor_StartRepeatedMotion(7, 2);
  Event_Wait(15);
}

// The byte at 0x5a of an actor, as the field its bit 0 is set through.

extern u32 gTrialRoad_LeftKnightLeavesScript[];

// ★ Named from the game: "The room is filled with traps and snares. Use the
// power of Contigo to avoid them." (MsgTrialRoadTrapInstructions)
void TrialRoad_ShowTraps(void) {
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPosition(gMainRoutineWork.currentOwner, 0, PIXELS(1016));
  Actor_SetPosition(8, PIXELS(1016), PIXELS(1016));
  for (i = 9; i < 80; i++) {
    struct Actor *object = Actor_GetById(i);

    if (object->x != 0)
      object->x -= 0x900000;
  }
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetMovementTypeById(13, gTrialRoad_LeftKnightEntersScript);
  Actor_SetMovementTypeById(14, gTrialRoad_RightKnightEntersScript);
  Event_StartScreenTransition();
  Event_Wait(20);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(184 << 16, -1, 600 << 16, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(120 << 16, -1, 552 << 16, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(120 << 16, -1, 408 << 16, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(120 << 16, -1, 120 << 16, 1);
  Event_SetMessageId((s32)MsgTrialRoadTrapInstructions);
  Event_ShowMessageAndWait(8, 0);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(216 << 16, -1, 120 << 16, 1);
  FieldCamera_WaitForArrival();
  Event_ShowMessageAndWait(8, 0);
  Event_ShowMessageAndWait(4, 0);
  Actor_SetMovementTypeById(13, gTrialRoad_LeftKnightToChestScript);
  Actor_RefreshSelectorById(13);
  Actor_SetModeById(13, 1);
  Event_Wait(10);
  Actor_SetSpeed(13, 0x4000, 0x2000);
  OBJECT_BYTE(Actor_GetById(13), 0x5a) &= ~1;
  Actor_WalkByAndWaitById(13, 0, 8);
  Event_Wait(1);
  OBJECT_BYTE(Actor_GetById(13), 0x5a) |= 1;
  Event_Wait(20);
  Actor_SetSpeed(13, 0x20000, 0x10000);
  Actor_SetModeById(13, 5);
  Actor_SetDestinationOffset(13, 0, -40);
  Actor_WaitForMoveById(13);
  Sound_Play(103);
  MapGrid_SetShake(0x20000, 0x40000, 0x10000);
  Actor_GetById(13)->velocityY = 0x40000;
  MapGrid_SetShake(-1, -1, 0xe666);
  OBJECT_BYTE(Actor_GetById(13), 0x5a) &= ~1;
  Actor_WalkByAndWaitById(13, 0, 16);
  Event_Wait(1);
  ((struct ActorSolid *)Actor_GetById(13))->solid |= 1;
  Event_Wait(60);
  Actor_ArmCallback(13, FACING_SOUTH, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(13, 4);
  Event_ShowMessageAndWait(8, 0);
  Actor_FaceDirection(13, FACING_NORTHWEST, 0);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  Actor_SetSpeed(13, 0x10000, 0x8000);
  Actor_WalkByAndWait(13, -16, -16);
  Actor_FaceDirection(13, FACING_WEST, 0);
  Sound_Play(124);
  Actor_SetModeById(9, 6);
  Event_Wait(30);
  Event_ShowMessageAndWait(8, 0);
  Actor_WalkToAndWaitById(13, 200, 168);
  Actor_FaceDirection(13, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(125);
  Actor_SetModeById(9, 5);
  Event_Wait(20);
  Sound_Play(138);
  MapGrid_CopyCellsLayered(127, 3, 1, 2, 48, 3);
  Map_CopyCellAttributeRect(0, 127, 1, 1, 14, 8);
  Event_Wait(20);
  Actor_SetMovementTypeById(13, gTrialRoad_LeftKnightLeavesScript);
  Actor_RefreshSelectorById(13);
  Sound_Play(123);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetMovementTypeById(14, gTrialRoad_RightKnightToChestsScript);
  FieldCamera_MoveTo(328 << 16, -1, 120 << 16, 1);
  FieldCamera_WaitForArrival();
  Event_ShowMessageAndWait(8, 0);
  Event_ShowMessageAndWait(8, 0);
  Actor_FaceDirection(14, FACING_NORTHEAST, 0);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_Wait(10);
  Actor_SetSpeed(14, 0x10000, 0x8000);
  Actor_WalkByAndWait(14, 16, -16);
  Actor_ArmCallback(14, 0, 0);
  Sound_Play(124);
  Actor_SetModeById(11, 6);
  Actor_SetModeById(12, 6);
  Event_Wait(30);
  Actor_WalkToAndWait(14, 350, 168);
  Actor_FaceDirection(14, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(125);
  Actor_SetModeById(11, 5);
  Event_Wait(40);
  Actor_WalkToAndWait(14, 364, 168);
  Actor_ArmCallback(14, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(125);
  Actor_SetModeById(12, 5);
  Event_Wait(20);
  Sound_Play(138);
  MapGrid_CopyCellsLayered(127, 3, 1, 2, 53, 3);
  Map_CopyCellAttributeRect(0, 127, 1, 1, 19, 8);
  Event_Wait(30);
  Actor_ArmCallback(14, FACING_NORTHWEST, 0);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_ShowMessageAndWait(8, 0);
  FieldCamera_MoveWithinMapBounds(408 << 16, -1, 120 << 16, 1);
  FieldCamera_WaitForArrival();
  Event_ShowMessageAndWait(8, 0);
  Actor_SetMovementTypeById(14, gTrialRoad_RightKnightLeavesScript);
  Actor_RefreshSelectorById(14);
  Event_ShowMessageAndWait(8, 0);
  Import_EventSetExitRequest(10);
}

// Updates the sixteen movers at gInventorySnapshot + 0x188 each frame: at its
// upAt and downAt phases a mover calls Sound_Play (206 or 395) and moves by
// 4 or -8 a frame until it reaches its high or low offset; its phase counts
// round its period unless flag 260 is set. Each mover's shape, collision
// height and cell attribute byte follow its actor and offset, and its marker
// actor is placed over it, bobbing on a sine while the offset is not 0.

// Updates the sixteen movers at gInventorySnapshot + 0x188 each frame: at its
// upAt and downAt phases a mover calls Sound_Play (206 or 395) and moves by 4
// or -8 a frame until it reaches its high or low offset; its phase counts
// round its period unless flag 260 is set. Each mover's shape, collision
// height and cell attribute byte follow its actor and offset, and its marker
// actor is placed over it, bobbing on a sine while the offset is not 0.
void TrialRoad_UpdateFountains(void) {
  struct TrialRoadFountains *work =
      (struct TrialRoadFountains *)(gInventorySnapshot + 0x188);
  struct TrialRoadFountain *mover = work->fountains;
  struct TrialRoadMapWork *map = Ram_HeapSlots->mapWork;
  s32 shape = work->shape;
  u8 *shapes = *(u8 **)((u8 *)map + 316);
  u8 *cells = map->layers[0].cells;
  s32 *collision;
  struct Actor *actor;
  struct Actor *marker;
  s32 cell;
  u8 *attributes;
  s32 i;

  i = 0;
  collision = (s32 *)Ram_MapCollision + shape;
  for (; i <= 15; collision++, i++, mover++) {
    if (mover->id == 0)
      continue;
    actor = Actor_GetById(mover->id);
    if (actor->x == 0 && actor->z == 0)
      continue;
    if (mover->phase == mover->riseAt) {
      Sound_Play(206);
      mover->speed = 4;
    }
    if (mover->phase == mover->fallAt) {
      Sound_Play(395);
      mover->speed = -8;
    }
    if (mover->speed != 0) {
      mover->height += mover->speed;
      if (mover->height >= mover->high) {
        mover->height = mover->high;
        mover->speed = 0;
        mover->bob = 0;
      }
      if (mover->height <= mover->low) {
        mover->height = mover->low;
        mover->speed = 0;
        mover->bob = 0;
      }
    }
    if (!GameFlag_Get(260)) {
      if (++mover->phase >= mover->period)
        mover->phase = 0;
    }
    cell = (actor->x >> 20) + ((actor->z >> 20) << 7);
    shapes[cell] = (u8)work->shape + i;
    *collision = ((((s16 *)&actor->y)[1] + mover->height) / 8) << 8;
    attributes = cells + cell * 4;
    if (mover->speed != 0)
      attributes[3] = 0;
    else {
      s32 bits = 128;
      bits |= attributes[3];
      attributes[3] = bits;
    }
    if (mover->height != 0) {
      s32 bits = 16;
      bits |= attributes[3];
      attributes[3] = bits;
    } else
      attributes[3] = 0;
    marker = mover->spout;
    if (marker != 0) {
      if (mover->height == 0) {
        marker->y = actor->y - 0x100000;
      } else {
        s32 wave;
        mover->bob += 0x1000;
        wave = Iwram_MulQ16(0x20000, Trig_Sin(mover->bob)) - 0xe0000;
        marker->y = actor->y + (mover->height << 16) + wave;
      }
      marker->x = actor->x;
      marker->z = actor->z;
    }
  }
}
#undef FIELD_AT
#undef SET_LAYER

// Trial Road's movers' sprites, the setup of its gauge, a rising platform's
// entry, the trap table's setup, TrialRoad_CheckCrumblingStep, the placing of
// actors 8, 9 and 10, TrialRoad_StepOnTrap and a second copy of
// Effect_AdvanceMotionAtLeaderPriority, in ROM order.

// Queues the Trial Road movers' sprites.
void TrialRoad_DrawFountains(void) {
  struct TrialRoadFountains *base =
      (struct TrialRoadFountains *)(gInventorySnapshot + 0x188);
  struct TrialRoadFountain *mover = base->fountains;
  s32 i;
  s32 tile;
  s32 frame;
  s32 count;
  s32 viewX;
  s32 viewZ;
  s32 viewY;
  s32 rise;
  struct MapWork *map;
  s32 *view;
  struct Actor *object;
  u32 *entry;
  u32 *cursor;
  s32 lift;
  s32 x;
  s32 height;
  s32 z;
  s32 sy;
  s32 depth;
  s32 bob;

  tile = gResourceTableEntries[base->vramEntry].offset >> 5;
  entry = base->sprites;
  count = 0;
  rise = -0x20000;
  map = Ram_HeapSlots->mapWork;
  view = &map->viewX;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  viewY = map->origin[1];

  for (i = 0; i < 16; i++, mover++) {
    if (mover->id == 0 || mover->height == 0)
      continue;
    object = Actor_GetById(mover->id);
    frame = ((gFrameCount % 12) / 3) * 8 + 32;
    lift = 0;
    if (lift < mover->height) {
    next:
      if (count > 79)
        goto skip;
      x = object->x - viewX;
      height = object->y + (lift << 16) - viewY;
      z = object->z - viewZ - viewY;
      x = (x >> 16) - 8;
      sy = z - height;
      sy = (sy >> 16) - 16;
      depth = ((height + z) >> 16) + 58;
      if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
        goto skip;
      x &= 0x1ff;
      sy &= 0xff;
      cursor = entry;
      *cursor++ = 0;
      *cursor++ = sy | (x << 16) | 0x40002000;
      *cursor = (tile + frame) | 0x800;
      Oam_AddSpriteAtDepth(entry, depth);
      entry += 3;
      count++;
    skip:
      lift += 16;
      if (lift < mover->height)
        goto next;
    }
    if (count > 79)
      continue;
    x = object->x - viewX;
    height = object->y + (mover->height << 16) - viewY;
    z = object->z - viewZ - viewY;
    x = (x >> 16) - 8;
    sy = z - height;
    bob = Iwram_MulQ16(rise, Trig_Sin(mover->bob));
    sy = ((sy + bob + rise) >> 16) - 4;
    depth = ((height + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      continue;
    x &= 0x1ff;
    sy &= 0xff;
    cursor = entry;
    *cursor++ = 0;
    *cursor++ = sy | (x << 16) | 0x40002000;
    *cursor = (tile + frame - 32) | 0x800;
    Oam_AddSpriteAtDepth(entry, depth);
    entry += 3;
    count++;
  }
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void *Pool_Alloc(s32 size) { return Heap_AllocEwram(size); }

void TrialRoad_StartFountains(s32 unused, s32 shape) {
  struct TrialRoadFountains *work =
      (struct TrialRoadFountains *)(gInventorySnapshot + 0x188);
  void *buffer;

  if (!Event_GetFlag(0x109))
    Iwram_ClearWords(work, 0x1c8);
  buffer = Pool_Alloc(0x800);
  Decompress_TaggedLz(gField_Tiles64D, buffer);
  work->vramEntry = VramCache_FindFreeEntry();
  VramCache_Load(work->vramEntry, 0x800, buffer);
  Heap_Release(buffer);
  work->sprites = Heap_AllocEwram(0x3c0);
  work->shape = shape;
  Task_Add(TrialRoad_UpdateFountains, 0x48d);
  Task_Add(TrialRoad_DrawFountains, 0x480);
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 0x40;
}

struct TrialRoadFountain *TrialRoad_AddFountain(s32 slot, s32 id, s32 timer,
                                                s32 low, s32 high) {
  struct TrialRoadFountains *work =
      (struct TrialRoadFountains *)(gInventorySnapshot + 0x188);
  struct TrialRoadFountain *fountain = &work->fountains[slot];
  struct Actor *object = Actor_GetById(id);
  struct Actor *spout;

  Actor_GetById(id);
  Actor_SetSpriteFlags(object, 0);
  if (!Event_GetFlag(0x109)) {
    if (high < low) {
      s32 swap = low;

      low = high;
      high = swap;
    }
    fountain->id = id;
    fountain->height = 0;
    fountain->phase = timer;
    fountain->period = 360;
    fountain->high = high;
    fountain->riseAt = 60;
    fountain->fallAt = 240;
    fountain->low = low;
    fountain->unk10 = 1;
    object->motionFlags = 0;
    object->collisionFlags = 8;
    fountain->speed = 0;
    fountain->bob = 0;
    fountain->unk14 = 0;
    object->priorityFlags |= 2;
  }
  spout = ((struct Actor *)Import_ActorCreate(0x8000, object->x, object->y,
                                              object->z));
  spout->collisionFlags = 0;
  spout->motionFlags = 0;
  spout->priorityFlags |= 128;
  fountain->spout = spout;
  return fountain;
}

// The trap table's setup: a trap whose second flag is set is put away; any
// other marks its cell, writes a shape for its height into the shape grid, and
// takes mode 0 when its first flag is set. Then the leader is lifted onto the
// ground if it stands below it.

// The trap table's setup: a trap whose second flag is set is put away; any
// other marks its cell, writes a shape for its height into the shape grid, and
// takes mode 0 when its first flag is set. Then the leader is lifted onto the
// ground if it stands below it.
DEFINE_PILLARS_RESTORE_RAISED(TrialRoad_RestoreCrumblingPillars,
                              Actor_GetCellIndex, MapGrid_GetTerrainKind,
                              Actor_DisableById, MapCollision_FindOrAddTag,
                              Actor_SelectAnimation)

DEFINE_TRAPS_CHECK(TrialRoad_CheckCrumblingStep, Field_TryJumpForward,
                   Actor_FindAtCell, Actor_SelectAnimation,
                   TrialRoad_SpawnRubble, TrialRoad_SetLeaderCellShape,
                   TrialRoad_SpawnDustRing)

void TrialRoad_CrumblePillar(const void *data, s32 id) {
  const struct TrapEntry *entry = data;
  struct Actor *object = Actor_GetById(id);
  s32 flag;

  while (entry->actor != 0xffff) {
    if (entry->actor != id) {
      entry++;
      continue;
    }
    {
      flag = entry->flag;
      Event_Start();
      if (!Event_GetFlag(flag + 1)) {
        Event_PlaySound(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        TrialRoad_SpawnRubble(0);
        wait(8);
        Actor_SelectAnimation(object, 2);
        TrialRoad_SetActorCellShape(-4, OBJECT_BYTE(object, 34), id);
        TrialRoad_SpawnRubble(1);
        wait(16);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        Actor_SetPositionAndResetMotion(object, 0, 0, 0);
        wait(2);
        Event_PlaySound(188);
        TrialRoad_SpawnDustRing();
        wait(20);
        Event_SetFlag(flag);
        Event_SetFlag(flag + 1);
      }
      Event_End();
      break;
    }
  }
}

void TrialRoad_SetPillarMode3(s32 id) {
  Actor_GetById(id)->unk5c[0] = 3;
  Actor_DisableById(id);
}

void Actor_MoveToObject(s32 id, s32 owner);
// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
// it copy actor 9's visual attributes.
DEFINE_PILLARS_RESTORE(TrialRoad_RestorePillars, TrialRoad_SetPillarMode3,
                       Actor_GetCellIndex, MapGrid_GetTerrainKind,
                       Actor_DisableById, MapCollision_FindOrAddTag)

void TrialRoad_TakePlaceOf(struct Actor *object, struct Actor *target) {
  Actor_SetPositionAndResetMotion(object, target->x, target->y, target->z);
  Actor_SetPositionAndResetMotion(target, 0, 0, 0);
}

void TrialRoad_StepOnTrap(const u16 *table) {
  struct Actor *leader;
  struct Actor *partner;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;
  u8 *motion;

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  partner = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Actor_FindAtCell(position, 0);
  if (trap != 0) {
    goto test;
  next:
    id = table[0];
    if (Actor_GetById(id) != trap) {
      table += 2;
      goto test;
    }
    flag = table[1];
    Event_Start();
    if (!GameFlag_Get(flag)) {
      Actor_SelectAnimation(partner, 1);
      TrialRoad_TakePlaceOf(partner, trap);
      wait(1);
      Sound_Play(125);
      Actor_SetSpriteColor(Actor_GetById(8), 7);
      wait(2);
      Actor_SelectAnimation(partner, 0);
      Actor_CopyGraphics(id, 9);
      Actor_SetSpriteColor(Actor_GetById(8), 0);
      wait(2);
      Actor_SetSpriteColor(Actor_GetById(8), 7);
      wait(4);
      Actor_SetSpriteColor(Actor_GetById(8), 0);
      TrialRoad_SpawnRubble(0);
      TrialRoad_TakePlaceOf(trap, partner);
      wait(1);
      GameFlag_Set(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Get(second)) {
        Actor_SelectAnimation(partner, 0);
        TrialRoad_TakePlaceOf(partner, trap);
        motion = &leader->motionFlags;
        wait(1);
        *motion = 0;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        TrialRoad_SpawnRubble(0);
        wait(8);
        Actor_SelectAnimation(partner, 2);
        TrialRoad_SetLeaderCellShape(-2, trap->terrainId);
        TrialRoad_SpawnRubble(1);
        wait(16);
        TrialRoad_SetLeaderCellShape(-2, trap->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        *motion = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(partner, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        TrialRoad_SpawnDustRing();
        wait(20);
        GameFlag_Set(second);
      }
    }
    Event_End();
    return;
  test:
    if (*table != 0xffff)
      goto next;
  }
}

// A second copy of Effect_AdvanceMotionAtLeaderPriority in this overlay:
// FieldEffect_AdvanceMotion, then the leader's sprite priority for the effect.
void TrialRoad_DriftRubble(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;
  s32 velocityX = effect->velocity.x;
  s32 velocityZ;
  struct MainRoutineWork *party;
  struct FieldSprite *sprite;
  union FieldObject *leader;
  struct EffectSpriteAttributes *to;
  struct EffectSpriteAttributes *from;
  struct EffectSpriteBits source;
  struct EffectSpriteBits bits;

  effect->x += velocityX;
  effect->y += effect->velocity.y;
  velocityZ = effect->velocity.z;
  effect->z += velocityZ;
  effect->velocity.x = velocityX - velocityX / 18;
  effect->velocity.z = velocityZ - velocityZ / 16;
  {
    /* FAKEMATCH(temporary): the x scale rate read into a block temporary,
     * which loads it before the x scale; as effect->scaleX +=
     * effect->scaleRateX, 2 of 59 instructions differ in tla-en. */
    s32 rate = effect->scaleRateX;

    effect->scaleX += rate;
  }
  sprite = effect->sprite;
  effect->scaleY += effect->scaleRateY;
  sprite->rotation += effect->spin;

  party = &gMainRoutineWork;

  leader = Actor_GetById(party->currentOwner);
  to = (struct EffectSpriteAttributes *)effect->sprite;
  from = (struct EffectSpriteAttributes *)leader->effect.sprite;
  source = from->bits;
  bits = to->bits;
  bits.priority = source.priority;
  to->bits = bits;
}

extern s32 gTrialRoad_RubbleScript[];

void TrialRoad_SpawnRubble(s32 spread) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local, so x is computed before it; as
       * one expression, 14 of 122 instructions differ in tla-en. */
      s32 y = leader->y - (((Random16() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)Import_ActorCreate(
          285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript((struct Actor *)effect, gTrialRoad_RubbleScript);
      effect->scaleY = effect->scaleX = 0xb333;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulQ16(Trig_Cos(i << 13), 0x13333);
        effect->velocity.z = Trig_Sin(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16() << 9) >> 16) - 0x100;
      effect->scaleRateX = -0x800;
      effect->scaleRateY = -0x600;
      effect->update = TrialRoad_DriftRubble;
    }
    i++;
  } while (i <= 7);
}

extern s32 gTrialRoad_DustScript[];

void TrialRoad_SpawnDustRing(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  i = 0;
  do {
    effect = (struct FieldEffect *)Import_ActorCreate(285, leader->x, leader->y,
                                                      leader->z);
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript((struct Actor *)effect, gTrialRoad_DustScript);
      effect->velocity.x = Trig_Cos(i << 12);
      effect->velocity.y = 0;
      effect->velocity.z = Trig_Sin(i << 12);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += ((Random16() * 3) << 14 >> 16) - 0x6000;
      effect->velocity.z += ((Random16() * 3) << 13 >> 16) - 0x3000;
      effect->spin = ((Random16() << 12) >> 16) - 0x800;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = TrialRoad_DriftRubble;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
    i++;
  } while (i <= 16);
}

DEFINE_ACTOR_SET_CELL_SHAPE(TrialRoad_SetActorCellShape, MapGrid_GetTerrainKind,
                            MapCollision_FindOrAddTag)

void TrialRoad_SetLeaderCellShape(s32 a, s32 b) {
  TrialRoad_SetActorCellShape(a, b, gMainRoutineWork.currentOwner);
}

extern u32 gTrialRoad_HexDigits[];

// Writes value as eight hexadecimal digits and a terminating null.
UNUSED void String_WriteU32AsHex(u8 *buf, u32 value) {
  s32 i;
  const u8 *digits = gTrialRoad_HexDigits;

  buf += 8;
  *buf = 0;
  buf--;
  i = 7;
  // A goto loop, which builds the 15 on every pass: as a for loop, the 15 is
  // moved out of it (differ 13).
next:
  *buf = digits[value & 15];
  value >>= 4;
  buf--;
  i--;
  if (i >= 0)
    goto next;
}

extern u8 gSceneState[];

void TrialRoad_Idle(void) {}

void TrialRoad_SetBalanceStateReady(void) {
  u16 *state = (u16 *)gSceneState;
  u16 ready = 9;

  *state = ready;
}

void TrialRoad_WaitForBalanceState(void) {
  s16 *state = (s16 *)gSceneState;

  while (*state != 9) {
    wait(1);
  }
}

// The Trial Road rival's per-frame update.

// Updates the Trial Road rival by the scene state's mode: 4 follows the
// leader's position mirrored across the course; 3 places the rival and its
// partner from the linked player's record; 1 replays the recorded path; 2
// records the actor's position.
void TrialRoad_MoveRival(void) {
  struct TrialRoadLinkWork *work =
      *(struct TrialRoadLinkWork **)((u8 *)Ram_HeapSlots + 236);
  struct TrialRoadReplay *state = &gSceneState;
  struct Actor *rival = Actor_GetById(state->id);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x, z, turn, dx, dz;
  u16 heading;

  if (rival == 0)
    return;
  if (state->mode == 4) {
    x = leader->x;
    z = leader->z;
    if ((u32)(z - 0x1000000) > 0x1300000)
      x = 0x2100000 - (x & 0xffff0000);
    if (gMainRoutineWork.entrance & 1)
      x += 0x1300000;
    else
      x -= 0x1300000;
    if (rival->x != x || rival->z != z) {
      dx = x - rival->x;
      dz = z - rival->z;
      heading = ArcTan2(dz, dx);
      turn = (s16)(heading - rival->facing);
      if (turn > 0x1000)
        turn = 0x1000;
      if (turn < -0x1000)
        turn = -0x1000;
      rival->facing += turn;
      rival->x = x;
      rival->z = z;
    }
    if (state->still > 2)
      Actor_SelectAnimation(rival, 1);
    else
      Actor_SelectAnimation(rival, 5);
  } else if (state->mode == 3) {
    s32 player;
    s16 *signature;
    s32 angle, link, ok, partner;
    s16 st;

    player = ((s32)GameFlag_GetByte(760)) ^ 1;
    signature = (s16 *)gLinkPeerSignatures[player];
    if (((gLinkStatus >> player) & 1) == 0)
      return;
    x = *signature++;
    z = *signature++;
    angle = *signature++;
    st = *signature++;
    link = st >> 8;
    ok = st & 0xff;
    if (GameFlag_Get(dz = 529) && link != 0) {
      state->mode = 9;
      Actor_SelectAnimation(rival, 1);
      if (work->linkX < rival->x)
        x = work->linkX + 0xc0000;
      else
        x = work->linkX - 0xc0000;
      if (!GameFlag_Get(530)) {
        z = work->linkZ + 0x100000;
        rival->timer = work->linkAngle;
      } else {
        z = work->linkZ - 0x100000;
        rival->timer = work->unke6;
      }
      rival->acceleration = 0x4000;
      rival->speedLimit = 0x10000;
      Actor_SetMoveTarget(rival, x, 0, z);
      Actor_StartIdle(rival);
    } else {
      x <<= 16;
      z <<= 16;
      rival->facing = angle;
      rival->x = x;
      rival->z = z;
      Actor_SelectAnimation(rival, ok);
    }
    partner = *signature++;
    x = *signature++;
    z = *signature++;
    angle = *signature++;
    ok = *signature++;
    if (partner == 0)
      return;
    rival = Actor_GetById(partner);
    if (rival == 0)
      return;
    rival->x = x << 16;
    rival->facing = angle;
    rival->z = z << 16;
    Actor_SelectAnimation(rival, ok);
  } else if (state->mode == 1) {
    if (state->resource != work->resource) {
      Decompress_TaggedLz(Resource_GetTableEntry(state->resource), work->path);
      work->resource = state->resource;
    }
    x = work->path[state->point];
    z = work->path[state->point + 1];
    if (!GameFlag_Get(260))
      state->point += 2;
    if (x == 0 && z == 0) {
      state->mode = 9;
      return;
    }
    x <<= 16;
    z <<= 16;
    if (state->mirrored != 0 && (u32)(z - 0x1000000) > 0x1300000)
      x = 0x2100000 - x;
    if (gMainRoutineWork.entrance & 1)
      x += 0x1300000;
    else
      x -= 0x1300000;
    if (rival->x != x || rival->z != z) {
      dx = x - rival->x;
      dz = z - rival->z;
      heading = ArcTan2(dz, dx);
      turn = (s16)(heading - rival->facing);
      if (turn > 0x1000)
        turn = 0x1000;
      if (turn < -0x1000)
        turn = -0x1000;
      rival->facing += turn;
      rival->x = x;
      rival->z = z;
      state->still = 0;
    } else {
      state->still++;
    }
    if (state->still > 2)
      Actor_SelectAnimation(rival, 1);
    else
      Actor_SelectAnimation(rival, 5);
  } else if (state->mode == 2) {
    x = rival->x >> 16;
    z = rival->z >> 16;
    work->path[state->point] = x;
    work->path[state->point + 1] = z;
    if (!GameFlag_Get(260))
      state->point += 2;
    if (state->point == 7198) {
      // A u16 read back as s16 at its use: the same value, the spelling that
      // matches.
      u16 next = state->point + 1;

      work->path[state->point] = 0;
      work->path[(s16)next] = 0;
      state->id = work->rival;
      state->point = 0;
      state->mode = 1;
    }
  }
}

// next object the overlay links, and the overlay's data.

/* VIEW(s51a0): tag struct TrialRoadLinkWork as struct TrialRoadLinkWork__s51a0;
 * s15a8.c has another definition; tried with the definition the rest of the
 * object has, it does not compile in tla-ja:
 * games/tla/src/scene/trial_road.c:3454: structure has no member named
 * `vramEntry'. */
struct TrialRoadLinkWork__s51a0 {
  u32 oam[54];
  s16 vramEntry;
  s16 fade;
  s16 scrollX;
  s16 scrollY;
  s16 unke0;
  s16 a;
  s16 b;
  u8 unke6[6];
  s32 originX;
  s32 originZ;
};

extern const u8 gTrialRoad_ProgressPicture[];

// The picture's buffer: 576 bytes of EWRAM, the size VramCache_Load copies.
static inline void *TrialRoadLink_AllocPicture(void) {
  return Heap_AllocEwram(576);
}

// left and right are the arms' offsets, each opening with its sign, so the
// subtractions compile in the order the native has them.
#define MARKER(left, right, which, tileAdd)                                    \
  do {                                                                         \
    s32 lift;                                                                  \
    /* FAKEMATCH(pin): originZ in r6; as a plain s32 *, 304 of 497             \
       instructions differ in tla-en. */                                       \
    register s32 *originZ asm("r6");                                           \
    /* FAKEMATCH(pin): x in r9; as a plain s32, 365 of 497 instructions        \
       differ in tla-en. */                                                    \
    register s32 x asm("r9");                                                  \
    /* FAKEMATCH(pin): arg in r0; as a plain s32, 44 of 497 instructions       \
       differ in tla-en. */                                                    \
    register s32 arg asm("r0");                                                \
    s32 side = gMainRoutineWork.entrance & 1;                                  \
    if (side) {                                                                \
      x = (actor->x - work->originX) / PIXELS(16) left;                        \
      arg = which;                                                             \
    } else {                                                                   \
      x = (actor->x - work->originX) / PIXELS(16) right;                       \
      arg = which;                                                             \
    }                                                                          \
    originZ = &work->originZ;                                                  \
    lift = TrialRoad_GetStage(arg);                                            \
    y = (actor->z - *originZ) / PIXELS(16) + work->scrollY - lift * 36 + 4;    \
    /* Once-only: ends the block, so y + 4 is scheduled before x & 511. */     \
    do {                                                                       \
    } while (0);                                                               \
    x &= 511;                                                                  \
    y &= 255;                                                                  \
    *cursor++ = 0;                                                             \
    y |= x << 16;                                                              \
    y |= markerSize;                                                           \
    *cursor++ = y;                                                             \
    *cursor++ = (tile + (tileAdd)) | 0xe400;                                   \
    Oam_AddSpriteAtDepth(entry++, 255);                                        \
  } while (0)

// The Trial Road link display's scheduled callback, which
// TrialRoad_ShowProgress starts. Its fade state falls during events or while
// flag 262 is set and otherwise rises to 2 (unke0 holds it at 2); rising from
// 0 to 1 it loads the palette and picture again, and at 0 it frees its VRAM.
// Otherwise it queues two rows of sprites, each up to four pairs 36 pixels
// apart down from the scrolled offset, sliding in 6 pixels per fade step (the
// right row mirrored), and on frames 5 to 15 of every 16 a marker over each of
// the two watched actors, projected at 1/16 scale from their positions and the
// course side.
void TrialRoad_DrawProgress(void) {
  struct TrialRoadLinkWork__s51a0 *work = Ram_HeapSlots->unkec[0];
  u32 *cursor = (u32 *)work;
  struct TrialRoadSprite *entry = (struct TrialRoadSprite *)work;
  s16 *vramEntry = &work->vramEntry;
  u32 row, i;
  s32 tile = gResourceTableEntries[*vramEntry].offset >> 5;
  u32 flip, word;
  s32 y;
  // One constant for its whole life: the markers' OAM size bits (attribute 1
  // size 1, 16 by 16), held in a register across both markers.
  u32 markerSize = 0x40000000;
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct Actor *actor;

  if (work->unke0 != 0) {
    work->fade = 2;
  } else if (GameFlag_Get(262) || event->unk166 != 0) {
    if (work->fade > 0)
      work->fade--;
  } else if (work->fade <= 1) {
    if (++work->fade == 1) {
      void *buffer;
      Dma_Set(gTrialRoad_ProgressPalette, (void *)0x050003c0, 0x80000010,
              REG_DMA3);
      buffer = TrialRoadLink_AllocPicture();
      Decompress_TaggedLz(gTrialRoad_ProgressPicture, buffer);
      VramCache_Load(*vramEntry, 576, buffer);
      Heap_Release(buffer);
    }
  }
  if (work->fade == 0) {
    VramCache_ReleaseVram(work->vramEntry);
    return;
  }
  for (row = 0; row < 2; row++) {
    s32 x;
    y = (work->scrollY + 8) & 255;
    if (row == 0) {
      x = work->fade * 6 + work->scrollX - 12;
      x &= 511;
      flip = 0;
    } else {
      x = 236 - work->fade * 6 - work->scrollX;
      x &= 511;
      flip = 0x10000000;
    }
    for (i = 0; i < 4 && y < 160; i++) {
      *cursor++ = 0;
      *cursor++ = y | (x << 16) | flip | 0x80008000;
      *cursor++ = tile | 0xe400;
      Oam_AddSpriteAtDepth(entry++, 255);
      word = y + 32;
      *cursor++ = 0;
      *cursor++ = word | (x << 16) | flip | 0x4000;
      *cursor++ = (tile + 8) | 0xe400;
      Oam_AddSpriteAtDepth(entry++, 255);
      y += 36;
    }
  }
  if ((gFrameCount & 15) < 5)
    return;
  actor = ObjectTable_Get(work->a);
  if (actor != NULL) {
    MARKER(+work->fade * 6 + work->scrollX - 19,
           -work->fade * 6 - work->scrollX + 232, 0, 10);
  }
  actor = ObjectTable_Get(work->b);
  if (actor == NULL || actor->x == 0)
    return;
  {
    // XXX: this side's offset adds scrollX where the first marker's subtracts
    // it; the native does so, and a render of the course's far side would show
    // whether it is a slip.
    MARKER(+work->fade * 6 + work->scrollX + 189,
           -work->fade * 6 + work->scrollX + 23, 1, 14);
  }
}

void TrialRoad_ShowProgress(s32 a, s32 b) {
  struct TrialRoadLink *link = HeapSlot_AllocEwram(236, 0x3934);
  void *buffer = Heap_AllocEwram(576);
  s32 zero;
  s32 owner;
  u16 *unkda;
  u16 *unke0;
  s32 *height;
  s32 *unkf0;
  s16 *vramEntry;

  link->a = a;
  link->b = b;
  owner = gMainRoutineWork.currentOwner;
  height = &link->height;
  unkf0 = &link->unkf0;
  unkda = &link->unkda;
  unke0 = &link->unke0;
  vramEntry = &link->vramEntry;
  if (a == owner) {
    *height = 0xa00000;
    *unkf0 = 0xf9a00000;
  } else {
    *height = 0x700000;
    *unkf0 = 0xf9a00000;
  }
  Actor_GetById(a);
  Actor_GetById(b);
  zero = 0;
  *unkda = zero;
  *unke0 = zero;
  Decompress_TaggedLz(gTrialRoad_ProgressPicture, buffer);
  *vramEntry = VramCache_FindFreeEntry();
  VramCache_Load(*vramEntry, 576, buffer);
  Task_Add(TrialRoad_DrawProgress, 0x476);
  Heap_Release(buffer);
}

void TrialRoad_StartRival(s32 a) {
  u8 *work = Ram_HeapSlots->unkec[0];
  struct TrialRoadControl *control = (struct TrialRoadControl *)gSceneState;

  if (Event_GetFlag(0x109) == 0) {
    /* FAKEMATCH(temporary): a local reads the work value before the three
     * stores; as the value stored directly, 4 of 24 instructions differ in
     * tla-en. */
    s32 value = *(u16 *)(work + 228);

    control->field02 = a;
    control->field04 = value;
    control->field08 = 0;
  }
  Task_Add(TrialRoad_MoveRival, 0x485);
}

void TrialRoad_PlayRivalRecording(s32 a) {
  struct TrialRoadControl *control = (struct TrialRoadControl *)gSceneState;
  s32 zero = 0;

  *(u16 *)((Ram_HeapSlots->unkec[0]) + 234) = zero;
  control->field0a = a;
  control->field06 = zero;
  control->state = 1;
  TrialRoad_MoveRival();
}

s32 TrialRoad_RivalIsWaiting(void) {
  s32 state = *(s16 *)gSceneState;

  return IsZero(state ^ 9);
}

void TrialRoad_DoNothing(void) {}

s32 gTrialRoad_SettleScript[] = {
    46,
    (s32)TrialRoad_SettleOnGround,
    17,
};

s32 gTrialRoad_PillarTopScript[] = {
    46,
    (s32)TrialRoad_FollowPillar,
    17,
};

u32 gTrialRoad_PoundPartners[] = {
    0x14151213,
};

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_MediumScript[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_LongScript[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17, 17,
};

u32 gTrialRoad_GaspHopScript[] = {
    23, 3, 0x1000, 0, 1, 13,
    0xffff, 0,
};

void (*gTrialRoad_StageSetUps[])(void) = {
    TrialRoad_SetUpPillarStage,
    TrialRoad_SetUpTorchStage,
    TrialRoad_SetUpBushPillarStage,
    TrialRoad_SetUpPoundStage,
    TrialRoad_SetUpCrumblingStage,
    TrialRoad_SetUpLogStage,
    TrialRoad_SetUpFountainStage,
    TrialRoad_SetUpSandStage,
};

u32 gTrialRoad_RivalRecordings[] = {
    450, 451, 452, 453, 454, 455,
    456, 457, 458, 459,
};

s16 gTrialRoad_StageMapCells[][2] = {
    {1, 48}, {15, 48}, {29, 48}, {1, 70},
    {15, 70}, {29, 70}, {1, 92}, {15, 92},
};

s8 gTrialRoad_DemoSceneryStages[] = {
    0, 1, 2, 3, 4, 4, 4, 5,
    6, 7,
};

s8 gTrialRoad_SceneryStages[] = {
    0, 1, 2, 3, 4, 4, 4, 5,
    6, 7,
};

s8 gTrialRoad_KeyStages[] = {
    0, 2, 3, 7, 1, 4, 5, 6,
};

u32 gTrialRoad_RightKnightEntersScript[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    22, 5, 0x1380000, 22, 7, 0x2a80000,
    39, 5, 4, 0x1380000, 0, 0x2780000,
    1, 4, 0x1580000, 0, 0x2680000, 1,
    4, 0x1980000, 0, 0x2680000, 1, 4,
    0x1a80000, 0, 0x2580000, 1, 4, 0x1a80000,
    0, 0x1c80000, 1, 17,
};

u32 gTrialRoad_LeftKnightEntersScript[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    22, 5, 0xe80000, 22, 7, 0x2a80000,
    39, 5, 4, 0xe80000, 0, 0x2780000,
    1, 4, 0xc80000, 0, 0x2680000, 1,
    4, 0x880000, 0, 0x2680000, 1, 4,
    0x780000, 0, 0x2580000, 1, 4, 0x780000,
    0, 0x1c80000, 1, 17,
};

u32 gTrialRoad_LeftKnightToChestScript[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    22, 5, 0x780000, 22, 7, 0x1280000,
    39, 5, 4, 0x780000, 0, 0xd80000,
    1, 4, 0x880000, 0, 0xc80000, 1,
    4, 0xd80000, 0, 0xc80000, 1, 4,
    0xe80000, 0, 0xb80000, 1, 0, 2,
    22, 3, 0xc000, 17,
};

u32 gTrialRoad_LeftKnightLeavesScript[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    39, 5, 4, 0xd80000, 0, 0xa80000,
    1, 4, 0xe80000, 0, 0x980000, 1,
    4, 0xe80000, 0, 0x880000, 1, 22,
    5, 0, 22, 7, 0, 17,
};

u32 gTrialRoad_RightKnightToChestsScript[] = {
    22, 15, 0x10000, 22, 16, 0x8000,
    22, 5, 0x1580000, 22, 7, 0xb80000,
    39, 2, 4, 0x1380000, 0, 0xa80000,
    1, 39, 1, 0, 2, 22,
    3, 0x8000, 0, 2, 22, 3,
    0x6000, 0, 2, 22, 3, 0x4000,
    17,
};

u32 gTrialRoad_RightKnightLeavesScript[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    39, 5, 4, 0x1780000, 0, 0xc80000,
    1, 4, 0x1d80000, 0, 0xc80000, 1,
    4, 0x1e80000, 0, 0xb80000, 1, 4,
    0x1e80000, 0, 0x880000, 1, 39, 1,
    0, 2, 22, 3, 0xa000, 0,
    2, 22, 3, 0x8000, 0, 2,
    22, 3, 0x6000, 0, 2, 22,
    3, 0x4000, 17,
};
// clang-format on
#undef MARKER

#undef DEFINE_TRIAL_ROAD_SET_POOL_TILES
#undef FIELD_AT
#undef SET_LAYER
#undef MARKER
#undef gTrialRoad_PaletteCycleRunning
#undef gTrialRoad_PaletteCycleFrame

// clang-format off
LATE_DATA s32 gTrialRoad_RubbleScript[] = {
    0, 33, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

LATE_DATA s32 gTrialRoad_DustScript[] = {
    0, 44, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};
// clang-format on

void TrialRoad_FightGasp(void);
void TrialRoad_OpenChests(void);
void TrialRoad_JumpForward(void);
void TrialRoad_BlowBushOffPoundPillar(void);
void TrialRoad_StandOnBlock(void);
void TrialRoad_OpenSummitGate(s32 sound);
void TrialRoad_StepPastChests(void);

// clang-format off
LAST_DATA u32 gTrialRoad_HexDigits[] = {
    0x33323130, 0x37363534, 0x42413938, 0x46454443, 0,
};

LAST_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 440, 0, 184, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 344, 0, 264, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LAST_DATA u32 gSceneExits[] = {
    SCENE_EXITS(247),
    SCENE_EXIT(1, 247, 3),
    SCENE_EXIT(2, 247, 4),
    SCENE_EXIT(3, 247, 1),
    SCENE_EXIT(4, 247, 2),
    SCENE_EXIT(5, 241, 1),
    SCENE_EXIT(6, 241, 2),
    SCENE_EXIT(7, 248, 1),
    SCENE_EXIT(8, 248, 2),
    SCENE_EXIT(9, 241, 9),
    SCENE_EXIT(10, 241, 11),
    SCENE_EXITS(249),
    SCENE_EXIT(1, 249, 2),
    SCENE_EXIT(2, 249, 1),
    SCENE_EXIT(3, 248, 3),
    SCENE_EXIT(4, 250, 1),
    SCENE_EXITS(250),
    SCENE_EXIT(1, 249, 4),
    SCENE_EXITS(248),
    SCENE_EXIT(1, 247, 33),
    SCENE_EXIT(2, 247, 34),
    SCENE_EXIT(3, 249, 3),
    SCENE_EXIT(4, 244, 30),
    SCENE_EXIT(5, 244, 31),
    SCENE_EXITS_END,
};

LAST_DATA struct ActorPlacement gTrialRoad_Placements[] = {
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_PillarStagePlacements[] = {
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(152), FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(344), FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(456), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(392), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(456), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(392), FACING_EAST, 2, 1},
    {307, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, PIXELS(248), 0, PIXELS(400),
     FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_TorchStagePlacements[] = {
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(152), FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(424), FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(424), FACING_EAST, 2, 1},
    {290, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(440), FACING_EAST, 2, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(440), FACING_EAST, 2, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(456), FACING_EAST, 2, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(456), FACING_EAST, 2, 1},
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_BushPillarStagePlacements[] = {
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(152), FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(424), FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(424), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(440), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(456), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(456), FACING_EAST, 2, 1},
    {307, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, PIXELS(232), 0, PIXELS(432),
     FACING_EAST, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, PIXELS(296), 0, PIXELS(432),
     FACING_EAST, 2, 1},
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_PoundStagePlacements[] = {
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(152), FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(456), FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(456), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(456), FACING_EAST, 2, 1},
    {406, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(424), FACING_EAST, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(424), FACING_EAST, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(424), FACING_EAST, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(424), FACING_EAST, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, PIXELS(264), 0, PIXELS(464),
     FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_CrumblingStagePlacements[] = {
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(152), FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {404, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(440), FACING_EAST, 2, 0},
    {404, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(440), FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_LogStagePlacements[] = {
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(152), FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(392), FACING_EAST, 2, 0},
    {261, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(392), FACING_EAST, 2, 0},
    {261, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(440), FACING_EAST, 2, 0},
    {260, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(456), FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_FountainStagePlacements[] = {
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(152), FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(472), FACING_EAST, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(376), FACING_EAST, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(376), FACING_EAST, 2, 1},
    {489, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(376), FACING_EAST, 2, 1},
    {489, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(440), FACING_EAST, 2, 1},
    {489, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(440), FACING_EAST, 2, 1},
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_SandStagePlacements[] = {
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(152), FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(408), FACING_EAST, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(408), FACING_EAST, 2, 1},
    {307, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, PIXELS(312), 0, PIXELS(336),
     FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_Room3Placements[] = {
    {329, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(280), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_Room4Placements[] = {
    {246, 0x7b, 1, PIXELS(344), 0, PIXELS(264), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_Room2Placements[] = {
    {4, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(328), FACING_EAST, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

LAST_DATA s32 gTrialRoad_Room1Placements[] = {
    (s32)gTrialRoad_PillarStagePlacements,
    (s32)gTrialRoad_TorchStagePlacements,
    (s32)gTrialRoad_BushPillarStagePlacements,
    (s32)gTrialRoad_PoundStagePlacements,
    (s32)gTrialRoad_CrumblingStagePlacements,
    (s32)gTrialRoad_LogStagePlacements,
    (s32)gTrialRoad_FountainStagePlacements,
    (s32)gTrialRoad_SandStagePlacements,
};

LAST_DATA struct Trigger gTrialRoad_Triggers[] = {
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_PillarStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, 0x284, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, 0x284, EVENT(TrialRoad_OpenChests)},
    {0x1fe14, 0x409, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40a, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40b, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40c, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {0x4602, 51, IF_FLAG_SET(0x208), EVENT(TrialRoad_JumpAcrossGap)},
    {0x4602, 52, IF_FLAG_SET(0x208), EVENT(TrialRoad_JumpForward)},
    {8, 0, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 17, IF_FLAG_SET(0x307), EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 18, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 19, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, IF_FLAG_SET(0x307), EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 18, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 20, 0x307, EVENT(TrialRoad_BlowBushOffLonePillar)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_TorchStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, 0x284, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, 0x284, EVENT(TrialRoad_OpenChests)},
    {0x1fe14, 0x409, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40a, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40b, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40c, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {0xc602, 33, CONDITION_ALWAYS, EVENT(TrialRoad_JumpMeltedPuddle)},
    {0x4602, 33, CONDITION_ALWAYS, EVENT(TrialRoad_JumpMeltedPuddle)},
    {8, 0, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 21, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 22, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockTorch)},
    {9, 0, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 21, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 22, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 17, IF_FLAG_SET(0x214), EVENT(TrialRoad_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 18, IF_FLAG_SET(0x215), EVENT(TrialRoad_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 19, IF_FLAG_SET(0x216), EVENT(TrialRoad_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 20, IF_FLAG_SET(0x217), EVENT(TrialRoad_FreezePuddle)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_BushPillarStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, 0x284, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, 0x284, EVENT(TrialRoad_OpenChests)},
    {0x1fe14, 0x409, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40a, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40b, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40c, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {0x4602, 33, CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {8, 0, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, IF_FLAG_SET(0x327), EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, IF_FLAG_SET(0x328), EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 17, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 18, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 19, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, IF_FLAG_SET(0x327), EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, IF_FLAG_SET(0x328), EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 18, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 20, 0x327, EVENT(TrialRoad_BlowBushOffPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 21, 0x328, EVENT(TrialRoad_BlowBushOffPillar)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_PoundStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, 0x284, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, 0x284, EVENT(TrialRoad_OpenChests)},
    {0x1fe14, 0x409, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40a, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40b, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40c, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {0x202, 50, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {0x202, 51, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {0x202, 52, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {0x202, 53, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {TRIGGER_STEP, 50, 0x335, EVENT(TrialRoad_StepOnPillar18)},
    {TRIGGER_STEP, 51, IF_FLAG_SET(0x335), EVENT(TrialRoad_StepOnPillar19)},
    {TRIGGER_STEP, 52, 0x337, EVENT(TrialRoad_StepOnPillar20)},
    {TRIGGER_STEP, 53, IF_FLAG_SET(0x337), EVENT(TrialRoad_StepOnPillar21)},
    {0x202, 60, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(TrialRoad_CheckPoundedPillarsCovered)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, IF_FLAG_SET(0x339), EVENT(TrialRoad_CheckPoundedPillarsCovered)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS, EVENT(TrialRoad_CheckPoundedPillarsCovered)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 22, 0x339, EVENT(TrialRoad_BlowBushOffPoundPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 18, 0x335, EVENT(TrialRoad_PoundPillar18)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 19, IF_FLAG_SET(0x335), EVENT(TrialRoad_PoundPillar19)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 20, 0x337, EVENT(TrialRoad_PoundPillar20)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 21, IF_FLAG_SET(0x337), EVENT(TrialRoad_PoundPillar21)},
    {TRIGGER_TABLE_END},
};

LAST_DATA s16 gTrialRoad_CrumblingPillars[] = {
    15, 834, 16, 836, -1, 0,
};
// clang-format on

LAST_DATA struct Trigger gTrialRoad_CrumblingStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, 0x284, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, 0x284, EVENT(TrialRoad_OpenChests)},
    {0x1fe14, 0x409, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40a, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40b, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40c, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {0x4602, 33, CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {0xc602, 33, CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {0xc602, 50, CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {0xc602, 51, CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {0x602, 34, CONDITION_ALWAYS, EVENT(TrialRoad_CheckCrumblingPillars)},
    {0x8602, 34, CONDITION_ALWAYS, EVENT(TrialRoad_CheckCrumblingPillars)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50, 0x343,
     EVENT(TrialRoad_CrumblePillar15)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 51, 0x345,
     EVENT(TrialRoad_CrumblePillar16)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_LogStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, 0x284, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, 0x284, EVENT(TrialRoad_OpenChests)},
    {0x1fe14, 0x409, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40a, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40b, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40c, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {0x602, 33, CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {0x8602, 50, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog16West)},
    {0x602, 50, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog16East)},
    {0x8602, 51, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog17West)},
    {0x602, 51, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog17East)},
    {0xc602, 50, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog18North)},
    {0x4602, 50, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog18South)},
    {0xc602, 51, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog18North)},
    {0x4602, 51, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog18South)},
    {0xc602, 52, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog18North)},
    {0x4602, 52, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog18SouthFromRow52)},
    {0xc602, 53, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog18North)},
    {0x4602, 53, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog18South)},
    {8, 0, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15,
     CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS,
     EVENT(TrialRoad_BlockMovedPillar)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_FountainStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, 0x284, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, 0x284, EVENT(TrialRoad_OpenChests)},
    {0x1fe14, 0x409, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40a, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40b, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40c, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(TrialRoad_StandOnBlock)},
    {8, 0, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15,
     CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS,
     EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 50, 0x363,
     EVENT(TrialRoad_UncoverSpout16)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 51, 0x364,
     EVENT(TrialRoad_UncoverSpout17)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 52, 0x365,
     EVENT(TrialRoad_UncoverSpout18)},
    {TRIGGER_TABLE_END},
};

LAST_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

LAST_DATA struct Trigger gTrialRoad_SandStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, 0x284, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, 0x284, EVENT(TrialRoad_OpenChests)},
    {0x1fe14, 0x409, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40a, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40b, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40c, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(TrialRoad_DriftWithStream)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(TrialRoad_StartSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(TrialRoad_EndSand)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 17, 0x374,
     EVENT(TrialRoad_RevealVine)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 8, 0x289, EVENT(TrialRoad_LiftCaveRock)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 33,
     CONDITION_ALWAYS, EVENT(TrialRoad_CutBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 34,
     CONDITION_ALWAYS, EVENT(TrialRoad_CutMadPlantBushes)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(TrialRoad_FightGasp)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(TrialRoad_RevealPoolTiles)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(TrialRoad_HidePoolTiles)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_Room2Triggers[] = {
    {TRIGGER_STEP, 33, 0x283, EVENT(TrialRoad_OpenSummitGate)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x9ff), EVENT(TrialRoad_ChallengeMoapa)},
    {TRIGGER_STEP, 41, IF_FLAG_SET(0x9ff), EVENT(TrialRoad_ChallengeMoapa)},
    {49, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x9fc,
     EVENT(TrialRoad_SetSummitPillar)},
    {9, 0, 0x9fc, EVENT(TrialRoad_SetSummitPillar)},
    {TRIGGER_TABLE_END},
};

LAST_DATA s32 gTrialRoad_Room1Triggers[] = {
    (s32)gTrialRoad_PillarStageTriggers,     (s32)gTrialRoad_TorchStageTriggers,
    (s32)gTrialRoad_BushPillarStageTriggers, (s32)gTrialRoad_PoundStageTriggers,
    (s32)gTrialRoad_CrumblingStageTriggers,  (s32)gTrialRoad_LogStageTriggers,
    (s32)gTrialRoad_FountainStageTriggers,   (s32)gTrialRoad_SandStageTriggers,
};

LAST_DATA s32 gTrialRoad_PaletteCycleFrame[] = {
    -1,
};

LAST_DATA u32 gTrialRoad_PaletteCycleRunning[] = {
    1,
};
