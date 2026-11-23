
/// @file
/// Aqua Rock's inner rooms, inside the rock on the island south of the Apojii
/// Islands, which Felix enters from the rock's outer slopes. The file holds ten
/// rooms, most of them several screens, and their water puzzles. A grey Moai
/// that Douse fills pours water and floods its room, lifting the yellow
/// stepping blocks; a stone pillar Moved before a Moai's mouth stops it and the
/// water sinks. Vents blow Felix off their ledge until Move sets a pillar in
/// front of them. A Water Stone, the big blue drop, sends a stream along the
/// blocks when Douse is cast on it. There are puddles to Frost, a tightrope, a
/// Psynergy Stone, and in the Vial chest's room a waterfall that carries Felix
/// back up to the slopes. Setting the Aquarius Stone in the empty column of the
/// great pool's room opens a causeway and a gold door in the dais at its heart,
/// which lead to the tablet where Piers learns Parch
/// (AquaRockInside_ReadParchTablet). With Parch, Felix can dry the pool by the
/// sunken Rusty Sword chest and stop the Moai whose stream hides Steam, the
/// Mercury Djinni (AquaRockInside_FightSteam).

#include "system/task.h"
#include "types.h"
#include "macros.h"
#include "window/message_argument.h"
#include "item_ids.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "field/field_sequence.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/scene_tables.h"
#include "system/iwram_call.h"
#include "system/io_reg.h"
#include "field/field_sprite.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "scene/dehkan_plateau.h"
#include "scene/scene.h"
#include "window/item_icon.h"
#include "graphics/palette_buffers.h"

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);
void Sprite_StartAnimation(void *animation, s32 value);
u8 *Sprite_AddAnimation(void *animation, s32 id);

#include "field/map_grid.h"

/// A water pillar or spout drawn over its cell: its place, height and resting
/// height, kind, state (1 a spout, 2 a pillar on an actor), the actor's
/// position it follows, and its top's and column's sprite attributes.
struct PillarEntry {
  s32 x;
  s32 height;
  s32 z;
  s32 base;
  u16 kind;
  u16 state;
  s32 *position;
  u32 top[3];
  u32 column[3];
};

/// The most water pillars and spouts a room has.
#define PILLAR_ENTRIES 32

/// The kinds of spout, each a run of collision codes from the first a room's
/// spouts take, and of pillar top: gAquaRockInside_PillarTiles holds a 16x16
/// top for each, then the 16x8 pieces of column under them, one for each step
/// of height up to PILLAR_COLUMN_PIECES.
#define SPOUT_KINDS 8
#define PILLAR_COLUMN_PIECES 8
#define PILLAR_TOP_TILES SPRITE_TILES_8BPP(16, 16)
#define PILLAR_COLUMN_TILES SPRITE_TILES_8BPP(16, 8)

/// The depth the water pillars and the streams' splashes take past their screen
/// row, behind an actor and the scene's other sprites (SCENE_SPRITE_DEPTH) at
/// the same spot.
#define WATER_SPRITE_DEPTH 50

/// The water pillars and spouts AquaRockInside_DrawPillars draws, and their
/// VRAM entry.
struct PillarWork {
  u16 vramEntry;
  u16 count;
  struct PillarEntry entries[PILLAR_ENTRIES];
};

/// The head of one of the two streams a Water Stone sends: its place and the
/// way it runs.
struct BeamEntry {
  s32 x;
  s32 y;
  s32 z;
  u16 unusedAfterZ; ///< no code names it
  u16 direction;
};

/// A splash the streams leave on an open cell, and its sprite's attributes.
struct Splash {
  s32 x;
  s32 y;
  s32 z;
  u16 active;
  u16 direction;
  u32 sprite[3];
};

/// The most splashes the streams leave at once.
#define SPLASH_ENTRIES 32

/// The streams and their splashes, which AquaRockInside_DrawSplashes draws.
struct BeamWork {
  u16 vramEntry;
  u16 count;
  struct BeamEntry entries[2];
  struct Splash splashes[SPLASH_ENTRIES];
};

void Actor_StartScript(struct Actor *actor, const void *data);
void Map_LoadCellsAndShapes(void);
void MessageArgument_Push(s32 item, s32 mode);
void Field_TryJumpForward(void);
void Actor_SpawnWadingSplash(struct Actor *actor);
s32 Actor_EmitMist(struct Actor *actor);

void AquaRockInside_DrainStream(void);
s32 AquaRockInside_BlockCellUnder(s32 id, s32 value);
void AquaRockInside_SetPillarCellJumpableInWater(void);
void AquaRockInside_AddMoaiWater(s32 id);
void AquaRockInside_DrawPillars(void);
void AquaRockInside_SetPillarsHeight(s32 value);
void AquaRockInside_DrawSplashes(void);
void AquaRockInside_StartSplashes(void);
s32 AquaRockInside_AddSplash(s32 x, s32 y, s32 z, s32 kind);
s32 AquaRockInside_AdvanceStreams(s32 a, s32 b);
void Vent_Bounce(struct Actor *actor);
void Actor_StartScripted(struct Actor *actor, s32 priority);
void Vent_Puff(s32 *position, s32 base);
s32 Vent_SetBlowing(s32 index, s32 blowing);
s32 Vent_GetCounter(void);
s32 Vent_AdvanceCounter(s32 limit);

extern s32 gAquaRockInside_SpringLevel;
struct PillarWork gAquaRockInside_Pillars BSS_DATA;
struct BeamWork gAquaRockInside_Splashes BSS_DATA;
extern u32 gAquaRockInside_SparkScript[];
extern u32 gAquaRockInside_SplashScript[];
extern u32 gWadingSplash_Script[];
extern u32 gWaterDropScript[];
/// The water pillars' tiles, compressed (graphics/aqua_rock_inside/tiles.s).
extern const u8 gAquaRockInside_PillarTiles[];
/// The splashes' tiles, compressed.
extern const u8 gAquaRockInside_SplashTiles[];
extern u32 gTaopoSwamp_PuffRises[];

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x, s32 y,
                                                    s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

static __inline__ void Camera_MoveWithinMapBounds_(FIXED x, FIXED a, FIXED z,
                                                   s32 b) {
  FieldCamera_MoveWithinMapBounds(x, a, z, b);
}

static __inline__ void *MemAlloc_(s32 size) { return Heap_Alloc(size); }
#include "flags.h"
#include "djinn_ids.h"
#include "field/field_sequence_event.h"
#include "field/field.h"
#include "field/map.h"
#include "types.h"
#include "scene/ground_effects.h"
#include "sound/sound.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "version.h"
#include "field/actor.h"
#include "system/vector3.h"
#include "field/event_inline.h"
#include "system/io_write_queue.h"
#include "system/frame.h"
#include "system/field_effect.h"
#include "system/io_reg.h"
#include "system/ram_buffer.h"
#include "field/call.h"
#include "system/iwram_call.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "shared/tables.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_prepare.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

struct Actor;

void AquaRockInside_WaitToLand(struct Actor *actor);

void Leader_RiseFromWater(void);
void AquaRockInside_StartPillars(void);
void AquaRockInside_AddSpouts(u32 first, s32 level, s32 baseLevel);
void AquaRockInside_ShowAquariusStone(void);
void AquaRockInside_AddPillar(s32 id, s32 kind, s32 height);

void FieldEffect_StartBlankOverlay();
void Tightrope_Place();

#include "text/msg_ids.h"

/// How far the water rises or falls when a Moai fills, in pixels.
#define WATER_LEVEL_STEP 32

/// Aqua Rock's lasting flags. The Water of Life chest's room's west and east
/// stone pillars, each Moved into its gap (AquaRockInside_SetPillarInGap).
#define FLAG_AQUA_ROCK_WEST_PILLAR_SET 0x95a
#define FLAG_AQUA_ROCK_EAST_PILLAR_SET 0x95b
/// The stone pillar of Steam's room, pushed off its ledge to the floor below.
#define FLAG_AQUA_ROCK_STEAM_ROOM_PILLAR_DROPPED 0x95c
/// The Aquarius Stone set in the column of the great pool's room.
#define FLAG_AQUA_ROCK_AQUARIUS_STONE_SET 0x95d
/// Douse on the Water Stones: the streams they turn, as each room's
/// AquaRockInside_AdvanceStreams leaves them.
#define FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED 0xa64
#define FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED 0xa65
#define FLAG_AQUA_ROCK_AQUARIUS_STONE_WATER_STONE_DOUSED 0xa66
#define FLAG_AQUA_ROCK_CRYSTAL_POWDER_WATER_STONE_DOUSED 0xa67
/// The pool of the Rusty Sword's room, dried by Parch.
#define FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_POOL_PARCHED 0xa6a
/// Set once Piers has read the tablet and learned Parch.
#define FLAG_PIERS_LEARNED_PARCH 0x9ae
/// This visit's flags, room by room. Douse on the Water of Life chest's room's
/// east and west Moai: each has filled, and the water has risen.
#define FLAG_WATER_OF_LIFE_CHEST_EAST_MOAI_FILLED FLAG_SCENE_TEMP_30
#define FLAG_WATER_OF_LIFE_CHEST_WEST_MOAI_FILLED FLAG_SCENE_TEMP_31
/// Frost on the Water of Life chest's room's puddle.
#define FLAG_WATER_OF_LIFE_CHEST_PUDDLE_FROZEN FLAG_SCENE_TEMP_50
/// Parch on the Moai of Steam's room: its stream has drained away.
#define FLAG_STEAM_ROOM_STREAM_DRAINED FLAG_SCENE_TEMP_40
/// Douse on the Rusty Sword chest's room's Moai, and then on its Water Stone.
#define FLAG_RUSTY_SWORD_CHEST_MOAI_FILLED FLAG_SCENE_TEMP_49
#define FLAG_RUSTY_SWORD_CHEST_WATER_STONE_DOUSED FLAG_SCENE_TEMP_48
/// Move on the pillar before the Vial chest's room's Moai, which stops its
/// water.
#define FLAG_VIAL_CHEST_MOAI_BLOCKED FLAG_SCENE_TEMP_38
/// Frost on each of the Vial chest's room's three puddles.
#define FLAG_VIAL_CHEST_PUDDLE_A_FROZEN FLAG_SCENE_TEMP_51
#define FLAG_VIAL_CHEST_PUDDLE_B_FROZEN FLAG_SCENE_TEMP_52
#define FLAG_VIAL_CHEST_PUDDLE_C_FROZEN FLAG_SCENE_TEMP_53
/// Douse on the Tear Stone's room's Moai, and then on its Water Stone.
#define FLAG_TEAR_STONE_MOAI_FILLED FLAG_SCENE_TEMP_4B
#define FLAG_TEAR_STONE_WATER_STONE_DOUSED FLAG_SCENE_TEMP_4A
/// The Parch tablet's room's water has sunk and left its last pool, until
/// Parch dries it.
#define FLAG_PARCH_TABLET_POOL_LEFT FLAG_AREA_TEMP_49
/// The cell of the mouth of the Moai of Steam's room, cell (48, 15): Parch on
/// it, then raised to drain the stream.
#define TRIGGER_STEAM_ROOM_MOAI 88
/// The cells in front of the west and east vents of the Crystal Powder and
/// Vial chests' rooms, where the gust blows Felix off the ledge
/// (AquaRockInside_StepIntoVentGust); a stone pillar Moved in front of a vent
/// shields them.
#define TRIGGER_WEST_VENT_GUST 77
#define TRIGGER_EAST_VENT_GUST 78

/// The Vial chest's room's doorways at the north end of its upper passages,
/// where the party comes out after leaving its lower floor by the south edge
/// (exits 8 and 10).
enum {
  ENTRANCE_VIAL_CHEST_WEST_DOORWAY = 9,
  ENTRANCE_VIAL_CHEST_EAST_DOORWAY = 11
};

/// The collision codes the rooms' water reads (MapCell.collisionCode).
enum AquaRockInsideCollisionCode {
  /// The first of the eight codes of the bends that turn a stream
  /// (gAquaRockInside_StreamTurns' columns); their cells raise spouts too
  /// (AquaRockInside_AddSpouts).
  COLLISION_STREAM_BEND = 33,
  /// The first of the eight codes of a room's second set of spouts.
  COLLISION_SECOND_SPOUTS = 40,
  /// A stream's goal: AquaRockInside_AdvanceStreams counts the streams that
  /// reach it.
  COLLISION_STREAM_GOAL = 55,
  /// An open cell: a stream runs on across it and leaves a splash.
  COLLISION_STREAM_OPEN = 255
};

/// In gAquaRockInside_StreamTurns, a bend a stream cannot run through: it
/// stops there.
#define STREAM_STOP 8

/// The actors of the inner rooms the set-up places, numbering each room's
/// placement table, named by the Psynergy their triggers answer: a Moai
/// (SPRITE_MOAI) that Douse fills, so that the water rises, or a
/// Water Stone (SPRITE_WATER_DROP_LARGE, a tear-shaped rock) that Douse sets
/// spouting, which turns the streams; a stone pillar (SPRITE_STONE_PILLAR)
/// Moved in front of a vent or a Moai or into a gap, and the water pillars
/// topped by yellow blocks (SPRITE_YELLOW_BLOCK) AquaRockInside_AddPillar
/// raises.
enum AquaRockInsideWaterOfLifeChestActorId {
  ACTOR_WATER_OF_LIFE_CHEST_EAST_MOAI = ACTOR_FIRST_PLACED,
  ACTOR_WATER_OF_LIFE_CHEST_WEST_MOAI,
  ACTOR_WATER_OF_LIFE_CHEST_WEST_PILLAR,
  ACTOR_WATER_OF_LIFE_CHEST_EAST_PILLAR,
  /// The small blue rock the water lifts (AquaRockInside_SetPillarHeight).
  ACTOR_WATER_OF_LIFE_CHEST_BLUE_ROCK,
  ACTOR_WATER_OF_LIFE_CHEST_PUDDLE
};

enum AquaRockInsideSteamActorId {
  ACTOR_STEAM_ROOM_MOAI = ACTOR_FIRST_PLACED,
  /// The pillar pushed off its ledge (AquaRockInside_PushPillarOffLedge).
  ACTOR_STEAM_ROOM_PILLAR,
  /// The Mercury Djinni Steam (SPRITE_MERCURY_DJINNI).
  ACTOR_STEAM
};

enum AquaRockInsideAquariusStoneActorId {
  ACTOR_AQUARIUS_STONE_WATER_STONE = ACTOR_FIRST_PLACED,
  /// Two hidden actors where the stone appears; the second shows it
  /// (AquaRockInside_ShowAquariusStone).
  ACTOR_AQUARIUS_STONE_BASE,
  ACTOR_AQUARIUS_STONE
};

enum AquaRockInsideRustySwordChestActorId {
  ACTOR_RUSTY_SWORD_CHEST_WATER_STONE = ACTOR_FIRST_PLACED,
  ACTOR_RUSTY_SWORD_CHEST_MOAI,
  /// The two Moai of the upper ledge.
  ACTOR_RUSTY_SWORD_CHEST_MOAI_A,
  ACTOR_RUSTY_SWORD_CHEST_MOAI_B,
  ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A,
  ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_B,
  ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_C,
  /// The room's chest, the first actor its map's chests make.
  ACTOR_RUSTY_SWORD_CHEST = 64
};

enum AquaRockInsideCrystalPowderChestActorId {
  ACTOR_CRYSTAL_POWDER_CHEST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_CRYSTAL_POWDER_CHEST_WATER_STONE
};

enum AquaRockInsideVialChestActorId {
  /// The pillar Moved before the Moai (AquaRockInside_BlockMoaiWithPillar).
  ACTOR_VIAL_CHEST_MOAI_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_VIAL_CHEST_MOAI,
  /// The two pillars beside the vents (AquaRockInside_StepIntoVentGust).
  ACTOR_VIAL_CHEST_WEST_VENT_PILLAR,
  ACTOR_VIAL_CHEST_EAST_VENT_PILLAR,
  /// A hidden actor that splashes at the foot of the waterfall
  /// (AquaRockInside_SplashBeneath).
  ACTOR_VIAL_CHEST_SPLASH,
  /// The three puddles Frost freezes (AquaRockInside_FreezeRoomPuddle).
  ACTOR_VIAL_CHEST_PUDDLE_A,
  ACTOR_VIAL_CHEST_PUDDLE_B,
  ACTOR_VIAL_CHEST_PUDDLE_C
};

enum AquaRockInsideTightropeActorId {
  ACTOR_TIGHTROPE_WEST_POST = ACTOR_FIRST_PLACED,
  ACTOR_TIGHTROPE_EAST_POST
};

enum AquaRockInsideTearStoneActorId {
  ACTOR_TEAR_STONE_WATER_STONE = ACTOR_FIRST_PLACED,
  ACTOR_TEAR_STONE_MOAI,
  /// The nine water pillars of the puzzle, lettered A to I.
  ACTOR_TEAR_STONE_WATER_PILLAR_A,
  ACTOR_TEAR_STONE_WATER_PILLAR_B,
  ACTOR_TEAR_STONE_WATER_PILLAR_C,
  ACTOR_TEAR_STONE_WATER_PILLAR_D,
  ACTOR_TEAR_STONE_WATER_PILLAR_E,
  ACTOR_TEAR_STONE_WATER_PILLAR_F,
  ACTOR_TEAR_STONE_WATER_PILLAR_G,
  ACTOR_TEAR_STONE_WATER_PILLAR_H,
  ACTOR_TEAR_STONE_WATER_PILLAR_I,
  /// The stone pillars Moved into the puzzle (AquaRockInside_MovePuzzlePillar).
  ACTOR_TEAR_STONE_PILLAR_A,
  ACTOR_TEAR_STONE_PILLAR_B
};

/// The Parch tablet's room (gAquaRockInside_ParchTabletPlacements).
enum AquaRockInsideParchTabletActorId {
  /// The six water pillars.
  ACTOR_PARCH_TABLET_FIRST_WATER_PILLAR = ACTOR_FIRST_PLACED,
  /// The tablet Piers reads to learn Parch.
  ACTOR_PARCH_TABLET = ACTOR_PARCH_TABLET_FIRST_WATER_PILLAR + 6
};

/// Talking to Steam, the Mercury Djinni standing at the head of the stream
/// before the Moai of Steam's room, starts a battle with it. Felix reaches it
/// once Parch has dried the stream (AquaRockInside_SteamRoomStreamDrains).
void AquaRockInside_FightSteam(void) {
  Field_RequestDjinnBattle(ACTOR_STEAM, DJINNI_STEAM);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gAquaRockInside_Placements[];
extern struct ActorPlacement gAquaRockInside_WaterOfLifeChestPlacements[];
extern struct ActorPlacement gAquaRockInside_SteamPlacements[];
extern struct ActorPlacement gAquaRockInside_AquariusStonePlacements[];
extern struct ActorPlacement gAquaRockInside_RustySwordChestPlacements[];
extern struct ActorPlacement gAquaRockInside_CrystalPowderChestPlacements[];
extern struct ActorPlacement gAquaRockInside_VialChestPlacements[];
extern struct ActorPlacement gAquaRockInside_PsynergyStonePlacements[];
extern struct ActorPlacement gAquaRockInside_TightropePlacements[];
extern struct ActorPlacement gAquaRockInside_TearStonePlacements[];
extern struct ActorPlacement gAquaRockInside_ParchTabletPlacements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST)
    return gAquaRockInside_WaterOfLifeChestPlacements;
  if (scene == SCENE_AQUA_ROCK_INSIDE_STEAM)
    return gAquaRockInside_SteamPlacements;
  if (scene == SCENE_AQUA_ROCK_INSIDE_AQUARIUS_STONE)
    return gAquaRockInside_AquariusStonePlacements;
  if (scene == SCENE_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST)
    return gAquaRockInside_RustySwordChestPlacements;
  if (scene == SCENE_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST)
    return gAquaRockInside_CrystalPowderChestPlacements;
  if (scene == SCENE_AQUA_ROCK_INSIDE_VIAL_CHEST)
    return gAquaRockInside_VialChestPlacements;
  if (scene == SCENE_AQUA_ROCK_INSIDE_PSYNERGY_STONE)
    return gAquaRockInside_PsynergyStonePlacements;
  if (scene == SCENE_AQUA_ROCK_INSIDE_TIGHTROPE)
    return gAquaRockInside_TightropePlacements;
  if (scene == SCENE_AQUA_ROCK_INSIDE_TEAR_STONE)
    return gAquaRockInside_TearStonePlacements;
  if (scene == SCENE_AQUA_ROCK_INSIDE_PARCH_TABLET)
    return gAquaRockInside_ParchTabletPlacements;
  return gAquaRockInside_Placements;
}

/// Moving or pushing one of the two stone pillars of the Water of Life chest's
/// room into its gap: the west pillar north along the dotted track to the round
/// hole at its end (cell (9, 12)), the east one two squares east into its gap
/// among the yellow stepping blocks (column 51). There it settles a pixel lower
/// and stays for good, making the shortcut the walkthrough speaks of; with the
/// west one in place, its square and cell (13, 12) become places Felix hops
/// across (MAP_CELL_JUMP).
void AquaRockInside_SetPillarInGap(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);

  if (id == ACTOR_WATER_OF_LIFE_CHEST_WEST_PILLAR) {
    s32 row = FIELD_CELL(actor->z.fixed);

    if (row == 12) {
      actor->motionFlags = 0;
      {
        s32 step = -toFIXED(1);

        actor->y.fixed += step;
      }
      Event_SetFlag(FLAG_AQUA_ROCK_WEST_PILLAR_SET);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 9, row);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 13, row);
    }
  } else if (id == ACTOR_WATER_OF_LIFE_CHEST_EAST_PILLAR) {
    if (FIELD_CELL(actor->x.fixed) == 51) {
      actor->motionFlags = 0;
      {
        FIXED step = -toFIXED(1);

        actor->y.fixed += step;
      }
      Event_SetFlag(FLAG_AQUA_ROCK_EAST_PILLAR_SET);
    }
  }
}

/// Walking forward at the edge of a gap in the water hops Felix across onto
/// the yellow block beyond: at the foot of the vents' ledge in the Crystal
/// Powder chest's room (cells (17, 24) and (19, 24)), at the east stone
/// pillar's squares in the Water of Life chest's room (cells (49, 13) and
/// (50, 13)) and at two spots of the Rusty Sword chest's room (cells (12, 44)
/// and (20, 44)). The field's Field_TryJumpForward finds the block to land on.
void AquaRockInside_JumpForward(void) { Field_TryJumpForward(); }

/// Douse on the grey east Moai of the Water of Life chest's room: it turns
/// green and pours water from its mouth, the room floods, and the yellow
/// stepping blocks and the small blue rock rise with the water.
void AquaRockInside_DouseWaterOfLifeChestEastMoai(s32 unused, s32 id) {
  struct MapLayer *layer =
      &((struct MapWork *)Ram_HeapSlots->mapGrid)->layers[1];
  s32 i;
  s32 column;
  s32 objectId = id;

  FieldActor_SetSpriteColor(Actor_Get(objectId), SPRITE_COLOR_NORMAL);
  AquaRockInside_AddMoaiWater(objectId);
  Actor_SetAnimation(objectId, 3);
  layer->offsetY -= toFIXED(WATER_LEVEL_STEP);
  Map_LoadCellsAndShapes();
  MapGrid_CenterOnFollowedPoint();
  column =
      FIELD_CELL(Actor_Get(ACTOR_WATER_OF_LIFE_CHEST_BLUE_ROCK)->x.fixed) - 42;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_RISE_WITH_SPARKS_HIGH);
  // AquaRockInside_SetPillarHeight, defined below, is called with no prototype
  // in scope.
  for (i = 0; i <= WATER_LEVEL_STEP - 1; i++) {
    AquaRockInside_SetPillarsHeight(INTtoFIXED(i));
    AquaRockInside_SetPillarHeight(column, 0);
    layer->offsetY += toFIXED(1);
    System_Wait(4);
  }
  if (Event_GetFlag(FLAG_WATER_OF_LIFE_CHEST_PUDDLE_FROZEN))
    MapGrid_CopyCellAttributes(41, 42, 1, 1, 43, 44);
  Event_End();
  Event_SetFlag(FLAG_WATER_OF_LIFE_CHEST_EAST_MOAI_FILLED);
}

/// Douse on the grey west Moai of the Water of Life chest's room: it pours
/// water from its mouth, the room floods and the yellow stepping blocks rise.
void AquaRockInside_DouseWaterOfLifeChestWestMoai(s32 unused, s32 id) {
  struct MapLayer *layer =
      &((struct MapWork *)Ram_HeapSlots->mapGrid)->layers[1];
  s32 i;

  FieldActor_SetSpriteColor(Actor_Get(id), SPRITE_COLOR_NORMAL);
  AquaRockInside_AddMoaiWater(id);
  Actor_SetAnimation(id, 3);
  layer->offsetY -= toFIXED(WATER_LEVEL_STEP);
  Map_LoadCellsAndShapes();
  MapGrid_CenterOnFollowedPoint();
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_RISE_WITH_SPARKS_HIGH);
  for (i = 0; i <= WATER_LEVEL_STEP - 1; i++) {
    AquaRockInside_SetPillarsHeight(INTtoFIXED(i));
    layer->offsetY += toFIXED(1);
    System_Wait(4);
  }
  Event_End();
  Event_SetFlag(FLAG_WATER_OF_LIFE_CHEST_WEST_MOAI_FILLED);
}

/// Nothing more happens when Felix uses Move on the small blue rock of the
/// Water of Life chest's room: Move slides it as it slides a pillar, and the
/// trigger's routine adds nothing.
void AquaRockInside_DoNothing(void) {}

/// Frost on the puddle of the Water of Life chest's room, at cell (43, 44),
/// freezes it, and Felix can hop across it (MAP_CELL_JUMP): the walkthrough's
/// first way through the room's puzzle.
void AquaRockInside_FreezePuddle(void) {
  MapGrid_CopyCellAttributes(41, 42, 1, 1, 43, 44);
  Event_SetFlag(FLAG_WATER_OF_LIFE_CHEST_PUDDLE_FROZEN);
}

#if TLA_PATCH_1
/// From patch 1: lets the actor fall and waits until it lands with a thump.
void AquaRockInside_WaitToLand(struct Actor *actor) {
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  do {
    System_Wait(1);
  } while (actor->velocityY != 0);
  Event_PlaySound(SE_THUMP);
  System_Wait(10);
  actor->motionFlags = 0;
}
#endif

/// Moving or pushing the stone pillar of Steam's room east off the edge of its
/// ledge (column 39) drops it to the floor below, onto the round mark there;
/// in patch 1 it lands with a thump. Its square becomes one Felix hops across
/// (MAP_CELL_JUMP).
void AquaRockInside_PushPillarOffLedge(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);
  s32 x;

  if (id == ACTOR_STEAM_ROOM_PILLAR) {
    x = FIELD_CELL(actor->x.fixed);
    if (x == 39) {
#if !TLA_PATCH_1
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      System_Wait(15);
      actor->motionFlags = 0;
      Event_SetFlag(FLAG_AQUA_ROCK_STEAM_ROOM_PILLAR_DROPPED);
      MapGrid_CopyCellAttributes(41, 9, 1, 1, x, id);
#else
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      AquaRockInside_WaitToLand(actor);
      Event_SetFlag(FLAG_AQUA_ROCK_STEAM_ROOM_PILLAR_DROPPED);
      MapGrid_CopyCellAttributes(41, 9, 1, 1, x, id);
      Event_End();
#endif
    }
  }
}

/// Parch on the Moai of Steam's room, at its mouth (cell (48, 15)): the Moai
/// turns grey and stops pouring, and the stream it fed starts to drain
/// (AquaRockInside_SteamRoomStreamDrains, which raising the trigger runs).
void AquaRockInside_ParchSteamRoomMoai(void) {
  Actor_SetAnimation(ACTOR_STEAM_ROOM_MOAI, 4);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_STEAM_ROOM_MOAI),
                            SPRITE_COLOR_GREY);
  MapGrid_CopyCells(123, 15, 5, 3, 110, 15);
  gFieldWork->raisedTrigger = TRIGGER_STEAM_ROOM_MOAI;
}

/// Each frame while a stream drains: its level falls a step and the water's
/// pictures follow it down from the Moai, until the bed is dry.
void AquaRockInside_DrainStream(void) {
  s32 count = gAquaRockInside_SpringLevel - 1;
  s32 row;

  gAquaRockInside_SpringLevel = count;
  row = count / 8;
  MapGrid_CopyCells((count & 1) * 5 + 118, 47 - row, 5, 2, 110, 47 - row);
  if (gAquaRockInside_SpringLevel == 0)
    FieldTask_Remove(AquaRockInside_DrainStream);
}

/// After Parch on its Moai, the stream of Steam's room drains away: Felix
/// faces south and the camera follows the water down the stream's bed to its
/// far end, then the bed (cells (46, 16) to (50, 47)) becomes dry ground.
void AquaRockInside_SteamRoomStreamDrains(void) {
  s32 priority;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Event_PlaySound(SE_RISE_WITH_SPARKS_LOW);
  priority = TASK_PASS_FRAME;
  FieldTask_AddFrame(AquaRockInside_DrainStream, priority);
  FieldCamera_SetSpeedAndAcceleration(2.5, 0.25);
  FieldCamera_MoveTo(776, CAMERA_KEEP, 648, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(30);
  Event_End();
  Event_SetFlag(FLAG_STEAM_ROOM_STREAM_DRAINED);
  MapGrid_CopyCellAttributes(59, 16, 5, 32, 46, 16);
}

/// The Aquarius Stone shown set in the column.
void AquaRockInside_ShowAquariusStone(void) {
  struct Actor *object;
  struct Sprite *sprite;

  FieldActor_ShowItemIconById(ACTOR_AQUARIUS_STONE, ITEM_AQUARIUS_STONE);
  // UB: this room has no actor 20 (it places actors 8 to 10 and treasure
  // spots 64 and 65), so Actor_Get returns NULL and the stores below go
  // through it.
  object = Actor_Get(20);
#ifdef UBFIX
  if (object == NULL)
    return;
#endif
  sprite = object->sprite;
  sprite->retainCacheEntry = 1;
  object->motionFlags = 0;
  object->y.fixed = toFIXED(24);
}

/// The great pool's pictures as it drains, from full to dry.
#define SPRING_LEVELS 16

/// Where on the map each of the pool's pictures is taken from: a column and a
/// row for each.
extern u8 gAquaRockInside_SpringLevelCells[SPRING_LEVELS * 2];

/// Using the Aquarius Stone on the empty column at the south of the great
/// pool's room sets it there. The music fades, the ground shakes harder and
/// harder, and as the camera pans north to the dais in the middle of the pool
/// the water falls away to leave a causeway and a gold door in the dais; a
/// jingle plays once the shaking stops.
void AquaRockInside_UseAquariusStone(void) {
  s32 priority;
  s32 i;
  s32 j;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  AquaRockInside_ShowAquariusStone();
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  FieldEvent_Wait(60);
  Event_PlaySound(SE_WAVE);
  MapGrid_SetShake(1, 1, 1);
  FieldEvent_Wait(20);
  MapGrid_SetShake(1, 2, 1);
  FieldEvent_Wait(20);
  MapGrid_SetShake(1, 3, 1);
  FieldEvent_Wait(40);
  // AquaRockInside_DrainStream runs here too, as in Steam's room, rewriting
  // cells (110, 16) to (114, 47); in this room's map those cells lie far east
  // of the room (x 1760 on, the room ending at x 704), so nothing the player
  // sees is them.
  priority = TASK_PASS_FRAME;
  FieldTask_AddFrame(AquaRockInside_DrainStream, priority);
  FieldCamera_SetSpeedAndAcceleration(2.5, 0.25);
  FieldCamera_MoveTo(312, CAMERA_KEEP, 504, 1);
  Event_PlaySound(SE_SCATTER);
  MapGrid_CopyCellsLayered(75, 96, 7, 13, 80, 27);
  MapGrid_CopyCellsLayered(91, 96, 7, 10, 16, 31);
  MapGrid_CopyCellsLayered(99, 96, 7, 14, 16, 91);
  MapGrid_CopyCellAttributes(43, 0, 5, 9, 17, 31);
  for (i = 0; i < SPRING_LEVELS - 1; i++) {
    for (j = 0; j <= 6; j++) {
      {
        s32 index = (i + (1 & ~j)) * 2;

        MapGrid_CopyCellsLayered(gAquaRockInside_SpringLevelCells[index],
                                 gAquaRockInside_SpringLevelCells[index + 1], 7,
                                 13, 80, 27);
        System_Wait(1);
      }
    }
  }
  FieldCamera_WaitForArrival();
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  FieldEvent_Wait(30);
  Event_PlaySound(MUS_PUZZLE_SOLVED);
  Sound_WaitForBgmRestore();
  Event_End();
  Scene_PlayMusic();
  Event_SetFlag(FLAG_AQUA_ROCK_AQUARIUS_STONE_SET);
  Event_SetFlag(FLAG_ITEM_USED_UP);
}

/// Walking into the gold door in the dais of the great pool's room, once the
/// Aquarius Stone is set (cell (19, 31)): it opens with a thump and Felix walks
/// through, by `exit`, to the Parch tablet's room.
void AquaRockInside_OpenDoorToTablet(s32 exit) {
  s32 width = 1;
  s32 height = 2;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_THUMP);
  MapGrid_CopyCellsTo(107, 96, 19, 29, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(108, 96, 19, 29, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(107, 98, 19, 29, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(108, 98, 19, 29, width, height);
  FieldEvent_Wait(15);
  Actor_Get(gMainRoutineWork.leaderActorId)->motionFlags = 0;
  Actor_Get(gMainRoutineWork.leaderActorId)->speedLimit =
      Actor_Get(gMainRoutineWork.leaderActorId)->acceleration = toFIXED(1) / 2;
  Event_PlaySound(SE_MAP_EXIT);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, 312, 496);
  FieldEvent_Wait(5);
  FieldEvent_SetExitRequest(exit);
}

/// Checking the empty column at the south of the great pool's room: "It's a
/// column with a spot where you can place something.", and the field menu
/// opens if the party holds the Aquarius Stone. Once the stone is set, the
/// message is "You placed the Aquarius Stone."
void AquaRockInside_CheckColumn(void) {
  s32 seen;
  const u8 *message;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  seen = Event_GetFlag(FLAG_AQUA_ROCK_AQUARIUS_STONE_SET);
  message = MsgCheckedTheColumn;
  if (!seen) {
    Message_ShowCentered(message, 1);
    Message_ShowCentered(message + 1, 1);
    if (FieldPartyInventory_FindMember(ITEM_AQUARIUS_STONE) != -1)
      gFieldWork->fieldMenu = 1;
  } else {
    Message_ShowCentered(message, 1);
    MessageArgument_Push(ITEM_AQUARIUS_STONE, MESSAGE_ARGUMENT_ITEM);
    Message_ShowCentered(message + 2, 1);
  }
  Event_End();
}

/// Douse on the Water Stone on the dais of the great pool's room: two streams
/// of sparkles run east and west from it along the yellow blocks, six squares.
void AquaRockInside_DouseAquariusStoneWaterStone(void) {
  s32 i;

  AquaRockInside_StartSplashes();
  AquaRockInside_AddSplash(16, 4, 16, 2);
  AquaRockInside_AddSplash(22, 4, 16, 0);
  for (i = 0; i < 6; i++) {
    AquaRockInside_AdvanceStreams(COLLISION_STREAM_BEND, COLLISION_STREAM_GOAL);
    System_Wait(15);
  }
  Event_SetFlag(FLAG_AQUA_ROCK_AQUARIUS_STONE_WATER_STONE_DOUSED);
}

/// Parch on the pool around the sunken Rusty Sword chest dries it for good,
/// leaving the chest within reach.
void AquaRockInside_ParchPool(void) {
  MapGrid_CopyCellsLayered(106, 14, 14, 13, 81, 14);
  MapGrid_CopyCellsLayered(44, 78, 14, 12, 17, 78);
  MapGrid_CopyCellAttributes(81, 14, 14, 13, 17, 14);
  Event_SetFlag(FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_POOL_PARCHED);
}

/// Gives the cell under actor `id` the collision code `value`, so a stream
/// turns there.
s32 AquaRockInside_BlockCellUnder(s32 id, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor = Actor_Get(id);

  if (actor != 0) {
    struct MapCell *cells = map->layers[0].cells;
    struct MapCell *cell = &cells[FIELD_CELL(actor->x.fixed) +
                                  FIELD_CELL(actor->z.fixed) * MAPGRID_SIZE];

    cell->collisionCode = value;
    cell->attributeB |= MAP_CELL_JUMP;
  }
}

/// Nothing visible happens when the Moai of the Rusty Sword chest's room's
/// puzzle has filled: the square under the water pillar Felix can Move takes
/// MAP_CELL_JUMP and MAP_CELL_WATER, a square to hop across in the water.
void AquaRockInside_SetPillarCellJumpableInWater(void) {
  struct Actor *actor = Actor_Get(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A);

  MapGrid_CopyCellAttributes(64, 43, 1, 1, FIELD_CELL(actor->x.fixed),
                             FIELD_CELL(actor->z.fixed));
}

/// Nothing visible happens when the water pillar Felix can Move in the Rusty
/// Sword chest's room's puzzle comes to rest: its new square becomes one he
/// hops across (MAP_CELL_JUMP).
void AquaRockInside_SetPillarCellJumpable(void) {
  struct Actor *actor = Actor_Get(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A);

  MapGrid_CopyCellAttributes(64, 42, 1, 1, FIELD_CELL(actor->x.fixed),
                             FIELD_CELL(actor->z.fixed));
}

/// Nothing visible happens as Felix starts to Move or push the water pillar of
/// the Rusty Sword chest's room's puzzle: the square it leaves takes
/// MAP_CELL_PUSH, as the ground a pushed thing slides over does.
void AquaRockInside_SetPillarCellPushable(void) {
  struct Actor *actor = Actor_Get(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A);

  MapGrid_CopyCellAttributes(64, 41, 1, 1, FIELD_CELL(actor->x.fixed),
                             FIELD_CELL(actor->z.fixed));
}

/// Douse on the Water Stone of the Rusty Sword chest's room's puzzle, once its
/// Moai has flooded the room: the stream runs along the water pillars, turned
/// by the one Felix has Moved, and if it reaches its goal it stays turned for
/// good. Before the Moai fills, nothing happens.
void AquaRockInside_DouseRustySwordChestWaterStone(void) {
  s32 i;
  s32 result;

  if (Event_GetFlag(FLAG_RUSTY_SWORD_CHEST_MOAI_FILLED)) {
    AquaRockInside_BlockCellUnder(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A,
                                  COLLISION_STREAM_BEND + 5);
    AquaRockInside_StartSplashes();
    AquaRockInside_AddSplash(12, 2, 45, 1);
    for (i = 0; i < 4; i++)
      result = AquaRockInside_AdvanceStreams(COLLISION_STREAM_BEND,
                                             COLLISION_STREAM_GOAL);
    Event_SetFlag(FLAG_RUSTY_SWORD_CHEST_WATER_STONE_DOUSED);
    if (result == 1)
      Event_SetFlag(FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED);
  }
}

/// Douse on the grey Moai of the Rusty Sword chest's room's puzzle: it turns
/// blue and pours water from its mouth, and the room floods around the water
/// pillars, so that the Water Stone's stream can run.
void AquaRockInside_DouseRustySwordChestMoai(s32 unused, s32 id) {
  struct MapLayer *layer =
      &((struct MapWork *)Ram_HeapSlots->mapGrid)->layers[1];
  s32 i;

  FieldActor_SetSpriteColor(Actor_Get(ACTOR_RUSTY_SWORD_CHEST_MOAI),
                            SPRITE_COLOR_BRG);
  AquaRockInside_AddMoaiWater(id);
  Actor_SetAnimation(id, 3);
  layer->offsetY -= toFIXED(WATER_LEVEL_STEP);
  Map_LoadCellsAndShapes();
  MapGrid_CenterOnFollowedPoint();
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_RISE_WITH_SPARKS_LOW);
  for (i = 0; i < WATER_LEVEL_STEP; i++) {
    layer->offsetY += toFIXED(1);
    System_Wait(4);
  }
  Event_End();
  AquaRockInside_SetPillarCellJumpableInWater();
  Event_SetFlag(FLAG_RUSTY_SWORD_CHEST_MOAI_FILLED);
}

/// Felix jumps down the ledge he faces.
void AquaRockInside_JumpDownLedge() {
  Actor_JumpDownLedge(gMainRoutineWork.leaderActorId, 6, 0);
  Event_End();
}

/// Stepping in front of a vent in the Vial or Crystal Powder chest's room
/// (cells (9, 34) and (13, 34) in the first, (17, 22) and (19, 22) in the
/// second, with the squares south of them): its gust blows Felix south,
/// spinning, to the edge of the ledge and over it. A stone pillar Moved in
/// front of the vent shields him, and nothing happens.
void AquaRockInside_StepIntoVentGust(s32 trigger) {
  s32 clear = 1;
  struct Actor *actor;
  struct Vector3 target;
  s32 result;

  if (gMainRoutineWork.next == SCENE_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST) {
    s32 column =
        FIELD_CELL(Actor_Get(ACTOR_CRYSTAL_POWDER_CHEST_PILLAR)->x.fixed);

    if (trigger == TRIGGER_WEST_VENT_GUST && column == 17)
      clear = 0;
    if (trigger == TRIGGER_EAST_VENT_GUST && column == 19)
      clear = 0;
  }
  if (gMainRoutineWork.next == SCENE_AQUA_ROCK_INSIDE_VIAL_CHEST) {
    if (trigger == TRIGGER_WEST_VENT_GUST &&
        FIELD_CELL(Actor_Get(ACTOR_VIAL_CHEST_WEST_VENT_PILLAR)->x.fixed) == 9)
      clear = 0;
    if (trigger == TRIGGER_EAST_VENT_GUST &&
        FIELD_CELL(Actor_Get(ACTOR_VIAL_CHEST_EAST_VENT_PILLAR)->x.fixed) == 13)
      clear = 0;
  }
  if (clear == 0)
    return;
  actor = Actor_Get(gMainRoutineWork.leaderActorId);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  for (;;) {
    target.x = actor->x.fixed;
    target.y = actor->y.fixed;
    target.z = actor->z.fixed + toFIXED(8);
    result = MapGrid_CheckCollision(actor, &target);
    if (result > 0)
      break;
    if (result < 0) {
      actor->facing = FACING_SOUTH;
      AquaRockInside_JumpDownLedge(trigger);
      return;
    }
    {
      s32 offset;
      FIXED x;

      actor->z.fixed += toFIXED(2);
      x = actor->x.fixed;
      offset = (x & (toFIXED(16) - 1)) - toFIXED(8);
      {
        s32 limit = toFIXED(1) + 1;

        if (offset >= limit)
          offset = toFIXED(1);
        limit = -(toFIXED(1) + 1);
        if (offset <= limit)
          offset = -toFIXED(1);
      }
      actor->x.fixed = x - offset;
    }
    actor->facing += TRIG_TURN / 16;
    System_Wait(1);
  }
}

/// Douse on the Water Stone of the Crystal Powder chest's room: a stream of
/// sparkles runs west from it along the yellow blocks, five squares.
void AquaRockInside_DouseCrystalPowderChestWaterStone(void) {
  s32 i;

  AquaRockInside_StartSplashes();
  AquaRockInside_AddSplash(44, 0, 34, 2);
  for (i = 0; i < 5; i++) {
    AquaRockInside_AdvanceStreams(COLLISION_STREAM_BEND, COLLISION_STREAM_GOAL);
    System_Wait(15);
  }
  Event_SetFlag(FLAG_AQUA_ROCK_CRYSTAL_POWDER_WATER_STONE_DOUSED);
}

/// Stepping into the foot of the waterfall in the Vial chest's room, at the
/// north end of its pool (cell (10, 10)): Felix tumbles, turns blue with his
/// arms raised and is carried up out of sight, and comes out by `value` in the
/// Mist Sabre chest's room on Aqua Rock's slopes.
void AquaRockInside_RideFalls(s32 value) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  FIXED speed;
  s32 i;
  u8 *flags;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  flags = &actor->motionFlags;
  speed = 0;
  *flags = 0;
  actor->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_SelectAnimation(actor, ANIM_LEADER_TUMBLE_IN_WATER);
  Event_PlaySound(SE_SPLASH_SMALL);
  System_Wait(13);
  Actor_SetSpriteColorMode(actor, SPRITE_COLOR_BLUE_TINT);
  Actor_SelectAnimation(actor, ANIM_LEADER_ARMS_RAISED);
  Event_PlaySound(SE_SPIRAL);
  for (i = 0; i <= 59; i++) {
    if (i == 30)
      Event_FinishScreenTransition();
    speed += toFIXED(1) / 4;
    actor->y.fixed += speed;
    // XXX (no evidence): the splashes are made at the third puddle's place,
    // away from the waterfall; whether the waterfall's own splash actor
    // (ACTOR_VIAL_CHEST_SPLASH) was meant, nothing in reach settles.
    if ((i & 7) == 0)
      Actor_SpawnWadingSplash(Actor_Get(ACTOR_VIAL_CHEST_PUDDLE_C));
    System_Wait(1);
  }
  Event_End();
  FieldEvent_SetExitRequest(value);
}

/// Moving the stone pillar of the Vial chest's room in front of the green
/// Moai's mouth (column 49) stops its water: the pool below sinks a level and
/// three puddles are left on its floor for Frost.
void AquaRockInside_BlockMoaiWithPillar(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);

  if (FIELD_CELL(actor->x.fixed) == 49) {
    struct MapLayer *layer =
        &((struct MapWork *)Ram_HeapSlots->mapGrid)->layers[1];
    s32 i;
    s32 fade;

    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
    Event_PlaySound(SE_RISE_WITH_SPARKS_LOW);
    Actor_SetAnimation(ACTOR_VIAL_CHEST_MOAI, 2);
    MapGrid_CopyCellsLayered(114, 39, 1, 2, 113, 39);
    for (i = 0; i < WATER_LEVEL_STEP; i++) {
      layer->offsetY -= toFIXED(1);
      System_Wait(4);
    }
    for (i = ACTOR_VIAL_CHEST_PUDDLE_A; i <= ACTOR_VIAL_CHEST_PUDDLE_C; i++) {
      actor = Actor_GetById(i);
      actor->x.fixed = INTtoFIXED(actor->timer);
      actor->z.fixed = toFIXED(actor->userValue);
      actor->motionFlags = 0;
      actor->groundHeight = -toFIXED(WATER_LEVEL_STEP);
      actor->y.fixed = -toFIXED(WATER_LEVEL_STEP);
      FieldActor_SetSpritePriority(i, 3);
    }
    BlendScript_Suspend();
    fade = 12;
    do {
      IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                          BLEND_ALPHA(fade, BLEND_WEIGHT_MAX),
                          IO_WRITE_HALFWORD);
      System_Wait(4);
      fade--;
    } while (fade >= 0);
    MapGrid_CopyCellAttributes(105, 41, 12, 13, 41, 41);
    MapGrid_CopyCellsLayered(105, 105, 12, 13, 105, 41);
    BlendScript_Resume();
    Event_End();
    Event_SetFlag(FLAG_VIAL_CHEST_MOAI_BLOCKED);
  }
}

/// Frost on one of the three puddles left in the Vial chest's room's sunken
/// pool freezes it, and Felix can hop across it (MAP_CELL_JUMP).
void AquaRockInside_FreezeRoomPuddle(s32 unused, s32 id) {
  if ((u32)(id - ACTOR_VIAL_CHEST_PUDDLE_A) <= 2) {
    struct Actor *actor = Actor_Get(id);

    MapGrid_CopyCellAttributes(47, 45, 1, 1, FIELD_CELL(actor->x.fixed),
                               FIELD_CELL(actor->z.fixed));
    Event_SetFlag(FLAG_VIAL_CHEST_PUDDLE_A_FROZEN - ACTOR_VIAL_CHEST_PUDDLE_A +
                  id);
    actor->priorityFlags = 0;
  }
}

/// Nothing visible happens as Felix starts to Move or push one of the stone
/// pillars beside the vents of the Vial chest's room: the ground along their
/// row (row 34) takes MAP_CELL_PUSH, keeping the vents' gusts, so the pillar
/// can slide in front of a vent.
void AquaRockInside_FreeVentPillars(void) {
  MapGrid_CopyCellAttributes(8, 63, 7, 1, 8, 34);
}

/// When either stone pillar beside the vents of the Vial chest's room comes to
/// rest, nothing visible happens: both stop, and the square under each loses
/// the gust and becomes one Felix hops across (MAP_CELL_JUMP).
void AquaRockInside_SettleVentPillars(s32 unused, s32 id) {
  if ((u32)(id - ACTOR_VIAL_CHEST_WEST_VENT_PILLAR) <= 1) {
    struct Actor *actor = Actor_Get(ACTOR_VIAL_CHEST_WEST_VENT_PILLAR);

    actor->motionFlags = 0;
    {
      s32 row = 33;

      MapGrid_CopyCellAttributes(8, row, 1, 1, FIELD_CELL(actor->x.fixed), 34);
    }
    actor = Actor_Get(ACTOR_VIAL_CHEST_EAST_VENT_PILLAR);
    actor->motionFlags = 0;
    {
      s32 row = 33;

      MapGrid_CopyCellAttributes(8, row, 1, 1, FIELD_CELL(actor->x.fixed), 34);
    }
  }
}

/// Waits, for 60 frames at most, until the actor stops moving up or down.
void AquaRockInside_WaitToRise(struct Actor *actor) {
  s32 frames = 60;

  while (frames != 0) {
    System_Wait(1);
    frames--;
    if (actor->velocityY == 0)
      break;
  }
}

/// Checking the big Psynergy Stone in the middle of its cave (cell (12, 13)):
/// "Everyone's PP was replenished!"
void AquaRockInside_CheckPsynergyStone(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered(MsgCheckedThePsynergy, 1);
  Event_PlaySound(SE_RECOVERY);
  Party_AddPp(PARTY_PP_REPLENISH, 0);
  FieldEvent_Wait(10);
  Event_ClearFlag(FLAG_NOTHING_HAPPENS);
  Message_ShowCentered(MsgCheckedThePsynergy + 1, 1);
  Event_End();
}

/// Move on the west of the two stone pillars at the south of the Tear Stone's
/// room (row 39): the ground follows it, the two pillars' starting squares
/// going back to floor and its new square taking the pillar's.
void AquaRockInside_MovePuzzlePillar(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);

  MapGrid_CopyCellAttributes(17, 39, 2, 1, 20, 39);
  MapGrid_CopyCellAttributes(23, 39, 1, 1, FIELD_CELL(actor->x.fixed), 39);
}

/// Douse on the Water Stone of the Tear Stone's room's puzzle, once its Moai
/// has flooded the room: the stream runs along the water pillars, turned by
/// the three Felix can Move, and if it reaches its goal it stays turned for
/// good. Before the Moai fills, nothing happens.
void AquaRockInside_DouseTearStoneWaterStone(void) {
  s32 i;
  s32 result;

  if (Event_GetFlag(FLAG_TEAR_STONE_MOAI_FILLED)) {
    Call2((void (*)())AquaRockInside_BlockCellUnder,
          ACTOR_TEAR_STONE_WATER_PILLAR_C, COLLISION_STREAM_BEND + 5);
    Call2((void (*)())AquaRockInside_BlockCellUnder,
          ACTOR_TEAR_STONE_WATER_PILLAR_D, COLLISION_STREAM_BEND + 6);
    AquaRockInside_BlockCellUnder(ACTOR_TEAR_STONE_WATER_PILLAR_F,
                                  COLLISION_STREAM_BEND + 4);
    AquaRockInside_StartSplashes();
    AquaRockInside_AddSplash(38, 2, 19, 1);
    for (i = 0; i < 9; i++)
      result = AquaRockInside_AdvanceStreams(COLLISION_STREAM_BEND,
                                             COLLISION_STREAM_GOAL);
    Event_SetFlag(FLAG_TEAR_STONE_WATER_STONE_DOUSED);
    if (result == 1)
      Event_SetFlag(FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED);
  }
}

/// Douse on the grey Moai of the Tear Stone's room's puzzle: it turns blue and
/// pours water from its mouth, and the room floods around the water pillars.
void AquaRockInside_DouseTearStoneMoai(s32 unused, s32 id) {
  struct MapLayer *layer =
      &((struct MapWork *)Ram_HeapSlots->mapGrid)->layers[1];
  s32 i;

  FieldActor_SetSpriteColor(Actor_Get(id), SPRITE_COLOR_BRG);
  AquaRockInside_AddMoaiWater(id);
  Actor_SetAnimation(id, 3);
  layer->offsetY -= toFIXED(WATER_LEVEL_STEP);
  Map_LoadCellsAndShapes();
  MapGrid_CenterOnFollowedPoint();
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_RISE_WITH_SPARKS_HIGH);
  for (i = 0; i < WATER_LEVEL_STEP; i++) {
    layer->offsetY += toFIXED(1);
    System_Wait(4);
  }
  Event_End();
  Event_SetFlag(FLAG_TEAR_STONE_MOAI_FILLED);
}

/// The times the last pool's cells flicker between two of its pictures, two
/// frames each, as the water starts to sink.
enum { POOL_FLICKERS = 15 };

/// Once Piers has learned Parch, the ground trembles and the water round the
/// tablet's dais sinks away, but a pool is left on the steps before the
/// tablet, which keeps Felix from leaving until Parch dries it.
void AquaRockInside_WaterSinks(void) {
  struct MapLayer *layer =
      &((struct MapWork *)Ram_HeapSlots->mapGrid)->layers[1];
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  layer->offsetY -= toFIXED(WATER_LEVEL_STEP);
  MapGrid_CenterOnFollowedPoint();
  MapGrid_SetShake(1, 1, 1);
  for (i = 0; i < POOL_FLICKERS; i++) {
    {
      s32 column = 77;
      s32 row = 8;

      MapGrid_CopyCellsLayered(70, 8, 7, 6, column, row);
      System_Wait(1);
      MapGrid_CopyCellsLayered(50, 8, 7, 6, column, row);
      System_Wait(1);
    }
  }
  System_Wait(10);
  for (i = 0; i < WATER_LEVEL_STEP * 4; i++) {
    layer->offsetY += toFIXED(1) / 4;
    System_Wait(1);
  }
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_End();
  MapGrid_CopyCellAttributes(38, 9, 7, 6, 13, 9);
  Event_SetFlag(FLAG_PARCH_TABLET_POOL_LEFT);
}

/// Parch on the pool left on the steps before the Parch tablet dries it, and
/// the way out of the room is open.
void AquaRockInside_ParchLastPool(void) {
  MapGrid_CopyCellsLayered(70, 8, 7, 6, 77, 8);
  MapGrid_CopyCellAttributes(38, 17, 7, 6, 13, 9);
  Event_ClearFlag(FLAG_PARCH_TABLET_POOL_LEFT);
}

/// A small glowing spark at the actor, running gAquaRockInside_SparkScript.
void AquaRockInside_SpawnSpark(struct Actor *actor) {
  struct Actor *child = Import_ActorCreate_(SPRITE_PARTICLE, actor->x.fixed,
                                            actor->y.fixed, actor->z.fixed);

  child->scaleY = FIXED_ONE * 2 / 5;
  child->scaleX = FIXED_ONE * 2 / 5;
  child->motionFlags = 0;
  FieldActor_SetSpriteColor(child, SPRITE_COLOR_RBG);
  Scene_SetActorSpritePriority(child, 1);
  Actor_SelectAnimation(child, 1);
  Actor_SetSpriteFlags(child, 0);
  Actor_SetBlendMode(child, 1);
  Actor_StartScript(child, gAquaRockInside_SparkScript);
}

/// Turns an orbiting object by half its speed and puts it out from its anchor,
/// raised 8 pixels, by its radius.
static inline void Orbiter_Step(struct Orbiter *orbiter, struct Actor *anchor) {
  orbiter->angle += (s16)orbiter->speed >> 1;
  orbiter->x = anchor->x.fixed;
  orbiter->y = anchor->y.fixed + toFIXED(8);
  orbiter->z = anchor->z.fixed;
  FieldVector_AddPolarOffsetFX(orbiter->orbitRadius, orbiter->angle,
                               &orbiter->x);
}

/// Steps an orbiting object round its anchor: returns 0 once its countdown
/// runs out; else, unless its radius is down to 2 pixels, it leaves a spark,
/// closes 4 pixels and steps again, leaving another and closing 4 more.
/// Returns 1 while it runs.
s32 Orbiter_Spiral(struct Orbiter *orbiter) {
  struct Actor *anchor = orbiter->anchor;

  Orbiter_Step(orbiter, anchor);
  if (--orbiter->countdown == 0)
    return 0;
  if (orbiter->orbitRadius == toFIXED(2))
    return 1;
  AquaRockInside_SpawnSpark(orbiter);
  orbiter->orbitRadius -= toFIXED(4);
  Orbiter_Step(orbiter, anchor);
  AquaRockInside_SpawnSpark(orbiter);
  orbiter->orbitRadius -= toFIXED(4);
  return 1;
}

// Moves around in a circle.
#include "shared/orbit_update.h"

#include "shared/actor_aim_at_watched.h"

#include "shared/actor_step_to_watched.h"

#include "shared/actor_emit_bubbles.h"

extern u32 gAquaRockInside_TabletRiseScript[];
extern u32 gAquaRockInside_TabletGlowScript[];
extern s32 gAquaRockInside_SpiralSparkScript[];
extern u32 gAquaRockInside_PiersRiseScript[];
extern s32 gAquaRockInside_SwirlingSparkScript[];
extern s32 gAquaRockInside_TabletSettleScript[];
extern s32 gAquaRockInside_PiersSettleScript[];

/// The tablet (`firstId`) rises and glows, sparks gather on Piers (`secondId`)
/// and he learns Parch in a flash.
void AquaRockInside_PiersLearnsParch(s32 firstId, s32 secondId) {
  struct Actor *first = Actor_GetById(firstId);
  struct Actor *second = Actor_GetById(secondId);
  struct Actor *actor;
  struct Actor *spark;
  struct Actor *swirl;
  s32 i;
  u16 noMotion;
  const u8 *script;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  s32 unused[3];

  script = gAquaRockInside_PiersRiseScript;
  Sound_Play(SOUND_FADE_OUT_BGM);
  Event_Wait(30);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveWithinMapBounds(INTtoFIXED(FIXEDtoINT(first->x.fixed)),
                                  INTtoFIXED(FIXEDtoINT(first->y.fixed)),
                                  INTtoFIXED(FIXEDtoINT(first->z.fixed)), 1);
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(2, 2, 1);
  PaletteFade_SetSource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  first->motionFlags = 0;
  first->acceleration = toFIXED(1) / 5;
  first->speedLimit = toFIXED(1) / 5;
  Actor_SetMovementTypeById(firstId, gAquaRockInside_TabletRiseScript);
  Sound_Play(SE_TABLET_RISE);
  Event_Wait(45);
  Actor_ConfigureSpriteColor(firstId, SPRITE_COLOR_BLINK);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  for (i = 0; i != TABLET_RISING_SPARKS; i++) {
    actor = ((void *)Import_ActorCreate(SPRITE_PARTICLE, first->x.fixed,
                                        first->y.fixed, first->z.fixed));
    Sound_Play(SE_GLITTER);
    actor->x.fixed += INTtoFIXED((Random16() & 31) - 16);
    actor->y.fixed += INTtoFIXED((Random16() & 15) - 8);
    actor->speedLimit = toFIXED(8);
    actor->acceleration = toFIXED(1) / 5;
    actor->motionFlags = 0;
    Actor_SetSpriteColor(actor, SPRITE_COLOR_RBG);
    Scene_SetActorSpritePriority(actor, 1);
    Actor_SelectAnimation(actor, 7);
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetBlendMode(actor, 1);
    Actor_StartScript(actor, gAquaRockInside_TabletGlowScript);
    REG_BLDALPHA = BLEND_ALPHA(4, 16);
    wait(2);
    REG_BLDALPHA = BLEND_ALPHA(10, 16);
    wait(2);
    REG_BLDALPHA = BLEND_ALPHA(16, 16);
    wait(2);
    REG_BLDALPHA = BLEND_ALPHA(10, 16);
    wait(2);
  }
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  wait(30);
  for (i = 0; i != LEARN_SPARKS; i++) {
    spark = FieldActor_Create(SPRITE_PARTICLE, second->x.fixed, second->y.fixed,
                              second->z.fixed);
    Sound_Play(SE_SPARKS);
    spark->timer = (Random16() & 0x7fff) + DEGtoANG(180);
    spark->userValue = (((i & 1) << 1) - 1) << ((Random16() & 3) + 9);
    noMotion = 0;
    spark->motionFlags = noMotion;
    spark->riseBaseY = toFIXED(122);
    spark->watchedActor = second;
    spark->riseCounter =
        (LEARN_SPARKS - i) * LEARN_SPARK_INTERVAL + LEARN_SPIRAL_FRAMES;
    Actor_SetSpriteColor(spark, SPRITE_COLOR_RBG);
    Scene_SetActorSpritePriority(spark, 1);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    FieldVector_AddPolarOffsetFX(spark->riseBaseY, spark->timer,
                                 &spark->x.fixed);
    Actor_StartScript(spark, gAquaRockInside_SpiralSparkScript);
    if (i == 3) {
      Actor_SetSpriteColorById(secondId, SPRITE_COLOR_BLINK);
      second->motionFlags = noMotion;
      second->acceleration = toFIXED(1) / 5;
      second->speedLimit = toFIXED(1) / 5;
      Actor_SetMovementTypeById(secondId, script);
    }
    wait(LEARN_SPARK_INTERVAL);
  }
  Sound_Play(SE_SHIMMER);
  wait(16);
  for (i = 2; i != ORBIT_HALF_TURN; i += 2) {
    swirl = FieldActor_Create(SPRITE_PARTICLE, second->x.fixed,
                              second->y.fixed + i * toFIXED(1) - toFIXED(8),
                              second->z.fixed);
    swirl->timer = Random16();
    swirl->userValue = TRIG_TURN / 32;
    swirl->riseCounter = i;
    swirl->riseEnabled = 1;
    swirl->bounce = swirl->x.fixed;
    swirl->riseBaseY = 0;
    swirl->gravity = swirl->z.fixed;
    swirl->watchedActor = second;
    swirl->motionFlags = (u16)0;
    swirl->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetSpriteColor(swirl, SPRITE_COLOR_RBG);
    Actor_SelectAnimation(swirl, 7);
    Actor_SetSpriteFlags(swirl, 0);
    Actor_SetBlendMode(swirl, 1);
    Actor_StartScript(swirl, gAquaRockInside_SwirlingSparkScript);
  }
  Sound_Play(SE_SHIMMER);
  wait(50);
  PaletteFade_SetTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_StartInterpolation(8);
  wait(16);
  MapGrid_SetShake(0, 0, 0);
  Actor_ConfigureSpriteColor(firstId, SPRITE_COLOR_NORMAL);
  Actor_ConfigureSpriteColor(secondId, SPRITE_COLOR_NORMAL);
  Sound_Play(SE_FLASH);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(80);
  Actor_SetMovementTypeById(firstId, gAquaRockInside_TabletSettleScript);
  Actor_SetMovementTypeById(secondId, gAquaRockInside_PiersSettleScript);
  second->update = Actor_EmitBubbles;
  wait(120);
  second->update = 0;
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Scene_PlayMusic();
  wait(1);
}

void Actor_PlaceOnActor();

/// Reading the tablet from its south side: "Wielder of Water's strength... Lay
/// your hands upon this stone." Piers steps out of the party ("Water's
/// strength, huh? That sounds like my sort of thing"), touches it, rises in a
/// glow and learns Parch. The water then sinks, leaving a pool on the steps;
/// Piers wonders what went wrong, decides their power is being tested, and
/// walks back into Felix. Once Parch is learned only the tablet's words show.
void AquaRockInside_ReadParchTablet(void) {
  struct Actor *actor;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  // "Felix checked the tablet. It looks like there are strange characters..."
  Message_ShowCentered((s32)&MsgCheckedTheTabletIt, 1);
  if (Event_GetFlag(FLAG_PIERS_LEARNED_PARCH)) {
    Event_End();
    return;
  }
  FieldEvent_SetMessageId((s32)&MsgCheckedTheTabletIt);
  gFieldWork->message = (gFieldWork->message + 1);
  Actor_PlaceOnActor(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
  FieldEvent_Wait(1);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, 256, 184);
  Actor_WalkTo(ACTOR_PIERS, 274, 184);
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  FieldActor_WaitForMoveById(ACTOR_PIERS);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  FieldEvent_Wait(5);
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 248, 184);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_PIERS, 1);
  Actor_WalkToAndWait(ACTOR_PIERS, 264, 168);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Message_ShowCentered(((s32)&MsgCheckedTheTabletIt + 2), 1);
  AquaRockInside_PiersLearnsParch(ACTOR_PARCH_TABLET, ACTOR_PIERS);
  Party_LearnAbility(PIERS, ABILITY_PARCH);
  Event_SetFlag(FLAG_PIERS_LEARNED_PARCH);
  Event_PlaySound(SE_TREMOR);
  FieldEvent_Wait(30);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  AquaRockInside_WaterSinks();
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  FieldEvent_Wait(30);
  FieldEvent_SetMessageId((s32)&MsgWhatWentWrong);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Actor_FaceEachOther(gMainRoutineWork.leaderActorId, ACTOR_PIERS, 0);
  FieldEvent_Wait(10);
  // "What went wrong? What are we going to do, Felix?"
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Maybe our power is being tested."
  Event_ShowMessage(ACTOR_PIERS, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_PIERS, 1.2);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_WALK);
  actor = Actor_Get(gMainRoutineWork.leaderActorId);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_PIERS, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  FieldActor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Event_End();
}

/// Checking the Parch tablet from its north side: Felix walks round to the
/// front and reads it (AquaRockInside_ReadParchTablet), or once Parch is
/// learned only its words show.
void AquaRockInside_WalkToParchTablet(void) {
  if (Event_GetFlag(FLAG_PIERS_LEARNED_PARCH)) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgCheckedTheTabletIt, 1);
    Event_End();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 248, 152);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 248, 184);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 264, 184);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    AquaRockInside_ReadParchTablet();
  }
}

#define DEFINE_AQUA_ROCK_INSIDE_READ_TABLET(name, n1)                          \
  void name(void) {                                                            \
    if (Event_GetFlag(FLAG_PIERS_LEARNED_PARCH)) {                             \
      Event_Start();                                                           \
      FieldEvent_ResolvePendingActions(0);                                     \
      Message_ShowCentered(MsgCheckedTheTabletIt, 1);                          \
      Event_End();                                                             \
    } else {                                                                   \
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, n1, 184);            \
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 264, 184);           \
      Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);    \
      FieldEvent_Wait(10);                                                     \
      AquaRockInside_ReadParchTablet();                                        \
    }                                                                          \
  }

/// Checking the Parch tablet from its east side: Felix steps round to the front
/// and reads it.
DEFINE_AQUA_ROCK_INSIDE_READ_TABLET(AquaRockInside_ReadTabletFacingWest, 280)

/// Checking the Parch tablet from its west side: Felix steps round to the front
/// and reads it.
DEFINE_AQUA_ROCK_INSIDE_READ_TABLET(AquaRockInside_ReadTabletFacingEast, 248)

extern struct Trigger gAquaRockInside_Triggers[];
extern struct Trigger gAquaRockInside_WaterOfLifeChestTriggers[];
extern struct Trigger gAquaRockInside_SteamTriggers[];
extern struct Trigger gAquaRockInside_AquariusStoneTriggers[];
extern struct Trigger gAquaRockInside_RustySwordChestTriggers[];
extern struct Trigger gAquaRockInside_CrystalPowderChestTriggers[];
extern struct Trigger gAquaRockInside_VialChestTriggers[];
extern struct Trigger gAquaRockInside_PsynergyStoneTriggers[];
extern s16 gAquaRockInside_TightropeTriggers[];
extern struct Trigger gAquaRockInside_TearStoneTriggers[];
extern struct Trigger gAquaRockInside_ParchTabletTriggers[];

const u8 *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST)
    return gAquaRockInside_WaterOfLifeChestTriggers;
  if (scene == SCENE_AQUA_ROCK_INSIDE_STEAM)
    return gAquaRockInside_SteamTriggers;
  if (scene == SCENE_AQUA_ROCK_INSIDE_AQUARIUS_STONE)
    return gAquaRockInside_AquariusStoneTriggers;
  if (scene == SCENE_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST)
    return gAquaRockInside_RustySwordChestTriggers;
  if (scene == SCENE_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST)
    return gAquaRockInside_CrystalPowderChestTriggers;
  if (scene == SCENE_AQUA_ROCK_INSIDE_VIAL_CHEST)
    return gAquaRockInside_VialChestTriggers;
  if (scene == SCENE_AQUA_ROCK_INSIDE_PSYNERGY_STONE)
    return gAquaRockInside_PsynergyStoneTriggers;
  if (scene == SCENE_AQUA_ROCK_INSIDE_TIGHTROPE)
    return gAquaRockInside_TightropeTriggers;
  if (scene == SCENE_AQUA_ROCK_INSIDE_TEAR_STONE)
    return gAquaRockInside_TearStoneTriggers;
  if (scene == SCENE_AQUA_ROCK_INSIDE_PARCH_TABLET)
    return gAquaRockInside_ParchTabletTriggers;
  return gAquaRockInside_Triggers;
}

/// Gives a Moai its water, a sprite added to its own: the pool in the top of
/// its head and the water pouring from its mouth.
void AquaRockInside_AddMoaiWater(s32 id) {
  struct Actor *actor = Actor_Get(id);

  if (actor != 0) {
    struct SpriteAnimation *water =
        (struct SpriteAnimation *)Sprite_AddAnimation(actor->sprite,
                                                      SPRITE_MOAI_WATER);

    water->colorMode = SPRITE_COLOR_NORMAL;
    water->priority = 0;
    Actor_SelectAnimation(actor, 2);
  }
}

/// Every sixteenth frame, a splash under the actor.
void AquaRockInside_SplashBeneath(struct Actor *actor) {
  s32 phase = gFrameCount & 15;

  if (phase == 0) {
    struct Actor *child = Import_ActorCreate_(
        SPRITE_SPLASH_RING, actor->x.fixed, actor->y.fixed, actor->z.fixed);

    if (child != 0) {
      struct FieldSprite *sprite = child->sprite;

      Actor_StartScript(child, gAquaRockInside_SplashScript);
      child->motionFlags = 0;
      child->layer = 1;
      if (sprite != 0) {
        Sprite_StartAnimation(sprite, 2);
        sprite->flags = 0;
        sprite->rotation = DEGtoANG(90);
        sprite->priority = 2;
      }
    }
  }
}

/// Arriving in the Vial chest's room down its waterfall from the Mist Sabre
/// chest's room: Felix, tinted blue, drops spinning into the pool at the
/// waterfall's foot, splashes and comes to stand on the yellow block there.
void AquaRockInside_FallThroughWater(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  u8 *flags = &actor->motionFlags;
  s32 saved = *flags;
  FIXED speed;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Actor_SetSpriteColorMode(actor, SPRITE_COLOR_BLUE_TINT);
  *flags = 0;
  actor->y.fixed = toFIXED(80);
  speed = toFIXED(3);
  for (i = 0; i < 32; i++) {
    actor->y.fixed -= speed;
    actor->facing += DEGtoANG(45);
    speed += -(toFIXED(1) / 20);
    Actor_SetSpriteFlags(actor, 0);
    System_Wait(1);
  }
  Actor_SpawnWadingSplash(actor);
  Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 2);
  *flags = saved;
  actor->velocityY = toFIXED(8);
  Event_PlaySound(SE_SPLASH_SMALL);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 168, 200);
  Event_End();
}

/// How long the vents blow: an hour of frames.
#define VENTS_BLOW_FRAMES (60 * 60 * 60)

/// The vents' hook: they blow for VENTS_BLOW_FRAMES, with their sound, then
/// stop.
void AquaRockInside_UpdateVents(void) {
  s32 counter = Vent_GetCounter();

  switch (counter) {
  case 0:
    Vent_SetBlowing(0, 1);
    Vent_SetBlowing(1, 1);
    Scene_QueueMusic(SE_AMBIENT_WATER);
    break;
  case VENTS_BLOW_FRAMES - 1:
    Vent_SetBlowing(0, 0);
    Vent_SetBlowing(1, 0);
    Scene_QueueMusic(-1);
    break;
  }
  Vent_AdvanceCounter(VENTS_BLOW_FRAMES);
}

extern u16 gAquaRockInside_CrystalPowderChestVents[];
extern u16 gAquaRockInside_CrystalPowderChestVentPillars[];
extern u16 gAquaRockInside_VialChestVents[];
extern u16 gAquaRockInside_VialChestVentPillars[];

/// Aqua Rock's entry setup: in each room it shows the statues already filled by
/// their flags (or dims those still empty), opens what the story has opened and
/// arms the room's water and cell handlers by entrance.
s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.next;
  s32 entrance = gMainRoutineWork.entrance;
  struct Actor *actor;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (scene == SCENE_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST) {
    if (GameFlag_Get(FLAG_WATER_OF_LIFE_CHEST_EAST_MOAI_FILLED)) {
      AquaRockInside_AddMoaiWater(ACTOR_WATER_OF_LIFE_CHEST_EAST_MOAI);
      Actor_SetAnimation(ACTOR_WATER_OF_LIFE_CHEST_EAST_MOAI, 3);
    } else {
      FieldActor_SetSpriteColor(
          Actor_GetById(ACTOR_WATER_OF_LIFE_CHEST_EAST_MOAI),
          SPRITE_COLOR_GREY);
    }
    if (GameFlag_Get(FLAG_WATER_OF_LIFE_CHEST_WEST_MOAI_FILLED)) {
      AquaRockInside_AddMoaiWater(ACTOR_WATER_OF_LIFE_CHEST_WEST_MOAI);
      Actor_SetAnimation(ACTOR_WATER_OF_LIFE_CHEST_WEST_MOAI, 3);
    } else {
      FieldActor_SetSpriteColor(
          Actor_GetById(ACTOR_WATER_OF_LIFE_CHEST_WEST_MOAI),
          SPRITE_COLOR_GREY);
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_STEAM) {
    if (GameFlag_Get(FLAG_STEAM_ROOM_STREAM_DRAINED)) {
      MapGrid_CopyCellsLayered(118, 15, 5, 32, 110, 15);
      MapGrid_CopyCellAttributes(59, 16, 5, 32, 46, 16);
      FieldActor_SetSpriteColor(Actor_GetById(ACTOR_STEAM_ROOM_MOAI),
                                SPRITE_COLOR_GREY);
    } else {
      AquaRockInside_AddMoaiWater(ACTOR_STEAM_ROOM_MOAI);
      Actor_SetAnimation(ACTOR_STEAM_ROOM_MOAI, 3);
    }
    actor = Actor_GetById(ACTOR_STEAM_ROOM_PILLAR);
    actor->motionFlags = 0;
    if (GameFlag_Get(FLAG_AQUA_ROCK_STEAM_ROOM_PILLAR_DROPPED)) {
      Actor_SetPosition(ACTOR_STEAM_ROOM_PILLAR, 632, 152);
      MapGrid_CopyCellAttributes(41, 9, 1, 1, 39, 9);
      actor->y.fixed = 0;
      actor->groundHeight = 0;
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_AQUARIUS_STONE) {
    if (entrance == 7)
      Leader_RiseFromWater();
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST) {
    AquaRockInside_AddMoaiWater(ACTOR_RUSTY_SWORD_CHEST_MOAI_A);
    Actor_SetAnimation(ACTOR_RUSTY_SWORD_CHEST_MOAI_A, 3);
    AquaRockInside_AddMoaiWater(ACTOR_RUSTY_SWORD_CHEST_MOAI_B);
    Actor_SetAnimation(ACTOR_RUSTY_SWORD_CHEST_MOAI_B, 3);
    if ((u32)(entrance - 6) <= 1) {
      if (GameFlag_Get(FLAG_RUSTY_SWORD_CHEST_MOAI_FILLED)) {
        FieldActor_SetSpriteColor(Actor_GetById(ACTOR_RUSTY_SWORD_CHEST_MOAI),
                                  SPRITE_COLOR_BRG);
        AquaRockInside_AddMoaiWater(ACTOR_RUSTY_SWORD_CHEST_MOAI);
        Actor_SetAnimation(ACTOR_RUSTY_SWORD_CHEST_MOAI, 3);
        AquaRockInside_SetPillarCellJumpableInWater();
      } else {
        FieldActor_SetSpriteColor(Actor_GetById(ACTOR_RUSTY_SWORD_CHEST_MOAI),
                                  SPRITE_COLOR_GREY);
      }
    }
    Actor_GetById(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_B)->riseCounter = 1;
    Actor_GetById(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_C)->riseCounter = 1;
    actor = Actor_GetById(ACTOR_RUSTY_SWORD_CHEST);
    if (actor != 0) {
      actor->motionFlags = 0;
      actor->groundHeight = -toFIXED(WATER_LEVEL_STEP);
      actor->y.fixed = -toFIXED(WATER_LEVEL_STEP);
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST) {
    if ((u32)(entrance - 5) <= 1) {
      // Vents_Start, defined below, is called with no prototype
      // in scope.
      Vents_Start(gAquaRockInside_CrystalPowderChestVents,
                  gAquaRockInside_CrystalPowderChestVentPillars,
                  AquaRockInside_UpdateVents);
      Scene_QueueMusic(SE_AMBIENT_WATER);
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_VIAL_CHEST) {
    if (entrance == 5 || entrance == 7) {
      Vents_Start(gAquaRockInside_VialChestVents,
                  gAquaRockInside_VialChestVentPillars,
                  AquaRockInside_UpdateVents);
      Scene_QueueMusic(SE_AMBIENT_WATER);
      AquaRockInside_SettleVentPillars(0, ACTOR_VIAL_CHEST_WEST_VENT_PILLAR);
    }
    if (!GameFlag_Get(FLAG_VIAL_CHEST_MOAI_BLOCKED)) {
      AquaRockInside_AddMoaiWater(ACTOR_VIAL_CHEST_MOAI);
      Actor_SetAnimation(ACTOR_VIAL_CHEST_MOAI, 3);
    }
    actor = Actor_GetById(ACTOR_VIAL_CHEST_SPLASH);
    Actor_SetSpriteFlags(actor, 0);
    actor->collisionFlags = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->update = (ObjectUpdate)AquaRockInside_SplashBeneath;
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        entrance == AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_MIST_SABRE_CHEST)
      AquaRockInside_FallThroughWater();
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_TEAR_STONE) {
    if (GameFlag_Get(FLAG_TEAR_STONE_MOAI_FILLED)) {
      FieldActor_SetSpriteColor(Actor_GetById(ACTOR_TEAR_STONE_MOAI),
                                SPRITE_COLOR_BRG);
      AquaRockInside_AddMoaiWater(ACTOR_TEAR_STONE_MOAI);
      Actor_SetAnimation(ACTOR_TEAR_STONE_MOAI, 3);
    } else {
      FieldActor_SetSpriteColor(Actor_GetById(ACTOR_TEAR_STONE_MOAI),
                                SPRITE_COLOR_GREY);
    }
    Actor_GetById(ACTOR_TEAR_STONE_WATER_PILLAR_A)->riseCounter = 1;
    Actor_GetById(ACTOR_TEAR_STONE_WATER_PILLAR_B)->riseCounter = 1;
    Actor_GetById(ACTOR_TEAR_STONE_WATER_PILLAR_E)->riseCounter = 1;
    Actor_GetById(ACTOR_TEAR_STONE_WATER_PILLAR_G)->riseCounter = 1;
    Actor_GetById(ACTOR_TEAR_STONE_WATER_PILLAR_H)->riseCounter = 1;
    Actor_GetById(ACTOR_TEAR_STONE_WATER_PILLAR_I)->riseCounter = 1;
  }
  return 0;
}

#include "text/msg_ids.h"

/// Sets BG1CNT's priority bits, through the volatile copy the routine keeps.
#define SET_BG1CNT_PRIORITY(priority)                                          \
  {                                                                            \
    u16 value;                                                                 \
                                                                               \
    value = (control = REG_BG1CNT & ~BGCNT_PRIORITY(3));                       \
    do {                                                                       \
      REG_BG1CNT = value | (priority);                                         \
    } while (0);                                                               \
  }

void Tightrope_Initialize();

/// Aqua Rock's map preparation: for each room and entrance it sets the water
/// level, places the statues and boulders the flags have moved, opens or
/// closes the cells around them, and in the rooms whose water drains it shows
/// the drained state once its flag is set. The Vial chest's room puts its
/// puddles out of sight (x and z cleared) until a pillar has stopped its Moai.
s32 Scene_PrepareMap(void) {
  s32 scene = gMainRoutineWork.next;
  s32 entrance = gMainRoutineWork.entrance;
  volatile u16 control;
  s32 streamStep, rustySwordStep, rustySwordPillar, crystalPowderStep,
      hiddenPuddle, vialPuddle, tearStoneStep, tearStonePillar;
  struct Actor *actor;
  struct MapWork *map;

  Map_SetUpperLevelWading(Ram_HeapSlots->mapGrid);
  AquaRockInside_StartPillars();
  if (scene == SCENE_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST) {
    if ((u32)(entrance - 3) <= 1 || entrance == 6 || entrance == 8) {
      AquaRockInside_AddSpouts(COLLISION_STREAM_BEND, 0, -2);
    } else if (entrance == 5 || entrance == 7) {
      actor = Actor_GetById(ACTOR_WATER_OF_LIFE_CHEST_BLUE_ROCK);
      actor->motionFlags = 0;
      actor->groundHeight = 0;
      actor->y.fixed = 0;
      Actor_SelectAnimation((s32)actor, 0);
      actor = Actor_GetById(ACTOR_WATER_OF_LIFE_CHEST_PUDDLE);
      actor->motionFlags = 0;
      actor->groundHeight = 0;
      actor->y.fixed = 0;
      if (!GameFlag_Get(FLAG_WATER_OF_LIFE_CHEST_EAST_MOAI_FILLED)) {
        MapGrid_CopyCellsLayered(104, 80, 16, 10, 104, 40);
        MapGrid_CopyCellsLayered(110, 116, 18, 12, 39, 105);
        MapGrid_CopyCellAttributes(64, 32, 64, 32, 0, 32);
        AquaRockInside_AddSpouts(COLLISION_STREAM_BEND, 0, 0);
      } else {
        AquaRockInside_AddSpouts(COLLISION_STREAM_BEND, 4, 0);
        actor = Actor_GetById(ACTOR_WATER_OF_LIFE_CHEST_BLUE_ROCK);
        AquaRockInside_SetPillarHeight(FIELD_CELL(actor->x.fixed) - 42, 0);
      }
      if (GameFlag_Get(FLAG_WATER_OF_LIFE_CHEST_PUDDLE_FROZEN)) {
        MapGrid_CopyCellAttributes(41, 42, 1, 1, 43, 44);
      }
    } else if ((u32)(entrance - 9) <= 1) {
      if (!GameFlag_Get(FLAG_WATER_OF_LIFE_CHEST_WEST_MOAI_FILLED)) {
        MapGrid_CopyCellsLayered(77, 80, 23, 20, 77, 32);
        MapGrid_CopyCellsLayered(112, 96, 16, 20, 12, 98);
        MapGrid_CopyCellAttributes(64, 32, 64, 32, 0, 32);
        AquaRockInside_AddSpouts(COLLISION_STREAM_BEND, 0, 0);
      } else {
        AquaRockInside_AddSpouts(COLLISION_STREAM_BEND, 4, 0);
      }
    }
    if (GameFlag_Get(FLAG_AQUA_ROCK_WEST_PILLAR_SET)) {
      Actor_SetPosition(ACTOR_WATER_OF_LIFE_CHEST_WEST_PILLAR, 184, 200);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 9, 12);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 13, 12);
    }
    if (GameFlag_Get(FLAG_AQUA_ROCK_EAST_PILLAR_SET)) {
      Actor_SetPosition(ACTOR_WATER_OF_LIFE_CHEST_EAST_PILLAR, 824, 216);
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_STEAM) {
    if ((u32)(entrance - 1) <= 1) {
      AquaRockInside_AddSpouts(COLLISION_STREAM_BEND, 0, -2);
    }
    actor = Actor_GetById(ACTOR_STEAM_ROOM_MOAI);
    actor->motionFlags = 0;
    actor->groundHeight = 0;
    actor->y.fixed = 0;
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_AQUARIUS_STONE) {
    FieldEffect_StartBlankOverlay(0);
    Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_AQUARIUS_STONE_BASE), 0);
    Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_AQUARIUS_STONE), 0);
    AquaRockInside_AddSpouts(COLLISION_STREAM_BEND, 8, -2);
    AquaRockInside_AddSpouts(COLLISION_SECOND_SPOUTS, 8, 8);
    if (GameFlag_Get(FLAG_AQUA_ROCK_AQUARIUS_STONE_WATER_STONE_DOUSED)) {
      AquaRockInside_StartSplashes();
      AquaRockInside_AddSplash(16, 4, 16, 2);
      AquaRockInside_AddSplash(22, 4, 16, 0);
      for (streamStep = 0; streamStep < 6; streamStep++)
        AquaRockInside_AdvanceStreams(COLLISION_STREAM_BEND,
                                      COLLISION_STREAM_GOAL);
    }
    if (GameFlag_Get(FLAG_AQUA_ROCK_AQUARIUS_STONE_SET)) {
      MapGrid_CopyCellsLayered(75, 96, 7, 13, 80, 27);
      MapGrid_CopyCellsLayered(91, 96, 7, 10, 16, 31);
      MapGrid_CopyCellsLayered(99, 96, 7, 14, 16, 91);
      MapGrid_CopyCellAttributes(43, 0, 5, 9, 17, 31);
      AquaRockInside_ShowAquariusStone();
    } else {
      MapGrid_CopyCellAttributes(17, 32, 5, 1, 17, 31);
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST) {
    FieldEffect_StartBlankOverlay(0);
    if ((u32)(entrance - 6) <= 1) {
      Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
          ACTOR_PRIORITY_IGNORES_RISES;
      Value3((s32 (*)())AquaRockInside_AddPillar,
             ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A, 5, 4);
      Value3((s32 (*)())AquaRockInside_AddPillar,
             ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_B, 3, 4);
      Value3((s32 (*)())AquaRockInside_AddPillar,
             ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_C, 4, 4);
      AquaRockInside_AddSpouts(COLLISION_SECOND_SPOUTS, 4, 4);
      if (!GameFlag_Get(FLAG_RUSTY_SWORD_CHEST_MOAI_FILLED) &&
          !Event_GetFlag(FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED)) {
        map = Ram_HeapSlots->mapGrid;
        MapGrid_CopyCellsLayered(64, 64, 16, 13, 71, 39);
        MapGrid_CopyCellAttributes(71, 39, 16, 15, 7, 39);
        AquaRockInside_SetPillarCellJumpable();
        map->enabled[2] = 1;
        SET_BG1CNT_PRIORITY(1);
      } else if (Event_GetFlag(
                     FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED) ||
                 GameFlag_Get(FLAG_RUSTY_SWORD_CHEST_WATER_STONE_DOUSED)) {
        if (Event_GetFlag(FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED)) {
          Actor_SetPosition(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A, 280, 712);
        }
        AquaRockInside_BlockCellUnder(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A,
                                      COLLISION_STREAM_BEND + 5);
        AquaRockInside_StartSplashes();
        AquaRockInside_AddSplash(12, 2, 45, 1);
        for (rustySwordStep = 0; rustySwordStep < 8; rustySwordStep++)
          AquaRockInside_AdvanceStreams(COLLISION_STREAM_BEND,
                                        COLLISION_STREAM_GOAL);
        Event_SetFlag(FLAG_RUSTY_SWORD_CHEST_MOAI_FILLED);
      }
      for (rustySwordPillar = ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A;
           rustySwordPillar <= ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_C;
           rustySwordPillar++) {
        actor = Actor_GetById(rustySwordPillar);
        actor->motionFlags = 0;
        actor->groundHeight = 0;
        actor->y.fixed = 0;
      }
    } else {
      AquaRockInside_AddSpouts(COLLISION_STREAM_BEND, 0, -4);
    }
    if (GameFlag_Get(FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_POOL_PARCHED)) {
      AquaRockInside_ParchPool();
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST) {
    AquaRockInside_AddSpouts(COLLISION_STREAM_BEND, 0, -2);
    AquaRockInside_AddSpouts(COLLISION_SECOND_SPOUTS, 0, 0);
    FieldEffect_StartBlankOverlay(0);
    if (((u32)(entrance - 1) <= 3 || entrance == 7) &&
        GameFlag_Get(FLAG_AQUA_ROCK_CRYSTAL_POWDER_WATER_STONE_DOUSED)) {
      AquaRockInside_StartSplashes();
      AquaRockInside_AddSplash(44, 0, 34, 2);
      for (crystalPowderStep = 0; crystalPowderStep <= 4; crystalPowderStep++)
        AquaRockInside_AdvanceStreams(COLLISION_STREAM_BEND,
                                      COLLISION_STREAM_GOAL);
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_VIAL_CHEST) {
    FieldEffect_StartBlankOverlay(0);
    if (entrance ==
            AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_NORTHWEST ||
        entrance ==
            AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_MIST_SABRE_CHEST) {
      AquaRockInside_AddSpouts(COLLISION_STREAM_BEND, 0, -2);
    } else if (
        entrance == ENTRANCE_VIAL_CHEST_WEST_DOORWAY ||
        entrance == ENTRANCE_VIAL_CHEST_EAST_DOORWAY ||
        entrance ==
            AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_SOUTH) {
      AquaRockInside_AddSpouts(COLLISION_STREAM_BEND, 0, -2);
    }
    if (!GameFlag_Get(FLAG_VIAL_CHEST_MOAI_BLOCKED)) {
      for (hiddenPuddle = ACTOR_VIAL_CHEST_PUDDLE_A;
           hiddenPuddle <= ACTOR_VIAL_CHEST_PUDDLE_C; hiddenPuddle++) {
        actor = Actor_GetById(hiddenPuddle);
        actor->motionFlags = 0;
        actor->x.fixed = 0;
        actor->z.fixed = 0;
      }
      return 0;
    } else {
      MapGrid_CopyCellsLayered(114, 39, 1, 2, 113, 39);
      MapGrid_CopyCellAttributes(105, 41, 12, 13, 41, 41);
      MapGrid_CopyCellsLayered(105, 105, 12, 13, 105, 41);
      for (vialPuddle = ACTOR_VIAL_CHEST_PUDDLE_A;
           vialPuddle <= ACTOR_VIAL_CHEST_PUDDLE_C; vialPuddle++) {
        actor = Actor_GetById(vialPuddle);
        if (GameFlag_Get(FLAG_VIAL_CHEST_PUDDLE_A_FROZEN -
                         ACTOR_VIAL_CHEST_PUDDLE_A + vialPuddle)) {
          MapGrid_CopyCellAttributes(47, 45, 1, 1, FIELD_CELL(actor->x.fixed),
                                     FIELD_CELL(actor->z.fixed));
        }
      }
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_PSYNERGY_STONE) {
    AquaRockInside_AddSpouts(COLLISION_STREAM_BEND, 0, -2);
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_TIGHTROPE) {
    FieldEffect_StartBlankOverlay(0);
    AquaRockInside_AddSpouts(COLLISION_STREAM_BEND, 0, -2);
    Tightrope_Initialize();
    Tightrope_Place(0, ACTOR_TIGHTROPE_WEST_POST, ACTOR_TIGHTROPE_EAST_POST);
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_TEAR_STONE) {
    if ((u32)(entrance - 3) <= 1) {
      Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
          ACTOR_PRIORITY_IGNORES_RISES;
      Value3((s32 (*)())AquaRockInside_AddPillar,
             ACTOR_TEAR_STONE_WATER_PILLAR_A, 6, 4);
      Value3((s32 (*)())AquaRockInside_AddPillar,
             ACTOR_TEAR_STONE_WATER_PILLAR_B, 3, 4);
      Value3((s32 (*)())AquaRockInside_AddPillar,
             ACTOR_TEAR_STONE_WATER_PILLAR_C, 5, 4);
      Value3((s32 (*)())AquaRockInside_AddPillar,
             ACTOR_TEAR_STONE_WATER_PILLAR_D, 6, 4);
      Value3((s32 (*)())AquaRockInside_AddPillar,
             ACTOR_TEAR_STONE_WATER_PILLAR_E, 5, 4);
      Value3((s32 (*)())AquaRockInside_AddPillar,
             ACTOR_TEAR_STONE_WATER_PILLAR_F, 4, 4);
      Value3((s32 (*)())AquaRockInside_AddPillar,
             ACTOR_TEAR_STONE_WATER_PILLAR_G, 4, 4);
      Value3((s32 (*)())AquaRockInside_AddPillar,
             ACTOR_TEAR_STONE_WATER_PILLAR_H, 3, 4);
      Value3((s32 (*)())AquaRockInside_AddPillar,
             ACTOR_TEAR_STONE_WATER_PILLAR_I, 6, 4);
      AquaRockInside_AddSpouts(COLLISION_SECOND_SPOUTS, 4, 4);
      if (!GameFlag_Get(FLAG_TEAR_STONE_MOAI_FILLED) &&
          !Event_GetFlag(FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED)) {
        map = Ram_HeapSlots->mapGrid;
        MapGrid_CopyCellsLayered(64, 64, 19, 17, 100, 13);
        MapGrid_CopyCellsLayered(35, 105, 23, 24, 35, 73);
        MapGrid_CopyCellAttributes(100, 13, 19, 19, 36, 13);
        map->enabled[2] = 1;
        SET_BG1CNT_PRIORITY(1);
      } else if (Event_GetFlag(FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED) ||
                 GameFlag_Get(FLAG_TEAR_STONE_WATER_STONE_DOUSED)) {
        if (Event_GetFlag(FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED)) {
          Actor_SetPosition(ACTOR_TEAR_STONE_WATER_PILLAR_C, 808, 296);
          Actor_SetPosition(ACTOR_TEAR_STONE_WATER_PILLAR_D, 680, 360);
          Actor_SetPosition(ACTOR_TEAR_STONE_WATER_PILLAR_F, 744, 424);
        }
        Call2((void (*)())AquaRockInside_BlockCellUnder,
              ACTOR_TEAR_STONE_WATER_PILLAR_C, COLLISION_STREAM_BEND + 5);
        Call2((void (*)())AquaRockInside_BlockCellUnder,
              ACTOR_TEAR_STONE_WATER_PILLAR_D, COLLISION_STREAM_BEND + 6);
        AquaRockInside_BlockCellUnder(ACTOR_TEAR_STONE_WATER_PILLAR_F,
                                      COLLISION_STREAM_BEND + 4);
        AquaRockInside_StartSplashes();
        AquaRockInside_AddSplash(38, 2, 19, 1);
        for (tearStoneStep = 0; tearStoneStep < 8; tearStoneStep++)
          AquaRockInside_AdvanceStreams(COLLISION_STREAM_BEND,
                                        COLLISION_STREAM_GOAL);
        Event_SetFlag(FLAG_TEAR_STONE_MOAI_FILLED);
      }
    }
    for (tearStonePillar = ACTOR_TEAR_STONE_WATER_PILLAR_A;
         tearStonePillar <= ACTOR_TEAR_STONE_PILLAR_B; tearStonePillar++) {
      actor = Actor_GetById(tearStonePillar);
      actor->motionFlags = 0;
      actor->groundHeight = 0;
      actor->y.fixed = 0;
    }
    actor = Actor_GetById(ACTOR_TEAR_STONE_PILLAR_A);
    MapGrid_CopyCellAttributes(23, 39, 1, 1, FIELD_CELL(actor->x.fixed), 39);
    if ((u32)(entrance - 1) <= 1) {
      FieldEffect_StartBlankOverlay(0);
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_PARCH_TABLET) {
    FieldEffect_StartBlankOverlay(0);
    AquaRockInside_AddSpouts(COLLISION_STREAM_BEND, 0, -2);
    Event_SetFlag(FLAG_RETREAT_BLOCKED);
    if (GameFlag_Get(FLAG_PARCH_TABLET_POOL_LEFT)) {
      MapGrid_CopyCellAttributes(38, 9, 7, 6, 13, 9);
    } else {
      MapGrid_CopyCellsLayered(77, 8, 7, 6, 50, 8);
      MapGrid_CopyCellsLayered(70, 8, 7, 6, 77, 8);
      MapGrid_CopyCellAttributes(38, 17, 7, 6, 13, 9);
    }
  }
  return 0;
}

#include "shared/actor_spawn_wading_splash.h"

#include "shared/actor_emit_water_drop.h"

#include "shared/leader_flow_with_stream.h"

#include "shared/actor_show_parts.h"

#include "shared/leader_spin_away.h"

// On odd frames, spawns a drifting effect above the object, swaying with the
// frame count.
#include "shared/actor_emit_mist.h"

#include "shared/leader_sink_away.h"

#include "shared/leader_rise_from_water.h"

/// Draws the scene's pillars: a top sprite for each active one and, when it
/// stands above its base, a column sprite as tall as its height.
void AquaRockInside_DrawPillars(void) {
  s32 *view = &((struct MapWork *)Ram_HeapSlots->mapGrid)->viewX;
  struct PillarWork *work = &gAquaRockInside_Pillars;
  s32 viewX = view[0];
  struct PillarEntry *entry = work->entries;
  s32 viewZ;
  s32 originY;
  s32 depth;
  s32 i;
  u32 tile;

  viewX &= FIELD_PIXEL_MASK;
  viewZ = view[1] & FIELD_PIXEL_MASK;
  originY = ((struct MapWork *)Ram_HeapSlots->mapGrid)->origin[1];
  tile = gVramCacheEntries[work->vramEntry].offset >> 5;

  for (i = 0; i < PILLAR_ENTRIES; i++) {
    if (entry->state != 0) {
      s32 *position = entry->position;
      s32 x;
      s32 y;
      s32 z;
      s32 screenY;
      s32 attributes;

      if (position != 0) {
        x = position[0] - viewX;
        y = position[1] + entry->height - originY;
        z = position[2] - viewZ - originY;
        screenY = z - y;
      } else {
        x = entry->x - viewX;
        y = entry->height - originY;
        z = entry->z - viewZ - originY;
        screenY = z - y;
      }
      if (entry->height != entry->base || entry->state == 2)
        attributes = OBJ_PRIORITY(2);
      else
        attributes = OBJ_PRIORITY(3);
      x = FIXEDtoINT(x) - 8;
      screenY = FIXEDtoINT(screenY) - 8;
      depth = FIXEDtoINT(y + z) + WATER_SPRITE_DEPTH;
      if ((u32)(x + 15) <= 270) {
        s32 rise;

        if ((u32)(screenY + 47) <= 238) {
          x &= 511;
          screenY &= 255;
          rise = entry->kind * PILLAR_TOP_TILES;
          entry->top[0] = 0;
          screenY |= OBJ_X(x);
          screenY |= OBJ_SIZE_16X16 | OBJ_256_COLORS;
          entry->top[1] = screenY;
          entry->top[2] = (tile + rise) | attributes;
          Oam_AddSpriteAtDepthClamped(entry->top, depth);
        }
        screenY = z - y;
        screenY = FIXEDtoINT(screenY) + 8;
        if ((u32)(screenY + 47) <= 238) {
          rise = FIXEDtoINT(entry->height - entry->base);
          if (rise != 0) {
            u32 *column;
            rise--;
            screenY &= 255;
            column = entry->column;
            if ((u32)rise > PILLAR_COLUMN_PIECES - 1)
              rise = PILLAR_COLUMN_PIECES - 1;
            rise = rise * PILLAR_COLUMN_TILES + SPOUT_KINDS * PILLAR_TOP_TILES;
            column[0] = 0;
            screenY |= OBJ_X(x);
            screenY |= OBJ_SIZE_16X8 | OBJ_256_COLORS;
            entry->column[1] = screenY;
            entry->column[2] = attributes | (tile + rise);
            Oam_AddSpriteAtDepthClamped(column, depth);
          }
        }
      }
    }
    entry++;
  }
}

/// Clears the water pillars, loads their tiles into VRAM and starts their
/// drawing.
void AquaRockInside_StartPillars(void) {
  void *buffer;
  s32 priority;

  Iwram_ClearWordsEntry(&gAquaRockInside_Pillars, sizeof(struct PillarWork));
  buffer = MemAlloc_(3200);
  Decompress_TaggedLz(gAquaRockInside_PillarTiles, buffer);
  gAquaRockInside_Pillars.vramEntry = VramCache_FindFreeEntry();
  Vram_Load(gAquaRockInside_Pillars.vramEntry, 3200, buffer);
  Heap_Release(buffer);
  priority = TASK_PASS_FRAME - 100;
  FieldTask_AddFrame(AquaRockInside_DrawPillars, priority);
}

/// Adds a water spout on every map cell of the given layer whose spout kind
/// lies in [first, first + SPOUT_KINDS), at the given heights, and gives those
/// cells MAP_SHAPE_MARKED, its entry set to `level`.
void AquaRockInside_AddSpouts(u32 first, s32 level, s32 baseLevel) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *layer = map->layers;
  struct MapCell *cells = layer->cells;
  s32 originX = layer->offsetX + map->minX;
  s32 originZ = layer->offsetY + map->minZ;
  struct PillarWork *work = &gAquaRockInside_Pillars;
  s32 width = FIELD_CELL(map->maxX - map->minX);
  s32 depth = FIELD_CELL(map->maxZ - map->minZ);
  struct PillarEntry *spout = &work->entries[work->count];
  s32 corner = (FIELD_CELL(map->minZ) << 7) + FIELD_CELL(map->minX);
  s16 row;

  *(s32 *)(gMapGridShapes + MAP_SHAPE_MARKED * 4) = MAP_SHAPE(0, level, 0, 0);
  cells += corner;
  row = 0;
  if (row < depth) {
    struct MapCell *cell;
    s16 column;
    FIXED nextRow;

    do {
      cell = cells + ((u16)row << 7);
      column = 0;
      if (column < width) {
        s32 nextColumn;

        do {
          {
            u32 kind = cell->collisionCode;

            if (kind != 0 && kind >= first && kind < first + SPOUT_KINDS) {
              spout->x = FIELD_CELL_ORIGIN((u16)column) + originX + toFIXED(8);
              spout->z = FIELD_CELL_ORIGIN((u16)row) + originZ + toFIXED(8);
              spout->base = baseLevel * toFIXED(8);
              spout->height = level * toFIXED(8);
              spout->kind = kind - first;
              spout->position = 0;
              spout->state = 1;
              spout++;
              work->count++;
              {
                u8 *terrain = map->layers[0].terrainIds + corner;

                terrain[(u16)column + ((u16)row << 7)] = MAP_SHAPE_MARKED;
              }
            }
          }
          nextColumn = INTtoFIXED(column) + toFIXED(1);
          column = FIXEDtoINT(nextColumn);
          cell++;
        } while ((s32)FIXEDtoINT((u32)nextColumn) < width);
      }
      nextRow = INTtoFIXED(row) + toFIXED(1);
      row = FIXEDtoINT(nextRow);
    } while ((s32)FIXEDtoINT((u32)nextRow) < depth);
  }
}

/// Adds a water pillar on actor `id`, of `kind`, risen to `height` steps.
void AquaRockInside_AddPillar(s32 id, s32 kind, s32 height) {
  struct Actor *actor = Actor_Get(id);
  struct PillarWork *work = &gAquaRockInside_Pillars;
  struct PillarEntry *entry = &work->entries[work->count];

  if (actor != 0) {
    entry->x = 0;
    entry->z = 0;
    entry->base = height * toFIXED(8);
    entry->height = height * toFIXED(8);
    entry->state = 2;
    entry->kind = kind;
    entry->position = &actor->x.fixed;
    work->count++;
  }
}

/// Raises every water pillar to `height`, MAP_SHAPE_MARKED taking it too.
void AquaRockInside_SetPillarsHeight(s32 height) {
  struct PillarWork *work = &gAquaRockInside_Pillars;
  struct PillarEntry *entry;
  s32 i;

  *(s32 *)(Ram_MapCollision + MAP_SHAPE_MARKED * 4) =
      MAP_SHAPE(0, (height / toFIXED(8)) & 0xff, 0, 0);
  entry = work->entries;
  for (i = 0; i < PILLAR_ENTRIES; i++) {
    if (entry->x != 0)
      entry->height = height;
    entry++;
  }
}

/// Raises water pillar `index` alone to `height`, its cell taking
/// MAP_SHAPE_RAISED_ALONE at that height.
void AquaRockInside_SetPillarHeight(u32 index, s32 height) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct PillarWork *work = &gAquaRockInside_Pillars;

  if (index < PILLAR_ENTRIES) {
    struct PillarEntry *entry = &work->entries[index];

    if (entry->x != 0) {
      *(s32 *)(Ram_MapCollision + MAP_SHAPE_RAISED_ALONE * 4) =
          MAP_SHAPE(0, (height / toFIXED(8)) & 0xff, 0, 0);
      {
        u8 *ids = map->layers[0].terrainIds;
        s32 at = FIELD_CELL(entry->x) + FIELD_CELL(entry->z) * MAPGRID_SIZE;
        s32 v = MAP_SHAPE_RAISED_ALONE;
        ids[at] = v;
      }
      entry->height = height;
    }
  }
}

/// Draws thirty-two sprites that each shake by a random few pixels a frame.
void AquaRockInside_DrawSplashes(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 *view = &map->viewX;
  struct BeamWork *splashes = &gAquaRockInside_Splashes;
  s32 viewX = view[0];
  s32 viewZ;
  s32 originY;
  struct Splash *splash;
  s32 i;
  u32 tile;
  s32 priority = OBJ_PRIORITY(2);

  viewX &= FIELD_PIXEL_MASK;
  viewZ = view[1] & FIELD_PIXEL_MASK;
  originY = map->origin[1];
  tile = gVramCacheEntries[splashes->vramEntry].offset >> 5;
  splash = splashes->splashes;
  i = SPLASH_ENTRIES - 1;

  do {
    if (splash->active != 0) {
      s32 x = splash->x - viewX;
      s32 y = splash->y - originY;
      s32 z = splash->z;
      s32 screenY, rawX, rawY;
      z -= viewZ;
      z -= originY;
      screenY = z - y;
      rawX = FIXEDtoINT(x) + (Random_Below(10));

      x = rawX - 10;
      rawY = FIXEDtoINT(screenY) + (Random_Below(10));
      screenY = rawY - 10;
      z = FIXEDtoINT(y + z) + WATER_SPRITE_DEPTH;

      if ((u32)(x + 15) <= 270 && (u32)(screenY + 47) <= 238) {
        u32 *frame = &splash->sprite[2];
        s32 column = x & 511;

        screenY &= 255;
        splash->sprite[0] = 0;
        splash->sprite[1] =
            screenY | OBJ_X(column) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
        *frame = (tile + ((Random_Below(3)) << 3)) | priority;
        Oam_AddSpriteAtDepthClamped(splash->sprite, z);
      }
    }
    i--;
    splash++;
  } while (i >= 0);
}

/// Clears the streams' splashes, loads their tiles into VRAM and starts their
/// drawing.
void AquaRockInside_StartSplashes(void) {
  void *buffer;
  s32 priority;

  Iwram_ClearWordsEntry(&gAquaRockInside_Splashes, sizeof(struct BeamWork));
  buffer = MemAlloc_(1024);
  Decompress_TaggedLz(gAquaRockInside_SplashTiles, buffer);
  gAquaRockInside_Splashes.vramEntry = VramCache_FindFreeEntry();
  Vram_Load(gAquaRockInside_Splashes.vramEntry, 1024, buffer);
  Heap_Release(buffer);
  priority = TASK_PASS_FRAME - 100;
  FieldTask_AddFrame(AquaRockInside_DrawSplashes, priority);
  Event_PlaySound(SE_SHIMMER);
}

/// Starts a stream from cell (x, z), at height y, running in direction `kind`;
/// the second call starts the second stream.
s32 AquaRockInside_AddSplash(s32 x, s32 y, s32 z, s32 kind) {
  struct BeamWork *work = &gAquaRockInside_Splashes;
  struct BeamEntry *entry = work->entries;

  if (entry->x != 0)
    entry++;
  SET_POSITION(entry, FIELD_CELL_CENTER(x), FIELD_CELL_ORIGIN(y),
               FIELD_CELL_CENTER(z));
  entry->direction = kind;
}

extern s8 gAquaRockInside_StreamTurns[];

/// Advances Aqua Rock's two water streams one cell along the map: each stream
/// follows the turn its cell's kind gives, leaves a splash on every open cell
/// it crosses, stops on a wall kind and counts the cells of the given kind it
/// reaches; then it clears the splashes past the new count and opens
/// the cells under those left.
s32 AquaRockInside_AdvanceStreams(s32 first, s32 target) {
  s32 reached = 0;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct BeamWork *state = &gAquaRockInside_Splashes;
  s32 count = state->count;
  s32 turn;
  s32 again = 0;
  s32 i;
  struct BeamEntry *stream = state->entries;
  struct Splash *splash;

  for (i = 0; i < 2; i++) {
    if (stream->x != 0) {
      splash = &state->splashes[state->count];
      do {
        struct MapCell *cells = map->layers[0].cells;
        struct MapCell *cell = &cells[FIELD_CELL(stream->x) +
                                      FIELD_CELL(stream->z) * MAPGRID_SIZE];
        s32 kind = cell->collisionCode;

        if (kind == COLLISION_STREAM_OPEN) {
          splash->x = stream->x;
          splash->z = stream->z;
          splash->y = stream->y;
          splash->active = 1;
          splash++;
          state->count++;
          turn = 0;
          again = 1;
        } else if (kind >= first && kind <= first + 7) {
          turn = gAquaRockInside_StreamTurns[(stream->direction << 3) + kind -
                                             first];
          if (turn != STREAM_STOP)
            count = state->count;
          again = 0;
        } else if (kind == target) {
          turn = STREAM_STOP;
          count = state->count;
          reached++;
          again = 0;
        } else if (kind == 0) {
          turn = STREAM_STOP;
          again = 0;
        }
        if (turn != STREAM_STOP) {
          // The unsigned % 4: as & 3 the mask narrows to a halfword and pools.
          stream->direction = (u32)(stream->direction + turn) % 4;
          FieldVector_AddPolarOffset(16, stream->direction * DEGtoANG(90),
                                     &stream->x);
        }
      } while (again != 0);
      state->count = count;
    }
    stream++;
  }
  splash = &state->splashes[count];
  if (count < SPLASH_ENTRIES) {
    i = SPLASH_ENTRIES - count;
    do {
      if (splash->active != 0)
        splash->active = 0;
      splash++;
    } while (--i != 0);
  }
  splash = state->splashes;
  if (count != 0) {
    i = count;
    do {
      if (splash->active != 0) {
        struct MapCell *cells = map->layers[0].cells;
        struct MapCell *cell = &cells[FIELD_CELL(splash->x) +
                                      FIELD_CELL(splash->z) * MAPGRID_SIZE];

        cell->collisionCode = 0;
        cell->attributeB |= MAP_CELL_JUMP;
      }
      splash++;
    } while (--i != 0);
  }
  return reached;
}
#undef SET_BG1CNT_PRIORITY

#include "../field/actor_vent_bounce.c"

#include "shared/actor_start_scripted.h"

/// A puff of mist from a vent at the position, rising from `base`.
void Vent_Puff(s32 *position, s32 base) {
  struct Actor *actor = Import_ActorCreate_(
      SPRITE_DUST_CLOUD, position[0], position[1], position[2] - toFIXED(2));
  s32 angle;

  if (actor != 0) {
    actor->bounce = Trig_Cos(((u32)(Random16() * (FIXED_ONE * 3 / 16)) >> 16) +
                             (FIXED_ONE * 3 / 16));
    actor->gravity = (u16)Random16();
    actor->riseBaseY = (u16)(Random16() << 1) + base;
    angle = (u16)Random16() - DEGtoANG(180);
    actor->timer = angle;
    Actor_StartScripted(actor, 3);
    actor->update = (ObjectUpdate)Vent_Bounce;
    Actor_SetBlendMode(actor, 1);
  }
}

#include "shared/vent_update.h"

#include "shared/vents_start.h"

/// Unused: whether vent index is blowing now.
UNUSED s32 Vent_IsBlowing(s32 index) {
  struct VentWork *work;

  work = VENT_WORK;
  return VENT_AT(work, index)->entries[0].active;
}

/// Starts or stops vent index's gust.
s32 Vent_SetBlowing(s32 index, s32 blowing) {
  struct VentWork *work;

  work = VENT_WORK;
  VENT_AT(work, index)->entries[0].active = blowing;
}

/// The vents' frame counter, which their cycles time their gusts by.
s32 Vent_GetCounter(void) { return VENT_WORK->counter; }

#include "shared/vent_advance_counter.h"

/// Unused: sets the speed of vent index's gust, in 16.16 pixels a frame.
UNUSED void Vent_SetGust(s32 index, s32 value) {
  struct VentWork *work;

  work = VENT_WORK;
  VENT_AT(work, index)->entries[0].value = value;
}

#include "shared/vent_set_id.h"

// clang-format off
#include "shared/field_effect_duration_scripts.c"

/// Where each of the great pool's pictures lies in the map, x then z.
u8 gAquaRockInside_SpringLevelCells[SPRING_LEVELS * 2] = {67, 66, 75, 66, 83, 66, 91, 66, 99, 66, 107, 66, 115, 66, 67, 81, 75, 81, 83, 81, 91, 81, 99, 81, 107, 81, 115, 81, 67, 96, 75, 96};

/// A spark's script.
u32 gAquaRockInside_SparkScript[] = {
    SC_WAIT(6),
    SC_RELEASE,
    SC_END,
};

/// A glow rising from the Parch tablet.
u32 gAquaRockInside_TabletGlowScript[] = {
    SC_MOVE_BY(0, 160, 0),
    SC_SYNC,
    SC_RELEASE,
    SC_END,
};

/// A spark spiralling in to Piers, leaving a trail of small ones.
s32 gAquaRockInside_SpiralSparkScript[] = {
    SC_CALL(Orbiter_Spiral),
    SC_RELEASE,
    SC_END,
};

/// A glowing spark swirling round Piers.
s32 gAquaRockInside_SwirlingSparkScript[] = {
    SC_CALL(Orbit_Update),
    SC_CALL(Actor_AimAtWatched),
    SC_CALL(Actor_StepToWatched),
    SC_RELEASE,
    SC_END,
};

/// The Parch tablet rising.
u32 gAquaRockInside_TabletRiseScript[] = {
    SC_MOVE_BY(0, 24, 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 4),
    SC_END,
};

/// Piers rising as he learns Parch.
u32 gAquaRockInside_PiersRiseScript[] = {
    SC_MOVE_BY(0, 16, 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 4),
    SC_END,
};

/// The Parch tablet settling.
s32 gAquaRockInside_TabletSettleScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_MOVE_BY(0, -24, 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 0),
    SC_END,
};

/// Piers settling after he learns Parch.
s32 gAquaRockInside_PiersSettleScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_MOVE_BY(0, -16, 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 3),
    SC_END,
};

/// A splash's script at the foot of the waterfall.
u32 gAquaRockInside_SplashScript[] = {
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_RELEASE,
};

/// The Crystal Powder chest's room's vents: x, z and the speed of each gust,
/// ended by 0; and the pillar their puffs land on.
u16 gAquaRockInside_CrystalPowderChestVents[] = {
    280, 344, 2, 312, 344, 2, 0,
};
u16 gAquaRockInside_CrystalPowderChestVentPillars[] = {
    ACTOR_CRYSTAL_POWDER_CHEST_PILLAR, 0,
};

/// The Vial chest's room's vents, and the two pillars their puffs land on.
u16 gAquaRockInside_VialChestVents[] = {
    152, 536, 2, 216, 536, 2, 0,
};
u16 gAquaRockInside_VialChestVentPillars[] ALIGNED(4) = {
    ACTOR_VIAL_CHEST_WEST_VENT_PILLAR, ACTOR_VIAL_CHEST_EAST_VENT_PILLAR, 0, 0,
};

/// A splash's script: it shows for 20 frames, then is released.
u32 gWadingSplash_Script[] = {
    SC_WAIT(20),
    SC_RELEASE,
};

u32 gWaterDropScript[] = {
    SC_ADD(SW_Y, toFIXED(0.25)),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_RELEASE,
};
// clang-format on

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY
#undef DEFINE_AQUA_ROCK_INSIDE_READ_TABLET
#undef SET_BG1CNT_PRIORITY

// clang-format off
/// A vent's puff's rise.
LATE_DATA u32 gTaopoSwamp_PuffRises[] = {
    0, 44, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 320, 0, 320, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(AquaRockInsideWaterOfLifeChest),
    SCENE_EXIT(AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_TO_AQUA_ROCK_INSIDE_TIGHTROPE, AquaRockInsideTightrope, AQUA_ROCK_INSIDE_TIGHTROPE_FROM_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST),
    SCENE_EXIT(2, AquaRockInsideWaterOfLifeChest, 3),
    SCENE_EXIT(3, AquaRockInsideWaterOfLifeChest, 2),
    SCENE_EXIT(4, AquaRockInsideWaterOfLifeChest, 5),
    SCENE_EXIT(5, AquaRockInsideWaterOfLifeChest, 4),
    SCENE_EXIT(6, AquaRockInsideWaterOfLifeChest, 7),
    SCENE_EXIT(7, AquaRockInsideWaterOfLifeChest, 6),
    SCENE_EXIT(8, AquaRockInsideWaterOfLifeChest, 9),
    SCENE_EXIT(9, AquaRockInsideWaterOfLifeChest, 8),
    SCENE_EXIT(AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST, AquaRockInsideRustySwordChest, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE, AquaRockInsideAquariusStone, AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST),
    SCENE_EXITS(AquaRockInsideSteam),
    SCENE_EXIT(AQUA_ROCK_INSIDE_STEAM_TO_AQUA_ROCK_INSIDE_PSYNERGY_STONE, AquaRockInsidePsynergyStone, AQUA_ROCK_INSIDE_PSYNERGY_STONE_FROM_AQUA_ROCK_INSIDE_STEAM),
    SCENE_EXIT(2, AquaRockInsideSteam, 3),
    SCENE_EXIT(3, AquaRockInsideSteam, 2),
    SCENE_EXIT(AQUA_ROCK_INSIDE_STEAM_TO_AQUA_ROCK_INSIDE_TIGHTROPE, AquaRockInsideTightrope, AQUA_ROCK_INSIDE_TIGHTROPE_FROM_AQUA_ROCK_INSIDE_STEAM),
    SCENE_EXIT(AQUA_ROCK_INSIDE_STEAM_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE_NORTH_A, AquaRockInsideAquariusStone, AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_STEAM_NORTH_A),
    SCENE_EXIT(AQUA_ROCK_INSIDE_STEAM_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE_NORTH_B, AquaRockInsideAquariusStone, AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_STEAM_NORTH_B),
    SCENE_EXIT(7, AquaRockInsideSteam, 8),
    SCENE_EXIT(8, AquaRockInsideSteam, 7),
    SCENE_EXIT(AQUA_ROCK_INSIDE_STEAM_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE_SOUTH, AquaRockInsideAquariusStone, AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_STEAM_SOUTH),
    SCENE_EXITS(AquaRockInsideAquariusStone),
    SCENE_EXIT(AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST, AquaRockInsideWaterOfLifeChest, AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_FROM_AQUA_ROCK_INSIDE_AQUARIUS_STONE),
    SCENE_EXIT(AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_STEAM_NORTH_A, AquaRockInsideSteam, AQUA_ROCK_INSIDE_STEAM_FROM_AQUA_ROCK_INSIDE_AQUARIUS_STONE_NORTH_A),
    SCENE_EXIT(AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_PARCH_TABLET, AquaRockInsideParchTablet, AQUA_ROCK_INSIDE_PARCH_TABLET_FROM_AQUA_ROCK_INSIDE_AQUARIUS_STONE),
    SCENE_EXIT(AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_WEST, AquaRockInsideRustySwordChest, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_INSIDE_AQUARIUS_STONE_WEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_EAST, AquaRockInsideRustySwordChest, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_INSIDE_AQUARIUS_STONE_EAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_STEAM_SOUTH, AquaRockInsideSteam, AQUA_ROCK_INSIDE_STEAM_FROM_AQUA_ROCK_INSIDE_AQUARIUS_STONE_SOUTH),
    SCENE_EXIT(AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_PSYNERGY_STONE, AquaRockInsidePsynergyStone, AQUA_ROCK_INSIDE_PSYNERGY_STONE_FROM_AQUA_ROCK_INSIDE_AQUARIUS_STONE),
    SCENE_EXIT(AQUA_ROCK_INSIDE_AQUARIUS_STONE_TO_AQUA_ROCK_INSIDE_STEAM_NORTH_B, AquaRockInsideSteam, AQUA_ROCK_INSIDE_STEAM_FROM_AQUA_ROCK_INSIDE_AQUARIUS_STONE_NORTH_B),
    SCENE_EXITS(AquaRockInsideRustySwordChest),
    SCENE_EXIT(AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_MOAI, AquaRockMoai, AQUA_ROCK_MOAI_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST, AquaRockInsideWaterOfLifeChest, AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_TEAR_STONE, AquaRockInsideTearStone, AQUA_ROCK_INSIDE_TEAR_STONE_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE_WEST, AquaRockInsideAquariusStone, AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_WEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE_EAST, AquaRockInsideAquariusStone, AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_EAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST, AquaRockInsideCrystalPowderChest, AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST),
    SCENE_EXIT(7, AquaRockInsideRustySwordChest, 8),
    SCENE_EXIT(8, AquaRockInsideRustySwordChest, 7),
    SCENE_EXIT(AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_TO_AQUA_ROCK_INSIDE_TIGHTROPE, AquaRockInsideTightrope, AQUA_ROCK_INSIDE_TIGHTROPE_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST),
    SCENE_EXITS(AquaRockInsideCrystalPowderChest),
    SCENE_EXIT(AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_TO_AQUA_ROCK_INSIDE_VIAL_CHEST_NORTHWEST, AquaRockInsideVialChest, AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_NORTHWEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_TO_AQUA_ROCK_INSIDE_VIAL_CHEST_NORTH, AquaRockInsideVialChest, AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_NORTH),
    SCENE_EXIT(AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_TO_AQUA_ROCK_INSIDE_VIAL_CHEST_EAST, AquaRockInsideVialChest, AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_EAST),
    SCENE_EXIT(4, AquaRockInsideCrystalPowderChest, 5),
    SCENE_EXIT(5, AquaRockInsideCrystalPowderChest, 4),
    SCENE_EXIT(AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST, AquaRockInsideRustySwordChest, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_TO_AQUA_ROCK_INSIDE_VIAL_CHEST_SOUTH, AquaRockInsideVialChest, AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_SOUTH),
    SCENE_EXITS(AquaRockInsideVialChest),
    SCENE_EXIT(AQUA_ROCK_INSIDE_VIAL_CHEST_TO_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_NORTHWEST, AquaRockInsideCrystalPowderChest, AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_FROM_AQUA_ROCK_INSIDE_VIAL_CHEST_NORTHWEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_VIAL_CHEST_TO_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_NORTH, AquaRockInsideCrystalPowderChest, AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_FROM_AQUA_ROCK_INSIDE_VIAL_CHEST_NORTH),
    SCENE_EXIT(AQUA_ROCK_INSIDE_VIAL_CHEST_TO_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_EAST, AquaRockInsideCrystalPowderChest, AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_FROM_AQUA_ROCK_INSIDE_VIAL_CHEST_EAST),
    SCENE_EXIT(4, AquaRockInsideVialChest, 5),
    SCENE_EXIT(5, AquaRockInsideVialChest, 4),
    SCENE_EXIT(6, AquaRockInsideVialChest, 7),
    SCENE_EXIT(7, AquaRockInsideVialChest, 6),
    SCENE_EXIT(8, AquaRockInsideVialChest, ENTRANCE_VIAL_CHEST_WEST_DOORWAY),
    SCENE_EXIT(9, AquaRockInsideVialChest, 8),
    SCENE_EXIT(10, AquaRockInsideVialChest, ENTRANCE_VIAL_CHEST_EAST_DOORWAY),
    SCENE_EXIT(11, AquaRockInsideVialChest, 10),
    SCENE_EXIT(AQUA_ROCK_INSIDE_VIAL_CHEST_TO_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_SOUTH, AquaRockInsideCrystalPowderChest, AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST_FROM_AQUA_ROCK_INSIDE_VIAL_CHEST_SOUTH),
    SCENE_EXIT(AQUA_ROCK_INSIDE_VIAL_CHEST_TO_AQUA_ROCK_MIST_SABRE_CHEST, AquaRockMistSabreChest, AQUA_ROCK_MIST_SABRE_CHEST_FROM_AQUA_ROCK_INSIDE_VIAL_CHEST),
    SCENE_EXITS(AquaRockInsidePsynergyStone),
    SCENE_EXIT(AQUA_ROCK_INSIDE_PSYNERGY_STONE_TO_AQUA_ROCK_INSIDE_STEAM, AquaRockInsideSteam, AQUA_ROCK_INSIDE_STEAM_FROM_AQUA_ROCK_INSIDE_PSYNERGY_STONE),
    SCENE_EXIT(AQUA_ROCK_INSIDE_PSYNERGY_STONE_TO_AQUA_ROCK_INSIDE_TIGHTROPE_EAST, AquaRockInsideTightrope, AQUA_ROCK_INSIDE_TIGHTROPE_FROM_AQUA_ROCK_INSIDE_PSYNERGY_STONE_EAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_PSYNERGY_STONE_TO_AQUA_ROCK_INSIDE_TIGHTROPE_WEST, AquaRockInsideTightrope, AQUA_ROCK_INSIDE_TIGHTROPE_FROM_AQUA_ROCK_INSIDE_PSYNERGY_STONE_WEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_PSYNERGY_STONE_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE, AquaRockInsideAquariusStone, AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_PSYNERGY_STONE),
    SCENE_EXITS(AquaRockInsideTightrope),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST, AquaRockInsideWaterOfLifeChest, AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST_FROM_AQUA_ROCK_INSIDE_TIGHTROPE),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_PSYNERGY_STONE_WEST, AquaRockInsidePsynergyStone, AQUA_ROCK_INSIDE_PSYNERGY_STONE_FROM_AQUA_ROCK_INSIDE_TIGHTROPE_WEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_PSYNERGY_STONE_EAST, AquaRockInsidePsynergyStone, AQUA_ROCK_INSIDE_PSYNERGY_STONE_FROM_AQUA_ROCK_INSIDE_TIGHTROPE_EAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST, AquaRockInsideRustySwordChest, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_INSIDE_TIGHTROPE),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_STEAM, AquaRockInsideSteam, AQUA_ROCK_INSIDE_STEAM_FROM_AQUA_ROCK_INSIDE_TIGHTROPE),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_TEAR_STONE_EAST, AquaRockInsideTearStone, AQUA_ROCK_INSIDE_TEAR_STONE_FROM_AQUA_ROCK_INSIDE_TIGHTROPE_EAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_TEAR_STONE_NORTHEAST, AquaRockInsideTearStone, AQUA_ROCK_INSIDE_TEAR_STONE_FROM_AQUA_ROCK_INSIDE_TIGHTROPE_NORTHEAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TIGHTROPE_TO_AQUA_ROCK_INSIDE_TEAR_STONE_WEST, AquaRockInsideTearStone, AQUA_ROCK_INSIDE_TEAR_STONE_FROM_AQUA_ROCK_INSIDE_TIGHTROPE_WEST),
    SCENE_EXITS(AquaRockInsideTearStone),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TEAR_STONE_TO_AQUA_ROCK_INSIDE_TIGHTROPE_WEST, AquaRockInsideTightrope, AQUA_ROCK_INSIDE_TIGHTROPE_FROM_AQUA_ROCK_INSIDE_TEAR_STONE_WEST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TEAR_STONE_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST, AquaRockInsideRustySwordChest, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_INSIDE_TEAR_STONE),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TEAR_STONE_TO_AQUA_ROCK_INSIDE_TIGHTROPE_NORTHEAST, AquaRockInsideTightrope, AQUA_ROCK_INSIDE_TIGHTROPE_FROM_AQUA_ROCK_INSIDE_TEAR_STONE_NORTHEAST),
    SCENE_EXIT(AQUA_ROCK_INSIDE_TEAR_STONE_TO_AQUA_ROCK_INSIDE_TIGHTROPE_EAST, AquaRockInsideTightrope, AQUA_ROCK_INSIDE_TIGHTROPE_FROM_AQUA_ROCK_INSIDE_TEAR_STONE_EAST),
    SCENE_EXITS(AquaRockInsideParchTablet),
    SCENE_EXIT(AQUA_ROCK_INSIDE_PARCH_TABLET_TO_AQUA_ROCK_INSIDE_AQUARIUS_STONE, AquaRockInsideAquariusStone, AQUA_ROCK_INSIDE_AQUARIUS_STONE_FROM_AQUA_ROCK_INSIDE_PARCH_TABLET),
    SCENE_EXITS_END,
};

/// The scene's other rooms' actors.
LATE_DATA struct ActorPlacement gAquaRockInside_Placements[] = {
    {SCENE_TABLE_END},
};

/// The Water of Life chest's room's actors.
LATE_DATA struct ActorPlacement gAquaRockInside_WaterOfLifeChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 679, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 551, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EMBER_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// Steam's room's actors, in AquaRockInsideSteamActorId's order.
LATE_DATA struct ActorPlacement gAquaRockInside_SteamPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 247, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_STEAM), MOVEMENT_TYPE_STAND, 776, 0, 264, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The Aquarius Stone's room's actors.
LATE_DATA struct ActorPlacement gAquaRockInside_AquariusStonePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WATER_DROP_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The Rusty Sword chest's room's actors.
LATE_DATA struct ActorPlacement gAquaRockInside_RustySwordChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WATER_DROP_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 647, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 87, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 103, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

/// The Crystal Powder chest's room's actors.
LATE_DATA struct ActorPlacement gAquaRockInside_CrystalPowderChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WATER_DROP_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The Vial chest's room's actors.
LATE_DATA struct ActorPlacement gAquaRockInside_VialChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 631, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The Psynergy Stone's room's actors.
LATE_DATA struct ActorPlacement gAquaRockInside_PsynergyStonePlacements[] = {
    {SCENE_TABLE_END},
};

/// The tightrope's room's actors: the tightrope's posts.
LATE_DATA struct ActorPlacement gAquaRockInside_TightropePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The Tear Stone's room's actors.
LATE_DATA struct ActorPlacement gAquaRockInside_TearStonePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WATER_DROP_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 231, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

/// The Parch tablet's room's actors.
LATE_DATA struct ActorPlacement gAquaRockInside_ParchTabletPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SPRITE_SLATE, CONDITION_ALWAYS, 1, 100, 0, 0, FACING_SOUTH, 2, 2},
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The scene's other rooms' triggers.
LATE_DATA struct Trigger gAquaRockInside_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// The Water of Life chest's room's triggers.
LATE_DATA struct Trigger gAquaRockInside_WaterOfLifeChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 50, CONDITION_ALWAYS, EVENT(AquaRockInside_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 50, CONDITION_ALWAYS, EVENT(AquaRockInside_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 51, CONDITION_ALWAYS, EVENT(AquaRockInside_JumpForward)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WATER_OF_LIFE_CHEST_WEST_PILLAR, FLAG_AQUA_ROCK_WEST_PILLAR_SET, EVENT(AquaRockInside_SetPillarInGap)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WATER_OF_LIFE_CHEST_EAST_PILLAR, FLAG_AQUA_ROCK_EAST_PILLAR_SET, EVENT(AquaRockInside_SetPillarInGap)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AquaRockInside_SetPillarInGap)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_WATER_OF_LIFE_CHEST_EAST_MOAI, FLAG_WATER_OF_LIFE_CHEST_EAST_MOAI_FILLED, EVENT(AquaRockInside_DouseWaterOfLifeChestEastMoai)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_WATER_OF_LIFE_CHEST_WEST_MOAI, FLAG_WATER_OF_LIFE_CHEST_WEST_MOAI_FILLED, EVENT(AquaRockInside_DouseWaterOfLifeChestWestMoai)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WATER_OF_LIFE_CHEST_BLUE_ROCK, CONDITION_ALWAYS, (s32)((u8 *)AquaRockInside_DoNothing + 1)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_WATER_OF_LIFE_CHEST_PUDDLE, FLAG_WATER_OF_LIFE_CHEST_PUDDLE_FROZEN, EVENT(AquaRockInside_FreezePuddle)},
    {TRIGGER_TABLE_END},
};

/// The level of a draining stream (AquaRockInside_DrainStream), 256 when full.
LATE_DATA s32 gAquaRockInside_SpringLevel = 256;

/// Steam's room's triggers.
LATE_DATA struct Trigger gAquaRockInside_SteamTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_STEP, 40, CONDITION_ALWAYS, EVENT(Leader_FlowWithStream)},
    {TRIGGER_TALK, ACTOR_STEAM, CONDITION_ALWAYS, EVENT(AquaRockInside_FightSteam)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STEAM_ROOM_PILLAR, FLAG_AQUA_ROCK_STEAM_ROOM_PILLAR_DROPPED, EVENT(AquaRockInside_PushPillarOffLedge)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AquaRockInside_PushPillarOffLedge)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), TRIGGER_STEAM_ROOM_MOAI, FLAG_STEAM_ROOM_STREAM_DRAINED, EVENT(AquaRockInside_ParchSteamRoomMoai)},
    {TRIGGER_RAISED, TRIGGER_STEAM_ROOM_MOAI, FLAG_STEAM_ROOM_STREAM_DRAINED, EVENT(AquaRockInside_SteamRoomStreamDrains)},
    {TRIGGER_TABLE_END},
};

/// The Aquarius Stone's room's triggers.
LATE_DATA struct Trigger gAquaRockInside_AquariusStoneTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK, 3, CONDITION_ALWAYS, EVENT(AquaRockInside_OpenDoorToTablet)},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {STEP_TRIGGER_FACING(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT(Leader_SinkAway)},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_TALK, ACTOR_AQUARIUS_STONE_BASE, CONDITION_ALWAYS, EVENT(AquaRockInside_CheckColumn)},
    {USE_ITEM_ON_ACTOR(ITEM_AQUARIUS_STONE), ACTOR_AQUARIUS_STONE_BASE, FLAG_AQUA_ROCK_AQUARIUS_STONE_SET, EVENT(AquaRockInside_UseAquariusStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_AQUARIUS_STONE_WATER_STONE, FLAG_AQUA_ROCK_AQUARIUS_STONE_WATER_STONE_DOUSED, EVENT(AquaRockInside_DouseAquariusStoneWaterStone)},
    {TRIGGER_TABLE_END},
};

/// The Rusty Sword chest's room's triggers.
LATE_DATA struct Trigger gAquaRockInside_RustySwordChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {STEP_TRIGGER_WALKING, 55, CONDITION_ALWAYS, EVENT(AquaRockInside_JumpForward)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A, CONDITION_ALWAYS, EVENT(AquaRockInside_SetPillarCellPushable)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AquaRockInside_SetPillarCellPushable)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A, CONDITION_ALWAYS, EVENT(AquaRockInside_SetPillarCellJumpable)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AquaRockInside_SetPillarCellJumpable)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 50, FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_POOL_PARCHED, EVENT(AquaRockInside_ParchPool)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_RUSTY_SWORD_CHEST_WATER_STONE, FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED, EVENT(AquaRockInside_DouseRustySwordChestWaterStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_RUSTY_SWORD_CHEST_MOAI, FLAG_RUSTY_SWORD_CHEST_MOAI_FILLED, EVENT(AquaRockInside_DouseRustySwordChestMoai)},
    {TRIGGER_TABLE_END},
};

/// The Crystal Powder chest's room's triggers.
LATE_DATA struct Trigger gAquaRockInside_CrystalPowderChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, TRIGGER_WEST_VENT_GUST, CONDITION_ALWAYS, EVENT(AquaRockInside_StepIntoVentGust)},
    {TRIGGER_STEP, TRIGGER_EAST_VENT_GUST, CONDITION_ALWAYS, EVENT(AquaRockInside_StepIntoVentGust)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 70, CONDITION_ALWAYS, EVENT(AquaRockInside_JumpForward)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CRYSTAL_POWDER_CHEST_PILLAR, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_CRYSTAL_POWDER_CHEST_WATER_STONE, FLAG_AQUA_ROCK_CRYSTAL_POWDER_WATER_STONE_DOUSED, EVENT(AquaRockInside_DouseCrystalPowderChestWaterStone)},
    {TRIGGER_TABLE_END},
};

/// The Vial chest's room's triggers.
LATE_DATA struct Trigger gAquaRockInside_VialChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 13, CONDITION_ALWAYS, EVENT(AquaRockInside_RideFalls)},
    {TRIGGER_STEP, TRIGGER_WEST_VENT_GUST, CONDITION_ALWAYS, EVENT(AquaRockInside_StepIntoVentGust)},
    {TRIGGER_STEP, TRIGGER_EAST_VENT_GUST, CONDITION_ALWAYS, EVENT(AquaRockInside_StepIntoVentGust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_MOAI_PILLAR, FLAG_VIAL_CHEST_MOAI_BLOCKED, EVENT(AquaRockInside_BlockMoaiWithPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_VIAL_CHEST_PUDDLE_A, FLAG_VIAL_CHEST_PUDDLE_A_FROZEN, EVENT(AquaRockInside_FreezeRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_VIAL_CHEST_PUDDLE_B, FLAG_VIAL_CHEST_PUDDLE_B_FROZEN, EVENT(AquaRockInside_FreezeRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_VIAL_CHEST_PUDDLE_C, FLAG_VIAL_CHEST_PUDDLE_C_FROZEN, EVENT(AquaRockInside_FreezeRoomPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_WEST_VENT_PILLAR, CONDITION_ALWAYS, EVENT(AquaRockInside_FreeVentPillars)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_EAST_VENT_PILLAR, CONDITION_ALWAYS, EVENT(AquaRockInside_FreeVentPillars)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AquaRockInside_FreeVentPillars)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_WEST_VENT_PILLAR, CONDITION_ALWAYS, EVENT(AquaRockInside_SettleVentPillars)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_EAST_VENT_PILLAR, CONDITION_ALWAYS, EVENT(AquaRockInside_SettleVentPillars)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AquaRockInside_SettleVentPillars)},
    {TRIGGER_TABLE_END},
};

/// The Psynergy Stone's room's triggers.
LATE_DATA struct Trigger gAquaRockInside_PsynergyStoneTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXAMINE, 30, CONDITION_ALWAYS, EVENT(AquaRockInside_CheckPsynergyStone)},
    {TRIGGER_TABLE_END},
};

/// The tightrope's room's triggers.
LATE_DATA s16 gAquaRockInside_TightropeTriggers[] = {
    49, 0, 1, -1, 1, 0, 49, 0,
    2, -1, 2, 0, 49, 0, 3, -1,
    3, 0, 33, 0, 4, -1, 4, 0,
    49, 0, 5, -1, 5, 0, 1, 0,
    6, -1, 6, 0, 1, 0, 7, -1,
    7, 0, 1, 0, 8, -1, 8, 0,
    -1, -1, 0, 0, 0, 0,
};

/// The Tear Stone's room's triggers.
LATE_DATA struct Trigger gAquaRockInside_TearStoneTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 40, CONDITION_ALWAYS, EVENT(Leader_FlowWithStream)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TEAR_STONE_PILLAR_A, CONDITION_ALWAYS, EVENT(AquaRockInside_MovePuzzlePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TEAR_STONE_WATER_PILLAR_C, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TEAR_STONE_WATER_PILLAR_D, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TEAR_STONE_WATER_PILLAR_F, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_TEAR_STONE_WATER_STONE, FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED, EVENT(AquaRockInside_DouseTearStoneWaterStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_TEAR_STONE_MOAI, FLAG_TEAR_STONE_MOAI_FILLED, EVENT(AquaRockInside_DouseTearStoneMoai)},
    {TRIGGER_TABLE_END},
};

/// The Parch tablet's room's triggers.
LATE_DATA struct Trigger gAquaRockInside_ParchTabletTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 30, CONDITION_ALWAYS, EVENT(AquaRockInside_ReadParchTablet)},
    {EXAMINE_TRIGGER_FACING(FACING_SOUTH), 30, CONDITION_ALWAYS, EVENT(AquaRockInside_WalkToParchTablet)},
    {EXAMINE_TRIGGER_FACING(FACING_WEST), 30, CONDITION_ALWAYS, EVENT(AquaRockInside_ReadTabletFacingWest)},
    {EXAMINE_TRIGGER_FACING(FACING_EAST), 30, CONDITION_ALWAYS, EVENT(AquaRockInside_ReadTabletFacingEast)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 40, IF_FLAG_SET(FLAG_PARCH_TABLET_POOL_LEFT), EVENT(AquaRockInside_ParchLastPool)},
    {TRIGGER_TABLE_END},
};

/// How a stream turns on each bend's cell, by its direction and the cell's kind;
/// 8 stops it.
LATE_DATA s8 gAquaRockInside_StreamTurns[] = {
    8, 8, 0, 8, -1, 8, 1, 8,
    8, 0, 8, -1, 1, 8, 8, 8,
    8, 8, 0, 1, 8, -1, 8, 8,
    8, 0, 8, 8, 8, 1, -1, 8,
};
// clang-format on
