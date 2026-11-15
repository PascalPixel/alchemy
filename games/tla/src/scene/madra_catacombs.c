/// @file
/// The Madra Catacombs, under the town: a ladder room entered from Madra's two
/// ladders, the tunnel the Champa dug, the deserted village with its pillars,
/// rope, puddle and the door Reveal opens, the room whose bookshelf Tremor
/// shakes until a chest falls into the room below, and the tablet room whose
/// red double door the Ruin Key opens, lighting its flames before Moloch's
/// tablet. Once the Black Crystal is stolen (flag 0x8ab) the Champa's way in is
/// blocked.

#include "flags.h"
#include "sprite_ids.h"
#include "item_ids.h"
#include "shared/tables.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/map_grid.h"
#include "system/iwram_call.h"
#include "field/event_inline.h"
#include "field/field_sprite.h"
#include "field/field.h"
#include "field/scene_tables.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "field/actor_script.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "text/msg_ids.h"

/// The ropes Lash ties, the tunnel's and the village's, which stay tied until
/// the party leaves.
#define FLAG_TUNNEL_ROPE_TIED FLAG_SCENE_TEMP_00
#define FLAG_VILLAGE_ROPE_TIED FLAG_SCENE_TEMP_01

/// Set once the pillar Move pushed onto cell (40, 13) has dropped into the
/// deserted village's hole and filled it.
#define FLAG_MADRA_CATACOMBS_HOLE_FILLED 0x8c8
/// Set once the Ruin Key has opened the tablet room's double door; its flames
/// burn from then on.
#define FLAG_MADRA_CATACOMBS_TABLET_DOOR_OPEN 0x8c9
/// Set once Move has left the village's second pillar on cell (44, 26).
#define FLAG_MADRA_CATACOMBS_PILLAR_PLACED 0x8ca
/// Set once the button Reveal shows has opened the doorway into the village.
#define FLAG_MADRA_CATACOMBS_DOORWAY_OPEN 0x8cb
/// Set once Tremor has shaken the chest off the bookshelf into the room below.
#define FLAG_MADRA_CATACOMBS_CHEST_FELL 0x8cc
/// Set once Move has left the climbable pillar on cell (35, 17).
#define FLAG_MADRA_CATACOMBS_CLIMBABLE_PILLAR_PLACED 0x9ef
/// Set while Reveal shows the push button by the closed doorway.
#define FLAG_MADRA_CATACOMBS_BUTTON_REVEALED FLAG_SCENE_TEMP_03

/// The deserted village's actors that its events move.
enum MadraCatacombsVillageActorId {
  /// The pillar Move carries onto the upper level to climb by.
  ACTOR_CLIMBABLE_PILLAR = ACTOR_FIRST_PLACED,
  /// The posts at the two ends of the rope Lash makes taut.
  ACTOR_VILLAGE_ROPE_LOW_POST,
  ACTOR_VILLAGE_ROPE_HIGH_POST,
  /// The pillar Move pushes into the hole.
  ACTOR_HOLE_PILLAR = ACTOR_FIRST_PLACED + 3,
  /// The pillar Move pushes onto cell (44, 26).
  ACTOR_PUSHED_PILLAR,
  /// The puddle Frost freezes into a pillar.
  ACTOR_VILLAGE_PUDDLE,
  /// The chest by the rope, drawn over the ledge.
  ACTOR_VILLAGE_CHEST = 64
};

/// The tunnel the Champa dug (MadraCatacombsChampaTunnel), by the rows of its
/// table.
enum MadraCatacombsTunnelActorId {
  /// The posts at the two ends of the tunnel's rope.
  ACTOR_TUNNEL_ROPE_EAST_POST = ACTOR_FIRST_PLACED,
  ACTOR_TUNNEL_ROPE_WEST_POST,
  /// A Madran exploring the tunnel until Briggs is caught: "I can't believe
  /// the Champa just stumbled onto this cave accidentally..."
  ACTOR_TUNNEL_EXPLORER
};

/// The tablet room's actors: the two halves of the red double door, the five
/// flames that flare up down each side as it opens, nearest the door last, and
/// the ten flames that burn on where they flared.
enum MadraCatacombsTabletRoomActorId {
  ACTOR_LEFT_DOOR = ACTOR_FIRST_PLACED,
  ACTOR_RIGHT_DOOR,
  ACTOR_FIRST_WEST_FLARE,
  ACTOR_FIRST_EAST_FLARE = ACTOR_FIRST_WEST_FLARE + 5,
  ACTOR_FIRST_FLAME = ACTOR_FIRST_EAST_FLARE + 5
};

/// The bookshelf room's bookshelf, and the chest on it that Tremor shakes off;
/// the room below places the fallen chest.
enum MadraCatacombsBookshelfActorId {
  ACTOR_BOOKSHELF = ACTOR_FIRST_PLACED,
  ACTOR_SHAKEN_CHEST = 64,
  ACTOR_FALLEN_CHEST = 65
};

struct Actor;

s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

extern struct SceneEntrance gMadraCatacombs_Entrances[];
extern struct SceneEntrance gMadraCatacombs_StoneDoorEntrances[];

extern struct SceneEntrance gMadraCatacombs_BookshelfEntrances[];

extern s16 gMadraCatacombs_LadderRoomRegions[];

extern struct ActorPlacement gMadraCatacombs_Placements[];

extern struct ActorPlacement gMadraCatacombs_EntrancePlacements[];

extern struct ActorPlacement gMadraCatacombs_ChampaTunnelPlacements[];

extern struct ActorPlacement gMadraCatacombs_BookshelfPlacements[];
extern struct ActorPlacement gMadraCatacombs_TabletPlacements[];
extern struct Trigger gMadraCatacombs_EntranceTriggers[];
extern struct Trigger gMadraCatacombs_LaddersTriggers[];
extern struct Trigger gMadraCatacombs_ChampaTunnelTriggers[];
extern struct Trigger gMadraCatacombs_StoneDoorTriggers[];
extern struct Trigger gMadraCatacombs_BookshelfTriggers[];
extern struct Trigger gMadraCatacombs_TabletTriggers[];

void MadraCatacombs_SetUpVillage(void);

void MadraCatacombs_SetUpChampaTunnel(void);

void MadraCatacombs_SetUpBookshelfRoom(void);

void MadraCatacombs_SetUpTabletRoom(void);

extern s16 gMadraCatacombs_PushedPillar[];
extern s16 gMadraCatacombs_Puddles[];

extern s32 gMadraCatacombs_FlameFlaresUp[];

extern s32 gMadraCatacombs_GreatFlameFlaresUp[];
extern s16 gMadraCatacombs_ClimbablePillar[];
extern s32 gMadraCatacombs_WalkIntoDoorway[];

void Actor_DisableById(s32 id);

s32 Field_FindNearbyTalkTarget(s32 owner);

s32 Leader_CanPushObjectAhead(void);

void MadraCatacombs_RockChestWithBookshelf(void);

void MadraCatacombs_LeaveBlockedVillage(void);

void Rope_Place(s32 a, s32 b, s32 c, s32 d);

struct Actor *Leader_FindActorAhead(void);
void Actor_PlaceMovedBlock(void);
void Leader_PushObjectAhead(void);

s32 Actor_GetCellIndex(struct Actor *object);

// Raises each listed actor's cells: the shape grid and the cell flags under it,
// then lifts the leader if the new terrain is above the leader.
#include "shared/pillars_raise.h"

#include "shared/pillars_lower_held_cells.h"

#include "shared/held_actor_raise_cell.h"

extern struct Trigger gMadraCatacombs_Triggers[];

void MadraCatacombs_SetUpRoomBelowBookshelf(void);

const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_STONE_DOOR)
    return gMadraCatacombs_StoneDoorEntrances;
  if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
    return gMadraCatacombs_BookshelfEntrances;
  return gMadraCatacombs_Entrances;
}

const s16 *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_MADRA_CATACOMBS_LADDERS)
    return gMadraCatacombs_LadderRoomRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_ENTRANCE)
    return gMadraCatacombs_EntrancePlacements;
  if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_CHAMPA_TUNNEL)
    return gMadraCatacombs_ChampaTunnelPlacements;
  if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
    return gMadraCatacombs_BookshelfPlacements;
  if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_TABLET)
    return gMadraCatacombs_TabletPlacements;
  return gMadraCatacombs_Placements;
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_ENTRANCE)
    return gMadraCatacombs_EntranceTriggers;
  if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_LADDERS)
    return gMadraCatacombs_LaddersTriggers;
  if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_CHAMPA_TUNNEL)
    return gMadraCatacombs_ChampaTunnelTriggers;
  if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_STONE_DOOR)
    return gMadraCatacombs_StoneDoorTriggers;
  if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
    return gMadraCatacombs_BookshelfTriggers;
  if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_TABLET)
    return gMadraCatacombs_TabletTriggers;
  return gMadraCatacombs_Triggers;
}

void Actor_LiftTargetBlock(const void *data);
void FlagBlock_ApplyFlags(const void *data);

void MadraCatacombs_TryLockedDoor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  // "The door won't budge. ...Must be locked."
  FieldMessage_Show((s32)&MsgFieldDoorWontBudge, 1);
  work->fieldMenu = 1;
  Event_End();
}

#include "shared/actor_drop_and_wait.h"

/// The pillar Move pushes onto cell (40, 13) drops into the hole there, which
/// it fills for good .
void MadraCatacombs_DropPillarIntoHole(void) {
  struct Actor *object = Actor_GetById(ACTOR_HOLE_PILLAR);
  s32 z;

  if (FIELD_CELL(object->x.fixed) == 40) {
    z = FIELD_CELL(object->z.fixed);
    if (z == 13) {
      Event_Start();
      Event_ResolvePendingActions(0);
      Actor_DropAndWait(object);
      MapGrid_CopyCellAttributes(41, 13, 2, 1, 39, z);
      Event_SetFlag(FLAG_MADRA_CATACOMBS_HOLE_FILLED);
      Event_End();
    }
  }
}

void MadraCatacombs_StartMovingPushedPillar(void) {
  Actor_LiftTargetBlock(gMadraCatacombs_PushedPillar);
}

/// The second pillar, moved onto cell (44, 26), stays there .
void MadraCatacombs_FinishMovingPushedPillar(void) {
  struct Actor *object = Actor_GetById(ACTOR_PUSHED_PILLAR);

  Actor_PlaceMovedBlock();
  if (FIELD_CELL(object->x.fixed) == 44 && FIELD_CELL(object->z.fixed) == 26)
    Event_SetFlag(FLAG_MADRA_CATACOMBS_PILLAR_PLACED);
}

void MadraCatacombs_FreezePuddle(void) {
  FlagBlock_ApplyFlags(gMadraCatacombs_Puddles);
}

void FieldCamera_WaitForArrival(void);
void MadraCatacombs_LightFlames(void);

/// The Ruin Key opens the double door, which slides apart, and the
/// room beyond lights up.
void MadraCatacombs_UnlockWithRuinKey(void) {
  struct Actor *left = Actor_GetById(ACTOR_LEFT_DOOR);
  struct Actor *right = Actor_GetById(ACTOR_RIGHT_DOOR);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(SE_STONE_RUMBLE);
  for (i = 0; i < 120; i++) {
    left->x.fixed += -(px(1) / 10);
    /* FAKEMATCH(signature): read and written as long, another alias set than
     * the left door's int store. */
    *(long *)&right->x.fixed += px(1) / 10;
    wait(1);
  }
  FieldCamera_SetSpeed(3.2);
  FieldCamera_MoveTo(160, CAMERA_KEEP, 240, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(160, CAMERA_KEEP, 186, 1);
  Event_PlaySound(SE_UNLOCK);
  Actor_SetPosition(ACTOR_FIRST_WEST_FLARE + 4, 136, 258);
  Actor_SetPosition(ACTOR_FIRST_EAST_FLARE + 4, 184, 258);
  Actor_SetScript(ACTOR_FIRST_WEST_FLARE + 4, gMadraCatacombs_FlameFlaresUp);
  Actor_SetScript(ACTOR_FIRST_EAST_FLARE + 4, gMadraCatacombs_FlameFlaresUp);
  Event_Wait(40);
  Event_PlaySound(SE_UNLOCK);
  Actor_SetPosition(ACTOR_FIRST_WEST_FLARE + 3, 120, 226);
  Actor_SetPosition(ACTOR_FIRST_EAST_FLARE + 3, 200, 226);
  Actor_SetScript(ACTOR_FIRST_WEST_FLARE + 3, gMadraCatacombs_FlameFlaresUp);
  Actor_SetScript(ACTOR_FIRST_EAST_FLARE + 3, gMadraCatacombs_FlameFlaresUp);
  Event_Wait(40);
  Event_PlaySound(SE_UNLOCK);
  Actor_SetPosition(ACTOR_FIRST_WEST_FLARE + 2, 120, 194);
  Actor_SetPosition(ACTOR_FIRST_EAST_FLARE + 2, 200, 194);
  Actor_SetScript(ACTOR_FIRST_WEST_FLARE + 2, gMadraCatacombs_FlameFlaresUp);
  Actor_SetScript(ACTOR_FIRST_EAST_FLARE + 2, gMadraCatacombs_FlameFlaresUp);
  Event_Wait(60);
  Event_PlaySound(SE_SPLASH);
  Actor_SetPosition(ACTOR_FIRST_WEST_FLARE + 1, 136, 162);
  Actor_SetPosition(ACTOR_FIRST_EAST_FLARE + 1, 184, 162);
  Actor_SetPosition(ACTOR_FIRST_WEST_FLARE, 136, 138);
  Actor_SetPosition(ACTOR_FIRST_EAST_FLARE, 184, 138);
  Actor_SetScript(ACTOR_FIRST_WEST_FLARE + 1,
                  gMadraCatacombs_GreatFlameFlaresUp);
  Actor_SetScript(ACTOR_FIRST_EAST_FLARE + 1,
                  gMadraCatacombs_GreatFlameFlaresUp);
  Actor_SetScript(ACTOR_FIRST_WEST_FLARE, gMadraCatacombs_GreatFlameFlaresUp);
  Actor_SetScript(ACTOR_FIRST_EAST_FLARE, gMadraCatacombs_GreatFlameFlaresUp);
  Event_Wait(80);
  MadraCatacombs_LightFlames();
  PartyInventory_Remove(ITEM_RUIN_KEY);
  Event_SetFlag(FLAG_MADRA_CATACOMBS_TABLET_DOOR_OPEN);
  Event_End();
}

void Actor_MoveToObject(s32 id, s32 value);
void Rope_Initialize(void);
void FlagBlock_PlaceAll(const void *data);
void Actor_PlaceBlocks(const void *data);
/// The climbable pillar settles where Move left it, its cells raised; on cell
/// (35, 17) it stays there .
void MadraCatacombs_SettleClimbablePillar(struct Actor *object);
void MadraCatacombs_LeaveBlockedTunnel(void);

/// Reveal shows the push button by the closed doorway, pressed in once the door
/// has opened.
void MadraCatacombs_RevealButton(void) {
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_DOORWAY_OPEN) != 0)
    MapGrid_CopyCellsTo(72, 63, 36, 38, 1, 2);
  else
    MapGrid_CopyCellsTo(71, 63, 36, 38, 1, 2);
  Event_SetFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED);
}

void MadraCatacombs_HideButton(void) {
  MapGrid_CopyCellsTo(70, 63, 36, 38, 1, 2);
  Event_ClearFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED);
}

void MadraCatacombs_HearEyesOfTruth(void) {
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED) != 0 &&
      Event_GetFlag(FLAG_MADRA_CATACOMBS_DOORWAY_OPEN) == 0) {
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
    Event_SetFlag(FLAG_MADRA_CATACOMBS_DOORWAY_OPEN);
    Event_End();
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    // "Look upon me with eyes of truth... Then the door will open..."
    FieldMessage_Show((s32)&MsgFieldEyesOfTruth, 1);
    Event_End();
  }
}

/// Tremor shakes the bookshelf; once the chest on it has rocked past its edge
/// it falls through to the room below.
void MadraCatacombs_ShakeBookshelf(void) {
  struct Actor *object;
  s32 x;

  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL) == 0) {
    object = Actor_GetById(ACTOR_SHAKEN_CHEST);
    x = object->x.fixed;
    if (x < (px(186))) {
      Event_Start();
      Event_ResolvePendingActions(0);
      Event_PlaySound(SE_DUST_PUFF);
      object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      wait(10);
      Actor_SetPosition(ACTOR_SHAKEN_CHEST, 0, 0);
      Event_SetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL);
      Event_End();
    }
  }
}

void MadraCatacombs_SettleClimbablePillar(struct Actor *object) {
  HeldActor_RaiseCell();
  if (FIELD_CELL(object->x.fixed) == 35 && FIELD_CELL(object->z.fixed) == 17)
    Event_SetFlag(FLAG_MADRA_CATACOMBS_CLIMBABLE_PILLAR_PLACED);
}

/// Walking into the climbable pillar while holding it with Move pushes it a
/// cell on.
void MadraCatacombs_PushClimbablePillar(void) {
  struct Actor *object;

  if (Leader_CanPushObjectAhead() != 0) {
    object = Actor_GetById(
        Field_FindNearbyTalkTarget(gMainRoutineWork.leaderActorId));
    Pillars_LowerHeldCells(gMadraCatacombs_ClimbablePillar);
    Leader_PushObjectAhead();
    MadraCatacombs_SettleClimbablePillar(object);
  }
}

void MadraCatacombs_StartMovingClimbablePillar(void) {
  Pillars_LowerHeldCells(gMadraCatacombs_ClimbablePillar);
}

/// A door opens, sliding stone (mode 2) or swinging (mode 1), and the leader
/// walks through it as MadraCatacombs_EnterDoorway does.
void MadraCatacombs_OpenDoorAndEnter(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void MadraCatacombs_FinishMovingClimbablePillar(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (work->movedBlock != 0)
    MadraCatacombs_SettleClimbablePillar(work->movedBlock);
}

/// The tablet room's flames, lit: actors 10 to 19 stand in two columns and
/// actors 20 to 29 each go to one of them.
void MadraCatacombs_LightFlames(void) {
  u32 i;
  /* FAKEMATCH(pin): the actor pinned to r6. */
  register s32 actor __asm__("r6");
  s32 target;

  Actor_SetPosition(ACTOR_FIRST_WEST_FLARE + 4, 136, 258);
  Actor_SetPosition(ACTOR_FIRST_EAST_FLARE + 4, 184, 258);
  Actor_SetPosition(ACTOR_FIRST_WEST_FLARE + 3, 120, 226);
  Actor_SetPosition(ACTOR_FIRST_EAST_FLARE + 3, 200, 226);
  Actor_SetPosition(ACTOR_FIRST_WEST_FLARE + 2, 120, 194);
  Actor_SetPosition(ACTOR_FIRST_EAST_FLARE + 2, 200, 194);
  Actor_SetPosition(ACTOR_FIRST_WEST_FLARE + 1, 136, 162);
  Actor_SetPosition(ACTOR_FIRST_EAST_FLARE + 1, 184, 162);
  Actor_SetPosition(ACTOR_FIRST_WEST_FLARE, 136, 138);
  Actor_SetPosition(ACTOR_FIRST_EAST_FLARE, 184, 138);
  target = ACTOR_FIRST_WEST_FLARE;
  actor = ACTOR_FIRST_FLAME;
  for (i = 0; i < 10; i++) {
    Actor_SetAnimation(actor, 0);
    Actor_MoveToObject(actor, target);
    target++;
    actor++;
  }
  MapGrid_CopyCellAttributes(0, 0, 2, 2, 9, 22);
  wait(1);
}

/// The chest on the bookshelf tilts as the bookshelf rocks, and each time the
/// bookshelf comes upright again it nudges the chest two pixels west with a
/// hop.
void MadraCatacombs_RockChestWithBookshelf(void) {
  struct Actor *bookshelf = Actor_GetById(ACTOR_BOOKSHELF);
  struct Actor *chest = Actor_GetById(ACTOR_SHAKEN_CHEST);
  struct Sprite *sprite = chest->sprite;

  sprite->rotation = ((struct Sprite *)bookshelf->sprite)->rotation;
  if (Event_GetFlag(FLAG_SCENE_TEMP_04) != 0 && sprite->rotation == 0) {
    Event_PlaySound(SE_BOOKSHELF_SLIDE);
    chest->x.fixed += px(-2);
    chest->velocityY = px(2);
    Event_ClearFlag(FLAG_SCENE_TEMP_04);
  }
  if (sprite->rotation != 0)
    Event_SetFlag(FLAG_SCENE_TEMP_04);
}

/// The leader steps to the middle of a doorway, walks in and leaves by the exit
/// the event work names.
void MadraCatacombs_EnterDoorway(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;
  s32 cell;

  object = Actor_GetById(party->leaderActorId);
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetAnimation(party->leaderActorId, ANIM_WALK);
  Actor_SetSpeed(party->leaderActorId, 0.5);
  cell = FIELD_PIXEL(object->x.fixed);
  Actor_SetMoveTarget(object,
                      px(8 - cell % MAPGRID_CELL_PIXELS) + object->x.fixed,
                      object->y.fixed, object->z.fixed);
  FieldActor_WaitForMove(object);
  object->facing = FACING_NORTH;
  Actor_SetScript(party->leaderActorId, gMadraCatacombs_WalkIntoDoorway);
  wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->stepTrigger);
}

/// A door opens, its open picture copied over it from (sourceX, sourceZ), and
/// the leader walks through: a stone door two cells wide slides with a thump, a
/// door one cell wide opens as the leader steps to its middle.
void MadraCatacombs_OpenDoorAndEnter(s32 sourceX, s32 sourceZ, s32 x, s32 z,
                                     s32 width, s32 height) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *object;
  s32 cell;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (width == 2)
    Event_PlaySound(SE_THUMP);
  else
    Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(sourceX, sourceZ, x, z, width, height);
  Event_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  if (width == 1) {
    object = Actor_GetById(gMainRoutineWork.leaderActorId);
    cell = FIELD_PIXEL(object->x.fixed);
    Actor_SetMoveTarget(object,
                        px(8 - cell % MAPGRID_CELL_PIXELS) + object->x.fixed,
                        object->y.fixed, object->z.fixed);
    FieldActor_WaitForMove(object);
    object->facing = FACING_NORTH;
  }
  Actor_SetScript(gMainRoutineWork.leaderActorId,
                  gMadraCatacombs_WalkIntoDoorway);
  wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->stepTrigger);
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
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);    \
    Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId),        \
                         SPRITE_COLOR_HIDDEN);                                 \
    wait(1);                                                                   \
    Event_StartScreenTransition();                                             \
    Event_WaitTransitionFrames();                                              \
    Event_Wait(40);                                                            \
    Import_EventSetExitRequest(n1);                                            \
  }

DEFINE_LEAVE_BLOCKED_WAY(MadraCatacombs_LeaveBlockedVillage, 3)

DEFINE_LEAVE_BLOCKED_WAY(MadraCatacombs_LeaveBlockedTunnel, 8)

s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_ENTRANCE)
    MadraCatacombs_SetUpVillage();
  else if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_CHAMPA_TUNNEL)
    MadraCatacombs_SetUpChampaTunnel();
  else if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_STONE_DOOR)
    MadraCatacombs_SetUpRoomBelowBookshelf();
  else if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
    MadraCatacombs_SetUpBookshelfRoom();
  else if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_TABLET)
    MadraCatacombs_SetUpTabletRoom();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void MadraCatacombs_SetUpVillage(void) {
  s32 a;
  s32 b;

  Event_ClearFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED);
  Actor_SetSpritePriority(ACTOR_VILLAGE_CHEST, 3);
  Rope_Initialize();
  b = FLAG_VILLAGE_ROPE_TIED - 255;
  a = 1;
  /* FAKEMATCH(barrier): a and b held at an empty asm. */
  __asm__("" : "+r"(a), "+r"(b));
  Rope_Place(a, b + 255, ACTOR_VILLAGE_ROPE_LOW_POST,
             ACTOR_VILLAGE_ROPE_HIGH_POST);
  Actor_DisableById(ACTOR_HOLE_PILLAR);
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_HOLE_FILLED) != 0) {
    MapGrid_CopyCellAttributes(41, 13, 2, 1, 39, 13);
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_HOLE_PILLAR),
                                         px(648), 0, px(216));
  }
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_PILLAR_PLACED) != 0)
    Actor_SetPosition(ACTOR_PUSHED_PILLAR, 712, 424);
  Actor_PlaceBlocks(gMadraCatacombs_PushedPillar);
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_CLIMBABLE_PILLAR_PLACED) != 0)
    Actor_SetPosition(ACTOR_CLIMBABLE_PILLAR, 568, 280);
  Actor_GetById(ACTOR_CLIMBABLE_PILLAR)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Pillars_Raise(gMadraCatacombs_ClimbablePillar);
  FlagBlock_PlaceAll(gMadraCatacombs_Puddles);
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_DOORWAY_OPEN) != 0) {
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
  Rope_Place(0, FLAG_TUNNEL_ROPE_TIED, ACTOR_TUNNEL_ROPE_EAST_POST,
             ACTOR_TUNNEL_ROPE_WEST_POST);
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT) != 0) {
    Actor_SetPosition(ACTOR_TUNNEL_EXPLORER, 0, 0);
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
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL) == 0)
    Actor_SetPosition(ACTOR_FALLEN_CHEST, 0, 0);
}

void MadraCatacombs_SetUpBookshelfRoom(void) {
  struct Actor *object;

  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL) != 0) {
    Actor_SetPosition(ACTOR_SHAKEN_CHEST, 0, 0);
  } else {
    object = Actor_GetById(ACTOR_SHAKEN_CHEST);
    object->collisionFlags = ACTOR_NOT_TARGETED;
    object->motionFlags = ACTOR_FALLS;
    object->y.fixed = px(32);
    object->groundHeight = px(32);
    object->update = MadraCatacombs_RockChestWithBookshelf;
    Actor_GetById(ACTOR_BOOKSHELF)->scaleY = Q16_ONE * 4 / 5;
  }
}

/// Mirrors the door's right half and stands both halves on the ground; once
/// the Ruin Key has opened it they stand 12 pixels apart (already so when the
/// party comes back from a battle) and the flames burn.
void MadraCatacombs_SetUpTabletRoom(void) {
  struct Actor *left = Actor_GetById(ACTOR_LEFT_DOOR);
  struct Actor *right = Actor_GetById(ACTOR_RIGHT_DOOR);

  left->scaleX = Q16_ONE * 5 / 4;
  right->scaleX = -Q16_ONE * 5 / 4;
  left->motionFlags = 0;
  right->motionFlags = 0;
  left->y.fixed = 0;
  right->y.fixed = 0;
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_TABLET_DOOR_OPEN) != 0) {
    if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
      left->x.fixed += px(-12);
      right->x.fixed += px(12);
    }
    MadraCatacombs_LightFlames();
  }
}

// clang-format off
s32 gMadraCatacombs_WalkIntoDoorway[] = {                                                               
      SC_SET(SW_MOTION, 0),                                                    
      SC_ADD(SW_Z, -0x8000),                                                   
      SC_ADD(SW_SCALE_X, -Q16_ONE / 64),                                               
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 64),                                               
      SC_WAIT(1),                                                              
      SC_LOOP(24, SC_TOP),                                                     
      SC_END,                                                                  
  };

s32 gMadraCatacombs_FlameFlaresUp[] = {
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
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gMadraCatacombs_StoneDoorEntrances[] = {
    {3, CONDITION_ALWAYS, 512, 0, 224, FACING_NORTH, 0, 392, 40, 632, 280, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gMadraCatacombs_BookshelfEntrances[] = {
    {5, CONDITION_ALWAYS, 728, 0, 88, FACING_SOUTH, 0, 680, 32, 920, 192, 0},
    {6, CONDITION_ALWAYS, 872, 0, 88, FACING_SOUTH, 0, 680, 32, 920, 192, 0},
    {7, CONDITION_ALWAYS, 872, 0, 120, FACING_EAST, 0, 680, 32, 920, 192, 0},
    {8, CONDITION_ALWAYS, 792, 0, 152, FACING_NORTH, 0, 680, 32, 920, 192, 0},
    {SCENE_TABLE_END},
};

s16 gMadraCatacombs_LadderRoomRegions[] = {
    112, 48, 144, 128, 64, 160, -1, 1,
    336, 48, 144, 352, 64, 160, -1, 3,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(MadraCatacombsEntrance),
    SCENE_EXIT(1, MadraCatacombsBookshelf, 8),
    SCENE_EXIT(2, MadraCatacombsStoneDoor, 7),
    SCENE_EXIT(3, MadraCatacombsChampaTunnel, 1),
    SCENE_EXIT(4, MadraCatacombsStoneDoor, 13),
    SCENE_EXIT(5, MadraCatacombsChampaTunnel, 6),
    SCENE_EXITS(MadraCatacombsLadders),
    SCENE_EXIT(1, Madra, 15),
    SCENE_EXIT(2, MadraCatacombsChampaTunnel, 7),
    SCENE_EXIT(3, Madra, 16),
    SCENE_EXIT(4, MadraCatacombsChampaTunnel, 4),
    SCENE_EXITS(MadraCatacombsChampaTunnel),
    SCENE_EXIT(1, MadraCatacombsEntrance, 3),
    SCENE_EXIT(2, MadraCatacombsChampaTunnel, 3),
    SCENE_EXIT(3, MadraCatacombsChampaTunnel, 2),
    SCENE_EXIT(4, MadraCatacombsLadders, 4),
    SCENE_EXIT(5, MadraCatacombsChampaTunnel, 8),
    SCENE_EXIT(6, MadraCatacombsEntrance, 5),
    SCENE_EXIT(7, MadraCatacombsLadders, 2),
    SCENE_EXIT(8, MadraCatacombsChampaTunnel, 5),
    SCENE_EXITS(MadraCatacombsStoneDoor),
    SCENE_EXIT(1, MadraCatacombsBookshelf, 1),
    SCENE_EXIT(2, MadraCatacombsStoneDoor, 11),
    SCENE_EXIT(3, MadraCatacombsStoneDoor, 9),
    SCENE_EXIT(4, MadraCatacombsBookshelf, 2),
    SCENE_EXIT(5, MadraCatacombsTablet, 4),
    SCENE_EXIT(6, MadraCatacombsTablet, 1),
    SCENE_EXIT(7, MadraCatacombsEntrance, 2),
    SCENE_EXIT(8, MadraCatacombsStoneDoor, 12),
    SCENE_EXIT(9, MadraCatacombsStoneDoor, 3),
    SCENE_EXIT(10, MadraCatacombsBookshelf, 7),
    SCENE_EXIT(11, MadraCatacombsStoneDoor, 2),
    SCENE_EXIT(12, MadraCatacombsStoneDoor, 8),
    SCENE_EXIT(13, MadraCatacombsEntrance, 4),
    SCENE_EXITS(MadraCatacombsBookshelf),
    SCENE_EXIT(1, MadraCatacombsStoneDoor, 1),
    SCENE_EXIT(2, MadraCatacombsStoneDoor, 4),
    SCENE_EXIT(3, MadraCatacombsBookshelf, 5),
    SCENE_EXIT(4, MadraCatacombsBookshelf, 6),
    SCENE_EXIT(5, MadraCatacombsBookshelf, 3),
    SCENE_EXIT(6, MadraCatacombsBookshelf, 4),
    SCENE_EXIT(7, MadraCatacombsStoneDoor, 10),
    SCENE_EXIT(8, MadraCatacombsEntrance, 1),
    SCENE_EXITS(MadraCatacombsTablet),
    SCENE_EXIT(1, MadraCatacombsStoneDoor, 6),
    SCENE_EXIT(2, MadraCatacombsTablet, 3),
    SCENE_EXIT(3, MadraCatacombsTablet, 2),
    SCENE_EXIT(4, MadraCatacombsStoneDoor, 5),
    SCENE_EXITS_END,
};

struct ActorPlacement gMadraCatacombs_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gMadraCatacombs_EntrancePlacements[] = {
    ACTOR_PLACEMENT(250, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMadraCatacombs_ChampaTunnelPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(71, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 794, 0, 236, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMadraCatacombs_BookshelfPlacements[] = {
    ACTOR_PLACEMENT(679, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 202, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMadraCatacombs_TabletPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, 7, 150, 0, 366, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, 7, 170, 0, 366, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
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

struct Trigger gMadraCatacombs_EntranceTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(MadraCatacombs_EnterDoorway)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(MadraCatacombs_EnterDoorway)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(MadraCatacombs_EnterDoorway)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(MadraCatacombs_EnterDoorway)},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_VILLAGE_ROPE_LOW_POST, 0x201, 0},
    {TRIGGER_PUSHED, 10, FLAG_MADRA_CATACOMBS_HOLE_FILLED, EVENT(MadraCatacombs_DropPillarIntoHole)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HOLE_PILLAR, FLAG_MADRA_CATACOMBS_HOLE_FILLED, EVENT(MadraCatacombs_DropPillarIntoHole)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PUSHED_PILLAR, FLAG_MADRA_CATACOMBS_PILLAR_PLACED, EVENT(MadraCatacombs_StartMovingPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_PILLAR, FLAG_MADRA_CATACOMBS_PILLAR_PLACED, EVENT(MadraCatacombs_FinishMovingPushedPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 11, FLAG_MADRA_CATACOMBS_CLIMBABLE_PILLAR_PLACED, EVENT(MadraCatacombs_PushClimbablePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLIMBABLE_PILLAR, FLAG_MADRA_CATACOMBS_CLIMBABLE_PILLAR_PLACED, EVENT(MadraCatacombs_StartMovingClimbablePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLIMBABLE_PILLAR, FLAG_MADRA_CATACOMBS_CLIMBABLE_PILLAR_PLACED, EVENT(MadraCatacombs_FinishMovingClimbablePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_VILLAGE_PUDDLE, 0x202, EVENT(MadraCatacombs_FreezePuddle)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(MadraCatacombs_RevealButton)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(MadraCatacombs_HideButton)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(MadraCatacombs_HearEyesOfTruth)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMadraCatacombs_LaddersTriggers[] = {
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gMadraCatacombs_ChampaTunnelTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_TUNNEL_ROPE_EAST_POST, 0x200, 0},
    {TRIGGER_TALK, ACTOR_TUNNEL_EXPLORER, CONDITION_ALWAYS,
     MSGNO(MsgFieldCaveFoundByChampa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TUNNEL_EXPLORER,
     CONDITION_ALWAYS, MSGNO(MsgFieldWhoDugCave)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMadraCatacombs_StoneDoorTriggers[] = {
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

struct Trigger gMadraCatacombs_BookshelfTriggers[] = {
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

struct Trigger gMadraCatacombs_TabletTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(MadraCatacombs_OpenTabletRoomDoor)},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {USE_ITEM(ITEM_RUIN_KEY), 10, CONDITION_ALWAYS,
     EVENT(MadraCatacombs_UnlockWithRuinKey)},
    {TRIGGER_EXAMINE, 10, FLAG_MADRA_CATACOMBS_TABLET_DOOR_OPEN,
     EVENT(MadraCatacombs_TryLockedDoor)},
    {TRIGGER_TABLE_END},
};
