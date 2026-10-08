#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "types.h"
#include "dma.h"
#include "game_state.h"
#include "scene_ids.h"
#include "fieldrun.h"
#include "io_reg.h"
#include "irq.h"
#include "callback_scheduler.h"
#include "btlsys.h"
#include "flag_ids.h"
s32 Runtime_BlankDisplayAndRunFar(s32 entrance);
s32 Runtime_BlankDisplayLoadValueAndRunFar(s32 entrance);

#define BattleFx_LoadResourceGroup ((void (*)(s32))BattleFx_LoadResourceGroup)

enum {
    SCENE_BLANK = 0x1fb,
    SCENE_TABLE_LOAD = 0x1fc,
    SCENE_TABLE_RUN = 0x1fd,
    SCENE_BATTLE = 0x1fe
};

extern u8 gDebugMode;

void Audio_PlayCueFromEventWork(void);
void AudioCommand_PlayFar(s32 cue);

static __inline__ s32 Scene_RunTable(s32 entrance)
{
    void *palette = (void *)&BG_PLTT_COLOR(14, 0);
    void *saved = MemAlloc(64);
    s32 result;

    Dma_Set(palette, saved, DMA_ENABLE32 | DMA_32BIT | (64 / 4), REG_DMA3);
    result = Runtime_BlankDisplayAndRunFar(entrance);
    Dma_Set(saved, palette, DMA_ENABLE32 | DMA_32BIT | (64 / 4), REG_DMA3);
    MemFree(saved);
    return result;
}

static __inline__ s32 Scene_LoadTable(s32 entrance)
{
    void *palette = (void *)&BG_PLTT_COLOR(14, 0);
    void *saved = MemAlloc(64);
    s32 result;

    Dma_Set(palette, saved, DMA_ENABLE32 | DMA_32BIT | (64 / 4), REG_DMA3);
    result = Runtime_BlankDisplayLoadValueAndRunFar(entrance);
    Dma_Set(saved, palette, DMA_ENABLE32 | DMA_32BIT | (64 / 4), REG_DMA3);
    MemFree(saved);
    return result;
}

void Game_ResetForNewGame(s32 mode)
{
    const struct SceneRecord *table;
    const struct SceneRecord *record;
    s32 entrance;

    table = Field_SceneTable;
    if (gDebugMode) {
        if (mode == 1) {
            gGameState.scene = (s32)&SceneId_HaidiaIe;
            gGameState.entrance = mode;
            goto start;
        }
        if (mode == 2) {
            gGameState.scene = (s32)&SceneId_Clear;
            gGameState.entrance = 1;
            goto start;
        }
    }
    GameState_InitDefaultsFar();
    gGameState.scene = (s32)&SceneId_Title;
    gGameState.entrance = 2;
start:
    PaletteGlow_UpdateFar(gGameState.palette_glow[0], gGameState.palette_glow[1]);
    Resource_InitializeTable();
    Scheduler_ResetTaskTable();
    Scheduler_ResetTaskTable();
    for (;;) {
        if (GameFlag_TestFar(0x101))
            GameFlag_ClearBitFar(0x101);
        else
            AudioCommand_PlayFar(0x120);
        record = &table[gGameState.scene];
        entrance = gGameState.entrance;
        {
            volatile u16 *dma0 = REG_DMA0;

            dma0[5] &= 0xc5ff;
            dma0[5] &= 0x7fff;
            dma0[5];
        }
        Scheduler_ResetTaskTable();
        Runtime_SetIrqHandler(1, 0, 0);
        Runtime_SetIrqHandler(2, 0, 0);
        Runtime_InitializeHeap();
        Bg0_ClearTilemap();
        Resource_InitializeTable();
        if (gGameState.scene >= SCENE_BLANK) {
            switch (gGameState.scene) {
            case SCENE_BATTLE:
                entrance = Battle_RunEncounterFar(entrance);
                break;
            case SCENE_TABLE_RUN:
                entrance = Scene_RunTable(entrance);
                break;
            case SCENE_TABLE_LOAD:
                entrance = Scene_LoadTable(entrance);
                break;
            case SCENE_BLANK:
                entrance = 0;
                break;
            }
            Party_SetReturnPoint(entrance);
            continue;
        }
        Scene_ResetFlagsOnEnter(gGameState.scene, GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION));
        Scene_ResolveInteractionResult();
        if (!GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION)) {
            if (GameFlag_TestFar(0x11a) || GameFlag_TestFar(0x11b))
                GameFlag_ClearBitFar(0x11a);
            else
                Audio_PlayCueFromEventWork();
        } else if (gGameState.return_cue != -1) {
            AudioCommand_PlayFar(gGameState.return_cue);
        } else {
            Audio_PlayCueFromEventWork();
        }
        gGameState.map = record->map_index;
        BattleFx_LoadResourceGroup(0);
        MapGroupTable_SelectEntry(Field_RunScene(entrance));
    }
}
