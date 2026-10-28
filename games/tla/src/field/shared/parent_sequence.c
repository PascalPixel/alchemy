#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "mem.h"
#include "adventure.h"
#include "field_scene.h"
#include "scene_ids.h"

extern u8 gDebugMode;
extern u8 gRenderOamEnabled;

s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Sound_PlayFar(s32 cue);
void Func_080ad090(void);
void Func_08038348(s32 first, s32 second);
void VramCache_Reset(void);
void Task_ResetAll(void);
void Runtime_SetIrqHandler(s32 index, s32 a, s32 b);
void Runtime_InitializeHeap(void);
void Func_08014b70(void);
s32 Battle_RunEncounterFar(s32 entrance);
s32 Runtime_BlankDisplayAndRunFar(s32 entrance);
s32 Resource_FarCall00E(s32 entrance);
s32 Resource_FarCall00F(s32 entrance);
void Scene_ReturnFromSequence(s32 result);
void Func_080c9c38(s32 unused, s32 keep);
void Scene_ResolveInteractionResult(void);
void Party_PlaySavedMusic(void);
void BattleEffect_LoadResourceGroup(s32 unused);
s32 SceneSequence_Run(s32 entrance);
void Scene_TakeExit(s32 exitKind);

/// Sequence ids above the scene table's rows, as The Broken Seal's
/// SEQUENCE_ values: the sequences other than the scene sequence.
enum {
  // XXX: what sequence Resource_FarCall00F runs here, where The Broken
  // Seal's SEQUENCE_NONE runs nothing; Func_081b83c4's listing
  // (recon/tla/raw/081b83c4) and a scene that leaves for it would settle it.
  SEQUENCE_UNK_1FB = 0x1fb,
  SEQUENCE_SLOTS = 0x1fc, ///< the slots sequence (ReelGame_Run)
  SEQUENCE_DICE = 0x1fd,  ///< the dice sequence (LuckyDice_Run)
  SEQUENCE_BATTLE = 0x1fe ///< a battle; the entrance is the formation
};

enum { MINIGAME_PALETTE_FIRST = 14, MINIGAME_PALETTE_BYTES = 64 };

#define RUN_SAVING_PALETTES(call)                                              \
  {                                                                            \
    void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);         \
    void *saved = MemAlloc(MINIGAME_PALETTE_BYTES);                            \
                                                                               \
    Dma_Set(palette, saved,                                                    \
            (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),     \
            REG_DMA3);                                                         \
    entrance = call(entrance);                                                 \
    Dma_Set(saved, palette,                                                    \
            (DMA_ENABLE | DMA_32BIT) << 16 | (MINIGAME_PALETTE_BYTES / 4),     \
            REG_DMA3);                                                         \
    MemFree(saved);                                                            \
  }

/// The parent sequence, as The Broken Seal's ParentSequence_Run, with this
/// game's four debug starts, gRenderOamEnabled set after stopping DMA 0, and
/// three sequences that keep BG palettes 14 and 15.
void ParentSequence_Run(s32 mode) {
  const struct SceneRecord *table;
  const struct SceneRecord *record;
  s32 entrance;

  table = gField_SceneTable;
  if (gDebugMode && mode == 1) {
    gAdventure.scene = (s32)&SceneId_VenusLighthouseEntrance;
    gAdventure.entrance = 10;
  } else if (gDebugMode && mode == 2) {
    gAdventure.scene = (s32)&SceneId_Ov64a;
    gAdventure.entrance = 1;
  } else if (gDebugMode && mode == 3) {
    gAdventure.scene = (s32)&SceneId_Ov6b4;
    gAdventure.entrance = 1;
  } else if (gDebugMode && mode == 4) {
    gAdventure.scene = (s32)&SceneId_Title;
    gAdventure.entrance = 8;
  } else {
    Func_080ad090();
    gAdventure.scene = (s32)&SceneId_Title;
    gAdventure.entrance = 1;
  }
  Func_08038348(gAdventure.paletteGlow[0], gAdventure.paletteGlow[1]);
  VramCache_Reset();
  Task_ResetAll();
  Task_ResetAll();
  for (;;) {
    if (GameFlag_Test(0x101))
      GameFlag_ClearBit(0x101);
    else
      Sound_PlayFar(0x120);
    record = &table[gAdventure.scene];
    entrance = gAdventure.entrance;
    {
      volatile u16 *dma0 = REG_DMA0;

      dma0[5] &= 0xc5ff;
      dma0[5] &= 0x7fff;
      dma0[5];
    }
    gRenderOamEnabled = 1;
    Task_ResetAll();
    Runtime_SetIrqHandler(1, 0, 0);
    Runtime_SetIrqHandler(2, 0, 0);
    Runtime_InitializeHeap();
    Func_08014b70();
    VramCache_Reset();
    if (gAdventure.scene >= SEQUENCE_UNK_1FB) {
      switch (gAdventure.scene) {
      case SEQUENCE_BATTLE:
        entrance = Battle_RunEncounterFar(entrance);
        break;
      case SEQUENCE_DICE:
        RUN_SAVING_PALETTES(Runtime_BlankDisplayAndRunFar);
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

    Func_080c9c38(gAdventure.scene, GameFlag_Test(0x109));
    Scene_ResolveInteractionResult();
    if (!GameFlag_Test(0x109)) {
      if (GameFlag_Test(0x11a) || GameFlag_Test(0x11b))
        GameFlag_ClearBit(0x11a);
      else
        Party_PlaySavedMusic();
    } else if (gAdventure.returnCue != -1) {
      Sound_PlayFar(gAdventure.returnCue);
    } else {
      Party_PlaySavedMusic();
    }
    gAdventure.map = record->mapIndex;
    BattleEffect_LoadResourceGroup(0);
    entrance = SceneSequence_Run(entrance);
    Scene_TakeExit(entrance);
  }
}
