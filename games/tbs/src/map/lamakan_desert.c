/// @file
/// Lamakan Desert (MapId_LamakanDesertEast to MapId_LamakanDesertCave):
/// four maps that are all Lamakan Desert in the game's area names and share one
/// map event table, built around a heat gauge and the secrets Reveal uncovers.
///
/// The heat is a count of steps on hazard ground (gGameState.hazardSteps):
/// LamakanDesert_Initialize sets a limit of 600 on every map, at which the
/// party loses a quarter of its HP, and on all but LamakanDesertCave starts the
/// heat gauge, a thermometer down the screen's left edge (HeatGauge_Draw). Each
/// moving frame on the sand adds a step (HeatGauge_CountStep,
/// LAMAKAN_DESERT_CELL_SAND and the bg events' metatile behaviors 100 to 107).
/// As the gauge passes a quarter, a half and three quarters,
/// HeatGauge_RaiseQuarterTriggers sets FLAG_HEAT_QUARTER to
/// FLAG_HEAT_THREE_QUARTERS and raises LAMAKAN_DESERT_RAISED_HEAT_QUARTER to
/// LAMAKAN_DESERT_RAISED_HEAT_THREE_QUARTERS, and Garet, Ivan or Mia complains
/// of the heat (LamakanDesert_ComplainAboutHeat). Stepping into an oasis
/// (LAMAKAN_DESERT_CELL_OASIS), the leader hops into the middle of the nearest
/// one and bathes until the heat has drained (LamakanDesert_BatheInOasis), and
/// a running Reveal ends.
///
/// Entry saves the patches of the map grid that hold the desert's secrets (the
/// oases, the Ant Lions' pits, the Manticore, Smog, a Jupiter Djinni, and the
/// chests and Psynergy Stones) and hides them under a shimmering heat haze
/// (Reveal_HideSecrets); Reveal brings them back and stops the haze
/// (Reveal_ShowSecrets) until it wears off. The oases' and pits' cells exist
/// only while Reveal shines, as the monks of Lama Temple warn: `Reveal will
/// help you find oases to cool you against the heat.`
///
/// Stepping by a pit (LAMAKAN_DESERT_CELL_PIT) pulls the leader into it for a
/// battle with an Ant Lion (LamakanDesert_FallIntoPit), and slopes of sand
/// (LAMAKAN_DESERT_CELL_SLOPE) carry the leader south
/// (LamakanDesert_SlideDownDune).
///
/// In LamakanDesertManticore, until it is beaten
/// (FLAG_LAMAKAN_MANTICORE_BEATEN), the Manticore rises out of the sand as the
/// leader comes near (coord event 40, LamakanDesert_MeetManticore). Talking to
/// it starts its battle, which ends at LamakanDesertManticore's
/// LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE, and arriving there marks it
/// beaten.
///
/// LamakanDesertEast's LAMAKAN_DESERT_EAST_TO_WORLD_MAP and LamakanDesertCave's
/// LAMAKAN_DESERT_CAVE_TO_WORLD_MAP lead out onto the world map;
/// LamakanDesertCave, a cave, has no heat gauge. Coord event 9 takes
/// LAMAKAN_DESERT_CAVE_TO_WORLD_MAP, and leaving that way while Hsu is still
/// trapped (FLAG_HSU_RESCUED clear) sets FLAG_HSU_RESCUED and
/// FLAG_HSU_RESCUED_WITHOUT_PARTY, which xian_interiors.c reads to have Feizhi
/// thank Master Hama alone.
///
/// Open questions:
///
/// - XXX (no evidence): why the patched editions (TBS_PATCH_3) set
///   FLAG_KALAY_REACHED on coming back from the Manticore's battle, so that a
///   party that has beaten the Manticore counts as having reached Kalay before
///   it enters the town; the code shows the effect, but only Camelot's notes
///   for the patch would say what it fixed.

#include "vram_tab.h"
#include "unit/import.h"
#include "unit/map/lamakan_desert.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "object_event.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "fixed_point_position.h"
#include "field_runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "text/msg_ids.h"
#include "map_ids.h"
#include "iwram_call.h"
#include "field_service.h"
#include "field_effect.h"
#include "game_state.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "mem.h"
#include "dma.h"
#include "io_write_queue.h"
#include "menu_list.h"
#include "heap_state.h"
#include "ram_buffer.h"
#include "vram_block.h"
#include "io_reg.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "songs.h"
#include "flags.h"
#include "djinn_ids.h"
#include "item_ids.h"

static void HeatGauge_SetHeld(u16 held);

/// The values HeatGauge_RaiseQuarterTriggers raises as the heat gauge passes
/// each mark, and the metatile behaviors of the desert's ground.
enum LamakanDesertTrigger {
  LAMAKAN_DESERT_RAISED_HEAT_QUARTER = 1,
  LAMAKAN_DESERT_RAISED_HEAT_HALF = 2,
  LAMAKAN_DESERT_RAISED_HEAT_THREE_QUARTERS = 3,
  /// An oasis's water, in the patches Reveal shows (renders of the maps with
  /// the patches shown: the pool at (384, 128) in LamakanDesertEast).
  LAMAKAN_DESERT_CELL_OASIS = 20,
  LAMAKAN_DESERT_CELL_SAND = 21, ///< sand, where each moving frame adds heat
  LAMAKAN_DESERT_CELL_PIT = 22,  ///< by an Ant Lion's pit, under Reveal too
  LAMAKAN_DESERT_CELL_SLOPE = 30 ///< sand that carries the leader south
};

enum {
  /// FLAG_MAP_TEMP_00: a bath, a pit or the Manticore is under way.
  /// ★ Named from its area name: Lamakan Desert
  FLAG_DESERT_EVENT_RUNNING = FLAG_MAP_TEMP_00,
  /// FLAG_MAP_TEMP_01: Reveal shows the pits.
  /// ★ Named from the game: "Reveal" (MsgAbilityName)
  FLAG_SECRETS_REVEALED = FLAG_MAP_TEMP_01,
  /// The heat gauge has passed a quarter, where the first complaints come.
  /// ★ Named from the game: "This desert is much hotter than I ever imagined"
  /// (MsgMountainNorthAltinMineWestLama)
  FLAG_HEAT_QUARTER = FLAG_MAPSEC_TEMP_00,
  /// ★ Named from the game: "I can't keep my cool when it's this hot"
  /// (MsgMountainNorthAltinMineWestLama)
  FLAG_HEAT_HALF = FLAG_MAPSEC_TEMP_01,
  /// ★ Named from the game: "I can't stand another minute of this!"
  /// (MsgMountainNorthAltinMineWestLama)
  FLAG_HEAT_THREE_QUARTERS = FLAG_MAPSEC_TEMP_02,
  /// Who has complained since the gauge last passed a mark: one per member,
  /// in the order of their object events.
  /// ★ Named from the game: "My feet are burning!"
  /// (MsgMountainNorthAltinMineWestLama)
  FLAG_GARET_COMPLAINED = FLAG_MAPSEC_TEMP_03,
  /// ★ Named from the game: "Just walking around here saps every ounce of my
  /// strength." (MsgMountainNorthAltinMineWestLama)
  FLAG_IVAN_COMPLAINED = FLAG_MAPSEC_TEMP_04,
  /// ★ Named from the game: "My clothes are drenched in sweat!"
  /// (MsgMountainNorthAltinMineWestLama)
  FLAG_MIA_COMPLAINED = FLAG_MAPSEC_TEMP_05
};

/// The object events, numbered in the order of each map's object event
/// templates from LOCALID_FIRST_PLACED. LamakanDesertEast to
/// LamakanDesertManticore place the Manticore, then the LamakanDesertEast and
/// LamakanDesertManticore one pit and LamakanDesertSmog all four pits and Smog;
/// LamakanDesertCave places none.
enum {
  MANTICORE = LOCALID_FIRST_PLACED, ///< talking to it starts its battle
  /// The first of the Ant Lions' pits Reveal shows, up to LAST_PIT.
  FIRST_PIT = MANTICORE + 1,
  LAST_PIT = FIRST_PIT + 3,
  SMOG = LAST_PIT + 1 ///< Smog, a Jupiter Djinni
};

/// The desert's two Psynergy Stones, map-section temp flags: each is back once
/// the party has left Lamakan Desert.
enum {
  /// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
  FLAG_LAMAKAN_DESERT_FIRST_PSYNERGY_STONE = FLAG_MAPSEC_TEMP_50,
  /// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
  FLAG_LAMAKAN_DESERT_SECOND_PSYNERGY_STONE = FLAG_MAPSEC_TEMP_51
};

#define NULL ((void *)0)

#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// LamakanDesertSmog: talking to Smog, whom Reveal uncovers, adds the
/// Jupiter Djinni to the party.
static void LamakanDesert_TalkToSmog(void) {
  Djinn_JoinParty(SMOG, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_SMOG));
}

#include "common/object_waitdown.h"

// The update a leader pulled into an Ant Lion's pit runs: sand sounds every 8
// frames and a spray of sand every 16.
// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 104 to 100 bytes in tbs-en.
#ifdef UBFIX
static void Leader_SpraySand(union FieldObject *fieldObject) {
#else
static s32 Leader_SpraySand(union FieldObject *fieldObject) {
#endif
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  struct EffectOptions params;
  s32 phase;

  if ((gFrameCount & 7) == 0) {
    Engine_AudioPlayCue(SE_118);
  }
  phase = gFrameCount & 15;
  if (phase != 0) {
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  }
  params.startScaleX = 0xcccc;
  params.startScaleY = 0xcccc;
  params.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
  FieldEffect_Spawn(
      objectEvent->x.fixed, objectEvent->y.fixed, objectEvent->z.fixed, 0,
      phase, phase,
      EFFECT_USE_SPIN | EFFECT_USE_START_SCALE | EFFECT_SCRIPT_MEDIUM, &params);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Where the party can enter each of the four maps.
const struct MapWarpDest *LamakanDesert_GetWarpDests(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_LamakanDesertEast) {
    return gLamakanDesertEastWarpDests;
  }
  if (mapId == (s32)&MapId_LamakanDesertSmog) {
    return gLamakanDesertSmogWarpDests;
  }
  if (mapId == (s32)&MapId_LamakanDesertManticore) {
    return gLamakanDesertManticoreWarpDests;
  }
  if (mapId == (s32)&MapId_LamakanDesertCave) {
    return gLamakanDesertCaveWarpDests;
  }
  return gLamakanDesertWarpDestsOther;
}

const struct MapWarpRegion *LamakanDesert_GetRegions(void) { return 0; }

/// All four desert maps share the packed warp table.
const u32 *LamakanDesert_GetWarps(void) { return gLamakanDesertWarps; }

/// The pits, the Manticore and Smog of each map. Arriving at
/// LamakanDesertManticore's LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE, the
/// way back from the Manticore's battle, marks it beaten; in the patched
/// editions it also sets FLAG_KALAY_REACHED.
const struct ObjectEventTemplate *LamakanDesert_GetObjectEventTemplates(void) {
  if (gGameState.mapId == (s32)&MapId_LamakanDesertManticore) {
    if (gGameState.warpDest == LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE) {
      Engine_GameFlagSet(FLAG_LAMAKAN_MANTICORE_BEATEN);
#if TBS_PATCH_3
      Engine_GameFlagSet(FLAG_KALAY_REACHED);
#endif
    }
  }
  if (gGameState.mapId == (s32)&MapId_LamakanDesertEast) {
    return gLamakanDesertEastObjectEventTemplates;
  }
  if (gGameState.mapId == (s32)&MapId_LamakanDesertSmog) {
    return gLamakanDesertSmogObjectEventTemplates;
  }
  if (gGameState.mapId == (s32)&MapId_LamakanDesertManticore) {
    return gLamakanDesertManticoreObjectEventTemplates;
  }
  return gLamakanDesertObjectEventTemplatesOther;
}

/// Lamakan Desert, coord event 9: LAMAKAN_DESERT_CAVE_TO_WORLD_MAP. If
/// Master Hsu is still trapped, he is rescued without the party (see the file
/// comment).
static void LamakanDesert_LeaveForTheWorldMap(void) {
  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_HSU_RESCUED) == 0) {
    if (GameFlag_IsSet(FLAG_HSU_RESCUED_WITHOUT_PARTY) == 0) {
      GameFlag_Set(FLAG_HSU_RESCUED_WITHOUT_PARTY);
      GameFlag_Set(FLAG_HSU_RESCUED);
    }
  }
  Script_PlayCue(SE_MAP_EXIT);
  Engine_ScriptRequestWarp(LAMAKAN_DESERT_CAVE_TO_WORLD_MAP);
  Engine_ScriptEnd();
}

/// The desert's map events, the same on every map.
const struct MapEvent *LamakanDesert_GetEvents(void) {
  return gLamakanDesertEvents;
}

// Native event return forms: see MATCHING.md.
#ifdef UBFIX
/// Reveal wears off, and the desert's secrets sink under the sand again. It
/// also runs on every entry to Lamakan Desert: the pits, the Manticore and Smog
/// vanish under the sand (cells 70, 68 or 69, 99 over the saved patches), the
/// chests and Psynergy Stones hide and the heat haze shimmers.
static void Reveal_HideSecrets(void) {
#else
/// Reveal wears off, and the desert's secrets sink under the sand again. It
/// also runs on every entry to Lamakan Desert: the pits, the Manticore and Smog
/// vanish under the sand (cells 70, 68 or 69, 99 over the saved patches), the
/// chests and Psynergy Stones hide and the heat haze shimmers.
static s32 Reveal_HideSecrets(void) {
#endif
  s32 markerId;

  Engine_GameFlagClear(FLAG_SECRETS_REVEALED);
  if (gGameState.mapId == (s32)&MapId_LamakanDesertEast) {
    MapGrid_CopyCells(70, 68, 4, 2, 22, 7);
    MapGrid_CopyCells(70, 68, 4, 2, 8, 10);
    MapGrid_CopyCells(70, 68, 4, 2, 23, 21);
    MapGrid_CopyCellAttributes(70, 68, 4, 1, 23, 23);
    MapGrid_CopyCells(70, 68, 4, 2, 16, 42);
    MapGrid_CopyCells(70, 68, 4, 2, 36, 44);
    MapGrid_CopyCells(70, 68, 4, 2, 14, 55);
  } else if (gGameState.mapId == (s32)&MapId_LamakanDesertSmog) {
    MapGrid_CopyCells(70, 68, 4, 2, 42, 5);
    MapGrid_CopyCells(70, 68, 4, 2, 20, 11);
    MapGrid_CopyCellAttributes(70, 68, 4, 1, 20, 13);
    MapGrid_CopyCells(70, 68, 4, 2, 14, 12);
    MapGrid_CopyCells(70, 68, 4, 2, 56, 18);
    MapGrid_CopyCells(70, 68, 4, 2, 7, 22);
    MapGrid_CopyCellAttributes(70, 68, 4, 1, 7, 24);
    MapGrid_CopyCells(70, 68, 4, 2, 44, 23);
    MapGrid_CopyCellAttributes(70, 68, 4, 1, 44, 25);
    MapGrid_CopyCells(70, 68, 4, 2, 38, 24);
    MapGrid_CopyCells(70, 68, 4, 2, 26, 28);
    MapGrid_CopyCells(70, 68, 4, 2, 17, 35);
    MapGrid_CopyCells(70, 68, 4, 2, 50, 36);
    MapGrid_CopyCells(70, 68, 4, 2, 34, 43);
    MapGrid_CopyCellAttributes(70, 68, 4, 1, 34, 45);
    MapGrid_CopyCells(70, 68, 4, 2, 6, 46);
    MapGrid_CopyCells(70, 68, 4, 2, 27, 55);
    MapGrid_CopyCells(70, 68, 4, 2, 43, 56);
  } else if (gGameState.mapId == (s32)&MapId_LamakanDesertManticore) {
    Engine_MapGridCopyCellsLayered(69, 99, 4, 2, 8, 16);
    Engine_MapGridCopyCellsLayered(69, 99, 4, 2, 6, 20);
    Engine_MapGridCopyCellsLayered(69, 99, 4, 2, 10, 23);
    MapGrid_CopyCellAttributes(69, 99, 4, 2, 8, 14);
    MapGrid_CopyCellAttributes(69, 99, 4, 2, 6, 18);
    MapGrid_CopyCellAttributes(69, 99, 4, 1, 6, 20);
    Engine_MapGridCopyCellAttributes(69, 99, 4, 2, 10, 21);
    MapGrid_CopyCells(0, 121, 5, 7, 8, 32);
    MapGrid_CopyCells(0, 121, 5, 7, 43, 32);
    MapGrid_CopyCells(6, 120, 3, 1, 9, 5);
    MapGrid_CopyCells(9, 120, 3, 1, 44, 5);
    MapGrid_CopyCellAttributes(9, 0, 3, 3, 9, 6);
  }
  Engine_ObjectEventSetPosition(MANTICORE, 0, 0);
  Engine_ObjectEventSetPosition(FIRST_PIT, 0, 0);
  Engine_ObjectEventSetPosition(FIRST_PIT + 1, 0, 0);
  Engine_ObjectEventSetPosition(FIRST_PIT + 2, 0, 0);
  Engine_ObjectEventSetPosition(LAST_PIT, 0, 0);
  Engine_ObjectEventSetPosition(SMOG, 0, 0);
  // Hide the searchable markers independently of the object event templates.
  markerId = 100;
  do {
    Engine_MarkerSetPosition(markerId, 0, 0);
    markerId = markerId + 1;
  } while (markerId <= 107);
  if (gGameState.mapId != (s32)&MapId_LamakanDesertCave) {
    DisplayScroll_StartHBlankDma(0, 0x40000, 0x10000, 0x2000, 0x10000, 0x8000,
                                 0x4000);
  }
}

#ifdef UBFIX
/// Casting Reveal uncovers what the desert's sand hides. In Lamakan Desert, the
/// saved patches of sand come back, the pits, the Manticore and Smog stand
/// where they are, the chests and Psynergy Stones show and the haze stops.
static void Reveal_ShowSecrets(void) {
#else
/// Casting Reveal uncovers what the desert's sand hides. In Lamakan Desert, the
/// saved patches of sand come back, the pits, the Manticore and Smog stand
/// where they are, the chests and Psynergy Stones show and the haze stops.
static s32 Reveal_ShowSecrets(void) {
#endif
  s32 i;

  Engine_GameFlagClear(FLAG_DESERT_EVENT_RUNNING);
  Engine_GameFlagSet(FLAG_SECRETS_REVEALED);
  // Restore each hidden patch from the spare cells saved on entry.
  if (gGameState.mapId == (s32)&MapId_LamakanDesertEast) {
    MapGrid_CopyCells(64, 126, 4, 2, 22, 7);
    MapGrid_CopyCells(68, 126, 4, 2, 8, 10);
    MapGrid_CopyCells(72, 126, 4, 2, 23, 21);
    MapGrid_CopyCellAttributes(72, 126, 4, 2, 23, 22);
    MapGrid_CopyCells(76, 126, 4, 2, 16, 42);
    MapGrid_CopyCells(80, 126, 4, 2, 36, 44);
    MapGrid_CopyCells(84, 126, 4, 2, 14, 55);
    ObjectEvent_SetPosition(FIRST_PIT, PIXELS(400), PIXELS(364));
  } else if (gGameState.mapId == (s32)&MapId_LamakanDesertSmog) {
    MapGrid_CopyCells(64, 126, 4, 2, 42, 5);
    MapGrid_CopyCells(68, 126, 4, 2, 20, 11);
    MapGrid_CopyCellAttributes(68, 126, 4, 2, 20, 12);
    MapGrid_CopyCells(72, 126, 4, 2, 14, 12);
    MapGrid_CopyCells(76, 126, 4, 2, 56, 18);
    MapGrid_CopyCells(80, 126, 4, 2, 7, 22);
    MapGrid_CopyCellAttributes(80, 126, 4, 2, 7, 23);
    MapGrid_CopyCells(84, 126, 4, 2, 44, 23);
    MapGrid_CopyCellAttributes(84, 126, 4, 2, 44, 24);
    MapGrid_CopyCells(88, 126, 4, 2, 38, 24);
    MapGrid_CopyCells(92, 126, 4, 2, 26, 28);
    MapGrid_CopyCells(96, 126, 4, 2, 17, 35);
    MapGrid_CopyCells(100, 126, 4, 2, 50, 36);
    MapGrid_CopyCells(104, 126, 4, 2, 34, 43);
    MapGrid_CopyCellAttributes(104, 126, 4, 2, 34, 44);
    MapGrid_CopyCells(108, 126, 4, 2, 6, 46);
    MapGrid_CopyCells(112, 126, 4, 2, 27, 55);
    MapGrid_CopyCells(116, 126, 4, 2, 43, 56);
    ObjectEvent_SetPosition(FIRST_PIT, PIXELS(352), PIXELS(204));
    ObjectEvent_SetPosition(FIRST_PIT + 1, PIXELS(736), PIXELS(396));
    ObjectEvent_SetPosition(FIRST_PIT + 2, PIXELS(144), PIXELS(380));
    ObjectEvent_SetPosition(LAST_PIT, PIXELS(576), PIXELS(716));
    ObjectEvent_SetPosition(SMOG, PIXELS(648), PIXELS(408));
  } else if (gGameState.mapId == (s32)&MapId_LamakanDesertManticore) {
    MapGrid_CopyCells(64, 124, 4, 4, 8, 14);
    MapGrid_CopyCells(68, 124, 4, 4, 6, 18);
    MapGrid_CopyCellAttributes(68, 124, 4, 1, 6, 20);
    MapGrid_CopyCells(72, 124, 4, 4, 10, 21);
    MapGrid_CopyCells(10, 121, 5, 7, 8, 32);
    MapGrid_CopyCells(5, 121, 5, 7, 43, 32);
    MapGrid_CopyCells(0, 120, 3, 1, 9, 5);
    MapGrid_CopyCells(3, 120, 3, 1, 44, 5);
    ObjectEvent_SetPosition(MANTICORE, PIXELS(168), PIXELS(92));
    ObjectEvent_SetPosition(FIRST_PIT, PIXELS(128), PIXELS(316));
    MapGrid_CopyCellAttributes(6, 0, 3, 3, 9, 6);
    if (Engine_GameFlagIsSet(FLAG_LAMAKAN_MANTICORE_BEATEN) == 0) {
      MapGrid_CopyCells(0, 119, 3, 1, 9, 5);
    }
  }
  for (i = 100; i <= 107; i++) {
    Marker_SetPosition(i, -1, -1);
  }
  Marker_MoveLeaderAway();
  if (gGameState.mapId != (s32)&MapId_LamakanDesertCave) {
    DisplayScroll_DisarmHBlankDma();
  }
}

/// The party fights the Manticore. In LamakanDesertManticore, talking to the
/// Manticore starts its battle, which returns the party to
/// LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE.
static void LamakanDesert_FightManticore(void) {
  Engine_ObjectEventRunRepeatedMotion(MANTICORE, 2);
  FieldScript_SetNextDestination(
      (s32)&MapId_LamakanDesertManticore,
      LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE);
  Battle_SetStart(BATTLE_START_SCRIPTED);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 5);
}

/// Garet, Ivan or Mia complains about the heat. In Lamakan Desert this is
/// raised as LAMAKAN_DESERT_RAISED_HEAT_QUARTER to _THREE_QUARTERS as the heat
/// gauge passes a quarter, a half and three quarters, and the one who speaks is
/// picked at random (three tries for one who has not spoken yet).
static void LamakanDesert_ComplainAboutHeat(s32 mark) {
  s32 tries;
  s32 quarter;
  s32 alreadySpoke;
  s32 random;
  s32 flag;
  s32 member;

  quarter = mark;
  for (tries = 0; tries <= 2; tries++) {
    random = Engine_RandomNext();
    member = (u32)((random << 1) + random) >> 16; // 0 to 2
    flag = member + FLAG_GARET_COMPLAINED;
    alreadySpoke = Engine_GameFlagIsSet(flag);
    if (alreadySpoke == 0) {
      Engine_GameFlagSet(flag);
      break;
    }
  }
  Engine_ScriptStart();
  // Three lines a mark, one for each member. They come right after Alpine
  // Crossing's signpost (`North: Altin Mine West: Lama Temple`) and have no
  // names of their own, so they count from a Fuchin Temple message two before
  // it.
  Engine_ScriptSetMessage((((quarter << 1) + quarter) + member) +
                          MSGNO(MsgFuchinTempleIDoNotThinkMasterHama));
  Engine_ScriptShowMessage((member + 1), 0);
  Engine_ScriptEnd();
}

/// Every frame: raises LAMAKAN_DESERT_RAISED_HEAT_QUARTER to _THREE_QUARTERS as
/// the gauge passes each quarter, and clears each mark and the complaints when
/// the gauge drops back below it. Nothing is raised while Reveal is showing the
/// pits.
static void HeatGauge_RaiseQuarterTriggers(void) {
  struct EventWork *work = gEventWork;
  s32 percent = gGameState.hazardSteps * 100 / gGameState.hazardLimit;

  if (GameFlag_IsSet(FLAG_SECRETS_REVEALED) != 0)
    return;
  if (GameFlag_IsSet(FLAG_HEAT_THREE_QUARTERS) != 0 && percent <= 74) {
    GameFlag_Clear(FLAG_HEAT_THREE_QUARTERS);
    GameFlag_Clear(FLAG_GARET_COMPLAINED);
    GameFlag_Clear(FLAG_IVAN_COMPLAINED);
    GameFlag_Clear(FLAG_MIA_COMPLAINED);
  }
  if (GameFlag_IsSet(FLAG_HEAT_HALF) != 0 && percent <= 49) {
    GameFlag_Clear(FLAG_HEAT_HALF);
    GameFlag_Clear(FLAG_GARET_COMPLAINED);
    GameFlag_Clear(FLAG_IVAN_COMPLAINED);
    GameFlag_Clear(FLAG_MIA_COMPLAINED);
  }
  if (GameFlag_IsSet(FLAG_HEAT_QUARTER) != 0 && percent <= 24) {
    GameFlag_Clear(FLAG_HEAT_QUARTER);
    GameFlag_Clear(FLAG_GARET_COMPLAINED);
    GameFlag_Clear(FLAG_IVAN_COMPLAINED);
    GameFlag_Clear(FLAG_MIA_COMPLAINED);
  }
  if (GameFlag_IsSet(FLAG_HEAT_QUARTER) == 0 && percent > 24) {
    GameFlag_Set(FLAG_HEAT_QUARTER);
    work->raisedTrigger = LAMAKAN_DESERT_RAISED_HEAT_QUARTER;
  }
  if (GameFlag_IsSet(FLAG_HEAT_HALF) == 0 && percent > 49) {
    GameFlag_Set(FLAG_HEAT_HALF);
    work->raisedTrigger = LAMAKAN_DESERT_RAISED_HEAT_HALF;
  }
  if (GameFlag_IsSet(FLAG_HEAT_THREE_QUARTERS) == 0 && percent > 74) {
    GameFlag_Set(FLAG_HEAT_THREE_QUARTERS);
    work->raisedTrigger = LAMAKAN_DESERT_RAISED_HEAT_THREE_QUARTERS;
  }
}

/// The distance in whole pixels between two x, z positions.
static s32 LamakanDesert_DistanceInPixels(s32 *firstPosition,
                                          s32 *secondPosition) {
  s32 dx = (*secondPosition++ - *firstPosition++) >> 16;
  s32 dz = (*secondPosition - firstPosition[1]) >> 16;
  s32 squaredDeltaZ = dz * dz;
  s32 squaredDeltaX = dx * dx;

  return Iwram_Sqrt(squaredDeltaX + squaredDeltaZ);
}

/// The middle of an oasis, where a leader stepping into it bathes.
struct LamakanDesertOasis {
  s32 x;
  s32 z;
};

/// Lamakan Desert, stepping into an oasis (LAMAKAN_DESERT_CELL_OASIS), which
/// only Reveal shows: the leader hops into the middle of the nearest of the
/// map's oases, bathes with a heart over his head while the gauge drains 5
/// steps a frame, for at least a second, then hops out.
static void LamakanDesert_BatheInOasis(void) {
  u8 *work;
  s32 frames;
  s32 best;
  s32 count;
  const s32 *points;
  s32 left;
  // Global allocation ranks point (5 refs over 12 instructions), count (13
  // over 56) and offset (7 over 34) and hands out r5, r6 and r7 in that order;
  // the native needs offset between the other two (8 or 9 refs).
  /* FAKEMATCH(pin): offset fixed in r6, which leaves r7 to count; as a plain
   * s32 offset, 11 of 228 instructions differ in tbs-en. */
  register s32 offset asm("r6");
  const s32 *point;
  s32 nearest;
  s32 distance;
  struct ObjectEvent *leader;
  struct EffectOptions options;
  struct EffectOptions puff;
  s16 *progress;
  u16 *revealFrames;
  s32 zero;

  work = (u8 *)gEventWork;
  frames = 60;
  best = 0xf00000;
  GameFlag_Set(FLAG_DESERT_EVENT_RUNNING);
  HeatGauge_SetHeld(1);
  if (gGameState.mapId == (s32)&MapId_LamakanDesertEast) {
    count = 3;
    points = gLamakanDesertEast_Oases;
  } else if (gGameState.mapId == (s32)&MapId_LamakanDesertSmog) {
    count = 5;
    points = gLamakanDesertSmog_Oases;
  } else {
    count = 2;
    points = gLamakanDesertOasesOther;
  }
  left = count;
  // The selected table has 2, 3 or 5 points. The square root fits in 16 bits,
  // below best, so the first iteration always sets nearest; ties use the last.
  point = points;
  if (count != 0) {
    offset = 0;
    do {
      distance = LamakanDesert_DistanceInPixels(
          &ObjectEvent_Get(LOCALID_PARTY_LEADER)->x.fixed, (s32 *)point);
      if (distance <= best) {
        best = distance;
        nearest = left - count;
      }
      offset += 8;
      point = (const s32 *)((const u8 *)points + offset);
    } while (--count != 0);
  }
  nearest <<= 1;
  // Hop the leader into the middle of the oasis before draining the heat gauge.
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x20000, 0x10000);
  {
    struct ObjectEvent *objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    const struct LamakanDesertOasis *spot =
        (const struct LamakanDesertOasis *)&points[nearest];

    FieldObject_SetPosition(objectEvent, spot->x, 0, spot->z);
  }
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->velocityY = 0x60000;
  Script_PlayCue(SE_JUMP);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  MapObjectEvent_WaitBelowHeight(
      leader, ObjectEvent_Get(LOCALID_PARTY_LEADER)->y.fixed);
  Script_PlayCue(SE_WATER_MOVE);
  {
    struct ObjectEvent *objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);

    options.type = 214;
    options.startScaleX = 0x8000;
    options.startScaleY = 0xcccc;
    options.targetScaleX = 0x10000;
    options.targetScaleY = 0x13333;
    FieldEffect_Spawn(
        objectEvent->x.fixed, objectEvent->y.fixed, objectEvent->z.fixed, 0, 0,
        0, EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET,
        &options);
  }
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_HEART, 0);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIMATION_LEADER_BATHE);
  revealFrames = (u16 *)&((struct FieldWork *)work)->revealFrames;
  progress = &gGameState.hazardSteps;
  zero = 0;
  // A running Reveal's countdown stays full while the leader bathes,
  // at least 60 frames and until the heat has drained.
  do {
    s32 held = 600;

    *revealFrames = held;
    frames--;
    if (*progress != 0) {
      *progress -= 5;
      if (*progress <= 0) {
        *progress = zero;
      } else if (frames == 0) {
        frames = 1;
      }
    }
    Task_Wait(1);
  } while (frames != 0);
  {
    struct ObjectEvent *objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);

    puff.type = 214;
    puff.startScaleX = 0x8000;
    puff.startScaleY = 0xcccc;
    puff.targetScaleX = 0x8000;
    puff.targetScaleY = 0x13333;
    FieldEffect_Spawn(objectEvent->x.fixed, objectEvent->y.fixed,
                      objectEvent->z.fixed, 0, frames, frames,
                      EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                          EFFECT_SCALE_TO_TARGET,
                      &puff);
  }
  Script_PlayCue(SE_STOP_SLOT3);
  Script_PlayCue(SE_JUMP);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->velocityY = 0x60000;
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Script_Wait(10);
  // frames is 0 here, so a running Reveal ends as the leader gets up.
  *(u16 *)&((struct FieldWork *)work)->revealFrames = frames;
  HeatGauge_SetHeld(0);
}

/// Lamakan Desert, walking on sand (coord events 21 and 100 to 107): each
/// moving frame adds a step of heat and kicks up dust, faster on the run.
static void HeatGauge_CountStep(void) {
  struct ObjectEvent *objectEvent;
  struct EventWork *work;
  struct EffectOptions params;
  s32 dx;
  s32 z;
  s32 phase;
  objectEvent = ObjectEvent_GetById(gGameState.selectedLocalId);
  work = gEventWork;
  if (objectEvent->targetX == OBJ_EVENT_NO_TARGET) {
    return;
  }
  gGameState.hazardSteps++;
  if (work->touchedTrigger == LAMAKAN_DESERT_CELL_SLOPE) {
    return;
  }
  if (objectEvent->speed <= 0x10000) {
    if ((gFrameCount & 15) != 0) {
      return;
    }
    params.startScaleX = 0x8000;
    params.startScaleY = 0x8000;
    dx = 0;
    params.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    if (objectEvent->facing != 0 && objectEvent->facing != 0x8000) {
      z = objectEvent->z.fixed;
      dx = 0x20000 - ((((z >> 20) & 1) * 5) << 16);
    } else {
      z = objectEvent->z.fixed;
    }
    FieldEffect_Spawn(
        objectEvent->x.fixed + dx, objectEvent->y.fixed, z, 0, 0, 0,
        EFFECT_USE_SPIN | EFFECT_USE_START_SCALE | EFFECT_SCRIPT_MEDIUM,
        &params);
  } else {
    phase = gFrameCount & 7;
    if (phase != 0) {
      return;
    }
    // Reads the frame counter again and drops the value, as the native does.
    gFrameCount;
    params.startScaleX = 0xcccc;
    params.startScaleY = 0xcccc;
    params.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    FieldEffect_Spawn(objectEvent->x.fixed, objectEvent->y.fixed + 0x20000,
                      objectEvent->z.fixed, 0,
                      ((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999, phase,
                      EFFECT_USE_SPIN | EFFECT_USE_START_SCALE |
                          EFFECT_SCRIPT_MEDIUM,
                      &params);
  }
}

/// Lamakan Desert, coord event 22, stepping near a hidden pit: the leader looks
/// at the nearest pit, is pulled backwards into it and a battle with an Ant
/// Lion begins.
static void LamakanDesert_FallIntoPit(void) {
  struct ObjectEvent *target;
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *record;
  s32 id;
  s32 best;
  s32 min;
  s32 dx;
  s32 dz;

  target = ObjectEvent_Get(gGameState.selectedLocalId);
  best = FIRST_PIT;
  GameFlag_Set(FLAG_DESERT_EVENT_RUNNING);
  min = 0x100000;
  // Select the closest live pit using Manhattan distance in whole pixels.
  for (id = FIRST_PIT; id <= LAST_PIT; id++) {
    struct ObjectEvent *other = ObjectEvent_Get(id);

    if (other != 0) {
      dx = (target->x.fixed - other->x.fixed) / 0x10000;
      dz = (target->z.fixed - other->z.fixed) / 0x10000;
      {
        s32 ax = dx;

        if (ax < 0) {
          ax = -ax;
        }
        if (dz < 0) {
          dz = -dz;
        }
        if (ax + dz < min) {
          best = id;
          min = ax + dz;
        }
      }
    }
  }
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  // Face the pit, then reverse the walking direction while being pulled in.
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, best, 0);
  Engine_ScriptWait(20);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ScriptWait(60);
  ObjectEvent_SetAttachedEffect(
      LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  record = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  objectEvent->facing = (record->facing + 0x8000) & -0x1000;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_LEADER_RUN);
  ObjectEvent_SetActionById(LOCALID_PARTY_LEADER, 24);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x1999, 0xccc);
  record = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
#ifdef UBFIX
  record->update = Leader_SpraySand;
#else
  record->update = (void (*)(union FieldObject *))Leader_SpraySand;
#endif
  record = ObjectEvent_Get(best);
  if (record != 0) {
    Engine_ObjectEventSetDestination(LOCALID_PARTY_LEADER, record->x.part.pixel,
                                     record->z.part.pixel);
  }
  Engine_ScriptWait(60);
  ObjectEvent_ShowEmote(best, EMOTE_IN_FRONT | EMOTE_HEART, 0);
  Engine_ScriptWait(60);
  ObjectEvent_SetAttachedEffect(
      LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  record = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  record->update = NULL;
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 4);
}

/// Lamakan Desert, on a slope of sand (LAMAKAN_DESERT_CELL_SLOPE): the sand
/// carries the leader south 1.5 pixels a frame, and at a ledge the leader hops
/// down 8 pixels.
static void LamakanDesert_SlideDownDune(void) {
  struct ObjectEvent *center;
  s32 id;
  struct ObjectEvent *objectEvent;
  struct FieldPosition probePosition;
  struct EffectOptions params;
  s32 hit;
  s32 phase;
  s32 x;

  HeatGauge_CountStep();
  {
    struct EventWork *work = gEventWork;

    id = gGameState.selectedLocalId;
    center = work->viewCenter;
  }
  objectEvent = ObjectEvent_GetById(id);
  // First check for a ledge immediately below the sliding object event.
  probePosition.x = objectEvent->x.fixed;
  probePosition.y = objectEvent->y.fixed;
  probePosition.z = objectEvent->z.fixed + 0x18000;
  hit = FieldObject_CheckMovementCollision(objectEvent, &probePosition);
  phase = gFrameCount & 4;
  if (phase == 0) {
    params.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    x = objectEvent->x.fixed + (((u32)(Engine_RandomNext() * 12) >> 16) << 16) -
        0x60000;
    FieldEffect_Spawn(x, objectEvent->y.fixed, objectEvent->z.fixed, 0, phase,
                      ((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999 + 0x7ffd,
                      EFFECT_USE_SPIN, &params);
  }
  if (hit < 0) {
    // Let the fall complete before returning to ordinary field movement.
    ObjectEvent_SetAttachedEffect(id, ATTACHED_EFFECT_IN_FRONT |
                                          ATTACHED_EFFECT_ONCE);
    Engine_ObjectSetPosition(objectEvent, objectEvent->x.fixed,
                             objectEvent->y.fixed,
                             objectEvent->z.fixed + 0x80000);
    ObjectEvent_SetMode(objectEvent, ANIMATION_LEADER_LEAP);
    FieldObject_WaitForMove(objectEvent);
    do {
      Engine_TaskWait(1);
    } while (objectEvent->y.fixed != objectEvent->terrainHeight);
    ObjectEvent_SetMode(objectEvent, ANIMATION_LEADER_SPRING);
    Engine_TaskWait(3);
    return;
  }
  // Probe ahead and on both diagonals before advancing object event and camera.
  probePosition.x = objectEvent->x.fixed;
  probePosition.y = objectEvent->y.fixed;
  probePosition.z = objectEvent->z.fixed + 0x80000;
  hit = FieldObject_CheckMovementCollision(objectEvent, &probePosition);
  if (hit > 0) {
    return;
  }
  probePosition.x = objectEvent->x.fixed + 0x5b333;
  probePosition.y = objectEvent->y.fixed;
  probePosition.z = objectEvent->z.fixed + 0x5b333;
  hit = FieldObject_CheckMovementCollision(objectEvent, &probePosition);
  if (hit > 0) {
    return;
  }
  probePosition.x = objectEvent->x.fixed - 0x5b333;
  probePosition.y = objectEvent->y.fixed;
  probePosition.z = objectEvent->z.fixed + 0x5b333;
  hit = FieldObject_CheckMovementCollision(objectEvent, &probePosition);
  if (hit > 0) {
    return;
  }
  center->z.fixed += 0x18000;
  objectEvent->z.fixed += 0x18000;
}

/// Lamakan Desert, coord event 40 in LamakanDesertManticore, until the
/// Manticore is beaten: the leader stops and backs away as the Manticore rises
/// out of the sand.
static void LamakanDesert_MeetManticore(void) {
  s32 paused;
  struct ObjectEvent *record;
  struct FieldPosition backwardOffset;
  s32 holdFrames;
  u16 *revealFrames;
  u8 *eventBytes;

  eventBytes = (u8 *)gEventWork;
  HeatGauge_CountStep();
  if (GameFlag_IsSet(FLAG_LAMAKAN_MANTICORE_BEATEN) == 0) {
    paused = GameFlag_IsSet(FLAG_DESERT_EVENT_RUNNING);
    if (paused == 0) {
      GameFlag_Set(FLAG_DESERT_EVENT_RUNNING);
      // Suspend heat accumulation while the leader notices the Manticore.
      HeatGauge_SetHeld(1);
      // A running Reveal's countdown is refilled to 600 frames as the
      // Manticore rises and again once it has.
      revealFrames = (u16 *)&((struct FieldWork *)eventBytes)->revealFrames;
      holdFrames = 600;
      *revealFrames = holdFrames;
      record = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      record->velocityX = paused;
      record = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      record->velocityZ = paused;
      record = ObjectEvent_Get(LOCALID_PARTY_LEADER);
      record->targetX = OBJ_EVENT_NO_TARGET;
      record = ObjectEvent_Get(LOCALID_PARTY_LEADER);
      record->targetZ = OBJ_EVENT_NO_TARGET;
      Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
      ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, MANTICORE, 0);
      Engine_ScriptWait(40);
      Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
      ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER,
                                    ATTACHED_EFFECT_IN_FRONT |
                                        ATTACHED_EFFECT_ONCE);
      Engine_ScriptWait(40);
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
          ~OBJ_EVENT_FACES_MOTION;
      backwardOffset.x = paused;
      // Step backward without turning away from the emerging Manticore.
      backwardOffset.y = paused;
      backwardOffset.z = paused;
      record = ObjectEvent_Get(LOCALID_PARTY_LEADER);
      FieldVector_AddPolarOffset(-0x100000, record->facing, &backwardOffset);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x20000, 0x10000);
      Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_WALK);
      ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER,
                                       backwardOffset.x / 0x10000,
                                       backwardOffset.z / 0x10000);
      Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
      Engine_ScriptWait(2);
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags |=
          OBJ_EVENT_FACES_MOTION;
      Engine_ScriptWait(30);
      Script_PlayCue(SE_148);
      Engine_ObjectEventRunRepeatedMotion(MANTICORE, 2);
      Engine_ScriptWait(20);
      ObjectEvent_SetSpeed(MANTICORE, 0x28000, 0x14000);
      ObjectEvent_WalkToAndWait(MANTICORE, 168, 104);
      ObjectEvent_SetSpeed(MANTICORE, 0x8000, 0x4000);
      ObjectEvent_WalkToAndWait(MANTICORE, 168, 92);
      *revealFrames = holdFrames;
      HeatGauge_SetHeld(0);
    }
  }
}

/// Entering Lamakan Desert: the heat gauge (600 steps) starts on all but
/// LamakanDesertCave, the patches Reveal uncovers are saved to the spare cells
/// at 64, 126 or 64, 124, and the secrets are hidden.
s32 LamakanDesert_Initialize(s32 a0, s32 a1) {
  s32 v5;

  gGameState.hazardLimit = 600;
  gGameState.hazardMode = HAZARD_COUNT_ON_CELLS;
  gGameState.hazardDamage = HAZARD_DAMAGE_PERCENT | 25;
  if (gGameState.mapId == (s32)&MapId_LamakanDesertCave) {
  } else {
    gEventWork->startTransition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    HeatGauge_Start();
    FieldScheduler_AddCallback(HeatGauge_RaiseQuarterTriggers,
                               TASK_PRIORITY_MAP);
    if (gGameState.mapId == (s32)&MapId_LamakanDesertEast) {
      MapGrid_CopyCells(22, 7, 4, 2, 64, 126);
      MapGrid_CopyCells(8, 10, 4, 2, 68, 126);
      MapGrid_CopyCells(23, 21, 4, 2, 72, 126);
      MapGrid_CopyCells(16, 42, 4, 2, 76, 126);
      MapGrid_CopyCells(36, 44, 4, 2, 80, 126);
      MapGrid_CopyCells(14, 55, 4, 2, 84, 126);
    } else if (gGameState.mapId == (s32)&MapId_LamakanDesertSmog) {
      MapGrid_CopyCells(42, 5, 4, 2, 64, 126);
      MapGrid_CopyCells(20, 11, 4, 2, 68, 126);
      MapGrid_CopyCells(14, 12, 4, 2, 72, 126);
      MapGrid_CopyCells(56, 18, 4, 2, 76, 126);
      MapGrid_CopyCells(7, 22, 4, 2, 80, 126);
      MapGrid_CopyCells(44, 23, 4, 2, 84, 126);
      MapGrid_CopyCells(38, 24, 4, 2, 88, 126);
      MapGrid_CopyCells(26, 28, 4, 2, 92, 126);
      MapGrid_CopyCells(17, 35, 4, 2, 96, 126);
      MapGrid_CopyCells(50, 36, 4, 2, 100, 126);
      MapGrid_CopyCells(34, 43, 4, 2, 104, 126);
      MapGrid_CopyCells(6, 46, 4, 2, 108, 126);
      MapGrid_CopyCells(27, 55, 4, 2, 112, 126);
      MapGrid_CopyCells(43, 56, 4, 2, 116, 126);
    } else if (gGameState.mapId == (s32)&MapId_LamakanDesertManticore) {
      v5 = 124;
      FieldEffect_SetQueuedSoundAndPlay(SE_169);
      MapGrid_CopyCells(8, 14, 4, 4, 64, v5);
      MapGrid_CopyCells(6, 18, 4, 4, 68, v5);
      MapGrid_CopyCells(10, 21, 4, 4, 72, v5);
    }
    Reveal_HideSecrets();
  }
  return 0;
}

/// Unused: the Manticore walks back to its spot (168, 96) and settles. Nothing
/// calls it.
static UNUSED void Manticore_UnusedReturn(void) {
  ObjectEvent_SetSpeed(MANTICORE, 0x8000, 0x4000);
  Engine_ObjectEventSetAnimation(MANTICORE, 1);
  ObjectEvent_WalkToAndWait(MANTICORE, 168, 96);
  Engine_ObjectEventSetAnimation(MANTICORE, 2);
}

/// Every frame: draws the heat gauge down the screen's left edge, 8 pixels
/// further out while held. It slides off to the left while
/// FLAG_FIELD_HANDLING_REQUESTS is set, its bar shrinks as the heat
/// grows, and its colors turn redder (flickering greener past 60 of 120).
static void HeatGauge_Draw(void) {
  u8 *buffer;
  s32 tile;
  s16 *colors;
  s16 *color;
  u8 *display;
  u32 *column;
  u32 *word;
  u8 *source;
  u8 *destination;
  struct MenuSprite *sprite;
  s32 counter;
  u32 i;
  u32 fill;
  u32 *palette;
  u32 red;
  u32 green;
  u32 blue;
  s32 level;
  u32 attributes;
  s32 x;
  u32 offset;

  tile = gVramBlockCache[(*(s16 *)&gHeatGauge_VramSlot)].offset >> 5;
  if ((*(s16 *)&gHeatGauge_Held) != 0) {
    gHeatGaugeSlide = 2;
  } else if (Engine_GameFlagIsSet(FLAG_FIELD_HANDLING_REQUESTS) != 0) {
    if (gHeatGaugeSlide > 0) {
      gHeatGaugeSlide--;
    }
  } else if (gHeatGaugeSlide <= 1) {
    if (++gHeatGaugeSlide == 1) {
      Dma_Set(gHeatGauge_Palette, (void *)0x050003c0, 0x80000010, REG_DMA3);
    }
  }
  counter = gHeatGaugeSlide;
  if (counter == 0) {
    VramCache_ReleaseVram((*(s16 *)&gHeatGauge_VramSlot));
    return;
  }
  display = *Ram_DisplayWork;
  if (display != NULL) {
    s16 *line = (s16 *)(display + display[0x539] * 644);
    s32 shift = (s16)(counter << 3);

    line = (s16 *)((u8 *)line + 38);
    for (i = 0; i < 144; i++) {
      *line = shift;
      line += 2;
    }
  }

  // Tint the six changing palette entries before composing the gauge tiles.
  buffer = MemAlloc(0x900);
  Dma_Set(gHeatGauge_Palette, buffer, 0x80000010, REG_DMA3);
  colors = (s16 *)(buffer + 12);
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
      green = green + (level << 6) / 120 - 32;
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
    *color++ = (blue << 10) | (green << 5) | red;
  }
  palette = (u32 *)0x050003cc;
  word = (u32 *)(buffer + 12);
  IoWriteQueue_Store32((u32)palette++, *word++);
  IoWriteQueue_Store32((u32)palette++, *word++);
  IoWriteQueue_Store32((u32)palette++, *word++);

  gHeatGaugeLevel = gGameState.hazardSteps * 120 / gGameState.hazardLimit;
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

  // Fill the remaining bar and overlay its nontransparent frame pixels.
  Dma_Set(gHeatGauge_TileBuffer, buffer, 0x84000240, REG_DMA3);
  if (gHeatGaugeFlash <= 118) {
    column = (u32 *)(buffer + 80);
    fill = 0xeeeeeeee;
    for (i = 12; i < 128 - gHeatGaugeLevel; i++) {
      column[8] = fill;
      *column++ = fill;
      if ((i & 7) == 7) {
        column += 8;
      }
    }
    column[0] = ((u32 *)buffer)[0];
    column[8] = ((u32 *)buffer)[8];
  }
  destination = buffer;
  source = buffer + 0x480;
  for (i = 0; i < 0x480; i++) {
    u8 value = *source++;

    if (value != 0) {
      *destination = value;
    }
    destination++;
  }
  Engine_VramLoad((*(s16 *)&gHeatGauge_VramSlot), 0x480, buffer);

  // Submit four 16x32 sprites down the screen's left edge and the 16x16 cap
  // at the gauge's foot; x counts down the screen, since attr01 puts it in
  // attr0's row.
  attributes = 0x80008000;
  sprite = gHeatGauge_Sprites;
  for (i = 0, offset = 0, x = 8; i < 5; i++) {
    u32 y = (gHeatGaugeSlide * 8 - 16) & 0x1ff;

    if (i == 4) {
      attributes = 0x40000000;
    }
    sprite->next = NULL;
    sprite->oam.word.attr01 = (y << 16) | x | attributes;
    sprite->oam.word.attr23 = 0xe400 | tile;
    Runtime_PushSlotEntry(((u8 *)gHeatGauge_Sprites + offset), 255);
    sprite++;
    tile += 8;
    offset += 12;
    x += 32;
  }
  MemFree(buffer);
}

/// Loads the gauge's tiles, claims VRAM for them and starts drawing it.
void HeatGauge_Start(void) {
  s32 slot;
  u16 *slotAddress;

  FieldResource_DecodeType01(gHeatGauge_Tiles, gHeatGauge_TileBuffer);
  slotAddress = &gHeatGauge_VramSlot;
  slot = Resource_FindFreeEntry();
  *slotAddress = slot;
  Engine_VramLoad((s16)slot, 0x480, 0);
  gHeatGaugeSlide = 0;
  gHeatGauge_Held = 0;
  FieldScheduler_AddCallback(HeatGauge_Draw, 0xc76);
}

/// While held (1), the gauge stays on screen.
static void HeatGauge_SetHeld(u16 value) { gHeatGauge_Held = value; }

const s32 gShortEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

const s32 gMediumEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

const s32 gLongEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

// clang-format off
/// Each map's oases, the middle of each as x, z pairs.
const s32 gLamakanDesertEast_Oases[] = {
    PIXELS(384), PIXELS(128),
    PIXELS(288), PIXELS(688),
    PIXELS(608), PIXELS(720),
};

const s32 gLamakanDesertSmog_Oases[] = {
    PIXELS(704), PIXELS(96),
    PIXELS(448), PIXELS(464),
    PIXELS(832), PIXELS(592),
    PIXELS(128), PIXELS(752),
    PIXELS(464), PIXELS(896),
};

const s32 gLamakanDesertOasesOther[] = {
    PIXELS(192), PIXELS(224),
    PIXELS(192), PIXELS(352),
};

const s32 *gEffectScripts[] = {
    gShortEffectScript,
    gMediumEffectScript,
    gLongEffectScript,
};

struct MapWarpDest gLamakanDesertWarpDestsOther[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, 0, 0, 0, 0, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gLamakanDesertEastWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { LAMAKAN_DESERT_EAST_FROM_WORLD_MAP, CONDITION_ALWAYS, 984, 0, 952, FACING_WEST, 0, 0, 0, 1016, 1016, 0 },
    { LAMAKAN_DESERT_EAST_FROM_LAMAKAN_DESERT_SMOG, CONDITION_ALWAYS, 400, 0, 24, FACING_SOUTH, 0, 0, 0, 1016, 1016, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gLamakanDesertSmogWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { LAMAKAN_DESERT_SMOG_FROM_LAMAKAN_DESERT_EAST, CONDITION_ALWAYS, 912, 0, 1000, FACING_NORTH, 0, 0, 0, 1016, 1016, 0 },
    { LAMAKAN_DESERT_SMOG_FROM_LAMAKAN_DESERT_MANTICORE, CONDITION_ALWAYS, 24, 0, 584, FACING_EAST, 0, 0, 0, 1016, 1016, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gLamakanDesertManticoreWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_SMOG, CONDITION_ALWAYS, 1511, 0, 104, FACING_WEST, 0, 0, 8, 1528, 504, 0 },
    { LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_EAST, CONDITION_ALWAYS, 727, 0, 104, FACING_SOUTH, 0, 0, 8, 1528, 504, 0 },
    { LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_SOUTH, CONDITION_ALWAYS, 599, 0, 296, FACING_SOUTH, 0, 0, 8, 1528, 504, 0 },
    { LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_WEST, CONDITION_ALWAYS, 167, 0, 120, FACING_SOUTH, 0, 0, 8, 1528, 504, 0 },
    { LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE, CONDITION_ALWAYS, 167, 0, 136, FACING_NORTH, 0, 0, 8, 1528, 504, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gLamakanDesertCaveWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 424, 0, 824, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_SOUTH, CONDITION_ALWAYS, 424, 0, 968, FACING_NORTH, 0, 304, 560, 624, 1024, 0 },
    { LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_EAST, CONDITION_ALWAYS, 552, 0, 776, FACING_NORTH, 0, 304, 560, 624, 1024, 0 },
    { LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_WEST, CONDITION_ALWAYS, 440, 0, 312, FACING_NORTH, 0, 32, 16, 496, 320, 0 },
    { LAMAKAN_DESERT_CAVE_FROM_WORLD_MAP, CONDITION_ALWAYS, 136, 0, 296, FACING_NORTH, 0, 32, 16, 496, 320, 0 },
    { MAP_TABLE_END },
};

u32 gLamakanDesertWarps[] = {
    MAP_WARPS(LamakanDesertEast),
    MAP_WARP(LAMAKAN_DESERT_EAST_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LAMAKAN_DESERT_EAST),
    MAP_WARP(LAMAKAN_DESERT_EAST_TO_LAMAKAN_DESERT_SMOG, LamakanDesertSmog, LAMAKAN_DESERT_SMOG_FROM_LAMAKAN_DESERT_EAST),
    MAP_WARPS(LamakanDesertSmog),
    MAP_WARP(LAMAKAN_DESERT_SMOG_TO_LAMAKAN_DESERT_EAST, LamakanDesertEast, LAMAKAN_DESERT_EAST_FROM_LAMAKAN_DESERT_SMOG),
    MAP_WARP(LAMAKAN_DESERT_SMOG_TO_LAMAKAN_DESERT_MANTICORE, LamakanDesertManticore, LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_SMOG),
    MAP_WARPS(LamakanDesertManticore),
    MAP_WARP(LAMAKAN_DESERT_MANTICORE_TO_LAMAKAN_DESERT_SMOG, LamakanDesertSmog, LAMAKAN_DESERT_SMOG_FROM_LAMAKAN_DESERT_MANTICORE),
    MAP_WARP(LAMAKAN_DESERT_MANTICORE_TO_LAMAKAN_DESERT_CAVE_EAST, LamakanDesertCave, LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_EAST),
    MAP_WARP(LAMAKAN_DESERT_MANTICORE_TO_LAMAKAN_DESERT_CAVE_SOUTH, LamakanDesertCave, LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_SOUTH),
    MAP_WARP(LAMAKAN_DESERT_MANTICORE_TO_LAMAKAN_DESERT_CAVE_WEST, LamakanDesertCave, LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_WEST),
    MAP_WARPS(LamakanDesertCave),
    MAP_WARP(LAMAKAN_DESERT_CAVE_TO_LAMAKAN_DESERT_MANTICORE_EAST, LamakanDesertManticore, LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_EAST),
    MAP_WARP(LAMAKAN_DESERT_CAVE_TO_LAMAKAN_DESERT_MANTICORE_SOUTH, LamakanDesertManticore, LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_SOUTH),
    MAP_WARP(LAMAKAN_DESERT_CAVE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LAMAKAN_DESERT_CAVE),
    MAP_WARP(LAMAKAN_DESERT_CAVE_TO_LAMAKAN_DESERT_MANTICORE_WEST, LamakanDesertManticore, LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_WEST),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gLamakanDesertObjectEventTemplatesOther[] = {
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gLamakanDesertEastObjectEventTemplates[] = {
    { 0xc3, FLAG_LAMAKAN_MANTICORE_BEATEN, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xc1, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(400), 0, PIXELS(364), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gLamakanDesertSmogObjectEventTemplates[] = {
    { 0xc3, FLAG_LAMAKAN_MANTICORE_BEATEN, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xc1, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(352), 0, PIXELS(204), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xc1, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(736), 0, PIXELS(396), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xc1, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(144), 0, PIXELS(380), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xc1, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(576), 0, PIXELS(716), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { JUPITER_DJINNI, FLAG_DJINNI(DJINNI_SMOG), MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gLamakanDesertManticoreObjectEventTemplates[] = {
    { 0xc3, FLAG_LAMAKAN_MANTICORE_BEATEN, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xc1, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(128), 0, PIXELS(316), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { MAP_TABLE_END },
};

/// The four maps' one map event table. Its warp rows keep their numbers: each
/// takes the warp of that number in whichever map the party is in, which is a
/// different way out in each (LAMAKAN_DESERT_EAST_TO_WORLD_MAP,
/// LAMAKAN_DESERT_SMOG_TO_LAMAKAN_DESERT_EAST and the others).
struct MapEvent gLamakanDesertEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, 1 },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, 2 },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, 3 },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, 4 },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, 1 },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, 2 },
    { MAP_EVENT_WARP, 7, CONDITION_ALWAYS, 3 },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, 4 },
    { MAP_EVENT_COORD, 9, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_LeaveForTheWorldMap) },
    { MAP_EVENT_COORD, LAMAKAN_DESERT_CELL_OASIS, FLAG_DESERT_EVENT_RUNNING, EVENT_SCRIPT(LamakanDesert_BatheInOasis) },
    { MAP_EVENT_COORD, LAMAKAN_DESERT_CELL_SAND, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { MAP_EVENT_COORD, LAMAKAN_DESERT_CELL_PIT, FLAG_DESERT_EVENT_RUNNING, EVENT_SCRIPT(LamakanDesert_FallIntoPit) },
    { MAP_EVENT_COORD, LAMAKAN_DESERT_CELL_SLOPE, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_SlideDownDune) },
    { MAP_EVENT_COORD, 40, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_MeetManticore) },
    { MAP_EVENT_TALK, MANTICORE, FLAG_LAMAKAN_MANTICORE_BEATEN, EVENT_SCRIPT(LamakanDesert_FightManticore) },
    { MAP_EVENT_TALK, SMOG, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_TalkToSmog) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_LAMAKAN_DESERT_VULCAN_AXE, FIND_ITEM(ITEM_VULCAN_AXE) },
    { BG_EVENT(BG_EVENT_UNNAMED), 101, FLAG_LAMAKAN_DESERT_FIRST_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { BG_EVENT(BG_EVENT_CHEST), 102, FLAG_LAMAKAN_DESERT_LUCKY_PEPPER, FIND_ITEM(ITEM_LUCKY_PEPPER) },
    { BG_EVENT(BG_EVENT_UNNAMED), 103, FLAG_LAMAKAN_DESERT_SECOND_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { BG_EVENT(BG_EVENT_CHEST), 105, FLAG_LAMAKAN_DESERT_777_COINS, FIND_COINS(777) },
    { BG_EVENT(BG_EVENT_CHEST), 106, FLAG_LAMAKAN_DESERT_POTION, FIND_ITEM(ITEM_POTION) },
    { BG_EVENT(BG_EVENT_CHEST), 107, FLAG_LAMAKAN_DESERT_MINT, FIND_ITEM(ITEM_MINT) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Reveal_ShowSecrets) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Reveal_HideSecrets) },
    { MAP_EVENT_RAISED, LAMAKAN_DESERT_RAISED_HEAT_QUARTER, FLAG_SECRETS_REVEALED, EVENT_SCRIPT(LamakanDesert_ComplainAboutHeat) },
    { MAP_EVENT_RAISED, LAMAKAN_DESERT_RAISED_HEAT_HALF, FLAG_SECRETS_REVEALED, EVENT_SCRIPT(LamakanDesert_ComplainAboutHeat) },
    { MAP_EVENT_RAISED, LAMAKAN_DESERT_RAISED_HEAT_THREE_QUARTERS, FLAG_SECRETS_REVEALED, EVENT_SCRIPT(LamakanDesert_ComplainAboutHeat) },
    { MAP_EVENT_COORD, 100, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { MAP_EVENT_COORD, 101, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { MAP_EVENT_COORD, 102, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { MAP_EVENT_COORD, 103, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { MAP_EVENT_COORD, 104, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { MAP_EVENT_COORD, 105, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { MAP_EVENT_COORD, 106, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { MAP_EVENT_COORD, 107, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { MAP_EVENTS_END },
};
// clang-format on

/// The gauge: how full it is (0 to 120), how far it has slid onto the
/// screen, and a flash as it fills.
s16 gHeatGaugeLevel = 0;

s16 gHeatGaugeSlide = 0;

s16 gHeatGaugeFlash = 0;
