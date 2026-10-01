#include "TYPES.H"
#include "DMA.H"
#include "GAME_STATE.H"
#include "SCENE_IDS.H"

/* One row of the scene table. */
struct SceneRecord {
    s16 resource_id;
    s8 group;
    s8 variant;
    u16 map_index;
    u16 reserved;
};

/* Scenes past the table are not maps: a blank screen, the two table
   screens that draw over the last object palettes, and a battle. */
enum {
    SCENE_BLANK = 0x1fb,
    SCENE_TABLE_LOAD = 0x1fc,
    SCENE_TABLE_RUN = 0x1fd,
    SCENE_BATTLE = 0x1fe
};

#define REG_DMA0 ((volatile u16 *)0x040000b0)
#define REG_DMA3 ((volatile u32 *)0x040000d4)
#define OBJ_PALETTE_LAST ((void *)0x050001c0)
#define DMA_COPY_16_WORDS 0x84000010

extern u8 gDebugMode;
extern struct SceneRecord Field_SceneTable[];

void Runtime_BumpFree(void *buffer);
void Runtime_SetIrqHandler(s32 index, s32 handler, s32 context);
void Resource_InitializeTable(void);
void Scheduler_ResetTaskTable(void);
void Bg0_ClearTilemap(void);
void Runtime_InitializeHeap(void);
void *Runtime_BumpAllocate(s32 size);
void PaletteGlow_UpdateFar(s32 first, s32 second);
void GameState_InitDefaultsFar(void);
s32 GameFlag_TestFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void MapGroupTable_SelectEntry(void);
void Party_SetReturnPoint(s32 reason);
void BattleFx_LoadResourceGroup(s32 group);
void Scene_ResetFlagsOnEnter(s32 scene, s32 keep);
void Audio_PlayCueFromEventWork(void);
void Scene_ResolveInteractionResult(void);
void Func_0808c4f8(s32 entrance);
s32 Battle_RunEncounterFar(s32 entrance);
s32 FarCall_BlankRunTable(s32 entrance);
s32 FarCall_BlankLoadTable(s32 entrance);
void Audio_PlayCue(s32 cue);

/* The table screens borrow the last two object palettes; each keeps them
   in a heap buffer while it runs and puts them back. */
static __inline__ s32 Scene_RunTable(s32 entrance)
{
    void *palette = OBJ_PALETTE_LAST;
    void *saved = Runtime_BumpAllocate(64);
    s32 result;

    Dma_Set(palette, saved, DMA_COPY_16_WORDS, REG_DMA3);
    result = FarCall_BlankRunTable(entrance);
    Dma_Set(saved, palette, DMA_COPY_16_WORDS, REG_DMA3);
    Runtime_BumpFree(saved);
    return result;
}

static __inline__ s32 Scene_LoadTable(s32 entrance)
{
    void *palette = OBJ_PALETTE_LAST;
    void *saved = Runtime_BumpAllocate(64);
    s32 result;

    Dma_Set(palette, saved, DMA_COPY_16_WORDS, REG_DMA3);
    result = FarCall_BlankLoadTable(entrance);
    Dma_Set(saved, palette, DMA_COPY_16_WORDS, REG_DMA3);
    Runtime_BumpFree(saved);
    return result;
}

/*
 * The game's scene loop, which never returns. It starts at the title, or
 * with the debug switch at one of two test scenes, and then runs scene
 * after scene: each pass resets the tasks, the heap and the resources, and
 * either runs one of the special scenes and lets its result choose where
 * the party goes next, or enters the map scene the game state names.
 */
void Game_ResetForNewGame(s32 mode)
{
    struct SceneRecord *table;
    struct SceneRecord *record;
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
            Audio_PlayCue(0x120);
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
        Scene_ResetFlagsOnEnter(gGameState.scene, GameFlag_TestFar(0x109));
        Scene_ResolveInteractionResult();
        if (!GameFlag_TestFar(0x109)) {
            if (GameFlag_TestFar(0x11a) || GameFlag_TestFar(0x11b))
                GameFlag_ClearBitFar(0x11a);
            else
                Audio_PlayCueFromEventWork();
        } else if (gGameState.return_cue != -1) {
            Audio_PlayCue(gGameState.return_cue);
        } else {
            Audio_PlayCueFromEventWork();
        }
        gGameState.map = record->map_index;
        BattleFx_LoadResourceGroup(0);
        Func_0808c4f8(entrance);
        MapGroupTable_SelectEntry();
    }
}
