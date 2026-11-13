/// @file
/// Lamakan Desert (SceneId_LamakanDesertEast to SceneId_LamakanDesertCave):
/// four scenes that are all Lamakan Desert in the game's area names and share
/// one trigger table, built around a heat gauge and the secrets Reveal
/// uncovers.
///
/// The heat is a count of steps on hazard ground
/// (gMainRoutineWork.hazardSteps). LamakanDesert_Initialize sets a limit of 600
/// on every scene, at which the party loses a quarter of its HP. On all but
/// LamakanDesertCave it starts the heat gauge, a thermometer down the screen's
/// left edge (HeatGauge_Draw).
///
/// Each moving frame on the sand adds a step (HeatGauge_CountStep,
/// CELL_BEHAVIOR_LAMAKAN_DESERT_SAND and the examine triggers' cell behaviors
/// 100 to 107).
///
/// As the gauge passes a quarter, a half and three quarters, Garet, Ivan or Mia
/// complains of the heat (LamakanDesert_ComplainAboutHeat). For that,
/// HeatGauge_RaiseQuarterTriggers sets FLAG_HEAT_QUARTER to
/// FLAG_HEAT_THREE_QUARTERS and raises LAMAKAN_DESERT_RAISED_HEAT_QUARTER to
/// LAMAKAN_DESERT_RAISED_HEAT_THREE_QUARTERS.
///
/// Stepping into an oasis (CELL_BEHAVIOR_LAMAKAN_DESERT_OASIS), the leader hops
/// into the middle of the nearest one and bathes until the heat has drained
/// (LamakanDesert_BatheInOasis), and a running Reveal ends.
///
/// Entry saves the patches of the map grid that hold the desert's secrets and
/// hides them under a shimmering heat haze (Reveal_HideSecrets). The secrets
/// are the oases, the Ant Lions' pits, the Manticore, Smog, a Jupiter Djinni,
/// and the chests and Psynergy Stones.
///
/// Reveal brings them back and stops the haze (Reveal_ShowSecrets) until it
/// wears off. The oases' and pits' cells exist only while Reveal shines, as the
/// monks of Lama Temple warn: `Reveal will help you find oases to cool you
/// against the heat.`
///
/// Stepping by a pit (CELL_BEHAVIOR_LAMAKAN_DESERT_PIT) pulls the leader into
/// it for a battle with an Ant Lion (LamakanDesert_FallIntoPit). Slopes of sand
/// (CELL_BEHAVIOR_LAMAKAN_DESERT_SLOPE) carry the leader south
/// (LamakanDesert_SlideDownDune).
///
/// In LamakanDesertManticore, until it is beaten
/// (FLAG_LAMAKAN_MANTICORE_BEATEN), the Manticore rises out of the sand as the
/// leader comes near (step trigger 40, LamakanDesert_MeetManticore). Talking to
/// it starts its battle, which ends at LamakanDesertManticore's
/// LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE, and arriving there marks it
/// beaten.
///
/// LamakanDesertEast's LAMAKAN_DESERT_EAST_TO_WORLD_MAP and LamakanDesertCave's
/// LAMAKAN_DESERT_CAVE_TO_WORLD_MAP lead out onto the World Map.
/// LamakanDesertCave, a cave, has no heat gauge.
///
/// Step trigger 9 takes LAMAKAN_DESERT_CAVE_TO_WORLD_MAP. Leaving that way
/// while Hsu is still trapped (FLAG_HSU_RESCUED clear) sets FLAG_HSU_RESCUED
/// and FLAG_HSU_RESCUED_WITHOUT_PARTY, which lacquer_rooms_b.c reads to have
/// Feizhi thank Master Hama alone.

#include "vram_cache.h"
#include "module/import.h"
#include "module/scene/lamakan_desert.h"
#include "module/scene/shared/field_effect_spawn.h"
#include "module/field/event_commands.h"
#include "module/system/system.h"
#include "actor.h"
#include "event.h"
#include "fixed_point_position.h"
#include "scene.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "io_write.h"
#include "game.h"
#include "graphics.h"
#include "display_transition.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "text/msg_ids.h"
#include "iwram_call.h"
#include "event_inline.h"
#include "field_effect.h"
#include "dma.h"
#include "io_write_queue.h"
#include "ram_buffer.h"
#include "ability_ids.h"
#include "flags.h"
#include "djinn_ids.h"
#include "item_ids.h"

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
  /// An oasis's water, in the patches Reveal shows (renders of the maps with
  /// the patches shown: the pool at (384, 128) in LamakanDesertEast).
  CELL_BEHAVIOR_LAMAKAN_DESERT_OASIS = 20,
  CELL_BEHAVIOR_LAMAKAN_DESERT_SAND =
      21, ///< sand, where each moving frame adds heat
  CELL_BEHAVIOR_LAMAKAN_DESERT_PIT =
      22, ///< by an Ant Lion's pit, under Reveal too
  CELL_BEHAVIOR_LAMAKAN_DESERT_SLOPE =
      30 ///< sand that carries the leader south
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
// the west; hidden, it is drawn over with plain sand; Reveal copies it back.
#define SECRET_SPARE_COLUMN(number) (64 + 4 * (number))
#define SECRET_SPARE_ROW 126
/// Saves the secret whose top-left cell is (column, row) to its spare patch.
#define SAVE_SECRET(number, column, row)                                       \
  MapGrid_CopyCells(column, row, 4, 2, SECRET_SPARE_COLUMN(number),            \
                    SECRET_SPARE_ROW)
/// Draws plain sand over a secret.
#define HIDE_SECRET(column, row) MapGrid_CopyCells(70, 68, 4, 2, column, row)
/// Copies a secret back from its spare patch.
#define SHOW_SECRET(number, column, row)                                       \
  MapGrid_CopyCells(SECRET_SPARE_COLUMN(number), SECRET_SPARE_ROW, 4, 2,       \
                    column, row)
// Five secrets also change the ground a row further south: hidden, the row
// two below the patch's top takes the sand's ground; shown, the two rows from
// one below its top take the ground of the saved patch's two rows.
#define HIDE_SECRET_SOUTH_ROW(column, row)                                     \
  MapGrid_CopyCellAttributes(70, 68, 4, 1, column, (row) + 2)
#define SHOW_SECRET_SOUTH_ROWS(number, column, row)                            \
  MapGrid_CopyCellAttributes(SECRET_SPARE_COLUMN(number), SECRET_SPARE_ROW, 4, \
                             2, column, (row) + 1)

#include "shared/field_effect_create.h"
#include "shared/field_effect_spawn.h"

/// In LamakanDesertSmog, talking to Smog, whom Reveal uncovers, adds the
/// Jupiter Djinni to the party.
static void LamakanDesert_TalkToSmog(void) {
  Djinn_JoinParty(ACTOR_SMOG, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_SMOG));
}

#include "shared/actor_wait_below_height.h"

/// The update a leader pulled into an Ant Lion's pit runs: sand sounds every 8
/// frames and a spray of sand every 16.
static UNREAD_RESULT LamakanDesert_LeaderSpraySand(struct Actor *actor) {
  struct EffectOptions params;
  s32 phase;

  if ((gFrameCount & 7) == 0) {
    Sound_Play(SE_FOOTSTEP);
  }
  phase = gFrameCount & 15;
  if (phase != 0) {
    RETURN_UNREAD(0);
  }
  params.startScaleX = 4 * Q16_ONE / 5;
  params.startScaleY = 4 * Q16_ONE / 5;
  params.spin = (Random_Below(4096)) + 0xf800;
  FieldEffect_Spawn(
      actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, phase, phase,
      EFFECT_USE_SPIN | EFFECT_USE_START_SCALE | EFFECT_SCRIPT_MEDIUM, &params);
  RETURN_UNREAD(0);
}

/// Where the party can enter each of the four scenes.
const struct SceneEntrance *LamakanDesert_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == (s32)&SceneId_LamakanDesertEast) {
    return gLamakanDesertEastEntrances;
  }
  if (sceneId == (s32)&SceneId_LamakanDesertSmog) {
    return gLamakanDesertSmogEntrances;
  }
  if (sceneId == (s32)&SceneId_LamakanDesertManticore) {
    return gLamakanDesertManticoreEntrances;
  }
  if (sceneId == (s32)&SceneId_LamakanDesertCave) {
    return gLamakanDesertCaveEntrances;
  }
  return gLamakanDesertEntrancesOther;
}

const struct SceneExitRegion *LamakanDesert_GetRegions(void) { return 0; }

/// All four desert scenes share the packed exit table.
const u32 *LamakanDesert_GetExits(void) { return gLamakanDesertExits; }

/// The pits, the Manticore and Smog of each scene.
const struct ActorPlacement *LamakanDesert_GetActorPlacements(void) {
  if (gMainRoutineWork.next == (s32)&SceneId_LamakanDesertManticore) {
    if (gMainRoutineWork.entrance ==
        LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE) {
      GameFlag_Set(FLAG_LAMAKAN_MANTICORE_BEATEN);
      // XXX: what this patch fixed; every Kalay scene sets the flag on entry
      // (kalay.c) and Vale, the Vault and the Sanctum read it, so a save in
      // an unpatched edition that reaches this battle with it clear would
      // settle it.
#if TBS_PATCH_3
      GameFlag_Set(FLAG_KALAY_REACHED);
#endif
    }
  }
  if (gMainRoutineWork.next == (s32)&SceneId_LamakanDesertEast) {
    return gLamakanDesertEastActorPlacements;
  }
  if (gMainRoutineWork.next == (s32)&SceneId_LamakanDesertSmog) {
    return gLamakanDesertSmogActorPlacements;
  }
  if (gMainRoutineWork.next == (s32)&SceneId_LamakanDesertManticore) {
    return gLamakanDesertManticoreActorPlacements;
  }
  return gLamakanDesertActorPlacementsOther;
}

/// In Lamakan Desert, step trigger 9 takes LAMAKAN_DESERT_CAVE_TO_WORLD_MAP.
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
const struct Trigger *LamakanDesert_GetTriggers(void) {
  return gLamakanDesertTriggers;
}

/// Reveal wears off, and the desert's secrets sink under the sand again.
static UNREAD_RESULT Reveal_HideSecrets(void) {
  s32 markerId;

  GameFlag_Clear(FLAG_SECRETS_REVEALED);
  if (gMainRoutineWork.next == (s32)&SceneId_LamakanDesertEast) {
    HIDE_SECRET(22, 7);
    HIDE_SECRET(8, 10);
    HIDE_SECRET(23, 21);
    HIDE_SECRET_SOUTH_ROW(23, 21);
    HIDE_SECRET(16, 42);
    HIDE_SECRET(36, 44);
    HIDE_SECRET(14, 55);
  } else if (gMainRoutineWork.next == (s32)&SceneId_LamakanDesertSmog) {
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
  } else if (gMainRoutineWork.next == (s32)&SceneId_LamakanDesertManticore) {
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
  markerId = 100;
  do {
    TreasureSpot_SetActorPosition(markerId, 0, 0);
    markerId = markerId + 1;
  } while (markerId <= 107);
  if (gMainRoutineWork.next != (s32)&SceneId_LamakanDesertCave) {
    LineScroll_StartWave(0, Q16_ONE * 4, Q16_ONE, Q16_ONE / 8, Q16_ONE,
                         Q16_ONE / 2, Q16_ONE / 4);
  }
}

/// Casting Reveal uncovers what the desert's sand hides.
static UNREAD_RESULT Reveal_ShowSecrets(void) {
  s32 i;

  GameFlag_Clear(FLAG_DESERT_EVENT_RUNNING);
  GameFlag_Set(FLAG_SECRETS_REVEALED);
  if (gMainRoutineWork.next == (s32)&SceneId_LamakanDesertEast) {
    SHOW_SECRET(0, 22, 7);
    SHOW_SECRET(1, 8, 10);
    SHOW_SECRET(2, 23, 21);
    SHOW_SECRET_SOUTH_ROWS(2, 23, 21);
    SHOW_SECRET(3, 16, 42);
    SHOW_SECRET(4, 36, 44);
    SHOW_SECRET(5, 14, 55);
    Actor_SetPosition(ACTOR_FIRST_PIT, 400, 364);
  } else if (gMainRoutineWork.next == (s32)&SceneId_LamakanDesertSmog) {
    SHOW_SECRET(0, 42, 5);
    SHOW_SECRET(1, 20, 11);
    SHOW_SECRET_SOUTH_ROWS(1, 20, 11);
    SHOW_SECRET(2, 14, 12);
    SHOW_SECRET(3, 56, 18);
    SHOW_SECRET(4, 7, 22);
    SHOW_SECRET_SOUTH_ROWS(4, 7, 22);
    SHOW_SECRET(5, 44, 23);
    SHOW_SECRET_SOUTH_ROWS(5, 44, 23);
    SHOW_SECRET(6, 38, 24);
    SHOW_SECRET(7, 26, 28);
    SHOW_SECRET(8, 17, 35);
    SHOW_SECRET(9, 50, 36);
    SHOW_SECRET(10, 34, 43);
    SHOW_SECRET_SOUTH_ROWS(10, 34, 43);
    SHOW_SECRET(11, 6, 46);
    SHOW_SECRET(12, 27, 55);
    SHOW_SECRET(13, 43, 56);
    Actor_SetPosition(ACTOR_FIRST_PIT, 352, 204);
    Actor_SetPosition(ACTOR_FIRST_PIT + 1, 736, 396);
    Actor_SetPosition(ACTOR_FIRST_PIT + 2, 144, 380);
    Actor_SetPosition(ACTOR_LAST_PIT, 576, 716);
    Actor_SetPosition(ACTOR_SMOG, 648, 408);
  } else if (gMainRoutineWork.next == (s32)&SceneId_LamakanDesertManticore) {
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
  for (i = 100; i <= 107; i++) {
    TreasureSpot_SetPosition(i, -1, -1);
  }
  TreasureSpot_MoveLeaderAway();
  if (gMainRoutineWork.next != (s32)&SceneId_LamakanDesertCave) {
    LineScroll_Stop();
  }
}

/// The party fights the Manticore. In LamakanDesertManticore, talking to the
/// Manticore starts its battle, which returns the party to
/// LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE.
static void LamakanDesert_FightManticore(void) {
  Actor_RunRepeatedMotion(ACTOR_MANTICORE, 2);
  Event_SetAfterBattleDestination(
      (s32)&SceneId_LamakanDesertManticore,
      LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 5);
}

/// Garet, Ivan or Mia complains about the heat.
static void LamakanDesert_ComplainAboutHeat(s32 mark) {
  s32 tries;
  s32 quarter;
  s32 random;
  s32 flag;
  s32 member;

  quarter = mark;
  for (tries = 0; tries <= 2; tries++) {
    random = Random_Next();
    member = (u32)((random << 1) + random) >> 16; // 0 to 2
    flag = member + FLAG_GARET_COMPLAINED;
    if (GameFlag_Get(flag) == 0) {
      GameFlag_Set(flag);
      break;
    }
  }
  Event_Start();
  // Three lines a mark, one for each party member. They come right after Alpine
  // Crossing's signpost (`North: Altin Mine West: Lama Temple`) and have no
  // names of their own, so they count from a Fuchin Temple message two before
  // it.
  Event_SetMessageId((((quarter << 1) + quarter) + member) +
                     MSGNO(MsgFuchinTempleIDoNotThinkMasterHama));
  Event_ShowMessage((member + 1), 0);
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
  if (Event_GetFlag(FLAG_HEAT_THREE_QUARTERS) != 0 && percent <= 74) {
    GameFlag_Clear(FLAG_HEAT_THREE_QUARTERS);
    GameFlag_Clear(FLAG_GARET_COMPLAINED);
    GameFlag_Clear(FLAG_IVAN_COMPLAINED);
    GameFlag_Clear(FLAG_MIA_COMPLAINED);
  }
  if (Event_GetFlag(FLAG_HEAT_HALF) != 0 && percent <= 49) {
    GameFlag_Clear(FLAG_HEAT_HALF);
    GameFlag_Clear(FLAG_GARET_COMPLAINED);
    GameFlag_Clear(FLAG_IVAN_COMPLAINED);
    GameFlag_Clear(FLAG_MIA_COMPLAINED);
  }
  if (Event_GetFlag(FLAG_HEAT_QUARTER) != 0 && percent <= 24) {
    GameFlag_Clear(FLAG_HEAT_QUARTER);
    GameFlag_Clear(FLAG_GARET_COMPLAINED);
    GameFlag_Clear(FLAG_IVAN_COMPLAINED);
    GameFlag_Clear(FLAG_MIA_COMPLAINED);
  }
  if (Event_GetFlag(FLAG_HEAT_QUARTER) == 0 && percent > 24) {
    GameFlag_Set(FLAG_HEAT_QUARTER);
    work->raisedTrigger = LAMAKAN_DESERT_RAISED_HEAT_QUARTER;
  }
  if (Event_GetFlag(FLAG_HEAT_HALF) == 0 && percent > 49) {
    GameFlag_Set(FLAG_HEAT_HALF);
    work->raisedTrigger = LAMAKAN_DESERT_RAISED_HEAT_HALF;
  }
  if (Event_GetFlag(FLAG_HEAT_THREE_QUARTERS) == 0 && percent > 74) {
    GameFlag_Set(FLAG_HEAT_THREE_QUARTERS);
    work->raisedTrigger = LAMAKAN_DESERT_RAISED_HEAT_THREE_QUARTERS;
  }
}

/// The distance in whole pixels from a position (x, height, z) to an oasis
/// (x, z), walking both a coordinate at a time.
static s32 LamakanDesert_PixelsToOasis(const s32 *position, const s32 *oasis) {
  s32 dx = (*oasis++ - *position++) >> 16;
  s32 dz = (*oasis - position[1]) >> 16;
  s32 squaredDeltaZ = dz * dz;
  s32 squaredDeltaX = dx * dx;

  return Iwram_Sqrt(squaredDeltaX + squaredDeltaZ);
}

/// In Lamakan Desert, stepping into an oasis
/// (CELL_BEHAVIOR_LAMAKAN_DESERT_OASIS), which only Reveal shows, the leader
/// hops into the middle of the nearest of the map's oases. He bathes with a
/// heart over his head while the gauge drains 5 steps a frame, for at least a
/// second, then hops out.
static void LamakanDesert_BatheInOasis(void) {
  struct FieldWork *work;
  s32 frames;
  s32 best;
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
  best = px(240);
  GameFlag_Set(FLAG_DESERT_EVENT_RUNNING);
  HeatGauge_SetHeld(1);
  if (gMainRoutineWork.next == (s32)&SceneId_LamakanDesertEast) {
    count = 3;
    oases = gLamakanDesertEast_Oases;
  } else if (gMainRoutineWork.next == (s32)&SceneId_LamakanDesertSmog) {
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
     * out, rank it above count in allocation; without them, 22 of 228
     * instructions differ in tbs-en. */
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
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  FieldActor_SetMoveTargetFixed(leader, oases[nearest], 0, oases[nearest + 1]);
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = px(6);
  Sound_Play(SE_JUMP);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_WaitBelowHeight(leader, Actor_Get(ACTOR_PARTY_LEADER)->y.fixed);
  Sound_Play(SE_WATER_MOVE);
  {
    struct Actor *bather = Actor_Get(ACTOR_PARTY_LEADER);

    splashIn.type = SPRITE_SPLASH;
    splashIn.startScaleX = Q16_ONE / 2;
    splashIn.startScaleY = 4 * Q16_ONE / 5;
    splashIn.targetScaleX = Q16_ONE;
    splashIn.targetScaleY = 6 * Q16_ONE / 5;
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
      Io_Write16(600, revealFrames);
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
    splashOut.startScaleX = Q16_ONE / 2;
    splashOut.startScaleY = 4 * Q16_ONE / 5;
    splashOut.targetScaleX = Q16_ONE / 2;
    splashOut.targetScaleY = 6 * Q16_ONE / 5;
    FieldEffect_Spawn(
        bather->x.fixed, bather->y.fixed, bather->z.fixed, 0, frames, frames,
        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET,
        &splashOut);
  }
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_JUMP);
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = px(6);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(10);
  work->revealFrames = frames;
  HeatGauge_SetHeld(0);
}

/// In Lamakan Desert, walking on sand (step triggers 21 and 100 to 107) adds a
/// step of heat each moving frame and kicks up dust, faster on the run.
static void HeatGauge_CountStep(void) {
  struct Actor *actor;
  struct FieldWork *work;
  struct EffectOptions params;
  s32 dx;
  s32 z;
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
  if (actor->speedLimit <= px(1)) {
    if ((gFrameCount & 15) != 0) {
      return;
    }
    params.startScaleX = Q16_ONE / 2;
    params.startScaleY = Q16_ONE / 2;
    dx = 0;
    params.spin = (Random_Below(4096)) + 0xf800;
    if (actor->facing != FACING_EAST && actor->facing != FACING_WEST) {
      z = actor->z.fixed;
      dx = px(2) - px(((z >> 20) & 1) * 5);
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
    // Reads the frame counter again and drops the value.
    gFrameCount;
    params.startScaleX = 4 * Q16_ONE / 5;
    params.startScaleY = 4 * Q16_ONE / 5;
    params.spin = (Random_Below(4096)) + 0xf800;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed + px(2), actor->z.fixed, 0,
                      (Random_Below(5)) * (px(1) / 10), phase,
                      EFFECT_USE_SPIN | EFFECT_USE_START_SCALE |
                          EFFECT_SCRIPT_MEDIUM,
                      &params);
  }
}

/// In Lamakan Desert, stepping near a hidden pit (step trigger 22), the leader
/// looks at the nearest pit and is pulled backwards into it, and a battle with
/// an Ant Lion begins.
static void LamakanDesert_FallIntoPit(void) {
  struct Actor *leader;
  struct Actor *turned;
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
  min = 0x100000;
  // The nearest pit still there, by the distance along x plus along z.
  for (id = ACTOR_FIRST_PIT; id <= ACTOR_LAST_PIT; id++) {
    pit = Actor_Get(id);
    if (pit != NULL) {
      dx = (leader->x.fixed - pit->x.fixed) / px(1);
      dz = (leader->z.fixed - pit->z.fixed) / px(1);
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
  turned = Actor_Get(ACTOR_PARTY_LEADER);
  turned->facing =
      (Actor_Get(ACTOR_PARTY_LEADER)->facing + TRIG_TURN / 2) & -0x1000;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_RUN);
  Actor_SetAnimationSpeedById(ACTOR_PARTY_LEADER, 24);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.1);
  Actor_Get(ACTOR_PARTY_LEADER)->update =
      (void (*)(struct Actor *))LamakanDesert_LeaderSpraySand;
  pit = Actor_Get(best);
  if (pit != NULL) {
    Actor_SetDestination(ACTOR_PARTY_LEADER, pit->x.part.pixel,
                         pit->z.part.pixel);
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

/// In Lamakan Desert, on a slope of sand (CELL_BEHAVIOR_LAMAKAN_DESERT_SLOPE),
/// the sand carries the leader south 1.5 pixels a frame.
static void LamakanDesert_SlideDownDune(void) {
  struct Actor *center;
  s32 id;
  struct Actor *actor;
  struct FieldPosition probePosition;
  struct EffectOptions params;
  s32 hit;
  s32 phase;
  s32 x;

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
  probePosition.z = actor->z.fixed + px(3) / 2;
  hit = FieldActor_CheckMovementCollision(actor, &probePosition);
  phase = gFrameCount & 4;
  if (phase == 0) {
    params.spin = (Random_Below(4096)) + 0xf800;
    x = actor->x.fixed + ((Random_Below(12)) << 16) - px(6);
    FieldEffect_Spawn(x, actor->y.fixed, actor->z.fixed, 0, phase,
                      (Random_Below(5)) * (px(1) / 10) + 5 * (px(1) / 10),
                      EFFECT_USE_SPIN, &params);
  }
  if (hit < 0) {
    Actor_SetAttachedEffect(id,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldActor_SetMoveTargetFixed(actor, actor->x.fixed, actor->y.fixed,
                                  actor->z.fixed + px(8));
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
  probePosition.z = actor->z.fixed + px(8);
  hit = FieldActor_CheckMovementCollision(actor, &probePosition);
  if (hit > 0) {
    return;
  }
  probePosition.x = actor->x.fixed + px(57) / 10;
  probePosition.y = actor->y.fixed;
  probePosition.z = actor->z.fixed + px(57) / 10;
  hit = FieldActor_CheckMovementCollision(actor, &probePosition);
  if (hit > 0) {
    return;
  }
  probePosition.x = actor->x.fixed - px(57) / 10;
  probePosition.y = actor->y.fixed;
  probePosition.z = actor->z.fixed + px(57) / 10;
  hit = FieldActor_CheckMovementCollision(actor, &probePosition);
  if (hit > 0) {
    return;
  }
  center->z.fixed += px(3) / 2;
  actor->z.fixed += px(3) / 2;
}

/// In Lamakan Desert, at step trigger 40 in LamakanDesertManticore, the leader
/// stops and backs away as the Manticore rises out of the sand.
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
  work->revealFrames = 600;
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
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, backward.x / px(1),
                             backward.z / px(1));
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
  work->revealFrames = 600;
  HeatGauge_SetHeld(0);
}

/// Entering Lamakan Desert starts the heat gauge (600 steps) on all but
/// LamakanDesertCave.
s32 LamakanDesert_Initialize(void) {
  gMainRoutineWork.hazardLimit = 600;
  gMainRoutineWork.hazardMode = HAZARD_COUNT_ON_CELLS;
  gMainRoutineWork.hazardDamage = HAZARD_DAMAGE_PERCENT | 25;
  if (gMainRoutineWork.next != (s32)&SceneId_LamakanDesertCave) {
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    HeatGauge_Start();
    Task_AddFrame(HeatGauge_RaiseQuarterTriggers, TASK_PASS_FRAME);
    if (gMainRoutineWork.next == (s32)&SceneId_LamakanDesertEast) {
      SAVE_SECRET(0, 22, 7);
      SAVE_SECRET(1, 8, 10);
      SAVE_SECRET(2, 23, 21);
      SAVE_SECRET(3, 16, 42);
      SAVE_SECRET(4, 36, 44);
      SAVE_SECRET(5, 14, 55);
    } else if (gMainRoutineWork.next == (s32)&SceneId_LamakanDesertSmog) {
      SAVE_SECRET(0, 42, 5);
      SAVE_SECRET(1, 20, 11);
      SAVE_SECRET(2, 14, 12);
      SAVE_SECRET(3, 56, 18);
      SAVE_SECRET(4, 7, 22);
      SAVE_SECRET(5, 44, 23);
      SAVE_SECRET(6, 38, 24);
      SAVE_SECRET(7, 26, 28);
      SAVE_SECRET(8, 17, 35);
      SAVE_SECRET(9, 50, 36);
      SAVE_SECRET(10, 34, 43);
      SAVE_SECRET(11, 6, 46);
      SAVE_SECRET(12, 27, 55);
      SAVE_SECRET(13, 43, 56);
    } else if (gMainRoutineWork.next == (s32)&SceneId_LamakanDesertManticore) {
      Scene_QueueMusic(SE_AMBIENT_WIND);
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

/// Every frame: draws the heat gauge down the screen's left edge, 8 pixels
/// further out while held.
static void HeatGauge_Draw(void) {
  u8 *buffer;
  s32 tile;
  s16 *colors;
  s16 *color;
  struct ScreenTransitionState *transition;
  u32 *pixels;
  u32 *word;
  u8 *frame;
  u8 *bar;
  struct MenuSprite *sprite;
  s32 slide;
  u32 i;
  u32 *palette;
  u32 red;
  u32 green;
  u32 blue;
  s32 level;
  u32 attributes;
  s32 y;
  u32 offset;

  // The gauge's two halfwords are read signed.
  tile = gVramCacheEntries[*(s16 *)&gHeatGauge_VramSlot].offset >> 5;
  if (*(s16 *)&gHeatGauge_Held != 0) {
    gHeatGaugeSlide = 2;
  } else if (GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS) != 0) {
    if (gHeatGaugeSlide > 0) {
      gHeatGaugeSlide--;
    }
  } else if (gHeatGaugeSlide <= 1) {
    if (++gHeatGaugeSlide == 1) {
      Dma_Set(gHeatGauge_Palette, (void *)&OBJ_PLTT_COLOR(14, 0),
              (DMA_ENABLE | DMA_16BIT) << 16 | 16, REG_DMA3);
    }
  }
  slide = gHeatGaugeSlide;
  if (slide == 0) {
    VramCache_ReleaseVram(*(s16 *)&gHeatGauge_VramSlot);
    return;
  }

  // Window 1's left edge follows the gauge on lines 8 to 151.
  transition = *Ram_DisplayWork;
  if (transition != NULL) {
    u16 *edge = transition->pages[transition->page];
    s32 left = (s16)(slide << 3);

    edge += 3 + 2 * 8;
    for (i = 0; i < 144; i++) {
      *edge = left;
      edge += 2;
    }
  }

  buffer = Heap_Alloc(2304);
  Dma_Set(gHeatGauge_Palette, buffer, (DMA_ENABLE | DMA_16BIT) << 16 | 16,
          REG_DMA3);
  colors = (s16 *)buffer + 6;
  color = colors;
  for (i = 6; i < 12; i++) {
    u16 value = *color;

    red = value & 31;
    green = (value >> 5) & 31;
    blue = ((value >> 10) & 31) - 20;
    level = gHeatGaugeLevel;
    red += level / 3;
    blue = blue - level / 6 + 20;
    if (level > 60 && (gFrameCount & 1)) {
      green += level * 64 / 120 - 32;
    }
    if (red > 31) {
      red = 31;
    }
    if (green > 31) {
      green = 31;
    }
    if (blue > 31) {
      blue = 31;
    }
    *color++ = RGB(red, green, blue);
  }
  palette = (u32 *)&OBJ_PLTT_COLOR(14, 6);
  word = (u32 *)(buffer + 6 * sizeof(u16));
  IoWriteQueue_Store32((u32)palette++, *word++);
  IoWriteQueue_Store32((u32)palette++, *word++);
  IoWriteQueue_Store32((u32)palette++, *word++);

  gHeatGaugeLevel =
      gMainRoutineWork.hazardSteps * 120 / gMainRoutineWork.hazardLimit;
  if (gHeatGaugeLevel > 118) {
    gHeatGaugeFlash = 119;
  }
  if (gHeatGaugeFlash != 0) {
    gHeatGaugeLevel = gHeatGaugeFlash;
    gHeatGaugeFlash -= 8;
    if (gHeatGaugeFlash <= 0) {
      gHeatGaugeFlash = 0;
    }
  }

  // The gauge's tiles, two 8-pixel columns of 4-bit pixels: fill the bar from
  // its top down to the heat level, then close it with the top's first row.
  Dma_Set(gHeatGauge_TileBuffer, buffer, DMA_COPY_WORDS(576), REG_DMA3);
  if (gHeatGaugeFlash <= 118) {
    pixels = (u32 *)buffer + 20;
    {
      u32 fill = 0xeeeeeeee;

      for (i = 12; i < 128 - gHeatGaugeLevel; i++) {
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
  // Lay the frame's opaque pixels over the bar.
  bar = buffer;
  frame = buffer + 1152;
  for (i = 0; i < 1152; i++) {
    u8 pixel = *frame++;

    if (pixel != 0) {
      *bar = pixel;
    }
    bar++;
  }
  VramCache_Load(*(s16 *)&gHeatGauge_VramSlot, 1152, buffer);

  // Four 16x32 sprites down the screen's left edge and the 16x16 cap at the
  // gauge's foot.
  attributes = OBJ_SIZE_16X32;
  sprite = gHeatGauge_Sprites;
  for (i = 0, offset = 0, y = 8; i < 5; i++) {
    u32 x = (gHeatGaugeSlide * 8 - 16) & 0x1ff;

    if (i == 4) {
      attributes = OBJ_SIZE_16X16;
    }
    sprite->next = NULL;
    sprite->oam.word.attr01 = (x << 16) | y | attributes;
    sprite->oam.word.attr23 = tile | OBJ_PRIORITY(1) | OBJ_PALETTE(14);
    Oam_AddSpriteAtDepth(((u8 *)gHeatGauge_Sprites + offset), 255);
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
  VramCache_Load((s16)gHeatGauge_VramSlot, 1152, 0);
  gHeatGaugeSlide = 0;
  gHeatGauge_Held = 0;
  Task_AddFrame(HeatGauge_Draw, TASK_PASS_FRAME - 10);
}

/// While held (1), the gauge stays on screen.
static void HeatGauge_SetHeld(u16 value) { gHeatGauge_Held = value; }

#define FIELD_EFFECT_DURATION_SCRIPT_QUALIFIER const
#include "shared/field_effect_duration_scripts.h"

// clang-format off
/// Each map's oases, the middle of each as x, z pairs.
const s32 gLamakanDesertEast_Oases[] = {
    px(384), px(128),
    px(288), px(688),
    px(608), px(720),
};

const s32 gLamakanDesertSmog_Oases[] = {
    px(704), px(96),
    px(448), px(464),
    px(832), px(592),
    px(128), px(752),
    px(464), px(896),
};

const s32 gLamakanDesertOasesOther[] = {
    px(192), px(224),
    px(192), px(352),
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
    { TRIGGER_STEP, 9, CONDITION_ALWAYS, EVENT(LamakanDesert_LeaveForTheWorldMap) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LAMAKAN_DESERT_OASIS, FLAG_DESERT_EVENT_RUNNING, EVENT(LamakanDesert_BatheInOasis) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LAMAKAN_DESERT_SAND, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LAMAKAN_DESERT_PIT, FLAG_DESERT_EVENT_RUNNING, EVENT(LamakanDesert_FallIntoPit) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LAMAKAN_DESERT_SLOPE, CONDITION_ALWAYS, EVENT(LamakanDesert_SlideDownDune) },
    { TRIGGER_STEP, 40, CONDITION_ALWAYS, EVENT(LamakanDesert_MeetManticore) },
    { TRIGGER_TALK, ACTOR_MANTICORE, FLAG_LAMAKAN_MANTICORE_BEATEN, EVENT(LamakanDesert_FightManticore) },
    { TRIGGER_TALK, ACTOR_SMOG, CONDITION_ALWAYS, EVENT(LamakanDesert_TalkToSmog) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_LAMAKAN_DESERT_VULCAN_AXE, FIND_ITEM(ITEM_VULCAN_AXE) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 101, FLAG_LAMAKAN_DESERT_FIRST_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_LAMAKAN_DESERT_LUCKY_PEPPER, FIND_ITEM(ITEM_LUCKY_PEPPER) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 103, FLAG_LAMAKAN_DESERT_SECOND_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 105, FLAG_LAMAKAN_DESERT_777_COINS, FIND_COINS(777) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 106, FLAG_LAMAKAN_DESERT_POTION, FIND_ITEM(ITEM_POTION) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 107, FLAG_LAMAKAN_DESERT_MINT, FIND_ITEM(ITEM_MINT) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Reveal_ShowSecrets) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Reveal_HideSecrets) },
    { TRIGGER_RAISED, LAMAKAN_DESERT_RAISED_HEAT_QUARTER, FLAG_SECRETS_REVEALED, EVENT(LamakanDesert_ComplainAboutHeat) },
    { TRIGGER_RAISED, LAMAKAN_DESERT_RAISED_HEAT_HALF, FLAG_SECRETS_REVEALED, EVENT(LamakanDesert_ComplainAboutHeat) },
    { TRIGGER_RAISED, LAMAKAN_DESERT_RAISED_HEAT_THREE_QUARTERS, FLAG_SECRETS_REVEALED, EVENT(LamakanDesert_ComplainAboutHeat) },
    { TRIGGER_STEP, 100, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_STEP, 101, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_STEP, 102, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_STEP, 103, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_STEP, 104, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_STEP, 105, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_STEP, 106, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_STEP, 107, CONDITION_ALWAYS, EVENT(HeatGauge_CountStep) },
    { TRIGGER_TABLE_END },
};
// clang-format on

/// The gauge: how full it is (0 to 120), how far it has slid onto the
/// screen, and a flash as it fills.
s16 gHeatGaugeLevel = 0;

s16 gHeatGaugeSlide = 0;

s16 gHeatGaugeFlash = 0;
