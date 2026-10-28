/// @file
/// Scene changes: exits, returns from battle, the main routine, entering.
// Part of overworld.c.

#include "../overworld_internal.h"

// The scene overlay's exit table: a header word holding a scene id, then that
// scene's exits, one packed word each (and a flag word after it when
// EXIT_HAS_FLAG is set). Scene id EXIT_TABLE_END ends the exits of a scene and
// the whole table. SCENE_EXIT and SCENE_EXIT_UNLESS (scene_tables.h) give an
// exit word's bits.
#define EXIT_SCENE_MASK 0xfff
#define ENTRANCE(word) (((word) & 0xff000) >> 12)
#define EXIT_ID(word) (((word) & 0x0ff00000) >> 20)
#define EXIT_HAS_FLAG 0x10000000
#define EXIT_TABLE_END 0x1ff
#define EXIT_ID_ANY 0xff

/// Chooses the scene and entrance to go to when the party leaves the
/// current scene. exitId is the kind of exit FieldSequence_Run ended on; the
/// scene's exit table gives the first exit of that kind (or of any kind) whose
/// flag, if it has one, is not set. An exit of kind 0 or the end of the scene's
/// exits falls back to the scene of its first exit, at entrance 0.
/// Called by MainRoutine_Run with FieldSequence_Run's result; sets
/// gMainRoutineWork.sceneId and .entrance, and leaves them alone for
/// EXIT_KEEP_DESTINATION, whose destination is set already, or for a scene with
/// no header in the table.
void Scene_TakeExit(s32 exitId) {
  s16 currentMap;
  const u32 *exitTable;
  const u32 *firstExit;
  u32 word;
  s32 sceneId;
  s32 destinationScene;
  s32 destinationEntrance;
  s32 entrance;
  s32 kind;
  s32 flag;

  currentMap = gMainRoutineWork.sceneId;
  exitTable = gOverlayArea.exits();
  destinationScene = EXIT_KEEP_DESTINATION;
  destinationEntrance = 0;
  if (exitId == EXIT_KEEP_DESTINATION)
    return;

  for (;;) {
    // Find the header of the current scene: exit words have high bits set.
    sceneId = *exitTable++;
    if (sceneId & ~EXIT_SCENE_MASK)
      continue;
    sceneId &= EXIT_SCENE_MASK;
    if (sceneId == EXIT_TABLE_END)
      break;
    if (sceneId != currentMap)
      continue;

    firstExit = exitTable;
    for (;;) {
      word = *exitTable++;
      sceneId = word & EXIT_SCENE_MASK;
      entrance = ENTRANCE(word);
      kind = EXIT_ID(word);
      flag = word & EXIT_HAS_FLAG;
      if (flag)
        flag = *exitTable++;
      if (sceneId == EXIT_TABLE_END || kind == 0) {
        destinationScene =
            *firstExit & 0x1ff; // only 9 bits, unlike EXIT_SCENE_MASK
        break;
      }
      if (kind == EXIT_ID_ANY || kind == exitId) {
        if (flag == 0 || GameFlag_GetFar(flag) == 0) {
          destinationScene = sceneId;
          destinationEntrance = entrance;
          break;
        }
      }
    }
    break;
  }

  // Commit the chosen exit only after a matching scene was found.
  if (destinationScene != EXIT_KEEP_DESTINATION) {
    gMainRoutineWork.sceneId = destinationScene;
    gMainRoutineWork.entrance = destinationEntrance;
  }
}

/// Chooses where the game continues after a sequence other than the field
/// sequence (see MainRoutine_Run). result is its result:
/// -1 when the party was defeated, which revives the leader with 1 HP (and,
/// with FLAG_CHILDHOOD, fully heals Isaac and Garet, characters 0 and 1) and
/// returns to the story battle's defeat point
/// (Event_SetDefeatDestination) or, when none is set, to the restart
/// point, usually the last Sanctum (Scene_SetRestartPoint). Any other
/// result goes to the after-battle destination an event set, or to the return
/// point (the scene the battle or minigame was started from) with the party
/// keeping its position (FLAG_KEEP_PARTY_POSITION). Called by
/// MainRoutine_Run after each sequence other than the field sequence (the
/// ids from SEQUENCE_NONE up); stores result in
/// gMainRoutineWork.sequenceResult and sets gMainRoutineWork.sceneId and
/// .entrance.
void Scene_ReturnFromSequence(s32 result) {
  struct Unit *unit;
  s32 i;
  s32 sceneId;
  s32 entrance;

  gMainRoutineWork.sequenceResult = result;
  if (result == -1) {
    // Revive the leader before choosing the restart point.
    unit = Unit_GetFar(gMainRoutineWork.leaderActorId);
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
    sceneId = gMainRoutineWork.defeatScene;
    entrance = gMainRoutineWork.defeatEntrance;
    if (sceneId == -1 && entrance == -1) {
      gMainRoutineWork.sceneId = gMainRoutineWork.restartScene;
      gMainRoutineWork.entrance = gMainRoutineWork.restartEntrance;
    } else {
      if (sceneId != -1)
        gMainRoutineWork.sceneId = sceneId;
      else
        gMainRoutineWork.sceneId = gMainRoutineWork.returnScene;
      if (entrance != -1)
        gMainRoutineWork.entrance = entrance;
      else
        gMainRoutineWork.entrance = gMainRoutineWork.returnEntrance;
    }
  } else {
    // An after-battle destination an event set overrides the return point.
    sceneId = gMainRoutineWork.afterBattleScene;
    entrance = gMainRoutineWork.afterBattleEntrance;
    if (sceneId != -1 || entrance != -1) {
      if (sceneId != -1)
        gMainRoutineWork.sceneId = sceneId;
      else
        gMainRoutineWork.sceneId = gMainRoutineWork.returnScene;
      if (entrance != -1)
        gMainRoutineWork.entrance = entrance;
      else
        gMainRoutineWork.entrance = gMainRoutineWork.returnEntrance;
    } else {
      gMainRoutineWork.sceneId = gMainRoutineWork.returnScene;
      gMainRoutineWork.entrance = gMainRoutineWork.returnEntrance;
      GameFlag_SetFar(FLAG_KEEP_PARTY_POSITION);
    }
  }
}

/// Returns the area sceneId belongs to. The scenes of one area
/// share their music, area name and the flags kept while inside it. Called by
/// Area_GetNameIndex.
s32 Scene_GetArea(s32 sceneId) { return gScenes[sceneId].area; }

/// The last two 16-colour background palettes, which the minigame runners save
/// and restore around a minigame; palette 15 holds the window colours
/// (WindowColor_Apply).
enum {
  MINIGAME_PALETTE_FIRST = 14, ///< BG palette 14
  MINIGAME_PALETTE_BYTES = 64  ///< palettes 14 and 15: 2 x 16 colours x 2 bytes
};

/// Runs the dice sequence (Lucky Dice), preserving BG palettes 14 and 15 in a
/// 64-byte Heap_Alloc copy freed afterwards. Returns the minigame's result, 0,
/// for Scene_ReturnFromSequence.
static __inline__ s32 MainRoutine_RunDice(s32 entrance) {
  void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);
  void *saved = Heap_Alloc(MINIGAME_PALETTE_BYTES);
  s32 result;

  Dma_Set(palette, saved,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  result = LuckyDice_StartFar(entrance);
  Dma_Set(saved, palette,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  Heap_Release(saved);
  return result;
}

/// Runs the slots sequence (Lucky Wheels), preserving BG palettes 14 and 15 as
/// MainRoutine_RunDice does. Returns LuckyWheels_Start's result, 0.
static __inline__ s32 MainRoutine_RunSlots(s32 entrance) {
  void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);
  void *saved = Heap_Alloc(MINIGAME_PALETTE_BYTES);
  s32 result;

  Dma_Set(palette, saved,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  result = LuckyWheels_StartFar(entrance);
  Dma_Set(saved, palette,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  Heap_Release(saved);
  return result;
}

/// The main routine: runs one sequence after another and never returns.
/// mode, an enum GameStart, chooses the first scene. Each pass resets the
/// runtime, then either runs the battle, dice or slots sequence and picks the
/// scene to return to, or runs the field sequence: loads the scene's code,
/// starts its music and runs it until the party takes an exit. Called with
/// GAME_START_TITLE through MainRoutine_RunFar by System_Initialize at
/// power-on and by Game_RestartQuest.
void MainRoutine_Run(s32 mode) {
  const struct Scene *table;
  const struct Scene *record;
  s32 entrance;

  table = gScenes;
  // Choose the first scene before initializing the per-scene runtime.
  if (gDebugMode && mode == GAME_START_DEBUG_VALE) {
    gMainRoutineWork.sceneId = (s32)&SceneId_ValePlaza;
    gMainRoutineWork.entrance = mode;
  } else if (gDebugMode && mode == GAME_START_DEBUG_FILE_SCREEN) {
    gMainRoutineWork.sceneId = (s32)&SceneId_FileScreen;
    gMainRoutineWork.entrance = FILE_SCREEN_FROM_SANCTUM;
  } else {
    MainRoutineWork_InitializeDefaultsFar();
    gMainRoutineWork.sceneId = (s32)&SceneId_Title;
    gMainRoutineWork.entrance = TITLE_AFTER_RUN;
  }
  // The window colours from the options: hue, then brightness.
  WindowColor_ApplyFar(gMainRoutineWork.windowColor[0],
                       gMainRoutineWork.windowColor[1]);
  VramCache_Reset();
  Task_ResetAll();
  Task_ResetAll();
  for (;;) {
    // FLAG_SKIP_SCENE_CHANGE_SOUND skips SE_STOP_ACTION_SE for one scene
    // change, then clears itself.
    if (GameFlag_GetFar(FLAG_SKIP_SCENE_CHANGE_SOUND))
      GameFlag_ClearFar(FLAG_SKIP_SCENE_CHANGE_SOUND);
    else
      Sound_PlayFar(SE_STOP_ACTION_SE);
    record = &table[gMainRoutineWork.sceneId];
    entrance = gMainRoutineWork.entrance;

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
    Heap_Initialize();
    DebugText_Clear();
    VramCache_Reset();
    // The battle, dice and slots sequences return a result, not an exit: the
    // return scene is chosen from the main routine's work, and no overlay is
    // loaded.
    if (gMainRoutineWork.sceneId >= SEQUENCE_NONE) {
      switch (gMainRoutineWork.sceneId) {
      case SEQUENCE_BATTLE:
        entrance = Battle_RunFar(entrance);
        break;
      case SEQUENCE_DICE:
        entrance = MainRoutine_RunDice(entrance);
        break;
      case SEQUENCE_SLOTS:
        entrance = MainRoutine_RunSlots(entrance);
        break;
      case SEQUENCE_NONE:
        entrance = 0;
        break;
      }
      Scene_ReturnFromSequence(entrance);
      continue;
    }

    // A scene: set its flags and music, load its code and run it.
    Scene_Arrive(gMainRoutineWork.sceneId,
                 GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION));
    Scene_ChooseMusic();
    if (!GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION)) {
      // Either flag keeps the music that is playing: FLAG_KEEP_MUSIC_ONCE for
      // this one change, FLAG_KEEP_MUSIC_HELD for as long as it stays set.
      if (GameFlag_GetFar(FLAG_KEEP_MUSIC_ONCE) ||
          GameFlag_GetFar(FLAG_KEEP_MUSIC_HELD))
        GameFlag_ClearFar(FLAG_KEEP_MUSIC_ONCE);
      else
        Scene_PlayMusic();
    } else if (gMainRoutineWork.returnMusic != -1) {
      // Back where the party was: resume the music saved when the Pause menu
      // last opened (field_sequence.c). A random battle and every normal entry
      // set returnMusic to -1, so the scene's own music plays instead.
      Sound_PlayFar(gMainRoutineWork.returnMusic);
    } else {
      Scene_PlayMusic();
    }
    gMainRoutineWork.mapId = record->mapIndex;
    Scene_LoadOverlay(0);
    Scene_TakeExit(FieldSequence_Run(entrance));
  }
}

/// Loads the current scene's overlay code (its resource) into gOverlayArea.
/// Called by MainRoutine_Run before FieldSequence_Run.
void Scene_LoadOverlay(s32 unused) {
  const struct Scene *table;

  table = gScenes;
  Overlay_Load(table[gMainRoutineWork.sceneId].resourceId, &gOverlayArea);
}

#define MAP_HEADER(id) (&gScenes[id])

/// Updates the flags and state that last for one scene or one area, on
/// entering the current scene. Unless keep is set (the party returns to where
/// it was, as after a battle), the flags of the last scene are cleared, and on
/// entering a new area so are the area's flags and step
/// counters, the area name is shown and the entrance becomes the
/// point Retreat returns to. Then the area's visited flag
/// (FLAG_AREA_VISITED) is set. Called by MainRoutine_Run before each
/// scene, with keep from FLAG_KEEP_PARTY_POSITION; sets
/// gMainRoutineWork.area and .sceneType.
void Scene_Arrive(s32 unused, s32 keep) {
  s32 area = MAP_HEADER(gMainRoutineWork.sceneId)->area;
  s32 flag;

  if (keep == 0) {
    // Local puzzle flags expire on every normal entry to a scene.
    for (flag = FLAG_SCENE_TEMP_FIRST; flag <= FLAG_SCENE_TEMP_LAST; flag++)
      GameFlag_ClearFar(flag);
    if (area != gMainRoutineWork.area) {
      // Area state and Retreat's destination survive moves within it.
      for (flag = FLAG_AREA_TEMP_FIRST; flag <= FLAG_AREA_TEMP_LAST; flag++)
        GameFlag_ClearFar(flag);
      GameFlag_SetFar(FLAG_SHOW_AREA_NAME);
      gMainRoutineWork.encounterSteps = 0;
      gMainRoutineWork.hazardSteps = 0;
      GameFlag_ClearFar(FLAG_VENUS_LIGHTHOUSE_LIT);
      GameFlag_ClearFar(FLAG_MERCURY_LIGHTHOUSE_LIT);
      GameFlag_ClearFar(FLAG_MARS_LIGHTHOUSE_LIT);
      GameFlag_ClearFar(FLAG_JUPITER_LIGHTHOUSE_LIT);
      gMainRoutineWork.retreatScene = gMainRoutineWork.sceneId;
      gMainRoutineWork.retreatEntrance = gMainRoutineWork.entrance;
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
    gMainRoutineWork.returnMusic = -1;
  }
  gMainRoutineWork.area = area;
  GameFlag_SetFar(FLAG_AREA_VISITED(area & 0x7f));
  gMainRoutineWork.sceneType = MAP_HEADER(gMainRoutineWork.sceneId)->sceneType;
  if (gMainRoutineWork.sceneType == SCENE_TYPE_TOWN)
    GameFlag_SetFar(FLAG_IN_TOWN);
  GameFlag_RefreshLureCapFar();
}

/// Starts the current scene's field music, chosen by
/// Scene_ChooseMusic. Called by MainRoutine_Run, and by
/// 34 scene overlays through Scene_PlayMusicFar.
void Scene_PlayMusic(void) { Sound_PlayFar(gMainRoutineWork.sceneMusic); }

#undef EXIT_SCENE_MASK
#undef ENTRANCE
#undef EXIT_ID
#undef EXIT_HAS_FLAG
#undef EXIT_TABLE_END
#undef EXIT_ID_ANY
#undef MAP_HEADER
