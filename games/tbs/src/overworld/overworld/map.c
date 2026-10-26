/// @file
/// Scene changes: exits, returns from battle, the sequence manager, entering.
// Part of overworld.c.

#include "../overworld_internal.h"

// The scene overlay's exit table: a header word holding a scene id, then that
// scene's exits, one packed word each (and a flag word after it when
// EXIT_HAS_FLAG is set). Scene id EXIT_TABLE_END ends the exits of a scene and
// the whole table. SCENE_EXIT and SCENE_EXIT_UNLESS (scene_tables.h) give an
// exit word's bits.
#define EXIT_MAP_MASK 0xfff
#define ENTRANCE(word) (((word) & 0xff000) >> 12)
#define EXIT_ID(word) (((word) & 0x0ff00000) >> 20)
#define EXIT_HAS_FLAG 0x10000000
#define EXIT_TABLE_END 0x1ff
#define EXIT_ID_ANY 0xff

/// Chooses the scene and entrance to go to when the party leaves the
/// current scene. exitId is the kind of exit SceneManager_Run ended on; the
/// scene's exit table gives the first exit of that kind (or of any kind) whose
/// flag, if it has one, is not set. An exit of kind 0 or the end of the scene's
/// exits falls back to the scene of its first exit, at entrance 0.
/// Called by SequenceManager_Run with SceneManager_Run's result; sets
/// gSaveBlock.sceneId and .entrance, and leaves them alone for EXIT_NONE, whose
/// destination is set already, or for a scene with no header in the table.
void Scene_TakeExit(s32 exitId) {
  s16 currentMap;
  const u32 *exitTable;
  const u32 *firstExit;
  u32 word;
  s32 sceneId;
  s32 nextScene;
  s32 nextEntrance;
  s32 entrance;
  s32 kind;
  s32 flag;

  currentMap = gSaveBlock.sceneId;
  exitTable = gOverlayArea.exits();
  nextScene = EXIT_NONE;
  nextEntrance = 0;
  if (exitId == EXIT_NONE)
    return;

  for (;;) {
    // Find the header of the current scene: exit words have high bits set.
    sceneId = *exitTable++;
    if (sceneId & ~EXIT_MAP_MASK)
      continue;
    sceneId &= EXIT_MAP_MASK;
    if (sceneId == EXIT_TABLE_END)
      break;
    if (sceneId != currentMap)
      continue;

    firstExit = exitTable;
    for (;;) {
      word = *exitTable++;
      sceneId = word & EXIT_MAP_MASK;
      entrance = ENTRANCE(word);
      kind = EXIT_ID(word);
      flag = word & EXIT_HAS_FLAG;
      if (flag)
        flag = *exitTable++;
      if (sceneId == EXIT_TABLE_END || kind == 0) {
        nextScene = *firstExit & 0x1ff; // only 9 bits, unlike EXIT_MAP_MASK
        break;
      }
      if (kind == EXIT_ID_ANY || kind == exitId) {
        if (flag == 0 || GameFlag_GetFar(flag) == 0) {
          nextScene = sceneId;
          nextEntrance = entrance;
          break;
        }
      }
    }
    break;
  }

  // Commit the chosen exit only after a matching scene header was found.
  if (nextScene != EXIT_NONE) {
    gSaveBlock.sceneId = nextScene;
    gSaveBlock.entrance = nextEntrance;
  }
}

/// Chooses where the game continues after a battle or anything else that is
/// not a scene (see SequenceManager_Run). result is its result:
/// -1 when the party was defeated, which revives the leader with 1 HP (and,
/// with FLAG_CHILDHOOD, fully heals Isaac and Garet, characters 0 and 1) and
/// returns to the story battle's defeat point
/// (FieldScript_SetDefeatDestination) or, when none is set, to the restart
/// point, usually the last Sanctum (Scene_SetDefeatReturnPoint). Any other
/// result goes to the next scene the script set, or to the default scene (the
/// scene the battle or minigame was started from) with the party keeping its
/// position (FLAG_KEEP_PARTY_POSITION). Called by SequenceManager_Run after
/// each scene from SEQUENCE_NONE on; stores result in gSaveBlock.runResult and
/// sets gSaveBlock.sceneId and .entrance.
void Scene_ReturnFromSequence(s32 result) {
  struct Unit *unit;
  s32 i;
  s32 sceneId;
  s32 entrance;

  gSaveBlock.runResult = result;
  if (result == -1) {
    // Revive the leader before choosing the defeat return point.
    unit = Unit_GetFar(gSaveBlock.leaderActorId);
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
    sceneId = gSaveBlock.defeatScene;
    entrance = gSaveBlock.defeatEntrance;
    if (sceneId == -1 && entrance == -1) {
      gSaveBlock.sceneId = gSaveBlock.restartScene;
      gSaveBlock.entrance = gSaveBlock.restartEntrance;
    } else {
      if (sceneId != -1)
        gSaveBlock.sceneId = sceneId;
      else
        gSaveBlock.sceneId = gSaveBlock.defaultScene;
      if (entrance != -1)
        gSaveBlock.entrance = entrance;
      else
        gSaveBlock.entrance = gSaveBlock.defaultEntrance;
    }
  } else {
    // An explicit script destination overrides the scene's default return.
    sceneId = gSaveBlock.nextScene;
    entrance = gSaveBlock.nextEntrance;
    if (sceneId != -1 || entrance != -1) {
      if (sceneId != -1)
        gSaveBlock.sceneId = sceneId;
      else
        gSaveBlock.sceneId = gSaveBlock.defaultScene;
      if (entrance != -1)
        gSaveBlock.entrance = entrance;
      else
        gSaveBlock.entrance = gSaveBlock.defaultEntrance;
    } else {
      gSaveBlock.sceneId = gSaveBlock.defaultScene;
      gSaveBlock.entrance = gSaveBlock.defaultEntrance;
      GameFlag_SetFar(FLAG_KEEP_PARTY_POSITION);
    }
  }
}

/// Returns the area sceneId belongs to. The scenes of one area
/// share their music, area name and the flags kept while inside it. Called by
/// Area_GetNameIndex.
s32 Scene_GetArea(s32 sceneId) { return gSceneHeaders[sceneId].area; }

/// The last two 16-colour background palettes, which the minigame runners save
/// and restore around a minigame; palette 15 holds the window colours
/// (WindowColor_Apply).
enum {
  MINIGAME_PALETTE_FIRST = 14, ///< BG palette 14
  MINIGAME_PALETTE_BYTES = 64  ///< palettes 14 and 15: 2 x 16 colours x 2 bytes
};

/// Runs the Lucky Dice minigame, preserving BG palettes 14 and 15 in a 64-byte
/// Alloc copy freed afterwards. Returns the minigame's result, 0, for
/// Scene_ReturnFromSequence.
static __inline__ s32 RunLuckyDice(s32 entrance) {
  void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);
  void *saved = Alloc(MINIGAME_PALETTE_BYTES);
  s32 result;

  Dma_Set(palette, saved,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  result = LuckyDice_StartFar(entrance);
  Dma_Set(saved, palette,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  Free(saved);
  return result;
}

/// Runs Lucky Wheels, preserving BG palettes 14 and 15 as
/// RunLuckyDice does. Returns LuckyWheels_Start's result, 0.
static __inline__ s32 RunLuckyWheels(s32 entrance) {
  void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);
  void *saved = Alloc(MINIGAME_PALETTE_BYTES);
  s32 result;

  Dma_Set(palette, saved,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  result = LuckyWheels_StartFar(entrance);
  Dma_Set(saved, palette,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  Free(saved);
  return result;
}

/// The game's sequence manager, which never returns. mode, an enum GameStart,
/// chooses the first scene. Each pass resets the
/// runtime, then either runs a battle or minigame and picks the scene to return
/// to, or loads a scene's code, starts its music and runs it until the
/// party takes an exit. Called with GAME_START_TITLE through
/// SequenceManager_RunFar by System_Initialize at power-on and by Game_Restart.
void SequenceManager_Run(s32 mode) {
  const struct SceneHeader *table;
  const struct SceneHeader *record;
  s32 entrance;

  table = gSceneHeaders;
  // Choose the first scene before initializing the per-scene runtime.
  if (gDebugMode && mode == GAME_START_DEBUG_VALE) {
    gSaveBlock.sceneId = (s32)&SceneId_ValePlaza;
    gSaveBlock.entrance = mode;
  } else if (gDebugMode && mode == GAME_START_DEBUG_MAIN_MENU) {
    gSaveBlock.sceneId = (s32)&SceneId_MainMenu;
    gSaveBlock.entrance = MAIN_MENU_FROM_SANCTUM;
  } else {
    SaveBlock_InitializeDefaultsFar();
    gSaveBlock.sceneId = (s32)&SceneId_Title;
    gSaveBlock.entrance = TITLE_AFTER_RUN;
  }
  // The window colours from the options: hue, then brightness.
  WindowColor_ApplyFar(gSaveBlock.windowColor[0], gSaveBlock.windowColor[1]);
  VramCache_Reset();
  Task_ResetAll();
  Task_ResetAll();
  for (;;) {
    // FLAG_SKIP_SCENE_CHANGE_SOUND skips SE_STOP_ACTION_SE for one scene
    // change, then clears itself.
    if (GameFlag_GetFar(FLAG_SKIP_SCENE_CHANGE_SOUND))
      GameFlag_ClearFar(FLAG_SKIP_SCENE_CHANGE_SOUND);
    else
      Sound_PlaySongFar(SE_STOP_ACTION_SE);
    record = &table[gSaveBlock.sceneId];
    entrance = gSaveBlock.entrance;

    // Stop DMA 0, which a scene may have left running for an HBlank effect.
    {
      volatile u16 *dma0 = REG_DMA0;

      dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
      dma0[5] &= ~DMA_ENABLE;
      dma0[5];
    }
    // Drop the tasks, the HBlank and VCount handlers (IRQs 1 and 2) and every
    // heap block the last scene, battle or minigame left.
    Task_ResetAll();
    Irq_SetHandler(IRQ_HBLANK, 0, 0);
    Irq_SetHandler(IRQ_VCOUNT, 0, 0);
    InitHeap();
    DebugText_Clear();
    VramCache_Reset();
    // A battle or minigame returns a result, not an exit: the return scene is
    // chosen from the save block, and no overlay is loaded.
    if (gSaveBlock.sceneId >= SEQUENCE_NONE) {
      switch (gSaveBlock.sceneId) {
      case SEQUENCE_BATTLE:
        entrance = Battle_RunFar(entrance);
        break;
      case SEQUENCE_LUCKY_DICE:
        entrance = RunLuckyDice(entrance);
        break;
      case SEQUENCE_LUCKY_WHEELS:
        entrance = RunLuckyWheels(entrance);
        break;
      case SEQUENCE_NONE:
        entrance = 0;
        break;
      }
      Scene_ReturnFromSequence(entrance);
      continue;
    }

    // A scene: set its flags and music, load its code and run it.
    Scene_ResetFlagsOnEnter(gSaveBlock.sceneId,
                            GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION));
    Scene_ChooseMusic();
    if (!GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION)) {
      // Either flag keeps the music that is playing: FLAG_KEEP_MUSIC for this
      // one change, FLAG_KEEP_MUSIC_HELD for as long as it stays set.
      if (GameFlag_GetFar(FLAG_KEEP_MUSIC) ||
          GameFlag_GetFar(FLAG_KEEP_MUSIC_HELD))
        GameFlag_ClearFar(FLAG_KEEP_MUSIC);
      else
        Scene_PlayMusic();
    } else if (gSaveBlock.returnMusic != -1) {
      // Back where the party was: resume the music saved when the pause menu
      // last opened (run_scene.c). A random battle and every normal entry set
      // returnMusic to -1, so the scene's own music plays instead.
      Sound_PlaySongFar(gSaveBlock.returnMusic);
    } else {
      Scene_PlayMusic();
    }
    gSaveBlock.mapId = record->mapIndex;
    Scene_LoadOverlay(0);
    Scene_TakeExit(SceneManager_Run(entrance));
  }
}

/// Loads the current scene's overlay code (its resource) into gOverlayArea.
/// Called by SequenceManager_Run before SceneManager_Run.
void Scene_LoadOverlay(s32 unused) {
  const struct SceneHeader *table;

  table = gSceneHeaders;
  Overlay_Load(table[gSaveBlock.sceneId].resourceId, &gOverlayArea);
}

#define MAP_HEADER(id) (&gSceneHeaders[id])

/// Updates the flags and state that last for one scene or one area, on
/// entering the current scene. Unless keep is set (the party returns to where
/// it was, as after a battle), the flags of the last scene are cleared, and on
/// entering a new area so are the area's flags and step
/// counters, the location name is shown and the entrance becomes the
/// point Retreat returns to. Then the area's visited flag
/// (FLAG_AREA_VISITED) is set. Called by SequenceManager_Run before each
/// scene, with keep from FLAG_KEEP_PARTY_POSITION; sets gSaveBlock.area and
/// .sceneType.
void Scene_ResetFlagsOnEnter(s32 unused, s32 keep) {
  s32 area = MAP_HEADER(gSaveBlock.sceneId)->area;
  s32 flag;

  if (keep == 0) {
    // Local puzzle flags expire on every normal entry to a scene.
    for (flag = FLAG_SCENE_TEMP_FIRST; flag <= FLAG_SCENE_TEMP_LAST; flag++)
      GameFlag_ClearFar(flag);
    if (area != gSaveBlock.area) {
      // Area state and Retreat's destination survive moves within it.
      for (flag = FLAG_AREA_TEMP_FIRST; flag <= FLAG_AREA_TEMP_LAST; flag++)
        GameFlag_ClearFar(flag);
      GameFlag_SetFar(FLAG_SHOW_LOCATION_NAME);
      gSaveBlock.encounterSteps = 0;
      gSaveBlock.hazardSteps = 0;
      GameFlag_ClearFar(FLAG_VENUS_LIGHTHOUSE_LIT);
      GameFlag_ClearFar(FLAG_MERCURY_LIGHTHOUSE_LIT);
      GameFlag_ClearFar(FLAG_MARS_LIGHTHOUSE_LIT);
      GameFlag_ClearFar(FLAG_JUPITER_LIGHTHOUSE_LIT);
      gSaveBlock.retreatScene = gSaveBlock.sceneId;
      gSaveBlock.retreatEntrance = gSaveBlock.entrance;
    }
    // The Djinn's hiding flags last for one scene, even within an area,
    // and so do the 16 after them to 0xdf, which nothing else uses.
    for (flag = FLAG_DJINN_HIDING_FIRST; flag <= 0xdf; flag++)
      GameFlag_ClearFar(flag);
    GameFlag_ClearFar(FLAG_LINK_BATTLE);
    GameFlag_ClearFar(FLAG_RETREAT_BLOCKED);
    // FLAG_NO_ENCOUNTERS_IN_AREA stops random battles
    // (Encounter_CountStepsAndSelectFormation).
    GameFlag_ClearFar(FLAG_NO_ENCOUNTERS_IN_AREA);
    GameFlag_ClearFar(FLAG_IN_TOWN);
    GameFlag_ClearFar(FLAG_WORLD_MAP_LEFT);
    gSaveBlock.returnMusic = -1;
  }
  gSaveBlock.area = area;
  GameFlag_SetFar(FLAG_AREA_VISITED(area & 0x7f));
  gSaveBlock.sceneType = MAP_HEADER(gSaveBlock.sceneId)->sceneType;
  if (gSaveBlock.sceneType == SCENE_TYPE_TOWN)
    GameFlag_SetFar(FLAG_IN_TOWN);
  GameFlag_RefreshLureCapFar();
}

/// Starts the current scene's field music, chosen by
/// Scene_ChooseMusic. Called by SequenceManager_Run, and by
/// 34 scene overlays through Scene_PlayMusicFar.
void Scene_PlayMusic(void) { Sound_PlaySongFar(gSaveBlock.sceneMusic); }

#undef EXIT_MAP_MASK
#undef ENTRANCE
#undef EXIT_ID
#undef EXIT_HAS_FLAG
#undef EXIT_TABLE_END
#undef EXIT_ID_ANY
#undef MAP_HEADER
