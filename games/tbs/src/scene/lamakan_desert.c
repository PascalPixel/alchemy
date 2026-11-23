/// @file
/// Lamakan Desert, south of Lama Temple: four scenes that share one trigger
/// table, built around a heat gauge and the secrets Reveal uncovers.
///
/// The party crosses it from the east. The first screen
/// (SceneId_LamakanDesertEast) is entered from the World Map in its south-east
/// corner and leads north. The second, the widest (SceneId_LamakanDesertSmog),
/// is scattered with rings of rocks, cacti and skeletons, and hides Smog, a
/// Jupiter Djinni; it leads out west. The third
/// (SceneId_LamakanDesertManticore) has rivers of sand that carry the party
/// south, with sandfalls at their heads. Behind two of them lie passages into a
/// cave (SceneId_LamakanDesertCave): the east and south ones into a small
/// cavern, and the north-west one, which the Manticore guards, into a corridor
/// that leads out west to the World Map. The cave has no heat gauge.
///
/// Walking on the sand fills a thermometer down the screen's left edge
/// (HeatGauge_Draw), faster on the run. As it passes a quarter, a half and
/// three quarters, Garet, Ivan or Mia complains of the heat
/// (LamakanDesert_ComplainAboutHeat); when it is full, the party loses a
/// quarter of its HP and it starts again. The steps are counted in
/// gMainRoutineWork.hazardSteps against HEAT_LIMIT.
///
/// Most rings of rocks hide something only Reveal shows: an oasis, an Ant
/// Lion's pit, a chest or a Psynergy Stone, and in the second screen Smog.
/// On entry each hidden patch of the map is saved aside and covered with
/// plain sand under a shimmering heat haze (Reveal_HideSecrets); Reveal brings
/// them back and stills the haze (Reveal_ShowSecrets) until it wears off. The
/// oases and the pits are there only while Reveal shines, as the monks of Lama
/// Temple warn: `Reveal will help you find oases to cool you against the
/// heat.` Stepping into an oasis, Isaac hops in and bathes until the heat has
/// drained (LamakanDesert_BatheInOasis); stepping by a pit, he is pulled into
/// it and an Ant Lion attacks (LamakanDesert_FallIntoPit).
///
/// In the third screen Reveal also parts the sandfalls over the passages, and
/// shows the Manticore standing in the north-west one. Until it is beaten
/// (FLAG_LAMAKAN_MANTICORE_BEATEN), coming near makes it roar and lunge at
/// Isaac (LamakanDesert_MeetManticore), and talking to it starts its battle.
///
/// Leaving the corridor for the World Map while Hsu is still trapped at the
/// Alpine Crossing (FLAG_HSU_RESCUED clear) frees him without the party: it
/// sets FLAG_HSU_RESCUED and FLAG_HSU_RESCUED_WITHOUT_PARTY, and in Xian
/// Feizhi then thanks Master Hama alone (lacquer_rooms_b.c).

#include "ability_ids.h"
#include "djinn_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/scene.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/display_transition.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/field_effect_spawn.h"
#include "scene/import.h"
#include "sound/sound.h"
#include "system/dma.h"
#include "system/point.h"
#include "system/io_write.h"
#include "system/io_write_queue.h"
#include "system/iwram_call.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/vram_cache.h"
#include "text/msg_ids.h"

/// The heat gauge's picture, 36 tiles in two columns of 18; its tile buffer
/// holds two, the bar and then the frame laid over it.
#define HEAT_GAUGE_TILE_BYTES (36 * TILE_SIZE_4BPP)
/// The heat gauge's levels: the party's steps scaled from 0 to
/// HEAT_GAUGE_LEVELS by the area's hazard limit.
#define HEAT_GAUGE_LEVELS 120
/// The gauge's place on the screen: 16 pixels wide, from line 8 down four
/// 16x32 sprites and a 16x16 bulb.
#define HEAT_GAUGE_WIDTH 16
#define HEAT_GAUGE_TOP 8
#define HEAT_GAUGE_HEIGHT (4 * 32 + 16)
/// The gauge's tube in its pictures, from the row below its cap to the row
/// above its bulb; the heat fills it from below.
#define HEAT_GAUGE_TUBE_TOP 12
#define HEAT_GAUGE_TUBE_FOOT 128
/// The word of the gauge's tiles that holds the left half of pixel row `row`:
/// each band of eight rows is a left tile, then a right one, eight words each.
#define HEAT_GAUGE_ROW_WORD(row) ((row) / 8 * 16 + (row) % 8)
/// The sprite palette the gauge is drawn in, its colour for the tube's empty
/// part (black), and the first of the six colours the heat reddens.
#define HEAT_GAUGE_PALETTE 14
#define HEAT_GAUGE_EMPTY_COLOR 14
#define HEAT_GAUGE_TINTED_COLOR 6
#define HEAT_GAUGE_TINTED_COLORS 6
/// The most the gauge's green brightens, at full heat, on its flickering
/// frames past half.
#define HEAT_GAUGE_GREEN_FLICKER 32

/// The steps of heat at which the party loses HEAT_DAMAGE_PERCENT of its HP
/// and the count starts again (gMainRoutineWork.hazardLimit).
#define HEAT_LIMIT 600
#define HEAT_DAMAGE_PERCENT 25
/// The gauge's marks, in percent of HEAT_LIMIT, past which Garet, Ivan or Mia
/// complains of the heat.
enum {
  HEAT_QUARTER_PERCENT = 25,
  HEAT_HALF_PERCENT = 50,
  HEAT_THREE_QUARTERS_PERCENT = 75
};

/// The VRAM cache entry holding the gauge's tiles.
s16 gHeatGauge_VramSlot BSS_DATA ALIGNED(16);
u8 gLamakanDesert_UnusedAfterGaugeSlot[14] BSS_DATA;
/// The gauge's five sprites: four 16x32 ones down the screen's left edge and a
/// 16x16 cap at its foot.
struct MenuSprite gHeatGauge_Sprites[5] BSS_DATA;
u8 gLamakanDesert_UnusedAfterGaugeSprites[20] BSS_DATA;
/// The gauge's decoded tiles, which each frame's drawing starts from.
u8 gHeatGauge_TileBuffer[2 * HEAT_GAUGE_TILE_BYTES] BSS_DATA;
/// 1 while the gauge is held on screen (HeatGauge_SetHeld).
s16 gHeatGauge_Held BSS_DATA;

void HeatGauge_Start(void);
/// The middles of the oases, where a leader stepping into one bathes: x and z
/// of each, in 16.16 fixed-point pixels.
extern const FIXED gLamakanDesertEast_Oases[];
extern const FIXED gLamakanDesertSmog_Oases[];
extern const FIXED gLamakanDesertOasesOther[];
extern struct SceneEntrance gLamakanDesertEntrancesOther[];
extern struct SceneEntrance gLamakanDesertEastEntrances[];
extern struct SceneEntrance gLamakanDesertSmogEntrances[];
extern struct SceneEntrance gLamakanDesertManticoreEntrances[];
extern struct SceneEntrance gLamakanDesertCaveEntrances[];
extern u32 gLamakanDesertExits[];
extern struct ActorPlacement gLamakanDesertActorPlacementsOther[];
extern struct ActorPlacement gLamakanDesertEastActorPlacements[];
extern struct ActorPlacement gLamakanDesertSmogActorPlacements[];
extern struct ActorPlacement gLamakanDesertManticoreActorPlacements[];
extern struct Trigger gLamakanDesertTriggers[];
extern s16 gHeatGauge_Level;
extern s16 gHeatGauge_Shown;
extern s16 gHeatGauge_Flash;

static void HeatGauge_SetHeld(u16 held);

/// The values HeatGauge_RaiseQuarterTriggers raises as the heat gauge passes
/// each mark.
enum LamakanDesertRaisedTrigger {
  LAMAKAN_DESERT_RAISED_HEAT_QUARTER = 1,
  LAMAKAN_DESERT_RAISED_HEAT_HALF = 2,
  LAMAKAN_DESERT_RAISED_HEAT_THREE_QUARTERS = 3
};

/// The cell behaviors of the desert's ground.
enum LamakanDesertCellBehavior {
  /// An oasis's water, in the patches Reveal shows.
  CELL_BEHAVIOR_LAMAKAN_DESERT_OASIS = 20,
  CELL_BEHAVIOR_LAMAKAN_DESERT_SAND =
      21, ///< sand, where each moving frame adds heat
  CELL_BEHAVIOR_LAMAKAN_DESERT_PIT =
      22, ///< by an Ant Lion's pit, under Reveal too
  CELL_BEHAVIOR_LAMAKAN_DESERT_SLOPE =
      30, ///< sand that carries the leader south
  /// The way out of the corridor behind the Manticore onto the World Map, at
  /// its west end, cell (8, 19).
  CELL_BEHAVIOR_LAMAKAN_DESERT_TO_WORLD_MAP = 9,
  /// The sand before the north-west passage of the third screen, which the
  /// Manticore guards, once Reveal has parted the sandfall over it.
  CELL_BEHAVIOR_LAMAKAN_DESERT_MANTICORE = 40
};

enum {
  /// FLAG_SCENE_TEMP_00: a bath, a pit or the Manticore is under way.
  FLAG_DESERT_EVENT_RUNNING = FLAG_SCENE_TEMP_00,
  /// FLAG_SCENE_TEMP_01: Reveal shows the pits.
  FLAG_SECRETS_REVEALED = FLAG_SCENE_TEMP_01,
  /// The heat gauge has passed a quarter, where the first complaints come.
  FLAG_HEAT_QUARTER = FLAG_AREA_TEMP_00,
  FLAG_HEAT_HALF = FLAG_AREA_TEMP_01,
  FLAG_HEAT_THREE_QUARTERS = FLAG_AREA_TEMP_02,
  /// Who has complained since the gauge last passed a mark: one per party
  /// member, in the order of their actors.
  FLAG_GARET_COMPLAINED = FLAG_AREA_TEMP_03,
  FLAG_IVAN_COMPLAINED = FLAG_AREA_TEMP_04,
  FLAG_MIA_COMPLAINED = FLAG_AREA_TEMP_05
};

/// The actors, numbered in the order of each scene's actor placements from
/// ACTOR_FIRST_PLACED. LamakanDesertEast to LamakanDesertManticore place the
/// Manticore, then the LamakanDesertEast and LamakanDesertManticore one pit and
/// LamakanDesertSmog all four pits and Smog. LamakanDesertCave places none.
enum {
  /// The Manticore, until FLAG_LAMAKAN_MANTICORE_BEATEN. Talking to it starts
  /// its battle.
  ACTOR_MANTICORE = ACTOR_FIRST_PLACED,
  /// The first of the Ant Lions' pits Reveal shows, up to ACTOR_LAST_PIT.
  ACTOR_FIRST_PIT = ACTOR_MANTICORE + 1,
  ACTOR_LAST_PIT = ACTOR_FIRST_PIT + 3,
  /// Smog, a Jupiter Djinni.
  ACTOR_SMOG = ACTOR_LAST_PIT + 1
};

/// The desert's two Psynergy Stones, area temp flags. Each is back once the
/// party has left Lamakan Desert.
enum {
  FLAG_LAMAKAN_DESERT_FIRST_PSYNERGY_STONE = FLAG_AREA_TEMP_50,
  FLAG_LAMAKAN_DESERT_SECOND_PSYNERGY_STONE = FLAG_AREA_TEMP_51
};

// A secret of the east and Smog scenes is a 4-by-2 patch of the map grid. On
// entry each is saved to a spare patch of its own on row 126, numbered from
// the west from column 64; hidden, it is drawn over with the plain sand at
// (70, 68); Reveal copies it back.
#define SECRET_WIDTH 4
#define SECRET_HEIGHT 2
#define SECRET_SPARE_FIRST_COLUMN 64
#define SECRET_SPARE_COLUMN(secret)                                            \
  (SECRET_SPARE_FIRST_COLUMN + SECRET_WIDTH * (secret))
#define SECRET_SPARE_ROW 126
#define PLAIN_SAND_COLUMN 70
#define PLAIN_SAND_ROW 68
/// Saves the secret whose top-left cell is (column, row) to its spare patch.
#define SAVE_SECRET(secret, column, row)                                       \
  MapGrid_CopyCells(column, row, SECRET_WIDTH, SECRET_HEIGHT,                  \
                    SECRET_SPARE_COLUMN(secret), SECRET_SPARE_ROW)
/// Draws plain sand over a secret.
#define HIDE_SECRET(column, row)                                               \
  MapGrid_CopyCells(PLAIN_SAND_COLUMN, PLAIN_SAND_ROW, SECRET_WIDTH,           \
                    SECRET_HEIGHT, column, row)
/// Copies a secret back from its spare patch.
#define SHOW_SECRET(secret, column, row)                                       \
  MapGrid_CopyCells(SECRET_SPARE_COLUMN(secret), SECRET_SPARE_ROW,             \
                    SECRET_WIDTH, SECRET_HEIGHT, column, row)
// A pit's secret also changes the ground a row further south: hidden, the row
// two below the patch's top takes the sand's ground; shown, the two rows from
// one below its top take the ground of the saved patch's two rows.
#define HIDE_SECRET_SOUTH_ROW(column, row)                                     \
  MapGrid_CopyCellAttributes(PLAIN_SAND_COLUMN, PLAIN_SAND_ROW, SECRET_WIDTH,  \
                             1, column, (row) + 2)
#define SHOW_SECRET_SOUTH_ROWS(secret, column, row)                            \
  MapGrid_CopyCellAttributes(SECRET_SPARE_COLUMN(secret), SECRET_SPARE_ROW,    \
                             SECRET_WIDTH, 2, column, (row) + 1)

/// LamakanDesertEast's secrets, numbered as their spare patches are, each
/// named for what Reveal uncovers in it: an oasis's pool (around its middle in
/// gLamakanDesertEast_Oases), a pit, or a treasure spot.
enum LamakanDesertEastSecret {
  EAST_SECRET_OASIS_NORTH,
  EAST_SECRET_VULCAN_AXE,
  EAST_SECRET_PIT,
  EAST_SECRET_OASIS_SOUTHWEST,
  EAST_SECRET_OASIS_SOUTHEAST,
  EAST_SECRET_FIRST_PSYNERGY_STONE
};

/// LamakanDesertSmog's secrets, the same way: five oases, the four pits of
/// ACTOR_FIRST_PIT to ACTOR_LAST_PIT, Smog on TREASURE_SPOT_UNDER_SMOG, and the
/// chests and Psynergy Stone.
enum LamakanDesertSmogSecret {
  SMOG_SECRET_OASIS_NORTHEAST,
  SMOG_SECRET_PIT_NORTH,
  SMOG_SECRET_LUCKY_PEPPER,
  SMOG_SECRET_SECOND_PSYNERGY_STONE,
  SMOG_SECRET_PIT_WEST,
  SMOG_SECRET_PIT_EAST,
  SMOG_SECRET_SMOG,
  SMOG_SECRET_OASIS_MIDDLE,
  SMOG_SECRET_777_COINS,
  SMOG_SECRET_OASIS_EAST,
  SMOG_SECRET_PIT_SOUTH,
  SMOG_SECRET_OASIS_SOUTHWEST,
  SMOG_SECRET_OASIS_SOUTH,
  SMOG_SECRET_POTION
};

/// The desert's treasure spots, whose cells Reveal uncovers with their
/// secrets: the chests and Psynergy Stones their examine triggers find, and
/// the cell under Smog, which no examine trigger answers. Each is sand
/// underfoot as well.
enum LamakanDesertTreasureSpot {
  TREASURE_SPOT_VULCAN_AXE = TREASURE_SPOT_FIRST_ID, ///< LamakanDesertEast
  TREASURE_SPOT_FIRST_PSYNERGY_STONE,                ///< LamakanDesertEast
  TREASURE_SPOT_LUCKY_PEPPER,                        ///< LamakanDesertSmog
  TREASURE_SPOT_SECOND_PSYNERGY_STONE,               ///< LamakanDesertSmog
  TREASURE_SPOT_UNDER_SMOG,                          ///< LamakanDesertSmog
  TREASURE_SPOT_777_COINS,                           ///< LamakanDesertSmog
  TREASURE_SPOT_POTION,                              ///< LamakanDesertSmog
  TREASURE_SPOT_MINT                                 ///< LamakanDesertManticore
};

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/// Talking to Smog, the Jupiter Djinni Reveal uncovers in a ring of rocks east
/// of the middle of Lamakan Desert's second screen, makes it join the party.
static void LamakanDesert_TalkToSmog(void) {
  Djinn_JoinParty(ACTOR_SMOG, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_SMOG));
}

#include "shared/actor_wait_below_height.c"

/// The update a leader pulled into an Ant Lion's pit runs: sand sounds every 8
/// frames and a spray of sand every 16.
static UNREAD_RESULT LamakanDesert_KickUpSand(struct Actor *actor) {
  struct EffectOptions params;
  s32 phase;

  if ((gFrameCount & 7) == 0) {
    Sound_Play(SE_FOOTSTEP);
  }
  phase = gFrameCount & 15;
  if (phase != 0) {
    RETURN_UNREAD(0);
  }
  params.startScaleX = 4 * FIXED_ONE / 5;
  params.startScaleY = 4 * FIXED_ONE / 5;
  params.spin = FIELD_EFFECT_RANDOM_SPIN();
  FieldEffect_Spawn(
      actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, phase, phase,
      EFFECT_USE_SPIN | EFFECT_USE_START_SCALE | EFFECT_SCRIPT_MEDIUM, &params);
  RETURN_UNREAD(0);
}

/// Where the party can enter each of the four scenes.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_LAMAKAN_DESERT_EAST) {
    return gLamakanDesertEastEntrances;
  }
  if (sceneId == SCENE_LAMAKAN_DESERT_SMOG) {
    return gLamakanDesertSmogEntrances;
  }
  if (sceneId == SCENE_LAMAKAN_DESERT_MANTICORE) {
    return gLamakanDesertManticoreEntrances;
  }
  if (sceneId == SCENE_LAMAKAN_DESERT_CAVE) {
    return gLamakanDesertCaveEntrances;
  }
  return gLamakanDesertEntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

/// All four desert scenes share the packed exit table.
const u32 *Scene_GetExits(void) { return gLamakanDesertExits; }

/// The pits, the Manticore and Smog of each scene.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_LAMAKAN_DESERT_MANTICORE) {
    if (gMainRoutineWork.entrance ==
        LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE) {
      GameFlag_Set(FLAG_LAMAKAN_MANTICORE_BEATEN);
      // The French, Spanish and Italian editions mark Kalay reached here too.
      // In play it is already set: every Kalay scene, the tunnel's entrance
      // among them, sets it on entry (kalay.c), and the ship to Tolbi sails
      // only once the tour group has left Kalay. XXX (no evidence): what the
      // patch guards against; no route in the code reaches this battle with
      // the flag clear, and the editions say nothing of why.
#if TBS_PATCH_3
      GameFlag_Set(FLAG_KALAY_REACHED);
#endif
    }
  }
  if (gMainRoutineWork.next == SCENE_LAMAKAN_DESERT_EAST) {
    return gLamakanDesertEastActorPlacements;
  }
  if (gMainRoutineWork.next == SCENE_LAMAKAN_DESERT_SMOG) {
    return gLamakanDesertSmogActorPlacements;
  }
  if (gMainRoutineWork.next == SCENE_LAMAKAN_DESERT_MANTICORE) {
    return gLamakanDesertManticoreActorPlacements;
  }
  return gLamakanDesertActorPlacementsOther;
}

/// Walking out of the west end of the cave corridor behind the Manticore (cell
/// (8, 19)) leaves Lamakan Desert for the World Map. If Hsu is still trapped
/// at the Alpine Crossing, he is freed without the party from then on.
static void LamakanDesert_LeaveForTheWorldMap(void) {
  Event_Start();
  if (Event_GetFlag(FLAG_HSU_RESCUED) == 0) {
    if (Event_GetFlag(FLAG_HSU_RESCUED_WITHOUT_PARTY) == 0) {
      GameFlag_Set(FLAG_HSU_RESCUED_WITHOUT_PARTY);
      GameFlag_Set(FLAG_HSU_RESCUED);
    }
  }
  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(LAMAKAN_DESERT_CAVE_TO_WORLD_MAP);
  Event_End();
}

/// The desert's triggers, the same in every scene.
const struct Trigger *Scene_GetTriggers(void) { return gLamakanDesertTriggers; }

/// Reveal wears off, and the desert's secrets sink under the sand again.
static UNREAD_RESULT Reveal_HideSecrets(void) {
  s32 markerId;

  GameFlag_Clear(FLAG_SECRETS_REVEALED);
  if (gMainRoutineWork.next == SCENE_LAMAKAN_DESERT_EAST) {
    HIDE_SECRET(22, 7);
    HIDE_SECRET(8, 10);
    HIDE_SECRET(23, 21);
    HIDE_SECRET_SOUTH_ROW(23, 21);
    HIDE_SECRET(16, 42);
    HIDE_SECRET(36, 44);
    HIDE_SECRET(14, 55);
  } else if (gMainRoutineWork.next == SCENE_LAMAKAN_DESERT_SMOG) {
    HIDE_SECRET(42, 5);
    HIDE_SECRET(20, 11);
    HIDE_SECRET_SOUTH_ROW(20, 11);
    HIDE_SECRET(14, 12);
    HIDE_SECRET(56, 18);
    HIDE_SECRET(7, 22);
    HIDE_SECRET_SOUTH_ROW(7, 22);
    HIDE_SECRET(44, 23);
    HIDE_SECRET_SOUTH_ROW(44, 23);
    HIDE_SECRET(38, 24);
    HIDE_SECRET(26, 28);
    HIDE_SECRET(17, 35);
    HIDE_SECRET(50, 36);
    HIDE_SECRET(34, 43);
    HIDE_SECRET_SOUTH_ROW(34, 43);
    HIDE_SECRET(6, 46);
    HIDE_SECRET(27, 55);
    HIDE_SECRET(43, 56);
  } else if (gMainRoutineWork.next == SCENE_LAMAKAN_DESERT_MANTICORE) {
    // Sand fills the three secrets in the west, north to south the Mint's
    // treasure spot, the pit and the oasis. Sandfalls close over the two
    // passages at the head of the slopes of sand, whose cells there are plain
    // slope again, and the ground before the west one no longer raises the
    // Manticore.
    MapGrid_CopyCellsLayered(69, 99, 4, 2, 8, 16);
    MapGrid_CopyCellsLayered(69, 99, 4, 2, 6, 20);
    MapGrid_CopyCellsLayered(69, 99, 4, 2, 10, 23);
    MapGrid_CopyCellAttributes(69, 99, 4, 2, 8, 14);
    MapGrid_CopyCellAttributes(69, 99, 4, 2, 6, 18);
    MapGrid_CopyCellAttributes(69, 99, 4, 1, 6, 20);
    MapGrid_CopyCellAttributeRect(69, 99, 4, 2, 10, 21);
    MapGrid_CopyCells(0, 121, 5, 7, 8, 32);
    MapGrid_CopyCells(0, 121, 5, 7, 43, 32);
    MapGrid_CopyCells(6, 120, 3, 1, 9, 5);
    MapGrid_CopyCells(9, 120, 3, 1, 44, 5);
    MapGrid_CopyCellAttributes(9, 0, 3, 3, 9, 6);
  }
  Actor_SetPosition(ACTOR_MANTICORE, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_PIT, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_PIT + 1, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_PIT + 2, 0, 0);
  Actor_SetPosition(ACTOR_LAST_PIT, 0, 0);
  Actor_SetPosition(ACTOR_SMOG, 0, 0);
  for (markerId = TREASURE_SPOT_VULCAN_AXE; markerId <= TREASURE_SPOT_MINT;
       markerId++) {
    TreasureSpot_SetActorPosition(markerId, 0, 0);
  }
  if (gMainRoutineWork.next != SCENE_LAMAKAN_DESERT_CAVE) {
    LineScroll_StartWave(0, FIXED_ONE * 4, FIXED_ONE, FIXED_ONE / 8, FIXED_ONE,
                         FIXED_ONE / 2, FIXED_ONE / 4);
  }
}

/// Casting Reveal uncovers what the desert's sand hides.
static UNREAD_RESULT Reveal_ShowSecrets(void) {
  s32 i;

  GameFlag_Clear(FLAG_DESERT_EVENT_RUNNING);
  GameFlag_Set(FLAG_SECRETS_REVEALED);
  if (gMainRoutineWork.next == SCENE_LAMAKAN_DESERT_EAST) {
    SHOW_SECRET(EAST_SECRET_OASIS_NORTH, 22, 7);
    SHOW_SECRET(EAST_SECRET_VULCAN_AXE, 8, 10);
    SHOW_SECRET(EAST_SECRET_PIT, 23, 21);
    SHOW_SECRET_SOUTH_ROWS(EAST_SECRET_PIT, 23, 21);
    SHOW_SECRET(EAST_SECRET_OASIS_SOUTHWEST, 16, 42);
    SHOW_SECRET(EAST_SECRET_OASIS_SOUTHEAST, 36, 44);
    SHOW_SECRET(EAST_SECRET_FIRST_PSYNERGY_STONE, 14, 55);
    Actor_SetPosition(ACTOR_FIRST_PIT, 400, 364);
  } else if (gMainRoutineWork.next == SCENE_LAMAKAN_DESERT_SMOG) {
    SHOW_SECRET(SMOG_SECRET_OASIS_NORTHEAST, 42, 5);
    SHOW_SECRET(SMOG_SECRET_PIT_NORTH, 20, 11);
    SHOW_SECRET_SOUTH_ROWS(SMOG_SECRET_PIT_NORTH, 20, 11);
    SHOW_SECRET(SMOG_SECRET_LUCKY_PEPPER, 14, 12);
    SHOW_SECRET(SMOG_SECRET_SECOND_PSYNERGY_STONE, 56, 18);
    SHOW_SECRET(SMOG_SECRET_PIT_WEST, 7, 22);
    SHOW_SECRET_SOUTH_ROWS(SMOG_SECRET_PIT_WEST, 7, 22);
    SHOW_SECRET(SMOG_SECRET_PIT_EAST, 44, 23);
    SHOW_SECRET_SOUTH_ROWS(SMOG_SECRET_PIT_EAST, 44, 23);
    SHOW_SECRET(SMOG_SECRET_SMOG, 38, 24);
    SHOW_SECRET(SMOG_SECRET_OASIS_MIDDLE, 26, 28);
    SHOW_SECRET(SMOG_SECRET_777_COINS, 17, 35);
    SHOW_SECRET(SMOG_SECRET_OASIS_EAST, 50, 36);
    SHOW_SECRET(SMOG_SECRET_PIT_SOUTH, 34, 43);
    SHOW_SECRET_SOUTH_ROWS(SMOG_SECRET_PIT_SOUTH, 34, 43);
    SHOW_SECRET(SMOG_SECRET_OASIS_SOUTHWEST, 6, 46);
    SHOW_SECRET(SMOG_SECRET_OASIS_SOUTH, 27, 55);
    SHOW_SECRET(SMOG_SECRET_POTION, 43, 56);
    Actor_SetPosition(ACTOR_FIRST_PIT, 352, 204);
    Actor_SetPosition(ACTOR_FIRST_PIT + 1, 736, 396);
    Actor_SetPosition(ACTOR_FIRST_PIT + 2, 144, 380);
    Actor_SetPosition(ACTOR_LAST_PIT, 576, 716);
    Actor_SetPosition(ACTOR_SMOG, 648, 408);
  } else if (gMainRoutineWork.next == SCENE_LAMAKAN_DESERT_MANTICORE) {
    // The three secrets come back from their spare patches. The sandfalls
    // part and the slopes lead into the passages; in the west one the
    // Manticore stands (CELL_BEHAVIOR_LAMAKAN_DESERT_MANTICORE), and a wall
    // closes it until the Manticore is beaten.
    MapGrid_CopyCells(64, 124, 4, 4, 8, 14);
    MapGrid_CopyCells(68, 124, 4, 4, 6, 18);
    MapGrid_CopyCellAttributes(68, 124, 4, 1, 6, 20);
    MapGrid_CopyCells(72, 124, 4, 4, 10, 21);
    MapGrid_CopyCells(10, 121, 5, 7, 8, 32);
    MapGrid_CopyCells(5, 121, 5, 7, 43, 32);
    MapGrid_CopyCells(0, 120, 3, 1, 9, 5);
    MapGrid_CopyCells(3, 120, 3, 1, 44, 5);
    Actor_SetPosition(ACTOR_MANTICORE, 168, 92);
    Actor_SetPosition(ACTOR_FIRST_PIT, 128, 316);
    MapGrid_CopyCellAttributes(6, 0, 3, 3, 9, 6);
    if (GameFlag_Get(FLAG_LAMAKAN_MANTICORE_BEATEN) == 0) {
      MapGrid_CopyCells(0, 119, 3, 1, 9, 5);
    }
  }
  for (i = TREASURE_SPOT_VULCAN_AXE; i <= TREASURE_SPOT_MINT; i++) {
    TreasureSpot_SetPosition(i, -1, -1);
  }
  TreasureSpot_MoveLeaderAway();
  if (gMainRoutineWork.next != SCENE_LAMAKAN_DESERT_CAVE) {
    LineScroll_Stop();
  }
}

/// Talking to the Manticore before the north-west passage of Lamakan Desert's
/// third screen makes it rear up and starts the battle. Afterwards the party
/// stands where it fought, below the opened passage, and the Manticore is gone
/// for good (LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE).
static void LamakanDesert_FightManticore(void) {
  Actor_RunRepeatedMotion(ACTOR_MANTICORE, 2);
  Event_SetAfterBattleDestination(
      SCENE_LAMAKAN_DESERT_MANTICORE,
      LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 5);
}

/// As Lamakan Desert's heat gauge passes a quarter, a half and three quarters,
/// Garet, Ivan or Mia, picked at random, complains of the heat.
static void LamakanDesert_ComplainAboutHeat(s32 mark) {
  s32 tries;
  s32 flag;
  s32 member;

  for (tries = 0; tries <= 2; tries++) {
    member = Random_Below(3);
    flag = member + FLAG_GARET_COMPLAINED;
    if (GameFlag_Get(flag) == 0) {
      GameFlag_Set(flag);
      break;
    }
  }
  Event_Start();
  // The lines follow Alpine Crossing's signpost (`North: Altin Mine West: Lama
  // Temple`), three a mark, Garet's, Ivan's and Mia's; at the last mark "Aghh!
  // I can't stand it! My feet are burning!", "I can't stand another minute of
  // this! Where's an oasis?" and "If we don't find an oasis soon, I'm gonna
  // shrivel up and blow away!"
  Event_SetMessageId((mark - LAMAKAN_DESERT_RAISED_HEAT_QUARTER) * 3 + member +
                     MSGNO(MsgMountainNorthAltinMineWestLama) + 1);
  Event_ShowMessage(member + 1, 0);
  Event_End();
}

/// Every frame: raises LAMAKAN_DESERT_RAISED_HEAT_QUARTER to _THREE_QUARTERS as
/// the gauge passes each quarter, and clears each mark and the complaints when
/// the gauge drops back below it. Nothing is raised while Reveal is showing the
/// pits.
static void HeatGauge_RaiseQuarterTriggers(void) {
  struct FieldWork *work = gFieldWork;
  s32 percent =
      gMainRoutineWork.hazardSteps * 100 / gMainRoutineWork.hazardLimit;

  if (GameFlag_Get(FLAG_SECRETS_REVEALED) != 0)
    return;
  if (Event_GetFlag(FLAG_HEAT_THREE_QUARTERS) != 0 &&
      percent < HEAT_THREE_QUARTERS_PERCENT) {
    GameFlag_Clear(FLAG_HEAT_THREE_QUARTERS);
    GameFlag_Clear(FLAG_GARET_COMPLAINED);
    GameFlag_Clear(FLAG_IVAN_COMPLAINED);
    GameFlag_Clear(FLAG_MIA_COMPLAINED);
  }
  if (Event_GetFlag(FLAG_HEAT_HALF) != 0 && percent < HEAT_HALF_PERCENT) {
    GameFlag_Clear(FLAG_HEAT_HALF);
    GameFlag_Clear(FLAG_GARET_COMPLAINED);
    GameFlag_Clear(FLAG_IVAN_COMPLAINED);
    GameFlag_Clear(FLAG_MIA_COMPLAINED);
  }
  if (Event_GetFlag(FLAG_HEAT_QUARTER) != 0 && percent < HEAT_QUARTER_PERCENT) {
    GameFlag_Clear(FLAG_HEAT_QUARTER);
    GameFlag_Clear(FLAG_GARET_COMPLAINED);
    GameFlag_Clear(FLAG_IVAN_COMPLAINED);
    GameFlag_Clear(FLAG_MIA_COMPLAINED);
  }
  if (Event_GetFlag(FLAG_HEAT_QUARTER) == 0 &&
      percent >= HEAT_QUARTER_PERCENT) {
    GameFlag_Set(FLAG_HEAT_QUARTER);
    work->raisedTrigger = LAMAKAN_DESERT_RAISED_HEAT_QUARTER;
  }
  if (Event_GetFlag(FLAG_HEAT_HALF) == 0 && percent >= HEAT_HALF_PERCENT) {
    GameFlag_Set(FLAG_HEAT_HALF);
    work->raisedTrigger = LAMAKAN_DESERT_RAISED_HEAT_HALF;
  }
  if (Event_GetFlag(FLAG_HEAT_THREE_QUARTERS) == 0 &&
      percent >= HEAT_THREE_QUARTERS_PERCENT) {
    GameFlag_Set(FLAG_HEAT_THREE_QUARTERS);
    work->raisedTrigger = LAMAKAN_DESERT_RAISED_HEAT_THREE_QUARTERS;
  }
}

/// The distance in whole pixels from a position (x, height, z) to an oasis
/// (x, z), walking both a coordinate at a time.
static s32 LamakanDesert_PixelsToOasis(const s32 *position, const s32 *oasis) {
  s32 dx = FIXEDtoINT(*oasis++ - *position++);
  s32 dz = FIXEDtoINT(*oasis - position[1]);
  s32 squaredDeltaZ = dz * dz;
  s32 squaredDeltaX = dx * dx;

  return Iwram_Sqrt(squaredDeltaX + squaredDeltaZ);
}

/// Stepping into an oasis in Lamakan Desert, which only Reveal shows in its
/// ring of rocks, Isaac hops into the middle of the pool and bathes with a
/// heart over his head while the heat gauge drains, for at least a second, then
/// hops out.
static void LamakanDesert_BatheInOasis(void) {
  struct FieldWork *work;
  s32 frames;
  FIXED best;
  s32 count;
  const s32 *oases;
  s32 total;
  s32 offset;
  const s32 *oasis;
  s32 nearest;
  s32 distance;
  struct Actor *leader;
  struct EffectOptions splashIn;
  struct EffectOptions splashOut;
  s16 *heat;
  u16 *revealFrames;

  work = gFieldWork;
  frames = 60;
  best = toFIXED(240);
  GameFlag_Set(FLAG_DESERT_EVENT_RUNNING);
  HeatGauge_SetHeld(1);
  if (gMainRoutineWork.next == SCENE_LAMAKAN_DESERT_EAST) {
    count = 3;
    oases = gLamakanDesertEast_Oases;
  } else if (gMainRoutineWork.next == SCENE_LAMAKAN_DESERT_SMOG) {
    count = 5;
    oases = gLamakanDesertSmog_Oases;
  } else {
    count = 2;
    oases = gLamakanDesertOasesOther;
  }
  // Every oasis is nearer than 240 pixels, so the first always counts; a tie
  // goes to the later.
  total = count;
  oasis = oases;
  if (count != 0) {
    offset = 0;
    do {
      distance = LamakanDesert_PixelsToOasis(
          &Actor_Get(ACTOR_PARTY_LEADER)->x.fixed, oasis);
      if (distance <= best) {
        best = distance;
        nearest = total - count;
      }
      offset += 2 * sizeof(s32);
      oasis = (const s32 *)((const u8 *)oases + offset);
    } while (--count != 0);
    /* FAKEMATCH(dead): two self-tests of offset, each with updates that cancel
     * out. */
    if (offset) {
      offset++;
      offset--;
    }
    if (offset) {
      offset++;
      offset--;
    }
  }
  // The oasis's x, then its z.
  nearest <<= 1;

  // Hop the leader into the middle of the oasis before draining the heat gauge.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
  FieldActor_SetMoveTargetFX(Actor_Get(ACTOR_PARTY_LEADER), oases[nearest], 0,
                             oases[nearest + 1]);
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = toFIXED(6);
  Sound_Play(SE_JUMP);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_WaitBelowHeight(leader, Actor_Get(ACTOR_PARTY_LEADER)->y.fixed);
  Sound_Play(SE_WATER_MOVE);
  {
    struct Actor *bather = Actor_Get(ACTOR_PARTY_LEADER);

    splashIn.type = SPRITE_SPLASH;
    splashIn.startScaleX = FIXED_ONE / 2;
    splashIn.startScaleY = 4 * FIXED_ONE / 5;
    splashIn.targetScaleX = FIXED_ONE;
    splashIn.targetScaleY = 6 * FIXED_ONE / 5;
    FieldEffect_Spawn(
        bather->x.fixed, bather->y.fixed, bather->z.fixed, 0, 0, 0,
        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET,
        &splashIn);
  }
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_HEART, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_BATHE);

  // A running Reveal's countdown stays full while the leader bathes, at least
  // 60 frames and until the heat has drained.
  revealFrames = &work->revealFrames;
  heat = &gMainRoutineWork.hazardSteps;
  {
    s32 drained = 0;

    do {
      Io_Write16(REVEAL_FRAMES, revealFrames);
      frames--;
      if (*heat != 0) {
        *heat -= 5;
        if (*heat <= 0) {
          *heat = drained;
        } else if (frames == 0) {
          frames = 1;
        }
      }
      wait(1);
    } while (frames != 0);
  }

  // frames is 0 from here: the splash out does not move, and a running
  // Reveal ends as the leader gets up.
  {
    struct Actor *bather = Actor_Get(ACTOR_PARTY_LEADER);

    splashOut.type = SPRITE_SPLASH;
    splashOut.startScaleX = FIXED_ONE / 2;
    splashOut.startScaleY = 4 * FIXED_ONE / 5;
    splashOut.targetScaleX = FIXED_ONE / 2;
    splashOut.targetScaleY = 6 * FIXED_ONE / 5;
    FieldEffect_Spawn(
        bather->x.fixed, bather->y.fixed, bather->z.fixed, 0, frames, frames,
        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET,
        &splashOut);
  }
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_JUMP);
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = toFIXED(6);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(10);
  work->revealFrames = frames;
  HeatGauge_SetHeld(0);
}

/// Walking on Lamakan Desert's sand fills the heat gauge and kicks up little
/// puffs of dust at Isaac's feet, bigger and more often on the run. Every frame
/// he moves adds a step of heat, running or not; on the rivers of sand it adds
/// heat without the dust.
static void HeatGauge_CountStep(void) {
  struct Actor *actor;
  struct FieldWork *work;
  struct EffectOptions params;
  FIXED dx;
  FIXED z;
  s32 phase;
  actor = Actor_Get(gMainRoutineWork.leaderActorId);
  work = gFieldWork;
  if (actor->targetX == ACTOR_NO_TARGET) {
    return;
  }
  gMainRoutineWork.hazardSteps++;
  if (work->stepTrigger == CELL_BEHAVIOR_LAMAKAN_DESERT_SLOPE) {
    return;
  }
  if (actor->speedLimit <= toFIXED(1)) {
    if ((gFrameCount & 15) != 0) {
      return;
    }
    params.startScaleX = FIXED_ONE / 2;
    params.startScaleY = FIXED_ONE / 2;
    dx = 0;
    params.spin = FIELD_EFFECT_RANDOM_SPIN();
    if (actor->facing != FACING_EAST && actor->facing != FACING_WEST) {
      z = actor->z.fixed;
      dx = toFIXED(2) - toFIXED((FIELD_CELL(z) & 1) * 5);
    } else {
      z = actor->z.fixed;
    }
    FieldEffect_Spawn(actor->x.fixed + dx, actor->y.fixed, z, 0, 0, 0,
                      EFFECT_USE_SPIN | EFFECT_USE_START_SCALE |
                          EFFECT_SCRIPT_MEDIUM,
                      &params);
  } else {
    phase = gFrameCount & 7;
    if (phase != 0) {
      return;
    }
    // DEAD: a read of gFrameCount nothing uses; the ROM keeps its load.
    gFrameCount;
    params.startScaleX = 4 * FIXED_ONE / 5;
    params.startScaleY = 4 * FIXED_ONE / 5;
    params.spin = FIELD_EFFECT_RANDOM_SPIN();
    FieldEffect_Spawn(
        actor->x.fixed, actor->y.fixed + toFIXED(2), actor->z.fixed, 0,
        (Random_Below(5)) * (toFIXED(1) / 10), phase,
        EFFECT_USE_SPIN | EFFECT_USE_START_SCALE | EFFECT_SCRIPT_MEDIUM,
        &params);
  }
}

/// The distance in pixels LamakanDesert_FallIntoPit's search for the nearest
/// pit starts from, farther than any pit can be.
#define PIT_SEARCH_DISTANCE (1 << 20)

/// Stepping beside an Ant Lion's pit in Lamakan Desert, which only Reveal
/// shows in its ring of rocks, Isaac turns his back to it and is dragged
/// backwards into it in a spray of sand, and an Ant Lion attacks.
static void LamakanDesert_FallIntoPit(void) {
  struct Actor *leader;
  struct Actor *pit;
  s32 id;
  s32 best;
  s32 min;
  s32 dx;
  s32 dz;
  s32 distance;

  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  best = ACTOR_FIRST_PIT;
  GameFlag_Set(FLAG_DESERT_EVENT_RUNNING);
  min = PIT_SEARCH_DISTANCE;
  // The nearest pit still there, by the distance along x plus along z.
  for (id = ACTOR_FIRST_PIT; id <= ACTOR_LAST_PIT; id++) {
    pit = Actor_Get(id);
    if (pit != NULL) {
      dx = (leader->x.fixed - pit->x.fixed) / toFIXED(1);
      dz = (leader->z.fixed - pit->z.fixed) / toFIXED(1);
      distance = dx;
      if (distance < 0) {
        distance = -distance;
      }
      if (dz < 0) {
        dz = -dz;
      }
      if (distance + dz < min) {
        best = id;
        min = distance + dz;
      }
    }
  }
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_FaceActor(ACTOR_PARTY_LEADER, best, 0);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(60);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  // The leader turns his back on the pit.
  Actor_Get(ACTOR_PARTY_LEADER)->facing =
      FACING_SIXTEENTH(Actor_Get(ACTOR_PARTY_LEADER)->facing + DEGtoANG(180));
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_RUN);
  Actor_SetAnimationSpeedById(ACTOR_PARTY_LEADER,
                              SPRITE_ANIMATION_STEP_NORMAL * 3 / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.1);
  Actor_Get(ACTOR_PARTY_LEADER)->update =
      (void (*)(struct Actor *))LamakanDesert_KickUpSand;
  pit = Actor_Get(best);
  if (pit != NULL) {
    Actor_SetDestination(ACTOR_PARTY_LEADER, pit->x.part[0].integer,
                         pit->z.part[0].integer);
  }
  Event_Wait(60);
  Actor_ShowEmote(best, EMOTE_IN_FRONT | EMOTE_HEART, 0);
  Event_Wait(60);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_PARTY_LEADER)->update = NULL;
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 4);
}

/// On a river of sand in Lamakan Desert's third screen, the sand carries Isaac
/// south, about a cell every ten frames, in a spray of sand, until something
/// stops him. Where the river drops over a ledge, he gives a start and leaps
/// down.
static void LamakanDesert_SlipDownSand(void) {
  struct Actor *center;
  s32 id;
  struct Actor *actor;
  struct FieldPosition probePosition;
  struct EffectOptions params;
  s32 hit;
  s32 phase;
  FIXED x;

  HeatGauge_CountStep();
  {
    struct FieldWork *work = gFieldWork;

    id = gMainRoutineWork.leaderActorId;
    center = work->camera;
  }
  actor = Actor_Get(id);
  // First check for a ledge immediately below the sliding actor.
  probePosition.x = actor->x.fixed;
  probePosition.y = actor->y.fixed;
  probePosition.z = actor->z.fixed + toFIXED(3) / 2;
  hit = FieldActor_CheckMovementCollision(actor, &probePosition);
  phase = gFrameCount & 4;
  if (phase == 0) {
    params.spin = FIELD_EFFECT_RANDOM_SPIN();
    x = actor->x.fixed + toFIXED(Random_Below(12)) - toFIXED(6);
    FieldEffect_Spawn(x, actor->y.fixed, actor->z.fixed, 0, phase,
                      (Random_Below(5)) * (toFIXED(1) / 10) +
                          5 * (toFIXED(1) / 10),
                      EFFECT_USE_SPIN, &params);
  }
  if (hit < 0) {
    Actor_SetAttachedEffect(id,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldActor_SetMoveTargetFX(actor, actor->x.fixed, actor->y.fixed,
                               actor->z.fixed + toFIXED(8));
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);
    Actor_WaitForMove(actor);
    do {
      wait(1);
    } while (actor->y.fixed != actor->groundHeight);
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    wait(3);
    return;
  }
  // Probe ahead and on both diagonals before advancing actor and camera.
  probePosition.x = actor->x.fixed;
  probePosition.y = actor->y.fixed;
  probePosition.z = actor->z.fixed + toFIXED(8);
  hit = FieldActor_CheckMovementCollision(actor, &probePosition);
  if (hit > 0) {
    return;
  }
  probePosition.x = actor->x.fixed + toFIXED(57) / 10;
  probePosition.y = actor->y.fixed;
  probePosition.z = actor->z.fixed + toFIXED(57) / 10;
  hit = FieldActor_CheckMovementCollision(actor, &probePosition);
  if (hit > 0) {
    return;
  }
  probePosition.x = actor->x.fixed - toFIXED(57) / 10;
  probePosition.y = actor->y.fixed;
  probePosition.z = actor->z.fixed + toFIXED(57) / 10;
  hit = FieldActor_CheckMovementCollision(actor, &probePosition);
  if (hit > 0) {
    return;
  }
  center->z.fixed += toFIXED(3) / 2;
  actor->z.fixed += toFIXED(3) / 2;
}

/// Coming near the north-west passage of Lamakan Desert's third screen, once
/// Reveal has parted its sandfall and shown the Manticore standing in it,
/// Isaac stops short, startled, and backs away as the Manticore roars, lunges
/// at him and settles back. It happens each time until the Manticore is
/// beaten.
static void LamakanDesert_MeetManticore(void) {
  struct FieldWork *work = gFieldWork;
  struct FieldPosition backward;

  HeatGauge_CountStep();
  if (GameFlag_Get(FLAG_LAMAKAN_MANTICORE_BEATEN) != 0) {
    return;
  }
  if (Event_GetFlag(FLAG_DESERT_EVENT_RUNNING) != 0) {
    return;
  }
  GameFlag_Set(FLAG_DESERT_EVENT_RUNNING);
  // The heat stops rising while the leader faces the Manticore, and a running
  // Reveal is kept full until it has risen.
  HeatGauge_SetHeld(1);
  work->revealFrames = REVEAL_FRAMES;
  Actor_Get(ACTOR_PARTY_LEADER)->velocityX = 0;
  Actor_Get(ACTOR_PARTY_LEADER)->velocityZ = 0;
  Actor_Get(ACTOR_PARTY_LEADER)->targetX = ACTOR_NO_TARGET;
  Actor_Get(ACTOR_PARTY_LEADER)->targetZ = ACTOR_NO_TARGET;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_MANTICORE, 0);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);

  // The leader backs away 16 pixels, still facing the Manticore.
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  backward.x = 0;
  backward.y = 0;
  backward.z = 0;
  FieldVector_AddPolarOffset(-16, Actor_Get(ACTOR_PARTY_LEADER)->facing,
                             &backward);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, backward.x / toFIXED(1),
                             backward.z / toFIXED(1));
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Event_Wait(2);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(30);

  Sound_Play(SE_BOSS_ROAR);
  Actor_RunRepeatedMotion(ACTOR_MANTICORE, 2);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_MANTICORE, 2.5);
  Actor_WalkToAndWait(ACTOR_MANTICORE, 168, 104);
  Actor_SetSpeed(ACTOR_MANTICORE, 0.5);
  Actor_WalkToAndWait(ACTOR_MANTICORE, 168, 92);
  work->revealFrames = REVEAL_FRAMES;
  HeatGauge_SetHeld(0);
}

/// Entering Lamakan Desert sets the heat's limit, HEAT_LIMIT steps, hides the
/// desert's secrets, and shows the heat gauge everywhere but in the cave.
s32 Scene_Initialize(void) {
  gMainRoutineWork.hazardLimit = HEAT_LIMIT;
  gMainRoutineWork.hazardMode = HAZARD_COUNT_ON_CELLS;
  gMainRoutineWork.hazardDamage = HAZARD_DAMAGE_PERCENT | HEAT_DAMAGE_PERCENT;
  if (gMainRoutineWork.next != SCENE_LAMAKAN_DESERT_CAVE) {
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    HeatGauge_Start();
    Task_AddFrame(HeatGauge_RaiseQuarterTriggers, TASK_PASS_FRAME);
    if (gMainRoutineWork.next == SCENE_LAMAKAN_DESERT_EAST) {
      SAVE_SECRET(EAST_SECRET_OASIS_NORTH, 22, 7);
      SAVE_SECRET(EAST_SECRET_VULCAN_AXE, 8, 10);
      SAVE_SECRET(EAST_SECRET_PIT, 23, 21);
      SAVE_SECRET(EAST_SECRET_OASIS_SOUTHWEST, 16, 42);
      SAVE_SECRET(EAST_SECRET_OASIS_SOUTHEAST, 36, 44);
      SAVE_SECRET(EAST_SECRET_FIRST_PSYNERGY_STONE, 14, 55);
    } else if (gMainRoutineWork.next == SCENE_LAMAKAN_DESERT_SMOG) {
      SAVE_SECRET(SMOG_SECRET_OASIS_NORTHEAST, 42, 5);
      SAVE_SECRET(SMOG_SECRET_PIT_NORTH, 20, 11);
      SAVE_SECRET(SMOG_SECRET_LUCKY_PEPPER, 14, 12);
      SAVE_SECRET(SMOG_SECRET_SECOND_PSYNERGY_STONE, 56, 18);
      SAVE_SECRET(SMOG_SECRET_PIT_WEST, 7, 22);
      SAVE_SECRET(SMOG_SECRET_PIT_EAST, 44, 23);
      SAVE_SECRET(SMOG_SECRET_SMOG, 38, 24);
      SAVE_SECRET(SMOG_SECRET_OASIS_MIDDLE, 26, 28);
      SAVE_SECRET(SMOG_SECRET_777_COINS, 17, 35);
      SAVE_SECRET(SMOG_SECRET_OASIS_EAST, 50, 36);
      SAVE_SECRET(SMOG_SECRET_PIT_SOUTH, 34, 43);
      SAVE_SECRET(SMOG_SECRET_OASIS_SOUTHWEST, 6, 46);
      SAVE_SECRET(SMOG_SECRET_OASIS_SOUTH, 27, 55);
      SAVE_SECRET(SMOG_SECRET_POTION, 43, 56);
    } else if (gMainRoutineWork.next == SCENE_LAMAKAN_DESERT_MANTICORE) {
      Scene_QueueMusic(SE_AMBIENT_WIND);
      // The three secrets in the west, saved 4 by 4 to spare patches on row
      // 124.
      MapGrid_CopyCells(8, 14, 4, 4, 64, 124);
      MapGrid_CopyCells(6, 18, 4, 4, 68, 124);
      MapGrid_CopyCells(10, 21, 4, 4, 72, 124);
    }
    Reveal_HideSecrets();
  }
  return 0;
}

/// Unused: the Manticore walks to (168, 96), 4 pixels south of where Reveal
/// shows it (168, 92), at the slow speed of LamakanDesert_MeetManticore's walk
/// back, then plays its one-shot animation 2. Nothing calls it.
static UNUSED void LamakanDesert_UnusedWalkManticore(void) {
  Actor_SetSpeed(ACTOR_MANTICORE, 0.5);
  Actor_SetAnimation(ACTOR_MANTICORE, 1);
  Actor_WalkToAndWait(ACTOR_MANTICORE, 168, 96);
  Actor_SetAnimation(ACTOR_MANTICORE, 2);
}

/// Every frame: draws the heat gauge down the screen's left edge. It slides off
/// the screen, 8 pixels a frame, while the field handles a request, unless it
/// is held, and back on afterwards.
static void HeatGauge_Draw(void) {
  u8 *buffer;
  s32 tile;
  s16 *colors;
  s16 *color;
  struct ScreenTransitionState *state;
  u32 *pixels;
  u32 *word;
  u8 *frame;
  u8 *bar;
  struct MenuSprite *sprite;
  s32 shown;
  u32 i;
  u32 *palette;
  u32 red;
  u32 green;
  u32 blue;
  s32 level;
  u32 attributes;
  s32 y;
  u32 offset;

  tile = gVramCacheEntries[gHeatGauge_VramSlot].offset >> 5;
  if (gHeatGauge_Held != 0) {
    gHeatGauge_Shown = 2;
  } else if (GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS) != 0) {
    if (gHeatGauge_Shown > 0) {
      gHeatGauge_Shown--;
    }
  } else if (gHeatGauge_Shown <= 1) {
    if (++gHeatGauge_Shown == 1) {
      Dma_Set(gHeatGauge_Palette,
              (void *)&OBJ_PLTT_COLOR(HEAT_GAUGE_PALETTE, 0),
              DMA_COPY_HALFWORDS(16), REG_DMA3);
    }
  }
  shown = gHeatGauge_Shown;
  if (shown == 0) {
    VramCache_ReleaseVram(gHeatGauge_VramSlot);
    return;
  }

  // Window 1 spans the gauge, from the screen's left edge to the gauge's
  // right, on the lines it covers: each line's WIN1H, past WININ and WINOUT.
  state = gScreenTransitionWork;
  if (state != NULL) {
    u16 *edge = state->pages[state->page];
    s32 right = (s16)(shown << 3);

    edge += 3 + 2 * HEAT_GAUGE_TOP;
    for (i = 0; i < HEAT_GAUGE_HEIGHT; i++) {
      *edge = right;
      edge += 2;
    }
  }

  buffer = Heap_Alloc(sizeof(gHeatGauge_TileBuffer));
  Dma_Set(gHeatGauge_Palette, buffer, DMA_COPY_HALFWORDS(16), REG_DMA3);
  colors = (s16 *)buffer + HEAT_GAUGE_TINTED_COLOR;
  color = colors;
  for (i = HEAT_GAUGE_TINTED_COLOR;
       i < HEAT_GAUGE_TINTED_COLOR + HEAT_GAUGE_TINTED_COLORS; i++) {
    u16 value = *color;

    red = RGB_RED(value);
    green = RGB_GREEN(value);
    // DEAD: the 20 taken from blue here and added back below cancel; the ROM
    // keeps the subtraction and the addition.
    blue = RGB_BLUE(value) - 20;
    level = gHeatGauge_Level;
    red += level / 3;
    blue = blue - level / 6 + 20;
    if (level > HEAT_GAUGE_LEVELS / 2 && (gFrameCount & 1)) {
      green += level * (2 * HEAT_GAUGE_GREEN_FLICKER) / HEAT_GAUGE_LEVELS -
               HEAT_GAUGE_GREEN_FLICKER;
    }
    if (red > RGB_LEVEL_MAX) {
      red = RGB_LEVEL_MAX;
    }
    if (green > RGB_LEVEL_MAX) {
      green = RGB_LEVEL_MAX;
    }
    if (blue > RGB_LEVEL_MAX) {
      blue = RGB_LEVEL_MAX;
    }
    *color++ = RGB(red, green, blue);
  }
  palette = (u32 *)&OBJ_PLTT_COLOR(HEAT_GAUGE_PALETTE, HEAT_GAUGE_TINTED_COLOR);
  word = (u32 *)(buffer + HEAT_GAUGE_TINTED_COLOR * sizeof(u16));
  IoWriteQueue_Store32((u32)palette++, *word++);
  IoWriteQueue_Store32((u32)palette++, *word++);
  IoWriteQueue_Store32((u32)palette++, *word++);

  gHeatGauge_Level = gMainRoutineWork.hazardSteps * HEAT_GAUGE_LEVELS /
                     gMainRoutineWork.hazardLimit;
  if (gHeatGauge_Level >= HEAT_GAUGE_LEVELS - 1) {
    gHeatGauge_Flash = HEAT_GAUGE_LEVELS - 1;
  }
  if (gHeatGauge_Flash != 0) {
    gHeatGauge_Level = gHeatGauge_Flash;
    gHeatGauge_Flash -= 8;
    if (gHeatGauge_Flash <= 0) {
      gHeatGauge_Flash = 0;
    }
  }

  // The gauge's tiles, two 8-pixel columns of 4-bit pixels: paint the tube
  // black from its top down to the heat level, then close it with the top's
  // first row.
  Dma_Set(gHeatGauge_TileBuffer, buffer,
          DMA_COPY_WORDS(sizeof(gHeatGauge_TileBuffer) / 4), REG_DMA3);
  if (gHeatGauge_Flash < HEAT_GAUGE_LEVELS - 1) {
    pixels = (u32 *)buffer + HEAT_GAUGE_ROW_WORD(HEAT_GAUGE_TUBE_TOP);
    {
      u32 fill = PIXELS_4BPP(HEAT_GAUGE_EMPTY_COLOR);

      for (i = HEAT_GAUGE_TUBE_TOP; i < HEAT_GAUGE_TUBE_FOOT - gHeatGauge_Level;
           i++) {
        pixels[8] = fill;
        *pixels++ = fill;
        if ((i & 7) == 7) {
          pixels += 8;
        }
      }
    }
    pixels[0] = ((u32 *)buffer)[0];
    pixels[8] = ((u32 *)buffer)[8];
  }
  // Lay the frame's opaque pixels over the tube.
  bar = buffer;
  frame = buffer + HEAT_GAUGE_TILE_BYTES;
  for (i = 0; i < HEAT_GAUGE_TILE_BYTES; i++) {
    u8 pixel = *frame++;

    if (pixel != 0) {
      *bar = pixel;
    }
    bar++;
  }
  VramCache_Load(gHeatGauge_VramSlot, HEAT_GAUGE_TILE_BYTES, buffer);

  // Four 16x32 sprites down the screen's left edge and the 16x16 cap at the
  // gauge's foot.
  attributes = OBJ_SIZE_16X32;
  sprite = gHeatGauge_Sprites;
  for (i = 0, offset = 0, y = HEAT_GAUGE_TOP; i < 5; i++) {
    u32 x = (gHeatGauge_Shown * 8 - HEAT_GAUGE_WIDTH) & OBJ_X_MASK;

    if (i == 4) {
      attributes = OBJ_SIZE_16X16;
    }
    sprite->next = NULL;
    sprite->oam.word.attr01 = OBJ_X(x) | y | attributes;
    sprite->oam.word.attr23 =
        tile | OBJ_PRIORITY(1) | OBJ_PALETTE(HEAT_GAUGE_PALETTE);
    Oam_AddSpriteAtDepth(((u8 *)gHeatGauge_Sprites + offset), OAM_DEPTH_FRONT);
    sprite++;
    tile += 8;
    offset += sizeof(*sprite);
    y += 32;
  }
  Heap_Release(buffer);
}

/// Loads the gauge's tiles, claims VRAM for them and starts drawing it.
void HeatGauge_Start(void) {
  FieldDecompress_TaggedLz(gHeatGauge_Tiles, gHeatGauge_TileBuffer);
  gHeatGauge_VramSlot = VramCache_FindFreeEntry();
  VramCache_Load(gHeatGauge_VramSlot, HEAT_GAUGE_TILE_BYTES, 0);
  gHeatGauge_Shown = 0;
  gHeatGauge_Held = 0;
  Task_AddFrame(HeatGauge_Draw, TASK_PASS_FRAME - 10);
}

/// While held (1), the gauge stays on screen.
static void HeatGauge_SetHeld(u16 value) { gHeatGauge_Held = value; }

// Lamakan Desert keeps its duration scripts with its read-only data.
#define FIELD_EFFECT_DURATION_SCRIPTS_READ_ONLY
#include "shared/field_effect_duration_scripts.c"

// clang-format off
/// Each map's oases, the middle of each as x, z pairs.
const s32 gLamakanDesertEast_Oases[] = {
    toFIXED(384), toFIXED(128),
    toFIXED(288), toFIXED(688),
    toFIXED(608), toFIXED(720),
};

const s32 gLamakanDesertSmog_Oases[] = {
    toFIXED(704), toFIXED(96),
    toFIXED(448), toFIXED(464),
    toFIXED(832), toFIXED(592),
    toFIXED(128), toFIXED(752),
    toFIXED(464), toFIXED(896),
};

const s32 gLamakanDesertOasesOther[] = {
    toFIXED(192), toFIXED(224),
    toFIXED(192), toFIXED(352),
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct SceneEntrance gLamakanDesertEntrancesOther[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, 0, 0, 0, 0, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gLamakanDesertEastEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { LAMAKAN_DESERT_EAST_FROM_WORLD_MAP, CONDITION_ALWAYS, 984, 0, 952, FACING_WEST, 0, 0, 0, 1016, 1016, 0 },
    { LAMAKAN_DESERT_EAST_FROM_LAMAKAN_DESERT_SMOG, CONDITION_ALWAYS, 400, 0, 24, FACING_SOUTH, 0, 0, 0, 1016, 1016, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gLamakanDesertSmogEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { LAMAKAN_DESERT_SMOG_FROM_LAMAKAN_DESERT_EAST, CONDITION_ALWAYS, 912, 0, 1000, FACING_NORTH, 0, 0, 0, 1016, 1016, 0 },
    { LAMAKAN_DESERT_SMOG_FROM_LAMAKAN_DESERT_MANTICORE, CONDITION_ALWAYS, 24, 0, 584, FACING_EAST, 0, 0, 0, 1016, 1016, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gLamakanDesertManticoreEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_SMOG, CONDITION_ALWAYS, 1511, 0, 104, FACING_WEST, 0, 0, 8, 1528, 504, 0 },
    { LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_EAST, CONDITION_ALWAYS, 727, 0, 104, FACING_SOUTH, 0, 0, 8, 1528, 504, 0 },
    { LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_SOUTH, CONDITION_ALWAYS, 599, 0, 296, FACING_SOUTH, 0, 0, 8, 1528, 504, 0 },
    { LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_WEST, CONDITION_ALWAYS, 167, 0, 120, FACING_SOUTH, 0, 0, 8, 1528, 504, 0 },
    { LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE, CONDITION_ALWAYS, 167, 0, 136, FACING_NORTH, 0, 0, 8, 1528, 504, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gLamakanDesertCaveEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 424, 0, 824, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_SOUTH, CONDITION_ALWAYS, 424, 0, 968, FACING_NORTH, 0, 304, 560, 624, 1024, 0 },
    { LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_EAST, CONDITION_ALWAYS, 552, 0, 776, FACING_NORTH, 0, 304, 560, 624, 1024, 0 },
    { LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_WEST, CONDITION_ALWAYS, 440, 0, 312, FACING_NORTH, 0, 32, 16, 496, 320, 0 },
    { LAMAKAN_DESERT_CAVE_FROM_WORLD_MAP, CONDITION_ALWAYS, 136, 0, 296, FACING_NORTH, 0, 32, 16, 496, 320, 0 },
    { SCENE_TABLE_END },
};

u32 gLamakanDesertExits[] = {
    SCENE_EXITS(LamakanDesertEast),
    SCENE_EXIT(LAMAKAN_DESERT_EAST_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LAMAKAN_DESERT_EAST),
    SCENE_EXIT(LAMAKAN_DESERT_EAST_TO_LAMAKAN_DESERT_SMOG, LamakanDesertSmog, LAMAKAN_DESERT_SMOG_FROM_LAMAKAN_DESERT_EAST),
    SCENE_EXITS(LamakanDesertSmog),
    SCENE_EXIT(LAMAKAN_DESERT_SMOG_TO_LAMAKAN_DESERT_EAST, LamakanDesertEast, LAMAKAN_DESERT_EAST_FROM_LAMAKAN_DESERT_SMOG),
    SCENE_EXIT(LAMAKAN_DESERT_SMOG_TO_LAMAKAN_DESERT_MANTICORE, LamakanDesertManticore, LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_SMOG),
    SCENE_EXITS(LamakanDesertManticore),
    SCENE_EXIT(LAMAKAN_DESERT_MANTICORE_TO_LAMAKAN_DESERT_SMOG, LamakanDesertSmog, LAMAKAN_DESERT_SMOG_FROM_LAMAKAN_DESERT_MANTICORE),
    SCENE_EXIT(LAMAKAN_DESERT_MANTICORE_TO_LAMAKAN_DESERT_CAVE_EAST, LamakanDesertCave, LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_EAST),
    SCENE_EXIT(LAMAKAN_DESERT_MANTICORE_TO_LAMAKAN_DESERT_CAVE_SOUTH, LamakanDesertCave, LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_SOUTH),
    SCENE_EXIT(LAMAKAN_DESERT_MANTICORE_TO_LAMAKAN_DESERT_CAVE_WEST, LamakanDesertCave, LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_WEST),
    SCENE_EXITS(LamakanDesertCave),
    SCENE_EXIT(LAMAKAN_DESERT_CAVE_TO_LAMAKAN_DESERT_MANTICORE_EAST, LamakanDesertManticore, LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_EAST),
    SCENE_EXIT(LAMAKAN_DESERT_CAVE_TO_LAMAKAN_DESERT_MANTICORE_SOUTH, LamakanDesertManticore, LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_SOUTH),
    SCENE_EXIT(LAMAKAN_DESERT_CAVE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LAMAKAN_DESERT_CAVE),
    SCENE_EXIT(LAMAKAN_DESERT_CAVE_TO_LAMAKAN_DESERT_MANTICORE_WEST, LamakanDesertManticore, LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_WEST),
    SCENE_EXITS_END,
};

struct ActorPlacement gLamakanDesertActorPlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ActorPlacement gLamakanDesertEastActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MANTICORE, FLAG_LAMAKAN_MANTICORE_BEATEN, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ANT_LION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        400, 0, 364, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gLamakanDesertSmogActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MANTICORE, FLAG_LAMAKAN_MANTICORE_BEATEN, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ANT_LION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        352, 0, 204, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ANT_LION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        736, 0, 396, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ANT_LION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        144, 0, 380, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ANT_LION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        576, 0, 716, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_SMOG), MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gLamakanDesertManticoreActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MANTICORE, FLAG_LAMAKAN_MANTICORE_BEATEN, MOVEMENT_TYPE_STAND,
        168, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ANT_LION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        128, 0, 316, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

/// The four scenes' one trigger table. Its exit rows keep their numbers: each
/// takes the exit of that number in whichever scene the party is in, which is a
/// different way out in each (LAMAKAN_DESERT_EAST_TO_WORLD_MAP,
/// LAMAKAN_DESERT_SMOG_TO_LAMAKAN_DESERT_EAST and the others).
struct Trigger gLamakanDesertTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3 },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4 },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, 1 },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, 2 },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, 3 },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, 4 },
    { TRIGGER_STEP, CELL_BEHAVIOR_LAMAKAN_DESERT_TO_WORLD_MAP, CONDITION_ALWAYS, EVENT(LamakanDesert_LeaveForTheWorldMap) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LAMAKAN_DESERT_OASIS, FLAG_DESERT_EVENT_RUNNING, EVENT(LamakanDesert_BatheInOasis) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LAMAKAN_DESERT_SAND, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LAMAKAN_DESERT_PIT, FLAG_DESERT_EVENT_RUNNING, EVENT(LamakanDesert_FallIntoPit) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LAMAKAN_DESERT_SLOPE, CONDITION_ALWAYS, EVENT(LamakanDesert_SlipDownSand) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LAMAKAN_DESERT_MANTICORE, CONDITION_ALWAYS, EVENT(LamakanDesert_MeetManticore) },
    { TRIGGER_TALK, ACTOR_MANTICORE, FLAG_LAMAKAN_MANTICORE_BEATEN, EVENT(LamakanDesert_FightManticore) },
    { TRIGGER_TALK, ACTOR_SMOG, CONDITION_ALWAYS, EVENT(LamakanDesert_TalkToSmog) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_VULCAN_AXE, FLAG_LAMAKAN_DESERT_VULCAN_AXE, FIND_ITEM(ITEM_VULCAN_AXE) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), TREASURE_SPOT_FIRST_PSYNERGY_STONE, FLAG_LAMAKAN_DESERT_FIRST_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_LUCKY_PEPPER, FLAG_LAMAKAN_DESERT_LUCKY_PEPPER, FIND_ITEM(ITEM_LUCKY_PEPPER) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), TREASURE_SPOT_SECOND_PSYNERGY_STONE, FLAG_LAMAKAN_DESERT_SECOND_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_777_COINS, FLAG_LAMAKAN_DESERT_777_COINS, FIND_COINS(777) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_POTION, FLAG_LAMAKAN_DESERT_POTION, FIND_ITEM(ITEM_POTION) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_MINT, FLAG_LAMAKAN_DESERT_MINT, FIND_ITEM(ITEM_MINT) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Reveal_ShowSecrets) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Reveal_HideSecrets) },
    { TRIGGER_RAISED, LAMAKAN_DESERT_RAISED_HEAT_QUARTER, FLAG_SECRETS_REVEALED, EVENT(LamakanDesert_ComplainAboutHeat) },
    { TRIGGER_RAISED, LAMAKAN_DESERT_RAISED_HEAT_HALF, FLAG_SECRETS_REVEALED, EVENT(LamakanDesert_ComplainAboutHeat) },
    { TRIGGER_RAISED, LAMAKAN_DESERT_RAISED_HEAT_THREE_QUARTERS, FLAG_SECRETS_REVEALED, EVENT(LamakanDesert_ComplainAboutHeat) },
    { TRIGGER_STEP, TREASURE_SPOT_VULCAN_AXE, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_STEP, TREASURE_SPOT_FIRST_PSYNERGY_STONE, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_STEP, TREASURE_SPOT_LUCKY_PEPPER, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_STEP, TREASURE_SPOT_SECOND_PSYNERGY_STONE, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_STEP, TREASURE_SPOT_UNDER_SMOG, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_STEP, TREASURE_SPOT_777_COINS, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_STEP, TREASURE_SPOT_POTION, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_STEP, TREASURE_SPOT_MINT, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_TABLE_END },
};
// clang-format on

/// The gauge: how full it is (0 to HEAT_GAUGE_LEVELS), how much of it is on
/// the screen (0 to 2, 8 pixels a step), and the level it drains from once the
/// heat strikes.
s16 gHeatGauge_Level = 0;

s16 gHeatGauge_Shown = 0;

s16 gHeatGauge_Flash = 0;
