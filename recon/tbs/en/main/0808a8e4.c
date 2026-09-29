/* 2026-09-29: eight minutes of permutation reached 2130 from 2520 through
 * 26 rewrites (a 0x7fff mask temporary, the scene read into s16 locals, an
 * inverted cue test and several same-type casts); not kept here. The scene
 * numbers 0, 1 and 5 are Value_ symbols, so the draft cannot be adopted as
 * written. */
/* Draft, not exact (2026-09-24): 628 bytes for the 612-byte owner, 195
   halfwords differ. The prologue, the start-up branches (scene values are
   pool-loaded link symbols; the entrance stores are immediates) and the
   flag and cue handling match. Residual: the reference keeps the palette
   address in r9 across the loop but reloads the DMA0 base (0x040000b0)
   inside it; this for-loop hoists the DMA base into a register and moves
   the pool, and a goto loop instead drops the palette register and shares
   0x101 across the two flag calls. The SDK DmaStop block spelling also
   drops the palette register (290). */

#include "TYPES.H"
#include "DMA.H"

/* One eight-byte row per scene. */
struct SceneEntry {
    u8 unknown_0[4];
    u16 music;                  /* 0x04 */
    u8 unknown_6[2];
};

/* The game state fields the main loop reads and writes. */
struct GameLoopState {
    u8 unknown_000[0x1c0];
    s16 scene;                  /* 0x1c0 */
    s16 entrance;               /* 0x1c2 */
    u8 unknown_1c4[0x16];
    u16 scene_music;            /* 0x1da */
    u8 unknown_1dc[0x29];
    u8 glow_205;                /* 0x205 */
    u8 glow_206;                /* 0x206 */
    u8 unknown_207[0x17];
    s16 resume_cue;             /* 0x21e */
};

extern struct GameLoopState gGameState;

/* Scene numbers are link-time values. */
extern u8 Value_00000000;
extern u8 Value_00000001;
extern u8 Value_00000005;
extern u8 gDebugMode;
extern struct SceneEntry Field_SceneTable[];

void Runtime_BumpFree(void *buffer);
void Runtime_SetIrqHandler(s32 index, s32 a, s32 b);
void Resource_InitializeTable(void);
void Scheduler_ResetTaskTable(void);
void Bg0_ClearTilemap(void);
void Runtime_InitializeHeap(void);
void *Runtime_BumpAllocate(s32 size);
void PaletteGlow_UpdateFar(s32 a, s32 b);
void GameState_InitDefaultsFar();
s32 GameFlag_TestFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void MapGroupTable_SelectEntry(void);
void Party_SetReturnPoint(s32 value);
void BattleFx_LoadResourceGroup(s32 group);
void Scene_ResetFlagsOnEnter(s32 scene, s32 flag);
void Audio_PlayCueFromEventWork(void);
void Scene_ResolveInteractionResult(void);
void Func_0808c4f8(s32 entrance);
s32 Battle_RunEncounterFar(s32 entrance);
s32 FarCall_BlankRunTable(s32 entrance);
s32 FarCall_BlankLoadTable(s32 entrance);
void Audio_PlayCue(s32 cue);

#define PALETTE_OBJ_1C0 ((void *)0x050001c0)
struct DmaChannel {
    const void *source;
    void *destination;
    u16 count;
    u16 control;
};

#define REG_DMA0 ((volatile struct DmaChannel *)0x040000b0)
#define REG_DMA3 ((volatile u32 *)0x040000d4)

void Game_ResetForNewGame(s32 mode)
{
    struct SceneEntry *table;
    struct SceneEntry *entry;
    void *palette;
    s32 entrance;
    void *saved;

    table = Field_SceneTable;
    if (gDebugMode) {
        if (mode == 1) {
            gGameState.scene = (s32)&Value_00000005;
            gGameState.entrance = mode;
            goto start;
        }
        if (mode == 2) {
            gGameState.scene = (s32)&Value_00000001;
            gGameState.entrance = 1;
            goto start;
        }
    }
    GameState_InitDefaultsFar();
    gGameState.scene = (s32)&Value_00000000;
    gGameState.entrance = 2;
start:
    PaletteGlow_UpdateFar(gGameState.glow_205, gGameState.glow_206);
    Resource_InitializeTable();
    Scheduler_ResetTaskTable();
    Scheduler_ResetTaskTable();
    palette = PALETTE_OBJ_1C0;
    for (;;) {
        if (GameFlag_TestFar(0x101))
            GameFlag_ClearBitFar(0x101);
        else
            Audio_PlayCue(0x120);
        entry = &table[gGameState.scene];
        entrance = gGameState.entrance;
        REG_DMA0->control &= 0xc5ff;
        REG_DMA0->control &= 0x7fff;
        REG_DMA0->control;
        Scheduler_ResetTaskTable();
        Runtime_SetIrqHandler(1, 0, 0);
        Runtime_SetIrqHandler(2, 0, 0);
        Runtime_InitializeHeap();
        Bg0_ClearTilemap();
        Resource_InitializeTable();
        if (gGameState.scene > 0x1fa) {
            switch (gGameState.scene) {
            case 0x1fb:
                entrance = 0;
                break;
            case 0x1fc:
                saved = Runtime_BumpAllocate(64);
                Dma_Set(palette, saved, 0x84000010, REG_DMA3);
                entrance = FarCall_BlankLoadTable(entrance);
                Dma_Set(saved, palette, 0x84000010, REG_DMA3);
                Runtime_BumpFree(saved);
                break;
            case 0x1fd:
                saved = Runtime_BumpAllocate(64);
                Dma_Set(palette, saved, 0x84000010, REG_DMA3);
                entrance = FarCall_BlankRunTable(entrance);
                Dma_Set(saved, palette, 0x84000010, REG_DMA3);
                Runtime_BumpFree(saved);
                break;
            case 0x1fe:
                entrance = Battle_RunEncounterFar(entrance);
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
        } else if (gGameState.resume_cue != -1) {
            Audio_PlayCue(gGameState.resume_cue);
        } else {
            Audio_PlayCueFromEventWork();
        }
        gGameState.scene_music = entry->music;
        BattleFx_LoadResourceGroup(0);
        Func_0808c4f8(entrance);
        MapGroupTable_SelectEntry();
    }
}
