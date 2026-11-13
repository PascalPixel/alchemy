/// @file
/// The Madra Catacombs, under the town: a ladder room entered from Madra's two
/// ladders, the tunnel the Champa dug, the deserted village with its pillars,
/// rope, puddle and the door Reveal opens, the room whose bookshelf Tremor
/// shakes until a chest falls into the room below, and the tablet room whose
/// red double door the Ruin Key opens, lighting its flames before Moloch's
/// tablet. Once the Black Crystal is stolen (flag 0x8ab) the Champa's way in is
/// blocked.

#include "shared/map_cells.h"
#include "flags.h"
#include "item_ids.h"
#include "shared/actors.h"
#include "shared/tables.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "map.h"
#include "system/iwram_call.h"
#include "field/event_inline.h"
#include "field_sprite.h"
#include "field/field.h"
#include "scene_tables.h"
#include "event_work.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "objcmd.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"

struct Actor;

s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

extern struct SceneEntrance gMadraCatacombs_Entrances[];
extern struct SceneEntrance gMadraCatacombs_Room4Entrances[];

extern struct SceneEntrance gMadraCatacombs_Room5Entrances[];

extern s16 gMadraCatacombs_LadderRoomRegions[];

extern struct ActorPlacement gMadraCatacombs_Placements[];

extern struct ActorPlacement gMadraCatacombs_Room1Placements[];

extern struct ActorPlacement gMadraCatacombs_Room3Placements[];

extern struct ActorPlacement gMadraCatacombs_Room5Placements[];
extern struct ActorPlacement gMadraCatacombs_Room6Placements[];
extern struct Trigger gMadraCatacombs_Room1Triggers[];
extern struct Trigger gMadraCatacombs_Room2Triggers[];
extern struct Trigger gMadraCatacombs_Room3Triggers[];
extern struct Trigger gMadraCatacombs_Room4Triggers[];
extern struct Trigger gMadraCatacombs_Room5Triggers[];
extern struct Trigger gMadraCatacombs_Room6Triggers[];

void MadraCatacombs_SetUpVillage(void);

void MadraCatacombs_SetUpChampaTunnel(void);

void MadraCatacombs_SetUpBookshelfRoom(void);

void MadraCatacombs_SetUpTabletRoom(void);

extern s16 gMadraCatacombs_PushedPillar[];
extern s16 gMadraCatacombs_Puddles[];

extern u32 gMadraCatacombs_FlameFlaresUp[];

extern s32 gMadraCatacombs_GreatFlameFlaresUp[];
extern s16 gMadraCatacombs_ClimbablePillar[];
extern s32 gMadraCatacombs_WalkIntoDoorway[];

void Actor_DisableById(s32 id);

s32 Field_FindNearbyTalkTarget(s32 owner);

s32 Leader_CanPushObjectAhead(void);

void MadraCatacombs_RockChestWithBookshelf(void);

void MadraCatacombs_LeaveBlockedVillage(void);

void Rope_Place(s32 a, s32 b, s32 c, s32 d);

#include "text/msg_ids.h"

struct Actor *Leader_FindActorAhead(void);
void Actor_PlaceMovedBlock(void);
void Leader_PushObjectAhead(void);

s32 Actor_GetCellIndex(struct Actor *object);

// Raises each listed actor's cells: the shape grid and the cell flags under it,
// then lifts the leader if the new terrain is above the leader.
DEFINE_PILLARS_RAISE(MadraCatacombs_RaisePillarCells, MapGrid_GetTerrainKind,
                     Actor_GetCellIndex, MapCollision_FindOrAddTag)

#define HELD_OBJECT(work) (*(struct Actor **)((u8 *)(work) + 0xcbc))

/// Lowers the cells under the held actor, or the one the leader carries, for
/// each listed actor it is: the shape grid takes the shape for its new height
/// and the cell flags under it are cleared; the actor is kept at 0xcbc of the
/// event work.
void MadraCatacombs_LowerHeldPillarCells(u16 *list) {
  struct Actor *held = Leader_FindActorAhead();
  void *work = Ram_HeapSlots->eventWork;
  u8 *grid;

  if (held == NULL) {
    s32 id = Field_FindNearbyPsynergyTarget(gMainRoutineWork.currentOwner, 1);

    if (id != -1)
      held = Actor_GetById(id);
  }
  grid = Ram_MapShapeGrid;
  HELD_OBJECT(work) = NULL;
  if (*list != 0xffff) {
  loop:
    if (held == Actor_GetById(*list)) {
      u8 *layer = &held->terrainId;
      u8 *cell = (u8 *)Ram_HeapSlots->mapWork->layers[*layer].cells;
      u8 *shape =
          grid + ((struct MapCell *)cell - (struct MapCell *)Ram_MapCellBuffer);
      s32 kind = MapGrid_GetTerrainKind(*layer, held->x, held->z);
      s32 height = Map_GetTerrainHeight(*layer, held->x, held->z);
      s32 index = Actor_GetCellIndex(held);
      s32 result = MapCollision_FindOrAddTag(*layer, kind, (height >> 19) - 4);

      shape += index;
      // The index goes on as the cell offset, four bytes a cell.
      index <<= 2;
      ((struct ShapeCell *)shape)->shape = result;
      cell += index;
      ((struct MapCell *)cell)->attributeB &= 63;
      cell += 0x200;
      ((struct MapCell *)cell)->attributeB &= 191;
      HELD_OBJECT(work) = held;
    }
    list++;
    if (*list != 0xffff)
      goto loop;
  }
}

DEFINE_HELD_ACTOR_RAISE_CELL(HeldActor_RaiseCell, MapGrid_GetTerrainKind,
                             Actor_GetCellIndex, MapCollision_FindOrAddTag)

struct SceneWork {
  u8 unk000[0x1ac];
  s32 unk1ac;
};

extern struct Trigger gMadraCatacombs_Triggers[];

void MadraCatacombs_SetUpRoomBelowBookshelf(void);

const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs4)
    return gMadraCatacombs_Room4Entrances;
  if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs5)
    return gMadraCatacombs_Room5Entrances;
  return gMadraCatacombs_Entrances;
}

const s16 *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_MadraCatacombs2)
    return gMadraCatacombs_LadderRoomRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs1)
    return gMadraCatacombs_Room1Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs3)
    return gMadraCatacombs_Room3Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs5)
    return gMadraCatacombs_Room5Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs6)
    return gMadraCatacombs_Room6Placements;
  return gMadraCatacombs_Placements;
}

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs1)
    return gMadraCatacombs_Room1Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs2)
    return gMadraCatacombs_Room2Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs3)
    return gMadraCatacombs_Room3Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs4)
    return gMadraCatacombs_Room4Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs5)
    return gMadraCatacombs_Room5Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs6)
    return gMadraCatacombs_Room6Triggers;
  return gMadraCatacombs_Triggers;
}

void Actor_LiftTargetBlock(const void *data);
void FlagBlock_ApplyFlags(const void *data);

void MadraCatacombs_TryLockedDoor(void) {
  struct EventWork15a *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)&MsgFieldDoorWontBudge, 1);
  work->flag = 1;
  Event_End();
}

DEFINE_ACTOR_DROP_AND_WAIT(Actor_DropAndWait)

/// The pillar Move pushes onto cell (40, 13) drops into the hole there, which
/// it fills for good (flag 0x8c8).
void MadraCatacombs_DropPillarIntoHole(void) {
  struct Actor *object = Actor_GetById(11);
  s32 z;

  if ((object->x >> 20) == 40) {
    z = object->z >> 20;
    if (z == 13) {
      Event_Start();
      Event_ResolvePendingActions(0);
      Actor_DropAndWait(object);
      MapGrid_CopyCellAttributes(41, 13, 2, 1, 39, z);
      Event_SetFlag(0x8c8);
      Event_End();
    }
  }
}

void MadraCatacombs_StartMovingPushedPillar(void) {
  Actor_LiftTargetBlock(gMadraCatacombs_PushedPillar);
}

/// The second pillar, moved onto cell (44, 26), stays there (flag 0x8ca).
void MadraCatacombs_FinishMovingPushedPillar(void) {
  struct Actor *object = Actor_GetById(12);

  Actor_PlaceMovedBlock();
  if ((object->x >> 20) == 44 && (object->z >> 20) == 26)
    Event_SetFlag(0x8ca);
}

void MadraCatacombs_FreezePuddle(void) {
  FlagBlock_ApplyFlags(gMadraCatacombs_Puddles);
}

void FieldCamera_WaitForArrival(void);
void MadraCatacombs_LightFlames(void);

/// The Ruin Key (item 459) opens the double door, which slides apart, and the
/// room beyond lights up.
void MadraCatacombs_UnlockWithRuinKey(void) {
  struct Actor *left = Actor_GetById(8);
  struct Actor *right = Actor_GetById(9);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(SE_STONE_RUMBLE);
  for (i = 0; i < 120; i++) {
    left->x += -0x1999;
    /* FAKEMATCH(signature): read and written as long, another alias set than
     * the left door's int store. */
    *(long *)&right->x += 0x1999;
    wait(1);
  }
  FieldCamera_SetSpeed(3.2);
  FieldCamera_MoveTo(160, CAMERA_KEEP, 240, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(160, CAMERA_KEEP, 186, 1);
  Event_PlaySound(SE_168);
  Actor_SetPosition(14, 136, 258);
  Actor_SetPosition(19, 184, 258);
  Actor_SetScript(14, gMadraCatacombs_FlameFlaresUp);
  Actor_SetScript(19, gMadraCatacombs_FlameFlaresUp);
  Event_Wait(40);
  Event_PlaySound(SE_168);
  Actor_SetPosition(13, 120, 226);
  Actor_SetPosition(18, 200, 226);
  Actor_SetScript(13, gMadraCatacombs_FlameFlaresUp);
  Actor_SetScript(18, gMadraCatacombs_FlameFlaresUp);
  Event_Wait(40);
  Event_PlaySound(SE_168);
  Actor_SetPosition(12, 120, 194);
  Actor_SetPosition(17, 200, 194);
  Actor_SetScript(12, gMadraCatacombs_FlameFlaresUp);
  Actor_SetScript(17, gMadraCatacombs_FlameFlaresUp);
  Event_Wait(60);
  Event_PlaySound(SE_SPLASH);
  Actor_SetPosition(11, 136, 162);
  Actor_SetPosition(16, 184, 162);
  Actor_SetPosition(10, 136, 138);
  Actor_SetPosition(15, 184, 138);
  Actor_SetScript(11, gMadraCatacombs_GreatFlameFlaresUp);
  Actor_SetScript(16, gMadraCatacombs_GreatFlameFlaresUp);
  Actor_SetScript(10, gMadraCatacombs_GreatFlameFlaresUp);
  Actor_SetScript(15, gMadraCatacombs_GreatFlameFlaresUp);
  Event_Wait(80);
  MadraCatacombs_LightFlames();
  PartyInventory_Remove(459);
  Event_SetFlag(0x8c9);
  Event_End();
}

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 flag;
};

void Actor_MoveToObject(s32 id, s32 value);
void Rope_Initialize(void);
void FlagBlock_PlaceAll(const void *data);
void Actor_PlaceBlocks(const void *data);
/// The climbable pillar settles where Move left it, its cells raised; on cell
/// (35, 17) it stays there (flag 0x9ef).
void MadraCatacombs_SettleClimbablePillar(struct Actor *object);
void MadraCatacombs_LeaveBlockedTunnel(void);

/// Reveal shows the push button by the closed doorway, pressed in once the door
/// has opened (flag 0x8cb); flag 0x203 holds while it shows.
void MadraCatacombs_RevealButton(void) {
  if (Event_GetFlag(0x8cb) != 0)
    MapGrid_CopyCellsTo(72, 63, 36, 38, 1, 2);
  else
    MapGrid_CopyCellsTo(71, 63, 36, 38, 1, 2);
  Event_SetFlag(FLAG_SCENE_TEMP_03);
}

void MadraCatacombs_HideButton(void) {
  MapGrid_CopyCellsTo(70, 63, 36, 38, 1, 2);
  Event_ClearFlag(FLAG_SCENE_TEMP_03);
}

void MadraCatacombs_HearEyesOfTruth(void) {
  if (Event_GetFlag(FLAG_SCENE_TEMP_03) != 0 && Event_GetFlag(0x8cb) == 0) {
    Event_Start();
    Event_ResolvePendingActions(1);
    Event_PlaySound(SE_RATTLE);
    MapGrid_CopyCellsTo(72, 63, 36, 38, 1, 2);
    Event_Wait(40);
    Event_PlaySound(SE_THUMP);
    MapGrid_CopyCellsTo(71, 66, 97, 39, 2, 2);
    wait(2);
    MapGrid_CopyCellsTo(73, 66, 97, 39, 2, 2);
    wait(2);
    MapGrid_CopyCellsTo(75, 66, 97, 39, 2, 2);
    MapGrid_CopyCellAttributes(33, 37, 2, 2, 33, 39);
    wait(1);
    Event_SetFlag(0x8cb);
    Event_End();
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldMessage_Show((s32)&MsgFieldEyesOfTruth, 1);
    Event_End();
  }
}

/// Tremor shakes the bookshelf; once the chest on it has rocked past its edge
/// it falls through to the room below (flag 0x8cc).
void MadraCatacombs_ShakeBookshelf(void) {
  struct Actor *object;
  s32 x;

  if (Event_GetFlag(0x8cc) == 0) {
    object = Actor_GetById(64);
    x = object->x;
    if (x < (186 << 16)) {
      Event_Start();
      Event_ResolvePendingActions(0);
      Event_PlaySound(SE_DUST_PUFF);
      object->motionFlags = 3;
      wait(10);
      Actor_SetPosition(64, 0, 0);
      Event_SetFlag(0x8cc);
      Event_End();
    }
  }
}

void MadraCatacombs_SettleClimbablePillar(struct Actor *object) {
  HeldActor_RaiseCell();
  if ((object->x >> 20) == 35 && (object->z >> 20) == 17)
    Event_SetFlag(0x9ef);
}

DEFINE_SCENE_RUN_IF_HELD(Scene_RunIfHeld, Leader_CanPushObjectAhead,
                         Field_FindNearbyTalkTarget,
                         MadraCatacombs_LowerHeldPillarCells,
                         gMadraCatacombs_ClimbablePillar,
                         Leader_PushObjectAhead,
                         MadraCatacombs_SettleClimbablePillar)

void MadraCatacombs_StartMovingClimbablePillar(void) {
  MadraCatacombs_LowerHeldPillarCells(gMadraCatacombs_ClimbablePillar);
}

/// A door opens, sliding stone (mode 2) or swinging (mode 1), and the leader
/// walks through it as MadraCatacombs_EnterDoorway does.
void MadraCatacombs_OpenDoorAndEnter(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void MadraCatacombs_FinishMovingClimbablePillar(void) {
  struct EventWorkCbc *work = Ram_HeapSlots->eventWork;

  if (work->active != 0)
    MadraCatacombs_SettleClimbablePillar(work->active);
}

/// The tablet room's flames, lit: actors 10 to 19 stand in two columns and
/// actors 20 to 29 each go to one of them.
void MadraCatacombs_LightFlames(void) {
  u32 i;
  /* FAKEMATCH(pin): the actor pinned to r6. */
  register s32 actor __asm__("r6");
  s32 target;

  Actor_SetPosition(14, 136, 258);
  Actor_SetPosition(19, 184, 258);
  Actor_SetPosition(13, 120, 226);
  Actor_SetPosition(18, 200, 226);
  Actor_SetPosition(12, 120, 194);
  Actor_SetPosition(17, 200, 194);
  Actor_SetPosition(11, 136, 162);
  Actor_SetPosition(16, 184, 162);
  Actor_SetPosition(10, 136, 138);
  Actor_SetPosition(15, 184, 138);
  target = 10;
  actor = 20;
  for (i = 0; i < 10; i++) {
    Actor_SetAnimation(actor, 0);
    Actor_MoveToObject(actor, target);
    target++;
    actor++;
  }
  MapGrid_CopyCellAttributes(0, 0, 2, 2, 9, 22);
  wait(1);
}

/// The chest on the bookshelf shakes with it, and each shake nudges it two
/// pixels west with a hop.
void MadraCatacombs_RockChestWithBookshelf(void) {
  struct Actor *lever = Actor_GetById(8);
  struct Actor *rock = Actor_GetById(64);
  struct AnimationRecord18 *animation = rock->sprite;

  animation->frame = ((struct AnimationRecord18 *)lever->sprite)->frame;
  if (Event_GetFlag(FLAG_SCENE_TEMP_04) != 0 && animation->frame == 0) {
    Event_PlaySound(SE_535);
    rock->x += PIXELS(-2);
    rock->velocityY = PIXELS(2);
    Event_ClearFlag(FLAG_SCENE_TEMP_04);
  }
  if (animation->frame != 0)
    Event_SetFlag(FLAG_SCENE_TEMP_04);
}

/// The leader steps to the middle of a doorway, walks in and leaves by the exit
/// the event work names.
void MadraCatacombs_EnterDoorway(void) {
  struct EventWork154 *work = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;
  s32 cell;

  object = Actor_GetById(party->currentOwner);
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetAnimation(party->currentOwner, 2);
  Actor_SetSpeed(party->currentOwner, 0.5);
  cell = object->x >> 16;
  Actor_SetMoveTarget(object, ((8 - cell % 16) << 16) + object->x, object->y,
                      object->z);
  FieldActor_WaitForMove(object);
  object->facing = FACING_NORTH;
  Actor_SetScript(party->currentOwner, gMadraCatacombs_WalkIntoDoorway);
  wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->returnEntrance);
}

void MadraCatacombs_OpenDoorAndEnter(s32 a, s32 b, s32 c, s32 d, s32 mode,
                                     s32 f) {
  struct EventWork154 *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  s32 cell;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (mode == 2)
    Event_PlaySound(SE_THUMP);
  else
    Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(a, b, c, d, mode, f);
  Event_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  if (mode == 1) {
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    cell = object->x >> 16;
    Actor_SetMoveTarget(object, ((8 - cell % 16) << 16) + object->x, object->y,
                        object->z);
    FieldActor_WaitForMove(object);
    object->facing = FACING_NORTH;
  }
  Actor_SetScript(gMainRoutineWork.currentOwner,
                  gMadraCatacombs_WalkIntoDoorway);
  wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->returnEntrance);
}

void MadraCatacombs_SlideOpenStoneDoor(void) {
  MadraCatacombs_OpenDoorAndEnter(78, 29, 78, 31, 2, 2);
}

void MadraCatacombs_OpenBookshelfRoomWestDoor(void) {
  MadraCatacombs_OpenDoorAndEnter(109, 1, 109, 3, 1, 2);
}

void MadraCatacombs_OpenBookshelfRoomEastDoor(void) {
  MadraCatacombs_OpenDoorAndEnter(118, 1, 118, 3, 1, 2);
}

void MadraCatacombs_OpenTabletRoomDoor(void) {
  MadraCatacombs_OpenDoorAndEnter(92, 29, 92, 31, 1, 2);
}

/// Arriving by a way the Champa have since blocked, the leader fades out and
/// leaves by exit n1.
#define DEFINE_LEAVE_BLOCKED_WAY(name, n1)                                     \
  void name(void) {                                                            \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);              \
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);     \
    Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 15);    \
    wait(1);                                                                   \
    Event_StartScreenTransition();                                             \
    Event_WaitTransitionFrames();                                              \
    Event_Wait(40);                                                            \
    Import_EventSetExitRequest(n1);                                            \
  }

DEFINE_LEAVE_BLOCKED_WAY(MadraCatacombs_LeaveBlockedVillage, 3)

DEFINE_LEAVE_BLOCKED_WAY(MadraCatacombs_LeaveBlockedTunnel, 8)

s32 Scene_Initialize(void) {
  ((struct SceneWork *)Ram_HeapSlots->eventWork)->unk1ac = 0x204;
  if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs1)
    MadraCatacombs_SetUpVillage();
  else if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs3)
    MadraCatacombs_SetUpChampaTunnel();
  else if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs4)
    MadraCatacombs_SetUpRoomBelowBookshelf();
  else if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs5)
    MadraCatacombs_SetUpBookshelfRoom();
  else if (gMainRoutineWork.scene == (s32)&SceneId_MadraCatacombs6)
    MadraCatacombs_SetUpTabletRoom();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void MadraCatacombs_SetUpVillage(void) {
  s32 a;
  s32 b;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *object;

  Event_ClearFlag(FLAG_SCENE_TEMP_03);
  Actor_SetSpritePriority(64, 3);
  Rope_Initialize();
  b = 0x102;
  a = 1;
  /* FAKEMATCH(barrier): a and b held at an empty asm. */
  __asm__("" : "+r"(a), "+r"(b));
  Rope_Place(a, b + 255, 9, 10);
  Actor_DisableById(11);
  if (Event_GetFlag(0x8c8) != 0) {
    MapGrid_CopyCellAttributes(41, 13, 2, 1, 39, 13);
    object = Actor_GetById(11);
    x = 162;
    z = 216;
    x <<= 18;
    y = 0;
    /* FAKEMATCH(barrier): x, y and z held at an empty asm, the second of the
     * pair above. */
    __asm__("" : "+r"(x), "+r"(y), "+r"(z));
    Actor_SetPositionAndResetMotion(object, x, y, z << 16);
  }
  if (Event_GetFlag(0x8ca) != 0)
    Actor_SetPosition(12, 712, 424);
  Actor_PlaceBlocks(gMadraCatacombs_PushedPillar);
  if (Event_GetFlag(0x9ef) != 0)
    Actor_SetPosition(8, 568, 280);
  Actor_GetById(8)->priorityFlags &= 0xfe;
  MadraCatacombs_RaisePillarCells(gMadraCatacombs_ClimbablePillar);
  FlagBlock_PlaceAll(gMadraCatacombs_Puddles);
  if (Event_GetFlag(0x8cb) != 0) {
    MapGrid_CopyCellsTo(75, 66, 97, 39, 2, 2);
    MapGrid_CopyCellAttributes(33, 37, 2, 2, 33, 39);
    wait(1);
  }
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT) != 0) {
    MapGrid_CopyCellsTo(82, 74, 116, 16, 5, 3);
    MapGrid_CopyCellAttributes(52, 11, 5, 1, 52, 18);
    if (gMainRoutineWork.entrance == 3)
      MadraCatacombs_LeaveBlockedVillage();
    else
      wait(10);
  }
}

void MadraCatacombs_SetUpChampaTunnel(void) {
  Rope_Initialize();
  Rope_Place(0, 0x200, 8, 9);
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT) != 0) {
    Actor_SetPosition(10, 0, 0);
    MapGrid_CopyCellsTo(68, 70, 113, 11, 5, 3);
    MapGrid_CopyCellsTo(50, 74, 50, 75, 2, 1);
    MapGrid_CopyCellAttributes(49, 9, 5, 1, 49, 13);
    if (gMainRoutineWork.entrance == 8)
      MadraCatacombs_LeaveBlockedTunnel();
    else
      wait(1);
  }
}

void MadraCatacombs_SetUpRoomBelowBookshelf(void) {
  if (Event_GetFlag(0x8cc) == 0)
    Actor_SetPosition(65, 0, 0);
}

void MadraCatacombs_SetUpBookshelfRoom(void) {
  struct Actor *object;

  if (Event_GetFlag(0x8cc) != 0) {
    Actor_SetPosition(64, 0, 0);
  } else {
    object = Actor_GetById(64);
    object->collisionFlags = 8;
    object->motionFlags = 2;
    object->y = PIXELS(32);
    object->terrainHeight = PIXELS(32);
    object->update = MadraCatacombs_RockChestWithBookshelf;
    Actor_GetById(8)->scaleY = 0xcccc;
  }
}

/// Mirrors actors 8 and 9 and puts them on the ground; once flag 0x8c9 is set,
/// each steps 12 pixels outward unless flag 0x109 is set too, and
/// MadraCatacombs_LightFlames runs.
void MadraCatacombs_SetUpTabletRoom(void) {
  struct Actor *left = Actor_GetById(8);
  struct Actor *right = Actor_GetById(9);

  left->scaleX = 0x14000;
  right->scaleX = -0x14000;
  left->motionFlags = 0;
  right->motionFlags = 0;
  left->y = 0;
  right->y = 0;
  if (Event_GetFlag(0x8c9) != 0) {
    if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
      left->x += PIXELS(-12);
      right->x += 0xc0000;
    }
    MadraCatacombs_LightFlames();
  }
}

// clang-format off
s32 gMadraCatacombs_WalkIntoDoorway[] = {                                                               
      SC_SET(SW_MOTION, 0),                                                    
      SC_ADD(SW_Z, -0x8000),                                                   
      SC_ADD(SW_SCALE_X, -1024),                                               
      SC_ADD(SW_SCALE_Y, -1024),                                               
      SC_WAIT(1),                                                              
      SC_LOOP(24, SC_TOP),                                                     
      SC_END,                                                                  
  };

u32 gMadraCatacombs_FlameFlaresUp[] = {
    SC_SET(SW_SCALE_X, 0x1000),
    SC_SET(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_END,
};

s32 gMadraCatacombs_GreatFlameFlaresUp[] = {
    SC_SET(SW_SCALE_X, 0x1000),
    SC_SET(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 0x8000),
    SC_ADD(SW_SCALE_Y, 0x4000),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, -0x2000),
    SC_ADD(SW_SCALE_Y, 0x6000),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_X, -0x2000),
    SC_ADD(SW_SCALE_Y, -0x4000),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(3)),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_END,
};

s16 gMadraCatacombs_PushedPillar[] = {
    12, -1,
};

s16 gMadraCatacombs_Puddles[] = {
    13, 514, -1,
};

s16 gMadraCatacombs_ClimbablePillar[] = {
    8, -1, 0,
};

struct SceneEntrance gMadraCatacombs_Entrances[] = {
    {0, -1, 172, 0, 149, 0x4000, 0, -1, -1, -1, -1, 0},
    {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

struct SceneEntrance gMadraCatacombs_Room4Entrances[] = {
    {3, -1, 512, 0, 224, 0xc000, 0, 392, 40, 632, 280, 0},
    {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

struct SceneEntrance gMadraCatacombs_Room5Entrances[] = {
    {5, -1, 728, 0, 88, 0x4000, 0, 680, 32, 920, 192, 0},
    {6, -1, 872, 0, 88, 0x4000, 0, 680, 32, 920, 192, 0},
    {7, -1, 872, 0, 120, 0, 0, 680, 32, 920, 192, 0},
    {8, -1, 792, 0, 152, 0xc000, 0, 680, 32, 920, 192, 0},
    {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

s16 gMadraCatacombs_LadderRoomRegions[] = {
    112, 48, 144, 128, 64, 160, -1, 1,
    336, 48, 144, 352, 64, 160, -1, 3,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(MadraCatacombs1),
    SCENE_EXIT(1, MadraCatacombs5, 8),
    SCENE_EXIT(2, MadraCatacombs4, 7),
    SCENE_EXIT(3, MadraCatacombs3, 1),
    SCENE_EXIT(4, MadraCatacombs4, 13),
    SCENE_EXIT(5, MadraCatacombs3, 6),
    SCENE_EXITS(MadraCatacombs2),
    SCENE_EXIT(1, Madra, 15),
    SCENE_EXIT(2, MadraCatacombs3, 7),
    SCENE_EXIT(3, Madra, 16),
    SCENE_EXIT(4, MadraCatacombs3, 4),
    SCENE_EXITS(MadraCatacombs3),
    SCENE_EXIT(1, MadraCatacombs1, 3),
    SCENE_EXIT(2, MadraCatacombs3, 3),
    SCENE_EXIT(3, MadraCatacombs3, 2),
    SCENE_EXIT(4, MadraCatacombs2, 4),
    SCENE_EXIT(5, MadraCatacombs3, 8),
    SCENE_EXIT(6, MadraCatacombs1, 5),
    SCENE_EXIT(7, MadraCatacombs2, 2),
    SCENE_EXIT(8, MadraCatacombs3, 5),
    SCENE_EXITS(MadraCatacombs4),
    SCENE_EXIT(1, MadraCatacombs5, 1),
    SCENE_EXIT(2, MadraCatacombs4, 11),
    SCENE_EXIT(3, MadraCatacombs4, 9),
    SCENE_EXIT(4, MadraCatacombs5, 2),
    SCENE_EXIT(5, MadraCatacombs6, 4),
    SCENE_EXIT(6, MadraCatacombs6, 1),
    SCENE_EXIT(7, MadraCatacombs1, 2),
    SCENE_EXIT(8, MadraCatacombs4, 12),
    SCENE_EXIT(9, MadraCatacombs4, 3),
    SCENE_EXIT(10, MadraCatacombs5, 7),
    SCENE_EXIT(11, MadraCatacombs4, 2),
    SCENE_EXIT(12, MadraCatacombs4, 8),
    SCENE_EXIT(13, MadraCatacombs1, 4),
    SCENE_EXITS(MadraCatacombs5),
    SCENE_EXIT(1, MadraCatacombs4, 1),
    SCENE_EXIT(2, MadraCatacombs4, 4),
    SCENE_EXIT(3, MadraCatacombs5, 5),
    SCENE_EXIT(4, MadraCatacombs5, 6),
    SCENE_EXIT(5, MadraCatacombs5, 3),
    SCENE_EXIT(6, MadraCatacombs5, 4),
    SCENE_EXIT(7, MadraCatacombs4, 10),
    SCENE_EXIT(8, MadraCatacombs1, 1),
    SCENE_EXITS(MadraCatacombs6),
    SCENE_EXIT(1, MadraCatacombs4, 6),
    SCENE_EXIT(2, MadraCatacombs6, 3),
    SCENE_EXIT(3, MadraCatacombs6, 2),
    SCENE_EXIT(4, MadraCatacombs4, 5),
    SCENE_EXITS_END,
};

struct ActorPlacement gMadraCatacombs_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gMadraCatacombs_Room1Placements[] = {
    ACTOR_PLACEMENT(250, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMadraCatacombs_Room3Placements[] = {
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(71, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 794, 0, 236, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMadraCatacombs_Room5Placements[] = {
    ACTOR_PLACEMENT(679, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 202, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMadraCatacombs_Room6Placements[] = {
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, 7, 150, 0, 366, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(330, CONDITION_ALWAYS, 7, 170, 0, 366, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gMadraCatacombs_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gMadraCatacombs_Room1Triggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(MadraCatacombs_EnterDoorway)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(MadraCatacombs_EnterDoorway)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(MadraCatacombs_EnterDoorway)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(MadraCatacombs_EnterDoorway)},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 9, 0x201, 0},
    {TRIGGER_PUSHED, 10, 0x8c8, EVENT(MadraCatacombs_DropPillarIntoHole)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x8c8, EVENT(MadraCatacombs_DropPillarIntoHole)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12, 0x8ca, EVENT(MadraCatacombs_StartMovingPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x8ca, EVENT(MadraCatacombs_FinishMovingPushedPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 11, 0x9ef, EVENT(Scene_RunIfHeld)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x9ef, EVENT(MadraCatacombs_StartMovingClimbablePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9ef, EVENT(MadraCatacombs_FinishMovingClimbablePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, 0x202, EVENT(MadraCatacombs_FreezePuddle)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(MadraCatacombs_RevealButton)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(MadraCatacombs_HideButton)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(MadraCatacombs_HearEyesOfTruth)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMadraCatacombs_Room2Triggers[] = {
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gMadraCatacombs_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x200, 0},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgFieldCaveFoundByChampa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgFieldWhoDugCave)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMadraCatacombs_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS,
     EVENT(MadraCatacombs_SlideOpenStoneDoor)},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_TABLE_END},
};

struct Trigger gMadraCatacombs_Room5Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
     EVENT(MadraCatacombs_OpenBookshelfRoomWestDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
     EVENT(MadraCatacombs_OpenBookshelfRoomEastDoor)},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), 8, CONDITION_ALWAYS,
     EVENT(MadraCatacombs_ShakeBookshelf)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMadraCatacombs_Room6Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(MadraCatacombs_OpenTabletRoomDoor)},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {USE_ITEM(ITEM_RUIN_KEY), 10, CONDITION_ALWAYS,
     EVENT(MadraCatacombs_UnlockWithRuinKey)},
    {TRIGGER_EXAMINE, 10, 0x8c9, EVENT(MadraCatacombs_TryLockedDoor)},
    {TRIGGER_TABLE_END},
};
