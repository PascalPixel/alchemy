/// @file
/// The Madra Catacombs, under the town: a ladder room entered from Madra's two
/// ladders, the tunnel the Champa dug, the deserted village with its pillars,
/// rope, puddle and the door Reveal opens, the room whose bookshelf Tremor
/// shakes until a chest falls into the room below, and the tablet room whose
/// red double door the Ruin Key opens, lighting its flames before Moloch's
/// tablet. Once FLAG_BRIGGS_CAUGHT is set the Champa's way in is blocked.

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
/// Frost on the village's puddle, which stays a pillar of ice until the party
/// leaves.
#define FLAG_VILLAGE_PUDDLE_FROZEN FLAG_SCENE_TEMP_02
/// Set while the bookshelf leans as Tremor shakes it, so that the chest on it
/// is nudged once each time it comes upright.
#define FLAG_BOOKSHELF_LEANING FLAG_SCENE_TEMP_04

/// Set once the pillar Move pushed onto cell (40, 13) has dropped into the
/// deserted village's hole and filled it.
#define FLAG_MADRA_CATACOMBS_HOLE_FILLED 0x8c8
/// Set once the Ruin Key has opened the tablet room's double door; its flames
/// burn from then on.
#define FLAG_MADRA_CATACOMBS_TABLET_DOOR_OPEN 0x8c9
/// Set once Move has left the village's second pillar on cell (44, 26).
#define FLAG_MADRA_CATACOMBS_PILLAR_PLACED 0x8ca
/// Set once the button Reveal shows has opened the village's golden door.
#define FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN 0x8cb
/// Set once Tremor has shaken the chest off the bookshelf into the room below.
#define FLAG_MADRA_CATACOMBS_CHEST_FELL 0x8cc
/// Set once Move has left the vine-covered pillar on cell (35, 17).
#define FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED 0x9ef
/// Set while Reveal shows the push button by the closed golden door.
#define FLAG_MADRA_CATACOMBS_BUTTON_REVEALED FLAG_SCENE_TEMP_03

/// The deserted village's actors that its events move.
enum MadraCatacombsVillageActorId {
  /// The vine-covered pillar Move carries onto the upper level to climb by.
  ACTOR_VINE_PILLAR = ACTOR_FIRST_PLACED,
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
  /// A man in the tunnel until Briggs is caught: "I can't believe the Champa
  /// just stumbled onto this cave accidentally..."
  ACTOR_MAN_ON_THE_CHAMPAS_CAVE
};

/// The flames that flare up down each side of the tablet room.
#define FLARES_PER_SIDE 5

/// The tablet room's actors: the two halves of the red double door, the
/// flames that flare up down each side as it opens, nearest the door last, and
/// an unseen marker that stands on each flame once they burn.
enum MadraCatacombsTabletRoomActorId {
  ACTOR_LEFT_DOOR = ACTOR_FIRST_PLACED,
  ACTOR_RIGHT_DOOR,
  ACTOR_FIRST_WEST_FLARE,
  ACTOR_FIRST_EAST_FLARE = ACTOR_FIRST_WEST_FLARE + FLARES_PER_SIDE,
  ACTOR_FIRST_FLAME_MARKER = ACTOR_FIRST_EAST_FLARE + FLARES_PER_SIDE
};

/// The bookshelf room's bookshelf, and the chest on it that Tremor shakes off;
/// the room below places the fallen chest.
enum MadraCatacombsBookshelfActorId {
  ACTOR_BOOKSHELF = ACTOR_FIRST_PLACED,
  ACTOR_SHELF_CHEST = 64,
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
extern s16 gMadraCatacombs_VinePillar[];
extern s32 gMadraCatacombs_WalkThroughDoor[];

void Actor_DisableById(s32 id);

s32 Field_FindNearbyTalkTarget(s32 owner);

s32 Leader_CanPushObjectAhead(void);

void MadraCatacombs_RockChestWithBookshelf(void);

/// Arriving in the catacombs' village by a way the Champa have since blocked:
/// the leader leaves again.
void MadraCatacombs_LeaveBlockedVillage(void);

void Rope_Place(s32 index, s32 flag, s32 post, s32 otherPost);

#include "shared/field_rope_place.c"

struct Actor *Leader_FindActorAhead(void);
void Actor_PlaceMovedBlock(void);
void Leader_PushObjectAhead(void);

s32 Actor_GetCellIndex(struct Actor *actor);

// Raises each listed actor's cells: the shape grid and the cell flags under it,
// then lifts the leader if the new terrain is above the leader.
#include "shared/pillar_raise.h"

#include "shared/pillar_lower_held_cells.h"

#include "shared/held_actor_raise_cell.h"

extern struct Trigger gMadraCatacombs_Triggers[];

void MadraCatacombs_SetUpStoneDoorRoom(void);

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

/// Examining a locked door: "The door won't budge. ...Must be locked."
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
/// it fills for good.
void MadraCatacombs_DropPillarIntoHole(void) {
  struct Actor *actor = Actor_GetById(ACTOR_HOLE_PILLAR);
  s32 z;

  if (FIELD_CELL(actor->x.fixed) == 40) {
    z = FIELD_CELL(actor->z.fixed);
    if (z == 13) {
      Event_Start();
      Event_ResolvePendingActions(0);
      Actor_DropAndWait(actor);
      MapGrid_CopyCellAttributes(41, 13, 2, 1, 39, z);
      Event_SetFlag(FLAG_MADRA_CATACOMBS_HOLE_FILLED);
      Event_End();
    }
  }
}

/// When Move takes hold of the second pillar: the cells it blocked are freed.
void MadraCatacombs_StartMovingPushedPillar(void) {
  Actor_LiftTargetBlock(gMadraCatacombs_PushedPillar);
}

/// The second pillar, moved onto cell (44, 26), stays there.
void MadraCatacombs_FinishMovingPushedPillar(void) {
  struct Actor *actor = Actor_GetById(ACTOR_PUSHED_PILLAR);

  Actor_PlaceMovedBlock();
  if (FIELD_CELL(actor->x.fixed) == 44 && FIELD_CELL(actor->z.fixed) == 26)
    Event_SetFlag(FLAG_MADRA_CATACOMBS_PILLAR_PLACED);
}

/// Frost on a puddle: it freezes into a pillar, its flag set
/// (FlagBlock_ApplyFlags).
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
    left->x.fixed += -(toFIXED(1) / 10);
    right->x.fixed += toFIXED(1) / 10;
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

void Actor_PlaceOnActor(s32 id, s32 value);
void Rope_Initialize(void);
void FlagBlock_PlaceAll(const void *data);
void Actor_PlaceBlocks(const void *data);
/// The vine-covered pillar settles where Move left it, its cells raised; on
/// cell (35, 17) it stays there.
void MadraCatacombs_SettleVinePillar(struct Actor *actor);
/// Arriving in the Champa tunnel by a way the Champa have since blocked: the
/// leader leaves again.
void MadraCatacombs_LeaveBlockedTunnel(void);

/// Reveal shows the push button by the golden door, pressed in once the door
/// has opened.
void MadraCatacombs_RevealButton(void) {
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN) != 0)
    MapGrid_CopyCellsTo(72, 63, 36, 38, 1, 2);
  else
    MapGrid_CopyCellsTo(71, 63, 36, 38, 1, 2);
  Event_SetFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED);
}

/// As Reveal fades: the push button is hidden again.
void MadraCatacombs_HideButton(void) {
  MapGrid_CopyCellsTo(70, 63, 36, 38, 1, 2);
  Event_ClearFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED);
}

/// Examining the wall by the golden door: once Reveal shows the push button
/// there, the button is pressed and the door opens; otherwise the inscription
/// reads "Look upon me with eyes of truth... Then the door will open..."
void MadraCatacombs_HearEyesOfTruth(void) {
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED) != 0 &&
      Event_GetFlag(FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN) == 0) {
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
    Event_SetFlag(FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN);
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
  struct Actor *actor;
  FIXED x;

  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL) == 0) {
    actor = Actor_GetById(ACTOR_SHELF_CHEST);
    x = actor->x.fixed;
    if (x < (toFIXED(186))) {
      Event_Start();
      Event_ResolvePendingActions(0);
      Event_PlaySound(SE_DUST_PUFF);
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      wait(10);
      Actor_SetPosition(ACTOR_SHELF_CHEST, 0, 0);
      Event_SetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL);
      Event_End();
    }
  }
}

void MadraCatacombs_SettleVinePillar(struct Actor *actor) {
  HeldActor_RaiseCell();
  if (FIELD_CELL(actor->x.fixed) == 35 && FIELD_CELL(actor->z.fixed) == 17)
    Event_SetFlag(FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED);
}

/// Walking into the vine-covered pillar while holding it with Move pushes it a
/// cell on.
void MadraCatacombs_PushVinePillar(void) {
  struct Actor *actor;

  if (Leader_CanPushObjectAhead() != 0) {
    actor = Actor_GetById(
        Field_FindNearbyTalkTarget(gMainRoutineWork.leaderActorId));
    Pillar_LowerHeldCells(gMadraCatacombs_VinePillar);
    Leader_PushObjectAhead();
    MadraCatacombs_SettleVinePillar(actor);
  }
}

/// When Move takes hold of the vine-covered pillar: its raised cells are
/// lowered.
void MadraCatacombs_StartMovingVinePillar(void) {
  Pillar_LowerHeldCells(gMadraCatacombs_VinePillar);
}

void MadraCatacombs_OpenDoorAndEnter(s32 sourceX, s32 sourceZ, s32 x, s32 z,
                                     s32 width, s32 height);

/// When the vine-covered pillar comes to rest after Move: settles it where it
/// stopped (MadraCatacombs_SettleVinePillar).
void MadraCatacombs_FinishMovingVinePillar(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (work->movedBlock != 0)
    MadraCatacombs_SettleVinePillar(work->movedBlock);
}

/// The tablet room's flames, lit: the flares stand in their two rows and a
/// marker goes to stand on each.
void MadraCatacombs_LightFlames(void) {
  u32 i;
  s32 target = ACTOR_FIRST_WEST_FLARE;
  s32 actor = ACTOR_FIRST_FLAME_MARKER;

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
  for (i = 0; i < FLARES_PER_SIDE * 2; i++) {
    Actor_SetAnimation(actor, 0);
    Actor_PlaceOnActor(actor, target);
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
  struct Actor *chest = Actor_GetById(ACTOR_SHELF_CHEST);
  struct Sprite *sprite = chest->sprite;

  sprite->rotation = ((struct Sprite *)bookshelf->sprite)->rotation;
  if (Event_GetFlag(FLAG_BOOKSHELF_LEANING) != 0 && sprite->rotation == 0) {
    Event_PlaySound(SE_BOOKSHELF_SLIDE);
    chest->x.fixed += toFIXED(-2);
    chest->velocityY = toFIXED(2);
    Event_ClearFlag(FLAG_BOOKSHELF_LEANING);
  }
  if (sprite->rotation != 0)
    Event_SetFlag(FLAG_BOOKSHELF_LEANING);
}

/// The leader steps to the middle of a door, walks in and leaves by the exit
/// the step trigger names.
void MadraCatacombs_EnterDoor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *actor;
  s32 cell;

  actor = Actor_GetById(party->leaderActorId);
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetAnimation(party->leaderActorId, ANIM_WALK);
  Actor_SetSpeed(party->leaderActorId, 0.5);
  cell = FIXEDtoINT(actor->x.fixed);
  Actor_SetMoveTarget(actor,
                      toFIXED(8 - cell % MAPGRID_CELL_PIXELS) + actor->x.fixed,
                      actor->y.fixed, actor->z.fixed);
  FieldActor_WaitForMove(actor);
  actor->facing = FACING_NORTH;
  Actor_SetScript(party->leaderActorId, gMadraCatacombs_WalkThroughDoor);
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
  struct Actor *actor;
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
    actor = Actor_GetById(gMainRoutineWork.leaderActorId);
    cell = FIXEDtoINT(actor->x.fixed);
    Actor_SetMoveTarget(
        actor, toFIXED(8 - cell % MAPGRID_CELL_PIXELS) + actor->x.fixed,
        actor->y.fixed, actor->z.fixed);
    FieldActor_WaitForMove(actor);
    actor->facing = FACING_NORTH;
  }
  Actor_SetScript(gMainRoutineWork.leaderActorId,
                  gMadraCatacombs_WalkThroughDoor);
  wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->stepTrigger);
}

/// The stone door, two cells wide, slides open and the leader goes through.
void MadraCatacombs_OpenStoneDoor(void) {
  MadraCatacombs_OpenDoorAndEnter(78, 29, 78, 31, 2, 2);
}

/// The bookshelf room's west door opens and the leader goes through.
void MadraCatacombs_OpenBookshelfRoomWestDoor(void) {
  MadraCatacombs_OpenDoorAndEnter(109, 1, 109, 3, 1, 2);
}

/// The bookshelf room's east door opens and the leader goes through.
void MadraCatacombs_OpenBookshelfRoomEastDoor(void) {
  MadraCatacombs_OpenDoorAndEnter(118, 1, 118, 3, 1, 2);
}

/// The tablet room's door opens and the leader goes through.
void MadraCatacombs_OpenTabletRoomDoor(void) {
  MadraCatacombs_OpenDoorAndEnter(92, 29, 92, 31, 1, 2);
}

/// Arriving by a way the Champa have since blocked, the leader fades out and
/// leaves by `exit`.
#define DEFINE_LEAVE_BLOCKED_WAY(name, exit)                                   \
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
    Import_EventSetExitRequest(exit);                                          \
  }

DEFINE_LEAVE_BLOCKED_WAY(
    MadraCatacombs_LeaveBlockedVillage,
    MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_WEST)

DEFINE_LEAVE_BLOCKED_WAY(MadraCatacombs_LeaveBlockedTunnel, 8)

s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_ENTRANCE)
    MadraCatacombs_SetUpVillage();
  else if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_CHAMPA_TUNNEL)
    MadraCatacombs_SetUpChampaTunnel();
  else if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_STONE_DOOR)
    MadraCatacombs_SetUpStoneDoorRoom();
  else if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
    MadraCatacombs_SetUpBookshelfRoom();
  else if (gMainRoutineWork.next == SCENE_MADRA_CATACOMBS_TABLET)
    MadraCatacombs_SetUpTabletRoom();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Sets up the catacombs' village: its rope, the pillars Move has placed, the
/// frozen puddle, the opened golden door and, once Briggs is caught, its
/// blocked way.
void MadraCatacombs_SetUpVillage(void) {
  Event_ClearFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED);
  Actor_SetSpritePriority(ACTOR_VILLAGE_CHEST, 3);
  Rope_Initialize();
  FieldRope_Place(1, FLAG_VILLAGE_ROPE_TIED, ACTOR_VILLAGE_ROPE_LOW_POST,
                  ACTOR_VILLAGE_ROPE_HIGH_POST);
  Actor_DisableById(ACTOR_HOLE_PILLAR);
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_HOLE_FILLED) != 0) {
    MapGrid_CopyCellAttributes(41, 13, 2, 1, 39, 13);
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_HOLE_PILLAR),
                                         toFIXED(648), 0, toFIXED(216));
  }
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_PILLAR_PLACED) != 0)
    Actor_SetPosition(ACTOR_PUSHED_PILLAR, 712, 424);
  Actor_PlaceBlocks(gMadraCatacombs_PushedPillar);
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED) != 0)
    Actor_SetPosition(ACTOR_VINE_PILLAR, 568, 280);
  Actor_GetById(ACTOR_VINE_PILLAR)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Pillar_Raise(gMadraCatacombs_VinePillar);
  FlagBlock_PlaceAll(gMadraCatacombs_Puddles);
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN) != 0) {
    MapGrid_CopyCellsTo(75, 66, 97, 39, 2, 2);
    MapGrid_CopyCellAttributes(33, 37, 2, 2, 33, 39);
    wait(1);
  }
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT) != 0) {
    MapGrid_CopyCellsTo(82, 74, 116, 16, 5, 3);
    MapGrid_CopyCellAttributes(52, 11, 5, 1, 52, 18);
    if (gMainRoutineWork.entrance ==
        MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_CHAMPA_TUNNEL_WEST)
      MadraCatacombs_LeaveBlockedVillage();
    else
      wait(10);
  }
}

/// Sets up the Champa tunnel: its rope and, once Briggs is caught, its blocked
/// way.
void MadraCatacombs_SetUpChampaTunnel(void) {
  Rope_Initialize();
  FieldRope_Place(0, FLAG_TUNNEL_ROPE_TIED, ACTOR_TUNNEL_ROPE_EAST_POST,
                  ACTOR_TUNNEL_ROPE_WEST_POST);
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT) != 0) {
    Actor_SetPosition(ACTOR_MAN_ON_THE_CHAMPAS_CAVE, 0, 0);
    MapGrid_CopyCellsTo(68, 70, 113, 11, 5, 3);
    MapGrid_CopyCellsTo(50, 74, 50, 75, 2, 1);
    MapGrid_CopyCellAttributes(49, 9, 5, 1, 49, 13);
    if (gMainRoutineWork.entrance == 8)
      MadraCatacombs_LeaveBlockedTunnel();
    else
      wait(1);
  }
}

/// Sets up the stone door's room, below the bookshelf: the fallen chest is
/// there only once it has fallen.
void MadraCatacombs_SetUpStoneDoorRoom(void) {
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL) == 0)
    Actor_SetPosition(ACTOR_FALLEN_CHEST, 0, 0);
}

/// Sets up the bookshelf room: the shaken chest stands on top of its shelf
/// until it has fallen.
void MadraCatacombs_SetUpBookshelfRoom(void) {
  struct Actor *actor;

  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL) != 0) {
    Actor_SetPosition(ACTOR_SHELF_CHEST, 0, 0);
  } else {
    actor = Actor_GetById(ACTOR_SHELF_CHEST);
    actor->collisionFlags = ACTOR_NOT_TARGETED;
    actor->motionFlags = ACTOR_FALLS;
    actor->y.fixed = toFIXED(32);
    actor->groundHeight = toFIXED(32);
    actor->update = MadraCatacombs_RockChestWithBookshelf;
    Actor_GetById(ACTOR_BOOKSHELF)->scaleY = FIXED_ONE * 4 / 5;
  }
}

/// Mirrors the door's right half and stands both halves on the ground; once
/// the Ruin Key has opened it they stand 12 pixels apart (already so when the
/// party comes back from a battle) and the flames burn.
void MadraCatacombs_SetUpTabletRoom(void) {
  struct Actor *left = Actor_GetById(ACTOR_LEFT_DOOR);
  struct Actor *right = Actor_GetById(ACTOR_RIGHT_DOOR);

  left->scaleX = FIXED_ONE * 5 / 4;
  right->scaleX = -FIXED_ONE * 5 / 4;
  left->motionFlags = 0;
  right->motionFlags = 0;
  left->y.fixed = 0;
  right->y.fixed = 0;
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_TABLET_DOOR_OPEN) != 0) {
    if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
      left->x.fixed += toFIXED(-12);
      right->x.fixed += toFIXED(12);
    }
    MadraCatacombs_LightFlames();
  }
}

// clang-format off
/// The leader's actor script for walking north through an opened door, growing
/// smaller as he goes.
s32 gMadraCatacombs_WalkThroughDoor[] = {                                                               
      SC_SET(SW_MOTION, 0),                                                    
      SC_ADD(SW_Z, -toFIXED(1) / 2),                                                   
      SC_ADD(SW_SCALE_X, -FIXED_ONE / 64),                                               
      SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),                                               
      SC_WAIT(1),                                                              
      SC_LOOP(24, SC_TOP),                                                     
      SC_END,                                                                  
  };

/// A tablet room flame's actor script as it flares up.
s32 gMadraCatacombs_FlameFlaresUp[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE / 16),
    SC_SET(SW_SCALE_Y, FIXED_ONE / 16),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_END,
};

/// The great flame's actor script as it flares up.
s32 gMadraCatacombs_GreatFlameFlaresUp[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE / 16),
    SC_SET(SW_SCALE_Y, FIXED_ONE / 16),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 2),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 4),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, FIXED_ONE * 3 / 8),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 4),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(3)),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_END,
};

/// The second pillar Move pushes, ended by -1.
s16 gMadraCatacombs_PushedPillar[] = {
    ACTOR_PUSHED_PILLAR, -1,
};

/// The puddles Frost freezes into pillars, each with its flag.
s16 gMadraCatacombs_Puddles[] = {
    ACTOR_VILLAGE_PUDDLE, FLAG_VILLAGE_PUDDLE_FROZEN, -1,
};

/// The vine-covered pillar Move pushes, ended by -1.
s16 gMadraCatacombs_VinePillar[] = {
    ACTOR_VINE_PILLAR, -1, 0,
};

/// The entrances of the catacombs' rooms with no table of their own.
struct SceneEntrance gMadraCatacombs_Entrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

/// The entrances of the stone door's room.
struct SceneEntrance gMadraCatacombs_StoneDoorEntrances[] = {
    {3, CONDITION_ALWAYS, 512, 0, 224, FACING_NORTH, 0, 392, 40, 632, 280, 0},
    {SCENE_TABLE_END},
};

/// The entrances of the bookshelf room.
struct SceneEntrance gMadraCatacombs_BookshelfEntrances[] = {
    {5, CONDITION_ALWAYS, 728, 0, 88, FACING_SOUTH, 0, 680, 32, 920, 192, 0},
    {6, CONDITION_ALWAYS, 872, 0, 88, FACING_SOUTH, 0, 680, 32, 920, 192, 0},
    {7, CONDITION_ALWAYS, 872, 0, 120, FACING_EAST, 0, 680, 32, 920, 192, 0},
    {8, CONDITION_ALWAYS, 792, 0, 152, FACING_NORTH, 0, 680, 32, 920, 192, 0},
    {SCENE_TABLE_END},
};

/// The exit regions of the ladder room.
s16 gMadraCatacombs_LadderRoomRegions[] = {
    112, 48, 144, 128, 64, 160, -1, 1,
    336, 48, 144, 352, 64, 160, -1, 3,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(MadraCatacombsEntrance),
    SCENE_EXIT(MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_BOOKSHELF, MadraCatacombsBookshelf, MADRA_CATACOMBS_BOOKSHELF_FROM_MADRA_CATACOMBS_ENTRANCE),
    SCENE_EXIT(MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_STONE_DOOR_NORTH, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_ENTRANCE_NORTH),
    SCENE_EXIT(MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_WEST, MadraCatacombsChampaTunnel, MADRA_CATACOMBS_CHAMPA_TUNNEL_FROM_MADRA_CATACOMBS_ENTRANCE_WEST),
    SCENE_EXIT(MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_STONE_DOOR_SOUTH, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_ENTRANCE_SOUTH),
    SCENE_EXIT(MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_EAST, MadraCatacombsChampaTunnel, MADRA_CATACOMBS_CHAMPA_TUNNEL_FROM_MADRA_CATACOMBS_ENTRANCE_EAST),
    SCENE_EXITS(MadraCatacombsLadders),
    SCENE_EXIT(MADRA_CATACOMBS_LADDERS_TO_MADRA_WEST, Madra, MADRA_FROM_MADRA_CATACOMBS_LADDERS_WEST),
    SCENE_EXIT(MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_NORTH, MadraCatacombsChampaTunnel, MADRA_CATACOMBS_CHAMPA_TUNNEL_FROM_MADRA_CATACOMBS_LADDERS_NORTH),
    SCENE_EXIT(MADRA_CATACOMBS_LADDERS_TO_MADRA_EAST, Madra, MADRA_FROM_MADRA_CATACOMBS_LADDERS_EAST),
    SCENE_EXIT(MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_SOUTH, MadraCatacombsChampaTunnel, MADRA_CATACOMBS_CHAMPA_TUNNEL_FROM_MADRA_CATACOMBS_LADDERS_SOUTH),
    SCENE_EXITS(MadraCatacombsChampaTunnel),
    SCENE_EXIT(MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_WEST, MadraCatacombsEntrance, MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_CHAMPA_TUNNEL_WEST),
    SCENE_EXIT(2, MadraCatacombsChampaTunnel, 3),
    SCENE_EXIT(3, MadraCatacombsChampaTunnel, 2),
    SCENE_EXIT(MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_SOUTH, MadraCatacombsLadders, MADRA_CATACOMBS_LADDERS_FROM_MADRA_CATACOMBS_CHAMPA_TUNNEL_SOUTH),
    SCENE_EXIT(5, MadraCatacombsChampaTunnel, 8),
    SCENE_EXIT(MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_EAST, MadraCatacombsEntrance, MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_CHAMPA_TUNNEL_EAST),
    SCENE_EXIT(MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_NORTH, MadraCatacombsLadders, MADRA_CATACOMBS_LADDERS_FROM_MADRA_CATACOMBS_CHAMPA_TUNNEL_NORTH),
    SCENE_EXIT(8, MadraCatacombsChampaTunnel, 5),
    SCENE_EXITS(MadraCatacombsStoneDoor),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_NORTHWEST, MadraCatacombsBookshelf, MADRA_CATACOMBS_BOOKSHELF_FROM_MADRA_CATACOMBS_STONE_DOOR_NORTHWEST),
    SCENE_EXIT(2, MadraCatacombsStoneDoor, 11),
    SCENE_EXIT(3, MadraCatacombsStoneDoor, 9),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_EAST, MadraCatacombsBookshelf, MADRA_CATACOMBS_BOOKSHELF_FROM_MADRA_CATACOMBS_STONE_DOOR_EAST),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_EAST, MadraCatacombsTablet, MADRA_CATACOMBS_TABLET_FROM_MADRA_CATACOMBS_STONE_DOOR_EAST),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_WEST, MadraCatacombsTablet, MADRA_CATACOMBS_TABLET_FROM_MADRA_CATACOMBS_STONE_DOOR_WEST),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_NORTH, MadraCatacombsEntrance, MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_STONE_DOOR_NORTH),
    SCENE_EXIT(8, MadraCatacombsStoneDoor, 12),
    SCENE_EXIT(9, MadraCatacombsStoneDoor, 3),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_SOUTH, MadraCatacombsBookshelf, MADRA_CATACOMBS_BOOKSHELF_FROM_MADRA_CATACOMBS_STONE_DOOR_SOUTH),
    SCENE_EXIT(11, MadraCatacombsStoneDoor, 2),
    SCENE_EXIT(12, MadraCatacombsStoneDoor, 8),
    SCENE_EXIT(MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_SOUTH, MadraCatacombsEntrance, MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_STONE_DOOR_SOUTH),
    SCENE_EXITS(MadraCatacombsBookshelf),
    SCENE_EXIT(MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_NORTHWEST, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_BOOKSHELF_NORTHWEST),
    SCENE_EXIT(MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_EAST, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_BOOKSHELF_EAST),
    SCENE_EXIT(3, MadraCatacombsBookshelf, 5),
    SCENE_EXIT(4, MadraCatacombsBookshelf, 6),
    SCENE_EXIT(5, MadraCatacombsBookshelf, 3),
    SCENE_EXIT(6, MadraCatacombsBookshelf, 4),
    SCENE_EXIT(MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_SOUTH, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_BOOKSHELF_SOUTH),
    SCENE_EXIT(MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_ENTRANCE, MadraCatacombsEntrance, MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_BOOKSHELF),
    SCENE_EXITS(MadraCatacombsTablet),
    SCENE_EXIT(MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_WEST, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_TABLET_WEST),
    SCENE_EXIT(2, MadraCatacombsTablet, 3),
    SCENE_EXIT(3, MadraCatacombsTablet, 2),
    SCENE_EXIT(MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_EAST, MadraCatacombsStoneDoor, MADRA_CATACOMBS_STONE_DOOR_FROM_MADRA_CATACOMBS_TABLET_EAST),
    SCENE_EXITS_END,
};

/// The actors placed in the catacombs' rooms with no table of their own.
struct ActorPlacement gMadraCatacombs_Placements[] = {
    {SCENE_TABLE_END},
};

/// The actors placed in the catacombs' entrance.
struct ActorPlacement gMadraCatacombs_EntrancePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_VINE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The actors placed in the Champa tunnel.
struct ActorPlacement gMadraCatacombs_ChampaTunnelPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 794, 0, 236, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/// The actors placed in the bookshelf room.
struct ActorPlacement gMadraCatacombs_BookshelfPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BOOKSHELF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 202, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The actors placed in the tablet room.
struct ActorPlacement gMadraCatacombs_TabletPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 150, 0, 366, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 170, 0, 366, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The triggers of the catacombs' rooms with no table of their own.
struct Trigger gMadraCatacombs_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// The triggers of the catacombs' entrance.
struct Trigger gMadraCatacombs_EntranceTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(MadraCatacombs_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(MadraCatacombs_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(MadraCatacombs_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(MadraCatacombs_EnterDoor)},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_VILLAGE_ROPE_LOW_POST, FLAG_VILLAGE_ROPE_TIED, 0},
    {TRIGGER_PUSHED, 10, FLAG_MADRA_CATACOMBS_HOLE_FILLED, EVENT(MadraCatacombs_DropPillarIntoHole)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HOLE_PILLAR, FLAG_MADRA_CATACOMBS_HOLE_FILLED, EVENT(MadraCatacombs_DropPillarIntoHole)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PUSHED_PILLAR, FLAG_MADRA_CATACOMBS_PILLAR_PLACED, EVENT(MadraCatacombs_StartMovingPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_PILLAR, FLAG_MADRA_CATACOMBS_PILLAR_PLACED, EVENT(MadraCatacombs_FinishMovingPushedPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 11, FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED, EVENT(MadraCatacombs_PushVinePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VINE_PILLAR, FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED, EVENT(MadraCatacombs_StartMovingVinePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VINE_PILLAR, FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED, EVENT(MadraCatacombs_FinishMovingVinePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_VILLAGE_PUDDLE, FLAG_VILLAGE_PUDDLE_FROZEN, EVENT(MadraCatacombs_FreezePuddle)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(MadraCatacombs_RevealButton)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(MadraCatacombs_HideButton)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(MadraCatacombs_HearEyesOfTruth)},
    {TRIGGER_TABLE_END},
};

/// The triggers of the ladder room.
struct Trigger gMadraCatacombs_LaddersTriggers[] = {
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TABLE_END},
};
// clang-format on

/// The triggers of the Champa tunnel.
struct Trigger gMadraCatacombs_ChampaTunnelTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_TUNNEL_ROPE_EAST_POST,
     FLAG_TUNNEL_ROPE_TIED, 0},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_CHAMPAS_CAVE, CONDITION_ALWAYS,
     MSGNO(MsgFieldCaveFoundByChampa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_CHAMPAS_CAVE,
     CONDITION_ALWAYS, MSGNO(MsgFieldWhoDugCave)},
    {TRIGGER_TABLE_END},
};

/// The triggers of the stone door's room.
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
     EVENT(MadraCatacombs_OpenStoneDoor)},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_TABLE_END},
};

/// The triggers of the bookshelf room.
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
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_BOOKSHELF, CONDITION_ALWAYS,
     EVENT(MadraCatacombs_ShakeBookshelf)},
    {TRIGGER_TABLE_END},
};

/// The triggers of the tablet room.
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
