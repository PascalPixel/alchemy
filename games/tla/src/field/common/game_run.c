#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "mem.h"
#include "party_state.h"
#include "field_scene.h"
#include "scene_ids.h"

extern u8 gDebugMode;
extern u8 gRenderOamEnabled;

s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Audio_PlayCue(s32 cue);
void Func_080ad090(void);
void Func_08038348(s32 first, s32 second);
void VramCache_Reset(void);
void Scheduler_ResetTaskTable(void);
void Runtime_SetIrqHandler(s32 index, s32 a, s32 b);
void Runtime_InitializeHeap(void);
void Func_08014b70(void);
s32 Battle_RunEncounterFar(s32 entrance);
s32 Runtime_BlankDisplayAndRunFar(s32 entrance);
s32 Resource_FarCall00E(s32 entrance);
s32 Resource_FarCall00F(s32 entrance);
void Func_080c977c(s32 result);
void Func_080c9c38(s32 unused, s32 keep);
void Scene_ResolveInteractionResult(void);
void Party_PlaySavedMusic(void);
void BattleEffect_LoadResourceGroup(s32 unused);
s32 Field_RunScene(s32 entrance);
void Scene_TakeExit(s32 exitKind);

enum {
  SCENE_MINIGAME_1FB = 0x1fb,
  SCENE_MINIGAME_1FC = 0x1fc,
  SCENE_MINIGAME_1FD = 0x1fd,
  SCENE_BATTLE = 0x1fe
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

/// The Broken Seal's Game_Run, with this game's four debug starts,
/// gRenderOamEnabled set after stopping DMA 0, and three minigame scenes
/// that keep BG palettes 14 and 15.
void Game_Run(s32 mode) {
  const struct SceneRecord *table;
  const struct SceneRecord *record;
  s32 entrance;

  table = gField_SceneTable;
  if (gDebugMode && mode == 1) {
    gPartyState.scene = (s32)&SceneId_VenusLighthouseEntrance;
    gPartyState.entrance = 10;
  } else if (gDebugMode && mode == 2) {
    gPartyState.scene = (s32)&SceneId_Ov64a;
    gPartyState.entrance = 1;
  } else if (gDebugMode && mode == 3) {
    gPartyState.scene = (s32)&SceneId_Ov6b4;
    gPartyState.entrance = 1;
  } else if (gDebugMode && mode == 4) {
    gPartyState.scene = (s32)&SceneId_Title;
    gPartyState.entrance = 8;
  } else {
    Func_080ad090();
    gPartyState.scene = (s32)&SceneId_Title;
    gPartyState.entrance = 1;
  }
  Func_08038348(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
  VramCache_Reset();
  Scheduler_ResetTaskTable();
  Scheduler_ResetTaskTable();
  for (;;) {
    if (GameFlag_Test(0x101))
      GameFlag_ClearBit(0x101);
    else
      Audio_PlayCue(0x120);
    record = &table[gPartyState.scene];
    entrance = gPartyState.entrance;
    {
      volatile u16 *dma0 = REG_DMA0;

      dma0[5] &= 0xc5ff;
      dma0[5] &= 0x7fff;
      dma0[5];
    }
    gRenderOamEnabled = 1;
    Scheduler_ResetTaskTable();
    Runtime_SetIrqHandler(1, 0, 0);
    Runtime_SetIrqHandler(2, 0, 0);
    Runtime_InitializeHeap();
    Func_08014b70();
    VramCache_Reset();
    if (gPartyState.scene >= SCENE_MINIGAME_1FB) {
      switch (gPartyState.scene) {
      case SCENE_BATTLE:
        entrance = Battle_RunEncounterFar(entrance);
        break;
      case SCENE_MINIGAME_1FD:
        RUN_SAVING_PALETTES(Runtime_BlankDisplayAndRunFar);
        break;
      case SCENE_MINIGAME_1FC:
        RUN_SAVING_PALETTES(Resource_FarCall00E);
        break;
      case SCENE_MINIGAME_1FB:
        RUN_SAVING_PALETTES(Resource_FarCall00F);
        break;
      }
      Func_080c977c(entrance);
      continue;
    }

    Func_080c9c38(gPartyState.scene, GameFlag_Test(0x109));
    Scene_ResolveInteractionResult();
    if (!GameFlag_Test(0x109)) {
      if (GameFlag_Test(0x11a) || GameFlag_Test(0x11b))
        GameFlag_ClearBit(0x11a);
      else
        Party_PlaySavedMusic();
    } else if (gPartyState.returnCue != -1) {
      Audio_PlayCue(gPartyState.returnCue);
    } else {
      Party_PlaySavedMusic();
    }
    gPartyState.map = record->mapIndex;
    BattleEffect_LoadResourceGroup(0);
    entrance = Field_RunScene(entrance);
    Scene_TakeExit(entrance);
  }
}
