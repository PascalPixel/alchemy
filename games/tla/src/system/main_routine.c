/// @file
/// The main routine and scene changes: exits, returns from a sequence,
/// entering a scene. As The Broken Seal's main_routine.c, in the same order;
/// in bank 04 it follows the field sequence's picture decoders.

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_unit.h"
#include "dma.h"
#include "field_event.h"
#include "heap_release.h"
#include "io_reg.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "scene_tables.h"
#include "actor.h"
#include "flag_call.h"
#include "frame.h"
#include "gamepak/far_call/lucky_dice.h"
#include "module/field/field.h"
#include "module/system/system.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/window.h"
#include "task.h"

extern struct SceneEntries gOverlayArea;

void *Unit_Get(s32 owner);
void Irq_SetHandler(s32 a, s32 b, void *callback);
s32 FieldSequence_Run(s32 entrance);

/// The scene overlay's exit table: a header word holding a scene id, then that
/// scene's exits, one packed word each (and a flag word after it when
/// EXIT_HAS_FLAG is set). Scene id EXIT_TABLE_END ends the exits of a scene and
/// the whole table.
#define EXIT_SCENE_MASK 0xfff
#define EXIT_ENTRANCE(word) (((word) & 0xff000) >> 12)
#define EXIT_KIND(word) (((word) & 0x0ff00000) >> 20)
#define EXIT_HAS_FLAG 0x10000000
#define EXIT_TABLE_END 0x1ff
#define EXIT_KIND_ANY 0xff
#define EXIT_NONE                                                              \
  999 ///< FieldSequence_Run's result when the scene was not left

/// Chooses the scene and entrance to go to when the party leaves the current
/// scene: exitKind is the kind of exit FieldSequence_Run ended on, and the
/// scene's exit table gives the first exit of that kind (or of any kind) whose
/// flag is not set or whose flag word is -1. An exit of kind 0 or the end of
/// the scene's exits falls back to the scene of its first exit.
// As ☀️'s function of the same name, which has no -1 flag word.
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

  currentScene = gMainRoutineWork.scene;
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
        // As one condition the two flag tests fold into a range check.
        if (flag == 0)
          goto take;
        if (flag == -1)
          goto take;
        if (GameFlag_Test(flag))
          continue;
      take:
        nextScene = scene;
        nextEntrance = entrance;
        break;
      }
    }
    break;
  }

  // Commit the chosen exit only after a matching scene header was found.
  if (nextScene != EXIT_NONE) {
    gMainRoutineWork.scene = nextScene;
    gMainRoutineWork.entrance = nextEntrance;
  }
}

/// The Broken Seal's Scene_ReturnFromSequence, without the childhood healing,
/// ending a defeat with Func_080ca3f4.
void Scene_ReturnFromSequence(s32 result) {
  struct BattleUnit *unit;
  s16 scene;
  s16 entrance;

  gMainRoutineWork.sequenceResult = result;
  if (result == -1) {
    unit = Unit_Get(gMainRoutineWork.currentOwner);
    if (unit->hp == 0) {
      unit->hp = 1;
      Unit_UpdateGauges(unit);
    }
    scene = gMainRoutineWork.pair1f2[0];
    entrance = gMainRoutineWork.pair1f2[1];
    if (scene != -1 || entrance != -1) {
      if (scene != -1)
        gMainRoutineWork.scene = scene;
      else
        gMainRoutineWork.scene = gMainRoutineWork.primary;
      if (entrance != -1)
        gMainRoutineWork.entrance = entrance;
      else
        gMainRoutineWork.entrance = gMainRoutineWork.secondary;
    } else {
      gMainRoutineWork.scene = gMainRoutineWork.savedScene;
      gMainRoutineWork.entrance = gMainRoutineWork.savedEntrance;
    }
    Func_080ca3f4(gMainRoutineWork.scene, gMainRoutineWork.entrance);
  } else {
    scene = gMainRoutineWork.pair1ee[0];
    entrance = gMainRoutineWork.pair1ee[1];
    if (scene != -1 || entrance != -1) {
      if (scene != -1)
        gMainRoutineWork.scene = scene;
      else
        gMainRoutineWork.scene = gMainRoutineWork.primary;
      if (entrance != -1)
        gMainRoutineWork.entrance = entrance;
      else
        gMainRoutineWork.entrance = gMainRoutineWork.secondary;
    } else {
      gMainRoutineWork.scene = gMainRoutineWork.primary;
      gMainRoutineWork.entrance = gMainRoutineWork.secondary;
      GameFlag_SetBit(0x109);
    }
  }
}

s8 BattleEffect_GetResourceGroup(s32 effectIndex) {
  return gScenes[effectIndex].group;
}

s16 Field_GetSceneAttributes(s32 scene) { return gScenes[scene].attributes; }

#define RUN_SAVING_PALETTES(call)                                              \
  {                                                                            \
    void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);         \
    void *saved = Heap_Alloc(MINIGAME_PALETTE_BYTES);                          \
                                                                               \
    Dma_Set(palette, saved, DMA_COPY_WORDS(MINIGAME_PALETTE_BYTES / 4),        \
            REG_DMA3);                                                         \
    entrance = call(entrance);                                                 \
    Dma_Set(saved, palette, DMA_COPY_WORDS(MINIGAME_PALETTE_BYTES / 4),        \
            REG_DMA3);                                                         \
    Heap_Release(saved);                                                       \
  }

/// The main routine, as The Broken Seal's MainRoutine_Run, with this
/// game's four debug starts, gRenderOamEnabled set after stopping DMA 0, and
/// three sequences that keep BG palettes 14 and 15.
void MainRoutine_Run(s32 mode) {
  const struct Scene *table;
  const struct Scene *record;
  s32 entrance;

  table = gScenes;
  if (gDebugMode && mode == 1) {
    gMainRoutineWork.scene = (s32)&SceneId_VenusLighthouseEntrance;
    gMainRoutineWork.entrance = 10;
  } else if (gDebugMode && mode == 2) {
    gMainRoutineWork.scene = (s32)&SceneId_Ov64a;
    gMainRoutineWork.entrance = 1;
  } else if (gDebugMode && mode == 3) {
    gMainRoutineWork.scene = (s32)&SceneId_Ov6b4;
    gMainRoutineWork.entrance = 1;
  } else if (gDebugMode && mode == 4) {
    gMainRoutineWork.scene = (s32)&SceneId_Title;
    gMainRoutineWork.entrance = 8;
  } else {
    MainRoutineWork_InitializeDefaultsFar();
    gMainRoutineWork.scene = (s32)&SceneId_Title;
    gMainRoutineWork.entrance = 1;
  }
  WindowColor_ApplyFar(gMainRoutineWork.paletteGlow[0],
                       gMainRoutineWork.paletteGlow[1]);
  VramCache_Reset();
  Task_ResetAll();
  Task_ResetAll();
  for (;;) {
    if (GameFlag_Test(0x101))
      GameFlag_ClearBit(0x101);
    else
      Sound_PlayFar(0x120);
    record = &table[gMainRoutineWork.scene];
    entrance = gMainRoutineWork.entrance;
    {
      volatile u16 *dma0 = REG_DMA0;

      dma0[5] &= 0xc5ff;
      dma0[5] &= 0x7fff;
      dma0[5];
    }
    gRenderOamEnabled = 1;
    Task_ResetAll();
    Irq_SetHandler(1, 0, 0);
    Irq_SetHandler(2, 0, 0);
    Heap_Initialize();
    DebugText_Clear();
    VramCache_Reset();
    if (gMainRoutineWork.scene >= SEQUENCE_UNK_1FB) {
      switch (gMainRoutineWork.scene) {
      case SEQUENCE_BATTLE:
        entrance = Battle_RunEncounterFar(entrance);
        break;
      case SEQUENCE_DICE:
        RUN_SAVING_PALETTES(LuckyDice_StartFar);
        break;
      case SEQUENCE_SLOTS:
        RUN_SAVING_PALETTES(Resource_FarCall00E);
        break;
      case SEQUENCE_UNK_1FB:
        RUN_SAVING_PALETTES(Resource_FarCall00F);
        break;
      }
      Scene_ReturnFromSequence(entrance);
      continue;
    }

    Scene_Arrive(gMainRoutineWork.scene, GameFlag_Test(0x109));
    Scene_ChooseMusic();
    if (!GameFlag_Test(0x109)) {
      if (GameFlag_Test(0x11a) || GameFlag_Test(0x11b))
        GameFlag_ClearBit(0x11a);
      else
        Party_PlaySavedMusic();
    } else if (gMainRoutineWork.returnCue != -1) {
      Sound_PlayFar(gMainRoutineWork.returnCue);
    } else {
      Party_PlaySavedMusic();
    }
    gMainRoutineWork.map = record->mapIndex;
    BattleEffect_LoadResourceGroup(0);
    entrance = FieldSequence_Run(entrance);
    Scene_TakeExit(entrance);
  }
}

void BattleEffect_LoadResourceGroup(s32 unused) {
  const struct Scene *table = gScenes;
  s16 scene = gMainRoutineWork.scene;

  Resource_LoadCode(table[scene].resourceId, &gOverlayArea);
}

#define SCENE_RECORD(id) (&gScenes[id])

/// As The Broken Seal's, with this game's flags, and Func_080c9de0 in place of
/// the area's visited flag.
void Scene_Arrive(s32 unused, s32 keep) {
  s32 group = SCENE_RECORD(gMainRoutineWork.scene)->group;
  s32 flag;

  if (keep == 0) {
    for (flag = 0x200; flag <= 0x2ff; flag++)
      GameFlag_ClearBit(flag);
    if (group != gMainRoutineWork.sceneGroup) {
      for (flag = 0x300; flag <= 0x3ff; flag++)
        GameFlag_ClearBit(flag);
      GameFlag_SetBit(0x12f);
      gMainRoutineWork.encounterSteps = 0;
      gMainRoutineWork.hazardSteps = 0;
      GameFlag_ClearBit(0x110);
      GameFlag_ClearBit(0x111);
      GameFlag_ClearBit(0x112);
      GameFlag_ClearBit(0x113);
      gMainRoutineWork.retreatScene = gMainRoutineWork.scene;
      gMainRoutineWork.retreatEntrance = gMainRoutineWork.entrance;
    }
    for (flag = 0x80; flag <= 0xdf; flag++)
      GameFlag_ClearBit(flag);
    GameFlag_ClearBit(0x16c);
    GameFlag_ClearBit(0x144);
    GameFlag_ClearBit(0x161);
    GameFlag_ClearBit(0x123);
    GameFlag_ClearBit(0x11c);
    GameFlag_ClearBit(0x146);
    GameFlag_ClearBit(0x17c);
    GameFlag_ClearBit(0x17d);
    GameFlag_ClearBit(0x17e);
    if (gMainRoutineWork.ownerOverride != 0)
      GameFlag_SetBit(0x176);
    else
      GameFlag_ClearBit(0x176);
    gMainRoutineWork.returnCue = -1;
  }
  gMainRoutineWork.sceneGroup = group;
  Func_080c9de0(gMainRoutineWork.scene, gMainRoutineWork.entrance);
  gMainRoutineWork.sceneVariant = SCENE_RECORD(gMainRoutineWork.scene)->variant;
  if (gMainRoutineWork.sceneVariant == 2)
    GameFlag_SetBit(0x123);
  GameFlag_RefreshLureCapFar();
}

void Party_PlaySavedMusic(void) { Sound_PlayFar(gMainRoutineWork.cue); }

#undef EXIT_SCENE_MASK
#undef EXIT_ENTRANCE
#undef EXIT_KIND
#undef EXIT_HAS_FLAG
#undef EXIT_TABLE_END
#undef EXIT_KIND_ANY
#undef EXIT_NONE
#undef RUN_SAVING_PALETTES
#undef SCENE_RECORD
