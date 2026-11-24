/// @file
/// The Madra Catacombs, a deserted village in a cave under the town. Madra's
/// two ladders, the one inside the walls and the one in the Champa's hole,
/// come down into a small ladder room, and the tunnel the Champa dug runs from
/// there to the village. On the first visit Felix comes in along the tunnel's
/// upper path, Moves a pillar off a ledge into a pit to cross it, and takes the
/// Tremor Bit from the village's broken building. Once Briggs is caught that
/// way is blocked off, and the village is reached from the south, through a
/// golden door whose push button only Reveal shows. Inside, Frost freezes a
/// puddle, Move shifts two pillars to climb by, Lash ties a rope across, and
/// Tremor rocks a bookshelf on the building's upper floor until the chest on
/// top falls through to the floor below; it holds the Ruin Key, which opens the
/// red double door down the stairs, lighting the braziers before Moloch's
/// tablet.

#include "flags.h"
#include "sprite_ids.h"
#include "item_ids.h"
#include "shared/tables.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
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

/// Set once the pillar Move pushed onto cell (40, 13) has dropped into the pit
/// in the village's northern ledge and filled it.
#define FLAG_MADRA_CATACOMBS_HOLE_FILLED 0x8c8
/// Set once the Ruin Key has opened the red double door before Moloch's
/// tablet; its braziers burn from then on.
#define FLAG_MADRA_CATACOMBS_TABLET_DOOR_OPEN 0x8c9
/// Set once Move has left the village's plain stone pillar on cell (44, 26).
#define FLAG_MADRA_CATACOMBS_PILLAR_PLACED 0x8ca
/// Set once the push button Reveal shows has opened the village's golden door.
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
  /// The pillar on the northern ledge that Move pushes into the pit.
  ACTOR_HOLE_PILLAR = ACTOR_FIRST_PLACED + 3,
  /// The plain stone pillar Move pushes onto cell (44, 26).
  ACTOR_PUSHED_PILLAR,
  /// The puddle Frost freezes into a pillar.
  ACTOR_VILLAGE_PUDDLE,
  /// The chest by the rope, the map's first treasure spot, drawn over the
  /// ledge.
  ACTOR_VILLAGE_CHEST = FIELD_TREASURE_ACTOR
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

/// The far end of the Champa tunnel's upper path, cell (51, 13): the number of
/// both the tunnel's entrance there and its exit back the way the party came.
enum { CHAMPA_TUNNEL_UPPER_PATH_END = 8 };

/// The cells whose triggers run the catacombs' events.
enum MadraCatacombsCellBehavior {
  /// The red double door before Moloch's tablet, cells (9, 22) and (10, 22).
  CELL_BEHAVIOR_MADRA_CATACOMBS_TABLET_DOOR = 10,
  /// Cell (33, 17), west of the vine-covered pillar's place.
  CELL_BEHAVIOR_MADRA_CATACOMBS_VINE_PILLAR_PUSH = 11
};

/// The braziers that flare up down each side of the hall before Moloch's
/// tablet.
#define FLARES_PER_SIDE 5

/// The tablet hall's actors: the two halves of the red double door, the
/// brazier flames that flare up down each side as it opens, numbered from the
/// tablet down to the door, and an unseen marker that stands on each flame once
/// they burn.
enum MadraCatacombsTabletRoomActorId {
  ACTOR_LEFT_DOOR = ACTOR_FIRST_PLACED,
  ACTOR_RIGHT_DOOR,
  ACTOR_FIRST_WEST_FLARE,
  ACTOR_FIRST_EAST_FLARE = ACTOR_FIRST_WEST_FLARE + FLARES_PER_SIDE,
  ACTOR_FIRST_FLAME_MARKER = ACTOR_FIRST_EAST_FLARE + FLARES_PER_SIDE
};

/// The bookshelf on the broken building's upper floor, and the chest on top of
/// it that Tremor shakes off; the room below places the fallen chest.
enum MadraCatacombsBookshelfActorId {
  ACTOR_BOOKSHELF = ACTOR_FIRST_PLACED,
  ACTOR_SHELF_CHEST = FIELD_TREASURE_ACTOR,
  ACTOR_FALLEN_CHEST
};

struct Actor;

s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

extern struct SceneEntrance gSceneEntrances[];
extern struct SceneEntrance gMadraCatacombs_StoneDoorEntrances[];

extern struct SceneEntrance gMadraCatacombs_BookshelfEntrances[];

extern struct SceneRegion gMadraCatacombs_LadderRoomRegions[];

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
extern s32 gLeader_WalkThroughDoorScript[];

void Actor_DisableById(s32 id);

s32 Field_FindNearbyTalkTarget(s32 owner);

s32 Leader_CanPushObjectAhead(void);

void MadraCatacombs_RockChestWithBookshelf(void);

/// Arriving in the village from the Champa tunnel's west end once that way is
/// blocked off: Felix vanishes and the screen fades back to the tunnel.
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
  if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_STONE_DOOR)
    return gMadraCatacombs_StoneDoorEntrances;
  if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
    return gMadraCatacombs_BookshelfEntrances;
  return gSceneEntrances;
}

const struct SceneRegion *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_MADRA_CATACOMBS_LADDERS)
    return gMadraCatacombs_LadderRoomRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_ENTRANCE)
    return gMadraCatacombs_EntrancePlacements;
  if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_CHAMPA_TUNNEL)
    return gMadraCatacombs_ChampaTunnelPlacements;
  if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
    return gMadraCatacombs_BookshelfPlacements;
  if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_TABLET)
    return gMadraCatacombs_TabletPlacements;
  return gMadraCatacombs_Placements;
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_ENTRANCE)
    return gMadraCatacombs_EntranceTriggers;
  if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_LADDERS)
    return gMadraCatacombs_LaddersTriggers;
  if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_CHAMPA_TUNNEL)
    return gMadraCatacombs_ChampaTunnelTriggers;
  if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_STONE_DOOR)
    return gMadraCatacombs_StoneDoorTriggers;
  if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
    return gMadraCatacombs_BookshelfTriggers;
  if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_TABLET)
    return gMadraCatacombs_TabletTriggers;
  return gMadraCatacombs_Triggers;
}

void Actor_LiftTargetBlock(const void *data);
void FlagBlock_ApplyFlags(const void *data);

/// Examining the red double door at the foot of the hall before Moloch's tablet
/// (cells (9, 22) and (10, 22)) before the Ruin Key has opened it: "The door
/// won't budge. ...Must be locked."
void MadraCatacombs_TryLockedDoor(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  Event_Start();
  Event_EndLastingPsynergy(0);
  // "The door won't budge. ...Must be locked."
  FieldMessage_Show((s32)&MsgFieldDoorWontBudge, 1);
  state->fieldMenu = 1;
  Event_End();
}

#include "shared/actor_drop_and_wait.h"

/// Felix Moves the pillar on the village's northern ledge one cell east, off
/// the edge (onto cell (40, 13)): it drops into the pit there and fills it,
/// its top level with the ledge, so the party can walk across. It stays there
/// for good. A push that leaves it anywhere else does nothing.
void MadraCatacombs_DropPillarIntoHole(void) {
  struct Actor *actor = Actor_GetById(ACTOR_HOLE_PILLAR);
  s32 z;

  if (FIELD_CELL(actor->x.fixed) == 40) {
    z = FIELD_CELL(actor->z.fixed);
    if (z == 13) {
      Event_Start();
      Event_EndLastingPsynergy(0);
      Actor_DropAndWait(actor);
      MapGrid_CopyCellAttributes(41, 13, 2, 1, 39, z);
      Event_SetFlag(FLAG_MADRA_CATACOMBS_HOLE_FILLED);
      Event_End();
    }
  }
}

/// Nothing visible happens as Move takes hold of the plain stone pillar east of
/// the village's middle; the game frees the ground it stood on, so that
/// it can slide.
void MadraCatacombs_StartMovingPushedPillar(void) {
  Actor_LiftTargetBlock(gMadraCatacombs_PushedPillar);
}

/// Felix Moves the plain stone pillar east of the village's middle; once it is
/// one cell east, on cell (44, 26), it stays there on later visits, a stepping
/// stone to jump across from the upper level.
void MadraCatacombs_FinishMovingPushedPillar(void) {
  struct Actor *actor = Actor_GetById(ACTOR_PUSHED_PILLAR);

  Actor_PlaceMovedBlock();
  if (FIELD_CELL(actor->x.fixed) == 44 && FIELD_CELL(actor->z.fixed) == 26)
    Event_SetFlag(FLAG_MADRA_CATACOMBS_PILLAR_PLACED);
}

/// Felix casts Frost on the puddle just east of the broken building's south
/// doorway: it freezes into a pillar of ice Felix can jump onto, until the
/// party leaves the catacombs.
void MadraCatacombs_FreezePuddle(void) {
  FlagBlock_ApplyFlags(gMadraCatacombs_Puddles);
}

void FieldCamera_WaitForArrival(void);
void MadraCatacombs_LightFlames(void);

/// Felix uses the Ruin Key at the red double door down the stairs (cells (9,
/// 22) and (10, 22)): the two halves rumble apart, the camera climbs the hall
/// beyond to Moloch's tablet, and the braziers down both sides flare up in
/// pairs from the door upwards, the two great pairs by the tablet last. The
/// key is used up.
void MadraCatacombs_UnlockWithRuinKey(void) {
  struct Actor *left = Actor_GetById(ACTOR_LEFT_DOOR);
  struct Actor *right = Actor_GetById(ACTOR_RIGHT_DOOR);
  u32 i;

  Event_Start();
  Event_EndLastingPsynergy(0);
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
/// The vine-covered pillar becomes ground to stand on where Move left it; on
/// cell (35, 17) it stays there on later visits.
void MadraCatacombs_SettleVinePillar(struct Actor *actor);
/// Coming up into the far end of the Champa tunnel's upper path (cell (51,
/// 13)) once that way is blocked off: Felix vanishes and the screen fades back
/// the way he came.
void MadraCatacombs_LeaveBlockedTunnel(void);

/// Casting Reveal in the village shows a push button in the wall just east of
/// the golden door on the south side (cell (36, 38)), pressed in once the door
/// has opened.
void MadraCatacombs_RevealButton(void) {
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN) != 0)
    MapGrid_CopyCellsTo(72, 63, 36, 38, 1, 2);
  else
    MapGrid_CopyCellsTo(71, 63, 36, 38, 1, 2);
  Event_SetFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED);
}

/// As Reveal wears off, the push button by the golden door disappears.
void MadraCatacombs_HideButton(void) {
  MapGrid_CopyCellsTo(70, 63, 36, 38, 1, 2);
  Event_ClearFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED);
}

/// Examining the wall just east of the golden door on the village's south side
/// (cell (36, 39)) reads "Look upon me with eyes of truth... Then the door will
/// open..." While Reveal shows the push button there, Felix presses it instead:
/// it rattles in, and the golden door slides open with a thump and stays open.
void MadraCatacombs_HearEyesOfTruth(void) {
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_BUTTON_REVEALED) != 0 &&
      Event_GetFlag(FLAG_MADRA_CATACOMBS_VILLAGE_DOOR_OPEN) == 0) {
    Event_Start();
    Event_EndLastingPsynergy(1);
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
    Event_EndLastingPsynergy(0);
    // "Look upon me with eyes of truth... Then the door will open..."
    FieldMessage_Show((s32)&MsgFieldEyesOfTruth, 1);
    Event_End();
  }
}

/// Felix casts Tremor on the bookshelf in the bedroom behind the west door of
/// the small hall on the broken building's upper floor: it rocks, and the red
/// chest on top creeps west each time it rights itself. Once the chest is far
/// enough over the edge it falls with a puff of dust, through the floor to the
/// room below, where it waits with the Ruin Key.
void MadraCatacombs_ShakeBookshelf(void) {
  struct Actor *actor;
  FIXED x;

  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL) == 0) {
    actor = Actor_GetById(ACTOR_SHELF_CHEST);
    x = actor->x.fixed;
    if (x < (toFIXED(186))) {
      Event_Start();
      Event_EndLastingPsynergy(0);
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

/// Walking east into the vine-covered pillar from cell (33, 17) while Move
/// holds it pushes it a cell on.
void MadraCatacombs_PushVinePillar(void) {
  struct Actor *actor;

  if (Leader_CanPushObjectAhead() != 0) {
    actor = Actor_GetById(
        Field_FindNearbyTalkTarget(gMainRoutineState.leaderActorId));
    Pillar_LowerHeldCells(gMadraCatacombs_VinePillar);
    Leader_PushObjectAhead();
    MadraCatacombs_SettleVinePillar(actor);
  }
}

/// Nothing visible happens as Move takes hold of the vine-covered pillar north
/// of the village's middle; the game lowers the ground it made, so that it can
/// slide.
void MadraCatacombs_StartMovingVinePillar(void) {
  Pillar_LowerHeldCells(gMadraCatacombs_VinePillar);
}

void MadraCatacombs_OpenDoorAndEnter(s32 sourceX, s32 sourceZ, s32 x, s32 z,
                                     s32 width, s32 height);

/// Felix Moves the vine-covered pillar north of the village's middle; where it
/// comes to rest becomes ground to stand on, so he can climb its vines to the
/// upper level, and one cell east, on cell (35, 17), it stays on later visits.
void MadraCatacombs_FinishMovingVinePillar(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  if (state->movedBlock != 0)
    MadraCatacombs_SettleVinePillar(state->movedBlock);
}

/// The braziers down the hall before Moloch's tablet burn: each flame stands
/// in its place, a marker stands on each, and the doorway can be walked
/// through.
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

/// The red chest on the bookshelf tilts as the bookshelf rocks, and each time
/// the bookshelf comes upright again the chest hops two pixels west with a
/// scrape.
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

/// Felix walks north into one of the dark doorways of the village's broken
/// building, or the opening on its east side: he steps to its middle, walks in
/// growing smaller, and the screen closes to the room beyond. The doorway at
/// cell (17, 20) leads to the small hall of the upper floor, the one at (18,
/// 22) is the main entrance into the ground floor's hall, the one at (21, 17)
/// opens into the east room with the stairs down to the red double door, and
/// the opening at (54, 18) leads into the Champa tunnel.
void MadraCatacombs_EnterDoor(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct MainRoutineState *party = &gMainRoutineState;
  struct Actor *actor;
  s32 cell;

  actor = Actor_GetById(party->leaderActorId);
  Event_Start();
  Event_EndLastingPsynergy(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetAnimation(party->leaderActorId, ANIM_WALK);
  Actor_SetSpeed(party->leaderActorId, 0.5);
  cell = FIXEDtoINT(actor->x.fixed);
  Actor_SetMoveTarget(actor,
                      toFIXED(8 - cell % MAPGRID_CELL_PIXELS) + actor->x.fixed,
                      actor->y.fixed, actor->z.fixed);
  FieldActor_WaitForMove(actor);
  actor->facing = FACING_NORTH;
  Actor_SetScript(party->leaderActorId, gLeader_WalkThroughDoorScript);
  wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(state->stepTrigger);
}

/// A door opens, its open picture copied over it from (sourceX, sourceZ), and
/// Felix walks through: a stone door two cells wide slides with a thump, a door
/// one cell wide opens as he steps to its middle.
void MadraCatacombs_OpenDoorAndEnter(s32 sourceX, s32 sourceZ, s32 x, s32 z,
                                     s32 width, s32 height) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *actor;
  s32 cell;

  Event_Start();
  Event_EndLastingPsynergy(0);
  if (width == 2)
    Event_PlaySound(SE_THUMP);
  else
    Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(sourceX, sourceZ, x, z, width, height);
  Event_Wait(20);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  if (width == 1) {
    actor = Actor_GetById(gMainRoutineState.leaderActorId);
    cell = FIXEDtoINT(actor->x.fixed);
    Actor_SetMoveTarget(
        actor, toFIXED(8 - cell % MAPGRID_CELL_PIXELS) + actor->x.fixed,
        actor->y.fixed, actor->z.fixed);
    FieldActor_WaitForMove(actor);
    actor->facing = FACING_NORTH;
  }
  Actor_SetScript(gMainRoutineState.leaderActorId,
                  gLeader_WalkThroughDoorScript);
  wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(state->stepTrigger);
}

/// Felix walks north up the purple carpet of the broken building's main hall
/// into the golden stone door at its end (cells (14, 33) and (15, 33)): it
/// slides open and he goes through to the room beyond, where the chest from
/// the bookshelf falls.
void MadraCatacombs_OpenStoneDoor(void) {
  MadraCatacombs_OpenDoorAndEnter(78, 29, 78, 31, 2, 2);
}

/// Felix walks into the west door of the small hall on the broken building's
/// upper floor (cell (45, 5)): it opens and he goes through to the bedroom with
/// the bookshelf.
void MadraCatacombs_OpenBookshelfRoomWestDoor(void) {
  MadraCatacombs_OpenDoorAndEnter(109, 1, 109, 3, 1, 2);
}

/// Felix walks into the east door of the small hall on the broken building's
/// upper floor (cell (54, 5)): it opens and he goes through to the bedroom with
/// the broken floor.
void MadraCatacombs_OpenBookshelfRoomEastDoor(void) {
  MadraCatacombs_OpenDoorAndEnter(118, 1, 118, 3, 1, 2);
}

/// Felix walks into the door in the north-west corner of the small room over
/// the drop (cell (28, 33)): it opens and he goes through to the narrow passage
/// with a red chest.
void MadraCatacombs_OpenTabletRoomDoor(void) {
  MadraCatacombs_OpenDoorAndEnter(92, 29, 92, 31, 1, 2);
}

/// Arriving by a way that is blocked off once Briggs is caught, Felix vanishes
/// and the screen fades out to `exit`.
#define DEFINE_LEAVE_BLOCKED_WAY(name, exit)                                   \
  void name(void) {                                                            \
    Event_Start();                                                             \
    Event_EndLastingPsynergy(0);                                               \
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);              \
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineState.leaderActorId), 0);   \
    Actor_SetSpriteColor(Actor_GetById(gMainRoutineState.leaderActorId),       \
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

DEFINE_LEAVE_BLOCKED_WAY(MadraCatacombs_LeaveBlockedTunnel,
                         CHAMPA_TUNNEL_UPPER_PATH_END)

s32 Scene_Initialize(void) {
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_ENTRANCE)
    MadraCatacombs_SetUpVillage();
  else if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_CHAMPA_TUNNEL)
    MadraCatacombs_SetUpChampaTunnel();
  else if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_STONE_DOOR)
    MadraCatacombs_SetUpStoneDoorRoom();
  else if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_BOOKSHELF)
    MadraCatacombs_SetUpBookshelfRoom();
  else if (gMainRoutineState.next == SCENE_MADRA_CATACOMBS_TABLET)
    MadraCatacombs_SetUpTabletRoom();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Sets up the deserted village: its rope, the pillars Move has placed, the
/// frozen puddle, the opened golden door and, once Briggs is caught, the
/// Champa tunnel's way in blocked off.
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
    if (gMainRoutineState.entrance ==
        MADRA_CATACOMBS_ENTRANCE_FROM_MADRA_CATACOMBS_CHAMPA_TUNNEL_WEST)
      MadraCatacombs_LeaveBlockedVillage();
    else
      wait(10);
  }
}

/// Sets up the Champa tunnel: its rope and, once Briggs is caught, the way on
/// to the village blocked off and the man who marvels at the cave gone.
void MadraCatacombs_SetUpChampaTunnel(void) {
  Rope_Initialize();
  FieldRope_Place(0, FLAG_TUNNEL_ROPE_TIED, ACTOR_TUNNEL_ROPE_EAST_POST,
                  ACTOR_TUNNEL_ROPE_WEST_POST);
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT) != 0) {
    Actor_SetPosition(ACTOR_MAN_ON_THE_CHAMPAS_CAVE, 0, 0);
    MapGrid_CopyCellsTo(68, 70, 113, 11, 5, 3);
    MapGrid_CopyCellsTo(50, 74, 50, 75, 2, 1);
    MapGrid_CopyCellAttributes(49, 9, 5, 1, 49, 13);
    if (gMainRoutineState.entrance == CHAMPA_TUNNEL_UPPER_PATH_END)
      MadraCatacombs_LeaveBlockedTunnel();
    else
      wait(1);
  }
}

/// Sets up the broken building's ground floor, below the bookshelf: the chest
/// with the Ruin Key is there only once Tremor has shaken it down.
void MadraCatacombs_SetUpStoneDoorRoom(void) {
  if (Event_GetFlag(FLAG_MADRA_CATACOMBS_CHEST_FELL) == 0)
    Actor_SetPosition(ACTOR_FALLEN_CHEST, 0, 0);
}

/// Sets up the broken building's upper floor: the red chest stands on top of
/// the bookshelf until Tremor has shaken it down.
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

/// Sets up the hall before Moloch's tablet: the red double door's two halves
/// stand closed; once the Ruin Key has opened it they stand 12 pixels apart
/// (already so when the party comes back from a battle) and the braziers
/// burn.
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
#include "shared/leader_walk_through_door_script.c"

/// A brazier's flame's actor script as it flares up.
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

/// The actor script of the great braziers by Moloch's tablet as they flare up.
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

/// The plain stone pillar Move pushes, ended by -1.
s16 gMadraCatacombs_PushedPillar[] = {
    ACTOR_PUSHED_PILLAR, -1,
};

/// The puddle Frost freezes into a pillar, with its flag.
s16 gMadraCatacombs_Puddles[] = {
    ACTOR_VILLAGE_PUDDLE, FLAG_VILLAGE_PUDDLE_FROZEN, -1,
};

/// The vine-covered pillar Move pushes, ended by -1.
s16 gMadraCatacombs_VinePillar[] = {
    ACTOR_VINE_PILLAR, -1, 0,
};

#include "shared/scene_entrances_default.c"

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

/// Climbing either ladder up out of the ladder room takes the party up into
/// Madra.
struct SceneRegion gMadraCatacombs_LadderRoomRegions[] = {
    {112, 48, 144, 128, 64, 160, CONDITION_ALWAYS, MADRA_CATACOMBS_LADDERS_TO_MADRA_WEST},
    {336, 48, 144, 352, 64, 160, CONDITION_ALWAYS, MADRA_CATACOMBS_LADDERS_TO_MADRA_EAST},
    {SCENE_TABLE_END},
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
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_BOOKSHELF, CONDITION_ALWAYS, EVENT(MadraCatacombs_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_STONE_DOOR_NORTH, CONDITION_ALWAYS, EVENT(MadraCatacombs_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_WEST, CONDITION_ALWAYS, EVENT(MadraCatacombs_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_STONE_DOOR_SOUTH, CONDITION_ALWAYS, EVENT(MadraCatacombs_EnterDoor)},
    {TRIGGER_EXIT, MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_EAST, CONDITION_ALWAYS, MADRA_CATACOMBS_ENTRANCE_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_EAST},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_VILLAGE_ROPE_LOW_POST, FLAG_VILLAGE_ROPE_TIED, 0},
    {TRIGGER_PUSHED, 10, FLAG_MADRA_CATACOMBS_HOLE_FILLED, EVENT(MadraCatacombs_DropPillarIntoHole)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HOLE_PILLAR, FLAG_MADRA_CATACOMBS_HOLE_FILLED, EVENT(MadraCatacombs_DropPillarIntoHole)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PUSHED_PILLAR, FLAG_MADRA_CATACOMBS_PILLAR_PLACED, EVENT(MadraCatacombs_StartMovingPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_PILLAR, FLAG_MADRA_CATACOMBS_PILLAR_PLACED, EVENT(MadraCatacombs_FinishMovingPushedPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_MADRA_CATACOMBS_VINE_PILLAR_PUSH, FLAG_MADRA_CATACOMBS_VINE_PILLAR_PLACED, EVENT(MadraCatacombs_PushVinePillar)},
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
    {TRIGGER_EXIT, MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_NORTH, CONDITION_ALWAYS, MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_NORTH},
    {TRIGGER_EXIT, MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_SOUTH, CONDITION_ALWAYS, MADRA_CATACOMBS_LADDERS_TO_MADRA_CATACOMBS_CHAMPA_TUNNEL_SOUTH},
    {TRIGGER_TABLE_END},
};
// clang-format on

/// The triggers of the Champa tunnel.
struct Trigger gMadraCatacombs_ChampaTunnelTriggers[] = {
    {TRIGGER_EXIT,
     MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_WEST,
     CONDITION_ALWAYS,
     MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_WEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT,
     MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_SOUTH,
     CONDITION_ALWAYS,
     MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_SOUTH},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT,
     MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_EAST,
     CONDITION_ALWAYS,
     MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_ENTRANCE_EAST},
    {TRIGGER_EXIT,
     MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_NORTH,
     CONDITION_ALWAYS,
     MADRA_CATACOMBS_CHAMPA_TUNNEL_TO_MADRA_CATACOMBS_LADDERS_NORTH},
    {TRIGGER_EXIT, CHAMPA_TUNNEL_UPPER_PATH_END, CONDITION_ALWAYS,
     CHAMPA_TUNNEL_UPPER_PATH_END},
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
    {TRIGGER_EXIT,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_NORTHWEST,
     CONDITION_ALWAYS,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_NORTHWEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_EAST,
     CONDITION_ALWAYS,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_EAST},
    {TRIGGER_EXIT, MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_EAST,
     CONDITION_ALWAYS,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_EAST},
    {TRIGGER_EXIT, MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_WEST,
     CONDITION_ALWAYS,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_TABLET_WEST},
    {TRIGGER_EXIT, MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_NORTH,
     CONDITION_ALWAYS,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_NORTH},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS,
     EVENT(MadraCatacombs_OpenStoneDoor)},
    {TRIGGER_EXIT,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_SOUTH,
     CONDITION_ALWAYS,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_BOOKSHELF_SOUTH},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_SOUTH,
     CONDITION_ALWAYS,
     MADRA_CATACOMBS_STONE_DOOR_TO_MADRA_CATACOMBS_ENTRANCE_SOUTH},
    {TRIGGER_TABLE_END},
};

/// The triggers of the bookshelf room.
struct Trigger gMadraCatacombs_BookshelfTriggers[] = {
    {TRIGGER_EXIT,
     MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_NORTHWEST,
     CONDITION_ALWAYS,
     MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_NORTHWEST},
    {TRIGGER_EXIT, MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_EAST,
     CONDITION_ALWAYS,
     MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_EAST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
     EVENT(MadraCatacombs_OpenBookshelfRoomWestDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
     EVENT(MadraCatacombs_OpenBookshelfRoomEastDoor)},
    {TRIGGER_EXIT,
     MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_SOUTH,
     CONDITION_ALWAYS,
     MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_STONE_DOOR_SOUTH},
    {TRIGGER_EXIT, MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_ENTRANCE,
     CONDITION_ALWAYS, MADRA_CATACOMBS_BOOKSHELF_TO_MADRA_CATACOMBS_ENTRANCE},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_BOOKSHELF, CONDITION_ALWAYS,
     EVENT(MadraCatacombs_ShakeBookshelf)},
    {TRIGGER_TABLE_END},
};

/// The triggers of the tablet room.
struct Trigger gMadraCatacombs_TabletTriggers[] = {
    {TRIGGER_EXIT, MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_WEST,
     CONDITION_ALWAYS,
     MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_WEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(MadraCatacombs_OpenTabletRoomDoor)},
    {TRIGGER_EXIT, MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_EAST,
     CONDITION_ALWAYS,
     MADRA_CATACOMBS_TABLET_TO_MADRA_CATACOMBS_STONE_DOOR_EAST},
    {USE_ITEM(ITEM_RUIN_KEY), CELL_BEHAVIOR_MADRA_CATACOMBS_TABLET_DOOR,
     CONDITION_ALWAYS, EVENT(MadraCatacombs_UnlockWithRuinKey)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_MADRA_CATACOMBS_TABLET_DOOR,
     FLAG_MADRA_CATACOMBS_TABLET_DOOR_OPEN,
     EVENT(MadraCatacombs_TryLockedDoor)},
    {TRIGGER_TABLE_END},
};
