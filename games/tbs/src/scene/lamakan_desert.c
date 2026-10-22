/// @file
/// Lamakan Desert (SceneId_LamakanDesertEast to SceneId_LamakanDesertCave), the
/// "evil desert" in which Xian's people say Lama Temple lies. All four maps are
/// Lamakan Desert in the game's area names, and they share one event table. The
/// monks of Lama Temple warn the party before it sets out: the "evil desert" is
/// hotter than the sun, the heat at its maximum does tremendous damage, and
/// Reveal finds the oases.
///
/// The heat is a count of steps on hazard ground (gGameState.hazardSteps):
/// LamakanDesert_Initialize sets a limit of 600 on every map, at which the
/// party loses a quarter of its HP, and on all but LamakanDesertCave starts the
/// heat gauge, a bar at the top of the screen (HeatGauge_Draw). Each moving
/// frame on the sand adds a step (HeatGauge_CountStep, cell codes 21 and 100 to
/// 107). As the gauge passes a quarter, a half and three quarters,
/// HeatGauge_RaiseQuarterTriggers sets FLAG_HEAT_QUARTER to
/// FLAG_HEAT_THREE_QUARTERS and raises LAMAKAN_DESERT_RAISED_HEAT_QUARTER to
/// LAMAKAN_DESERT_RAISED_HEAT_THREE_QUARTERS, and Garet, Ivan or Mia complains
/// of the heat (LamakanDesert_ComplainAboutHeat). Touching cell code 20, the
/// leader collapses at the map's nearest recovery point and gets up once the
/// heat has drained (LamakanDesert_CollapseFromHeat), and a running Reveal
/// ends.
///
/// The desert hides its secrets under the sand: the pits of the Ant Lions, the
/// Manticore, Smog, a Jupiter Djinni, and the chests and Psynergy Stones. Entry
/// saves the map patches Reveal uncovers and hides all of them under a
/// shimmering heat haze (Reveal_HideSecrets); Reveal brings them back and stops
/// the haze (Reveal_ShowSecrets) until it wears off. Talking to Smog, in
/// LamakanDesertSmog, adds the Djinni to the party.
///
/// Stepping by a pit (cell code 22) pulls the leader into it for a battle with
/// an Ant Lion (LamakanDesert_FallIntoPit), and slopes of sand
/// (LAMAKAN_DESERT_CELL_SLOPE) carry the leader south
/// (LamakanDesert_SlideDownDune).
///
/// In LamakanDesertManticore, until it is beaten
/// (FLAG_LAMAKAN_MANTICORE_BEATEN), the Manticore rises out of the sand as the
/// leader comes near (cell code 40, LamakanDesert_MeetManticore). Talking to it
/// starts its battle, which ends at LamakanDesertManticore's
/// LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE, and arriving there marks it
/// beaten.
///
/// LamakanDesertEast's LAMAKAN_DESERT_EAST_TO_WORLD_MAP and LamakanDesertCave's
/// LAMAKAN_DESERT_CAVE_TO_WORLD_MAP lead out onto the world map;
/// LamakanDesertCave, a cave, has no heat gauge. Cell code 9 takes
/// LAMAKAN_DESERT_CAVE_TO_WORLD_MAP and settles the fate of Master Hsu of Xian,
/// trapped under boulders on his way back from Lama Temple: if the party has
/// not freed him by then, leaving sets FLAG_HSU_RESCUED and
/// FLAG_HSU_RESCUED_WITHOUT_PARTY, and in Xian, Feizhi then thanks Master Hama
/// alone for freeing Hsu, instead of the party and Master Hama
/// (xian_interiors.c).
///
/// Open questions:
///
/// - XXX: the heat gauge's data keeps names that call it sand
///   (gLamakanDesert_SandPalette, _SandVramSlot, _SandWork, _SandTileBuffer
///   and _SandPhase, which holds the gauge in place), defined in
///   graphics/lamakan/sand.s and system/overlay/ovl_3a5.s; a render of the
///   gauge would confirm what the palette and tiles draw before they are
///   renamed there.
/// - XXX: what the leader's animations 5 (pulled into a pit) and 18
///   (collapsed) and object modes 6 and 7 (hopping down a slope's ledge) show;
///   the party's sprite sheets would settle names for them.
/// - XXX: where cell code 20 lies on the desert's maps, and so when the leader
///   collapses from the heat; the maps' collision cells would settle it.

#include "vram_tab.h"
#include "unit/import.h"
#include "unit/scene/lamakan_desert.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "field_obj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
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
#include "scene_ids.h"
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
/// each mark, and the cell code of the sand slopes.
enum LamakanDesertTrigger {
  LAMAKAN_DESERT_RAISED_HEAT_QUARTER = 1,
  LAMAKAN_DESERT_RAISED_HEAT_HALF = 2,
  LAMAKAN_DESERT_RAISED_HEAT_THREE_QUARTERS = 3,
  LAMAKAN_DESERT_CELL_SLOPE = 30 ///< sand that carries the leader south
};

enum {
  /// FLAG_SCENE_TEMP_00: a collapse, a pit or the Manticore is under way.
  FLAG_DESERT_EVENT_RUNNING = FLAG_SCENE_TEMP_00,
  /// FLAG_SCENE_TEMP_01: Reveal shows the pits.
  /// ★ Named from the game: "Reveal" (MsgAbilityName)
  FLAG_SECRETS_REVEALED = FLAG_SCENE_TEMP_01,
  /// The heat gauge has passed a quarter, where the first complaints come.
  /// ★ Named from the game: "This desert is much hotter than I ever imagined"
  /// (MsgMountainNorthAltinMineWestLama)
  FLAG_HEAT_QUARTER = FLAG_PLACE_TEMP_00,
  /// ★ Named from the game: "I can't keep my cool when it's this hot"
  /// (MsgMountainNorthAltinMineWestLama)
  FLAG_HEAT_HALF = FLAG_PLACE_TEMP_01,
  /// ★ Named from the game: "I can't stand another minute of this!"
  /// (MsgMountainNorthAltinMineWestLama)
  FLAG_HEAT_THREE_QUARTERS = FLAG_PLACE_TEMP_02,
  /// Who has complained since the gauge last passed a mark: one per member,
  /// in the order of their actors.
  /// ★ Named from the game: "My feet are burning!"
  /// (MsgMountainNorthAltinMineWestLama)
  FLAG_GARET_COMPLAINED = FLAG_PLACE_TEMP_03,
  /// ★ Named from the game: "Just walking around here saps every ounce of my
  /// strength." (MsgMountainNorthAltinMineWestLama)
  FLAG_IVAN_COMPLAINED = FLAG_PLACE_TEMP_04,
  /// ★ Named from the game: "My clothes are drenched in sweat!"
  /// (MsgMountainNorthAltinMineWestLama)
  FLAG_MIA_COMPLAINED = FLAG_PLACE_TEMP_05
};

/// The actors, numbered in the order of each map's placement table from
/// ACTOR_FIRST_PLACED. LamakanDesertEast to LamakanDesertManticore place the
/// Manticore, then the LamakanDesertEast and LamakanDesertManticore one pit and
/// LamakanDesertSmog all four pits and Smog; LamakanDesertCave places none.
enum {
  MANTICORE = ACTOR_FIRST_PLACED, ///< talking to it starts its battle
  /// The first of the Ant Lions' pits Reveal shows, up to LAST_PIT.
  FIRST_PIT = MANTICORE + 1,
  LAST_PIT = FIRST_PIT + 3,
  SMOG = LAST_PIT + 1 ///< Smog, a Jupiter Djinni
};

/// The desert's two Psynergy Stones, place flags: each is back once the
/// party has left Lamakan Desert.
enum {
  /// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
  FLAG_LAMAKAN_DESERT_FIRST_PSYNERGY_STONE = FLAG_PLACE_TEMP_50,
  /// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
  FLAG_LAMAKAN_DESERT_SECOND_PSYNERGY_STONE = FLAG_PLACE_TEMP_51
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
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
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
  FieldFx_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, phase, phase,
                EFFECT_USE_SPIN | EFFECT_USE_START_SCALE | 1, &params);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Where the party can enter each of the four maps.
const struct SceneEntrance *LamakanDesert_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_LamakanDesertEast) {
    return gLamakanDesertEastEntrances;
  }
  if (scene == (s32)&SceneId_LamakanDesertSmog) {
    return gLamakanDesertSmogEntrances;
  }
  if (scene == (s32)&SceneId_LamakanDesertManticore) {
    return gLamakanDesertManticoreEntrances;
  }
  if (scene == (s32)&SceneId_LamakanDesertCave) {
    return gLamakanDesertCaveEntrances;
  }
  return gLamakanDesertEntrancesOther;
}

const struct SceneRegion *LamakanDesert_GetRegions(void) { return 0; }

/// All four desert maps share the packed exit table.
const u32 *LamakanDesert_GetExits(void) { return gLamakanDesertExits; }

/// The pits, the Manticore and Smog of each map. Arriving at
/// LamakanDesertManticore's LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE, the
/// way back from the Manticore's battle, marks it beaten; in the patched
/// editions it also sets FLAG_REACHED_KALAY (XXX: why is not known).
const struct ScenePlacement *LamakanDesert_GetPlacements(void) {
  if (gGameState.scene == (s32)&SceneId_LamakanDesertManticore) {
    if (gGameState.entrance == LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE) {
      Engine_GameFlagSet(FLAG_LAMAKAN_MANTICORE_BEATEN);
#if TBS_PATCH_3
      Engine_GameFlagSet(FLAG_REACHED_KALAY);
#endif
    }
  }
  if (gGameState.scene == (s32)&SceneId_LamakanDesertEast) {
    return gLamakanDesertEastPlacements;
  }
  if (gGameState.scene == (s32)&SceneId_LamakanDesertSmog) {
    return gLamakanDesertSmogPlacements;
  }
  if (gGameState.scene == (s32)&SceneId_LamakanDesertManticore) {
    return gLamakanDesertManticorePlacements;
  }
  return gLamakanDesertPlacementsOther;
}

/// Lamakan Desert, touching cell code 9: LAMAKAN_DESERT_CAVE_TO_WORLD_MAP. If
/// Master Hsu is still trapped, he is rescued without the party (see the file
/// comment).
static void LamakanDesert_LeaveForTheWorldMap(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_HSU_RESCUED) == 0) {
    if (GameFlag_IsSet(FLAG_HSU_RESCUED_WITHOUT_PARTY) == 0) {
      GameFlag_Set(FLAG_HSU_RESCUED_WITHOUT_PARTY);
      GameFlag_Set(FLAG_HSU_RESCUED);
    }
  }
  Event_PlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(LAMAKAN_DESERT_CAVE_TO_WORLD_MAP);
  Engine_EventEnd();
}

/// The desert's triggers, the same on every map.
const struct SceneEvent *LamakanDesert_GetEvents(void) {
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
  s32 mapObjectId;

  Engine_GameFlagClear(FLAG_SECRETS_REVEALED);
  if (gGameState.scene == (s32)&SceneId_LamakanDesertEast) {
    Map_CopyCells(70, 68, 4, 2, 22, 7);
    Map_CopyCells(70, 68, 4, 2, 8, 10);
    Map_CopyCells(70, 68, 4, 2, 23, 21);
    Map_CopyCellAttributes(70, 68, 4, 1, 23, 23);
    Map_CopyCells(70, 68, 4, 2, 16, 42);
    Map_CopyCells(70, 68, 4, 2, 36, 44);
    Map_CopyCells(70, 68, 4, 2, 14, 55);
  } else if (gGameState.scene == (s32)&SceneId_LamakanDesertSmog) {
    Map_CopyCells(70, 68, 4, 2, 42, 5);
    Map_CopyCells(70, 68, 4, 2, 20, 11);
    Map_CopyCellAttributes(70, 68, 4, 1, 20, 13);
    Map_CopyCells(70, 68, 4, 2, 14, 12);
    Map_CopyCells(70, 68, 4, 2, 56, 18);
    Map_CopyCells(70, 68, 4, 2, 7, 22);
    Map_CopyCellAttributes(70, 68, 4, 1, 7, 24);
    Map_CopyCells(70, 68, 4, 2, 44, 23);
    Map_CopyCellAttributes(70, 68, 4, 1, 44, 25);
    Map_CopyCells(70, 68, 4, 2, 38, 24);
    Map_CopyCells(70, 68, 4, 2, 26, 28);
    Map_CopyCells(70, 68, 4, 2, 17, 35);
    Map_CopyCells(70, 68, 4, 2, 50, 36);
    Map_CopyCells(70, 68, 4, 2, 34, 43);
    Map_CopyCellAttributes(70, 68, 4, 1, 34, 45);
    Map_CopyCells(70, 68, 4, 2, 6, 46);
    Map_CopyCells(70, 68, 4, 2, 27, 55);
    Map_CopyCells(70, 68, 4, 2, 43, 56);
  } else if (gGameState.scene == (s32)&SceneId_LamakanDesertManticore) {
    Engine_MapCopyCellsLayered(69, 99, 4, 2, 8, 16);
    Engine_MapCopyCellsLayered(69, 99, 4, 2, 6, 20);
    Engine_MapCopyCellsLayered(69, 99, 4, 2, 10, 23);
    Map_CopyCellAttributes(69, 99, 4, 2, 8, 14);
    Map_CopyCellAttributes(69, 99, 4, 2, 6, 18);
    Map_CopyCellAttributes(69, 99, 4, 1, 6, 20);
    Engine_MapCopyCellAttributes(69, 99, 4, 2, 10, 21);
    Map_CopyCells(0, 121, 5, 7, 8, 32);
    Map_CopyCells(0, 121, 5, 7, 43, 32);
    Map_CopyCells(6, 120, 3, 1, 9, 5);
    Map_CopyCells(9, 120, 3, 1, 44, 5);
    Map_CopyCellAttributes(9, 0, 3, 3, 9, 6);
  }
  Engine_ActorSetPosition(MANTICORE, 0, 0);
  Engine_ActorSetPosition(FIRST_PIT, 0, 0);
  Engine_ActorSetPosition(FIRST_PIT + 1, 0, 0);
  Engine_ActorSetPosition(FIRST_PIT + 2, 0, 0);
  Engine_ActorSetPosition(LAST_PIT, 0, 0);
  Engine_ActorSetPosition(SMOG, 0, 0);
  // Hide the searchable map objects independently of the actor placements.
  mapObjectId = 100;
  do {
    Engine_MapObjectSetPosition(mapObjectId, 0, 0);
    mapObjectId = mapObjectId + 1;
  } while (mapObjectId <= 107);
  if (gGameState.scene != (s32)&SceneId_LamakanDesertCave) {
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
  if (gGameState.scene == (s32)&SceneId_LamakanDesertEast) {
    Map_CopyCells(64, 126, 4, 2, 22, 7);
    Map_CopyCells(68, 126, 4, 2, 8, 10);
    Map_CopyCells(72, 126, 4, 2, 23, 21);
    Map_CopyCellAttributes(72, 126, 4, 2, 23, 22);
    Map_CopyCells(76, 126, 4, 2, 16, 42);
    Map_CopyCells(80, 126, 4, 2, 36, 44);
    Map_CopyCells(84, 126, 4, 2, 14, 55);
    Actor_SetPosition(FIRST_PIT, PIXELS(400), PIXELS(364));
  } else if (gGameState.scene == (s32)&SceneId_LamakanDesertSmog) {
    Map_CopyCells(64, 126, 4, 2, 42, 5);
    Map_CopyCells(68, 126, 4, 2, 20, 11);
    Map_CopyCellAttributes(68, 126, 4, 2, 20, 12);
    Map_CopyCells(72, 126, 4, 2, 14, 12);
    Map_CopyCells(76, 126, 4, 2, 56, 18);
    Map_CopyCells(80, 126, 4, 2, 7, 22);
    Map_CopyCellAttributes(80, 126, 4, 2, 7, 23);
    Map_CopyCells(84, 126, 4, 2, 44, 23);
    Map_CopyCellAttributes(84, 126, 4, 2, 44, 24);
    Map_CopyCells(88, 126, 4, 2, 38, 24);
    Map_CopyCells(92, 126, 4, 2, 26, 28);
    Map_CopyCells(96, 126, 4, 2, 17, 35);
    Map_CopyCells(100, 126, 4, 2, 50, 36);
    Map_CopyCells(104, 126, 4, 2, 34, 43);
    Map_CopyCellAttributes(104, 126, 4, 2, 34, 44);
    Map_CopyCells(108, 126, 4, 2, 6, 46);
    Map_CopyCells(112, 126, 4, 2, 27, 55);
    Map_CopyCells(116, 126, 4, 2, 43, 56);
    Actor_SetPosition(FIRST_PIT, PIXELS(352), PIXELS(204));
    Actor_SetPosition(FIRST_PIT + 1, PIXELS(736), PIXELS(396));
    Actor_SetPosition(FIRST_PIT + 2, PIXELS(144), PIXELS(380));
    Actor_SetPosition(LAST_PIT, PIXELS(576), PIXELS(716));
    Actor_SetPosition(SMOG, PIXELS(648), PIXELS(408));
  } else if (gGameState.scene == (s32)&SceneId_LamakanDesertManticore) {
    Map_CopyCells(64, 124, 4, 4, 8, 14);
    Map_CopyCells(68, 124, 4, 4, 6, 18);
    Map_CopyCellAttributes(68, 124, 4, 1, 6, 20);
    Map_CopyCells(72, 124, 4, 4, 10, 21);
    Map_CopyCells(10, 121, 5, 7, 8, 32);
    Map_CopyCells(5, 121, 5, 7, 43, 32);
    Map_CopyCells(0, 120, 3, 1, 9, 5);
    Map_CopyCells(3, 120, 3, 1, 44, 5);
    Actor_SetPosition(MANTICORE, PIXELS(168), PIXELS(92));
    Actor_SetPosition(FIRST_PIT, PIXELS(128), PIXELS(316));
    Map_CopyCellAttributes(6, 0, 3, 3, 9, 6);
    if (Engine_GameFlagIsSet(FLAG_LAMAKAN_MANTICORE_BEATEN) == 0) {
      Map_CopyCells(0, 119, 3, 1, 9, 5);
    }
  }
  for (i = 100; i <= 107; i++) {
    MapObject_SetPosition(i, -1, -1);
  }
  Marker_MoveLeaderAway();
  if (gGameState.scene != (s32)&SceneId_LamakanDesertCave) {
    DisplayScroll_DisarmHBlankDma();
  }
}

/// The party fights the Manticore. In LamakanDesertManticore, talking to the
/// Manticore starts its battle, which returns the party to
/// LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE.
static void LamakanDesert_FightManticore(void) {
  Engine_ActorRunRepeatedMotion(MANTICORE, 2);
  Event_SetNextDestination((s32)&SceneId_LamakanDesertManticore,
                           LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE);
  Battle_SetStart(BATTLE_START_SCENE);
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
  Engine_EventBegin();
  // Three lines a mark, one for each member. They come right after Alpine
  // Crossing's signpost (`North: Altin Mine West: Lama Temple`) and have no
  // names of their own, so they count from a Fuchin Temple message two before
  // it.
  Engine_EventSetMessage((((quarter << 1) + quarter) + member) +
                         MSGNO(MsgFuchinTempleIDoNotThinkMasterHama));
  Engine_EventShowMessage((member + 1), 0);
  Engine_EventEnd();
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

/// A recovery point: where a collapsed leader comes to.
struct SafePoint {
  s32 x;
  s32 z;
};

/// Lamakan Desert, touching cell code 20: the leader drops at the nearest
/// recovery point of the map, lies collapsed while the gauge drains 5 steps a
/// frame, then gets up.
static void LamakanDesert_CollapseFromHeat(void) {
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
  struct FieldActor *leader;
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
  if (gGameState.scene == (s32)&SceneId_LamakanDesertEast) {
    count = 3;
    points = gLamakanDesertEast_RecoveryPoints;
  } else if (gGameState.scene == (s32)&SceneId_LamakanDesertSmog) {
    count = 5;
    points = gLamakanDesertSmog_RecoveryPoints;
  } else {
    count = 2;
    points = gLamakanDesertRecoveryPointsOther;
  }
  left = count;
  // The selected table has 2, 3 or 5 points. The square root fits in 16 bits,
  // below best, so the first iteration always sets nearest; ties use the last.
  point = points;
  if (count != 0) {
    offset = 0;
    do {
      distance = LamakanDesert_DistanceInPixels(
          &Actor_Get(ACTOR_PARTY_LEADER)->x.fixed, (s32 *)point);
      if (distance <= best) {
        best = distance;
        nearest = left - count;
      }
      offset += 8;
      point = (const s32 *)((const u8 *)points + offset);
    } while (--count != 0);
  }
  nearest <<= 1;
  // Move the collapsed leader to safety before draining the heat gauge.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
  {
    struct FieldActor *actor = Actor_Get(ACTOR_PARTY_LEADER);
    const struct SafePoint *spot = (const struct SafePoint *)&points[nearest];

    FieldObject_SetPosition(actor, spot->x, 0, spot->z);
  }
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = 0x60000;
  Event_PlayCue(SE_JUMP);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  SceneActor_WaitObjectBelowHeight(leader,
                                   Actor_Get(ACTOR_PARTY_LEADER)->y.fixed);
  Event_PlayCue(SE_241);
  {
    struct FieldActor *actor = Actor_Get(ACTOR_PARTY_LEADER);

    options.type = 214;
    options.startScaleX = 0x8000;
    options.startScaleY = 0xcccc;
    options.targetScaleX = 0x10000;
    options.targetScaleY = 0x13333;
    FieldFx_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, 0, 0,
                  EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                      EFFECT_SCALE_TO_TARGET,
                  &options);
  }
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_HEART, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 18);
  revealFrames = (u16 *)&((struct FieldWork *)work)->revealFrames;
  progress = &gGameState.hazardSteps;
  zero = 0;
  // A running Reveal's countdown stays full while the leader lies collapsed,
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
    struct FieldActor *actor = Actor_Get(ACTOR_PARTY_LEADER);

    puff.type = 214;
    puff.startScaleX = 0x8000;
    puff.startScaleY = 0xcccc;
    puff.targetScaleX = 0x8000;
    puff.targetScaleY = 0x13333;
    FieldFx_Spawn(
        actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, frames, frames,
        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET,
        &puff);
  }
  Event_PlayCue(SE_STOP_SLOT3);
  Event_PlayCue(SE_JUMP);
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = 0x60000;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(10);
  // frames is 0 here, so a running Reveal ends as the leader gets up.
  *(u16 *)&((struct FieldWork *)work)->revealFrames = frames;
  HeatGauge_SetHeld(0);
}

/// Lamakan Desert, walking on sand (cell codes 21 and 100 to 107): each moving
/// frame adds a step of heat and kicks up dust, faster on the run.
static void HeatGauge_CountStep(void) {
  struct FieldActor *actor;
  struct EventWork *event;
  struct EffectOptions params;
  s32 dx;
  s32 z;
  s32 phase;
  actor = Object_GetById(gGameState.selectedActor);
  event = gEventWork;
  if (actor->targetX == ACTOR_NO_TARGET) {
    return;
  }
  gGameState.hazardSteps++;
  if (event->touchedTrigger == LAMAKAN_DESERT_CELL_SLOPE) {
    return;
  }
  if (actor->speed <= 0x10000) {
    if ((gFrameCount & 15) != 0) {
      return;
    }
    params.startScaleX = 0x8000;
    params.startScaleY = 0x8000;
    dx = 0;
    params.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    if (actor->facing != 0 && actor->facing != 0x8000) {
      z = actor->z.fixed;
      dx = 0x20000 - ((((z >> 20) & 1) * 5) << 16);
    } else {
      z = actor->z.fixed;
    }
    FieldFx_Spawn(actor->x.fixed + dx, actor->y.fixed, z, 0, 0, 0,
                  EFFECT_USE_SPIN | EFFECT_USE_START_SCALE | 1, &params);
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
    FieldFx_Spawn(actor->x.fixed, actor->y.fixed + 0x20000, actor->z.fixed, 0,
                  ((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999, phase,
                  EFFECT_USE_SPIN | EFFECT_USE_START_SCALE | 1, &params);
  }
}

/// Lamakan Desert, cell code 22, stepping near a hidden pit: the leader looks
/// at the nearest pit, is pulled backwards into it and a battle with an Ant
/// Lion begins.
static void LamakanDesert_FallIntoPit(void) {
  struct FieldActor *target;
  struct FieldActor *actor;
  struct FieldActor *record;
  s32 id;
  s32 best;
  s32 min;
  s32 dx;
  s32 dz;

  target = Actor_Get(gGameState.selectedActor);
  best = FIRST_PIT;
  GameFlag_Set(FLAG_DESERT_EVENT_RUNNING);
  min = 0x100000;
  // Select the closest live pit using Manhattan distance in whole pixels.
  for (id = FIRST_PIT; id <= LAST_PIT; id++) {
    struct FieldActor *other = Actor_Get(id);

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
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  // Face the pit, then reverse the walking direction while being pulled in.
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, best, 0);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(60);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  record = Object_GetById(ACTOR_PARTY_LEADER);
  actor->facing = (record->facing + 0x8000) & -0x1000;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 5);
  Object_SetActionById(ACTOR_PARTY_LEADER, 24);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1999, 0xccc);
  record = Object_GetById(ACTOR_PARTY_LEADER);
#ifdef UBFIX
  record->update = Leader_SpraySand;
#else
  record->update = (void (*)(union FieldObject *))Leader_SpraySand;
#endif
  record = Actor_Get(best);
  if (record != 0) {
    Engine_ActorSetDestination(ACTOR_PARTY_LEADER, record->x.part.pixel,
                               record->z.part.pixel);
  }
  Engine_EventWait(60);
  Actor_ShowEmote(best, EMOTE_IN_FRONT | EMOTE_HEART, 0);
  Engine_EventWait(60);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  record = Object_GetById(ACTOR_PARTY_LEADER);
  record->update = NULL;
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 4);
}

/// Lamakan Desert, on a slope of sand (LAMAKAN_DESERT_CELL_SLOPE): the sand
/// carries the leader south 1.5 pixels a frame, and at a ledge the leader hops
/// down 8 pixels.
static void LamakanDesert_SlideDownDune(void) {
  struct FieldActor *center;
  s32 id;
  struct FieldActor *actor;
  struct FieldPosition probePosition;
  struct EffectOptions params;
  s32 hit;
  s32 phase;
  s32 x;

  HeatGauge_CountStep();
  {
    struct EventWork *event = gEventWork;

    id = gGameState.selectedActor;
    center = event->viewCenter;
  }
  actor = Object_GetById(id);
  // First check for a ledge immediately below the sliding actor.
  probePosition.x = actor->x.fixed;
  probePosition.y = actor->y.fixed;
  probePosition.z = actor->z.fixed + 0x18000;
  hit = FieldObject_CheckMovementCollision(actor, &probePosition);
  phase = gFrameCount & 4;
  if (phase == 0) {
    params.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    x = actor->x.fixed + (((u32)(Engine_RandomNext() * 12) >> 16) << 16) -
        0x60000;
    FieldFx_Spawn(x, actor->y.fixed, actor->z.fixed, 0, phase,
                  ((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999 + 0x7ffd,
                  EFFECT_USE_SPIN, &params);
  }
  if (hit < 0) {
    // Let the fall complete before returning to ordinary field movement.
    Actor_SetAttachedEffect(id,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed,
                             actor->z.fixed + 0x80000);
    Object_SetMode(actor, 7);
    FieldObject_WaitForMove(actor);
    do {
      Engine_TaskWait(1);
    } while (actor->y.fixed != actor->terrainHeight);
    Object_SetMode(actor, 6);
    Engine_TaskWait(3);
    return;
  }
  // Probe ahead and on both diagonals before advancing actor and camera.
  probePosition.x = actor->x.fixed;
  probePosition.y = actor->y.fixed;
  probePosition.z = actor->z.fixed + 0x80000;
  hit = FieldObject_CheckMovementCollision(actor, &probePosition);
  if (hit > 0) {
    return;
  }
  probePosition.x = actor->x.fixed + 0x5b333;
  probePosition.y = actor->y.fixed;
  probePosition.z = actor->z.fixed + 0x5b333;
  hit = FieldObject_CheckMovementCollision(actor, &probePosition);
  if (hit > 0) {
    return;
  }
  probePosition.x = actor->x.fixed - 0x5b333;
  probePosition.y = actor->y.fixed;
  probePosition.z = actor->z.fixed + 0x5b333;
  hit = FieldObject_CheckMovementCollision(actor, &probePosition);
  if (hit > 0) {
    return;
  }
  center->z.fixed += 0x18000;
  actor->z.fixed += 0x18000;
}

/// Lamakan Desert, cell code 40 in LamakanDesertManticore, until the Manticore
/// is beaten: the leader stops and backs away as the Manticore rises out of the
/// sand.
static void LamakanDesert_MeetManticore(void) {
  s32 paused;
  struct FieldActor *record;
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
      record = Object_GetById(ACTOR_PARTY_LEADER);
      record->velocityX = paused;
      record = Object_GetById(ACTOR_PARTY_LEADER);
      record->velocityZ = paused;
      record = Actor_Get(ACTOR_PARTY_LEADER);
      record->targetX = ACTOR_NO_TARGET;
      record = Actor_Get(ACTOR_PARTY_LEADER);
      record->targetZ = ACTOR_NO_TARGET;
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
      Actor_FaceActor(ACTOR_PARTY_LEADER, MANTICORE, 0);
      Engine_EventWait(40);
      Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Engine_EventWait(40);
      Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
      backwardOffset.x = paused;
      // Step backward without turning away from the emerging Manticore.
      backwardOffset.y = paused;
      backwardOffset.z = paused;
      record = Actor_Get(ACTOR_PARTY_LEADER);
      FieldVector_AddPolarOffset(-0x100000, record->facing, &backwardOffset);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
      Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, backwardOffset.x / 0x10000,
                                 backwardOffset.z / 0x10000);
      Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
      Engine_EventWait(2);
      Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
      Engine_EventWait(30);
      Event_PlayCue(SE_148);
      Engine_ActorRunRepeatedMotion(MANTICORE, 2);
      Engine_EventWait(20);
      Actor_SetSpeed(MANTICORE, 0x28000, 0x14000);
      Actor_WalkToAndWait(MANTICORE, 168, 104);
      Actor_SetSpeed(MANTICORE, 0x8000, 0x4000);
      Actor_WalkToAndWait(MANTICORE, 168, 92);
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
  if (gGameState.scene == (s32)&SceneId_LamakanDesertCave) {
  } else {
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    HeatGauge_Start();
    FieldScheduler_AddCallback(HeatGauge_RaiseQuarterTriggers,
                               TASK_PRIORITY_SCENE);
    if (gGameState.scene == (s32)&SceneId_LamakanDesertEast) {
      Map_CopyCells(22, 7, 4, 2, 64, 126);
      Map_CopyCells(8, 10, 4, 2, 68, 126);
      Map_CopyCells(23, 21, 4, 2, 72, 126);
      Map_CopyCells(16, 42, 4, 2, 76, 126);
      Map_CopyCells(36, 44, 4, 2, 80, 126);
      Map_CopyCells(14, 55, 4, 2, 84, 126);
    } else if (gGameState.scene == (s32)&SceneId_LamakanDesertSmog) {
      Map_CopyCells(42, 5, 4, 2, 64, 126);
      Map_CopyCells(20, 11, 4, 2, 68, 126);
      Map_CopyCells(14, 12, 4, 2, 72, 126);
      Map_CopyCells(56, 18, 4, 2, 76, 126);
      Map_CopyCells(7, 22, 4, 2, 80, 126);
      Map_CopyCells(44, 23, 4, 2, 84, 126);
      Map_CopyCells(38, 24, 4, 2, 88, 126);
      Map_CopyCells(26, 28, 4, 2, 92, 126);
      Map_CopyCells(17, 35, 4, 2, 96, 126);
      Map_CopyCells(50, 36, 4, 2, 100, 126);
      Map_CopyCells(34, 43, 4, 2, 104, 126);
      Map_CopyCells(6, 46, 4, 2, 108, 126);
      Map_CopyCells(27, 55, 4, 2, 112, 126);
      Map_CopyCells(43, 56, 4, 2, 116, 126);
    } else if (gGameState.scene == (s32)&SceneId_LamakanDesertManticore) {
      v5 = 124;
      FieldFx_SetQueuedSoundAndPlay(SE_169);
      Map_CopyCells(8, 14, 4, 4, 64, v5);
      Map_CopyCells(6, 18, 4, 4, 68, v5);
      Map_CopyCells(10, 21, 4, 4, 72, v5);
    }
    Reveal_HideSecrets();
  }
  return 0;
}

/// Unused: the Manticore walks back to its spot (168, 96) and settles. Nothing
/// calls it.
static UNUSED void Manticore_UnusedReturn(void) {
  Actor_SetSpeed(MANTICORE, 0x8000, 0x4000);
  Engine_ActorSetAnimation(MANTICORE, 1);
  Actor_WalkToAndWait(MANTICORE, 168, 96);
  Engine_ActorSetAnimation(MANTICORE, 2);
}

/// Every frame: draws the heat gauge. It sits 8 pixels lower while held, slides
/// away while FLAG_FIELD_HANDLING_REQUESTS is set, its bar shrinks as the heat
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

  tile = gVramBlockCache[(*(s16 *)&gLamakanDesert_SandVramSlot)].offset >> 5;
  if ((*(s16 *)&gLamakanDesert_SandPhase) != 0) {
    gHeatGaugeSlide = 2;
  } else if (Engine_GameFlagIsSet(FLAG_FIELD_HANDLING_REQUESTS) != 0) {
    if (gHeatGaugeSlide > 0) {
      gHeatGaugeSlide--;
    }
  } else if (gHeatGaugeSlide <= 1) {
    if (++gHeatGaugeSlide == 1) {
      Dma_Set(gLamakanDesert_SandPalette, (void *)0x050003c0, 0x80000010,
              REG_DMA3);
    }
  }
  counter = gHeatGaugeSlide;
  if (counter == 0) {
    VramCache_ReleaseVram((*(s16 *)&gLamakanDesert_SandVramSlot));
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
  Dma_Set(gLamakanDesert_SandPalette, buffer, 0x80000010, REG_DMA3);
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
  Dma_Set(gLamakanDesert_SandTileBuffer, buffer, 0x84000240, REG_DMA3);
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
  Engine_VramLoad((*(s16 *)&gLamakanDesert_SandVramSlot), 0x480, buffer);

  // Submit four wide sprites and the narrow cap at the gauge's right end.
  attributes = 0x80008000;
  sprite = gLamakanDesert_SandWork;
  for (i = 0, offset = 0, x = 8; i < 5; i++) {
    u32 y = (gHeatGaugeSlide * 8 - 16) & 0x1ff;

    if (i == 4) {
      attributes = 0x40000000;
    }
    sprite->next = NULL;
    sprite->oam.word.attr01 = (y << 16) | x | attributes;
    sprite->oam.word.attr23 = 0xe400 | tile;
    Runtime_PushSlotEntry(((u8 *)gLamakanDesert_SandWork + offset), 255);
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

  FieldResource_DecodeType01(gLamakanDesert_SandEffectTiles,
                             gLamakanDesert_SandTileBuffer);
  slotAddress = &gLamakanDesert_SandVramSlot;
  slot = Resource_FindFreeEntry();
  *slotAddress = slot;
  Engine_VramLoad((s16)slot, 0x480, 0);
  gHeatGaugeSlide = 0;
  gLamakanDesert_SandPhase = 0;
  FieldScheduler_AddCallback(HeatGauge_Draw, 0xc76);
}

/// While held (1), the gauge stays on screen.
static void HeatGauge_SetHeld(u16 value) { gLamakanDesert_SandPhase = value; }

const s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

const s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

const s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

// clang-format off
/// Each map's recovery points, as x, z pairs.
const s32 gLamakanDesertEast_RecoveryPoints[] = {
    PIXELS(384), PIXELS(128),
    PIXELS(288), PIXELS(688),
    PIXELS(608), PIXELS(720),
};

const s32 gLamakanDesertSmog_RecoveryPoints[] = {
    PIXELS(704), PIXELS(96),
    PIXELS(448), PIXELS(464),
    PIXELS(832), PIXELS(592),
    PIXELS(128), PIXELS(752),
    PIXELS(464), PIXELS(896),
};

const s32 gLamakanDesertRecoveryPointsOther[] = {
    PIXELS(192), PIXELS(224),
    PIXELS(192), PIXELS(352),
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
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

struct ScenePlacement gLamakanDesertPlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gLamakanDesertEastPlacements[] = {
    { 0xc3, FLAG_LAMAKAN_MANTICORE_BEATEN, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(400), 0, PIXELS(364), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gLamakanDesertSmogPlacements[] = {
    { 0xc3, FLAG_LAMAKAN_MANTICORE_BEATEN, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(352), 0, PIXELS(204), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(736), 0, PIXELS(396), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(144), 0, PIXELS(380), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(576), 0, PIXELS(716), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { JUPITER_DJINNI, FLAG_DJINNI_0 + DJINNI_SMOG, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gLamakanDesertManticorePlacements[] = {
    { 0xc3, FLAG_LAMAKAN_MANTICORE_BEATEN, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(128), 0, PIXELS(316), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

/// The four scenes' one event table. Its exit rows keep their numbers: each
/// takes the exit of that number in whichever scene the party is in, which is a
/// different way out in each (LAMAKAN_DESERT_EAST_TO_WORLD_MAP,
/// LAMAKAN_DESERT_SMOG_TO_LAMAKAN_DESERT_EAST and the others).
struct SceneEvent gLamakanDesertEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 4 },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_LeaveForTheWorldMap) },
    { EVENT_TOUCH, 20, FLAG_DESERT_EVENT_RUNNING, EVENT_SCRIPT(LamakanDesert_CollapseFromHeat) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { EVENT_TOUCH, 22, FLAG_DESERT_EVENT_RUNNING, EVENT_SCRIPT(LamakanDesert_FallIntoPit) },
    { EVENT_TOUCH, LAMAKAN_DESERT_CELL_SLOPE, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_SlideDownDune) },
    { EVENT_TOUCH, 40, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_MeetManticore) },
    { EVENT_TALK, MANTICORE, FLAG_LAMAKAN_MANTICORE_BEATEN, EVENT_SCRIPT(LamakanDesert_FightManticore) },
    { EVENT_TALK, SMOG, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_TalkToSmog) },
    { SEARCH(SEARCH_CHEST), 100, FLAG_LAMAKAN_DESERT_VULCAN_AXE, FIND_ITEM(ITEM_VULCAN_AXE) },
    { SEARCH(SEARCH_UNNAMED), 101, FLAG_LAMAKAN_DESERT_FIRST_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { SEARCH(SEARCH_CHEST), 102, FLAG_LAMAKAN_DESERT_LUCKY_PEPPER, FIND_ITEM(ITEM_LUCKY_PEPPER) },
    { SEARCH(SEARCH_UNNAMED), 103, FLAG_LAMAKAN_DESERT_SECOND_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { SEARCH(SEARCH_CHEST), 105, FLAG_LAMAKAN_DESERT_777_COINS, FIND_COINS(777) },
    { SEARCH(SEARCH_CHEST), 106, FLAG_LAMAKAN_DESERT_POTION, FIND_ITEM(ITEM_POTION) },
    { SEARCH(SEARCH_CHEST), 107, FLAG_LAMAKAN_DESERT_MINT, FIND_ITEM(ITEM_MINT) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Reveal_ShowSecrets) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Reveal_HideSecrets) },
    { EVENT_RAISED, LAMAKAN_DESERT_RAISED_HEAT_QUARTER, FLAG_SECRETS_REVEALED, EVENT_SCRIPT(LamakanDesert_ComplainAboutHeat) },
    { EVENT_RAISED, LAMAKAN_DESERT_RAISED_HEAT_HALF, FLAG_SECRETS_REVEALED, EVENT_SCRIPT(LamakanDesert_ComplainAboutHeat) },
    { EVENT_RAISED, LAMAKAN_DESERT_RAISED_HEAT_THREE_QUARTERS, FLAG_SECRETS_REVEALED, EVENT_SCRIPT(LamakanDesert_ComplainAboutHeat) },
    { EVENT_TOUCH, 100, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { EVENT_TOUCH, 101, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { EVENT_TOUCH, 102, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { EVENT_TOUCH, 103, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { EVENT_TOUCH, 104, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { EVENT_TOUCH, 105, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { EVENT_TOUCH, 106, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { EVENT_TOUCH, 107, CONDITION_ALWAYS, EVENT_SCRIPT(HeatGauge_CountStep) },
    { SCENE_EVENTS_END },
};
// clang-format on

/// The gauge: how full it is (0 to 120), how far it has slid onto the
/// screen, and a flash as it fills.
s16 gHeatGaugeLevel = 0;

s16 gHeatGaugeSlide = 0;

s16 gHeatGaugeFlash = 0;
