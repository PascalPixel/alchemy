/// @file
/// Scene changes: exits, returns from battle, the main loop, entering.
// Part of object.c.

#include "../object_internal.h"

// The scene overlay's exit table: a header word holding a scene id, then that
// scene's exits, one packed word each (and a flag word after it when
// EXIT_HAS_FLAG is set). Scene id EXIT_TABLE_END ends the exits of a scene and
// the whole table. SCENE_EXIT and SCENE_EXIT_IF (field_scene.h) give an exit
// word's bits.
#define EXIT_SCENE_MASK 0xfff
#define EXIT_ENTRANCE(word) (((word) & 0xff000) >> 12)
#define EXIT_KIND(word) (((word) & 0x0ff00000) >> 20)
#define EXIT_HAS_FLAG 0x10000000
#define EXIT_TABLE_END 0x1ff
#define EXIT_KIND_ANY 0xff

/// Chooses the scene and entrance to go to when the party leaves the current
/// scene. exitKind is the kind of exit Field_RunScene ended on; the scene's
/// exit table gives the first exit of that kind (or of any kind) whose flag, if
/// it has one, is not set. An exit of kind 0 or the end of the scene's exits
/// falls back to the scene of its first exit, at entrance 0. Called by
/// Game_ResetForNewGame with Field_RunScene's result; sets gGameState.scene and
/// .entrance, and leaves them alone for EXIT_NONE, whose destination is set
/// already, or for a scene with no header in the table.
void Scene_TakeExit(s32 exitKind) {
  s16 currentScene;
  const u32 *exitTable;
  const u32 *firstExit;
  u32 word;
  s32 scene;
  s32 nextScene;
  s32 nextEntrance;
  s32 entrance;
  s32 kind;
  s32 flag;

  currentScene = gGameState.scene;
  exitTable = gOverlayArea.exits();
  nextScene = EXIT_NONE;
  nextEntrance = 0;
  if (exitKind == EXIT_NONE)
    return;

  for (;;) {
    // Find the header of the current scene: exit words have high bits set.
    scene = *exitTable++;
    if (scene & ~EXIT_SCENE_MASK)
      continue;
    scene &= EXIT_SCENE_MASK;
    if (scene == EXIT_TABLE_END)
      break;
    if (scene != currentScene)
      continue;

    firstExit = exitTable;
    for (;;) {
      word = *exitTable++;
      scene = word & EXIT_SCENE_MASK;
      entrance = EXIT_ENTRANCE(word);
      kind = EXIT_KIND(word);
      flag = word & EXIT_HAS_FLAG;
      if (flag)
        flag = *exitTable++;
      if (scene == EXIT_TABLE_END || kind == 0) {
        nextScene = *firstExit & 0x1ff; // only 9 bits, unlike EXIT_SCENE_MASK
        break;
      }
      if (kind == EXIT_KIND_ANY || kind == exitKind) {
        if (flag == 0 || GameFlag_TestFar(flag) == 0) {
          nextScene = scene;
          nextEntrance = entrance;
          break;
        }
      }
    }
    break;
  }

  // Commit the chosen exit only after a matching scene header was found.
  if (nextScene != EXIT_NONE) {
    gGameState.scene = nextScene;
    gGameState.entrance = nextEntrance;
  }
}

/// Chooses where the game continues after a battle or another scene that is
/// not a field map (see Game_ResetForNewGame). result is that scene's result:
/// -1 when the party was defeated, which revives the leader with 1 HP (and,
/// with FLAG_CHILDHOOD, fully heals Isaac and Garet, characters 0 and 1) and
/// returns to the defeat point, or to the last saved point when none is set.
/// Any other result goes to the next scene the event set, or to the default
/// scene (the field scene the battle or minigame was started from) with the
/// party keeping its position (FLAG_KEEP_PARTY_POSITION). Called by
/// Game_ResetForNewGame after each scene from SCENE_BLANK on; stores result in
/// gGameState.sceneChangeReason and sets gGameState.scene and .entrance.
void Scene_ReturnFromBattle(s32 result) {
  struct BattleUnit *unit;
  s32 i;
  s32 scene;
  s32 entrance;

  gGameState.sceneChangeReason = result;
  if (result == -1) {
    // Revive the leader before choosing the defeat return point.
    unit = UnitGetFar(gGameState.selectedActor);
    if (unit->hp == 0) {
      unit->hp = 1;
      Unit_UpdateGauges(unit);
    }
    if (GameFlag_TestFar(FLAG_CHILDHOOD)) {
      for (i = 0; i < 2; i++) {
        unit = UnitGetFar(i);
        unit->hp = unit->maxHp;
        unit->pp = unit->maxPp;
        Unit_UpdateGauges(unit);
      }
    }
    scene = gGameState.defeatScene;
    entrance = gGameState.defeatEntrance;
    if (scene == -1 && entrance == -1) {
      gGameState.scene = gGameState.savedScene;
      gGameState.entrance = gGameState.savedEntrance;
    } else {
      if (scene != -1)
        gGameState.scene = scene;
      else
        gGameState.scene = gGameState.defaultScene;
      if (entrance != -1)
        gGameState.entrance = entrance;
      else
        gGameState.entrance = gGameState.defaultEntrance;
    }
  } else {
    // An explicit event destination overrides the scene's default return.
    scene = gGameState.nextScene;
    entrance = gGameState.nextEntrance;
    if (scene != -1 || entrance != -1) {
      if (scene != -1)
        gGameState.scene = scene;
      else
        gGameState.scene = gGameState.defaultScene;
      if (entrance != -1)
        gGameState.entrance = entrance;
      else
        gGameState.entrance = gGameState.defaultEntrance;
    } else {
      gGameState.scene = gGameState.defaultScene;
      gGameState.entrance = gGameState.defaultEntrance;
      GameFlag_SetBitFar(FLAG_KEEP_PARTY_POSITION);
    }
  }
}

/// Returns the group scene belongs to. The scenes of one place share a group,
/// which selects their music, area name and the flags kept while inside it.
/// Called by Scene_ResolveInteractionResult.
s32 BattleFx_GetResourceGroup(s32 scene) {
  return gField_SceneTable[scene].group;
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
/// Scene_ReturnFromBattle.
static __inline__ s32 RunLuckyDice(s32 entrance) {
  void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);
  void *saved = MemAlloc(MINIGAME_PALETTE_BYTES);
  s32 result;

  Dma_Set(palette, saved,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  result = Runtime_BlankDisplayAndRunFar(entrance);
  Dma_Set(saved, palette,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  MemFree(saved);
  return result;
}

/// Runs the slot machine (Lucky Wheels), preserving BG palettes 14 and 15 as
/// RunLuckyDice does. Returns ReelGame_Start's result, 0.
static __inline__ s32 RunReelGame(s32 entrance) {
  void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);
  void *saved = MemAlloc(MINIGAME_PALETTE_BYTES);
  s32 result;

  Dma_Set(palette, saved,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  result = ReelGame_StartFar(entrance);
  Dma_Set(saved, palette,
          (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),
          REG_DMA3);
  MemFree(saved);
  return result;
}

/// The game's main loop, which never returns. mode chooses the first scene: 0
/// starts at the title screen with a fresh game state; in debug mode, 1 starts
/// in Isaac's home in Vale and 2 at the clear screen. Each pass resets the
/// runtime, then either runs a battle or minigame and picks the scene to return
/// to, or loads a field scene's code, starts its music and runs it until the
/// party takes an exit. Called with mode 0 through Game_ResetForNewGameFar by
/// System_Initialize at power-on and by Game_Restart.
void Game_ResetForNewGame(s32 mode) {
  const struct SceneRecord *table;
  const struct SceneRecord *record;
  s32 entrance;

  table = gField_SceneTable;
  // Choose the first scene before initializing the per-scene runtime.
  if (gDebugMode && mode == 1) {
    gGameState.scene = (s32)&SceneId_ValeHome;
    gGameState.entrance = mode;
  } else if (gDebugMode && mode == 2) {
    gGameState.scene = (s32)&SceneId_Clear;
    gGameState.entrance = 1;
  } else {
    GameState_InitDefaultsFar();
    gGameState.scene = (s32)&SceneId_Title;
    gGameState.entrance = 2;
  }
  // The window colours from the options: hue, then brightness.
  PaletteGlow_UpdateFar(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
  VramCache_Reset();
  Scheduler_ResetTaskTable();
  Scheduler_ResetTaskTable();
  for (;;) {
    // Flag 0x101 (FLAG_SKIP_SCENE_CHANGE_SOUND) skips SE_288 for one scene
    // change, then clears itself.
    if (GameFlag_TestFar(0x101))
      GameFlag_ClearBitFar(0x101);
    else
      AudioCommand_PlayFar(SE_288);
    record = &table[gGameState.scene];
    entrance = gGameState.entrance;

    // Stop DMA 0, which a scene may have left running for an HBlank effect.
    {
      volatile u16 *dma0 = REG_DMA0;

      dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
      dma0[5] &= ~DMA_ENABLE;
      dma0[5];
    }
    // Drop the tasks, the HBlank and VCount handlers (IRQs 1 and 2) and every
    // heap block the last scene, battle or minigame left.
    Scheduler_ResetTaskTable();
    Runtime_SetIrqHandler(1, 0, 0);
    Runtime_SetIrqHandler(2, 0, 0);
    Runtime_InitializeHeap();
    DebugText_Clear();
    VramCache_Reset();
    // A battle or minigame returns a result, not an exit: the return scene is
    // chosen from the game state, and no overlay is loaded.
    if (gGameState.scene >= SCENE_BLANK) {
      switch (gGameState.scene) {
      case SCENE_BATTLE:
        entrance = Battle_RunEncounterFar(entrance);
        break;
      case SCENE_LUCKY_DICE:
        entrance = RunLuckyDice(entrance);
        break;
      case SCENE_REEL_GAME:
        entrance = RunReelGame(entrance);
        break;
      case SCENE_BLANK:
        entrance = 0;
        break;
      }
      Scene_ReturnFromBattle(entrance);
      continue;
    }

    // A field scene: set its flags and music, load its code and run it.
    Scene_ResetFlagsOnEnter(gGameState.scene,
                            GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION));
    Scene_ResolveInteractionResult();
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
      // last opened (run_scene.c). A random battle and every normal entry set
      // returnCue to -1, so the scene's own music plays instead.
      AudioCommand_PlayFar(gGameState.returnCue);
    } else {
      Audio_PlayCueFromEventWork();
    }
    gGameState.map = record->mapIndex;
    Scene_LoadOverlay(0);
    Scene_TakeExit(Field_RunScene(entrance));
  }
}

/// Loads the current scene's overlay code (its resource) into gOverlayArea.
/// Called by Game_ResetForNewGame before Field_RunScene.
void Scene_LoadOverlay(s32 unused) {
  const struct SceneRecord *table;

  table = gField_SceneTable;
  Resource_LoadCode(table[gGameState.scene].resourceId, &gOverlayArea);
}

#define SCENE_RECORD(id) (&gField_SceneTable[id])

/// Updates the flags and state that last for one scene or one group of scenes,
/// on entering the current scene. Unless keep is set (the party returns to
/// where it was, as after a battle), the flags of the last scene are cleared,
/// and on entering a new group so are the group's flags and step counters, the
/// location name is shown and the entrance becomes the point Retreat returns
/// to. Then the group's visited flag (0x180 + group) is set. Called by
/// Game_ResetForNewGame before each field scene, with keep from
/// FLAG_KEEP_PARTY_POSITION; sets gGameState.sceneGroup and .sceneVariant.
void Scene_ResetFlagsOnEnter(s32 unused, s32 keep) {
  s32 group = SCENE_RECORD(gGameState.scene)->group;
  s32 flag;

  if (keep == 0) {
    // Local puzzle flags expire on every normal scene entrance.
    for (flag = 0x200; flag <= 0x2ff; flag++)
      GameFlag_ClearBitFar(flag);
    if (group != gGameState.sceneGroup) {
      // Group state and Retreat's destination survive moves within a place.
      for (flag = 0x300; flag <= 0x3ff; flag++)
        GameFlag_ClearBitFar(flag);
      GameFlag_SetBitFar(FLAG_SHOW_LOCATION_NAME);
      gGameState.encounterSteps = 0;
      gGameState.hazardSteps = 0;
      GameFlag_ClearBitFar(FLAG_VENUS_LIGHTHOUSE_LIT);
      GameFlag_ClearBitFar(FLAG_MERCURY_LIGHTHOUSE_LIT);
      GameFlag_ClearBitFar(FLAG_MARS_LIGHTHOUSE_LIT);
      GameFlag_ClearBitFar(FLAG_JUPITER_LIGHTHOUSE_LIT);
      gGameState.retreatScene = gGameState.scene;
      gGameState.retreatEntrance = gGameState.entrance;
    }
    // Flags 0x80 to 0xdf last for one scene, even within a group.
    for (flag = 0x80; flag <= 0xdf; flag++)
      GameFlag_ClearBitFar(flag);
    GameFlag_ClearBitFar(FLAG_LINK_BATTLE);
    GameFlag_ClearBitFar(FLAG_RETREAT_BLOCKED);
    // Flag 0x161 (353) stops random battles (Encounter_SelectEnemyGroup).
    GameFlag_ClearBitFar(0x161);
    GameFlag_ClearBitFar(0x123);
    GameFlag_ClearBitFar(FLAG_NOT_ON_MAP);
    gGameState.returnCue = -1;
  }
  gGameState.sceneGroup = group;
  GameFlag_SetBitFar((group & 0x7f) + 0x180);
  // Flag 0x123 (FLAG_SCENE_VARIANT_2) marks a scene of variant 2; see
  // SceneRecord.variant (field_runtime.h) for which scenes those are.
  gGameState.sceneVariant = SCENE_RECORD(gGameState.scene)->variant;
  if (gGameState.sceneVariant == 2)
    GameFlag_SetBitFar(0x123);
  GameFlag_RefreshLureCapFar();
}

/// Starts the current scene's field music, chosen by
/// Scene_ResolveInteractionResult. Called by Game_ResetForNewGame, and by
/// 34 scene overlays through Audio_PlayCueFromEventWorkFar.
void Audio_PlayCueFromEventWork(void) { AudioCommand_PlayFar(gGameState.cue); }

#undef EXIT_SCENE_MASK
#undef EXIT_ENTRANCE
#undef EXIT_KIND
#undef EXIT_HAS_FLAG
#undef EXIT_TABLE_END
#undef EXIT_KIND_ANY
#undef SCENE_RECORD
