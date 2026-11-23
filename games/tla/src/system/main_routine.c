/// @file
/// The main routine and scene changes: exits, returns from a sequence,
/// entering a scene. As The Broken Seal's main_routine.c, in the same order;
/// in bank 04 it follows the field sequence's picture decoders.

#include "flags.h"
#include "scene_ids.h"
#include "field/scene_tables.h"
#include "sounds.h"
#include "types.h"
#include "battle/battle_unit.h"
#include "field/field.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/lucky_dice.h"
#include "gamepak/far_call/window.h"
#include "sound/sound.h"
#include "system/dma.h"
#include "system/frame.h"
#include "system/heap_release.h"
#include "system/io_reg.h"
#include "system/irq.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "system/task.h"

extern struct SceneEntries gOverlayArea;

struct Unit *Unit_Get(u32 unitId);
s32 FieldSequence_Run(s32 entrance);

// The scene overlay's exit table: a header word holding a scene id, then that
// scene's exits, one packed word each (and a flag word after it when
// EXIT_HAS_FLAG is set). Scene id SCENE_EXITS_END ends the exits of a scene and
// the whole table.

/// The scene of a table's first exit word, the destination when no exit
/// matches: nine bits, where EXIT_SCENE_MASK takes twelve.
#define EXIT_DEFAULT_SCENE_MASK 0x1ff

/// Chooses the scene and entrance to go to when the party leaves the current
/// scene. exitId is the kind of exit FieldSequence_Run ended on; the scene's
/// exit table gives the first exit of that kind (or of any kind) whose flag, if
/// it has one, is not set. As The Broken Seal's, where a flag word of -1 also
/// takes the exit.
void Scene_TakeExit(s32 exitId) {
  s16 currentScene;
  const u32 *exitTable;
  const u32 *firstExit;
  u32 word;
  s32 sceneId;
  s32 destinationScene;
  s32 destinationEntrance;
  s32 entrance;
  s32 kind;
  s32 flag;

  currentScene = gMainRoutineWork.next;
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
    if (sceneId == SCENE_EXITS_END)
      break;
    if (sceneId != currentScene)
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
      if (sceneId == SCENE_EXITS_END || kind == 0) {
        destinationScene = *firstExit & EXIT_DEFAULT_SCENE_MASK;
        break;
      }
      if (kind == EXIT_ID_ANY || kind == exitId) {
        // As one condition the two flag tests fold into a range check.
        if (flag != 0) {
          if (flag != -1) {
            if (GameFlag_Get(flag))
              continue;
          }
        }
        destinationScene = sceneId;
        destinationEntrance = entrance;
        break;
      }
    }
    break;
  }

  // Commit the chosen exit only after a matching scene was found.
  if (destinationScene != EXIT_KEEP_DESTINATION) {
    gMainRoutineWork.next = destinationScene;
    gMainRoutineWork.entrance = destinationEntrance;
  }
}

/// Chooses where the game continues after a sequence other than the field
/// sequence (see MainRoutine_Run). result is its result: -1 when the party was
/// defeated, which revives the leader with 1 HP and returns to the story
/// battle's defeat point (Event_SetDefeatDestination, defeatScene) or, when
/// none is set, to the restart point, usually the last Sanctum
/// (Scene_SetRestartPoint, savedScene); the world map is then told the scene
/// was left (WorldMap_SetReturnLeavingScene). As The Broken Seal's, with no
/// childhood healing.
void Scene_ReturnFromSequence(s32 result) {
  struct Unit *unit;
  s16 sceneId;
  s16 entrance;

  gMainRoutineWork.sequenceResult = result;
  if (result == -1) {
    // Revive the leader before choosing the restart point.
    unit = Unit_Get(gMainRoutineWork.leaderActorId);
    if (unit->hp == 0) {
      unit->hp = 1;
      Unit_UpdateGauges(unit);
    }
    sceneId = gMainRoutineWork.defeatScene;
    entrance = gMainRoutineWork.defeatEntrance;
    if (sceneId != -1 || entrance != -1) {
      if (sceneId != -1)
        gMainRoutineWork.next = sceneId;
      else
        gMainRoutineWork.next = gMainRoutineWork.returnScene;
      if (entrance != -1)
        gMainRoutineWork.entrance = entrance;
      else
        gMainRoutineWork.entrance = gMainRoutineWork.returnEntrance;
    } else {
      gMainRoutineWork.next = gMainRoutineWork.restartScene;
      gMainRoutineWork.entrance = gMainRoutineWork.restartEntrance;
    }
    WorldMap_SetReturnLeavingScene(gMainRoutineWork.next,
                                   gMainRoutineWork.entrance);
  } else {
    // An after-battle destination an event set overrides the return point.
    sceneId = gMainRoutineWork.afterBattleScene;
    entrance = gMainRoutineWork.afterBattleEntrance;
    if (sceneId != -1 || entrance != -1) {
      if (sceneId != -1)
        gMainRoutineWork.next = sceneId;
      else
        gMainRoutineWork.next = gMainRoutineWork.returnScene;
      if (entrance != -1)
        gMainRoutineWork.entrance = entrance;
      else
        gMainRoutineWork.entrance = gMainRoutineWork.returnEntrance;
    } else {
      gMainRoutineWork.next = gMainRoutineWork.returnScene;
      gMainRoutineWork.entrance = gMainRoutineWork.returnEntrance;
      GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
    }
  }
}

/// Returns the area sceneId belongs to. The scenes of one area share their
/// music, area name and the flags kept while inside it.
s32 Scene_GetArea(s32 sceneId) { return gScenes[sceneId].area; }

/// Returns the scene's attributes, the last halfword of its gScenes row, which
/// The Broken Seal leaves 0; the field sequence splits it into two nibbles.
s16 Field_GetSceneAttributes(s32 sceneId) {
  return gScenes[sceneId].attributes;
}

/// The last two 16-colour background palettes, which the minigame runners save
/// and restore around a minigame; palette 15 holds the window colours
/// (WindowColor_Apply).
enum {
  MINIGAME_PALETTE_FIRST = 14, ///< BG palette 14
  MINIGAME_PALETTE_BYTES = 64  ///< palettes 14 and 15: 2 x 16 colours x 2 bytes
};

/// Runs a minigame's sequence, preserving BG palettes 14 and 15 in a 64-byte
/// Heap_Alloc copy freed afterwards, and stores its start routine's result for
/// Scene_ReturnFromSequence. The Broken Seal writes the same as an inline
/// routine for each minigame.
#define MAIN_ROUTINE_RUN_MINIGAME(start)                                       \
  {                                                                            \
    void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);         \
    void *saved = Heap_Alloc(MINIGAME_PALETTE_BYTES);                          \
                                                                               \
    Dma_Set(palette, saved, DMA_COPY_WORDS(MINIGAME_PALETTE_BYTES / 4),        \
            REG_DMA3);                                                         \
    entrance = start(entrance);                                                \
    Dma_Set(saved, palette, DMA_COPY_WORDS(MINIGAME_PALETTE_BYTES / 4),        \
            REG_DMA3);                                                         \
    Heap_Release(saved);                                                       \
  }

/// The debug start's entrance into Venus Lighthouse's grounds: the path south
/// of the round stone platform.
#define DEBUG_VENUS_LIGHTHOUSE_START 10

/// The main routine: runs one sequence after another and never returns. mode,
/// an enum GameStart, chooses the first scene. As The Broken Seal's, with this
/// game's four debug starts, rendering re-enabled after DMA 0 stops, and three
/// minigames, Super Lucky Dice the third.
void MainRoutine_Run(s32 mode) {
  const struct Scene *table;
  const struct Scene *record;
  s32 entrance;

  table = gScenes;
  // Choose the first scene before initializing the per-scene runtime.
  if (gDebugMode && mode == GAME_START_DEBUG_VENUS_LIGHTHOUSE) {
    gMainRoutineWork.next = SCENE_VENUS_LIGHTHOUSE_ENTRANCE;
    gMainRoutineWork.entrance = DEBUG_VENUS_LIGHTHOUSE_START;
  } else if (gDebugMode && mode == GAME_START_DEBUG_CLEAR) {
    gMainRoutineWork.next = SCENE_CLEAR;
    gMainRoutineWork.entrance = 1;
  } else if (gDebugMode && mode == GAME_START_DEBUG_MT_ALEPH) {
    gMainRoutineWork.next = SCENE_MT_ALEPH_SUMMIT;
    gMainRoutineWork.entrance = 1;
  } else if (gDebugMode && mode == GAME_START_DEBUG_TITLE) {
    gMainRoutineWork.next = SCENE_TITLE;
    gMainRoutineWork.entrance = 8;
  } else {
    MainRoutineWork_InitializeDefaultsFar();
    gMainRoutineWork.next = SCENE_TITLE;
    gMainRoutineWork.entrance = 1;
  }
  // The window colours from the options: hue, then brightness.
  WindowColor_ApplyFar(gMainRoutineWork.windowHue,
                       gMainRoutineWork.windowBrightness);
  VramCache_Reset();
  Task_ResetAll();
  Task_ResetAll();
  for (;;) {
    // FLAG_SKIP_SCENE_CHANGE_SOUND skips SE_STOP_ACTION_SE (The Broken Seal's
    // SE_STOP_ACTION_SE) for one scene change, then clears itself.
    if (GameFlag_Get(FLAG_SKIP_SCENE_CHANGE_SOUND))
      GameFlag_Clear(FLAG_SKIP_SCENE_CHANGE_SOUND);
    else
      Sound_PlayFar(SE_STOP_ACTION_SE);
    record = &table[gMainRoutineWork.next];
    entrance = gMainRoutineWork.entrance;

    // Stop DMA 0, which a scene may have left running for an HBlank effect.
    {
      volatile u16 *dma0 = REG_DMA0;

      dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
      dma0[5] &= ~DMA_ENABLE;
      dma0[5];
    }
    gOamCopyEnabled = 1;
    // Drop the tasks, the HBlank and VCount handlers and every heap block the
    // last scene, battle or minigame left.
    Task_ResetAll();
    Irq_SetHandler(IRQ_HBLANK, 0, 0);
    Irq_SetHandler(IRQ_VCOUNT, 0, 0);
    Heap_Initialize();
    DebugText_Clear();
    VramCache_Reset();
    // The battle and the three minigames return a result, not an exit: the
    // return scene is chosen from the main routine's work, and no overlay is
    // loaded.
    if (gMainRoutineWork.next >= SEQUENCE_SUPER_DICE) {
      switch (gMainRoutineWork.next) {
      case SEQUENCE_BATTLE:
        entrance = Battle_RunFar(entrance);
        break;
      case SEQUENCE_DICE:
        MAIN_ROUTINE_RUN_MINIGAME(LuckyDice_StartFar);
        break;
      case SEQUENCE_SLOTS:
        MAIN_ROUTINE_RUN_MINIGAME(LuckyWheels_StartFar);
        break;
      case SEQUENCE_SUPER_DICE:
        MAIN_ROUTINE_RUN_MINIGAME(SuperLuckyDice_StartFar);
        break;
      }
      Scene_ReturnFromSequence(entrance);
      continue;
    }

    // A scene: set its flags and music, load its code and run it.
    Scene_Arrive(gMainRoutineWork.next, GameFlag_Get(FLAG_KEEP_PARTY_POSITION));
    Scene_ChooseMusic();
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      // Either flag keeps the music that is playing: FLAG_KEEP_MUSIC_ONCE for
      // this one change, FLAG_KEEP_MUSIC_HELD for as long as it stays set.
      if (GameFlag_Get(FLAG_KEEP_MUSIC_ONCE) ||
          GameFlag_Get(FLAG_KEEP_MUSIC_HELD))
        GameFlag_Clear(FLAG_KEEP_MUSIC_ONCE);
      else
        Scene_PlayMusic();
    } else if (gMainRoutineWork.returnMusic != -1) {
      // Back where the party was: resume the music saved when the Pause menu
      // last opened (field_sequence/run.c).
      Sound_PlayFar(gMainRoutineWork.returnMusic);
    } else {
      Scene_PlayMusic();
    }
    gMainRoutineWork.mapId = record->mapIndex;
    Scene_LoadOverlay(0);
    entrance = FieldSequence_Run(entrance);
    Scene_TakeExit(entrance);
  }
}

/// Loads the current scene's overlay code (its resource) into gOverlayArea.
void Scene_LoadOverlay(s32 unused) {
  const struct Scene *table;

  table = gScenes;
  Overlay_Load(table[gMainRoutineWork.next].resourceId, &gOverlayArea);
}

/// The scene's row of gScenes.
#define MAP_HEADER(id) (&gScenes[id])

/// Updates the flags and state that last for one scene or one area, on entering
/// the current scene. As The Broken Seal's, with this game's flags, and
/// WorldMap_MarkPlaceVisited in place of the area's visited flag.
void Scene_Arrive(s32 unused, s32 keep) {
  s32 area = MAP_HEADER(gMainRoutineWork.next)->area;
  s32 flag;

  if (keep == 0) {
    // Local puzzle flags expire on every normal entry to a scene.
    for (flag = FLAG_SCENE_TEMP_FIRST; flag <= FLAG_SCENE_TEMP_LAST; flag++)
      GameFlag_Clear(flag);
    if (area != gMainRoutineWork.area) {
      // Area state and Retreat's destination survive moves within it.
      for (flag = FLAG_AREA_TEMP_FIRST; flag <= FLAG_AREA_TEMP_LAST; flag++)
        GameFlag_Clear(flag);
      GameFlag_Set(FLAG_SHOW_AREA_NAME);
      gMainRoutineWork.encounterSteps = 0;
      gMainRoutineWork.hazardSteps = 0;
      GameFlag_Clear(FLAG_IN_VENUS_LIGHTHOUSE);
      GameFlag_Clear(FLAG_IN_MERCURY_LIGHTHOUSE);
      GameFlag_Clear(FLAG_IN_MARS_LIGHTHOUSE);
      GameFlag_Clear(FLAG_IN_JUPITER_LIGHTHOUSE);
      gMainRoutineWork.retreatScene = gMainRoutineWork.next;
      gMainRoutineWork.retreatEntrance = gMainRoutineWork.entrance;
    }
    // The Djinn's hiding flags last for one scene, even within an area,
    // and so do the flags after them to FLAG_ARRIVAL_CLEARED_LAST.
    for (flag = FLAG_DJINN_HIDING_FIRST; flag <= FLAG_ARRIVAL_CLEARED_LAST;
         flag++)
      GameFlag_Clear(flag);
    GameFlag_Clear(FLAG_LINK_BATTLE);
    GameFlag_Clear(FLAG_RETREAT_BLOCKED);
    // FLAG_NO_ENCOUNTERS_IN_AREA stops random battles.
    GameFlag_Clear(FLAG_NO_ENCOUNTERS_IN_AREA);
    GameFlag_Clear(FLAG_IN_TOWN);
    GameFlag_Clear(FLAG_WORLD_MAP_LEFT);
    GameFlag_Clear(FLAG_SAND_CANNOT_END);
    GameFlag_Clear(FLAG_CANNOT_SAVE_HERE);
    GameFlag_Clear(FLAG_NO_PLACE_TO_SAVE);
    GameFlag_Clear(FLAG_IN_BATTLE_ARENA);
    if (gMainRoutineWork.ownerOverride != 0)
      GameFlag_Set(FLAG_SAILING);
    else
      GameFlag_Clear(FLAG_SAILING);
    gMainRoutineWork.returnMusic = -1;
  }
  gMainRoutineWork.area = area;
  WorldMap_MarkPlaceVisited(gMainRoutineWork.next, gMainRoutineWork.entrance);
  gMainRoutineWork.sceneType = MAP_HEADER(gMainRoutineWork.next)->sceneType;
  if (gMainRoutineWork.sceneType == SCENE_TYPE_TOWN)
    GameFlag_Set(FLAG_IN_TOWN);
  GameFlag_RefreshLureCapFar();
}

/// Starts the current scene's field music, chosen by Scene_ChooseMusic.
void Scene_PlayMusic(void) { Sound_PlayFar(gMainRoutineWork.sceneMusic); }

#undef MAIN_ROUTINE_RUN_MINIGAME
#undef MAP_HEADER
