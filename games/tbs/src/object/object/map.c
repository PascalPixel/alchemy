/// @file
/// Map changes: warps, returns from battle, the main loop, entering.
// Part of object.c.

#include "../object_internal.h"

// The map overlay's warp table: a header word holding a map id, then that
// map's warps, one packed word each (and a flag word after it when
// WARP_HAS_FLAG is set). Map id WARP_TABLE_END ends the warps of a map and
// the whole table. MAP_WARP and MAP_WARP_IF (map_tables.h) give a warp
// word's bits.
#define WARP_MAP_MASK 0xfff
#define WARP_DEST(word) (((word) & 0xff000) >> 12)
#define WARP_ID(word) (((word) & 0x0ff00000) >> 20)
#define WARP_HAS_FLAG 0x10000000
#define WARP_TABLE_END 0x1ff
#define WARP_ID_ANY 0xff

/// Chooses the map and warp destination to go to when the party leaves the
/// current map. warpId is the kind of warp Field_RunMap ended on; the map's
/// warp table gives the first warp of that kind (or of any kind) whose flag, if
/// it has one, is not set. A warp of kind 0 or the end of the map's warps
/// falls back to the map of its first warp, at warp destination 0. Called by
/// Game_Run with Field_RunMap's result; sets gGameState.mapId and
/// .warpDest, and leaves them alone for WARP_NONE, whose destination is set
/// already, or for a map with no header in the table.
void Map_TakeWarp(s32 warpId) {
  s16 currentMap;
  const u32 *warpTable;
  const u32 *firstWarp;
  u32 word;
  s32 mapId;
  s32 nextMap;
  s32 nextWarpDest;
  s32 warpDest;
  s32 kind;
  s32 flag;

  currentMap = gGameState.mapId;
  warpTable = gOverlayArea.warps();
  nextMap = WARP_NONE;
  nextWarpDest = 0;
  if (warpId == WARP_NONE)
    return;

  for (;;) {
    // Find the header of the current map: warp words have high bits set.
    mapId = *warpTable++;
    if (mapId & ~WARP_MAP_MASK)
      continue;
    mapId &= WARP_MAP_MASK;
    if (mapId == WARP_TABLE_END)
      break;
    if (mapId != currentMap)
      continue;

    firstWarp = warpTable;
    for (;;) {
      word = *warpTable++;
      mapId = word & WARP_MAP_MASK;
      warpDest = WARP_DEST(word);
      kind = WARP_ID(word);
      flag = word & WARP_HAS_FLAG;
      if (flag)
        flag = *warpTable++;
      if (mapId == WARP_TABLE_END || kind == 0) {
        nextMap = *firstWarp & 0x1ff; // only 9 bits, unlike WARP_MAP_MASK
        break;
      }
      if (kind == WARP_ID_ANY || kind == warpId) {
        if (flag == 0 || GameFlag_TestFar(flag) == 0) {
          nextMap = mapId;
          nextWarpDest = warpDest;
          break;
        }
      }
    }
    break;
  }

  // Commit the chosen warp only after a matching map header was found.
  if (nextMap != WARP_NONE) {
    gGameState.mapId = nextMap;
    gGameState.warpDest = nextWarpDest;
  }
}

/// Chooses where the game continues after a battle or anything else that is
/// not a map (see Game_Run). result is its result:
/// -1 when the party was defeated, which revives the leader with 1 HP (and,
/// with FLAG_CHILDHOOD, fully heals Isaac and Garet, characters 0 and 1) and
/// returns to the defeat point, or to the last saved point when none is set.
/// Any other result goes to the next map the script set, or to the default
/// map (the map the battle or minigame was started from) with the
/// party keeping its position (FLAG_KEEP_PARTY_POSITION). Called by
/// Game_Run after each map from RUN_BLANK on; stores result in
/// gGameState.mapChangeReason and sets gGameState.mapId and .warpDest.
void Map_ReturnFromBattle(s32 result) {
  struct BattleUnit *unit;
  s32 i;
  s32 mapId;
  s32 warpDest;

  gGameState.mapChangeReason = result;
  if (result == -1) {
    // Revive the leader before choosing the defeat return point.
    unit = Unit_GetFar(gGameState.selectedObjectEvent);
    if (unit->hp == 0) {
      unit->hp = 1;
      Unit_UpdateGauges(unit);
    }
    if (GameFlag_TestFar(FLAG_CHILDHOOD)) {
      for (i = 0; i < 2; i++) {
        unit = Unit_GetFar(i);
        unit->hp = unit->hpMax;
        unit->pp = unit->ppMax;
        Unit_UpdateGauges(unit);
      }
    }
    mapId = gGameState.defeatMap;
    warpDest = gGameState.defeatWarpDest;
    if (mapId == -1 && warpDest == -1) {
      gGameState.mapId = gGameState.savedMap;
      gGameState.warpDest = gGameState.savedWarpDest;
    } else {
      if (mapId != -1)
        gGameState.mapId = mapId;
      else
        gGameState.mapId = gGameState.defaultMap;
      if (warpDest != -1)
        gGameState.warpDest = warpDest;
      else
        gGameState.warpDest = gGameState.defaultWarpDest;
    }
  } else {
    // An explicit script destination overrides the map's default return.
    mapId = gGameState.nextMap;
    warpDest = gGameState.nextWarpDest;
    if (mapId != -1 || warpDest != -1) {
      if (mapId != -1)
        gGameState.mapId = mapId;
      else
        gGameState.mapId = gGameState.defaultMap;
      if (warpDest != -1)
        gGameState.warpDest = warpDest;
      else
        gGameState.warpDest = gGameState.defaultWarpDest;
    } else {
      gGameState.mapId = gGameState.defaultMap;
      gGameState.warpDest = gGameState.defaultWarpDest;
      GameFlag_SetBitFar(FLAG_KEEP_PARTY_POSITION);
    }
  }
}

/// Returns the map section mapId belongs to. The maps of one map section share
/// their music, area name and the flags kept while inside it.
/// Called by Map_ResolveInteractionResult.
s32 FieldEffect_GetResourceGroup(s32 mapId) {
  return gField_MapTable[mapId].group;
}

/// The last two 16-colour background palettes, which the minigame runners save
/// and restore around a minigame; palette 15 holds the window colours
/// (PaletteGlow_Update).
enum {
  MINIGAME_PALETTE_FIRST = 14, ///< BG palette 14
  MINIGAME_PALETTE_BYTES = 64  ///< palettes 14 and 15: 2 x 16 colours x 2 bytes
};

/// Runs the Lucky Dice minigame, preserving BG palettes 14 and 15 in a 64-byte
/// MemAlloc copy freed afterwards. Returns the minigame's result, 0, for
/// Map_ReturnFromBattle.
static __inline__ s32 RunLuckyDice(s32 warpDest) {
  void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);
  void *saved = MemAlloc(MINIGAME_PALETTE_BYTES);
  s32 result;

  Dma_Set(palette, saved,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  result = Runtime_BlankDisplayAndRunFar(warpDest);
  Dma_Set(saved, palette,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  MemFree(saved);
  return result;
}

/// Runs the slot machine (Lucky Wheels), preserving BG palettes 14 and 15 as
/// RunLuckyDice does. Returns ReelGame_Start's result, 0.
static __inline__ s32 RunReelGame(s32 warpDest) {
  void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);
  void *saved = MemAlloc(MINIGAME_PALETTE_BYTES);
  s32 result;

  Dma_Set(palette, saved,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  result = ReelGame_StartFar(warpDest);
  Dma_Set(saved, palette,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  MemFree(saved);
  return result;
}

/// The game's main loop, which never returns. mode, an enum GameRunMode,
/// chooses the first map. Each pass resets the
/// runtime, then either runs a battle or minigame and picks the map to return
/// to, or loads a map's code, starts its music and runs it until the
/// party takes a warp. Called with GAME_RUN_TITLE through Game_RunFar by
/// System_Initialize at power-on and by Game_Restart.
void Game_Run(s32 mode) {
  const struct MapRecord *table;
  const struct MapRecord *record;
  s32 warpDest;

  table = gField_MapTable;
  // Choose the first map before initializing the per-map runtime.
  if (gDebugMode && mode == GAME_RUN_DEBUG_VALE) {
    gGameState.mapId = (s32)&MapId_ValeGate;
    gGameState.warpDest = mode;
  } else if (gDebugMode && mode == GAME_RUN_DEBUG_CLEAR) {
    gGameState.mapId = (s32)&MapId_Clear;
    gGameState.warpDest = CLEAR_FROM_SANCTUM;
  } else {
    GameState_InitializeDefaultsFar();
    gGameState.mapId = (s32)&MapId_Title;
    gGameState.warpDest = TITLE_AFTER_RUN;
  }
  // The window colours from the options: hue, then brightness.
  PaletteGlow_UpdateFar(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
  VramCache_Reset();
  Scheduler_ResetTaskTable();
  Scheduler_ResetTaskTable();
  for (;;) {
    // FLAG_SKIP_MAP_CHANGE_SOUND skips SE_STOP_SLOT3 for one map
    // change, then clears itself.
    if (GameFlag_TestFar(FLAG_SKIP_MAP_CHANGE_SOUND))
      GameFlag_ClearBitFar(FLAG_SKIP_MAP_CHANGE_SOUND);
    else
      AudioCommand_PlayFar(SE_STOP_SLOT3);
    record = &table[gGameState.mapId];
    warpDest = gGameState.warpDest;

    // Stop DMA 0, which a map may have left running for an HBlank effect.
    {
      volatile u16 *dma0 = REG_DMA0;

      dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
      dma0[5] &= ~DMA_ENABLE;
      dma0[5];
    }
    // Drop the tasks, the HBlank and VCount handlers (IRQs 1 and 2) and every
    // heap block the last map, battle or minigame left.
    Scheduler_ResetTaskTable();
    Runtime_SetIrqHandler(1, 0, 0);
    Runtime_SetIrqHandler(2, 0, 0);
    Runtime_InitializeHeap();
    DebugText_Clear();
    VramCache_Reset();
    // A battle or minigame returns a result, not a warp: the return map is
    // chosen from the game state, and no overlay is loaded.
    if (gGameState.mapId >= RUN_BLANK) {
      switch (gGameState.mapId) {
      case RUN_BATTLE:
        warpDest = Battle_RunEncounterFar(warpDest);
        break;
      case RUN_LUCKY_DICE:
        warpDest = RunLuckyDice(warpDest);
        break;
      case RUN_REEL_GAME:
        warpDest = RunReelGame(warpDest);
        break;
      case RUN_BLANK:
        warpDest = 0;
        break;
      }
      Map_ReturnFromBattle(warpDest);
      continue;
    }

    // A map: set its flags and music, load its code and run it.
    Map_ResetFlagsOnEnter(gGameState.mapId,
                          GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION));
    Map_ResolveInteractionResult();
    if (!GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION)) {
      // Either flag keeps the music that is playing: FLAG_KEEP_MUSIC for this
      // one change, FLAG_KEEP_MUSIC_HELD for as long as it stays set.
      if (GameFlag_TestFar(FLAG_KEEP_MUSIC) ||
          GameFlag_TestFar(FLAG_KEEP_MUSIC_HELD))
        GameFlag_ClearBitFar(FLAG_KEEP_MUSIC);
      else
        Audio_PlayCueFromEventWork();
    } else if (gGameState.returnCue != -1) {
      // Back where the party was: resume the music saved when the pause menu
      // last opened (run_map.c). A random battle and every normal entry set
      // returnCue to -1, so the map's own music plays instead.
      AudioCommand_PlayFar(gGameState.returnCue);
    } else {
      Audio_PlayCueFromEventWork();
    }
    gGameState.layoutId = record->layoutIndex;
    Map_LoadOverlay(0);
    Map_TakeWarp(Field_RunMap(warpDest));
  }
}

/// Loads the current map's overlay code (its resource) into gOverlayArea.
/// Called by Game_Run before Field_RunMap.
void Map_LoadOverlay(s32 unused) {
  const struct MapRecord *table;

  table = gField_MapTable;
  Resource_LoadCode(table[gGameState.mapId].resourceId, &gOverlayArea);
}

#define MAP_RECORD(id) (&gField_MapTable[id])

/// Updates the flags and state that last for one map or one map section, on
/// entering the current map. Unless keep is set (the party returns to where it
/// was, as after a battle), the flags of the last map are cleared, and on
/// entering a new map section so are the map section's flags and step
/// counters, the location name is shown and the warp destination becomes the
/// point Retreat returns to. Then the map section's visited flag
/// (FLAG_MAPSEC_VISITED) is set. Called by Game_Run before each map, with keep
/// from FLAG_KEEP_PARTY_POSITION; sets gGameState.mapSec and .mapType.
void Map_ResetFlagsOnEnter(s32 unused, s32 keep) {
  s32 group = MAP_RECORD(gGameState.mapId)->group;
  s32 flag;

  if (keep == 0) {
    // Local puzzle flags expire on every normal entry to a map.
    for (flag = FLAG_MAP_TEMP_FIRST; flag <= FLAG_MAP_TEMP_LAST; flag++)
      GameFlag_ClearBitFar(flag);
    if (group != gGameState.mapSec) {
      // Map section state and Retreat's destination survive moves within it.
      for (flag = FLAG_MAPSEC_TEMP_FIRST; flag <= FLAG_MAPSEC_TEMP_LAST; flag++)
        GameFlag_ClearBitFar(flag);
      GameFlag_SetBitFar(FLAG_SHOW_LOCATION_NAME);
      gGameState.encounterSteps = 0;
      gGameState.hazardSteps = 0;
      GameFlag_ClearBitFar(FLAG_VENUS_LIGHTHOUSE_LIT);
      GameFlag_ClearBitFar(FLAG_MERCURY_LIGHTHOUSE_LIT);
      GameFlag_ClearBitFar(FLAG_MARS_LIGHTHOUSE_LIT);
      GameFlag_ClearBitFar(FLAG_JUPITER_LIGHTHOUSE_LIT);
      gGameState.retreatMap = gGameState.mapId;
      gGameState.retreatWarpDest = gGameState.warpDest;
    }
    // The Djinn's hiding flags last for one map, even within a map section, and
    // so do the 16 after them to 0xdf, which nothing else uses.
    for (flag = FLAG_DJINNI_HIDING_0; flag <= 0xdf; flag++)
      GameFlag_ClearBitFar(flag);
    GameFlag_ClearBitFar(FLAG_LINK_BATTLE);
    GameFlag_ClearBitFar(FLAG_RETREAT_BLOCKED);
    // FLAG_NO_ENCOUNTERS_IN_MAPSEC stops random battles
    // (Encounter_SelectEnemyGroup).
    GameFlag_ClearBitFar(FLAG_NO_ENCOUNTERS_IN_MAPSEC);
    GameFlag_ClearBitFar(FLAG_IN_TOWN);
    GameFlag_ClearBitFar(FLAG_WORLD_MAP_LEFT);
    gGameState.returnCue = -1;
  }
  gGameState.mapSec = group;
  GameFlag_SetBitFar(FLAG_MAPSEC_VISITED(group & 0x7f));
  gGameState.mapType = MAP_RECORD(gGameState.mapId)->variant;
  if (gGameState.mapType == MAP_TYPE_TOWN)
    GameFlag_SetBitFar(FLAG_IN_TOWN);
  GameFlag_RefreshLureCapFar();
}

/// Starts the current map's field music, chosen by
/// Map_ResolveInteractionResult. Called by Game_Run, and by
/// 34 map overlays through Audio_PlayCueFromEventWorkFar.
void Audio_PlayCueFromEventWork(void) { AudioCommand_PlayFar(gGameState.cue); }

#undef WARP_MAP_MASK
#undef WARP_DEST
#undef WARP_ID
#undef WARP_HAS_FLAG
#undef WARP_TABLE_END
#undef WARP_ID_ANY
#undef MAP_RECORD
