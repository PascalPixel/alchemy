
/// @file
/// Aqua Rock's inside, the island south of the Apojii Islands, in the rooms of
/// this overlay: rapids and falls the party runs against or rides, vents that
/// blow until a pillar is Moved over them (Vent_Puff), Moai that lower
/// the water, puddles to Frost, Water Stones to Douse and a tightrope. At its
/// heart Piers reads the tablet and learns Parch
/// (AquaRockInside_ReadParchTablet); the Mercury Djinni Steam waits in one of
/// its rooms (AquaRockInside_FightSteam).

#include "system/task.h"
#include "types.h"
#include "window/message_argument.h"
#include "item_ids.h"
#include "system/system.h"
#include "system/ram_buffer.h"
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
#include "window/item_icon.h"

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Sprite_StartAnimation(void *animation, s32 value);
u8 *Sprite_AddAnimation(void *animation, s32 id);

#include "field/map_grid.h"

#define DIFF(a, b) ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))

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

struct PillarWork {
  u16 vramEntry;
  u16 count;
  struct PillarEntry entries[32];
};

struct BeamEntry {
  s32 x;
  s32 y;
  s32 z;
  u16 unk0c;
  u16 direction;
};

struct Splash {
  s32 x;
  s32 y;
  s32 z;
  u16 active;
  u16 direction;
  u32 sprite[3];
};

struct BeamWork {
  u16 vramEntry;
  u16 count;
  struct BeamEntry entries[2];
  struct Splash splashes[32];
};

struct VentEntry {
  s16 active;
  s16 timer;
  s32 x;
  s32 y;
  s32 z;
  s32 value;
};

struct VentWork {
  s32 counter;
  s32 (*hook)(void);
  struct VentEntry entries[16];
  u16 ids[4];
};

extern u8 gInventorySnapshot[];
#define VENT_WORK ((struct VentWork *)(gInventorySnapshot + 0x188))

struct VentRow {
  u8 unk00[8];
  s16 active;
  s16 timer;
  s32 x;
  s32 y;
  s32 z;
  s32 value;
};

#define VENT_ROW(index) ((struct VentRow *)((u8 *)VENT_WORK + (index) * 20))

struct FieldSpriteFlags {
  u8 unk00[0x11];
  u8 flag0 : 1;
};

void Actor_StartScript(struct Actor *object, const void *data);
void Map_LoadCellsAndShapes(void);
void MessageArgument_Push(s32 item, s32 mode);
void Field_TryJumpForward(void);
void AquaRockInside_SpawnSplash(struct Actor *object);
s32 Actor_EmitMist(struct Actor *object);

void AquaRockInside_LowerSpring(void);
s32 AquaRockInside_BlockCellUnder(s32 id, s32 value);
void AquaRockInside_SetPillarCellLow(void);
void AquaRockInside_ReadParchTablet(void);
void AquaRockInside_SetMoaiFace(s32 id);
void AquaRockInside_DrawPillars(void);
void AquaRockInside_SetPillarsHeight(s32 value);
void AquaRockInside_DrawSplashes(void);
void AquaRockInside_StartSplashes(void);
s32 AquaRockInside_AddSplash(s32 x, s32 y, s32 z, s32 kind);
s32 AquaRockInside_AdvanceStreams(s32 a, s32 b);
void Vent_Bounce(struct Actor *object);
void Actor_StartScripted(struct Actor *object, s32 priority);
void Vent_Puff(s32 *position, s32 base);
s32 AquaRockInside_SetVentActive(s32 index, s32 value);
s32 AquaRockInside_GetVentCounter(void);
s32 Vents_AdvanceCounter(s32 limit);

extern s32 gAquaRockInside_SpringLevel;
extern struct PillarWork gAquaRockInside_Pillars;
extern struct BeamWork gAquaRockInside_Splashes;
extern u32 gAquaRockInside_SparkScript[];
extern u32 gAquaRockInside_SplashScript[];
extern u32 gAquaRockInside_StreamSplashScript[];
extern u32 gWaterDropScript[];
extern const u8 gAquaRockInside_PillarTiles[];
extern const u8 gAquaRockInside_SplashTiles[];
extern u32 gTaopoSwamp_PuffRises[];

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x, s32 y,
                                                    s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 a, s32 z, s32 b) {
  FieldCamera_MoveWithinMapBounds(x, a, z, b);
}

static __inline__ void *MemAlloc_(s32 size) { return Heap_Alloc(size); }
#include "flags.h"
#include "djinn_ids.h"
#include "field/field_sequence_event.h"
#include "field/field.h"
#include "field/map.h"
#include "types.h"
#include "scene/scene.h"
#include "scene/ground_effects.h"
#include "sound/sound.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
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
#include "macros.h"
#include "shared/tables.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_prepare.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

struct Actor;

struct WaterOutlet {
  u16 state;
  u16 index;
  s32 x;
  s32 y;
  s32 z;
  s32 level;
};

struct WaterWork {
  u32 unk00;
  void (*handler)(void);
  struct WaterOutlet outlets[16];
  u16 levels[4];
};

void AquaRockInside_SetPillarHeight(u32 index, s32 height);

void AquaRockInside_WaitToLand(struct Actor *object);

void Leader_RiseFromWater(void);
void AquaRockInside_FinishPushingRock(s32 a, s32 b);
void AquaRockInside_StartPillars(void);
void AquaRockInside_AddSpouts(u32 first, s32 level, s32 baseLevel);
void AquaRockInside_ShowAquariusStone(void);
void AquaRockInside_AddPillar(s32 id, s32 kind, s32 height);
void AquaRockInside_ParchPool(void);

void AquaRockInside_JumpForward(void);

void AquaRockInside_ParchSpring(void);

void AquaRockInside_SpringDries(void);
void AquaRockInside_UseAquariusStone(void);
void AquaRockInside_CheckColumn(void);

void AquaRockInside_DouseAquariusStoneWaterStone(void);

void AquaRockInside_SetPillarCellHigh(void);

void AquaRockInside_DouseRustySwordChestWaterStone(void);

void AquaRockInside_DouseCrystalPowderChestWaterStone(void);

void AquaRockInside_StartPushingRock(void);
void AquaRockInside_CheckPsynergyStone(void);
void AquaRockInside_DouseTearStoneWaterStone(void);
void AquaRockInside_ParchLastPool(void);
void AquaRockInside_WalkToParchTablet(void);
void AquaRockInside_ReadTabletFacingWest(void);
void AquaRockInside_ReadTabletFacingEast(void);
void AquaRockInside_FlowWithStream(void);

void FieldEffect_StartBlankOverlay();
void Tightrope_Place();

#include "text/msg_ids.h"

/// Aqua Rock's lasting flags. The Water of Life room's vents, covered by its
/// west and east pillars (AquaRockInside_PlacePillarOverVent).
#define FLAG_AQUA_ROCK_WEST_VENT_COVERED 0x95a
#define FLAG_AQUA_ROCK_EAST_VENT_COVERED 0x95b
/// The pillar Moved off the ledge in Steam's room, fallen before the Moai.
#define FLAG_AQUA_ROCK_PILLAR_DROPPED_BY_MOAI 0x95c
/// The Aquarius Stone set in the column, which drains the spring.
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

extern u8 Data_0202c001[];

/// The actors of the inner rooms the set-up places, numbering each room's
/// placement table, named by the Psynergy their triggers answer: a Moai
/// (sprite 380, a stone head) that Douse fills, so that the water rises, or a
/// Water Stone (sprite 383, a tear-shaped rock) that Douse sets spouting, which
/// turns the streams; a stone pillar (SPRITE_STONE_PILLAR) Moved over a vent,
/// and the water pillars (sprite 312) AquaRockInside_AddPillar raises.
enum AquaRockInsideWaterOfLifeChestActorId {
  ACTOR_WATER_OF_LIFE_CHEST_EAST_MOAI = ACTOR_FIRST_PLACED,
  ACTOR_WATER_OF_LIFE_CHEST_WEST_MOAI,
  ACTOR_WATER_OF_LIFE_CHEST_WEST_VENT_PILLAR,
  ACTOR_WATER_OF_LIFE_CHEST_EAST_VENT_PILLAR,
  /// The pillar the water lifts (AquaRockInside_SetPillarHeight).
  ACTOR_WATER_OF_LIFE_CHEST_WATER_PILLAR,
  ACTOR_WATER_OF_LIFE_CHEST_PUDDLE
};

enum AquaRockInsideSteamActorId {
  ACTOR_STEAM_ROOM_MOAI = ACTOR_FIRST_PLACED,
  /// The pillar Moved before the Moai (AquaRockInside_MovePillarBeforeMoai).
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
  /// The pillar Moved to the falls (AquaRockInside_MovePillarByFalls).
  ACTOR_VIAL_CHEST_FALLS_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_VIAL_CHEST_MOAI,
  /// The two pillars pushed into the rapids (AquaRockInside_StepIntoRapids).
  ACTOR_VIAL_CHEST_RAPIDS_PILLAR_A,
  ACTOR_VIAL_CHEST_RAPIDS_PILLAR_B,
  /// A hidden actor that splashes under the falls
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

/// A pillar Moved onto its vent in the Water of Life's room: the west pillar at
/// row 12, the east one at column 51. It stops there a pixel up and covers the
/// vent for good; the west one also redraws the two cells it blocks.
void AquaRockInside_PlacePillarOverVent(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == ACTOR_WATER_OF_LIFE_CHEST_WEST_VENT_PILLAR) {
    s32 row = FIELD_CELL(object->z.fixed);

    if (row == 12) {
      object->motionFlags = 0;
      {
        s32 step = -px(1);

        object->y.fixed += step;
      }
      Event_SetFlag(FLAG_AQUA_ROCK_WEST_VENT_COVERED);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 9, row);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 13, row);
    }
  } else if (id == ACTOR_WATER_OF_LIFE_CHEST_EAST_VENT_PILLAR) {
    if (FIELD_CELL(object->x.fixed) == 51) {
      object->motionFlags = 0;
      {
        s32 step = -px(1);

        object->y.fixed += step;
      }
      Event_SetFlag(FLAG_AQUA_ROCK_EAST_VENT_COVERED);
    }
  }
}

void AquaRockInside_JumpForward(void) { Field_TryJumpForward(); }

void AquaRockInside_DouseWaterOfLifeChestEastMoai(s32 unused, s32 id) {
  struct MapLayer *layer = &Ram_HeapSlots->mapGrid->layers[1];
  s32 i;
  s32 column;
  s32 objectId = id;

  FieldActor_SetSpriteColor(Actor_Get(objectId), 0);
  AquaRockInside_SetMoaiFace(objectId);
  Actor_SetAnimation(objectId, 3);
  layer->offsetY -= px(32);
  Map_LoadCellsAndShapes();
  MapGrid_CenterOnFollowedPoint();
  column =
      FIELD_CELL(Actor_Get(ACTOR_WATER_OF_LIFE_CHEST_WATER_PILLAR)->x.fixed) -
      42;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_556);
  for (i = 0; i <= 31; i++) {
    AquaRockInside_SetPillarsHeight(i << 16);
    {
      /* FAKEMATCH(pin): the zero pinned to r1, set before the column is copied
       * to r0. */
      register s32 height asm("r1") = 0;

      AquaRockInside_SetPillarHeight(column, height);
    }
    layer->offsetY += px(1);
    System_Wait(4);
  }
  if (Event_GetFlag(FLAG_SCENE_TEMP_50))
    MapGrid_CopyCellAttributes(41, 42, 1, 1, 43, 44);
  Event_End();
  Event_SetFlag(FLAG_SCENE_TEMP_30);
}

void AquaRockInside_DouseWaterOfLifeChestWestMoai(s32 unused, s32 id) {
  struct MapLayer *layer = &Ram_HeapSlots->mapGrid->layers[1];
  s32 i;

  FieldActor_SetSpriteColor(Actor_Get(id), 0);
  AquaRockInside_SetMoaiFace(id);
  Actor_SetAnimation(id, 3);
  layer->offsetY -= px(32);
  Map_LoadCellsAndShapes();
  MapGrid_CenterOnFollowedPoint();
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_556);
  for (i = 0; i <= 31; i++) {
    AquaRockInside_SetPillarsHeight(i << 16);
    layer->offsetY += px(1);
    System_Wait(4);
  }
  Event_End();
  Event_SetFlag(FLAG_SCENE_TEMP_31);
}

void AquaRockInside_DoNothing(void) {}

void AquaRockInside_FreezePuddle(void) {
  MapGrid_CopyCellAttributes(41, 42, 1, 1, 43, 44);
  Event_SetFlag(FLAG_SCENE_TEMP_50);
}

#if TLA_PATCH_1
void AquaRockInside_WaitToLand(struct Actor *object) {
  object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  do {
    System_Wait(1);
  } while (object->velocityY != 0);
  Event_PlaySound(SE_THUMP);
  System_Wait(10);
  object->motionFlags = 0;
}
#endif

void AquaRockInside_MovePillarBeforeMoai(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 x;

  if (id == ACTOR_STEAM_ROOM_PILLAR) {
    x = FIELD_CELL(object->x.fixed);
    if (x == 39) {
#if !TLA_PATCH_1
      object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      System_Wait(15);
      object->motionFlags = 0;
      Event_SetFlag(FLAG_AQUA_ROCK_PILLAR_DROPPED_BY_MOAI);
      MapGrid_CopyCellAttributes(41, 9, 1, 1, x, id);
#else
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      AquaRockInside_WaitToLand(object);
      Event_SetFlag(FLAG_AQUA_ROCK_PILLAR_DROPPED_BY_MOAI);
      MapGrid_CopyCellAttributes(41, 9, 1, 1, x, id);
      Event_End();
#endif
    }
  }
}

void AquaRockInside_ParchSpring(void) {
  Actor_SetAnimation(ACTOR_STEAM_ROOM_MOAI, 4);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_STEAM_ROOM_MOAI), 6);
  MapGrid_CopyCells(123, 15, 5, 3, 110, 15);
  gFieldWork->raisedTrigger = 88;
}

void AquaRockInside_LowerSpring(void) {
  s32 count = gAquaRockInside_SpringLevel - 1;
  s32 row;

  gAquaRockInside_SpringLevel = count;
  row = count / 8;
  MapGrid_CopyCells((count & 1) * 5 + 118, 47 - row, 5, 2, 110, 47 - row);
  if (gAquaRockInside_SpringLevel == 0)
    FieldTask_Remove(AquaRockInside_LowerSpring);
}

void AquaRockInside_SpringDries(void) {
  s32 priority;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Event_PlaySound(SE_557);
  priority = 0x480;
  FieldTask_AddFrame(AquaRockInside_LowerSpring, priority);
  FieldCamera_SetSpeedAndAcceleration(2.5, 0.25);
  FieldCamera_MoveTo(776, CAMERA_KEEP, 648, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(30);
  Event_End();
  Event_SetFlag(FLAG_SCENE_TEMP_40);
  MapGrid_CopyCellAttributes(59, 16, 5, 32, 46, 16);
}

void AquaRockInside_ShowAquariusStone(void) {
  struct Actor *object;
  struct FieldSpriteFlags *sprite;

  FieldActor_ShowItemIconById(ACTOR_AQUARIUS_STONE, ITEM_AQUARIUS_STONE);
  // XXX: actor 20 is none of this room's three; a probe of the room after the
  // stone is set would show which sprite this lifts 24 pixels.
  object = Actor_Get(20);
  sprite = object->sprite;
  sprite->flag0 = 1;
  object->motionFlags = 0;
  object->y.fixed = px(24);
}

extern u8 gAquaRockInside_SpringLevelCells[];

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
  priority = 0x480;
  FieldTask_AddFrame(AquaRockInside_LowerSpring, priority);
  FieldCamera_SetSpeedAndAcceleration(2.5, 0.25);
  FieldCamera_MoveTo(312, CAMERA_KEEP, 504, 1);
  Event_PlaySound(SE_SCATTER);
  MapGrid_CopyCellsLayered(75, 96, 7, 13, 80, 27);
  MapGrid_CopyCellsLayered(91, 96, 7, 10, 16, 31);
  MapGrid_CopyCellsLayered(99, 96, 7, 14, 16, 91);
  MapGrid_CopyCellAttributes(43, 0, 5, 9, 17, 31);
  i = 0;
row:
  j = 0;
cell:
  {
    s32 index = (i + (1 & ~j)) * 2;

    MapGrid_CopyCellsLayered(gAquaRockInside_SpringLevelCells[index],
                             gAquaRockInside_SpringLevelCells[index + 1], 7, 13,
                             80, 27);
    System_Wait(1);
  }
  j++;
  if (j <= 6)
    goto cell;
  i++;
  if (i <= 14)
    goto row;
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

void AquaRockInside_StepOnLoosePlank(s32 value) {
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
      Actor_Get(gMainRoutineWork.leaderActorId)->acceleration = px(1) / 2;
  Event_PlaySound(SE_MAP_EXIT);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, 312, 496);
  FieldEvent_Wait(5);
  FieldEvent_SetExitRequest(value);
}

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

void AquaRockInside_DouseAquariusStoneWaterStone(void) {
  s32 i;

  AquaRockInside_StartSplashes();
  AquaRockInside_AddSplash(16, 4, 16, 2);
  AquaRockInside_AddSplash(22, 4, 16, 0);
  for (i = 5; i >= 0; i--) {
    AquaRockInside_AdvanceStreams(33, 55);
    System_Wait(15);
  }
  Event_SetFlag(FLAG_AQUA_ROCK_AQUARIUS_STONE_WATER_STONE_DOUSED);
}

void AquaRockInside_ParchPool(void) {
  MapGrid_CopyCellsLayered(106, 14, 14, 13, 81, 14);
  MapGrid_CopyCellsLayered(44, 78, 14, 12, 17, 78);
  MapGrid_CopyCellAttributes(81, 14, 14, 13, 17, 14);
  Event_SetFlag(FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_POOL_PARCHED);
}

s32 AquaRockInside_BlockCellUnder(s32 id, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *object = Actor_Get(id);

  if (object != 0) {
    struct MapCell *cells = map->layers[0].cells;
    struct MapCell *cell =
        &cells[FIELD_CELL(object->x.fixed) + FIELD_CELL(object->z.fixed) * 128];

    cell->collisionCode = value;
    cell->attributeB |= 0x80;
  }
}

void AquaRockInside_SetPillarCellLow(void) {
  struct Actor *object = Actor_Get(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A);

  MapGrid_CopyCellAttributes(64, 43, 1, 1, FIELD_CELL(object->x.fixed),
                             FIELD_CELL(object->z.fixed));
}

void AquaRockInside_SetPillarCellMiddle(void) {
  struct Actor *object = Actor_Get(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A);

  MapGrid_CopyCellAttributes(64, 42, 1, 1, FIELD_CELL(object->x.fixed),
                             FIELD_CELL(object->z.fixed));
}

void AquaRockInside_SetPillarCellHigh(void) {
  struct Actor *object = Actor_Get(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A);

  MapGrid_CopyCellAttributes(64, 41, 1, 1, FIELD_CELL(object->x.fixed),
                             FIELD_CELL(object->z.fixed));
}

void AquaRockInside_DouseRustySwordChestWaterStone(void) {
  s32 i;
  s32 result;

  if (Event_GetFlag(FLAG_SCENE_TEMP_49)) {
    AquaRockInside_BlockCellUnder(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A, 38);
    AquaRockInside_StartSplashes();
    AquaRockInside_AddSplash(12, 2, 45, 1);
    for (i = 3; i >= 0; i--)
      result = AquaRockInside_AdvanceStreams(33, 55);
    Event_SetFlag(FLAG_SCENE_TEMP_48);
    if (result == 1)
      Event_SetFlag(FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED);
  }
}

void AquaRockInside_DouseRustySwordChestMoai(s32 unused, s32 id) {
  struct MapLayer *layer = &Ram_HeapSlots->mapGrid->layers[1];
  s32 i;

  FieldActor_SetSpriteColor(Actor_Get(ACTOR_RUSTY_SWORD_CHEST_MOAI), 5);
  AquaRockInside_SetMoaiFace(id);
  Actor_SetAnimation(id, 3);
  layer->offsetY -= px(32);
  Map_LoadCellsAndShapes();
  MapGrid_CenterOnFollowedPoint();
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_557);
  for (i = 31; i >= 0; i--) {
    layer->offsetY += px(1);
    System_Wait(4);
  }
  Event_End();
  AquaRockInside_SetPillarCellLow();
  Event_SetFlag(FLAG_SCENE_TEMP_49);
}

void AquaRockInside_JumpDownLedge() {
  Actor_JumpDownLedge(gMainRoutineWork.leaderActorId, 6, 0);
  Event_End();
}

void AquaRockInside_StepIntoRapids(s32 trigger) {
  s32 clear = 1;
  struct Actor *object;
  struct Vector3 target;
  s32 result;

  if (gMainRoutineWork.next == SCENE_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST) {
    s32 column =
        FIELD_CELL(Actor_Get(ACTOR_CRYSTAL_POWDER_CHEST_PILLAR)->x.fixed);

    if (trigger == 77 && column == 17)
      clear = 0;
    if (trigger == 78 && column == 19)
      clear = 0;
  }
  if (gMainRoutineWork.next == SCENE_AQUA_ROCK_INSIDE_VIAL_CHEST) {
    if (trigger == 77 &&
        FIELD_CELL(Actor_Get(ACTOR_VIAL_CHEST_RAPIDS_PILLAR_A)->x.fixed) == 9)
      clear = 0;
    if (trigger == 78 &&
        FIELD_CELL(Actor_Get(ACTOR_VIAL_CHEST_RAPIDS_PILLAR_B)->x.fixed) == 13)
      clear = 0;
  }
  if (clear == 0)
    return;
  object = Actor_Get(gMainRoutineWork.leaderActorId);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  goto test;
step:
  if (result < 0) {
    object->facing = FACING_SOUTH;
    AquaRockInside_JumpDownLedge(trigger);
    return;
  }
  {
    s32 offset, x;

    object->z.fixed += px(2);
    x = object->x.fixed;
    offset = (x & 0xfffff) - 0x80000;
    {
      s32 limit = 0x10001;

      if (offset >= limit)
        offset = 0x10000;
      limit = -0x10001;
      if (offset <= limit)
        offset = -0x10000;
    }
    object->x.fixed = x - offset;
  }
  object->facing += 0x1000;
  System_Wait(1);
test:
  target.x = object->x.fixed;
  target.y = object->y.fixed;
  target.z = object->z.fixed + px(8);
  result = MapGrid_CheckCollision(object, &target);
  if (result <= 0)
    goto step;
}

void AquaRockInside_DouseCrystalPowderChestWaterStone(void) {
  s32 i;

  AquaRockInside_StartSplashes();
  AquaRockInside_AddSplash(44, 0, 34, 2);
  for (i = 4; i >= 0; i--) {
    AquaRockInside_AdvanceStreams(33, 55);
    System_Wait(15);
  }
  Event_SetFlag(FLAG_AQUA_ROCK_CRYSTAL_POWDER_WATER_STONE_DOUSED);
}

void AquaRockInside_RideFalls(s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 speed;
  s32 i;
  u8 *flags;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  flags = &object->motionFlags;
  speed = 0;
  *flags = 0;
  object->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_SelectAnimation(object, 18);
  Event_PlaySound(SE_SPLASH_SMALL);
  System_Wait(13);
  Actor_SetSpriteColorMode(object, 9);
  Actor_SelectAnimation(object, 28);
  Event_PlaySound(SE_566);
  for (i = 0; i <= 59; i++) {
    if (i == 30)
      Event_FinishScreenTransition();
    speed += px(1) / 4;
    object->y.fixed += speed;
    if ((i & 7) == 0)
      AquaRockInside_SpawnSplash(Actor_Get(ACTOR_VIAL_CHEST_PUDDLE_C));
    System_Wait(1);
  }
  Event_End();
  FieldEvent_SetExitRequest(value);
}

void AquaRockInside_MovePillarByFalls(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (FIELD_CELL(object->x.fixed) == 49) {
    struct MapLayer *layer = &Ram_HeapSlots->mapGrid->layers[1];
    s32 i;

    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
    Event_PlaySound(SE_557);
    Actor_SetAnimation(ACTOR_VIAL_CHEST_MOAI, 2);
    MapGrid_CopyCellsLayered(114, 39, 1, 2, 113, 39);
    for (i = 31; i >= 0; i--) {
      layer->offsetY -= px(1);
      System_Wait(4);
    }
    i = ACTOR_VIAL_CHEST_PUDDLE_A;
  place:
    object = Actor_GetById(i);
    object->x.fixed = object->timer << 16;
    object->z.fixed = px(object->userValue);
    object->motionFlags = 0;
    object->groundHeight = -px(32);
    object->y.fixed = -px(32);
    FieldActor_SetSpritePriority(i, 3);
    i++;
    if (i <= ACTOR_VIAL_CHEST_PUDDLE_C)
      goto place;
    BlendScript_Suspend();
    i = 12;
  blend:
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(i, BLEND_WEIGHT_MAX),
                        0x20000);
    System_Wait(4);
    i--;
    if (i >= 0)
      goto blend;
    MapGrid_CopyCellAttributes(105, 41, 12, 13, 41, 41);
    MapGrid_CopyCellsLayered(105, 105, 12, 13, 105, 41);
    BlendScript_Resume();
    Event_End();
    Event_SetFlag(FLAG_SCENE_TEMP_38);
  }
}

void AquaRockInside_FreezeRoomPuddle(s32 unused, s32 id) {
  if ((u32)(id - ACTOR_VIAL_CHEST_PUDDLE_A) <= 2) {
    struct Actor *object = Actor_Get(id);

    MapGrid_CopyCellAttributes(47, 45, 1, 1, FIELD_CELL(object->x.fixed),
                               FIELD_CELL(object->z.fixed));
    Event_SetFlag(FLAG_SCENE_TEMP_51 - ACTOR_VIAL_CHEST_PUDDLE_A + id);
    object->priorityFlags = 0;
  }
}

void AquaRockInside_StartPushingRock(void) {
  MapGrid_CopyCellAttributes(8, 63, 7, 1, 8, 34);
}

/// When either pillar pushed into the Vial's rapids arrives, stops both and
/// redraws row 33 at each one's column, so the rapids flow around them.
void AquaRockInside_FinishPushingRock(s32 unused, s32 id) {
  if ((u32)(id - ACTOR_VIAL_CHEST_RAPIDS_PILLAR_A) <= 1) {
    struct Actor *object = Actor_Get(ACTOR_VIAL_CHEST_RAPIDS_PILLAR_A);

    object->motionFlags = 0;
    {
      s32 row = 33;

      MapGrid_CopyCellAttributes(8, row, 1, 1, FIELD_CELL(object->x.fixed), 34);
    }
    object = Actor_Get(ACTOR_VIAL_CHEST_RAPIDS_PILLAR_B);
    object->motionFlags = 0;
    {
      s32 row = 33;

      MapGrid_CopyCellAttributes(8, row, 1, 1, FIELD_CELL(object->x.fixed), 34);
    }
  }
}

void AquaRockInside_WaitToRise(struct Actor *object) {
  s32 frames = 60;

  while (frames != 0) {
    System_Wait(1);
    frames--;
    if (object->velocityY == 0)
      break;
  }
}

void AquaRockInside_CheckPsynergyStone(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered(MsgCheckedThePsynergy, 1);
  Event_PlaySound(SE_RECOVERY);
  Party_AddPp(999, 0);
  FieldEvent_Wait(10);
  Event_ClearFlag(FLAG_NOTHING_HAPPENS);
  Message_ShowCentered(MsgCheckedThePsynergy + 1, 1);
  Event_End();
}

void AquaRockInside_MovePuzzlePillar(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  MapGrid_CopyCellAttributes(17, 39, 2, 1, 20, 39);
  MapGrid_CopyCellAttributes(23, 39, 1, 1, FIELD_CELL(object->x.fixed), 39);
}

void AquaRockInside_DouseTearStoneWaterStone(void) {
  s32 i;
  s32 result;

  if (Event_GetFlag(FLAG_SCENE_TEMP_4B)) {
    /* FAKEMATCH(signature): these two calls go through a void function type. */
    ((void (*)(s32, s32))AquaRockInside_BlockCellUnder)(
        ACTOR_TEAR_STONE_WATER_PILLAR_C, 38);
    ((void (*)(s32, s32))AquaRockInside_BlockCellUnder)(
        ACTOR_TEAR_STONE_WATER_PILLAR_D, 39);
    AquaRockInside_BlockCellUnder(ACTOR_TEAR_STONE_WATER_PILLAR_F, 37);
    AquaRockInside_StartSplashes();
    AquaRockInside_AddSplash(38, 2, 19, 1);
    for (i = 8; i >= 0; i--)
      result = AquaRockInside_AdvanceStreams(33, 55);
    Event_SetFlag(FLAG_SCENE_TEMP_4A);
    if (result == 1)
      Event_SetFlag(FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED);
  }
}

void AquaRockInside_DouseTearStoneMoai(s32 unused, s32 id) {
  struct MapLayer *layer = &Ram_HeapSlots->mapGrid->layers[1];
  s32 i;

  FieldActor_SetSpriteColor(Actor_Get(id), 5);
  AquaRockInside_SetMoaiFace(id);
  Actor_SetAnimation(id, 3);
  layer->offsetY -= px(32);
  Map_LoadCellsAndShapes();
  MapGrid_CenterOnFollowedPoint();
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_556);
  for (i = 31; i >= 0; i--) {
    layer->offsetY += px(1);
    System_Wait(4);
  }
  Event_End();
  Event_SetFlag(FLAG_SCENE_TEMP_4B);
}

void AquaRockInside_WaterSinks(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapGrid->layers[1];
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  layer->offsetY -= px(32);
  MapGrid_CenterOnFollowedPoint();
  MapGrid_SetShake(1, 1, 1);
  i = 14;
shake:
  {
    s32 column = 77;
    s32 row = 8;

    MapGrid_CopyCellsLayered(70, 8, 7, 6, column, row);
    System_Wait(1);
    MapGrid_CopyCellsLayered(50, 8, 7, 6, column, row);
    System_Wait(1);
  }
  i--;
  if (i >= 0)
    goto shake;
  System_Wait(10);
  for (i = 127; i >= 0; i--) {
    layer->offsetY += px(1) / 4;
    System_Wait(1);
  }
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_End();
  MapGrid_CopyCellAttributes(38, 9, 7, 6, 13, 9);
  Event_SetFlag(FLAG_AREA_TEMP_49);
}

void AquaRockInside_ParchLastPool(void) {
  MapGrid_CopyCellsLayered(70, 8, 7, 6, 77, 8);
  MapGrid_CopyCellAttributes(38, 17, 7, 6, 13, 9);
  Event_ClearFlag(FLAG_AREA_TEMP_49);
}

void AquaRockInside_SpawnSpark(struct Actor *object) {
  struct Actor *child = Import_ActorCreate_(0x2a0, object->x.fixed,
                                            object->y.fixed, object->z.fixed);

  child->scaleY = Q16_ONE * 2 / 5;
  child->scaleX = Q16_ONE * 2 / 5;
  child->motionFlags = 0;
  FieldActor_SetSpriteColor(child, 2);
  Scene_SetActorSpritePriority(child, 1);
  Actor_SelectAnimation(child, 1);
  Actor_SetSpriteFlags(child, 0);
  Actor_SetBlendMode(child, 1);
  Actor_StartScript(child, gAquaRockInside_SparkScript);
}

// Steps an orbiting object around its anchor: turns it by half its speed, puts
// it on the anchor (raised 8 pixels) and pushes it out by its radius; when its
// countdown runs out returns 0, else, unless its radius is down to 2 pixels,
// takes a second, shrinking step. Returns 1 while it runs.

/// Moves an orbiter two steps inward round its anchor.
s32 Orbiter_Spiral(struct Orbiter *orbiter) {
  struct Actor *anchor = orbiter->anchor;

  orbiter->angle += (s32)(orbiter->speed << 16) >> 17;
  orbiter->x = anchor->x.fixed;
  orbiter->y = anchor->y.fixed + px(8);
  orbiter->z = anchor->z.fixed;
  FieldVector_AddPolarOffsetFixed(orbiter->radius, orbiter->angle, &orbiter->x);
  if (--orbiter->countdown == 0)
    return 0;
  if (orbiter->radius == px(2))
    return 1;
  AquaRockInside_SpawnSpark(orbiter);
  orbiter->radius -= px(4);
  orbiter->angle += (s32)(orbiter->speed << 16) >> 17;
  orbiter->x = anchor->x.fixed;
  orbiter->y = anchor->y.fixed + px(8);
  orbiter->z = anchor->z.fixed;
  FieldVector_AddPolarOffsetFixed(orbiter->radius, orbiter->angle, &orbiter->x);
  AquaRockInside_SpawnSpark(orbiter);
  orbiter->radius -= px(4);
  return 1;
}

// Moves around in a circle.
#include "shared/orbit_update.h"

#include "shared/actor_aim_at_watched.h"

#include "shared/actor_step_to_watched.h"

#include "shared/actor_emit_bubbles.h"

extern u32 gAquaRockInside_TabletRiseScript[];
extern u32 gAquaRockInside_TabletGlowScript[];
extern s32 gAquaRockInside_RisingSparkScript[];
extern u32 gAquaRockInside_PiersRiseScript[];
extern s32 gAquaRockInside_RiserScript[];
extern s32 gAquaRockInside_TabletSettleScript[];
extern s32 gAquaRockInside_PiersSettleScript[];

void AquaRockInside_PiersLearnsParch(s32 firstId, s32 secondId) {
  struct Actor *first = Actor_GetById(firstId);
  struct Actor *second = Actor_GetById(secondId);
  struct Actor *object;
  struct Actor *spark;
  struct Actor *riser;
  s32 i;
  u16 noMotion;
  const u8 *script;
  // Unused: gives the stack frame its 12 bytes.
  s32 unused[3];

  script = gAquaRockInside_PiersRiseScript;
  Sound_Play(SOUND_FADE_OUT_BGM);
  Event_Wait(30);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveWithinMapBounds(FIELD_PIXEL(first->x.fixed) << 16,
                                  FIELD_PIXEL(first->y.fixed) << 16,
                                  FIELD_PIXEL(first->z.fixed) << 16, 1);
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(2, 2, 1);
  PaletteFade_SetSource(0x10000, 0);
  PaletteFade_SetTarget(0x10003, 1);
  PaletteFade_StartInterpolation(60);
  first->motionFlags = 0;
  first->acceleration = px(1) / 5;
  first->speedLimit = px(1) / 5;
  Actor_SetMovementTypeById(firstId, gAquaRockInside_TabletRiseScript);
  Sound_Play(SE_TABLET_RISE);
  Event_Wait(45);
  Actor_ConfigureSpriteColor(firstId, 256);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  for (i = 0; i != 16; i++) {
    object = ((void *)Import_ActorCreate(672, first->x.fixed, first->y.fixed,
                                         first->z.fixed));
    Sound_Play(SE_GLITTER);
    object->x.fixed += ((Random16() & 31) - 16) << 16;
    object->y.fixed += ((Random16() & 15) - 8) << 16;
    object->speedLimit = px(8);
    object->acceleration = px(1) / 5;
    object->motionFlags = 0;
    Actor_SetSpriteColor(object, 2);
    Scene_SetActorSpritePriority(object, 1);
    Actor_SelectAnimation(object, 7);
    Actor_SetSpriteFlags(object, 0);
    Actor_SetBlendMode(object, 1);
    Actor_StartScript(object, gAquaRockInside_TabletGlowScript);
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
  for (i = 0; i != 16; i++) {
    spark = FieldActor_Create(SPRITE_PARTICLE, second->x.fixed, second->y.fixed,
                              second->z.fixed);
    Sound_Play(SE_SPARKS);
    spark->timer = (Random16() & 0x7fff) + 0x8000;
    spark->userValue = (((i & 1) << 1) - 1) << ((Random16() & 3) + 9);
    noMotion = 0;
    spark->motionFlags = noMotion;
    spark->riseBaseY = 0x7a0000;
    spark->watchedActor = second;
    spark->riseCounter = ((16 - i) << 3) + 15;
    Actor_SetSpriteColor(spark, 2);
    Scene_SetActorSpritePriority(spark, 1);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    FieldVector_AddPolarOffsetFixed(spark->riseBaseY, spark->timer,
                                    &spark->x.fixed);
    Actor_StartScript(spark, gAquaRockInside_RisingSparkScript);
    if (i == 3) {
      Actor_SetSpriteColorById(secondId, 256);
      second->motionFlags = noMotion;
      second->acceleration = px(1) / 5;
      second->speedLimit = px(1) / 5;
      Actor_SetMovementTypeById(secondId, script);
    }
    wait(8);
  }
  Sound_Play(SE_SHIMMER);
  wait(16);
  for (i = 2; i != 32; i += 2) {
    riser =
        FieldActor_Create(SPRITE_PARTICLE, second->x.fixed,
                          second->y.fixed + i * px(1) - px(8), second->z.fixed);
    riser->timer = Random16();
    riser->userValue = 0x800;
    riser->riseCounter = i;
    riser->riseEnabled = 1;
    riser->bounce = riser->x.fixed;
    riser->riseBaseY = 0;
    riser->gravity = riser->z.fixed;
    riser->watchedActor = second;
    riser->motionFlags = (u16)0;
    riser->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetSpriteColor(riser, 2);
    Actor_SelectAnimation(riser, 7);
    Actor_SetSpriteFlags(riser, 0);
    Actor_SetBlendMode(riser, 1);
    Actor_StartScript(riser, gAquaRockInside_RiserScript);
  }
  Sound_Play(SE_SHIMMER);
  wait(50);
  PaletteFade_SetTarget(0x7fff, 0);
  PaletteFade_StartInterpolation(8);
  wait(16);
  MapGrid_SetShake(0, 0, 0);
  Actor_ConfigureSpriteColor(firstId, 0);
  Actor_ConfigureSpriteColor(secondId, 0);
  Sound_Play(SE_574);
  PaletteFade_SetTarget(0x10000, 0);
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

void Actor_MoveToObject();

void AquaRockInside_ReadParchTablet(void) {
  struct Actor *object;

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
  Actor_MoveToObject(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
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
  AquaRockInside_PiersLearnsParch(14, 7);
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
  object = Actor_Get(gMainRoutineWork.leaderActorId);
  if (object != 0) {
    Actor_SetDestination(ACTOR_PIERS, object->x.part[0].pixel,
                         object->z.part[0].pixel);
  }
  FieldActor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Event_End();
}

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

DEFINE_AQUA_ROCK_INSIDE_READ_TABLET(AquaRockInside_ReadTabletFacingWest, 280)

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

void AquaRockInside_SetMoaiFace(s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object != 0) {
    u8 *meta = Sprite_AddAnimation(object->sprite, 0x17d);

    meta[5] = 0;
    meta[6] = 0;
    Actor_SelectAnimation(object, 2);
  }
}

void AquaRockInside_SplashBeneath(struct Actor *object) {
  s32 phase = gFrameCount & 15;

  if (phase == 0) {
    struct Actor *child = Import_ActorCreate_(0x10d, object->x.fixed,
                                              object->y.fixed, object->z.fixed);

    if (child != 0) {
      struct FieldSprite *sprite = child->sprite;

      Actor_StartScript(child, gAquaRockInside_SplashScript);
      child->motionFlags = 0;
      child->layer = 1;
      if (sprite != 0) {
        Sprite_StartAnimation(sprite, 2);
        sprite->nextFrame = 0;
        sprite->rotation = 0x4000;
        sprite->priority = 2;
      }
    }
  }
}

void AquaRockInside_FallThroughWater(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
  u8 *flags = &object->motionFlags;
  s32 saved = *flags;
  s32 speed;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Actor_SetSpriteColorMode(object, 9);
  *flags = 0;
  object->y.fixed = px(80);
  speed = px(3);
  i = 31;
fall:
  object->y.fixed -= speed;
  object->facing += 0x2000;
  speed += -(px(1) / 20);
  Actor_SetSpriteFlags(object, 0);
  System_Wait(1);
  i--;
  if (i >= 0)
    goto fall;
  AquaRockInside_SpawnSplash(object);
  Actor_SetSpriteColorMode(object, 0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 2);
  *flags = saved;
  object->velocityY = px(8);
  Event_PlaySound(SE_SPLASH_SMALL);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 168, 200);
  Event_End();
}

void AquaRockInside_UpdateVents(void) {
  s32 counter = AquaRockInside_GetVentCounter();

  switch (counter) {
  case 0:
    AquaRockInside_SetVentActive(0, 1);
    AquaRockInside_SetVentActive(1, 1);
    Scene_QueueMusic(SE_AMBIENT_WATER);
    break;
  case 0x34bbf:
    AquaRockInside_SetVentActive(0, 0);
    AquaRockInside_SetVentActive(1, 0);
    Scene_QueueMusic(-1);
    break;
  }
  Vents_AdvanceCounter(0x34bc0);
}

extern u16 gAquaRockInside_FirstOutletSpots[];
extern u16 gAquaRockInside_FirstOutletLevels[];
extern u16 gAquaRockInside_SecondOutletSpots[];
extern u32 gAquaRockInside_SecondOutletLevels[];

s32 AquaRockInside_SetUpOutlets(const u8 *a, const u8 *b,
                                void (*handler)(void));

/// Aqua Rock's entry setup: in each room it shows the statues already filled by
/// their flags (or dims those still empty), opens what the story has opened and
/// arms the room's water and cell handlers by entrance.
s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.next;
  s32 entrance = gMainRoutineWork.entrance;
  struct Actor *object;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (scene == SCENE_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST) {
    if (GameFlag_Get(FLAG_SCENE_TEMP_30)) {
      AquaRockInside_SetMoaiFace(ACTOR_WATER_OF_LIFE_CHEST_EAST_MOAI);
      Actor_SetAnimation(ACTOR_WATER_OF_LIFE_CHEST_EAST_MOAI, 3);
    } else {
      FieldActor_SetSpriteColor(
          Actor_GetById(ACTOR_WATER_OF_LIFE_CHEST_EAST_MOAI), 6);
    }
    if (GameFlag_Get(FLAG_SCENE_TEMP_31)) {
      AquaRockInside_SetMoaiFace(ACTOR_WATER_OF_LIFE_CHEST_WEST_MOAI);
      Actor_SetAnimation(ACTOR_WATER_OF_LIFE_CHEST_WEST_MOAI, 3);
    } else {
      FieldActor_SetSpriteColor(
          Actor_GetById(ACTOR_WATER_OF_LIFE_CHEST_WEST_MOAI), 6);
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_STEAM) {
    if (GameFlag_Get(FLAG_SCENE_TEMP_40)) {
      MapGrid_CopyCellsLayered(118, 15, 5, 32, 110, 15);
      MapGrid_CopyCellAttributes(59, 16, 5, 32, 46, 16);
      FieldActor_SetSpriteColor(Actor_GetById(ACTOR_STEAM_ROOM_MOAI), 6);
    } else {
      AquaRockInside_SetMoaiFace(ACTOR_STEAM_ROOM_MOAI);
      Actor_SetAnimation(ACTOR_STEAM_ROOM_MOAI, 3);
    }
    object = Actor_GetById(ACTOR_STEAM_ROOM_PILLAR);
    object->motionFlags = 0;
    if (GameFlag_Get(FLAG_AQUA_ROCK_PILLAR_DROPPED_BY_MOAI)) {
      Actor_SetPosition(ACTOR_STEAM_ROOM_PILLAR, 632, 152);
      MapGrid_CopyCellAttributes(41, 9, 1, 1, 39, 9);
      object->y.fixed = 0;
      object->groundHeight = 0;
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_AQUARIUS_STONE) {
    if (entrance == 7)
      Leader_RiseFromWater();
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST) {
    AquaRockInside_SetMoaiFace(ACTOR_RUSTY_SWORD_CHEST_MOAI_A);
    Actor_SetAnimation(ACTOR_RUSTY_SWORD_CHEST_MOAI_A, 3);
    AquaRockInside_SetMoaiFace(ACTOR_RUSTY_SWORD_CHEST_MOAI_B);
    Actor_SetAnimation(ACTOR_RUSTY_SWORD_CHEST_MOAI_B, 3);
    if ((u32)(entrance - 6) <= 1) {
      if (GameFlag_Get(FLAG_SCENE_TEMP_49)) {
        FieldActor_SetSpriteColor(Actor_GetById(ACTOR_RUSTY_SWORD_CHEST_MOAI),
                                  5);
        AquaRockInside_SetMoaiFace(ACTOR_RUSTY_SWORD_CHEST_MOAI);
        Actor_SetAnimation(ACTOR_RUSTY_SWORD_CHEST_MOAI, 3);
        AquaRockInside_SetPillarCellLow();
      } else {
        FieldActor_SetSpriteColor(Actor_GetById(ACTOR_RUSTY_SWORD_CHEST_MOAI),
                                  6);
      }
    }
    Actor_GetById(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_B)->riseCounter = 1;
    Actor_GetById(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_C)->riseCounter = 1;
    object = Actor_GetById(ACTOR_RUSTY_SWORD_CHEST);
    if (object != 0) {
      object->motionFlags = 0;
      object->groundHeight = -px(32);
      object->y.fixed = -px(32);
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST) {
    if ((u32)(entrance - 5) <= 1) {
      AquaRockInside_SetUpOutlets(gAquaRockInside_FirstOutletSpots,
                                  gAquaRockInside_FirstOutletLevels,
                                  AquaRockInside_UpdateVents);
      Scene_QueueMusic(SE_AMBIENT_WATER);
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_VIAL_CHEST) {
    if (entrance == 5 || entrance == 7) {
      AquaRockInside_SetUpOutlets(gAquaRockInside_SecondOutletSpots,
                                  gAquaRockInside_SecondOutletLevels,
                                  AquaRockInside_UpdateVents);
      Scene_QueueMusic(SE_AMBIENT_WATER);
      AquaRockInside_FinishPushingRock(0, ACTOR_VIAL_CHEST_RAPIDS_PILLAR_A);
    }
    if (!GameFlag_Get(FLAG_SCENE_TEMP_38)) {
      AquaRockInside_SetMoaiFace(ACTOR_VIAL_CHEST_MOAI);
      Actor_SetAnimation(ACTOR_VIAL_CHEST_MOAI, 3);
    }
    object = Actor_GetById(ACTOR_VIAL_CHEST_SPLASH);
    Actor_SetSpriteFlags(object, 0);
    object->collisionFlags = 0;
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    object->update = (void (*)(void))AquaRockInside_SplashBeneath;
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) && entrance == 13)
      AquaRockInside_FallThroughWater();
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_TEAR_STONE) {
    if (GameFlag_Get(FLAG_SCENE_TEMP_4B)) {
      FieldActor_SetSpriteColor(Actor_GetById(ACTOR_TEAR_STONE_MOAI), 5);
      AquaRockInside_SetMoaiFace(ACTOR_TEAR_STONE_MOAI);
      Actor_SetAnimation(ACTOR_TEAR_STONE_MOAI, 3);
    } else {
      FieldActor_SetSpriteColor(Actor_GetById(ACTOR_TEAR_STONE_MOAI), 6);
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
    value = (control = REG_BG1CNT & 0xfffc);                                   \
    do {                                                                       \
      REG_BG1CNT = value | (priority);                                         \
    } while (0);                                                               \
  }

void Tightrope_Initialize();

/// Aqua Rock's map preparation: for each room and entrance it sets the water
/// level, places the statues and boulders the flags have moved, opens or
/// closes the cells around them, and in the rooms whose water drains it shows
/// the drained state once its flag is set. The Vial's room puts its puddles out
/// of sight (x and z cleared) until its pillar has fallen by the falls.
s32 Scene_PrepareMap(void) {
  s32 scene = gMainRoutineWork.next;
  s32 entrance = gMainRoutineWork.entrance;
  volatile u16 control;
  // i1, i3, i4, i6, i7, i12, i14, i17 and b each hold one constant for their
  // whole life, i2 holds a flag test and the others each count one loop.
  s32 i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, i16,
      i17;
  struct Actor *actor;
  s32 a, b;
  struct MapWork *map;

  Map_SetUpperLevelWading(Ram_HeapSlots->mapGrid);
  AquaRockInside_StartPillars();
  if (scene == SCENE_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST) {
    if ((u32)(entrance - 3) <= 1 || entrance == 6 || entrance == 8) {
      AquaRockInside_AddSpouts(33, 0, -2);
    } else if (entrance == 5 || entrance == 7) {
      actor = Actor_GetById(ACTOR_WATER_OF_LIFE_CHEST_WATER_PILLAR);
      i1 = 0;
      actor->motionFlags = i1;
      actor->groundHeight = i1;
      actor->y.fixed = i1;
      Actor_SelectAnimation((s32)actor, 0);
      actor = Actor_GetById(ACTOR_WATER_OF_LIFE_CHEST_PUDDLE);
      actor->motionFlags = i1;
      actor->groundHeight = i1;
      actor->y.fixed = i1;
      i2 = GameFlag_Get(FLAG_SCENE_TEMP_30);
      if (i2 == 0) {
        MapGrid_CopyCellsLayered(104, 80, 16, 10, 104, 40);
        MapGrid_CopyCellsLayered(110, 116, 18, 12, 39, 105);
        // i2 is the flag test's 0.
        MapGrid_CopyCellAttributes(64, 32, 64, 32, i2, 32);
        AquaRockInside_AddSpouts(33, 0, 0);
      } else {
        AquaRockInside_AddSpouts(33, 4, 0);
        actor = Actor_GetById(ACTOR_WATER_OF_LIFE_CHEST_WATER_PILLAR);
        AquaRockInside_SetPillarHeight(FIELD_CELL(actor->x.fixed) - 42, 0);
      }
      if (GameFlag_Get(FLAG_SCENE_TEMP_50)) {
        MapGrid_CopyCellAttributes(41, 42, 1, 1, 43, 44);
      }
    } else if ((u32)(entrance - 9) <= 1) {
      a = GameFlag_Get(FLAG_SCENE_TEMP_31);
      if (a == 0) {
        i3 = 32;
        MapGrid_CopyCellsLayered(77, 80, 23, 20, 77, i3);
        MapGrid_CopyCellsLayered(112, 96, 16, 20, 12, 98);
        // a is the flag test's 0.
        MapGrid_CopyCellAttributes(64, 32, 64, 32, a, i3);
        AquaRockInside_AddSpouts(33, 0, 0);
      } else {
        AquaRockInside_AddSpouts(33, 4, 0);
      }
    }
    if (GameFlag_Get(FLAG_AQUA_ROCK_WEST_VENT_COVERED)) {
      Actor_SetPosition(ACTOR_WATER_OF_LIFE_CHEST_WEST_VENT_PILLAR, 184, 200);
      i4 = 12;
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 9, i4);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 13, i4);
    }
    if (GameFlag_Get(FLAG_AQUA_ROCK_EAST_VENT_COVERED)) {
      Actor_SetPosition(ACTOR_WATER_OF_LIFE_CHEST_EAST_VENT_PILLAR, 824, 216);
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_STEAM) {
    if ((u32)(entrance - 1) <= 1) {
      AquaRockInside_AddSpouts(33, 0, -2);
    }
    actor = Actor_GetById(ACTOR_STEAM_ROOM_MOAI);
    actor->motionFlags = 0;
    actor->groundHeight = 0;
    actor->y.fixed = 0;
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_AQUARIUS_STONE) {
    FieldEffect_StartBlankOverlay(0);
    Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_AQUARIUS_STONE_BASE), 0);
    Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_AQUARIUS_STONE), 0);
    AquaRockInside_AddSpouts(33, 8, -2);
    AquaRockInside_AddSpouts(40, 8, 8);
    if (GameFlag_Get(FLAG_AQUA_ROCK_AQUARIUS_STONE_WATER_STONE_DOUSED)) {
      AquaRockInside_StartSplashes();
      AquaRockInside_AddSplash(16, 4, 16, 2);
      AquaRockInside_AddSplash(22, 4, 16, 0);
      for (i5 = 0; i5 < 6; i5++)
        AquaRockInside_AdvanceStreams(33, 55);
    }
    if (GameFlag_Get(FLAG_AQUA_ROCK_AQUARIUS_STONE_SET)) {
      MapGrid_CopyCellsLayered(75, 96, 7, 13, 80, 27);
      i6 = 16;
      b = 31;
      MapGrid_CopyCellsLayered(91, 96, 7, 10, i6, b);
      MapGrid_CopyCellsLayered(99, 96, 7, 14, i6, 91);
      MapGrid_CopyCellAttributes(43, 0, 5, 9, 17, b);
      AquaRockInside_ShowAquariusStone();
    } else {
      MapGrid_CopyCellAttributes(17, 32, 5, 1, 17, 31);
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST) {
    FieldEffect_StartBlankOverlay(0);
    if ((u32)(entrance - 6) <= 1)
      goto statues;
    goto no_statues;
  statues:
    {
      Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
          ACTOR_PRIORITY_IGNORES_RISES;
      Value3((s32 (*)())AquaRockInside_AddPillar,
             ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A, 5, 4);
      Value3((s32 (*)())AquaRockInside_AddPillar,
             ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_B, 3, 4);
      Value3((s32 (*)())AquaRockInside_AddPillar,
             ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_C, 4, 4);
      AquaRockInside_AddSpouts(40, 4, 4);
      if (!GameFlag_Get(FLAG_SCENE_TEMP_49) &&
          !Event_GetFlag(FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED)) {
        map = Ram_HeapSlots->mapGrid;
        i7 = 39;
        MapGrid_CopyCellsLayered(64, 64, 16, 13, 71, i7);
        MapGrid_CopyCellAttributes(71, 39, 16, 15, 7, i7);
        AquaRockInside_SetPillarCellMiddle();
        map->enabled[2] = 1;
        SET_BG1CNT_PRIORITY(1);
      } else if (Event_GetFlag(
                     FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED) ||
                 GameFlag_Get(FLAG_SCENE_TEMP_48)) {
        if (Event_GetFlag(FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED)) {
          Actor_SetPosition(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A, 280, 712);
        }
        AquaRockInside_BlockCellUnder(ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A,
                                      38);
        AquaRockInside_StartSplashes();
        AquaRockInside_AddSplash(12, 2, 45, 1);
        for (i8 = 0; i8 < 8; i8++)
          AquaRockInside_AdvanceStreams(33, 55);
        Event_SetFlag(FLAG_SCENE_TEMP_49);
      }
      i9 = ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A;
      do {
        actor = Actor_GetById(i9);
        actor->motionFlags = 0;
        actor->groundHeight = 0;
        actor->y.fixed = 0;
        i9++;
      } while (i9 <= ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_C);
      goto statues_cleared;
    }
  no_statues:
    AquaRockInside_AddSpouts(33, 0, -4);
  statues_cleared:
    if (GameFlag_Get(FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_POOL_PARCHED)) {
      AquaRockInside_ParchPool();
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST) {
    AquaRockInside_AddSpouts(33, 0, -2);
    AquaRockInside_AddSpouts(40, 0, 0);
    FieldEffect_StartBlankOverlay(0);
    if (((u32)(entrance - 1) <= 3 || entrance == 7) &&
        GameFlag_Get(FLAG_AQUA_ROCK_CRYSTAL_POWDER_WATER_STONE_DOUSED)) {
      AquaRockInside_StartSplashes();
      AquaRockInside_AddSplash(44, 0, 34, 2);
      for (i10 = 0; i10 <= 4; i10++)
        AquaRockInside_AdvanceStreams(33, 55);
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_VIAL_CHEST) {
    FieldEffect_StartBlankOverlay(0);
    if (entrance == 1 || entrance == 13) {
      AquaRockInside_AddSpouts(33, 0, -2);
    } else if (entrance == 9 || entrance == 11 || entrance == 12) {
      AquaRockInside_AddSpouts(33, 0, -2);
    }
    if (!GameFlag_Get(FLAG_SCENE_TEMP_38)) {
      i11 = ACTOR_VIAL_CHEST_PUDDLE_A;
      do {
        actor = Actor_GetById(i11);
        actor->motionFlags = 0;
        actor->x.fixed = 0;
        actor->z.fixed = 0;
        i11++;
      } while (i11 <= ACTOR_VIAL_CHEST_PUDDLE_C);
      goto done;
    } else {
      MapGrid_CopyCellsLayered(114, 39, 1, 2, 113, 39);
      i12 = 41;
      MapGrid_CopyCellAttributes(105, 41, 12, 13, i12, i12);
      MapGrid_CopyCellsLayered(105, 105, 12, 13, 105, i12);
      for (i13 = ACTOR_VIAL_CHEST_PUDDLE_A; i13 <= ACTOR_VIAL_CHEST_PUDDLE_C;
           i13++) {
        actor = Actor_GetById(i13);
        if (GameFlag_Get(FLAG_SCENE_TEMP_51 - ACTOR_VIAL_CHEST_PUDDLE_A +
                         i13)) {
          MapGrid_CopyCellAttributes(47, 45, 1, 1, FIELD_CELL(actor->x.fixed),
                                     FIELD_CELL(actor->z.fixed));
        }
      }
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_PSYNERGY_STONE) {
    AquaRockInside_AddSpouts(33, 0, -2);
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_TIGHTROPE) {
    FieldEffect_StartBlankOverlay(0);
    AquaRockInside_AddSpouts(33, 0, -2);
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
      AquaRockInside_AddSpouts(40, 4, 4);
      if (!GameFlag_Get(FLAG_SCENE_TEMP_4B) &&
          !Event_GetFlag(FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED)) {
        map = Ram_HeapSlots->mapGrid;
        i14 = 13;
        MapGrid_CopyCellsLayered(64, 64, 19, 17, 100, i14);
        MapGrid_CopyCellsLayered(35, 105, 23, 24, 35, 73);
        MapGrid_CopyCellAttributes(100, 13, 19, 19, 36, i14);
        map->enabled[2] = 1;
        SET_BG1CNT_PRIORITY(1);
      } else if (Event_GetFlag(FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED) ||
                 GameFlag_Get(FLAG_SCENE_TEMP_4A)) {
        if (Event_GetFlag(FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED)) {
          Actor_SetPosition(ACTOR_TEAR_STONE_WATER_PILLAR_C, 808, 296);
          Actor_SetPosition(ACTOR_TEAR_STONE_WATER_PILLAR_D, 680, 360);
          Actor_SetPosition(ACTOR_TEAR_STONE_WATER_PILLAR_F, 744, 424);
        }
        Call2((void (*)())AquaRockInside_BlockCellUnder,
              ACTOR_TEAR_STONE_WATER_PILLAR_C, 38);
        Call2((void (*)())AquaRockInside_BlockCellUnder,
              ACTOR_TEAR_STONE_WATER_PILLAR_D, 39);
        AquaRockInside_BlockCellUnder(ACTOR_TEAR_STONE_WATER_PILLAR_F, 37);
        AquaRockInside_StartSplashes();
        AquaRockInside_AddSplash(38, 2, 19, 1);
        for (i15 = 0; i15 < 8; i15++)
          AquaRockInside_AdvanceStreams(33, 55);
        Event_SetFlag(FLAG_SCENE_TEMP_4B);
      }
    }
    i16 = ACTOR_TEAR_STONE_WATER_PILLAR_A;
  clear_pillars:
    actor = Actor_GetById(i16);
    actor->motionFlags = 0;
    actor->groundHeight = 0;
    actor->y.fixed = 0;
    i16++;
    if (i16 <= ACTOR_TEAR_STONE_PILLAR_B)
      goto clear_pillars;
    actor = Actor_GetById(ACTOR_TEAR_STONE_PILLAR_A);
    MapGrid_CopyCellAttributes(23, 39, 1, 1, FIELD_CELL(actor->x.fixed), 39);
    if ((u32)(entrance - 1) <= 1) {
      FieldEffect_StartBlankOverlay(0);
    }
  } else if (scene == SCENE_AQUA_ROCK_INSIDE_PARCH_TABLET) {
    FieldEffect_StartBlankOverlay(0);
    AquaRockInside_AddSpouts(33, 0, -2);
    Event_SetFlag(FLAG_RETREAT_BLOCKED);
    if (GameFlag_Get(FLAG_AREA_TEMP_49)) {
      MapGrid_CopyCellAttributes(38, 9, 7, 6, 13, 9);
    } else {
      i17 = 8;
      MapGrid_CopyCellsLayered(77, 8, 7, 6, 50, i17);
      MapGrid_CopyCellsLayered(70, 8, 7, 6, 77, i17);
      MapGrid_CopyCellAttributes(38, 17, 7, 6, 13, 9);
    }
  }
done:
  return 0;
}

void AquaRockInside_SpawnSplash(struct Actor *object) {
  struct Actor *child = FieldActor_Create(
      14, object->x.fixed, object->y.fixed + px(4), object->z.fixed + px(4));
  struct FieldSprite *source = object->sprite;

  if (child != 0) {
    struct FieldSprite *sprite = child->sprite;

    child->groundHeight = object->groundHeight;
    Actor_StartScript(child, gAquaRockInside_StreamSplashScript);
    child->motionFlags = 0;
    if (sprite != 0) {
      Sprite_StartAnimation(sprite, 1);
      sprite->nextFrame = 0;
      sprite->priority = source->priority;
    }
  }
}

#include "shared/actor_emit_water_drop.h"

void AquaRockInside_FlowWithStream(void) {
  struct Actor *center;
  s32 id;
  struct Actor *actor;
  s32 position[3];
  s32 hit;

  {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;

    id = gMainRoutineWork.leaderActorId;
    center = work->camera;
  }
  actor = Actor_Get(id);
  if (gMainRoutineWork.next == SCENE_AQUA_ROCK_MOAI && actor->groundHeight == 0)
    return;
  position[0] = actor->x.fixed;
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed + px(2);
  hit = MapGrid_CheckCollision(actor, position);
  if ((gFrameCount & 4) == 0)
    AquaRockInside_SpawnSplash(actor);
  if (hit < 0) {
    Actor_SetAttachedEffect(id,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldActor_SetMoveTargetFixed(actor, actor->x.fixed, actor->y.fixed,
                                  actor->z.fixed + px(8));
    Actor_SelectAnimation(actor, 49);
    FieldActor_WaitForMove(actor);
    do {
      System_Wait(1);
    } while (actor->y.fixed != actor->groundHeight);
    AquaRockInside_SpawnSplash(actor);
    Actor_SelectAnimation(actor, 49);
    System_Wait(3);
    return;
  }
  position[0] = actor->x.fixed;
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed + px(8);
  hit = MapGrid_CheckCollision(actor, position);
  if (hit > 0)
    return;
  position[0] = actor->x.fixed + (px(57) / 10);
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed + (px(57) / 10);
  hit = MapGrid_CheckCollision(actor, position);
  if (hit > 0)
    return;
  position[0] = actor->x.fixed - (px(57) / 10);
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed + (px(57) / 10);
  hit = MapGrid_CheckCollision(actor, position);
  if (hit > 0)
    return;
  center->z.fixed += px(2);
  actor->z.fixed += px(2);
  actor->facing = FACING_SOUTH;
}

#include "shared/actor_show_parts.h"

void Leader_SpinAway(s32 entrance) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  leader->motionFlags = 0;
  leader->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Event_PlaySound(SE_SPLASH_SMALL);
  Actor_SelectAnimation(leader, 18);
  Event_PlaySound(SE_SPIN_AWAY);
  for (i = 0; i < 60; i++) {
    s32 angle;

    if (i == 30)
      Event_FinishScreenTransition();
    leader->y.fixed -= px(1) / 4;
    angle = leader->facing;
    leader->facing = angle + 0x1000;
    if ((i & 7) == 0)
      AquaRockInside_SpawnSplash(Actor_Get(ACTOR_VIAL_CHEST_PUDDLE_C));
    System_Wait(1);
  }
  Event_End();
  FieldEvent_SetExitRequest(entrance);
}

// On odd frames, spawns a drifting effect above the object, swaying with the
// frame count.
#include "shared/actor_emit_mist.h"

#include "shared/leader_sink_away.h"

#include "shared/leader_rise_from_water.h"

/// Draws the scene's pillars: a top sprite for each active one and, when it
/// stands above its base, a column sprite as tall as its height.
void AquaRockInside_DrawPillars(void) {
  s32 *view = &Ram_HeapSlots->mapGrid->viewX;
  struct PillarWork *work = &gAquaRockInside_Pillars;
  s32 viewX = view[0];
  struct PillarEntry *entry = work->entries;
  s32 viewZ;
  s32 originY;
  s32 depth;
  s32 i;
  u32 tile;

  viewX &= 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  originY = Ram_HeapSlots->mapGrid->origin[1];
  tile = gVramCacheEntries[work->vramEntry].offset >> 5;

  for (i = 0; i < 32; i++) {
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
        attributes = 0x800;
      else
        attributes = 0xc00;
      x = (x >> 16) - 8;
      screenY = (screenY >> 16) - 8;
      depth = ((y + z) >> 16) + 50;
      if ((u32)(x + 15) <= 270) {
        s32 rise;

        if ((u32)(screenY + 47) <= 238) {
          x &= 511;
          screenY &= 255;
          rise = entry->kind * 8;
          entry->top[0] = 0;
          screenY |= x << 16;
          screenY |= 0x40002000;
          entry->top[1] = screenY;
          entry->top[2] = (tile + rise) | attributes;
          Oam_AddSpriteAtDepthClamped(entry->top, depth);
        }
        screenY = z - y;
        screenY = (screenY >> 16) + 8;
        if ((u32)(screenY + 47) <= 238) {
          rise = (entry->height - entry->base) >> 16;
          if (rise != 0) {
            u32 *column;
            rise--;
            screenY &= 255;
            column = entry->column;
            if ((u32)rise > 7)
              rise = 7;
            rise = rise * 4 + 64;
            column[0] = 0;
            screenY |= x << 16;
            screenY |= 0x6000;
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

void AquaRockInside_StartPillars(void) {
  void *buffer;
  s32 priority;

  Iwram_ClearWordsEntry(&gAquaRockInside_Pillars, 0x604);
  buffer = MemAlloc_(0xc80);
  Decompress_TaggedLz(gAquaRockInside_PillarTiles, buffer);
  gAquaRockInside_Pillars.vramEntry = VramCache_FindFreeEntry();
  Vram_Load(gAquaRockInside_Pillars.vramEntry, 0xc80, buffer);
  Heap_Release(buffer);
  priority = 0x41c;
  FieldTask_AddFrame(AquaRockInside_DrawPillars, priority);
}

/// Adds a water spout on every map cell of the given layer whose spout kind
/// lies in [first, first + 8), at the given heights, and marks those cells as
/// spout cells (120).
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

  *(s32 *)(Data_0202c001 + 0x1df) = level << 8;
  cells += corner;
  row = 0;
  if (row < depth) {
    struct MapCell *cell;
    s16 column;
  rowTop:
    cell = cells + ((u16)row << 7);
    column = 0;
    if (column < width) {
    columnTop:
      {
        u32 kind = cell->collisionCode;

        if (kind != 0 && kind >= first && kind < first + 8) {
          spout->x = ((u16)column << 20) + originX + px(8);
          spout->z = ((u16)row << 20) + originZ + px(8);
          spout->base = baseLevel << 19;
          spout->height = level << 19;
          spout->kind = kind - first;
          spout->position = 0;
          spout->state = 1;
          spout++;
          work->count++;
          {
            u8 *terrain = map->layers[0].terrainIds + corner;

            terrain[(u16)column + ((u16)row << 7)] = 120;
          }
        }
      }
      {
        s32 next = (column << 16) + 0x10000;
        column = next >> 16;
        cell++;
        if ((s32)((u32)next >> 16) < width)
          goto columnTop;
      }
    }
    {
      s32 next = (row << 16) + 0x10000;
      row = next >> 16;
      if ((s32)((u32)next >> 16) < depth)
        goto rowTop;
    }
  }
}

void AquaRockInside_AddPillar(s32 id, s32 kind, s32 height) {
  struct Actor *object = Actor_Get(id);
  struct PillarWork *work = &gAquaRockInside_Pillars;
  struct PillarEntry *entry = &work->entries[work->count];

  if (object != 0) {
    entry->x = 0;
    entry->z = 0;
    entry->base = height << 19;
    entry->height = height << 19;
    entry->state = 2;
    entry->kind = kind;
    entry->position = &object->x.fixed;
    work->count++;
  }
}

void AquaRockInside_SetPillarsHeight(s32 height) {
  struct PillarWork *work = &gAquaRockInside_Pillars;
  struct PillarEntry *entry;
  s32 i;

  *(s32 *)(Ram_MapCollision + 0x1e0) = ((height / 0x80000) & 0xff) << 8;
  entry = work->entries;
  for (i = 31; i >= 0; i--) {
    if (entry->x != 0)
      entry->height = height;
    entry++;
  }
}

void AquaRockInside_SetPillarHeight(u32 index, s32 height) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct PillarWork *work = &gAquaRockInside_Pillars;

  if (index <= 31) {
    struct PillarEntry *entry = &work->entries[index];

    if (entry->x != 0) {
      *(s32 *)(Ram_MapCollision + 0x1e4) = ((height / 0x80000) & 0xff) << 8;
      {
        u8 *ids = map->layers[0].terrainIds;
        s32 at = FIELD_CELL(entry->x) + FIELD_CELL(entry->z) * 128;
        s32 v = 121;
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
  // The sprite word's priority bits (priority 2): a local that holds this one
  // value.
  s32 priority = 0x800;

  viewX &= 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  originY = map->origin[1];
  tile = gVramCacheEntries[splashes->vramEntry].offset >> 5;
  splash = splashes->splashes;
  i = 31;

  do {
    if (splash->active != 0) {
      s32 x = splash->x - viewX;
      s32 y = splash->y - originY;
      s32 z = splash->z;
      s32 screenY, rawX, rawY;
      z -= viewZ;
      z -= originY;
      screenY = z - y;
      rawX = (x >> 16) + (Random_Below(10));

      x = rawX - 10;
      rawY = (screenY >> 16) + (Random_Below(10));
      screenY = rawY - 10;
      z = ((y + z) >> 16) + 50;

      if ((u32)(x + 15) <= 270 && (u32)(screenY + 47) <= 238) {
        u32 *frame = &splash->sprite[2];
        s32 column = x & 511;

        screenY &= 255;
        splash->sprite[0] = 0;
        splash->sprite[1] = screenY | (column << 16) | 0x40002000;
        *frame = (tile + ((Random_Below(3)) << 3)) | priority;
        Oam_AddSpriteAtDepthClamped(splash->sprite, z);
      }
    }
    i--;
    splash++;
  } while (i >= 0);
}

void AquaRockInside_StartSplashes(void) {
  void *buffer;
  s32 priority;

  Iwram_ClearWordsEntry(&gAquaRockInside_Splashes, 0x3a4);
  buffer = MemAlloc_(0x400);
  Decompress_TaggedLz(gAquaRockInside_SplashTiles, buffer);
  gAquaRockInside_Splashes.vramEntry = VramCache_FindFreeEntry();
  Vram_Load(gAquaRockInside_Splashes.vramEntry, 0x400, buffer);
  Heap_Release(buffer);
  priority = 0x41c;
  FieldTask_AddFrame(AquaRockInside_DrawSplashes, priority);
  Event_PlaySound(SE_SHIMMER);
}

s32 AquaRockInside_AddSplash(s32 x, s32 y, s32 z, s32 kind) {
  struct BeamWork *work = &gAquaRockInside_Splashes;
  struct BeamEntry *entry = work->entries;

  if (entry->x != 0)
    entry++;
  SET_POSITION(entry, (x << 20) + 0x80000, y << 20, (z << 20) + 0x80000);
  entry->direction = kind;
}

extern s8 gAquaRockInside_StreamTurns[];

/// Advances Aqua Rock's two water streams one cell along the map: each stream
/// follows the turn its cell's kind gives, leaves a splash on every open cell
/// (255) it crosses, stops on a wall kind and counts the cells of the given
/// kind it reaches; then it clears the splashes past the new count and opens
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
        struct MapCell *cell =
            &cells[FIELD_CELL(stream->x) + FIELD_CELL(stream->z) * 128];
        s32 kind = cell->collisionCode;

        if (kind == 255) {
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
          if (turn != 8)
            count = state->count;
          again = 0;
        } else if (kind == target) {
          turn = 8;
          count = state->count;
          reached++;
          again = 0;
        } else if (kind == 0) {
          turn = 8;
          again = 0;
        }
        if (turn != 8) {
          // The unsigned % 4: as & 3 the mask narrows to a halfword and pools.
          stream->direction = (u32)(stream->direction + turn) % 4;
          FieldVector_AddPolarOffset(16, stream->direction << 14, &stream->x);
        }
      } while (again != 0);
      state->count = count;
    }
    stream++;
  }
  splash = &state->splashes[count];
  if (count < 32) {
    i = 32 - count;
  clear:
    if (splash->active != 0)
      splash->active = 0;
    splash++;
    if (--i != 0)
      goto clear;
  }
  splash = state->splashes;
  if (count != 0) {
    i = count;
  open:
    if (splash->active != 0) {
      struct MapCell *cells = map->layers[0].cells;
      struct MapCell *cell =
          &cells[FIELD_CELL(splash->x) + FIELD_CELL(splash->z) * 128];

      cell->collisionCode = 0;
      cell->attributeB |= 0x80;
    }
    splash++;
    if (--i != 0)
      goto open;
  }
  return reached;
}
#undef SET_BG1CNT_PRIORITY

#define VENT_TOUCHING Vent_IsTouching
#define VENT_BOUNCE Vent_Bounce
#include "../field/actor_vent_bounce.c"

#include "shared/actor_start_scripted.h"

void Vent_Puff(s32 *position, s32 base) {
  struct Actor *object =
      Import_ActorCreate_(0x11d, position[0], position[1], position[2] - px(2));
  s32 angle;

  if (object != 0) {
    object->bounce = Trig_Cos(((u32)(Random16() * (Q16_ONE * 3 / 16)) >> 16) +
                              (Q16_ONE * 3 / 16));
    object->gravity = (u16)Random16();
    object->riseBaseY = (u16)(Random16() << 1) + base;
    angle = (u16)Random16() - 0x8000;
    object->timer = angle;
    Actor_StartScripted(object, 3);
    object->update = (void (*)(void))Vent_Bounce;
    Actor_SetBlendMode(object, 1);
  }
}

#include "shared/vents_update.h"

/* VIEW(s3844): routine AquaRockInside_SetUpOutlets as void
 * AquaRockInside_SetUpOutlets(const u16 *spots, const u16 *levels, void
 * (*handler)(void)); games/tla/src/scene/aqua_rock_inside/s1620 has s32
 * AquaRockInside_SetUpOutlets(const u8 *a, const u8 *b, void (*handler)(void));
 * tried as the definition's void AquaRockInside_SetUpOutlets(const u16 *spots,
 * const u16 *levels, void(*handler) (void)), which the object already declares,
 * it does not compile in tla-ja (aqua_rock_inside.c):
 * games/tla/src/scene/aqua_rock_inside.c:2382: conflicting types for
 * `AquaRockInside_SetUpOutlets'. */
void AquaRockInside_SetUpOutlets__vents(
    const u16 *spots, const u16 *levels,
    void (*handler)(void)) __asm__("AquaRockInside_SetUpOutlets");
#define AquaRockInside_SetUpOutlets AquaRockInside_SetUpOutlets__vents
void AquaRockInside_SetUpOutlets(const u16 *spots, const u16 *levels,
                                 void (*handler)(void)) {
  struct WaterWork *work = (struct WaterWork *)(gInventorySnapshot + 0x188);
  struct WaterOutlet *outlet = work->outlets;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 x;
    s32 i;

    Iwram_ClearWordsEntry(work, 0x150);
    x = *spots++;
    i = 0;
    while (x > 0) {
      s32 z = spots[0];
      s32 level = spots[1];

      outlet->state = 0;
      outlet->x = x << 16;
      outlet->z = z << 16;
      outlet->y = Map_GetTerrainHeight(0, x << 16, (z << 16) + px(16));
      outlet->level = level << 16;
      outlet->index = i++;
      spots += 2;
      outlet++;
      if (i > 15)
        break;
      x = *spots++;
    }
    if (levels != 0) {
      x = *levels++;
      i = 0;
      if (x > 0) {
      nextLevel:
        work->levels[i++] = x;
        if (i > 3)
          goto levelsDone;
        x = *levels++;
        if (x > 0)
          goto nextLevel;
      }
    }
  levelsDone:;
  }
  work->handler = handler;
  if (REG_BLDCNT == 0) {
    {
      s32 alpha = 0xc08;
      REG_BLDALPHA = alpha;
    }
    {
      s32 cnt = 0x3f10;
      REG_BLDCNT = cnt;
    }
  }
  Task_AddFrame(Vents_Update, TASK_PASS_VBLANK);
}

s32 AquaRockInside_IsVentActive(s32 index) {
  u8 *base = (u8 *)VENT_WORK;

  return ((struct VentRow *)(base + index * 20))->active;
}

s32 AquaRockInside_SetVentActive(s32 index, s32 value) {
  u8 *base = (u8 *)VENT_WORK;

  ((struct VentRow *)(base + index * 20))->active = value;
}

s32 AquaRockInside_GetVentCounter(void) { return VENT_WORK->counter; }

#include "shared/vents_advance_counter.h"

void AquaRockInside_SetVentValue(s32 index, s32 value) {
  u8 *base = (u8 *)VENT_WORK;

  ((struct VentRow *)(base + index * 20))->value = value;
}

#include "shared/vents_set_id.h"

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u8 gAquaRockInside_SpringLevelCells[] = {67, 66, 75, 66, 83, 66, 91, 66, 99, 66, 107, 66, 115, 66, 67, 81, 75, 81, 83, 81, 91, 81, 99, 81, 107, 81, 115, 81, 67, 96, 75, 96};

u32 gAquaRockInside_SparkScript[] = {
    SC_WAIT(6),
    SC_RELEASE,
    SC_END,
};

u32 gAquaRockInside_TabletGlowScript[] = {
    SC_MOVE_BY(0, 160, 0),
    SC_SYNC,
    SC_RELEASE,
    SC_END,
};

s32 gAquaRockInside_RisingSparkScript[] = {                                                               
      46,                                                                      
      (s32)Orbiter_Spiral,                                                              
      38,                                                                      
      17,                                                                      
  };

s32 gAquaRockInside_RiserScript[] = {                                                               
      46, (s32)Orbit_Update, 46, (s32)Actor_AimAtWatched, 46, (s32)Actor_StepToWatched, 38, 17,                  
  };

u32 gAquaRockInside_TabletRiseScript[] = {
    SC_MOVE_BY(0, 24, 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 4),
    SC_END,
};

u32 gAquaRockInside_PiersRiseScript[] = {
    SC_MOVE_BY(0, 16, 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 4),
    SC_END,
};

s32 gAquaRockInside_TabletSettleScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_MOVE_BY(0, -24, 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 0),
    SC_END,
};

s32 gAquaRockInside_PiersSettleScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_MOVE_BY(0, -16, 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 3),
    SC_END,
};

u32 gAquaRockInside_SplashScript[] = {
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_RELEASE,
};

u16 gAquaRockInside_FirstOutletSpots[] = {
    280, 344, 2, 312, 344, 2, 0,
};

u16 gAquaRockInside_FirstOutletLevels[] = {
    8, 0,
};

u16 gAquaRockInside_SecondOutletSpots[] = {
    152, 536, 2, 216, 536, 2, 0,
};

u32 gAquaRockInside_SecondOutletLevels[] = {
    0xb000a, 0,
};

u32 gAquaRockInside_StreamSplashScript[] = {
    SC_WAIT(20),
    SC_RELEASE,
};

u32 gWaterDropScript[] = {
    SC_ADD(SW_Y, 0x4000),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_RELEASE,
};
// clang-format on

#undef AquaRockInside_SetUpOutlets
#undef VENT_TOUCHING
#undef VENT_BOUNCE
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
#undef DEFINE_AQUA_ROCK_INSIDE_READ_TABLET
#undef SET_BG1CNT_PRIORITY

void AquaRockInside_FightSteam(void);
void AquaRockInside_FreezePuddle(void);
void AquaRockInside_SetPillarCellMiddle(void);
void AquaRockInside_ReadParchTablet(void);

// clang-format off
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
    SCENE_EXIT(1, AquaRockInsideTightrope, 1),
    SCENE_EXIT(2, AquaRockInsideWaterOfLifeChest, 3),
    SCENE_EXIT(3, AquaRockInsideWaterOfLifeChest, 2),
    SCENE_EXIT(4, AquaRockInsideWaterOfLifeChest, 5),
    SCENE_EXIT(5, AquaRockInsideWaterOfLifeChest, 4),
    SCENE_EXIT(6, AquaRockInsideWaterOfLifeChest, 7),
    SCENE_EXIT(7, AquaRockInsideWaterOfLifeChest, 6),
    SCENE_EXIT(8, AquaRockInsideWaterOfLifeChest, 9),
    SCENE_EXIT(9, AquaRockInsideWaterOfLifeChest, 8),
    SCENE_EXIT(10, AquaRockInsideRustySwordChest, 2),
    SCENE_EXIT(11, AquaRockInsideAquariusStone, 1),
    SCENE_EXITS(AquaRockInsideSteam),
    SCENE_EXIT(1, AquaRockInsidePsynergyStone, 1),
    SCENE_EXIT(2, AquaRockInsideSteam, 3),
    SCENE_EXIT(3, AquaRockInsideSteam, 2),
    SCENE_EXIT(4, AquaRockInsideTightrope, 5),
    SCENE_EXIT(5, AquaRockInsideAquariusStone, 2),
    SCENE_EXIT(6, AquaRockInsideAquariusStone, 8),
    SCENE_EXIT(7, AquaRockInsideSteam, 8),
    SCENE_EXIT(8, AquaRockInsideSteam, 7),
    SCENE_EXIT(9, AquaRockInsideAquariusStone, 6),
    SCENE_EXITS(AquaRockInsideAquariusStone),
    SCENE_EXIT(1, AquaRockInsideWaterOfLifeChest, 11),
    SCENE_EXIT(2, AquaRockInsideSteam, 5),
    SCENE_EXIT(3, AquaRockInsideParchTablet, 1),
    SCENE_EXIT(4, AquaRockInsideRustySwordChest, 4),
    SCENE_EXIT(5, AquaRockInsideRustySwordChest, 5),
    SCENE_EXIT(6, AquaRockInsideSteam, 9),
    SCENE_EXIT(7, AquaRockInsidePsynergyStone, 4),
    SCENE_EXIT(8, AquaRockInsideSteam, 6),
    SCENE_EXITS(AquaRockInsideRustySwordChest),
    SCENE_EXIT(1, AquaRockMoai, 4),
    SCENE_EXIT(2, AquaRockInsideWaterOfLifeChest, 10),
    SCENE_EXIT(3, AquaRockInsideTearStone, 2),
    SCENE_EXIT(4, AquaRockInsideAquariusStone, 4),
    SCENE_EXIT(5, AquaRockInsideAquariusStone, 5),
    SCENE_EXIT(6, AquaRockInsideCrystalPowderChest, 6),
    SCENE_EXIT(7, AquaRockInsideRustySwordChest, 8),
    SCENE_EXIT(8, AquaRockInsideRustySwordChest, 7),
    SCENE_EXIT(9, AquaRockInsideTightrope, 4),
    SCENE_EXITS(AquaRockInsideCrystalPowderChest),
    SCENE_EXIT(1, AquaRockInsideVialChest, 1),
    SCENE_EXIT(2, AquaRockInsideVialChest, 2),
    SCENE_EXIT(3, AquaRockInsideVialChest, 3),
    SCENE_EXIT(4, AquaRockInsideCrystalPowderChest, 5),
    SCENE_EXIT(5, AquaRockInsideCrystalPowderChest, 4),
    SCENE_EXIT(6, AquaRockInsideRustySwordChest, 6),
    SCENE_EXIT(7, AquaRockInsideVialChest, 12),
    SCENE_EXITS(AquaRockInsideVialChest),
    SCENE_EXIT(1, AquaRockInsideCrystalPowderChest, 1),
    SCENE_EXIT(2, AquaRockInsideCrystalPowderChest, 2),
    SCENE_EXIT(3, AquaRockInsideCrystalPowderChest, 3),
    SCENE_EXIT(4, AquaRockInsideVialChest, 5),
    SCENE_EXIT(5, AquaRockInsideVialChest, 4),
    SCENE_EXIT(6, AquaRockInsideVialChest, 7),
    SCENE_EXIT(7, AquaRockInsideVialChest, 6),
    SCENE_EXIT(8, AquaRockInsideVialChest, 9),
    SCENE_EXIT(9, AquaRockInsideVialChest, 8),
    SCENE_EXIT(10, AquaRockInsideVialChest, 11),
    SCENE_EXIT(11, AquaRockInsideVialChest, 10),
    SCENE_EXIT(12, AquaRockInsideCrystalPowderChest, 7),
    SCENE_EXIT(13, AquaRockMistSabreChest, 1),
    SCENE_EXITS(AquaRockInsidePsynergyStone),
    SCENE_EXIT(1, AquaRockInsideSteam, 1),
    SCENE_EXIT(2, AquaRockInsideTightrope, 3),
    SCENE_EXIT(3, AquaRockInsideTightrope, 2),
    SCENE_EXIT(4, AquaRockInsideAquariusStone, 7),
    SCENE_EXITS(AquaRockInsideTightrope),
    SCENE_EXIT(1, AquaRockInsideWaterOfLifeChest, 1),
    SCENE_EXIT(2, AquaRockInsidePsynergyStone, 3),
    SCENE_EXIT(3, AquaRockInsidePsynergyStone, 2),
    SCENE_EXIT(4, AquaRockInsideRustySwordChest, 9),
    SCENE_EXIT(5, AquaRockInsideSteam, 4),
    SCENE_EXIT(6, AquaRockInsideTearStone, 4),
    SCENE_EXIT(7, AquaRockInsideTearStone, 3),
    SCENE_EXIT(8, AquaRockInsideTearStone, 1),
    SCENE_EXITS(AquaRockInsideTearStone),
    SCENE_EXIT(1, AquaRockInsideTightrope, 8),
    SCENE_EXIT(2, AquaRockInsideRustySwordChest, 3),
    SCENE_EXIT(3, AquaRockInsideTightrope, 7),
    SCENE_EXIT(4, AquaRockInsideTightrope, 6),
    SCENE_EXITS(AquaRockInsideParchTablet),
    SCENE_EXIT(1, AquaRockInsideAquariusStone, 3),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gAquaRockInside_Placements[] = {
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_WaterOfLifeChestPlacements[] = {
    ACTOR_PLACEMENT(380, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 679, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(380, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 551, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(292, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_SteamPlacements[] = {
    ACTOR_PLACEMENT(380, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 247, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_STEAM), MOVEMENT_TYPE_STAND, 776, 0, 264, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_AquariusStonePlacements[] = {
    ACTOR_PLACEMENT(383, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_RustySwordChestPlacements[] = {
    ACTOR_PLACEMENT(383, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(380, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 647, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(380, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 87, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(380, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 103, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_CrystalPowderChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(383, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_VialChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(380, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 631, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_PsynergyStonePlacements[] = {
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_TightropePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_TearStonePlacements[] = {
    ACTOR_PLACEMENT(383, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(380, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 231, FACING_SOUTH, TALK_FACING_KEEP, 0),
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

LATE_DATA struct ActorPlacement gAquaRockInside_ParchTabletPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {323, CONDITION_ALWAYS, 1, 0x64, 0, 0, FACING_SOUTH, 2, 2},
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gAquaRockInside_Triggers[] = {
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gAquaRockInside_WaterOfLifeChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x51, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x51, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x51, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 50, CONDITION_ALWAYS, EVENT(AquaRockInside_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 50, CONDITION_ALWAYS, EVENT(AquaRockInside_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 51, CONDITION_ALWAYS, EVENT(AquaRockInside_JumpForward)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WATER_OF_LIFE_CHEST_WEST_VENT_PILLAR, FLAG_AQUA_ROCK_WEST_VENT_COVERED, EVENT(AquaRockInside_PlacePillarOverVent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WATER_OF_LIFE_CHEST_EAST_VENT_PILLAR, FLAG_AQUA_ROCK_EAST_VENT_COVERED, EVENT(AquaRockInside_PlacePillarOverVent)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AquaRockInside_PlacePillarOverVent)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_WATER_OF_LIFE_CHEST_EAST_MOAI, FLAG_SCENE_TEMP_30, EVENT(AquaRockInside_DouseWaterOfLifeChestEastMoai)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_WATER_OF_LIFE_CHEST_WEST_MOAI, FLAG_SCENE_TEMP_31, EVENT(AquaRockInside_DouseWaterOfLifeChestWestMoai)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WATER_OF_LIFE_CHEST_WATER_PILLAR, CONDITION_ALWAYS, (s32)((u8 *)AquaRockInside_DoNothing + 1)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_WATER_OF_LIFE_CHEST_PUDDLE, FLAG_SCENE_TEMP_50, EVENT(AquaRockInside_FreezePuddle)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s32 gAquaRockInside_SpringLevel = 256;

LATE_DATA struct Trigger gAquaRockInside_SteamTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {0x51, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {0x51, 8, CONDITION_ALWAYS, 8},
    {0x51, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_STEP, 40, CONDITION_ALWAYS, EVENT(AquaRockInside_FlowWithStream)},
    {TRIGGER_TALK, ACTOR_STEAM, CONDITION_ALWAYS, EVENT(AquaRockInside_FightSteam)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STEAM_ROOM_PILLAR, FLAG_AQUA_ROCK_PILLAR_DROPPED_BY_MOAI, EVENT(AquaRockInside_MovePillarBeforeMoai)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AquaRockInside_MovePillarBeforeMoai)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 88, FLAG_SCENE_TEMP_40, EVENT(AquaRockInside_ParchSpring)},
    {TRIGGER_RAISED, 88, FLAG_SCENE_TEMP_40, EVENT(AquaRockInside_SpringDries)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gAquaRockInside_AquariusStoneTriggers[] = {
    {0x51, 1, CONDITION_ALWAYS, 1},
    {0x51, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK, 3, CONDITION_ALWAYS, EVENT(AquaRockInside_StepOnLoosePlank)},
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

LATE_DATA struct Trigger gAquaRockInside_RustySwordChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x51, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {0x51, 4, CONDITION_ALWAYS, 4},
    {0x51, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {STEP_TRIGGER_WALKING, 55, CONDITION_ALWAYS, EVENT(AquaRockInside_JumpForward)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A, CONDITION_ALWAYS, EVENT(AquaRockInside_SetPillarCellHigh)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AquaRockInside_SetPillarCellHigh)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RUSTY_SWORD_CHEST_WATER_PILLAR_A, CONDITION_ALWAYS, EVENT(AquaRockInside_SetPillarCellMiddle)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AquaRockInside_SetPillarCellMiddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 50, FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_POOL_PARCHED, EVENT(AquaRockInside_ParchPool)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_RUSTY_SWORD_CHEST_WATER_STONE, FLAG_AQUA_ROCK_RUSTY_SWORD_CHEST_STREAMS_TURNED, EVENT(AquaRockInside_DouseRustySwordChestWaterStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_RUSTY_SWORD_CHEST_MOAI, FLAG_SCENE_TEMP_49, EVENT(AquaRockInside_DouseRustySwordChestMoai)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gAquaRockInside_CrystalPowderChestTriggers[] = {
    {0x51, 1, CONDITION_ALWAYS, 1},
    {0x51, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {0x51, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, 77, CONDITION_ALWAYS, EVENT(AquaRockInside_StepIntoRapids)},
    {TRIGGER_STEP, 78, CONDITION_ALWAYS, EVENT(AquaRockInside_StepIntoRapids)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 70, CONDITION_ALWAYS, EVENT(AquaRockInside_JumpForward)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CRYSTAL_POWDER_CHEST_PILLAR, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_CRYSTAL_POWDER_CHEST_WATER_STONE, FLAG_AQUA_ROCK_CRYSTAL_POWDER_WATER_STONE_DOUSED, EVENT(AquaRockInside_DouseCrystalPowderChestWaterStone)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gAquaRockInside_VialChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {0x51, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {0x51, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x51, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {0x51, 11, CONDITION_ALWAYS, 11},
    {0x51, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 13, CONDITION_ALWAYS, EVENT(AquaRockInside_RideFalls)},
    {TRIGGER_STEP, 77, CONDITION_ALWAYS, EVENT(AquaRockInside_StepIntoRapids)},
    {TRIGGER_STEP, 78, CONDITION_ALWAYS, EVENT(AquaRockInside_StepIntoRapids)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_FALLS_PILLAR, FLAG_SCENE_TEMP_38, EVENT(AquaRockInside_MovePillarByFalls)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_VIAL_CHEST_PUDDLE_A, FLAG_SCENE_TEMP_51, EVENT(AquaRockInside_FreezeRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_VIAL_CHEST_PUDDLE_B, FLAG_SCENE_TEMP_52, EVENT(AquaRockInside_FreezeRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_VIAL_CHEST_PUDDLE_C, FLAG_SCENE_TEMP_53, EVENT(AquaRockInside_FreezeRoomPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_RAPIDS_PILLAR_A, CONDITION_ALWAYS, EVENT(AquaRockInside_StartPushingRock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_RAPIDS_PILLAR_B, CONDITION_ALWAYS, EVENT(AquaRockInside_StartPushingRock)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AquaRockInside_StartPushingRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_RAPIDS_PILLAR_A, CONDITION_ALWAYS, EVENT(AquaRockInside_FinishPushingRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_RAPIDS_PILLAR_B, CONDITION_ALWAYS, EVENT(AquaRockInside_FinishPushingRock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AquaRockInside_FinishPushingRock)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gAquaRockInside_PsynergyStoneTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXAMINE, 30, CONDITION_ALWAYS, EVENT(AquaRockInside_CheckPsynergyStone)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s16 gAquaRockInside_TightropeTriggers[] = {
    49, 0, 1, -1, 1, 0, 49, 0,
    2, -1, 2, 0, 49, 0, 3, -1,
    3, 0, 33, 0, 4, -1, 4, 0,
    49, 0, 5, -1, 5, 0, 1, 0,
    6, -1, 6, 0, 1, 0, 7, -1,
    7, 0, 1, 0, 8, -1, 8, 0,
    -1, -1, 0, 0, 0, 0,
};

LATE_DATA struct Trigger gAquaRockInside_TearStoneTriggers[] = {
    {0x51, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {0x51, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 40, CONDITION_ALWAYS, EVENT(AquaRockInside_FlowWithStream)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TEAR_STONE_PILLAR_A, CONDITION_ALWAYS, EVENT(AquaRockInside_MovePuzzlePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TEAR_STONE_WATER_PILLAR_C, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TEAR_STONE_WATER_PILLAR_D, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TEAR_STONE_WATER_PILLAR_F, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_TEAR_STONE_WATER_STONE, FLAG_AQUA_ROCK_TEAR_STONE_STREAMS_TURNED, EVENT(AquaRockInside_DouseTearStoneWaterStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_TEAR_STONE_MOAI, FLAG_SCENE_TEMP_4B, EVENT(AquaRockInside_DouseTearStoneMoai)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gAquaRockInside_ParchTabletTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 30, CONDITION_ALWAYS, EVENT(AquaRockInside_ReadParchTablet)},
    {EXAMINE_TRIGGER_FACING(FACING_SOUTH), 30, CONDITION_ALWAYS, EVENT(AquaRockInside_WalkToParchTablet)},
    {EXAMINE_TRIGGER_FACING(FACING_WEST), 30, CONDITION_ALWAYS, EVENT(AquaRockInside_ReadTabletFacingWest)},
    {EXAMINE_TRIGGER_FACING(FACING_EAST), 30, CONDITION_ALWAYS, EVENT(AquaRockInside_ReadTabletFacingEast)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 40, IF_FLAG_SET(0x349), EVENT(AquaRockInside_ParchLastPool)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s8 gAquaRockInside_StreamTurns[] = {
    8, 8, 0, 8, -1, 8, 1, 8,
    8, 0, 8, -1, 1, 8, 8, 8,
    8, 8, 0, 1, 8, -1, 8, 8,
    8, 0, 8, 8, 8, 1, -1, 8,
};
// clang-format on
