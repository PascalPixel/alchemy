/// @file
/// Map changes: warps, returns from battle, the main loop, entering.
// Part of overworld.c.

#include "../overworld_internal.h"

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
/// current map. warpId is the kind of warp Map_Run ended on; the map's
/// warp table gives the first warp of that kind (or of any kind) whose flag, if
/// it has one, is not set. A warp of kind 0 or the end of the map's warps
/// falls back to the map of its first warp, at warp destination 0. Called by
/// Game_Run with Map_Run's result; sets gSaveBlock.mapId and
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

  currentMap = gSaveBlock.mapId;
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
        if (flag == 0 || GameFlag_GetFar(flag) == 0) {
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
    gSaveBlock.mapId = nextMap;
    gSaveBlock.warpDest = nextWarpDest;
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
/// Game_Run after each map from RUN_NONE on; stores result in
/// gSaveBlock.runResult and sets gSaveBlock.mapId and .warpDest.
void Map_ReturnFromRun(s32 result) {
  struct Unit *unit;
  s32 i;
  s32 mapId;
  s32 warpDest;

  gSaveBlock.runResult = result;
  if (result == -1) {
    // Revive the leader before choosing the defeat return point.
    unit = Unit_GetFar(gSaveBlock.leaderLocalId);
    if (unit->hp == 0) {
      unit->hp = 1;
      Unit_UpdateGauges(unit);
    }
    if (GameFlag_GetFar(FLAG_CHILDHOOD)) {
      for (i = 0; i < 2; i++) {
        unit = Unit_GetFar(i);
        unit->hp = unit->hpMax;
        unit->pp = unit->ppMax;
        Unit_UpdateGauges(unit);
      }
    }
    mapId = gSaveBlock.defeatMap;
    warpDest = gSaveBlock.defeatWarpDest;
    if (mapId == -1 && warpDest == -1) {
      gSaveBlock.mapId = gSaveBlock.savedMap;
      gSaveBlock.warpDest = gSaveBlock.savedWarpDest;
    } else {
      if (mapId != -1)
        gSaveBlock.mapId = mapId;
      else
        gSaveBlock.mapId = gSaveBlock.defaultMap;
      if (warpDest != -1)
        gSaveBlock.warpDest = warpDest;
      else
        gSaveBlock.warpDest = gSaveBlock.defaultWarpDest;
    }
  } else {
    // An explicit script destination overrides the map's default return.
    mapId = gSaveBlock.nextMap;
    warpDest = gSaveBlock.nextWarpDest;
    if (mapId != -1 || warpDest != -1) {
      if (mapId != -1)
        gSaveBlock.mapId = mapId;
      else
        gSaveBlock.mapId = gSaveBlock.defaultMap;
      if (warpDest != -1)
        gSaveBlock.warpDest = warpDest;
      else
        gSaveBlock.warpDest = gSaveBlock.defaultWarpDest;
    } else {
      gSaveBlock.mapId = gSaveBlock.defaultMap;
      gSaveBlock.warpDest = gSaveBlock.defaultWarpDest;
      GameFlag_SetFar(FLAG_KEEP_PARTY_POSITION);
    }
  }
}

/// Returns the map section mapId belongs to. The maps of one map section share
/// their music, area name and the flags kept while inside it.
/// Called by Map_ChooseMusic.
s32 Map_GetMapSec(s32 mapId) { return gMapHeaders[mapId].mapSec; }

/// The last two 16-colour background palettes, which the minigame runners save
/// and restore around a minigame; palette 15 holds the window colours
/// (WindowColor_Apply).
enum {
  MINIGAME_PALETTE_FIRST = 14, ///< BG palette 14
  MINIGAME_PALETTE_BYTES = 64  ///< palettes 14 and 15: 2 x 16 colours x 2 bytes
};

/// Runs the Lucky Dice minigame, preserving BG palettes 14 and 15 in a 64-byte
/// Alloc copy freed afterwards. Returns the minigame's result, 0, for
/// Map_ReturnFromRun.
static __inline__ s32 RunLuckyDice(s32 warpDest) {
  void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);
  void *saved = Alloc(MINIGAME_PALETTE_BYTES);
  s32 result;

  Dma_Set(palette, saved,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  result = LuckyDice_StartFar(warpDest);
  Dma_Set(saved, palette,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  Free(saved);
  return result;
}

/// Runs Lucky Wheels, preserving BG palettes 14 and 15 as
/// RunLuckyDice does. Returns LuckyWheels_Start's result, 0.
static __inline__ s32 RunLuckyWheels(s32 warpDest) {
  void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);
  void *saved = Alloc(MINIGAME_PALETTE_BYTES);
  s32 result;

  Dma_Set(palette, saved,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  result = LuckyWheels_StartFar(warpDest);
  Dma_Set(saved, palette,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  Free(saved);
  return result;
}

/// The game's main loop, which never returns. mode, an enum GameRunMode,
/// chooses the first map. Each pass resets the
/// runtime, then either runs a battle or minigame and picks the map to return
/// to, or loads a map's code, starts its music and runs it until the
/// party takes a warp. Called with GAME_RUN_TITLE through Game_RunFar by
/// System_Initialize at power-on and by Game_Restart.
void Game_Run(s32 mode) {
  const struct MapHeader *table;
  const struct MapHeader *record;
  s32 warpDest;

  table = gMapHeaders;
  // Choose the first map before initializing the per-map runtime.
  if (gDebugMode && mode == GAME_RUN_DEBUG_VALE) {
    gSaveBlock.mapId = (s32)&MapId_ValeGate;
    gSaveBlock.warpDest = mode;
  } else if (gDebugMode && mode == GAME_RUN_DEBUG_MAIN_MENU) {
    gSaveBlock.mapId = (s32)&MapId_MainMenu;
    gSaveBlock.warpDest = MAIN_MENU_FROM_SANCTUM;
  } else {
    SaveBlock_InitializeDefaultsFar();
    gSaveBlock.mapId = (s32)&MapId_Title;
    gSaveBlock.warpDest = TITLE_AFTER_RUN;
  }
  // The window colours from the options: hue, then brightness.
  WindowColor_ApplyFar(gSaveBlock.windowColor[0], gSaveBlock.windowColor[1]);
  VramCache_Reset();
  Task_ResetAll();
  Task_ResetAll();
  for (;;) {
    // FLAG_SKIP_MAP_CHANGE_SOUND skips SE_STOP_SLOT3 for one map
    // change, then clears itself.
    if (GameFlag_GetFar(FLAG_SKIP_MAP_CHANGE_SOUND))
      GameFlag_ClearFar(FLAG_SKIP_MAP_CHANGE_SOUND);
    else
      Sound_PlaySongFar(SE_STOP_SLOT3);
    record = &table[gSaveBlock.mapId];
    warpDest = gSaveBlock.warpDest;

    // Stop DMA 0, which a map may have left running for an HBlank effect.
    {
      volatile u16 *dma0 = REG_DMA0;

      dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
      dma0[5] &= ~DMA_ENABLE;
      dma0[5];
    }
    // Drop the tasks, the HBlank and VCount handlers (IRQs 1 and 2) and every
    // heap block the last map, battle or minigame left.
    Task_ResetAll();
    Runtime_SetIrqHandler(1, 0, 0);
    Runtime_SetIrqHandler(2, 0, 0);
    InitHeap();
    DebugText_Clear();
    VramCache_Reset();
    // A battle or minigame returns a result, not a warp: the return map is
    // chosen from the save block, and no overlay is loaded.
    if (gSaveBlock.mapId >= RUN_NONE) {
      switch (gSaveBlock.mapId) {
      case RUN_BATTLE:
        warpDest = Battle_RunFar(warpDest);
        break;
      case RUN_LUCKY_DICE:
        warpDest = RunLuckyDice(warpDest);
        break;
      case RUN_LUCKY_WHEELS:
        warpDest = RunLuckyWheels(warpDest);
        break;
      case RUN_NONE:
        warpDest = 0;
        break;
      }
      Map_ReturnFromRun(warpDest);
      continue;
    }

    // A map: set its flags and music, load its code and run it.
    Map_ResetFlagsOnEnter(gSaveBlock.mapId,
                          GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION));
    Map_ChooseMusic();
    if (!GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION)) {
      // Either flag keeps the music that is playing: FLAG_KEEP_MUSIC for this
      // one change, FLAG_KEEP_MUSIC_HELD for as long as it stays set.
      if (GameFlag_GetFar(FLAG_KEEP_MUSIC) ||
          GameFlag_GetFar(FLAG_KEEP_MUSIC_HELD))
        GameFlag_ClearFar(FLAG_KEEP_MUSIC);
      else
        Map_PlayMusic();
    } else if (gSaveBlock.returnMusic != -1) {
      // Back where the party was: resume the music saved when the pause menu
      // last opened (run_map.c). A random battle and every normal entry set
      // returnMusic to -1, so the map's own music plays instead.
      Sound_PlaySongFar(gSaveBlock.returnMusic);
    } else {
      Map_PlayMusic();
    }
    gSaveBlock.layoutId = record->layoutIndex;
    Map_LoadOverlay(0);
    Map_TakeWarp(Map_Run(warpDest));
  }
}

/// Loads the current map's overlay code (its resource) into gOverlayArea.
/// Called by Game_Run before Map_Run.
void Map_LoadOverlay(s32 unused) {
  const struct MapHeader *table;

  table = gMapHeaders;
  Overlay_Load(table[gSaveBlock.mapId].resourceId, &gOverlayArea);
}

#define MAP_HEADER(id) (&gMapHeaders[id])

/// Updates the flags and state that last for one map or one map section, on
/// entering the current map. Unless keep is set (the party returns to where it
/// was, as after a battle), the flags of the last map are cleared, and on
/// entering a new map section so are the map section's flags and step
/// counters, the location name is shown and the warp destination becomes the
/// point Retreat returns to. Then the map section's visited flag
/// (FLAG_MAPSEC_VISITED) is set. Called by Game_Run before each map, with keep
/// from FLAG_KEEP_PARTY_POSITION; sets gSaveBlock.mapSec and .mapType.
void Map_ResetFlagsOnEnter(s32 unused, s32 keep) {
  s32 mapSec = MAP_HEADER(gSaveBlock.mapId)->mapSec;
  s32 flag;

  if (keep == 0) {
    // Local puzzle flags expire on every normal entry to a map.
    for (flag = FLAG_MAP_TEMP_FIRST; flag <= FLAG_MAP_TEMP_LAST; flag++)
      GameFlag_ClearFar(flag);
    if (mapSec != gSaveBlock.mapSec) {
      // Map section state and Retreat's destination survive moves within it.
      for (flag = FLAG_MAPSEC_TEMP_FIRST; flag <= FLAG_MAPSEC_TEMP_LAST; flag++)
        GameFlag_ClearFar(flag);
      GameFlag_SetFar(FLAG_SHOW_LOCATION_NAME);
      gSaveBlock.encounterSteps = 0;
      gSaveBlock.hazardSteps = 0;
      GameFlag_ClearFar(FLAG_VENUS_LIGHTHOUSE_LIT);
      GameFlag_ClearFar(FLAG_MERCURY_LIGHTHOUSE_LIT);
      GameFlag_ClearFar(FLAG_MARS_LIGHTHOUSE_LIT);
      GameFlag_ClearFar(FLAG_JUPITER_LIGHTHOUSE_LIT);
      gSaveBlock.retreatMap = gSaveBlock.mapId;
      gSaveBlock.retreatWarpDest = gSaveBlock.warpDest;
    }
    // The Djinn's hiding flags last for one map, even within a map section, and
    // so do the 16 after them to 0xdf, which nothing else uses.
    for (flag = FLAG_DJINN_HIDING_FIRST; flag <= 0xdf; flag++)
      GameFlag_ClearFar(flag);
    GameFlag_ClearFar(FLAG_LINK_BATTLE);
    GameFlag_ClearFar(FLAG_RETREAT_BLOCKED);
    // FLAG_NO_ENCOUNTERS_IN_MAPSEC stops random battles
    // (Encounter_CountStepsAndSelectFormation).
    GameFlag_ClearFar(FLAG_NO_ENCOUNTERS_IN_MAPSEC);
    GameFlag_ClearFar(FLAG_IN_TOWN);
    GameFlag_ClearFar(FLAG_WORLD_MAP_LEFT);
    gSaveBlock.returnMusic = -1;
  }
  gSaveBlock.mapSec = mapSec;
  GameFlag_SetFar(FLAG_MAPSEC_VISITED(mapSec & 0x7f));
  gSaveBlock.mapType = MAP_HEADER(gSaveBlock.mapId)->mapType;
  if (gSaveBlock.mapType == MAP_TYPE_TOWN)
    GameFlag_SetFar(FLAG_IN_TOWN);
  GameFlag_RefreshLureCapFar();
}

/// Starts the current map's field music, chosen by
/// Map_ChooseMusic. Called by Game_Run, and by
/// 34 map overlays through Map_PlayMusicFar.
void Map_PlayMusic(void) { Sound_PlaySongFar(gSaveBlock.mapMusic); }

#undef WARP_MAP_MASK
#undef WARP_DEST
#undef WARP_ID
#undef WARP_HAS_FLAG
#undef WARP_TABLE_END
#undef WARP_ID_ANY
#undef MAP_HEADER
