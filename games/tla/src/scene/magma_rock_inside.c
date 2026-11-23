/// @file
/// Magma Rock's inside, the rooms in the volcano that the great blast on its
/// top opened. Three halls are each watched by a great stone face: Burst on
/// the small statue beside it makes the statue spit a fireball into the face,
/// whose eyes light red as it pours lava over the hall's floor, and a floor
/// switch beside a grated gate lets the lava drain away again. While the lava
/// is up, platforms float round it and Felix leaps from its edge onto them, or
/// onto the pillars he has pushed into it. A Mars Djinni left over from the
/// first journey may wait in the entrance hall's south, Fury waits in its own
/// room, and the Lucky Medal, Mist Potion, Salamander Tail and Golem Core lie
/// in chests. Deep inside, stepping onto an islet in the lava sets off a rock
/// that bursts out of it, throwing burning rocks and flames round the room; a
/// crest on the stone wall there opens into a stairway to a sea of lava crossed
/// on stepping stones, where Jenna learns Blaze at the tablet. Blaze then
/// carries a flame to a torch to sink the pillar on the way out, and back in
/// the ember stone's room carries the fallen flames to the two torches that let
/// the party reach the stone and take the Magma Ball from it.

#include "sprite_ids.h"
#include "graphics/palette_buffers.h"
#include "djinn_ids.h"
#include "scene/falling_rocks.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_prepare.c"

#include "types.h"
#include "item_ids.h"
#include "flags.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/scene_tables.h"
#include "system/iwram_call.h"
#include "scene_ids.h"
#include "system/frame.h"
#include "field/event_inline.h"
#include "field/map_grid.h"
#include "field/field_sequence_event.h"
#include "field/field_sequence.h"
#include "field/field.h"
#include "field/field_coordinate.h"
#include "field/object.h"
#include "scene/magma_rock.h"
#include "scene/mars_lighthouse_aerie.h"
#include "scene/scene.h"
#include "system/scene.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/io_reg.h"
#include "system/field_effect.h"
#include "macros.h"
#include "field/field_sprite.h"
#include "field/call.h"
#include "edition.h"
#include "system/heap_release.h"
#include "window/item_icon.h"
#include "field/field_effect.h"
#include "field/particle.h"
#include "graphics/resource.h"
#include "system/task.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "text/msg_ids.h"

extern struct MapCell gMapGridCellBuffer[];

void Scene_SetActorSpritePriority(struct Actor *actor, s32 priority);
void Actor_SetBlendMode(struct Actor *actor, u32 value);

extern s32 gMagmaRockInside_FlameScript[];

extern s32 gMagmaRockInside_LastStepCell;

void MagmaRockInside_BurstCrackedPillar(s32 stage, s32 cue);
void MagmaRockInside_TrackStepCell(void);
void Actor_SetCellShape(s32 heightOffset, s32 layer, s32 owner);
void Actor_StartScript(struct Actor *actor, const void *data);
void Actor_DisableById(s32 id);

/// Creates an actor of the given sprite through the overlay's import.
static __inline__ struct Actor *Create(s32 kind, s32 x, s32 y, s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}
#include "shared/trial_road.h"
#include "shared/tables.h"
#include "shared/tundaria_tower.h"

// The lava rises in three parts of the volcano, each when a statue's fireball
// strikes the great stone face there, and drains when a floor switch by that
// part's gate is pressed; the flags hold the state until the party comes back
// in from outside.
#define FLAG_ENTRANCE_FLOODED FLAG_AREA_TEMP_00
#define FLAG_FURY_FLOODED FLAG_AREA_TEMP_01
#define FLAG_LOWER_ROOMS_FLOODED FLAG_AREA_TEMP_02
#define FLAG_WEST_TORCH_LIT FLAG_AREA_TEMP_03
#define FLAG_EAST_TORCH_LIT FLAG_AREA_TEMP_04
#define FLAG_TABLET_TORCH_LIT FLAG_AREA_TEMP_05
// Set as Burst breaks each of the cracked pillars' room's two pillars, until
// the party leaves Magma Rock.
#define FLAG_INSIDE_CRACKED_PILLAR_A_BURST FLAG_AREA_TEMP_50
#define FLAG_INSIDE_CRACKED_PILLAR_B_BURST FLAG_AREA_TEMP_51
// Each pushed pillar's flag keeps it where it was pushed.
#define FLAG_ENTRANCE_PILLAR_PUSHED 0x9f0
#define FLAG_FURY_PILLAR_PUSHED 0x9f1
#define FLAG_PILLAR_A_PUSHED 0x9f2
#define FLAG_PILLAR_B_PUSHED 0x9f3
#define FLAG_PILLAR_C_PUSHED 0x9f4
#define FLAG_NORTH_PILLAR_PUSHED 0x9f5
#define FLAG_EAST_PILLAR_PUSHED 0x9f6
#define FLAG_LEDGE_PILLAR_PUSHED 0x9f7
#define FLAG_ROCKS_RELEASED 0x9f8
#define FLAG_MAGMA_BALL_TAKEN 0x9f9
#define FLAG_BLAZE_LEARNED 0x9fa
// The Blaze tablet's room: a stone on the way to the tablet that the map's
// upper layer numbers raises the trigger STEP_CELL_TRIGGER_BASE plus its number
// when Felix stops on it, and its flag keeps the stone behind it raised.
#define FLAG_ANY_MARKER_LIT FLAG_SCENE_TEMP_00
#define FLAG_MARKER_LIT(marker) (FLAG_SCENE_TEMP_00 + (marker))
enum { STEP_CELL_TRIGGER_BASE = 200 };

/// The cell behaviours the rooms' maps mark their spots with, which the
/// trigger tables answer. A room may use a number in its own sense.
enum MagmaRockInsideCellBehavior {
  /// A floor switch beside a lava gate; the Golem Core's room has a second.
  CELL_FLOOR_SWITCH = 30,
  CELL_SECOND_FLOOR_SWITCH = 31,
  /// The ember stone's room: the islets in the lava that set the stone off,
  /// and the stone where it lands.
  CELL_ERUPTION_ISLET = 30,
  CELL_EMBER_STONE = 31,
  /// The Blaze tablet, read from any side.
  CELL_BLAZE_TABLET = 30,
  /// The cell before a fire-spitting statue that Burst is cast at.
  CELL_STATUE = 50,
  CELL_LOWER_WEST_STATUE = 51,
  CELL_LOWER_EAST_STATUE = 52,
  /// The lava's edge, where Felix leaps onto what stands in the lava; the
  /// cracked pillars' room's two pillars stand on edge cells of their own.
  CELL_LAVA_EDGE = 70,
  CELL_CRACKED_PILLAR_A = 71,
  CELL_CRACKED_PILLAR_B = 72,
  CELL_LAVA_EDGE_NORTH = 73, ///< leapt from walking north
  CELL_LAVA_EDGE_WEST = 75,  ///< leapt from walking west
  CELL_LAVA_EDGE_EAST = 78,  ///< the far side of the Lucky Medal room's lava
  /// The Blaze tablet room's stepping stones, two pictures of them in turn.
  CELL_STEPPING_STONE_A = 80,
  CELL_STEPPING_STONE_B = 81
};

/// The room past the Lucky Medal's chest: one lava platform.
enum MagmaRockInsideLuckyMedalChestActorId {
  ACTOR_LUCKY_MEDAL_PLATFORM = ACTOR_FIRST_PLACED
};

/// The entrance hall: the small statue whose fireball sets the great face
/// flooding it, the row of columns standing across its lava, the pillar pushed
/// into that row, an unseen marker on its floor switch and the Mars Djinni
/// "Fire19".
enum MagmaRockInsideEntranceActorId {
  ACTOR_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_COLUMN_A,
  ACTOR_COLUMN_B,
  ACTOR_COLUMN_C,
  ACTOR_COLUMN_D,
  ACTOR_COLUMN_E,
  ACTOR_COLUMN_F,
  ACTOR_ENTRANCE_PILLAR,
  ACTOR_ENTRANCE_SWITCH_MARKER,
  ACTOR_FIRE19 ///< the Mars Djinni left over from the first journey
};

/// Fury's room: the statue, an unseen marker on its floor switch, two lava
/// platforms, the pillar pushed into the lava, the tightrope's posts and Fury.
enum MagmaRockInsideFuryActorId {
  ACTOR_FURY_SWITCH_MARKER = ACTOR_FIRST_PLACED + 1,
  ACTOR_WEST_PLATFORM,
  ACTOR_FURY_PILLAR,
  ACTOR_EAST_PLATFORM,
  ACTOR_TIGHTROPE_WEST_POST,
  ACTOR_TIGHTROPE_EAST_POST,
  ACTOR_FURY
};

/// The room of the cracked pillars: three statues, the two cracked pillars
/// Burst breaks, ten lava platforms, three pillars to push and a column.
enum MagmaRockInsideCrackedPillarsActorId {
  ACTOR_CRACKED_PILLAR_A = ACTOR_FIRST_PLACED + 1,
  ACTOR_CRACKED_PILLAR_B = ACTOR_FIRST_PLACED + 3,
  ACTOR_LOWER_WEST_STATUE = ACTOR_FIRST_PLACED + 5,
  ACTOR_LOWER_EAST_STATUE,
  ACTOR_LAVA_PLATFORM_A,
  ACTOR_LAVA_PLATFORM_B,
  ACTOR_LAVA_PLATFORM_C,
  ACTOR_LAVA_PLATFORM_D,
  ACTOR_LAVA_PLATFORM_E,
  ACTOR_LAVA_PLATFORM_F,
  ACTOR_LAVA_PLATFORM_G,
  ACTOR_LAVA_PLATFORM_H,
  ACTOR_PILLAR_A,
  ACTOR_PILLAR_B,
  ACTOR_PILLAR_C,
  ACTOR_LOWER_COLUMN
};

/// The Golem Core's room: unseen markers on its two floor switches and the two
/// chains hanging down its wall either side of its doorway, which Whirlwind
/// swings.
enum MagmaRockInsideGolemCoreChestActorId {
  ACTOR_NORTH_SWITCH_MARKER = ACTOR_FIRST_PLACED,
  ACTOR_SOUTH_SWITCH_MARKER,
  ACTOR_HANGING_CHAIN_A,
  ACTOR_HANGING_CHAIN_B
};

/// The Salamander Tail's room: three lava platforms and two pillars to push.
enum MagmaRockInsideSalamanderTailChestActorId {
  ACTOR_NORTH_PLATFORM = ACTOR_FIRST_PLACED,
  ACTOR_NORTH_PILLAR,
  ACTOR_SOUTHWEST_PLATFORM,
  ACTOR_SOUTHEAST_PLATFORM,
  ACTOR_EAST_PILLAR
};

/// The room of the ember stone: the sunken pillars, the pillar the two torches
/// lower, the stone that holds the Magma Ball, the rocks and flames that fall
/// with it, the torches' flames and a pillar to push.
enum MagmaRockInsideEmberStoneActorId {
  ACTOR_SUNKEN_PILLAR_A = ACTOR_FIRST_PLACED,
  ACTOR_SUNKEN_PILLAR_B,
  ACTOR_SUNKEN_PILLAR_C,
  ACTOR_BLOCKING_PILLAR,
  ACTOR_EMBER_STONE,
  ACTOR_FALLEN_ROCK_A,
  ACTOR_FALLEN_ROCK_B,
  ACTOR_FALLEN_ROCK_C,
  ACTOR_FALLEN_ROCK_D,
  ACTOR_FALLEN_FLAME_A,
  ACTOR_FALLEN_FLAME_B,
  ACTOR_FALLEN_FLAME_C,
  ACTOR_FALLEN_FLAME_D,
  ACTOR_WEST_TORCH_FLAME,
  ACTOR_EAST_TORCH_FLAME,
  ACTOR_LEDGE_PILLAR
};

/// The flag that keeps a torch of the ember stone's room lit, by its flame's
/// actor: FLAG_WEST_TORCH_LIT or FLAG_EAST_TORCH_LIT.
#define FLAG_TORCH_LIT(flame)                                                  \
  ((flame) - ACTOR_WEST_TORCH_FLAME + FLAG_WEST_TORCH_LIT)

/// A torch Blaze lights: its flame starts a sixteenth of its size and grows by
/// a sixteenth FLAME_GROW_STEPS times, 5 frames apart; then the pillar the
/// torches guard sinks PILLAR_SINK_DEPTH pixels into the floor, a pixel every
/// 5 frames, as deep as the ember stone room's sunken pillars stand.
enum { FLAME_GROW_STEPS = 16 };

/// The quarter turn a facing lies in, as Felix faces a switch or the lava: an
/// eighth of a turn added, then this kept.
#define FACING_QUARTER_MASK (TRIG_TURN - DEGtoANG(90))

/// What MagmaRockInside_RaiseLava and MagmaRockInside_DrainLava keep while the
/// lava is already up; only its being nonzero is read.
enum { LAVA_ALREADY_UP = 212 };

/// A lava platform's path script is four corners, each a move and a wait for
/// arrival, five words: the second platform of each pair of the cracked
/// pillars' room starts at this word, the third corner, across from the first.
enum { PLATFORM_PATH_THIRD_CORNER = 10 };

/// The Blaze tablet's room: the tablet, and on the ledge in the lava's
/// south-west the pillar in the way out, the flame burning in the west bowl
/// beside it, which Blaze carries, and the flame of the east bowl it lights.
enum MagmaRockInsideBlazeTabletActorId {
  ACTOR_BLAZE_TABLET = ACTOR_FIRST_PLACED,
  ACTOR_TORCH_PILLAR,
  ACTOR_SOURCE_FLAME,
  ACTOR_TORCH_FLAME
};

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

#include "shared/position_distance.h"

#include "../field/actor_find_in_cell.c"

// Magma Rock's inside: the lava floods and drains, its platforms, statues and
// pillars, the Blaze tablet and the flames and torches it opens the way with.

struct Actor;
struct FieldSprite;
struct Particle;
struct ParticleSprite;

void MagmaRockInside_PausePlatform();

extern s16 gMagmaRockInside_EntrancePillars[];

extern s16 gMagmaRockInside_FuryPillars[];

extern s16 gMagmaRockInside_CrackedPillarsPillars[];

extern s16 gMagmaRockInside_EmberStonePillars[];

void Blocks_MarkCells(u16 *ids);

void MagmaRockInside_WaitForLanding(struct Actor *actor);

s32 MapCell_GetAttributes(u32 layer, s32 x, s32 y);

s32 Statue_FindUnderBurst(u16 *table);

s32 Position_FlatDistance(FIXED *from, FIXED *to);

void MagmaRockInside_TrailEmbers(struct Actor *actor);
void MagmaRockInside_RaiseLava(void);

void MagmaRockInside_DropRocks(s32 x, s32 y, s32 z, s32 count);

void MagmaRockInside_StartFallingRocks(void);

void MagmaRockInside_SetEntranceFlooded(void);
void MagmaRockInside_SetFuryFlooded(void);
void MagmaRockInside_SetLowerRoomsFlooded(void);

extern s32 gMagmaRockInside_CrackedPillarsFirstPairPath[];
extern s32 gMagmaRockInside_CrackedPillarsSecondPairPath[];
extern s32 gMagmaRockInside_CrackedPillarsThirdPairPath[];
extern s32 gMagmaRockInside_CrackedPillarsFourthPairPath[];

void FieldEffect_StartBlankOverlay(s32 value);

void MagmaRockInside_RaiseActorCells(u16 *list);

void HangingVine_Place(s32 id, s32 value);
s32 Actor_GetCellIndex(struct Actor *actor);
s32 Field_TryJumpForward(void);
struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

void Dust_Scatter(s32 spread);
void Leader_SetCellShapeAt(s32 heightOffset, s32 layer);
void MagmaRockInside_SpawnDustRing(void);
extern u32 gDust_ScatterScript[];

extern s32 gMagmaRockInside_DustScript[];

/// Decompresses a tagged LZ resource through the overlay's import.
void MagmaRockInside_DecompressTaggedLz(void *entry, void *destination);
/// Sets up an effect sprite of the given size, attributes and tile, through the
/// overlay's import.
void MagmaRockInside_InitRockSprite(struct EffectSprite *sprite, s32 width,
                                    s32 height, u32 attributes, s32 tile);

void Particle_UpdateFalling(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

void MagmaRockInside_BurstCrackedPillar(s32 stage, s32 cue);

void Actor_LiftTargetBlock(s16 *cells);
void Actor_PlaceMovedBlock(void);
void Import_FieldPsynergySetupActorPair(s32 owner, u16 value);
void Actor_PlaySoundInView(struct Actor *actor, s32 sound);
s32 gMagmaRockInside_StepCellWidth BSS_DATA;
s32 gMagmaRockInside_StepCellHeight BSS_DATA;
s32 gMagmaRockInside_StepCellX BSS_DATA;
s32 gMagmaRockInside_StepCellZ BSS_DATA;

extern s32 gPushLeaderSteps[];

#include "shared/block_push_onto.h"

extern struct MapCell gMapGridCellBuffer[];

/// Sets the collision code of each cell in a layer rectangle to value.
/// Layers 3 and above use the shared buffer.
s32 MagmaRockInside_SetCollisionRect(u32 layer, u32 x, u32 y, u32 width,
                                     u32 height, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapCell *cells;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cells = map->layers[layer].cells;
  else
    cells = gMapGridCellBuffer;
  cells += x + (y << 7);
  for (x = 0; x < height; x++) {
    struct MapCell *cell = cells + (x << 7);

    for (y = 0; y < width; y++, cell++)
      cell->collisionCode = value;
  }
  return 0;
}

/// Talking to Fury, the Mars Djinni on the ledge just west of the north doorway
/// into its room's east hall, starts a battle with it.
void MagmaRockInside_FightFury(void) {
  Field_RequestDjinnBattle(ACTOR_FURY, DJINNI_FURY);
}

/// Talking to the Mars Djinni in the entrance hall's south (it stands at cell
/// (18, 31)) makes it join the party. It is always the Djinni the text calls
/// "Fire19" (DJINNI_FIRE19), wearing the Mars Djinni's sprite, and it stands
/// there only while that Djinni has not joined (its FLAG_DJINNI); it is kept
/// for one of The Broken Seal's Mars Djinn the party never found there, and
/// the walkthrough says such a Djinni may wait here.
void MagmaRockInside_MeetDjinni(void) {
  Djinn_JoinParty(ACTOR_FIRE19, DJINNI_FIRE19 / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_FIRE19));
}

#include "shared/position_flat_distance.h"

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

#include "shared/map_cell_set_shape.h"

/// Waits up to 60 frames for an actor's vertical speed to come to zero.
void MagmaRockInside_WaitForLanding(struct Actor *actor) {
  s32 frames;
  s32 busy;

  frames = 60;
  while (frames != 0) {
    System_Wait(1);
    busy = actor->velocityY;
    frames--;
    if (busy == 0) {
      break;
    }
  }
}

#include "../field/map_cell_get.c"

#include "shared/map_cell_set.c"

#include "shared/blocks_mark_cells.c"

// Clears the object's mark and the cells ahead of and behind it that the mark
// blocked.
#include "shared/block_clear_cells.h"

/// A lava platform's update: holds it still while a menu, Psynergy, an item or
/// a transition is in progress.
void MagmaRockInside_PausePlatform(struct Actor *actor) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u8 *state = &actor->paused;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0 || work->transitionActive == 0)
    *state = 1;
  else
    *state = 0;
}

void Actor_StartScript();
s32 GameFlag_Get();

/// Sets a white lava platform afloat: risen and running round its path while
/// flag (its part's lava) is set, sunk under the floor otherwise; its update
/// holds it still during menus.
void MagmaRockInside_StartLavaPlatform(s32 id, s32 *path, s16 flag) {
  struct Actor *platform;
  s32 risen;

  platform = Actor_GetById(id);
  Actor_SelectAnimation(platform, 4);
  platform->steeringFlags = 0;
  platform->speedLimit = toFIXED(1) / 2;
  platform->acceleration = toFIXED(1) / 2;
  platform->motionFlags = ACTOR_FALLS;
  platform->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
  Actor_SetSpriteFlags(platform, 0);
  if (Event_GetFlag((u16)flag)) {
    platform->groundHeight = toFIXED(16);
    platform->y.fixed = toFIXED(16);
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
      Actor_StartScript(platform, path);
  } else {
    platform->groundHeight = -toFIXED(14);
    platform->y.fixed = -toFIXED(14);
  }
  platform->update = (ObjectUpdate)MagmaRockInside_PausePlatform;
}

s32 MagmaRockInside_SetCellShape(s32 terrain, s32 x, s32 z, s32 height);

#include "shared/actors_raise_cells.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gMagmaRockInside_Placements[];
extern struct ActorPlacement gMagmaRockInside_EntrancePlacements[];
extern struct ActorPlacement gMagmaRockInside_FuryPlacements[];
extern struct ActorPlacement gMagmaRockInside_CrackedPillarsPlacements[];
extern struct ActorPlacement gMagmaRockInside_GolemCoreChestPlacements[];
extern struct ActorPlacement gMagmaRockInside_SalamanderTailChestPlacements[];
extern struct ActorPlacement gMagmaRockInside_EmberStonePlacements[];
extern struct ActorPlacement gMagmaRockInside_BlazeTabletPlacements[];

extern s16 gMagmaRockInside_SalamanderTailChestPillars[];

/// The placed actors of the room the party is in.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene != SCENE_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST) {
    if (scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
      return gMagmaRockInside_EntrancePlacements;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_FURY)
      return gMagmaRockInside_FuryPlacements;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
      return gMagmaRockInside_CrackedPillarsPlacements;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST)
      return gMagmaRockInside_GolemCoreChestPlacements;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST)
      return gMagmaRockInside_SalamanderTailChestPlacements;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE)
      return gMagmaRockInside_EmberStonePlacements;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET)
      return gMagmaRockInside_BlazeTabletPlacements;
  }
  return gMagmaRockInside_Placements;
}

/// Nothing visible happens as Felix starts to push one of the yellow-topped
/// stone pillars, whether by walking into it or with Move; the game frees the
/// cells it walled off, so that it can slide.
void MagmaRockInside_StartPushingPillar(void) {
  struct FieldWork *work;
  s16 scene;

  work = Ram_HeapSlots->fieldWork;
  scene = gMainRoutineWork.next;
  if (scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
    Actor_LiftTargetBlock(gMagmaRockInside_EntrancePillars);
  else if (scene == SCENE_MAGMA_ROCK_INSIDE_FURY)
    Actor_LiftTargetBlock(gMagmaRockInside_FuryPillars);
  else if (scene == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
    Actor_LiftTargetBlock(gMagmaRockInside_CrackedPillarsPillars);
  else if (scene == SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST)
    Actor_LiftTargetBlock(gMagmaRockInside_SalamanderTailChestPillars);
  else if (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE)
    Actor_LiftTargetBlock(gMagmaRockInside_EmberStonePillars);
  MagmaRockInside_ClearBlockCells(work->movedBlock);
}

/// A yellow-topped stone pillar Felix has pushed comes to rest a cell further
/// on. Pushed off the ledge in the Salamander Tail's room or the ember stone's,
/// it drops with a thud, into the lava when it is up, sinking until only its
/// top shows. Once a pillar reaches its place its flag keeps it there when the
/// party comes back: the entrance hall's pushed north into the row of columns
/// (row 16 of the map), Fury's room's at row 41, the cracked pillars' room's
/// three at column 21, the Salamander Tail room's at columns 47 and 57 and the
/// ember stone room's at column 43.
void MagmaRockInside_FinishPushingPillar(void) {
  struct Actor *actor = gFieldWork->movedBlock;
  s32 scene;
  s32 x;
  s32 z;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_PlaceMovedBlock();
  scene = gMainRoutineWork.next;
  if (scene == SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST) {
    Blocks_MarkCells(gMagmaRockInside_SalamanderTailChestPillars);
    if (Map_GetTerrainHeight(2, actor->x.fixed, actor->z.fixed) !=
        actor->y.fixed) {
      actor->layer = 2;
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      System_Wait(1);
      if (Event_GetFlag(FLAG_LOWER_ROOMS_FLOODED)) {
        struct Actor *copy;

        actor->gravity = toFIXED(1) / 16;
        actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        REG_BLDALPHA = BLEND_ALPHA(0, 16);
        REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
        copy = OverlayObject_PrepareObject(actor->x.fixed, actor->groundHeight,
                                           actor->z.fixed + toFIXED(16),
                                           SPRITE_YELLOW_BLOCK);
        MagmaRockInside_WaitForLanding(actor);
        Event_PlaySound(SE_HEAVY_STOP);
        Actor_Release(copy);
        Actor_SelectAnimation(actor, 4);
        System_Wait(1);
        REG_BLDCNT = 0;
      } else {
        MagmaRockInside_WaitForLanding(actor);
        Event_PlaySound(SE_THUMP);
      }
      actor->motionFlags = 0;
      actor->layer = 0;
    }
  } else if (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE) {
    Blocks_MarkCells(gMagmaRockInside_EmberStonePillars);
    if (Map_GetTerrainHeight(2, actor->x.fixed, actor->z.fixed) !=
        actor->y.fixed) {
      struct Actor *copy;

      actor->layer = 2;
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      System_Wait(1);
      actor->gravity = toFIXED(1) / 16;
      actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      REG_BLDALPHA = BLEND_ALPHA(0, 16);
      REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
      copy = OverlayObject_PrepareObject(actor->x.fixed, actor->groundHeight,
                                         actor->z.fixed + toFIXED(16),
                                         SPRITE_YELLOW_BLOCK);
      MagmaRockInside_WaitForLanding(actor);
      Event_PlaySound(SE_HEAVY_STOP);
      Actor_Release(copy);
      Actor_SelectAnimation(actor, 4);
      System_Wait(1);
      REG_BLDCNT = 0;
      MagmaRockInside_WaitForLanding(actor);
      actor->motionFlags = 0;
      actor->layer = 0;
    }
  }
  x = FIELD_CELL(actor->x.fixed);
  z = FIELD_CELL(actor->z.fixed);
  scene = gMainRoutineWork.next;
  if (scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE) {
    if (z == 16)
      Event_SetFlag(FLAG_ENTRANCE_PILLAR_PUSHED);
  } else if (scene == SCENE_MAGMA_ROCK_INSIDE_FURY) {
    if (z == 41)
      Event_SetFlag(FLAG_FURY_PILLAR_PUSHED);
  } else if (scene == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS) {
    if (x == 21) {
      if (z == 43)
        Event_SetFlag(FLAG_PILLAR_A_PUSHED);
      if (z == 45)
        Event_SetFlag(FLAG_PILLAR_B_PUSHED);
      if (z == 47)
        Event_SetFlag(FLAG_PILLAR_C_PUSHED);
    }
  } else if (scene == SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST) {
    if (x == 47) {
      Event_SetFlag(FLAG_NORTH_PILLAR_PUSHED);
      MapGrid_CopyCellAttributes(46, 74, 1, 1, 46, 73);
    }
    if (x == 57)
      Event_SetFlag(FLAG_EAST_PILLAR_PUSHED);
  } else if (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE) {
    if (x == 43)
      Event_SetFlag(FLAG_LEDGE_PILLAR_PUSHED);
  }
  Event_End();
}

/// The overlay's import veneer to BattleEffect_StopMainActor.
s32 Import_BattleEffectStopMainActor(void);

/// Plays Burst from Felix at an actor, as when he casts it, for a statue's
/// fireball that strikes one: in the cracked pillars' room, the pillar it hits
/// breaks as if Burst had been cast at it.
void MagmaRockInside_PlayBurst(u16 value) {
  struct MainRoutineWork *party;

  FieldPsynergy_LoadResources(ABILITY_BURST, 1);
  party = &gMainRoutineWork;
  Import_FieldPsynergySetupActorPair(party->leaderActorId, value);
  FieldPsynergy_RunHandler(1);
  Import_BattleEffectStopMainActor();
  FieldPsynergy_End();
}

extern s32 gMagmaRockInside_EmberScript[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

/// A statue's fireball's update as it flies: it trails an ember every fourth
/// frame, a few pixels to either side, and whooshes every sixteenth.
void MagmaRockInside_TrailEmbers(struct Actor *actor) {
  struct EffectOptions options;
  u32 frame = gFrameCount;
  s32 randomZ;
  s32 vz;

  if ((frame & 3) != 0)
    return;
  options.type = SPRITE_FLAME_PUFF;
  options.priority = 2;
  options.script = gMagmaRockInside_EmberScript;
  if ((frame & 15) == 0)
    Actor_PlaySoundInView(actor, SE_WHOOSH);
  randomZ = Random16() & 15;
  vz = ((Random16() & 31) - 16) << 12;
  FieldEffect_Spawn(
      actor->x.fixed, actor->y.fixed, actor->z.fixed + ((randomZ - 8) << 15), 0,
      0, vz, EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | 1,
      &options);
}

#include "shared/statue_flash_under_burst.h"

#include "shared/statue_find_under_burst.h"

/// Nothing visible happens as Felix starts casting Burst at a fire-spitting
/// statue; the game finishes what the field still had pending, so that the
/// cast and the statue's fire play undisturbed.
void MagmaRockInside_ResolveBeforeBurst(void) {
  Event_ResolvePendingActions(0);
}

extern s16 gMagmaRockInside_Statues[];
extern s16 gMagmaRockInside_CrackedPillarsStatues[];

void Statue_FlashUnderBurst(struct Actor *actor);

/// As Felix casts Burst at the small statue beside a great stone face, the
/// statue blinks red, slowly through the small explosions and faster at the
/// last blast. In the entrance hall the statue's cell is (26, 12); in Fury's
/// room (20, 15); the cracked pillars' room has three, at (12, 12), (20, 33)
/// and (30, 33), and its own table.
void MagmaRockInside_TargetStatue(s32 stage) {
  u16 *table;
  struct FieldEffectWork *watch = Ram_HeapSlots->fieldEffectWork;
  s32 scene;

  table = gMagmaRockInside_Statues;
  FieldEvent_ResolvePendingActions(0);
  scene = gMainRoutineWork.next;
  if (scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE) {
  } else if (scene == SCENE_MAGMA_ROCK_INSIDE_FURY) {
  } else if (scene == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS) {
    table = gMagmaRockInside_CrackedPillarsStatues;
  }
  if (stage == BURST_STAGE_SMALL_BLAST) {
    struct Actor *actor;

    Statue_FindUnderBurst(table);
    actor = watch->child;
    actor->flashMask = 4;
    actor->update = (ObjectUpdate)Statue_FlashUnderBurst;
  }
  if (stage == BURST_STAGE_GREAT_BLAST) {
    struct Actor *actor;

    Statue_FindUnderBurst(table);
    actor = watch->child;
    actor->flashMask = 2;
    actor->update = (ObjectUpdate)Statue_FlashUnderBurst;
  }
}

/// An ember's update as a statue's fireball bursts: moves it by its velocity,
/// kept in bounce, gravity and riseBaseY, and slows it by an eighth.
void MagmaRockInside_DriftAndSlow(struct Actor *actor) {
  s32 dx, dy, dz;

  actor->x.fixed += dx = actor->bounce;
  actor->y.fixed += dy = actor->gravity;
  actor->z.fixed += dz = actor->riseBaseY;
  dx -= dx >> 3;
  dy -= dy >> 3;
  dz -= dz >> 3;
  actor->bounce = dx;
  actor->gravity = dy;
  actor->riseBaseY = dz;
}

extern s32 gMagmaRockInside_EmberScript[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x, s32 y,
                                                    s32 z) {
  return (struct Actor *)Import_ActorCreate(kind, x, y, z);
}

/// The frames a statue's fireball bursts where it lands, throwing out an ember
/// every other frame.
enum { FIRE_BURST_FRAMES = 16 };

/// A statue's fireball flies the way the statue faces, the camera following,
/// until it meets the edge of the fire's reach, a great stone face or
/// something standing in its path. It bursts against the edge or a stone
/// pillar; against a face it sets the face pouring lava
/// (MagmaRockInside_RaiseLava); against anything else it plays Burst there.
void MagmaRockInside_FireFromStatue(struct Actor *source) {
  struct Actor *flame = Import_ActorCreate_(SPRITE_FLAME_PUFF, source->x.fixed,
                                            source->y.fixed, source->z.fixed);
  struct Actor *other;
  FIXED position[3];
  struct EffectOptions options;
  s32 step = Trig_Cos(source->facing) << 1;
  s32 cell;
  s32 id;
  s32 bit;

  flame->motionFlags = 0;
  Actor_SelectAnimation(flame, 1);
  Actor_SetSpriteFlags(flame, 0);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  position[0] = flame->x.fixed;
  position[1] = flame->y.fixed;
  position[2] = flame->z.fixed;
  for (;;) {
    position[0] += step;
    cell =
        MapCell_GetAttributes(MAPGRID_LAYER_OVERHEAD, FIELD_CELL(position[0]),
                              FIELD_CELL(position[2] - position[1])) >>
        8;
    if (cell == CELL_BEHAVIOR_MAGMA_ROCK_FIRE_EDGE ||
        cell == CELL_BEHAVIOR_MAGMA_ROCK_LAVA_FALL)
      goto end;
    for (id = ACTOR_FIRST_PLACED; id != FIELD_TREASURE_ACTOR; id++) {
      other = gFieldWork->actors[id];
      if (other->script == 0)
        continue;
      if (FIELD_CELL(source->x.fixed) == FIELD_CELL(other->x.fixed) &&
          FIELD_CELL(source->y.fixed) == FIELD_CELL(other->y.fixed) &&
          FIELD_CELL(source->z.fixed) == FIELD_CELL(other->z.fixed))
        continue;
      if (Position_FlatDistance(&other->x.fixed, position) > 8)
        continue;
      if (FIELD_CELL(other->y.fixed) != FIELD_CELL(position[1]))
        continue;
      if (((struct Sprite *)other->sprite)->entries[0]->animationId ==
          SPRITE_STONE_PILLAR)
        goto end;
      goto hit;
    }
  }
end:
  flame->acceleration = toFIXED(2);
  flame->speedLimit = toFIXED(2);
  flame->update = (ObjectUpdate)MagmaRockInside_TrailEmbers;
  FieldActor_SetMoveTargetFX(flame, position[0], position[1], position[2]);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveToFX(position[0], position[1], position[2], 1);
  FieldActor_WaitForMove(flame);
  flame->update = NULL;
  Actor_Release(flame);
  for (id = 0; id != FIRE_BURST_FRAMES; id++) {
    bit = gFrameCount;
    bit &= 1;
    if (bit == 0) {
      options.type = SPRITE_FLAME_PUFF;
      options.priority = 1;
      options.update =
          (void (*)(union FieldObject *))MagmaRockInside_DriftAndSlow;
      options.script = gMagmaRockInside_EmberScript;
      FieldEffect_Spawn(position[0], position[1], position[2],
                        ((Random16() & 31) - 16) << 13,
                        ((Random16() & 31) - 16) << 13, bit,
                        EFFECT_USE_UPDATE | EFFECT_USE_SCRIPT |
                            EFFECT_USE_TYPE | EFFECT_USE_PRIORITY,
                        &options);
    }
    System_Wait(1);
  }
  System_Wait(24);
  if (cell == CELL_BEHAVIOR_MAGMA_ROCK_LAVA_FALL) {
    MagmaRockInside_RaiseLava();
    Event_End();
    return;
  }
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  Event_End();
  return;
hit:
  flame->acceleration = toFIXED(2);
  flame->speedLimit = toFIXED(2);
  flame->update = (ObjectUpdate)MagmaRockInside_TrailEmbers;
  FieldActor_SetMoveTargetFX(flame, position[0], position[1], position[2]);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveToFX(position[0], position[1], position[2], 1);
  FieldActor_WaitForMove(flame);
  flame->update = NULL;
  Actor_Release(flame);
  Event_SetFlag(FLAG_FIRE_BURST_PLAYING);
  MagmaRockInside_PlayBurst(id);
  Event_ClearFlag(FLAG_FIRE_BURST_PLAYING);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  Event_End();
}

void MagmaRockInside_EndFallingRocks(void);

/// Once Felix's Burst on the small statue beside a great stone face is over,
/// the statue flashes white and spits a fireball the way it faces. In the
/// three halls with a face the fireball strikes it, and the face's eyes light
/// red as it pours lava over the floor (MagmaRockInside_FireFromStatue).
void MagmaRockInside_FireStatue(void) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  struct Actor *actor;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(10);
  actor = work->child;
  actor->update = NULL;
  FieldActor_SetSpriteColor(actor, SPRITE_COLOR_WHITE);
  FieldEvent_Wait(3);
  FieldActor_SetSpriteColor(work->child, SPRITE_COLOR_NORMAL);
  FieldEvent_Wait(1);
  MagmaRockInside_FireFromStatue(work->child);
  Event_End();
}

/// A cracked pillar's animation as Burst breaks it: its two cracked pictures,
/// then its stump, 41 frames each. It starts at frame
/// CRACKED_PILLAR_CRUMBLE_FRAME of the cast, among the small explosions, and
/// the pillar has crumbled to its stump by the last blast, which takes it away.
enum {
  CRACKED_PILLAR_ANIMATION_CRUMBLE = 3,
  CRACKED_PILLAR_CRUMBLE_FRAME = 178
};

/// Burst on one of the tall cracked pillars: rocks fall round it while Burst
/// lasts, one at each small explosion and eight at the last blast, which also
/// takes the pillar away, leaving a cloud of dust; meanwhile the pillar
/// crumbles.
void MagmaRockInside_BurstCrackedPillar(s32 stage, s32 cue) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;

  if (cue == 0)
    MagmaRockInside_StartFallingRocks();
  if (stage == BURST_STAGE_SMALL_BLAST) {
    struct Actor *actor = work->child;

    MagmaRockInside_DropRocks(actor->x.fixed, actor->y.fixed + toFIXED(15),
                              actor->z.fixed, 1);
  }
  if (stage == BURST_STAGE_GREAT_BLAST) {
    struct Actor *actor = work->child;
    struct Actor *target;

    MagmaRockInside_DropRocks(actor->x.fixed, actor->y.fixed + toFIXED(15),
                              actor->z.fixed, 8);
    target = work->child;
    SET_ACTOR_POSITION(target, 0, 0, 0);
  }
  if (cue == PSYNERGY_EVENT_LAST_FRAME)
    MagmaRockInside_EndFallingRocks();
  if (cue == CRACKED_PILLAR_CRUMBLE_FRAME)
    Actor_SelectAnimation(work->child, CRACKED_PILLAR_ANIMATION_CRUMBLE);
}

/// Felix walks to the lava's edge where something stands in the lava two cells
/// ahead, a white platform, a column or a pillar pushed in, and springs across
/// onto it, keeping pace with a platform as it moves. Such edges are on both
/// sides of the Lucky Medal room's lava, at cells (44, 39) and (50, 39), and
/// along the lava in Fury's room, the cracked pillars' room and, once its lava
/// is up, the Salamander Tail's room. Where nothing stands there, nothing
/// happens.
void MagmaRockInside_LeapOverLava(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *target;
  FIXED position[3];
  FIXED stepX;
  FIXED stepZ;
  s32 i;
  s32 angle = (leader->facing + DEGtoANG(45)) & FACING_QUARTER_MASK;

  position[0] = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  position[1] = leader->y.fixed - toFIXED(16);
  position[2] = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  FieldVector_AddPolarOffset(32, angle, position);
  target = Actor_FindInCell(position, leader);
  if (target == 0)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
  System_Wait(6);
  Event_PlaySound(SE_JUMP);
  Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
  leader->speedLimit = toFIXED(3);
  leader->acceleration = toFIXED(2);
  leader->velocityY = toFIXED(24) / 5;
  leader->motionFlags &= ~(ACTOR_FOLLOWS_TERRAIN | ACTOR_MOTION_SPARE);
  Actor_SetSpriteFlags(leader, 0);
  stepX = (position[0] - leader->x.fixed) / 12;
  stepZ = (position[2] - leader->z.fixed) / 12;
  for (i = 0; i != 12; i++) {
    leader->x.fixed += target->x.fixed - position[0] + stepX;
    leader->z.fixed += target->z.fixed - position[2] + stepZ;
    position[0] = target->x.fixed;
    position[2] = target->z.fixed;
    System_Wait(1);
  }
  leader->x.fixed = target->x.fixed;
  leader->y.fixed = leader->groundHeight = target->y.fixed + toFIXED(16);
  leader->z.fixed = target->z.fixed;
  Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
  Actor_SetSpriteFlags(leader, 1);
  leader->motionFlags |= ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_End();
}

extern s32 gMagmaRockInside_FlameEmberScript[];

/// The bursts of flame, 8 frames apart, as a statue's fireball strikes a great
/// stone face.
enum { LAVA_FALL_FLAME_BURSTS = 16 };

/// A statue's fireball strikes the great stone face: the screen shakes as
/// flames burst over the face, it roars and its eyes light red, lava pours
/// from its mouth and spreads over the hall's floor until the hall is flooded.
/// When the lava is already up, the face only flares and pours again.
void MagmaRockInside_RaiseLava(void) {
  s32 srcX = 0;
  s32 srcZ = 0;
  FIXED baseX = 0;
  struct MapLayer *layer =
      &((struct MapWork *)Ram_HeapSlots->mapGrid)->layers[MAPGRID_LAYER_MIDDLE];
  struct EffectOptions options;
  s32 position[3];
  // LAVA_ALREADY_UP when the room is already flooded; then the frames the
  // lava takes to rise.
  u32 frame = 0;
  u32 i;
  u32 row;

  options.type = SPRITE_FLAME_PUFF;
  options.priority = 1;
  options.script = gMagmaRockInside_FlameEmberScript;
  options.startScaleX = FIXED_ONE * 9 / 5;
  options.startScaleY = FIXED_ONE * 9 / 5;
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE) {
    baseX = toFIXED(328);
    position[1] = toFIXED(32);
    position[2] = toFIXED(200);
    srcX = 20;
    srcZ = 69;
    if (Event_GetFlag(FLAG_ENTRANCE_FLOODED))
      frame = LAVA_ALREADY_UP;
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY) {
    baseX = toFIXED(232);
    position[1] = toFIXED(32);
    position[2] = toFIXED(248);
    srcX = 14;
    srcZ = 72;
    if (Event_GetFlag(FLAG_FURY_FLOODED))
      frame = LAVA_ALREADY_UP;
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS) {
    baseX = toFIXED(392);
    position[1] = toFIXED(32);
    position[2] = toFIXED(536);
    srcX = 24;
    srcZ = 90;
    if (Event_GetFlag(FLAG_LOWER_ROOMS_FLOODED))
      frame = LAVA_ALREADY_UP;
  }
  for (i = 0; i != LAVA_FALL_FLAME_BURSTS; i++) {
    position[0] = baseX + ((Random16() & 31) << 16);
    Event_PlaySound(SE_CRASH);
    FieldEffect_Spawn(position[0], position[1], position[2], 0,
                      (Random16() & 31) << 12, 0,
                      EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | 1,
                      &options);
    MapGrid_SetShake(5, 5, 1);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    Event_Wait(8);
  }
  Event_PlaySound(SE_BOSS_ROAR);
  options.priority = 1;
  options.startScaleX = FIXED_ONE * 2;
  options.startScaleY = FIXED_ONE * 2;
  options.targetScaleX = FIXED_ONE / 2;
  options.targetScaleY = FIXED_ONE / 2;
  baseX -= toFIXED(8);
  for (i = 0; i != 8; i++) {
    position[0] = baseX + i * toFIXED(6);
    FieldEffect_Spawn(position[0], position[1], position[2], 0, 0,
                      (Random16() & 31) << 10,
                      EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET |
                          EFFECT_USE_PRIORITY | 1,
                      &options);
  }
  MapGrid_CopyCellsTo(68, 70, srcX, srcZ, 3, 6);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_Wait(30);
  MapGrid_CopyCellsTo(71, 70, srcX, srcZ, 3, 6);
  options.type = SPRITE_FLAME_PUFF;
  options.priority = 1;
  options.palette = SPRITE_COLOR_RED_TINT;
  options.script = gMagmaRockInside_EmberScript;
  if (frame != 0) {
    MapGrid_CopyCellsTo(68, 70, srcX, srcZ, 3, 6);
    Event_Wait(30);
    position[2] -= toFIXED(40);
  } else {
    MapGrid_SetShake(2, 1, 1);
    Event_PlaySound(SE_TREMOR);
    for (row = 0; row != 12; row++) {
      for (i = 0; i != 6; i++) {
        position[0] = baseX + ((Random16() % 48) << 16);
        FieldEffect_Spawn(position[0], position[1],
                          position[2] + row * toFIXED(4) + toFIXED(8), 0,
                          toFIXED(1), 0,
                          EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                              EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                          &options);
      }
      wait(3);
      MapGrid_CopyCellsTo(74, 70, srcX, srcZ, 3, (row >> 2) + 6);
    }
    MapGrid_CopyCellsTo(74, 70, srcX, srcZ, 3, 8);
    frame = 0;
    do {
      if ((frame & 3) == 0) {
        for (i = 0; i != 4; i++) {
          position[0] = baseX + ((Random16() % 48) << 16);
          FieldEffect_Spawn(position[0], position[1], position[2] + toFIXED(48),
                            0, toFIXED(1), 0,
                            EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                                EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                            &options);
        }
      }
      if (frame == 0) {
        layer->offsetY -= toFIXED(32);
        MapGrid_CenterOnFollowedPoint();
        Event_Wait(1);
      } else if (frame <= 63) {
        REG_BLDALPHA = ((16 - (frame >> 2)) << 8) | (frame >> 2);
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
      } else if (frame == 64) {
        REG_BLDCNT = 0;
      } else if (frame <= 127) {
        layer->offsetY += toFIXED(1) / 2;
        position[2] -= toFIXED(1) / 2;
        MapGrid_CopyCellsTo(71, 76, srcX, srcZ - ((frame - 47) >> 5) + 8, 3, 1);
      } else if (frame == 128) {
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      }
      frame++;
      wait(1);
    } while (frame != 160);
    FieldMapGrid_WaitForShakeToSettle();
  }
  Event_PlaySound(SE_STOP_ACTION_SE);
  Event_PlaySound(SE_WHOOSH);
  options.startScaleY = FIXED_ONE * 9 / 5;
  options.startScaleX = FIXED_ONE * 9 / 5;
  for (i = 0; i != 6; i++) {
    position[0] = baseX + ((Random16() % 48) << 16);
    FieldEffect_Spawn(
        position[0], position[1], position[2] + toFIXED(48), 0, 0, 0,
        EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
        &options);
  }
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_CopyCellsTo(68, 64, srcX, srcZ, 3, 6);
  FieldMapGrid_WaitForShakeToSettle();
  Event_Wait(30);
  layer->offsetY = 0;
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
    MagmaRockInside_SetEntranceFlooded();
  else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY)
    MagmaRockInside_SetFuryFlooded();
  else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
    MagmaRockInside_SetLowerRoomsFlooded();
}

/// Felix steps onto the small floor switch beside a grated gate and presses it
/// down: the ground shakes and the gate lifts. If the lava is up it pours out
/// through the gate, embers rising as it fades from the floor; then the gate
/// drops shut and he steps back off the switch. The switches are at cell
/// (19, 27) in the entrance hall, (32, 21) in Fury's room, and (47, 13) and
/// (49, 42) in the Golem Core's room, which drain the lower rooms.
void MagmaRockInside_DrainLava(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  u32 facing = (leader->facing + DEGtoANG(45)) & FACING_QUARTER_MASK;
  s32 position[3];
  struct EffectOptions options;
  FIXED baseX = 0;
  s32 dstX = 0;
  s32 dstZ = 0;
  s32 open = 0;
  s32 srcX = 0;
  u8 *flags;
  // The gate's row on the map, then each row of embers over the drained fall.
  u32 row = 0;
  u32 j;
  s32 rowAbove;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  position[0] = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  position[2] = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, FIXEDtoINT(position[0]),
                      FIXEDtoINT(position[2]));
  FieldEvent_Wait(20);
  Event_PlaySound(SE_BUTTON_PRESS);
  flags = &leader->priorityFlags;
  *flags &= ~ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  FieldEvent_Wait(20);
  MapGrid_SetShake(1, 2, 1);
  Event_PlaySound(SE_TREMOR);
  FieldEvent_Wait(30);
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE) {
    srcX = 20;
    row = 89;
    dstX = 84;
    dstZ = 26;
    baseX = toFIXED(320);
    position[1] = 0;
    position[2] = toFIXED(424);
    if (Event_GetFlag(FLAG_ENTRANCE_FLOODED))
      open = LAVA_ALREADY_UP;
    Event_ClearFlag(FLAG_ENTRANCE_FLOODED);
    facing = FACING_EAST;
    MapCell_SetAttributes(0, 15, 13, MAP_CELL_CLIMBABLE);
    FieldActor_SetSpritePriority(ACTOR_ENTRANCE_PILLAR, 3);
    for (j = 0; j != 7; j++)
      Actor_SetAnimation(j + ACTOR_COLUMN_A, 1);
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY) {
    srcX = 28;
    row = 83;
    dstX = 92;
    dstZ = 21;
    baseX = toFIXED(448);
    position[1] = 0;
    position[2] = toFIXED(344);
    if (Event_GetFlag(FLAG_FURY_FLOODED))
      open = LAVA_ALREADY_UP;
    Event_ClearFlag(FLAG_FURY_FLOODED);
    MapCell_SetAttributes(0, 24, 15, MAP_CELL_CLIMBABLE);
    MapCell_SetAttributes(0, 9, 16, MAP_CELL_CLIMBABLE);
    Actor_SetAnimation(ACTOR_FURY_PILLAR, 4);
  } else if (gMainRoutineWork.next ==
             SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST) {
    if (gMainRoutineWork.entrance <= 8) {
      srcX = 49;
      row = 75;
      dstX = 113;
      dstZ = 13;
      baseX = toFIXED(792);
      position[1] = 0;
      position[2] = toFIXED(216);
      if (Event_GetFlag(FLAG_LOWER_ROOMS_FLOODED))
        open = LAVA_ALREADY_UP;
      Event_ClearFlag(FLAG_LOWER_ROOMS_FLOODED);
    } else {
      srcX = 51;
      row = 104;
      dstX = 115;
      dstZ = 42;
      baseX = toFIXED(824);
      position[1] = 0;
      position[2] = toFIXED(680);
      if (Event_GetFlag(FLAG_LOWER_ROOMS_FLOODED))
        open = LAVA_ALREADY_UP;
      Event_ClearFlag(FLAG_LOWER_ROOMS_FLOODED);
    }
  }
  Event_PlaySound(SE_BOOM);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  rowAbove = row - 1;
  MapGrid_CopyCellsTo(srcX, row, srcX, rowAbove, 3, 4);
  if (open == 0) {
    FieldMapGrid_WaitForShakeToSettle();
    FieldEvent_Wait(20);
  } else {
    MapGrid_SetShake(2, 1, 1);
    options.type = SPRITE_FLAME_PUFF;
    options.priority = 2;
    options.palette = SPRITE_COLOR_RED_TINT;
    options.script = gMagmaRockInside_EmberScript;
    for (row = 0; row != 10; row++) {
      for (j = 0; j != 6; j++) {
        u32 random = Random16();

        position[0] = baseX + ((random % 48) << 16);
        FieldEffect_Spawn(position[0], position[1],
                          position[2] + row * toFIXED(4) + toFIXED(23), 0,
                          toFIXED(1), 0,
                          EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                              EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                          &options);
      }
      System_Wait(3);
      MapGrid_CopyCellsTo(75, 64, dstX, dstZ, 3, (row >> 1) + 1);
    }
    for (j = 0; j != BLEND_WEIGHT_MAX * 4; j++) {
      REG_BLDALPHA = ((j >> 2) << 8) | (BLEND_WEIGHT_MAX - (j >> 2));
      REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
      System_Wait(3);
    }
    MapGrid_CopyCellsTo(78, 64, dstX, dstZ, 3, 5);
  }
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
    Actor_Get(ACTOR_ENTRANCE_PILLAR)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Event_PlaySound(SE_STOP_ACTION_SE);
  Event_PlaySound(SE_BOOM);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_CopyCellsTo(72, 63, srcX, rowAbove, 3, 4);
  FieldMapGrid_WaitForShakeToSettle();
  Event_PlaySound(SE_BUTTON_PRESS);
  ActorFlags_Set(flags, ACTOR_PRIORITY_WALKS_ON_PLATFORMS);
  FieldEvent_Wait(20);
  Actor_Get(gMainRoutineWork.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  position[0] = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  position[1] = leader->y.fixed;
  position[2] = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  FieldVector_AddPolarOffset(16, (s32)facing ^ DEGtoANG(180), position);
  leader->facing = facing;
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, FIXEDtoINT(position[0]),
                      FIXEDtoINT(position[2]));
  Actor_Get(gMainRoutineWork.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  Event_End();
}

#define DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR(name, n1, n2)                    \
  void name(s32 stage, s32 cue) {                                              \
    struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;             \
                                                                               \
    work->child = Actor_Get(n1);                                               \
    MagmaRockInside_BurstCrackedPillar(stage, cue);                            \
    Event_SetFlag(n2);                                                         \
  }

/// Burst cast at the tall cracked pillar east of the cracked pillars' room's
/// north statue (cell (17, 12)) breaks it: rocks fall round it as it crumbles
/// and the last blast leaves only dust. The statue's own fireball breaks it
/// the same way. `FLAG_INSIDE_CRACKED_PILLAR_A_BURST` keeps it gone.
DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR(MagmaRockInside_BurstCrackedPillarA,
                                      ACTOR_CRACKED_PILLAR_A,
                                      FLAG_INSIDE_CRACKED_PILLAR_A_BURST)

/// Burst cast at the cracked pillars' room's other tall cracked pillar (cells
/// (19, 14) and (24, 16)) breaks it: rocks fall round it as it crumbles and the
/// last blast leaves only dust. `FLAG_INSIDE_CRACKED_PILLAR_B_BURST` keeps it
/// gone.
DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR(MagmaRockInside_BurstCrackedPillarB,
                                      ACTOR_CRACKED_PILLAR_B,
                                      FLAG_INSIDE_CRACKED_PILLAR_B_BURST)

/// The update of a rock or flame the ember stone throws: it trails smoking
/// embers every other frame for 64 frames.
void MagmaRockInside_SmokeEmber(struct Actor *actor) {
  if (++actor->timer > 64)
    actor->update = NULL;
  if ((actor->timer & 1) == 0) {
    struct EffectOptions config;

    config.type = SPRITE_FLAME_PUFF;
    config.priority = 2;
    config.palette = SPRITE_COLOR_RED_TINT;
    config.script = gMagmaRockInside_EmberScript;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0,
                      toFIXED(1) / 16, 0,
                      EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                          EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                      &config);
  }
}

extern s32 gMagmaRockInside_FlameEmberScript[];

/// Throws a rock or flame up out of the lava where the ember stone lies, to
/// land at (x, z) in pixels, smoking as it flies (MagmaRockInside_SmokeEmber).
static __inline__ void Arrive(s32 id, s32 x, s32 z) {
  Actor_SetPosition(id, 776, 248);
  Actor_SetDestination(id, x, z);
  Actor_GetById(id)->velocityY = toFIXED(12);
  Actor_GetById(id)->gravity = toFIXED(1) / 2;
  Actor_GetById(id)->timer = 0;
  Actor_GetById(id)->update = MagmaRockInside_SmokeEmber;
}

/// The first time Felix hops onto the islet in the lava at cell (45, 19), in
/// the ember stone's room east of the torches' ledge, the room shakes and the
/// camera turns to the lava: embers spout and pound, four rocks and four
/// flames are thrown out to land on the islets round the spot, and last
/// the ember stone itself, a glowing red rock, lands on an islet to the west.
/// `FLAG_ROCKS_RELEASED` keeps them there.
void MagmaRockInside_ReleaseRocks(void) {
  FIXED position[3];
  struct EffectOptions options;
  s32 beat;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(SE_TREMOR);
  MapGrid_SetShake(3, 3, 1);
  Event_Wait(30);
  FieldCamera_MoveTo(776, CAMERA_KEEP, 272, 1);
  FieldCamera_WaitForArrival();
  options.type = SPRITE_FLAME_PUFF;
  options.priority = 2;
  options.palette = SPRITE_COLOR_RED_TINT;
  options.script = gMagmaRockInside_FlameEmberScript;
  options.startScaleY = FIXED_ONE * 3 / 2;
  options.startScaleX = FIXED_ONE * 3 / 2;
  position[1] = 0;
  // A beat every 3 frames: embers burst from the stone, a pound every eighth
  // beat; the rocks and flames are thrown eight beats apart from beat 32, and
  // the stone itself at beat 112.
  for (beat = 0; beat != 128; beat++) {
    position[0] = toFIXED(Random16() & 31) + (toFIXED(760));
    position[2] = toFIXED(Random16() & 31) + (toFIXED(256));
    FieldEffect_Spawn(position[0], position[1], position[2], 0, toFIXED(1), 0,
                      EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
                          EFFECT_USE_PALETTE,
                      &options);
    if ((beat & 7) == 0)
      Event_PlaySound(SE_POUND);
    if (beat == 32) {
      Arrive(ACTOR_FALLEN_ROCK_A, 744, 216);
    } else if (beat == 40) {
      Arrive(ACTOR_FALLEN_FLAME_D, 776, 328);
    } else if (beat == 48) {
      Arrive(ACTOR_FALLEN_ROCK_B, 856, 232);
    } else if (beat == 56) {
      Arrive(ACTOR_FALLEN_FLAME_C, 696, 312);
    } else if (beat == 64) {
      Arrive(ACTOR_FALLEN_ROCK_C, 728, 280);
    } else if (beat == 72) {
      Arrive(ACTOR_FALLEN_FLAME_B, 824, 280);
    } else if (beat == 80) {
      Arrive(ACTOR_FALLEN_ROCK_D, 824, 312);
    } else if (beat == 88) {
      Arrive(ACTOR_FALLEN_FLAME_A, 776, 232);
    } else if (beat == 112) {
      Actor_SetPosition(ACTOR_EMBER_STONE, 776, 248);
      Actor_SetDestination(ACTOR_EMBER_STONE, 696, 232);
      FieldCamera_MoveTo(696, CAMERA_KEEP, 232, 1);
      Actor_GetById(ACTOR_EMBER_STONE)->velocityY = toFIXED(13);
      Actor_GetById(ACTOR_EMBER_STONE)->gravity = toFIXED(1) / 2;
      Actor_GetById(ACTOR_EMBER_STONE)->timer = 0;
      Actor_GetById(ACTOR_EMBER_STONE)->update = MagmaRockInside_SmokeEmber;
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    }
    wait(3);
  }
  FieldActor_WaitForMoveById(ACTOR_EMBER_STONE);
  FieldMapGrid_WaitForShakeToSettle();
  Event_PlaySound(SE_STOP_ACTION_SE);
  Event_Wait(40);
  Event_SetFlag(FLAG_ROCKS_RELEASED);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  wait(10);
  Event_End();
}

/// Felix casts Blaze at one of the flames the ember stone threw into the lava:
/// Blaze sprays far, and unless the east torch on the ledge west of the lava is
/// already lit, a small hidden flame waits on it, which grows if the spray
/// reaches it (MagmaRockInside_GrowFlame).
void MagmaRockInside_TakeFlame(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->blazeLongSpray = 1;
  if (!Event_GetFlag(FLAG_EAST_TORCH_LIT)) {
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_EAST_TORCH_FLAME),
                              SPRITE_COLOR_HIDDEN);
    Actor_SetPosition(ACTOR_EAST_TORCH_FLAME, 632, 312);
    Actor_Get(ACTOR_EAST_TORCH_FLAME)->scaleX =
        Actor_Get(ACTOR_EAST_TORCH_FLAME)->scaleY = FIXED_ONE / 16;
  }
}

#define DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH(name, n1, n2, n3, n4)             \
  void name(void) {                                                            \
    if (!Event_GetFlag(n1)) {                                                  \
      FieldActor_SetSpriteColor(Actor_Get(n2), SPRITE_COLOR_HIDDEN);           \
      Actor_SetPosition(n2, n3, n4);                                           \
      Actor_Get(n2)->scaleX = Actor_Get(n2)->scaleY = FIXED_ONE / 16;          \
    }                                                                          \
  }

/// Felix casts Blaze at the east torch's flame on the ledge west of the ember
/// stone's lava: unless the west torch beside it is lit, a small hidden flame
/// waits on it, which grows if the spray reaches it
/// (MagmaRockInside_GrowFlame).
DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH(MagmaRockInside_LightTorch,
                                     FLAG_WEST_TORCH_LIT,
                                     ACTOR_WEST_TORCH_FLAME, 600, 312)

struct Actor;

/// Blaze's spray reaches a torch on the ledge west of the ember stone's lava,
/// and its flame grows until it burns full. Once both torches burn, the ground
/// rumbles, the camera turns to the stone pillar between them, and it sinks
/// into the floor, opening the doorway behind it (cell (38, 18)).
void MagmaRockInside_GrowFlame(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *object;
  struct Actor *actor;
  struct Actor *other;
  s32 i;

  object = Actor_GetById(work->psynergyHitObject);
  FieldActor_SetSpriteColor(object, SPRITE_COLOR_NORMAL);
  Event_PlaySound(SE_WHOOSH);
  i = 0;
  do {
    object->scaleY = object->scaleX += FIXED_ONE / FLAME_GROW_STEPS;
    wait(5);
    i++;
  } while (i != FLAME_GROW_STEPS);
  Event_SetFlag(FLAG_TORCH_LIT(work->psynergyHitObject));
  if (Event_GetFlag(FLAG_WEST_TORCH_LIT) &&
      Event_GetFlag(FLAG_EAST_TORCH_LIT)) {
    other = Actor_GetById(ACTOR_BLOCKING_PILLAR);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    MapGrid_SetShake(2, 1, 1);
    Event_PlaySound(SE_RUMBLE);
    FieldEvent_Wait(30);
    FieldCamera_MoveTo(616, CAMERA_KEEP, 296, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(30);
    Event_PlaySound(SE_RISE_WITH_SPARKS_LOW);
    REG_BLDALPHA = BLEND_ALPHA(0, 16);
    REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
    {
      struct Actor *prepared = OverlayObject_PrepareObject(
          other->x.fixed, other->groundHeight, other->z.fixed + toFIXED(48),
          SPRITE_YELLOW_BLOCK);

      other->motionFlags = 0;
      actor = prepared;
    }
    i = 0;
    do {
      other->y.fixed -= toFIXED(1);
      FieldEvent_Wait(5);
      i++;
    } while (i != PILLAR_SINK_DEPTH);
    Event_PlaySound(SE_FLAME_GROW);
    Actor_SelectAnimation(other, 4);
    Actor_Release(actor);
    FieldEvent_Wait(1);
    REG_BLDCNT = 0;
    MapCell_SetAttributes(0, 38, 18, 0);
    Event_End();
  }
}

/// Nothing visible happens when Blaze, cast at a flame in the ember stone's
/// room, reaches no torch; the game takes away the hidden flames it had set
/// waiting on the unlit torches.
void MagmaRockInside_PutOutFlames(void) {
  if (gFieldWork->psynergyHitObject == -1) {
    if (!Event_GetFlag(FLAG_EAST_TORCH_LIT))
      Actor_SetPosition(ACTOR_EAST_TORCH_FLAME, 0, 0);
    if (!Event_GetFlag(FLAG_WEST_TORCH_LIT))
      Actor_SetPosition(ACTOR_WEST_TORCH_FLAME, 0, 0);
  }
}

/// Examining the ember stone, the red rock the eruption threw onto an islet in
/// the lava (cell (43, 14)): "There's something burning like an ember within
/// the stone." Answering yes, Felix takes the Magma Ball from it, if someone
/// in the party can carry it.
void MagmaRockInside_ExamineEmberStone(void) {
  const u8 *message;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  message = MsgBlazeStoneEmber;
  Message_ShowCentered(message, 1);
  FieldEvent_SetMessageId(message + 1);
  Event_OpenMessage(ACTOR_EMBER_STONE, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId,
                                            0) == 0) {
    Party_GiveItem(ITEM_MAGMA_BALL, 0);
    if (FieldPartyInventory_FindMember(ITEM_MAGMA_BALL) == -1) {
      Event_End();
      return;
    }
    ItemDisplay_ShowItem(ITEM_MAGMA_BALL,
                         ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
    Actor_SetAnimation(ACTOR_EMBER_STONE, 2);
    Event_SetFlag(FLAG_MAGMA_BALL_TAKEN);
  }
  Event_End();
}

/// Walking north into the stone wall under its golden crest, at the end of the
/// sunken pillars' path in the ember stone room's west (cell (15, 21)), opens a
/// stairway in it step by step, and Felix walks in and on to the Blaze
/// tablet's room.
void MagmaRockInside_OpenStairs(void) {
  s32 width = 1;
  s32 height = 2;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_THUMP);
  MapGrid_CopyCellsTo(72, 67, 15, 19, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(73, 67, 15, 19, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(74, 67, 15, 19, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(75, 67, 15, 19, width, height);
  FieldEvent_Wait(15);
  Actor_Get(gMainRoutineWork.leaderActorId)->motionFlags = 0;
  Actor_Get(gMainRoutineWork.leaderActorId)->speedLimit =
      Actor_Get(gMainRoutineWork.leaderActorId)->acceleration = toFIXED(1) / 2;
  Event_PlaySound(SE_MAP_EXIT);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, 248, 328);
  FieldEvent_Wait(5);
  FieldEvent_SetExitRequest(1);
  Event_End();
}

/// A frame task: once Felix stops on a new cell, puts back the picture of the
/// stepping stone he left and, in the Blaze tablet's room, raises the trigger
/// of that stone if the map's upper layer numbers it (STEP_CELL_TRIGGER_BASE
/// plus its number, MagmaRockInside_StepOnMarker).
void MagmaRockInside_TrackStepCell(void) {
  struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);
  s32 key = CELL_KEY(x, z);
  s32 last;
  s32 value;
  s32 trigger;
  struct FieldWork *work;

  if (actor->velocityX != 0 || actor->velocityZ != 0)
    return;
  last = gMagmaRockInside_LastStepCell;
  if (last == -1 || key == last)
    return;
  x = CELL_KEY_X(last);
  z = CELL_KEY_Z(last);
  gMagmaRockInside_LastStepCell = -1;
  MapGrid_CopyCellsLayered(
      73, 72, gMagmaRockInside_StepCellWidth, gMagmaRockInside_StepCellHeight,
      gMagmaRockInside_StepCellX, gMagmaRockInside_StepCellZ);
  if (gMainRoutineWork.next != SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET)
    return;
  work = Ram_HeapSlots->fieldWork;
  value = MapCell_GetAttributes(MAPGRID_LAYER_OVERHEAD, x, z) >> 8;
  if (value != 0) {
    trigger = value + STEP_CELL_TRIGGER_BASE;
    work->raisedTrigger = trigger;
  }
}

/// Starts watching the stepping stones Felix walks on in the Blaze tablet's
/// room (MagmaRockInside_TrackStepCell).
void MagmaRockInside_StartStepCells(void) {
  gMagmaRockInside_LastStepCell = -1;
  FieldTask_AddFrame(MagmaRockInside_TrackStepCell, TASK_PASS_FRAME);
}

/// Nothing seems to happen as Felix steps onto one of the stepping stones that
/// cross the Blaze tablet room's lava, but he is seen standing on top of it:
/// the game saves the stone's picture under him (to cell (73, 72)) and draws
/// the picture kept at (70, 72) in its place, until he leaves it
/// (MagmaRockInside_TrackStepCell). The stones of this picture alternate with
/// the other kind along each row (MagmaRockInside_StampOtherStepCell), as Air's
/// Rock's do (AirsRockInside_StampWideCellUnderLeader).
void MagmaRockInside_StampStepCell(void) {
  struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 y = FIELD_CELL(actor->y.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);
  s32 key = CELL_KEY(x, z);

  if (key != gMagmaRockInside_LastStepCell) {
    gMagmaRockInside_LastStepCell = key;
    gMagmaRockInside_StepCellWidth = 1;
    gMagmaRockInside_StepCellHeight = 2;
    gMagmaRockInside_StepCellX = x + 64;
    gMagmaRockInside_StepCellZ = z - y;
    MapGrid_CopyCellsLayered(gMagmaRockInside_StepCellX,
                             gMagmaRockInside_StepCellZ, 1, 2, 73, 72);
    MapGrid_CopyCellsLayered(
        70, 72, gMagmaRockInside_StepCellWidth, gMagmaRockInside_StepCellHeight,
        gMagmaRockInside_StepCellX, gMagmaRockInside_StepCellZ);
  }
}

/// Nothing seems to happen as Felix steps onto a stepping stone of the other
/// kind in the Blaze tablet room's lava, but he is seen standing on top of it:
/// as MagmaRockInside_StampStepCell, drawing the picture kept at (71, 72).
void MagmaRockInside_StampOtherStepCell(void) {
  struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 y = FIELD_CELL(actor->y.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);
  s32 key = CELL_KEY(x, z);

  if (key != gMagmaRockInside_LastStepCell) {
    gMagmaRockInside_LastStepCell = key;
    gMagmaRockInside_StepCellWidth = 1;
    gMagmaRockInside_StepCellHeight = 2;
    gMagmaRockInside_StepCellX = x + 64;
    gMagmaRockInside_StepCellZ = z - y;
    MapGrid_CopyCellsLayered(gMagmaRockInside_StepCellX,
                             gMagmaRockInside_StepCellZ, 1, 2, 73, 72);
    MapGrid_CopyCellsLayered(
        71, 72, gMagmaRockInside_StepCellWidth, gMagmaRockInside_StepCellHeight,
        gMagmaRockInside_StepCellX, gMagmaRockInside_StepCellZ);
  }
}

/// The stepping stone at (x, z), in pixels, in the line of stepping stones that
/// leads to the Blaze tablet, glows white with a drip and fades away into the
/// lava (an object window over it, blended out), and its cell is walled off.
void MagmaRockInside_FadeStoneAway(s32 x, s32 z) {
  struct Actor *actor;
  s32 i;
  s32 cellZ;

  actor = FieldActor_Create(SPRITE_STACKED_CRATES, toFIXED(x), 0, toFIXED(z));
  REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  Scene_SetActorSpritePriority(actor, 0);
  Actor_SetSpriteFlags(actor, 0);
  ((struct FieldSprite *)actor->sprite)->blendMode = 2;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |
               WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WINOBJ_ALL;
  REG_DISPCNT |= DISPCNT_OBJWIN_ON;
  Event_PlaySound(SE_DRIP);
  for (i = 0; i != BLEND_WEIGHT_MAX / 2; i++) {
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, i * 2);
    System_Wait(1);
  }
  for (i = 0; i != BLEND_WEIGHT_MAX; i++) {
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - i, BLEND_WEIGHT_MAX);
    System_Wait(1);
  }
  x >>= 4;
  cellZ = (z >> 4) - 1;
  MapGrid_CopyCellsTo(65, 1, x + 64, cellZ, 1, 2);
  MapCell_SetAttributes(0, x, cellZ, CELL_BEHAVIOR_WALL << 8);
  Actor_Release(actor);
  Event_End();
  REG_BLDCNT = 0;
  REG_DISPCNT &= ~DISPCNT_OBJWIN_ON;
}

/// Felix walks north from the Blaze tablet room's entrance along the line of
/// stepping stones to the tablet's platform (column 16 of the map): on every
/// second stone, the stone he has just stepped off glows white with a drip and
/// fades away into the lava, so there is no way back. Five go in all, the
/// first the stone just north of the entrance's walkway, the last the stone
/// just below the tablet's platform. Retreat is barred in this room too, so
/// the party leaves by the way Blaze opens (MagmaRockInside_GrowTabletFlame).
/// The flags keep the stones gone.
void MagmaRockInside_StepOnMarker(void) {
  struct FieldWork *work = gFieldWork;
  struct Actor *leader;
  s16 *trigger;
  s32 cell, row = 54;

  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  cell =
      MapCell_GetAttributes(MAPGRID_LAYER_OVERHEAD, FIELD_CELL(leader->x.fixed),
                            FIELD_CELL(leader->z.fixed));
  trigger = &work->raisedTrigger;
  // The marker's row: 54 for the first, four rows up for each after it.
  row -= (*trigger - (STEP_CELL_TRIGGER_BASE + 1)) << 2;
  cell >>= 8;
  if (cell != 1 && cell != 7) {
    MagmaRockInside_StampStepCell();
    MagmaRockInside_FadeStoneAway(264, row * 8 + 10);
    if (!Event_GetFlag(FLAG_ANY_MARKER_LIT))
      Event_SetFlag(FLAG_ANY_MARKER_LIT);
    Event_SetFlag(FLAG_MARKER_LIT(*trigger - STEP_CELL_TRIGGER_BASE));
  } else if (Event_GetFlag(FLAG_ANY_MARKER_LIT)) {
    MagmaRockInside_FadeStoneAway(264, row * 8 + 10);
  }
}

/// Felix casts Blaze at the flame in the west bowl on the ledge in the Blaze
/// tablet room's south-west: unless the east bowl beside it is lit, a small
/// hidden flame waits on it, which grows if the spray reaches it
/// (MagmaRockInside_GrowTabletFlame).
DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH(MagmaRockInside_LightTabletTorch,
                                     FLAG_TABLET_TORCH_LIT, ACTOR_TORCH_FLAME,
                                     136, 360)

/// Blaze's spray reaches the east bowl on the Blaze tablet room's south-west
/// ledge, and its flame grows until it burns full; the ground rumbles, the
/// camera turns to the stone pillar between the bowls, and it sinks into the
/// floor until its top is a stepping stone across the lava (cell (7, 21)),
/// the way out of the room.
void MagmaRockInside_GrowTabletFlame(void) {
  struct Actor *object;
  struct Actor *actor;
  struct Actor *other;
  s32 i;

  object = Actor_Get(gFieldWork->psynergyHitObject);
  FieldActor_SetSpriteColor(object, SPRITE_COLOR_NORMAL);
  Event_PlaySound(SE_WHOOSH);
  i = 0;
  do {
    object->scaleY = object->scaleX += FIXED_ONE / FLAME_GROW_STEPS;
    System_Wait(5);
    i++;
  } while (i != FLAME_GROW_STEPS);
  Event_SetFlag(FLAG_TABLET_TORCH_LIT);
  other = Actor_Get(ACTOR_TORCH_PILLAR);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  MapGrid_SetShake(2, 1, 1);
  Event_PlaySound(SE_RUMBLE);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(120, CAMERA_KEEP, 344, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(30);
  Event_PlaySound(SE_RISE_WITH_SPARKS_LOW);
  REG_BLDALPHA = BLEND_ALPHA(0, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  {
    struct Actor *prepared = OverlayObject_PrepareObject(
        other->x.fixed, other->groundHeight, other->z.fixed + toFIXED(48),
        SPRITE_YELLOW_BLOCK);

    other->motionFlags = 0;
    actor = prepared;
  }
  i = 0;
  do {
    other->y.fixed -= toFIXED(1);
    FieldEvent_Wait(5);
    i++;
  } while (i != PILLAR_SINK_DEPTH);
  Event_PlaySound(SE_FLAME_GROW);
  Actor_SelectAnimation(other, 4);
  Actor_Release(actor);
  FieldEvent_Wait(1);
  REG_BLDCNT = 0;
  MapCell_SetAttributes(0, 7, 21, MAP_CELL_JUMP);
  Event_End();
}

/// Nothing visible happens when Blaze, cast at the flame on the Blaze tablet
/// room's south-west ledge, does not reach the east bowl; the game takes away
/// the hidden flame it had set waiting there.
void MagmaRockInside_PutOutTabletFlame(void) {
  if (gFieldWork->psynergyHitObject == -1) {
    if (!Event_GetFlag(FLAG_TABLET_TORCH_LIT))
      Actor_SetPosition(ACTOR_TORCH_FLAME, 0, 0);
  }
}

/// Leaves a small red spark behind a spark circling in to Jenna as she learns
/// Blaze (MagmaRockInside_OrbitParent).
void MagmaRockInside_SpawnSpark(struct Actor *source) {
  struct Actor *actor = Create(SPRITE_PARTICLE, source->x.fixed,
                               source->y.fixed, source->z.fixed);

  actor->scaleY = FIXED_ONE * 2 / 5;
  actor->scaleX = FIXED_ONE * 2 / 5;
  actor->motionFlags = 0;
  FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
  Scene_SetActorSpritePriority(actor, 1);
  Actor_SelectAnimation(actor, 1);
  Actor_SetSpriteFlags(actor, 0);
  Actor_SetBlendMode(actor, 1);
  Actor_StartScript(actor, gMagmaRockInside_FlameScript);
}

/// A spark circling Jenna as she learns Blaze, as its script reads the actor:
/// its place, the radius it circles at, the steps it has left, its angle and
/// speed, and the actor it circles.
struct SparkOrbiter {
  u8 beforeX[8];
  s32 x;
  s32 y;
  s32 z;
  u8 beforeRadius[56];
  s32 radius;
  u8 beforeTimer[18];
  u8 stepsLeft;
  u8 afterStepsLeft;
  u16 angle;
  u16 speed;
  struct SparkOrbiter *parent;
};

/// A spark's script call: circles in to its parent, leaving sparks, until its
/// steps run out; 0 ends its script.
s32 MagmaRockInside_OrbitParent(struct SparkOrbiter *object) {
  u16 *angle = &object->angle;
  u16 *speed = &object->speed;
  struct SparkOrbiter *parent = object->parent;
  s32 *position;
  s16 heading;

  object->angle += (s16)*speed >> 1;
  object->x = parent->x;
  object->y = parent->y + toFIXED(8);
  object->z = parent->z;
  heading = *angle;
  FieldVector_AddPolarOffsetFX(object->radius, heading, position = &object->x);
  if (--object->stepsLeft == 0)
    return 0;
  if (object->radius == toFIXED(2))
    return 1;
  MagmaRockInside_SpawnSpark(object);
  object->radius -= toFIXED(4);
  object->angle += (s16)*speed >> 1;
  *position = parent->x;
  object->y = parent->y + toFIXED(8);
  object->z = parent->z;
  FieldVector_AddPolarOffsetFX(object->radius, (s16)*angle, position);
  MagmaRockInside_SpawnSpark(object);
  object->radius -= toFIXED(4);
  return 1;
}

// Moves around in a circle.
#include "shared/orbit_update.h"

#include "shared/actor_aim_at_watched.h"

/// A rising spark's script call: drifts by its centre and radius, kept as
/// velocities, for its phase's count of steps; 0 ends its script.
s32 MagmaRockInside_DriftForSteps(struct Orbit *object) {
  u8 *steps;
  s32 left;
  u8 stepsLeft;

  object->x += object->centreX;
  object->y += object->orbitRadius;
  object->z += object->centreZ;
  steps = &object->phase;
  left = *steps + (u8)-1;
  *steps = left;
  stepsLeft = left;
  return stepsLeft != 0;
}

/// An actor's update: drops a white sparkle near it every eighth frame.
void MagmaRockInside_TrailSparks(struct Actor *actor) {
  struct EffectOptions options;
  FIXED position[3];
  u32 frame = gFrameCount & 7;

  if (frame != 0)
    return;
  options.palette = SPRITE_COLOR_WHITE;
  options.type = SPRITE_SPARKLE;
  options.priority = 1;
  position[0] = actor->x.fixed + (((Random16() & 31) - 16) << 16);
  position[1] = actor->y.fixed + toFIXED(Random16() & 31) + toFIXED(8);
  position[2] = actor->z.fixed;
  FieldEffect_Spawn(position[0], position[1], position[2] + toFIXED(4), 0,
                    toFIXED(1) / 2, frame,
                    EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                    &options);
}

extern s32 gMagmaRockInside_TabletRiseScript[];
extern s32 gMagmaRockInside_RisingSparkScript[];
extern s32 gMagmaRockInside_CirclingSparkScript[];
extern s32 gMagmaRockInside_JennaRiseScript[];
extern s32 gMagmaRockInside_OrbitScript[];
extern s32 gMagmaRockInside_TabletSettleScript[];
extern s32 gMagmaRockInside_JennaSettleScript[];

/// The Blaze tablet's lesson: the music fades, the ground shakes and the
/// screen takes on a warm tint as the tablet lifts, blinking and shooting
/// sparks up; red sparks circle in to the one who learns and rise round her as
/// she lifts too, the screen flashes white, and both settle back as the music
/// returns.
void MagmaRockInside_LearnBlaze(s32 tabletId, s32 learnerId) {
  struct Actor *tablet = Actor_GetById(tabletId);
  struct Actor *learner = Actor_GetById(learnerId);
  struct Actor *actor;
  struct Actor *spark;
  struct Actor *riser;
  s32 i;
  u16 noMotion;
  s32 probe;
  const u8 *script;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  s32 unused[3];

  script = gMagmaRockInside_JennaRiseScript;
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Event_Wait(30);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveToFX(toFIXED(tablet->x.part[0].integer),
                       toFIXED(tablet->y.part[0].integer),
                       toFIXED(tablet->z.part[0].integer), 1);
  Event_PlaySound(SE_RUMBLE);
  MapGrid_SetShake(2, 2, 1);
  PaletteFade_SetSource(PALETTE_TINT_NONE, 0);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 1);
  PaletteFade_StartInterpolation(60);
  tablet->motionFlags = 0;
  tablet->acceleration = toFIXED(1) / 5;
  tablet->speedLimit = toFIXED(1) / 5;
  Actor_SetScript(tabletId, gMagmaRockInside_TabletRiseScript);
  Event_PlaySound(SE_TABLET_RISE);
  Event_Wait(45);
  Actor_SetSpriteColorById(tabletId, SPRITE_COLOR_BLINK);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  i = 0;
  do {
    actor = (struct Actor *)Import_ActorCreate(
        SPRITE_PARTICLE, tablet->x.fixed, tablet->y.fixed, tablet->z.fixed);
    Event_PlaySound(SE_GLITTER);
    actor->x.fixed += ((Random16() & 31) - 16) << 16;
    actor->y.fixed += ((Random16() & 15) - 8) << 16;
    actor->speedLimit = toFIXED(8);
    actor->acceleration = toFIXED(1) / 5;
    actor->motionFlags = 0;
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
    Scene_SetActorSpritePriority(actor, 1);
    Actor_SelectAnimation(actor, 7);
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetBlendMode(actor, 1);
    Actor_StartScript(actor, gMagmaRockInside_RisingSparkScript);
    REG_BLDALPHA = BLEND_ALPHA(4, 16);
    wait(2);
    REG_BLDALPHA = BLEND_ALPHA(10, 16);
    wait(2);
    REG_BLDALPHA = BLEND_ALPHA(16, 16);
    wait(2);
    REG_BLDALPHA = BLEND_ALPHA(10, 16);
    wait(2);
    i++;
  } while (i != TABLET_RISING_SPARKS);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  wait(30);
  for (i = 0; i != LEARN_SPARKS; i++) {
    spark = FieldActor_Create(SPRITE_PARTICLE, learner->x.fixed,
                              learner->y.fixed, learner->z.fixed);
    Event_PlaySound(SE_SPARKS);
    spark->timer = (Random16() & 0x7fff) + DEGtoANG(180);
    spark->userValue = (((i & 1) << 1) - 1) << ((Random16() & 3) + 9);
    noMotion = 0;
    spark->motionFlags = noMotion;
    spark->riseBaseY = toFIXED(122);
    spark->watchedActor = learner;
    spark->stepsLeft =
        (LEARN_SPARKS - i) * LEARN_SPARK_INTERVAL + LEARN_SPIRAL_FRAMES;
    FieldActor_SetSpriteColor(spark, SPRITE_COLOR_RED_TINT);
    Scene_SetActorSpritePriority(spark, 1);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    FieldVector_AddPolarOffsetFX(spark->riseBaseY, spark->timer,
                                 &spark->x.fixed);
    Actor_StartScript(spark, gMagmaRockInside_CirclingSparkScript);
    if (i == 3) {
      Actor_SetSpriteColorById(learnerId, SPRITE_COLOR_BLINK);
      learner->motionFlags = noMotion;
      learner->acceleration = toFIXED(1) / 5;
      learner->speedLimit = toFIXED(1) / 5;
      Actor_SetScript(learnerId, script);
    }
    wait(LEARN_SPARK_INTERVAL);
  }
  Event_PlaySound(SE_SHIMMER);
  wait(16);
  i = 2;
  do {
    riser = FieldActor_Create(SPRITE_PARTICLE, learner->x.fixed,
                              learner->y.fixed + i * toFIXED(1) - toFIXED(8),
                              learner->z.fixed);
    riser->timer = Random16();
    riser->userValue = TRIG_TURN / 32;
    ((struct Orbit *)riser)->phase = i;
    ((struct Orbit *)riser)->growth = 1;
    riser->bounce = riser->x.fixed;
    riser->riseBaseY = 0;
    riser->gravity = riser->z.fixed;
    riser->watchedActor = learner;
    riser->motionFlags = (u16)0;
    riser->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    FieldActor_SetSpriteColor(riser, SPRITE_COLOR_RED_TINT);
    Actor_SelectAnimation(riser, 7);
    Actor_SetSpriteFlags(riser, 0);
    Actor_SetBlendMode(riser, 1);
    Actor_StartScript(riser, gMagmaRockInside_OrbitScript);
    i += 2;
  } while (i != ORBIT_HALF_TURN);
  Event_PlaySound(SE_SHIMMER);
  wait(50);
  PaletteFade_SetTarget(RGB_WHITE, 0);
  PaletteFade_StartInterpolation(8);
  wait(16);
  MapGrid_SetShake(0, 0, 0);
  Actor_SetSpriteColorById(tabletId, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(learnerId, SPRITE_COLOR_NORMAL);
  Event_PlaySound(SE_FLASH);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(80);
  Actor_SetScript(tabletId, gMagmaRockInside_TabletSettleScript);
  Actor_SetScript(learnerId, gMagmaRockInside_JennaSettleScript);
  learner->update = (ObjectUpdate)MagmaRockInside_TrailSparks;
  wait(120);
  learner->update = NULL;
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Scene_PlayMusic();
  wait(1);
}

s32 Actor_PlaceOnActor();

/// Felix reads the Blaze tablet from the south: "Wielder of Flame's strength,
/// Lay your hands upon this stone." The first time, Jenna steps out beside him
/// ("Flame's strength? Hey, that must mean it's finally my turn!"), he nods and
/// steps back, she lays her hands on the tablet and learns Blaze, and after
/// they nod to each other she walks back into the party. Once Blaze is
/// learned, only the inscription is shown.
void MagmaRockInside_ReadBlazeTablet(void) {
  struct Actor *actor;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered(MsgBlazeTabletInscription, 1);
  if (Event_GetFlag(FLAG_BLAZE_LEARNED) != 0) {
    Event_End();
  } else {
    FieldEvent_SetMessageId((s32)MsgBlazeTabletInscription);
    gFieldWork->message++;
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.7);
    Actor_SetSpeed(ACTOR_JENNA, 0.7);
    Actor_PlaceOnActor(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
    FieldEvent_Wait(1);
    Actor_WalkTo(gMainRoutineWork.leaderActorId, 256, 136);
    Actor_WalkTo(ACTOR_JENNA, 272, 136);
    FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    FieldActor_WaitForMoveById(ACTOR_JENNA);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
    FieldEvent_Wait(10);
    // "Flame's strength? Hey, that must mean it's finally my turn!"
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
    FieldEvent_Wait(30);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 256, 152);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
    FieldEvent_Wait(30);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_WalkToAndWait(ACTOR_JENNA, 264, 137);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
    FieldEvent_Wait(20);
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
    Message_ShowCentered(MsgBlazeTabletInscription + 3, 1);
#else
    Message_ShowCentered(MsgBlazeTabletInscription + 2, 1);
#endif
    MapGrid_SetShake(5, 7, 1);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    FieldMapGrid_WaitForShakeToSettle();
    MagmaRockInside_LearnBlaze(ACTOR_BLAZE_TABLET, ACTOR_JENNA);
    Party_LearnAbility(JENNA, ABILITY_BLAZE);
    Event_SetFlag(FLAG_BLAZE_LEARNED);
    Actor_FaceEachOther(gMainRoutineWork.leaderActorId, ACTOR_JENNA, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
    FieldEvent_Wait(20);
    Actor_SetSpeed(ACTOR_JENNA, 1.2);
    Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
    actor = Actor_Get(gMainRoutineWork.leaderActorId);
    if (actor != 0)
      Actor_SetDestination(ACTOR_JENNA, actor->x.part[0].integer,
                           actor->z.part[0].integer);
    FieldActor_WaitForMoveById(ACTOR_JENNA);
    Actor_SetPosition(ACTOR_JENNA, 0, 0);
    Actor_SetMovementType(Actor_Get(ACTOR_BLAZE_TABLET),
                          MOVEMENT_TYPE_TAKE_LEADER_LAYER);
    Event_End();
  }
}

/// Felix reads the Blaze tablet from behind, on its north side: the first time
/// he walks round its west side to the front and it is read there
/// (MagmaRockInside_ReadBlazeTablet); once Blaze is learned only its
/// inscription is shown.
void MagmaRockInside_ReadBlazeTabletFromNorth(void) {
  if (Event_GetFlag(FLAG_BLAZE_LEARNED)) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgBlazeTabletInscription, 1);
    Event_End();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 248, 107);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 248, 136);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 264, 136);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    MagmaRockInside_ReadBlazeTablet();
  }
}

#define DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM(name, n1)                    \
  void name(void) {                                                            \
    if (Event_GetFlag(FLAG_BLAZE_LEARNED)) {                                   \
      Event_Start();                                                           \
      FieldEvent_ResolvePendingActions(0);                                     \
      Message_ShowCentered(MsgBlazeTabletInscription, 1);                      \
      Event_End();                                                             \
    } else {                                                                   \
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, n1, 136);            \
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 264, 136);           \
      Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);    \
      FieldEvent_Wait(10);                                                     \
      MagmaRockInside_ReadBlazeTablet();                                       \
    }                                                                          \
  }

/// Felix reads the Blaze tablet from its east side: the first time he steps
/// round to the front and it is read there (MagmaRockInside_ReadBlazeTablet);
/// once Blaze is learned only its inscription is shown.
DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM(
    MagmaRockInside_ReadBlazeTabletFromEast, 280)

/// Felix reads the Blaze tablet from its west side: the first time he steps
/// round to the front and it is read there (MagmaRockInside_ReadBlazeTablet);
/// once Blaze is learned only its inscription is shown.
DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM(
    MagmaRockInside_ReadBlazeTabletFromWest, 248)

extern struct Trigger gMagmaRockInside_Triggers[];
extern struct Trigger gMagmaRockInside_EntranceTriggers[];
extern struct Trigger gMagmaRockInside_FuryTriggers[];
extern struct Trigger gMagmaRockInside_CrackedPillarsTriggers[];
extern struct Trigger gMagmaRockInside_GolemCoreChestTriggers[];
extern struct Trigger gMagmaRockInside_SalamanderTailChestTriggers[];
extern struct Trigger gMagmaRockInside_EmberStoneTriggers[];
extern struct Trigger gMagmaRockInside_BlazeTabletTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene != SCENE_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST) {
    if (scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
      return gMagmaRockInside_EntranceTriggers;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_FURY)
      return gMagmaRockInside_FuryTriggers;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
      return gMagmaRockInside_CrackedPillarsTriggers;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST)
      return gMagmaRockInside_GolemCoreChestTriggers;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST)
      return gMagmaRockInside_SalamanderTailChestTriggers;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE)
      return gMagmaRockInside_EmberStoneTriggers;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET)
      return gMagmaRockInside_BlazeTabletTriggers;
  }
  return gMagmaRockInside_Triggers;
}

/// The entrance hall as flooded: its flag set, the cell at (15, 13) no longer
/// climbable, and its columns and pillar shown standing in the lava.
void MagmaRockInside_SetEntranceFlooded(void) {
  s32 i;

  Event_SetFlag(FLAG_ENTRANCE_FLOODED);
  MapCell_SetAttributes(0, 15, 13, 0);
  for (i = 0; i != 6; i++)
    Actor_SetAnimation(i + ACTOR_COLUMN_A, 2);
  Actor_SetAnimation(ACTOR_ENTRANCE_PILLAR, 4);
}

/// Fury's room as flooded: its flag set, the cells at (24, 15) and (9, 16) no
/// longer climbable, and its pillar shown standing in the lava; from the
/// cracked pillars' room, only the cell at (26, 14).
void MagmaRockInside_SetFuryFlooded(void) {
  Event_SetFlag(FLAG_FURY_FLOODED);
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY) {
    MapCell_SetAttributes(0, 24, 15, 0);
    MapCell_SetAttributes(0, 9, 16, 0);
    Actor_SetAnimation(ACTOR_FURY_PILLAR, 4);
  } else {
    MapCell_SetAttributes(0, 26, 14, 0);
  }
}

/// The lower rooms as flooded: their flag set and, in the cracked pillars'
/// room, its eight white platforms afloat in pairs, each pair's second starting
/// across the square from the first, and its three pillars and column shown
/// standing in the lava.
void MagmaRockInside_SetLowerRoomsFlooded(void) {
  Event_SetFlag(FLAG_LOWER_ROOMS_FLOODED);
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS) {
    s32 *script = gMagmaRockInside_CrackedPillarsFirstPairPath;

    MagmaRockInside_StartLavaPlatform(ACTOR_LAVA_PLATFORM_A, script,
                                      FLAG_LOWER_ROOMS_FLOODED);
    MagmaRockInside_StartLavaPlatform(
        ACTOR_LAVA_PLATFORM_B, gMagmaRockInside_CrackedPillarsFirstPairPath,
        FLAG_LOWER_ROOMS_FLOODED);
    MagmaRockInside_StartLavaPlatform(
        ACTOR_LAVA_PLATFORM_C, gMagmaRockInside_CrackedPillarsSecondPairPath,
        FLAG_LOWER_ROOMS_FLOODED);
    MagmaRockInside_StartLavaPlatform(
        ACTOR_LAVA_PLATFORM_D, gMagmaRockInside_CrackedPillarsSecondPairPath,
        FLAG_LOWER_ROOMS_FLOODED);
    MagmaRockInside_StartLavaPlatform(
        ACTOR_LAVA_PLATFORM_E, gMagmaRockInside_CrackedPillarsThirdPairPath,
        FLAG_LOWER_ROOMS_FLOODED);
    MagmaRockInside_StartLavaPlatform(
        ACTOR_LAVA_PLATFORM_F, gMagmaRockInside_CrackedPillarsThirdPairPath,
        FLAG_LOWER_ROOMS_FLOODED);
    MagmaRockInside_StartLavaPlatform(
        ACTOR_LAVA_PLATFORM_G, gMagmaRockInside_CrackedPillarsFourthPairPath,
        FLAG_LOWER_ROOMS_FLOODED);
    MagmaRockInside_StartLavaPlatform(
        ACTOR_LAVA_PLATFORM_H, gMagmaRockInside_CrackedPillarsFourthPairPath,
        FLAG_LOWER_ROOMS_FLOODED);
    if (Actor_Get(ACTOR_LAVA_PLATFORM_B)->scriptCursor == 0) {
      Actor_Get(ACTOR_LAVA_PLATFORM_B)->scriptCursor =
          PLATFORM_PATH_THIRD_CORNER;
      Actor_Get(ACTOR_LAVA_PLATFORM_D)->scriptCursor =
          PLATFORM_PATH_THIRD_CORNER;
      Actor_Get(ACTOR_LAVA_PLATFORM_F)->scriptCursor =
          PLATFORM_PATH_THIRD_CORNER;
      Actor_Get(ACTOR_LAVA_PLATFORM_H)->scriptCursor =
          PLATFORM_PATH_THIRD_CORNER;
    }
    Actor_SetAnimation(ACTOR_PILLAR_A, 4);
    Actor_SetAnimation(ACTOR_PILLAR_B, 4);
    Actor_SetAnimation(ACTOR_PILLAR_C, 4);
    Actor_SetAnimation(ACTOR_LOWER_COLUMN, 2);
  }
}

/// The entrance hall pillar's update: its drawing order comes from the cells
/// under it, except while Felix stands at its height and not in front of it,
/// when it is drawn at the lowest priority.
void MagmaRockInside_LayerWithLeader(struct Actor *arg) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *actor;
  struct Actor *owner;
  s16 ownerY;
  s16 y;
  u8 *flags;

  actor = arg;
  owner = Actor_Get(party->leaderActorId);
  ownerY = FIXEDtoINT(owner->y.fixed);
  y = FIXEDtoINT(actor->y.fixed);
  flags = &actor->priorityFlags;

  if (ownerY != y ||
      (s16)(FIXEDtoINT(owner->z.fixed)) < (s16)(FIXEDtoINT(actor->z.fixed))) {
    *flags = 1;
  } else {
    *flags = 0;
    Scene_SetActorSpritePriority(actor, 3);
  }
}

#undef DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR

// Magma Rock's inside: the crumbling pillars and floors of its outside, unused
// here.

void Tightrope_Initialize(void);

extern s32 gMagmaRockInside_LuckyMedalPlatformPath[];
extern s32 gMagmaRockInside_FuryWestPlatformPath[];
extern s32 gMagmaRockInside_FuryEastPlatformPath[];
extern s32 gMagmaRockInside_CrackedPillarsWestPlatformPath[];
extern s32 gMagmaRockInside_CrackedPillarsEastPlatformPath[];
extern s32 gMagmaRockInside_SalamanderTailNorthPlatformPath[];
extern s32 gMagmaRockInside_SalamanderTailSouthwestPlatformPath[];
extern s32 gMagmaRockInside_SalamanderTailSoutheastPlatformPath[];
extern s16 gMagmaRockInside_Statues[];

void Actor_PlaceBlocks(const u8 *table);
void Tightrope_Place(s32 a, s32 b, s32 c);
s32 MapCell_SetAttributes(u32 layer, s32 x, s32 y, s32 value);

#define CLOSED_BLEND()                                                         \
  {                                                                            \
    REG_BLDALPHA = BLEND_ALPHA(0, 16);                                         \
    REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;      \
  }

/// Stands an unseen marker 20 pixels up on a floor switch, falling, as a
/// platform Felix stands on.
static __inline__ void Raise(s32 id) {
  struct Actor *actor = Actor_GetById(id);

  actor->motionFlags = ACTOR_FALLS;
  actor->y.fixed = actor->groundHeight = toFIXED(20);
  actor->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
}

/// Settles an actor at the given height, its gravity off.
static __inline__ void Lower(s32 id, s32 height) {
  Actor_GetById(id)->motionFlags = 0;
  Actor_GetById(id)->y.fixed = Actor_GetById(id)->groundHeight = height;
}

s32 Scene_Initialize(void) {
  s32 i;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST) {
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
    MagmaRockInside_StartLavaPlatform(ACTOR_LUCKY_MEDAL_PLATFORM,
                                      gMagmaRockInside_LuckyMedalPlatformPath,
                                      FLAG_ENTRANCE_FLOODED);
    if (!Event_GetFlag(FLAG_ENTRANCE_FLOODED)) {
      CLOSED_BLEND();
      MapGrid_CopyCellsTo(71, 64, 47, 98, 1, 1);
    }
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE) {
    s32 j;

    FieldEffect_StartBlankOverlay(0);
    Actor_GetById(ACTOR_ENTRANCE_PILLAR)->update =
        MagmaRockInside_LayerWithLeader;
    for (j = 0; j != 6; j++) {
      {
        struct Actor *actor = Actor_GetById(j + ACTOR_COLUMN_A);

        actor->motionFlags = 0;
        actor->y.fixed = actor->groundHeight = 0;
      }
    }
    if (Event_GetFlag(FLAG_ENTRANCE_PILLAR_PUSHED))
      Actor_GetById(ACTOR_ENTRANCE_PILLAR)->z.fixed = toFIXED(264);
    Actor_PlaceBlocks(gMagmaRockInside_EntrancePillars);
    MagmaRockInside_RaiseActorCells(gMagmaRockInside_Statues);
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
    Raise(ACTOR_ENTRANCE_SWITCH_MARKER);
    if (Event_GetFlag(FLAG_ENTRANCE_FLOODED))
      MagmaRockInside_SetEntranceFlooded();
    else
      CLOSED_BLEND();
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY) {
    FieldEffect_StartBlankOverlay(0);
    if (Event_GetFlag(FLAG_FURY_PILLAR_PUSHED))
      Actor_GetById(ACTOR_FURY_PILLAR)->z.fixed = toFIXED(664);
    Actor_PlaceBlocks(gMagmaRockInside_FuryPillars);
    Tightrope_Initialize();
    Tightrope_Place(0, ACTOR_TIGHTROPE_WEST_POST, ACTOR_TIGHTROPE_EAST_POST);
    MagmaRockInside_RaiseActorCells(gMagmaRockInside_Statues);
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
    Raise(ACTOR_FURY_SWITCH_MARKER);
    MagmaRockInside_StartLavaPlatform(ACTOR_WEST_PLATFORM,
                                      gMagmaRockInside_FuryWestPlatformPath,
                                      FLAG_FURY_FLOODED);
    MagmaRockInside_StartLavaPlatform(ACTOR_EAST_PLATFORM,
                                      gMagmaRockInside_FuryEastPlatformPath,
                                      FLAG_FURY_FLOODED);
    if (Event_GetFlag(FLAG_FURY_FLOODED))
      MagmaRockInside_SetFuryFlooded();
    else
      CLOSED_BLEND();
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS) {
    FieldEffect_StartBlankOverlay(0);
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
    MagmaRockInside_RaiseActorCells(gMagmaRockInside_CrackedPillarsStatues);
    if (gMainRoutineWork.entrance <= 7) {
      MagmaRockInside_StartLavaPlatform(
          ACTOR_WEST_PLATFORM, gMagmaRockInside_CrackedPillarsWestPlatformPath,
          FLAG_FURY_FLOODED);
      MagmaRockInside_StartLavaPlatform(
          ACTOR_EAST_PLATFORM, gMagmaRockInside_CrackedPillarsEastPlatformPath,
          FLAG_FURY_FLOODED);
      if (Event_GetFlag(FLAG_FURY_FLOODED)) {
        MagmaRockInside_SetFuryFlooded();
      } else {
        CLOSED_BLEND();
        MapGrid_CopyCellsTo(71, 64, 35, 82, 1, 1);
      }
    } else {
      Actor_GetById(ACTOR_PILLAR_A)->priorityFlags |=
          ACTOR_PRIORITY_IGNORES_RISES;
      Actor_GetById(ACTOR_PILLAR_B)->priorityFlags |=
          ACTOR_PRIORITY_IGNORES_RISES;
      Actor_GetById(ACTOR_PILLAR_C)->priorityFlags |=
          ACTOR_PRIORITY_IGNORES_RISES;
      Actor_SetSpritePriority(ACTOR_PILLAR_A, 3);
      Actor_SetSpritePriority(ACTOR_PILLAR_B, 3);
      Actor_SetSpritePriority(ACTOR_PILLAR_C, 3);
      if (Event_GetFlag(FLAG_PILLAR_A_PUSHED))
        Actor_GetById(ACTOR_PILLAR_A)->x.fixed = toFIXED(344);
      if (Event_GetFlag(FLAG_PILLAR_B_PUSHED))
        Actor_GetById(ACTOR_PILLAR_B)->x.fixed = toFIXED(344);
      if (Event_GetFlag(FLAG_PILLAR_C_PUSHED))
        Actor_GetById(ACTOR_PILLAR_C)->x.fixed = toFIXED(344);
      Actor_PlaceBlocks(gMagmaRockInside_CrackedPillarsPillars);
      Lower(ACTOR_LOWER_COLUMN, 0);
      Actor_GetById(ACTOR_LAVA_PLATFORM_A)->retainSpriteCache = 3;
      Actor_GetById(ACTOR_LAVA_PLATFORM_B)->retainSpriteCache = 3;
      Actor_GetById(ACTOR_LAVA_PLATFORM_C)->retainSpriteCache = 3;
      Actor_GetById(ACTOR_LAVA_PLATFORM_D)->retainSpriteCache = 3;
      Actor_GetById(ACTOR_LAVA_PLATFORM_E)->retainSpriteCache = 3;
      Actor_GetById(ACTOR_LAVA_PLATFORM_F)->retainSpriteCache = 3;
      Actor_GetById(ACTOR_LAVA_PLATFORM_G)->retainSpriteCache = 3;
      Actor_GetById(ACTOR_LAVA_PLATFORM_H)->retainSpriteCache = 3;
      MagmaRockInside_StartLavaPlatform(
          ACTOR_LAVA_PLATFORM_A, gMagmaRockInside_CrackedPillarsFirstPairPath,
          FLAG_LOWER_ROOMS_FLOODED);
      MagmaRockInside_StartLavaPlatform(
          ACTOR_LAVA_PLATFORM_B, gMagmaRockInside_CrackedPillarsFirstPairPath,
          FLAG_LOWER_ROOMS_FLOODED);
      MagmaRockInside_StartLavaPlatform(
          ACTOR_LAVA_PLATFORM_C, gMagmaRockInside_CrackedPillarsSecondPairPath,
          FLAG_LOWER_ROOMS_FLOODED);
      MagmaRockInside_StartLavaPlatform(
          ACTOR_LAVA_PLATFORM_D, gMagmaRockInside_CrackedPillarsSecondPairPath,
          FLAG_LOWER_ROOMS_FLOODED);
      MagmaRockInside_StartLavaPlatform(
          ACTOR_LAVA_PLATFORM_E, gMagmaRockInside_CrackedPillarsThirdPairPath,
          FLAG_LOWER_ROOMS_FLOODED);
      MagmaRockInside_StartLavaPlatform(
          ACTOR_LAVA_PLATFORM_F, gMagmaRockInside_CrackedPillarsThirdPairPath,
          FLAG_LOWER_ROOMS_FLOODED);
      MagmaRockInside_StartLavaPlatform(
          ACTOR_LAVA_PLATFORM_G, gMagmaRockInside_CrackedPillarsFourthPairPath,
          FLAG_LOWER_ROOMS_FLOODED);
      MagmaRockInside_StartLavaPlatform(
          ACTOR_LAVA_PLATFORM_H, gMagmaRockInside_CrackedPillarsFourthPairPath,
          FLAG_LOWER_ROOMS_FLOODED);
      if (Event_GetFlag(FLAG_LOWER_ROOMS_FLOODED))
        MagmaRockInside_SetLowerRoomsFlooded();
      else
        CLOSED_BLEND();
    }
  } else if (gMainRoutineWork.next ==
             SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST) {
    FieldEffect_StartBlankOverlay(0);
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
    HangingVine_Place(ACTOR_HANGING_CHAIN_A, 1);
    HangingVine_Place(ACTOR_HANGING_CHAIN_B, 1);
    Raise(ACTOR_NORTH_SWITCH_MARKER);
    Raise(ACTOR_SOUTH_SWITCH_MARKER);
    if (Event_GetFlag(FLAG_LOWER_ROOMS_FLOODED)) {
      MapCell_SetAttributes(0, 22, 26, 0);
      MapCell_SetAttributes(0, 25, 26, 0);
      Actor_SetSpritePriority(FIELD_TREASURE_ACTOR, 3);
    } else {
      CLOSED_BLEND();
    }
  } else if (gMainRoutineWork.next ==
             SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST) {
    FieldEffect_StartBlankOverlay(0);
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
    if (Event_GetFlag(FLAG_NORTH_PILLAR_PUSHED)) {
      MapGrid_CopyCellAttributes(46, 74, 1, 1, 46, 73);
      Actor_GetById(ACTOR_NORTH_PILLAR)->x.fixed = toFIXED(760);
    }
    if (Event_GetFlag(FLAG_EAST_PILLAR_PUSHED))
      Actor_SetPosition(ACTOR_EAST_PILLAR, 920, 616);
    Actor_PlaceBlocks(gMagmaRockInside_SalamanderTailChestPillars);
    MagmaRockInside_StartLavaPlatform(
        ACTOR_NORTH_PLATFORM, gMagmaRockInside_SalamanderTailNorthPlatformPath,
        FLAG_LOWER_ROOMS_FLOODED);
    MagmaRockInside_StartLavaPlatform(
        ACTOR_SOUTHWEST_PLATFORM,
        gMagmaRockInside_SalamanderTailSouthwestPlatformPath,
        FLAG_LOWER_ROOMS_FLOODED);
    MagmaRockInside_StartLavaPlatform(
        ACTOR_SOUTHEAST_PLATFORM,
        gMagmaRockInside_SalamanderTailSoutheastPlatformPath,
        FLAG_LOWER_ROOMS_FLOODED);
    if (Event_GetFlag(FLAG_LOWER_ROOMS_FLOODED)) {
      Actor_SetAnimation(ACTOR_NORTH_PILLAR, 4);
      if (Event_GetFlag(FLAG_EAST_PILLAR_PUSHED))
        Actor_SetAnimation(ACTOR_EAST_PILLAR, 4);
      if (Event_GetFlag(FLAG_NORTH_PILLAR_PUSHED))
        MapCell_SetAttributes(2, 46, 8, CELL_BEHAVIOR_WALL << 8);
      MapCell_SetAttributes(0, 10, 45, 0);
      MapCell_SetAttributes(0, 53, 40, 0);
    } else {
      CLOSED_BLEND();
      MapGrid_CopyCellsTo(71, 64, 50, 102, 1, 1);
    }
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE) {
    for (i = 0; i != 3; i++) {
      struct Actor *actor = Actor_GetById(i + ACTOR_SUNKEN_PILLAR_A);

      Actor_SelectAnimation(actor, 4);
      actor->motionFlags = 0;
      actor->y.fixed = actor->groundHeight = -toFIXED(PILLAR_SINK_DEPTH);
    }
    if (Event_GetFlag(FLAG_LEDGE_PILLAR_PUSHED)) {
      Actor_SetAnimation(ACTOR_LEDGE_PILLAR, 4);
      Actor_SetPosition(ACTOR_LEDGE_PILLAR, 696, 648);
    }
    Actor_PlaceBlocks(gMagmaRockInside_EmberStonePillars);
    FieldEffect_StartBlankOverlay(0);
    if (!Event_GetFlag(FLAG_WEST_TORCH_LIT))
      Actor_SetPosition(ACTOR_WEST_TORCH_FLAME, 0, 0);
    if (!Event_GetFlag(FLAG_EAST_TORCH_LIT))
      Actor_SetPosition(ACTOR_EAST_TORCH_FLAME, 0, 0);
    if (Event_GetFlag(FLAG_WEST_TORCH_LIT) &&
        Event_GetFlag(FLAG_EAST_TORCH_LIT)) {
      Lower(ACTOR_BLOCKING_PILLAR, -toFIXED(PILLAR_SINK_DEPTH));
      Actor_SetAnimation(ACTOR_BLOCKING_PILLAR, 4);
    } else {
      MapCell_SetAttributes(0, 38, 18, CELL_BEHAVIOR_WALL << 8);
    }
    if (Event_GetFlag(FLAG_MAGMA_BALL_TAKEN))
      Actor_SetAnimation(ACTOR_EMBER_STONE, 0);
    for (i = 0; i != 4; i++)
      Actor_SetAnimation(i + ACTOR_FALLEN_ROCK_A, 2);
    for (i = 0; i != ACTOR_LEDGE_PILLAR - ACTOR_EMBER_STONE; i++)
      Actor_SetSpriteFlags(Actor_GetById(i + ACTOR_EMBER_STONE), 0);
    if (!Event_GetFlag(FLAG_ROCKS_RELEASED)) {
      for (i = 0; i != 9; i++)
        Actor_SetPosition(i + ACTOR_EMBER_STONE, 0, 0);
    }
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET) {
    MagmaRockInside_StartStepCells();
    Event_SetFlag(FLAG_RETREAT_BLOCKED);
    for (i = 0; i != 6; i++) {
      if (Event_GetFlag(FLAG_MARKER_LIT(i + 2))) {
        s32 y = 24 - i * 2;

        MapGrid_CopyCellsTo(65, 1, 80, y, 1, 2);
        Call4((void (*)())MapCell_SetAttributes, 0, 16, y,
              CELL_BEHAVIOR_WALL << 8);
      }
    }
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_SOURCE_FLAME), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_TORCH_FLAME), 0);
    if (Event_GetFlag(FLAG_TABLET_TORCH_LIT)) {
      Lower(ACTOR_TORCH_PILLAR, -toFIXED(PILLAR_SINK_DEPTH));
      Actor_SetAnimation(ACTOR_TORCH_PILLAR, 4);
    } else {
      MapCell_SetAttributes(0, 7, 21, CELL_BEHAVIOR_WALL << 8);
      Actor_SetPosition(ACTOR_TORCH_FLAME, 0, 0);
    }
    Lower(ACTOR_BLAZE_TABLET, toFIXED(16));
    Actor_SetMovementType(Actor_GetById(ACTOR_BLAZE_TABLET),
                          MOVEMENT_TYPE_TAKE_LEADER_LAYER);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  s32 scene = gMainRoutineWork.next;
  s32 entrance = gMainRoutineWork.entrance;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    if ((scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE &&
         entrance == MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_STATUE) ||
        (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE &&
         entrance == MAGMA_ROCK_INSIDE_EMBER_STONE_FROM_MAGMA_ROCK_SLIDE)) {
      Event_ClearFlag(FLAG_ENTRANCE_FLOODED);
      Event_ClearFlag(FLAG_FURY_FLOODED);
      Event_ClearFlag(FLAG_LOWER_ROOMS_FLOODED);
    }
  }
  return 0;
}

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 MapCollision_FindOrAddTag(s32 unused, s32 key, s32 value);

// On entering, sets each listed pillar from its two flags, then lifts the
// leader onto the ground under it.
#include "shared/pillar_restore_raised.h"

#include "shared/leader_check_crumbling_step.h"

/// Unused: a crumbling pillar named in the (actor, flag) table gives way with a
/// shake, rubble and a thump, as on Magma Rock's outside.
UNUSED void MagmaRockInside_CrumblePillar(const void *data, s32 id) {
  const struct TrapEntry *entry = data;
  struct Actor *actor = Actor_Get(id);
  s32 flag;

  while (entry->actor != ACTOR_LIST_END) {
    if (entry->actor != id) {
      entry++;
      continue;
    }
    flag = entry->flag;
    Event_Start();
    if (!Event_GetFlag(flag + 1)) {
      Event_PlaySound(SE_PUSH);
      MapGrid_SetShake(1, 4, 1);
      Dust_Scatter(0);
      System_Wait(8);
      Actor_SelectAnimation(actor, 2);
      Actor_SetCellShape(-4, actor->layer, id);
      Dust_Scatter(1);
      System_Wait(16);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      System_Wait(8);
      FieldActor_SetPositionAndResetMotion(actor, 0, 0, 0);
      System_Wait(2);
      Event_PlaySound(SE_THUMP);
      MagmaRockInside_SpawnDustRing();
      System_Wait(20);
      Event_SetFlag(flag);
      Event_SetFlag(flag + 1);
    }
    Event_End();
    return;
  }
}

#include "shared/pillar_take_out_of_play.c"

// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
// it copy actor 9's visual attributes.
#include "shared/pillar_restore.h"

#include "shared/actor_take_place_of.h"

/// Unused: stepping onto a crumbling floor, as on Magma Rock's outside: the
/// first time it shakes and cracks, the second time it gives way with the
/// leader.
UNUSED void MagmaRockInside_StepOnTrap(const u16 *table) {
  struct Actor *leader;
  struct Actor *partner;
  struct Actor *trap;
  FIXED position[3];
  s32 id;
  s32 flag;
  s32 second;

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  partner = Actor_GetById(8);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  trap = Actor_FindAtCell(position, 0);
  if (trap == 0)
    return;
  while (*table != ACTOR_LIST_END) {
    id = table[0];
    if (Actor_GetById(id) != trap) {
      table += 2;
      continue;
    }
    {
      flag = table[1];
      Event_Start();
      if (!Event_GetFlag(flag)) {
        Actor_SelectAnimation(partner, 1);
        Actor_TakePlaceOf(partner, trap);
        wait(1);
        Event_PlaySound(SE_CHEST_CLOSE);
        FieldActor_SetSpriteColor(Actor_GetById(8), SPRITE_COLOR_WHITE);
        wait(2);
        Actor_SelectAnimation(partner, 0);
        Actor_CopyGraphics(id, 9);
        FieldActor_SetSpriteColor(Actor_GetById(8), SPRITE_COLOR_NORMAL);
        wait(2);
        FieldActor_SetSpriteColor(Actor_GetById(8), SPRITE_COLOR_WHITE);
        wait(4);
        FieldActor_SetSpriteColor(Actor_GetById(8), SPRITE_COLOR_NORMAL);
        Dust_Scatter(0);
        Actor_TakePlaceOf(trap, partner);
        wait(1);
        Event_SetFlag(flag);
      } else {
        second = flag + 1;
        if (!Event_GetFlag(second)) {
          Actor_SelectAnimation(partner, 0);
          Actor_TakePlaceOf(partner, trap);
          wait(1);
          leader->motionFlags = 0;
          Event_PlaySound(SE_PUSH);
          MapGrid_SetShake(1, 4, 1);
          Dust_Scatter(0);
          wait(8);
          Actor_SelectAnimation(partner, 2);
          Leader_SetCellShapeAt(-2, trap->layer);
          Dust_Scatter(1);
          wait(16);
          Leader_SetCellShapeAt(-2, trap->layer);
          wait(4);
          MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
          wait(8);
          leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
          wait(5);
          Actor_SetPositionAndResetMotion(partner, 0, 0, 0);
          wait(2);
          Event_PlaySound(SE_THUMP);
          MagmaRockInside_SpawnDustRing();
          wait(20);
          Event_SetFlag(second);
        }
      }
      Event_End();
      return;
    }
  }
}

#include "../field/field_effect_advance_motion_priority.c"

#undef CLOSED_BLEND
// Magma Rock's inside: its falling rocks and rubble, and its tables.

void Actor_StartScript(struct Actor *actor, const void *data);

#include "shared/rubble_scatter.c"

/// Throws a ring of 16 dust clouds out from the leader as a pillar lands.
void MagmaRockInside_SpawnDustRing(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  u32 frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  for (i = 0; i <= DUST_RING_PUFFS; i++) {
    effect = (struct FieldEffect *)FieldActor_Create(
        SPRITE_DUST_CLOUD, leader->x.fixed, leader->y.fixed, leader->z.fixed);
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript(effect, gMagmaRockInside_DustScript);
      effect->velocity.x = Trig_Cos(i * FACING_STEP);
      effect->velocity.y = 0;
      effect->velocity.z = Math_Sin(i * FACING_STEP);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += ((Random16() * 3) << 14 >> 16) - toFIXED(3) / 8;
      effect->velocity.z += ((Random16() * 3) << 13 >> 16) - toFIXED(3) / 16;
      effect->spin = ((Random16() << 12) >> 16) - TRIG_TURN / 32;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
  }
}

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

#include "shared/actor_set_cell_shape.h"

#include "shared/leader_set_cell_shape.h"

void BattleEffect_StepVector3(struct Particle *particle, s32 damping,
                              s32 gravity);

#include "shared/particle_update_falling.h"

/// Spawns count rocks around (x, y, z), each pushed out at a random angle and
/// thrown up and out along it.
void MagmaRockInside_DropRocks(s32 x, s32 y, s32 z, s32 count) {
  struct RockWork *work;
  s32 i;

  work = Ram_HeapSlots->menuWork;
  for (i = 0; i < count; i++) {
    s32 angle = Random16();
    struct Rock *rock = &work->rocks[work->next];

    rock->timer = 0;
    rock->position[0] = x;
    rock->position[1] = y;
    rock->position[2] = z;
    FieldVector_AddPolarOffsetFX((Random16() << 3) + toFIXED(8), angle,
                                 rock->position);
    rock->velocity[0] = 0;
    rock->velocity[1] = toFIXED(5);
    rock->velocity[2] = 0;
    FieldVector_AddPolarOffsetFX((Random16() << 1) + toFIXED(2), angle,
                                 rock->velocity);
    work->next++;
    work->next &= ROCK_COUNT - 1;
  }
}

/// Starts the falling rocks: loads their picture, prepares 64 small sprites and
/// adds the task that draws them.
void MagmaRockInside_StartFallingRocks(void) {
  struct RockWork *work =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct EffectSprite *sprite;

  MagmaRockInside_DecompressTaggedLz(
      Resource_GetTableEntry(RESOURCE_FALLING_ROCK), work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, sizeof(work->tiles), work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  for (i = 0; i < ROCK_COUNT; i++) {
    sprite = &work->sprites[i];
    MagmaRockInside_InitRockSprite(sprite, 8, 8, OBJ_SIZE_16X16, tile);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = SPRITE_COLOR_NORMAL;
    sprite->oam.priority = 0;
    sprite->depth = OAM_DEPTH_WINDOW;
    rock->timer = -1;
    rock++;
  }
  work->next = 0;
  FieldTask_AddFrame(Particle_UpdateFalling, TASK_PASS_FRAME);
}

/// Ends the falling rocks: removes their task and frees their picture and work.
void MagmaRockInside_EndFallingRocks(void) {
  struct RockWork *work = Ram_HeapSlots->menuWork;

  FieldTask_Remove(Particle_UpdateFalling);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

extern u32 gFieldEffect_LongScript[];

// clang-format off
#include "shared/field_effect_duration_scripts.c"

#include "shared/push_leader_steps.c"

/// An ember's actor script.
s32 gMagmaRockInside_EmberScript[] = {
    SC_ANIMATION(2),
    SC_WAIT(18),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

/// A flame ember's actor script.
s32 gMagmaRockInside_FlameEmberScript[] = {
    SC_ANIMATION(3),
    SC_WAIT(23),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

/// The Lucky Medal chest room's lava platform: round a rectangle, for as long
/// as the lava flows.
s32 gMagmaRockInside_LuckyMedalPlatformPath[] = {
    SC_MOVE(776, 16, 680), SC_SYNC,
    SC_MOVE(744, 16, 680), SC_SYNC,
    SC_MOVE(744, 16, 632), SC_SYNC,
    SC_MOVE(776, 16, 632), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// Fury's room's west lava platform: round a rectangle.
s32 gMagmaRockInside_FuryWestPlatformPath[] = {
    SC_MOVE(216, 16, 568), SC_SYNC,
    SC_MOVE(216, 16, 584), SC_SYNC,
    SC_MOVE(136, 16, 584), SC_SYNC,
    SC_MOVE(136, 16, 568), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// Fury's room's east lava platform: east to 760 once, and it stays there.
s32 gMagmaRockInside_FuryEastPlatformPath[] = {
    SC_MOVE(600, 16, 664), SC_SYNC,
    SC_MOVE(728, 16, 664), SC_SYNC,
    SC_MOVE(728, 16, 632), SC_SYNC,
    SC_MOVE(760, 16, 632), SC_SYNC,
    SC_MOVE(760, 16, 664), SC_SYNC,
    SC_END,
};

/// The west lava platform of the cracked pillars' room's upper hall, which
/// floods with Fury's room: round a rectangle.
s32 gMagmaRockInside_CrackedPillarsWestPlatformPath[] = {
    SC_MOVE(184, 16, 232), SC_SYNC,
    SC_MOVE(280, 16, 232), SC_SYNC,
    SC_MOVE(280, 16, 264), SC_SYNC,
    SC_MOVE(184, 16, 264), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// The east lava platform of the cracked pillars' room's upper hall, which
/// floods with Fury's room: round a rectangle.
s32 gMagmaRockInside_CrackedPillarsEastPlatformPath[] = {
    SC_MOVE(472, 16, 296), SC_SYNC,
    SC_MOVE(424, 16, 296), SC_SYNC,
    SC_MOVE(424, 16, 264), SC_SYNC,
    SC_MOVE(472, 16, 264), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// The cracked pillars' room's lava platforms A and B: one square, B starting
/// at its third corner (MagmaRockInside_SetLowerRoomsFlooded).
s32 gMagmaRockInside_CrackedPillarsFirstPairPath[] = {
    SC_MOVE(216, 16, 600), SC_SYNC,
    SC_MOVE(280, 16, 600), SC_SYNC,
    SC_MOVE(280, 16, 664), SC_SYNC,
    SC_MOVE(216, 16, 664), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// The cracked pillars' room's lava platforms C and D, as A and B.
s32 gMagmaRockInside_CrackedPillarsSecondPairPath[] = {
    SC_MOVE(312, 16, 664), SC_SYNC,
    SC_MOVE(376, 16, 664), SC_SYNC,
    SC_MOVE(376, 16, 600), SC_SYNC,
    SC_MOVE(312, 16, 600), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// The cracked pillars' room's lava platforms E and F, as A and B.
s32 gMagmaRockInside_CrackedPillarsThirdPairPath[] = {
    SC_MOVE(440, 16, 600), SC_SYNC,
    SC_MOVE(504, 16, 600), SC_SYNC,
    SC_MOVE(504, 16, 664), SC_SYNC,
    SC_MOVE(440, 16, 664), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// The cracked pillars' room's lava platforms G and H, as A and B.
s32 gMagmaRockInside_CrackedPillarsFourthPairPath[] = {
    SC_MOVE(536, 16, 664), SC_SYNC,
    SC_MOVE(600, 16, 664), SC_SYNC,
    SC_MOVE(600, 16, 600), SC_SYNC,
    SC_MOVE(536, 16, 600), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// The Salamander Tail's room's north lava platform: round a rectangle.
s32 gMagmaRockInside_SalamanderTailNorthPlatformPath[] = {
    SC_MOVE(376, 16, 264), SC_SYNC,
    SC_MOVE(376, 16, 232), SC_SYNC,
    SC_MOVE(408, 16, 232), SC_SYNC,
    SC_MOVE(408, 16, 264), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// The Salamander Tail's room's southwest lava platform: round a rectangle.
s32 gMagmaRockInside_SalamanderTailSouthwestPlatformPath[] = {
    SC_MOVE(216, 16, 680), SC_SYNC,
    SC_MOVE(280, 16, 680), SC_SYNC,
    SC_MOVE(280, 16, 712), SC_SYNC,
    SC_MOVE(216, 16, 712), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// The Salamander Tail's room's southeast lava platform: round a rectangle.
s32 gMagmaRockInside_SalamanderTailSoutheastPlatformPath[] = {
    SC_MOVE(376, 16, 680), SC_SYNC,
    SC_MOVE(312, 16, 680), SC_SYNC,
    SC_MOVE(312, 16, 712), SC_SYNC,
    SC_MOVE(376, 16, 712), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// The pillars Move pushes in the entrance, ended by -1.
s16 gMagmaRockInside_EntrancePillars[] = {
    ACTOR_ENTRANCE_PILLAR, -1,
};

/// The statue Burst wakes in the entrance hall and in Fury's room, as (actor,
/// the cell Burst is cast at, unused) rows ended by -1.
ALIGNED(4) s16 gMagmaRockInside_Statues[] = {
    ACTOR_STATUE, CELL_STATUE, -1, -1,
};

/// The pillars Move pushes in Fury's room, ended by -1.
s16 gMagmaRockInside_FuryPillars[] = {
    ACTOR_FURY_PILLAR, -1,
};

/// The cracked pillars' room's three statues, as (actor, the cell Burst is cast
/// at, unused) rows ended by -1 (Statue_FindUnderBurst).
s16 gMagmaRockInside_CrackedPillarsStatues[] = {
    ACTOR_STATUE, CELL_STATUE, -1,
    ACTOR_LOWER_WEST_STATUE, CELL_LOWER_WEST_STATUE, -1,
    ACTOR_LOWER_EAST_STATUE, CELL_LOWER_EAST_STATUE, -1,
    -1,
};

/// The pillars Move pushes in the cracked pillars' room, ended by -1.
s16 gMagmaRockInside_CrackedPillarsPillars[] = {
    ACTOR_PILLAR_A, ACTOR_PILLAR_B, ACTOR_PILLAR_C, -1,
};

/// The pillars Move pushes in the Salamander Tail's chest room, ended by -1.
s16 gMagmaRockInside_SalamanderTailChestPillars[] = {
    ACTOR_NORTH_PILLAR, ACTOR_EAST_PILLAR, -1,
};

/// The pillars Move pushes in the ember stone's room, ended by -1.
s16 gMagmaRockInside_EmberStonePillars[] = {
    ACTOR_LEDGE_PILLAR, -1, 0,
};

/// A small spark's actor script (MagmaRockInside_SpawnSpark).
s32 gMagmaRockInside_FlameScript[] = {
    SC_WAIT(6),
    SC_RELEASE,
    SC_END,
};

/// A spark shooting up from the Blaze tablet as it lifts: its actor script.
s32 gMagmaRockInside_RisingSparkScript[] = {                                                               
      SC_MOVE_BY(0, 160, 0),                                           
      SC_SYNC,                                                                 
      SC_RELEASE,                                                                 
      SC_END,                                                                  
  };

/// A circling spark's actor script (MagmaRockInside_OrbitParent).
s32 gMagmaRockInside_CirclingSparkScript[] = {
    SC_CALL(MagmaRockInside_OrbitParent),
    SC_RELEASE,
    SC_END,
};

s32 MagmaRockInside_DriftForSteps(struct Orbit *object);

/// A rising spark's actor script: it circles and drifts for its steps (MagmaRockInside_DriftForSteps).
s32 gMagmaRockInside_OrbitScript[] = {                                                               
      SC_CALL(Orbit_Update), SC_CALL(Actor_AimAtWatched), SC_CALL(MagmaRockInside_DriftForSteps), SC_RELEASE, SC_END,         
  };

/// The Blaze tablet's actor script as it lifts in the lesson (MagmaRockInside_LearnBlaze).
s32 gMagmaRockInside_TabletRiseScript[] = {                                                               
      SC_MOVE_BY(0, 24, 0),                                            
      SC_SYNC,                                                                 
      SC_SET(SW_MOTION, 4),                                                    
      SC_END,                                                                  
  };

/// Jenna's actor script as the sparks circle in to her (MagmaRockInside_LearnBlaze).
s32 gMagmaRockInside_JennaRiseScript[] = {                                                               
      SC_MOVE_BY(0, 16, 0),                                            
      SC_SYNC,                                                                 
      SC_SET(SW_MOTION, 4),                                                    
      SC_END,                                                                  
  };

/// The Blaze tablet's actor script as the lesson ends: it settles back.
s32 gMagmaRockInside_TabletSettleScript[] = {                                                               
      SC_SET(SW_MOTION, 0),                                                    
      SC_MOVE_BY(0, -24, 0),                                           
      SC_SYNC,                                                                 
      SC_SET(SW_MOTION, 0),                                                   
      SC_END,                                                                  
  };

/// Jenna's actor script as the lesson ends: she settles back.
s32 gMagmaRockInside_JennaSettleScript[] = {                                                               
      SC_SET(SW_MOTION, 0),                                                    
      SC_MOVE_BY(0, -16, 0),                                           
      SC_SYNC,                                                                 
      SC_SET(SW_MOTION, 3),                                                   
      SC_END,                                                                  
  };

/// A piece of rubble's actor script: it lasts 33 frames.
u32 gDust_ScatterScript[] = {                                                               
      SC_WAIT(33), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_RELEASE,                                                                 
  };

/// A dust cloud's actor script: it lasts 44 frames.
s32 gMagmaRockInside_DustScript[] = {                                                               
      SC_WAIT(44), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_RELEASE,                                                                 
  };

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(MagmaRockInsideLuckyMedalChest),
    SCENE_EXIT(1, MagmaRockInsideLuckyMedalChest, 4),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_A, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_B, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_B),
    SCENE_EXIT(4, MagmaRockInsideLuckyMedalChest, 1),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_A, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_B, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_C, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_C),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_FURY, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST),
    SCENE_EXITS(MagmaRockInsideEntrance),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_A, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_A, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_B, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_C, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_C),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_B, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_B),
    SCENE_EXIT(6, MagmaRockInsideEntrance, 7),
    SCENE_EXIT(7, MagmaRockInsideEntrance, 6),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_STATUE, MagmaRockStatue, MAGMA_ROCK_STATUE_FROM_MAGMA_ROCK_INSIDE_ENTRANCE),
    SCENE_EXITS(MagmaRockInsideFury),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_FURY),
    SCENE_EXIT(2, MagmaRockInsideFury, 6),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHWEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_NORTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_A, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_B, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_B),
    SCENE_EXIT(6, MagmaRockInsideFury, 2),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_WEST),
    SCENE_EXIT(8, MagmaRockInsideFury, 9),
    SCENE_EXIT(9, MagmaRockInsideFury, 8),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_SOUTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTH, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_SOUTH),
    SCENE_EXITS(MagmaRockInsideCrackedPillars),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHWEST, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_A, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_B, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_B),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_WEST, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_SOUTHWEST, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_SOUTH, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTH),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTH, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTH),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_A, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_B, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_WEST, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_WEST, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTHWEST, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_A, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_B, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B),
    SCENE_EXITS(MagmaRockInsideGolemCoreChest),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_WEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTHWEST, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_NORTHWEST),
    SCENE_EXIT(3, MagmaRockInsideGolemCoreChest, 9),
    SCENE_EXIT(4, MagmaRockInsideGolemCoreChest, 10),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTH, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_SOUTH),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_B),
    SCENE_EXIT(9, MagmaRockInsideGolemCoreChest, 3),
    SCENE_EXIT(10, MagmaRockInsideGolemCoreChest, 4),
    SCENE_EXITS(MagmaRockInsideSalamanderTailChest),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_WEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_NORTHWEST, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTH, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTH),
    SCENE_EXIT(8, MagmaRockInsideSalamanderTailChest, 14),
    SCENE_EXIT(9, MagmaRockInsideSalamanderTailChest, 16),
    SCENE_EXIT(10, MagmaRockInsideSalamanderTailChest, 17),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_SOUTH, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTH),
    SCENE_EXIT(12, MagmaRockInsideSalamanderTailChest, 18),
    SCENE_EXIT(13, MagmaRockInsideSalamanderTailChest, 19),
    SCENE_EXIT(14, MagmaRockInsideSalamanderTailChest, 8),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_EMBER_STONE, MagmaRockInsideEmberStone, MAGMA_ROCK_INSIDE_EMBER_STONE_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST),
    SCENE_EXIT(16, MagmaRockInsideSalamanderTailChest, 9),
    SCENE_EXIT(17, MagmaRockInsideSalamanderTailChest, 10),
    SCENE_EXIT(18, MagmaRockInsideSalamanderTailChest, 12),
    SCENE_EXIT(19, MagmaRockInsideSalamanderTailChest, 13),
    SCENE_EXITS(MagmaRockInsideEmberStone),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_INSIDE_BLAZE_TABLET, MagmaRockInsideBlazeTablet, MAGMA_ROCK_INSIDE_BLAZE_TABLET_FROM_MAGMA_ROCK_INSIDE_EMBER_STONE),
    SCENE_EXIT(2, MagmaRockInsideEmberStone, 5),
    SCENE_EXIT(3, MagmaRockInsideEmberStone, 6),
    SCENE_EXIT(4, MagmaRockInsideEmberStone, 7),
    SCENE_EXIT(5, MagmaRockInsideEmberStone, 2),
    SCENE_EXIT(6, MagmaRockInsideEmberStone, 3),
    SCENE_EXIT(7, MagmaRockInsideEmberStone, 4),
    SCENE_EXIT(8, MagmaRockInsideEmberStone, 9),
    SCENE_EXIT(9, MagmaRockInsideEmberStone, 8),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_EMBER_STONE),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_SLIDE, MagmaRockSlide, MAGMA_ROCK_SLIDE_FROM_MAGMA_ROCK_INSIDE_EMBER_STONE),
    SCENE_EXITS(MagmaRockInsideBlazeTablet),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_BLAZE_TABLET_TO_MAGMA_ROCK_INSIDE_EMBER_STONE, MagmaRockInsideEmberStone, MAGMA_ROCK_INSIDE_EMBER_STONE_FROM_MAGMA_ROCK_INSIDE_BLAZE_TABLET),
    SCENE_EXITS_END,
};

/// The actors placed in the Lucky Medal's room: its one white platform.
struct ActorPlacement gMagmaRockInside_Placements[] = {
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

/// The actors placed in the entrance.
struct ActorPlacement gMagmaRockInside_EntrancePlacements[] = {
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 6, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 7, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 8, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 9, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FIRE19), MOVEMENT_TYPE_STAND, 296, 0, 504, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The actors placed in Fury's room.
struct ActorPlacement gMagmaRockInside_FuryPlacements[] = {
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FURY), MOVEMENT_TYPE_STAND, 456, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The actors placed in the cracked pillars' room.
struct ActorPlacement gMagmaRockInside_CrackedPillarsPlacements[] = {
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, FLAG_INSIDE_CRACKED_PILLAR_A_BURST, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, FLAG_INSIDE_CRACKED_PILLAR_B_BURST, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 6, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 7, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 8, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 9, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 10, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 11, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 12, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 13, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 14, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 15, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 16, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 17, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 18, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 19, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

/// The actors placed in the Golem Core's chest room.
struct ActorPlacement gMagmaRockInside_GolemCoreChestPlacements[] = {
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The actors placed in the Salamander Tail's chest room.
struct ActorPlacement gMagmaRockInside_SalamanderTailChestPlacements[] = {
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

/// The actors placed in the ember stone's room.
struct ActorPlacement gMagmaRockInside_EmberStonePlacements[] = {
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_EMBER_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 9, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_LARGE_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_LARGE_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_LARGE_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 12, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_LARGE_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 13, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 14, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 15, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 16, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

/// The actors placed in the Blaze tablet's room.
struct ActorPlacement gMagmaRockInside_BlazeTabletPlacements[] = {
    {SPRITE_SLATE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

/// The triggers of the Lucky Medal's room.
struct Trigger gMagmaRockInside_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {STEP_TRIGGER_WALKING, CELL_LAVA_EDGE, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_LeapOverLava)},
    {STEP_TRIGGER_WALKING, CELL_LAVA_EDGE_EAST, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_LeapOverLava)},
    {TRIGGER_TABLE_END},
};

/// The triggers of the entrance.
struct Trigger gMagmaRockInside_EntranceTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_STEP, CELL_FLOOR_SWITCH, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_DrainLava)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_FIRE19, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_MeetDjinni)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_ENTRANCE_PILLAR, FLAG_ENTRANCE_PILLAR_PUSHED,
     EVENT(MagmaRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ENTRANCE_PILLAR,
     FLAG_ENTRANCE_PILLAR_PUSHED, EVENT(MagmaRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), CELL_STATUE,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), CELL_STATUE,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_STATUE, CONDITION_ALWAYS, EVENT(MagmaRockInside_FireStatue)},
    {TRIGGER_TABLE_END},
};

/// The triggers of Fury's room.
struct Trigger gMagmaRockInside_FuryTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, CELL_FLOOR_SWITCH, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_DrainLava)},
    {STEP_TRIGGER_WALKING, CELL_LAVA_EDGE, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_LeapOverLava)},
    {TRIGGER_TALK, ACTOR_FURY, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_FightFury)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FURY_PILLAR, FLAG_FURY_PILLAR_PUSHED,
     EVENT(MagmaRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FURY_PILLAR,
     FLAG_FURY_PILLAR_PUSHED, EVENT(MagmaRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), CELL_STATUE,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), CELL_STATUE,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_STATUE, CONDITION_ALWAYS, EVENT(MagmaRockInside_FireStatue)},
    {TRIGGER_TABLE_END},
};

/// The triggers of the cracked pillars' room.
struct Trigger gMagmaRockInside_CrackedPillarsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {STEP_TRIGGER_WALKING, CELL_LAVA_EDGE, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_LeapOverLava)},
    {STEP_TRIGGER_WALKING, CELL_CRACKED_PILLAR_A,
     IF_FLAG_SET(FLAG_FURY_FLOODED), EVENT(MagmaRockInside_LeapOverLava)},
    {STEP_TRIGGER_WALKING, CELL_CRACKED_PILLAR_B, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_LeapOverLava)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_LAVA_EDGE_NORTH,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_LeapOverLava)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_LAVA_EDGE_WEST,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_LeapOverLava)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PILLAR_A, FLAG_PILLAR_A_PUSHED,
     EVENT(MagmaRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_A, FLAG_PILLAR_A_PUSHED,
     EVENT(MagmaRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PILLAR_B, FLAG_PILLAR_B_PUSHED,
     EVENT(MagmaRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_B, FLAG_PILLAR_B_PUSHED,
     EVENT(MagmaRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PILLAR_C, FLAG_PILLAR_C_PUSHED,
     EVENT(MagmaRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_C, FLAG_PILLAR_C_PUSHED,
     EVENT(MagmaRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_CRACKED_PILLAR_A, FLAG_INSIDE_CRACKED_PILLAR_A_BURST,
     EVENT(MagmaRockInside_BurstCrackedPillarA)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_CRACKED_PILLAR_B, FLAG_INSIDE_CRACKED_PILLAR_B_BURST,
     EVENT(MagmaRockInside_BurstCrackedPillarB)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), CELL_STATUE,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), CELL_STATUE,
     FLAG_FIRE_BURST_PLAYING, EVENT(MagmaRockInside_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), CELL_STATUE,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_BurstCrackedPillarA)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_STATUE, CONDITION_ALWAYS, EVENT(MagmaRockInside_FireStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     CELL_LOWER_WEST_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_LOWER_WEST_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_LOWER_WEST_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_FireStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     CELL_LOWER_EAST_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_LOWER_EAST_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_LOWER_EAST_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_FireStatue)},
    {TRIGGER_TABLE_END},
};

/// The triggers of the Golem Core's chest room.
struct Trigger gMagmaRockInside_GolemCoreChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, CELL_FLOOR_SWITCH, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_DrainLava)},
    {TRIGGER_STEP, CELL_SECOND_FLOOR_SWITCH, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_DrainLava)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_HANGING_CHAIN_A,
     CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_HANGING_CHAIN_B,
     CONDITION_ALWAYS, 0},
    {TRIGGER_TABLE_END},
};

/// The triggers of the Salamander Tail's chest room.
struct Trigger gMagmaRockInside_SalamanderTailChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXIT, 19, CONDITION_ALWAYS, 19},
    {STEP_TRIGGER_WALKING, CELL_LAVA_EDGE,
     IF_FLAG_SET(FLAG_LOWER_ROOMS_FLOODED),
     EVENT(MagmaRockInside_LeapOverLava)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_NORTH_PILLAR, FLAG_NORTH_PILLAR_PUSHED,
     EVENT(MagmaRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_NORTH_PILLAR,
     FLAG_NORTH_PILLAR_PUSHED, EVENT(MagmaRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_EAST_PILLAR, FLAG_EAST_PILLAR_PUSHED,
     EVENT(MagmaRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR,
     FLAG_EAST_PILLAR_PUSHED, EVENT(MagmaRockInside_FinishPushingPillar)},
    {TRIGGER_TABLE_END},
};

/// The triggers of the ember stone's room.
struct Trigger gMagmaRockInside_EmberStoneTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_INSIDE_BLAZE_TABLET,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_OpenStairs)},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, CELL_ERUPTION_ISLET, FLAG_ROCKS_RELEASED,
     EVENT(MagmaRockInside_ReleaseRocks)},
    {TRIGGER_EXAMINE, CELL_EMBER_STONE, FLAG_MAGMA_BALL_TAKEN,
     EVENT(MagmaRockInside_ExamineEmberStone)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_LEDGE_PILLAR, FLAG_LEDGE_PILLAR_PUSHED,
     EVENT(MagmaRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEDGE_PILLAR,
     FLAG_LEDGE_PILLAR_PUSHED, EVENT(MagmaRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FALLEN_FLAME_A, CONDITION_ALWAYS, EVENT(MagmaRockInside_TakeFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FALLEN_FLAME_B, CONDITION_ALWAYS, EVENT(MagmaRockInside_TakeFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FALLEN_FLAME_C, CONDITION_ALWAYS, EVENT(MagmaRockInside_TakeFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FALLEN_FLAME_D, CONDITION_ALWAYS, EVENT(MagmaRockInside_TakeFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_WEST_TORCH_FLAME, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_EAST_TORCH_FLAME, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_LightTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
     ACTOR_EAST_TORCH_FLAME, FLAG_EAST_TORCH_LIT,
     EVENT(MagmaRockInside_GrowFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
     ACTOR_WEST_TORCH_FLAME, FLAG_WEST_TORCH_LIT,
     EVENT(MagmaRockInside_GrowFlame)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), ACTOR_FALLEN_FLAME_A, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), ACTOR_FALLEN_FLAME_B, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), ACTOR_FALLEN_FLAME_C, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), ACTOR_FALLEN_FLAME_D, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), ACTOR_EAST_TORCH_FLAME, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutFlames)},
    {TRIGGER_TABLE_END},
};

/// The cell the leader last stopped on, x in the high half and z in the low, or
/// -1 for none (MagmaRockInside_TrackStepCell).
s32 gMagmaRockInside_LastStepCell = -1;

/// The triggers of the Blaze tablet's room.
struct Trigger gMagmaRockInside_BlazeTabletTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, CELL_STEPPING_STONE_A, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StampStepCell)},
    {TRIGGER_STEP, CELL_STEPPING_STONE_B, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StampOtherStepCell)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BLAZE_TABLET, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ReadBlazeTablet)},
    {EXAMINE_TRIGGER_FACING(FACING_SOUTH), CELL_BLAZE_TABLET, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ReadBlazeTabletFromNorth)},
    {EXAMINE_TRIGGER_FACING(FACING_WEST), CELL_BLAZE_TABLET, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ReadBlazeTabletFromEast)},
    {EXAMINE_TRIGGER_FACING(FACING_EAST), CELL_BLAZE_TABLET, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ReadBlazeTabletFromWest)},
    {TRIGGER_RAISED, STEP_CELL_TRIGGER_BASE + 2, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StepOnMarker)},
    {TRIGGER_RAISED, STEP_CELL_TRIGGER_BASE + 3, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StepOnMarker)},
    {TRIGGER_RAISED, STEP_CELL_TRIGGER_BASE + 4, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StepOnMarker)},
    {TRIGGER_RAISED, STEP_CELL_TRIGGER_BASE + 5, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StepOnMarker)},
    {TRIGGER_RAISED, STEP_CELL_TRIGGER_BASE + 6, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StepOnMarker)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_SOURCE_FLAME, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_LightTabletTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_TORCH_FLAME, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
     ACTOR_TORCH_FLAME, FLAG_TABLET_TORCH_LIT,
     EVENT(MagmaRockInside_GrowTabletFlame)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), ACTOR_SOURCE_FLAME, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutTabletFlame)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), ACTOR_TORCH_FLAME, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutTabletFlame)},
    {TRIGGER_TABLE_END},
};

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY
#undef DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR
#undef DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH
#undef DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM
