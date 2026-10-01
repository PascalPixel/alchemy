/* NONMATCHING: alchemy drafts scores 15595, 451 differing instructions of
 * 950; it could not be scored before, because its offset macros did not parse.
 * This is the reel game itself: it allocates the ReelWork block (0x61c) and
 * the battle effect work, runs ReelGame_RunFrame as a callback, and frees
 * both. Resource numbers are directory rows now (EffectFarCalls and the
 * three Slots rows are named for it); small numbers the listing loads from
 * the pool before a strh are halfword constants, not names.
 * Remaining: the two tile-map loops keep one running offset in r8 and add
 * the map base at each store, where this draft walks three pointers; the
 * register writes reuse one base register chain in the listing; the frame
 * holds one word more than here; the particle and line loops are unchecked.
 * gReelSave needs its label twelve bytes into gCell (recon/tbs/sym_ewram.s). */
#include "TYPES.H"
#include "DMA.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "IO_REG.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"
#include "RESOURCE_IDS.H"

/* Builds and presents the particle scene, then releases its work blocks. */

typedef volatile u16 vu16;

/* The 28-byte particle record already established in 080e0c84.c.  Only
   x, y, vx, vy and the countdown at +0x18 are touched here. */
typedef struct {
    s32 x;
    s32 y;
    s32 rot;
    s32 vx;
    s32 vy;
    s32 unk14;
    s32 life;
} Particle;

/* The reel game state, as ReelGame_RunFrame (080f6440.c) lays it out. */
struct ReelRow {
    s32 pos;
    u8 cell[21];
    u8 held;
    s32 stop : 8;
};

struct ReelObject {
    volatile u32 attr01;
    volatile u32 attr2;
};

struct ReelWork {
    struct ReelRow row[5];
    s32 state;
    s32 cursor;
    s32 spins;
    s32 bet;
    u16 keys;
    u16 dir;
    u16 pressed;
    u16 repeat;
    u8 unknown_0a4[4];
    s32 timer;
    s32 line[7];
    struct ReelObject obj[128];
    s32 window;
    s32 sub_window;
    u8 unknown_4d0[8];
    u16 scanline_offsets[160];
    s32 phase;
};

struct ReelSave {
    u8 unknown_000[0x120];
    s8 won_prizes[16];
};

/* Allocation-id keyed heap cache shared by this family. */
extern void *gWorkSlot[];

/* The saved prize list, a label twelve bytes into gCell. */
extern struct ReelSave gReelSave;
extern u8 gOamCopyEnabled;
extern u8 gMapCellBuffer[];
extern u16 gBgScroll[];
void ReelGame_RunFrame(void);
void BattlePres_ProcessPendingTileTransfer(void);

/* Halfword table of sprite offsets into the 0x60e sprite block, indexed
   by the particle's remaining-life bucket. */
extern u16 Data_080f86f8[];


void *Runtime_AllocateHeapBlock(s32 id, s32 size);
void *Runtime_AllocateBlock(s32 id, s32 size);
void RuntimeDispatch_NoOpHook(s32 id);
void Scheduler_ResetTaskTable(void);
void *Resource_GetTableEntry(s32 id);
u32 Resource_DecodeType01(const void *source, void *destination);
void _call_via_r3(void *dest, s32 size, s32 source, void *state);
void FarCall_WindowTable(void);
extern u8 IwramClearWords[];
extern u8 IwramFillWords[];
void ReelGame_InitTitle(void);
u32 Random16(void);
s32 __umodsi3(s32 value, s32 divisor);
s32 __modsi3(s32 value, s32 divisor);
s32 __divsi3(s32 value, s32 divisor);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
s32 FarCall_EffectTable(s32 id, s32 a, s32 b, s32 c, s32 d);
s32 Graphics_ScaleRgb555(u16 *source, u16 *destination, s32 scale, s32 count);
s32 PartyInventory_CountItemFar(s32 id);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Palette_DarkenSceneStep(void);
void Palette_StepTowardResource(s32 id);
void BattleFx_DrawCanvasLine(s32 a, s32 b, s32 c, s32 d, s32 e);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
void Scheduler_RemoveCallback(void *callback);
void Runtime_ReleaseHeapBlock(s32 id);
void WaitFrames(s32 frames);

void Scene_RunParticleSequence(void)
{
    u8 *sprites;
    u8 *canvas;
    struct BattleEffectWork *work;
    struct ReelWork *state;
    u8 *tiles;
    u8 *resource;
    u8 *source;
    u16 *shade;
    vu16 *map;
    s32 n;
    u16 *cells;
    s32 *record;
    s32 window;
    struct ReelRow *entry;
    Particle *particle;
    DrawRectangleFn routine[2];
    s32 pick[8];
    s32 flags[7];
    s32 row;
    s32 col;
    s32 cnt;
    s32 i;
    s32 j;
    s32 k;
    s32 rank;
    s32 frame;
    s32 phase;
    s32 half;
    s32 speed;
    s32 angle;
    s32 x;
    s32 y;
    s32 px;
    s32 py;
    s32 scale;

    sprites = (u8 *)Runtime_AllocateHeapBlock(41, 0x60E);
    canvas = (u8 *)Runtime_AllocateHeapBlock(40, 0x8000);
    work = Runtime_AllocateBlock(39, sizeof(struct BattleEffectWork));
    state = Runtime_AllocateBlock(45, sizeof(struct ReelWork));
    tiles = gMapCellBuffer;
    RuntimeDispatch_NoOpHook((s32)&ResourceId_EffectFarCalls);
    gReelSave.won_prizes[0] = -1;
    state->repeat = 0;
    state->bet = 1;
    Scheduler_ResetTaskTable();

    /* Screen block 5: a 20 x 32 name table whose interior rectangle is
       one shared tile and whose remaining cells run consecutively. */
    gOamCopyEnabled = 0;
    map = (vu16 *)0x06002800;
    cnt = 0;
    n = 0;
    for (row = 0; row != 20; row++) {
        for (col = 0; col != 32; col++) {
            if (col >= 5 && col <= 24 && row > 2 && row <= 13) {
                map[n] = 0xA1A6;
            } else if (col > 29) {
                map[n] = 0;
            } else {
                map[n] = 0xA1A8 + cnt;
                cnt++;
            }
            n++;
        }
    }

    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD), sprites);

    resource = (u8 *)Resource_GetTableEntry((s32)&ResourceId_SlotsFramePicture);
    Dma_Set(resource, (void *)0x05000140, 0x84000008,
        (volatile u32 *)0x040000d4);
    Resource_DecodeType01(resource + 32, tiles);

    /* Move the decompressed 8bpp tiles into the character block, leaving
       out the cells the shared interior tile already covers. */
    cnt = 0;
    for (row = 0; row != 20; row++) {
        source = tiles + ((row * 15) << 6);
        for (col = 0; col != 30; col++) {
            if (!(col >= 5 && col <= 24 && row > 2 && row <= 13)) {
                Dma_Set(source, (void *)(0x0600B500 + cnt), 0x84000008,
                    (volatile u32 *)0x040000d4);
                cnt += 32;
            }
            source += 32;
        }
    }

    _call_via_r3((void *)0x06002D00, 768, (s32)tiles, IwramClearWords);

    /* Screen block 6: consecutive tiles inside rows 2..16, one filler
       tile outside them. */
    map = (vu16 *)0x06003000;
    n = 0;
    for (row = 0; row != 20; row++) {
        for (col = 0; col != 32; col++) {
            if (row >= 2 && row <= 16) {
                map[n] = row * 32 + col + 148;
            } else {
                map[n] = 0xbf;
            }
            n++;
        }
    }

    *(vu16 *)0x0400000A = 0x0509;
    *(vu16 *)0x0400000C = 0x0680;
    cells = gBgScroll;
    cells[0] = 0;
    cells[1] = 0;
    cells[2] = 0;
    cells[3] = 0;
    cells[4] = 0;
    cells[5] = 0;
    *(vu16 *)0x04000048 = 0x3737;
    *(vu16 *)0x0400004A = 0x2727;
    *(vu16 *)0x04000050 = 0x3F44;
    *(vu16 *)0x04000052 = 0x1010;
    *(vu16 *)0x04000014 = 0;
    *(vu16 *)0x04000018 = 0;
    *(vu16 *)0x04000016 = 0xFF60;
    *(vu16 *)0x0400001A = 0xFF60;
    *(vu16 *)0x04000040 = 0x28C8;
    *(vu16 *)0x04000044 = 0x1878;
    *(vu16 *)0x04000040 = 0xf0;
    *(vu16 *)0x04000044 = 0xa0;
    *(vu16 *)0x04000042 = 0xf0;
    *(vu16 *)0x04000046 = 0xa0;

    shade = (u16 *)((u8 *)work + 0x200);
    state->state = 0;
    state->cursor = 0;
    state->spins = 0;
    work->reel_stop_frames = 0;
    state->timer = 0;

    resource = (u8 *)Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
    Dma_Set(resource, (void *)0x05000000, 0x84000020,
        (volatile u32 *)0x040000d4);
    *(vu16 *)0x05000080 = 0x2F8B;
    *(vu16 *)0x05000082 = 0x5BF6;

    resource = (u8 *)Resource_GetTableEntry((s32)&ResourceId_SlotsIcons);
    Dma_Set(resource, (void *)0x05000200, 0x84000078,
        (volatile u32 *)0x040000d4);
    Resource_DecodeType01(resource + 480, tiles);
    Dma_Set(tiles, (void *)0x06010000, 0x84001B30,
        (volatile u32 *)0x040000d4);

    resource = (u8 *)Resource_GetTableEntry((s32)&ResourceId_SlotsJackpot);
    Dma_Set(resource, (void *)0x050003E0, 0x84000008,
        (volatile u32 *)0x040000d4);
    Resource_DecodeType01(resource + 32, tiles);
    Dma_Set(tiles, (void *)0x06016E00, 0x84000480,
        (volatile u32 *)0x040000d4);
    FarCall_WindowTable();
    ReelGame_InitTitle();

    /* Seed the five records: 21 cells of 0..4 each. */
    entry = state->row;
    for (i = 0; i != 5; i++) {
        entry->pos = 8;
        entry->held = 0;
        entry->stop = -1;
        for (j = 0; j != 21; j++) {
            entry->cell[j] = Random16() % 5;
        }
        entry++;
    }

    /* Then overwrite eight distinct cells of each record with the ranks
       0, 1, 2, 3, 4, 5, 5, 5. */
    entry = state->row;
    for (i = 0; i != 5; i++) {
        for (j = 0; j != 8; j++) {
            pick[j] = Random16() % 21;
            for (k = 0; k != j; k++) {
                if (pick[j] == pick[k]) {
                    j--;
                    break;
                }
            }
        }
        for (j = 0; j != 8; j++) {
            rank = j;
            if (rank > 5) {
                rank = 5;
            }
            entry->cell[pick[j]] = rank;
        }
        entry++;
    }

    FarCall_EffectTable(46, 8, 7, 3, 2);
    routine[0] = (DrawRectangleFn)gWorkSlot[46];
    FarCall_EffectTable(47, 8, 7, 3, 3);
    routine[1] = (DrawRectangleFn)gWorkSlot[47];
    _call_via_r3(canvas, 0x8000, 0, IwramFillWords);

    Dma_Set(canvas, (void *)0x06003500, 0x84002000,
        (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000000, work, 0x84000080,
        (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000200, shade, 0x84000080,
        (volatile u32 *)0x040000d4);
    Graphics_ScaleRgb555(shade, (u16 *)0x05000200, 0, 256);
    Graphics_ScaleRgb555((u16 *)work, (u16 *)0x05000000, 0, 256);
    *(vu16 *)0x04000000 = 0x3740;

    if (PartyInventory_CountItemFar(228) == 1) {
        window = UiWindow_CreateFar(6, 16, 18, 3, 6);
        state->window = window;
        UiText_DrawCharacterAtOffsetFar(0x909, window, 0, 0);
    } else {
        window = UiWindow_CreateFar(2, 16, 26, 4, 6);
        state->window = window;
        UiText_DrawCharacterAtOffsetFar(0x908, window, 0, 0);
        UiText_DrawCharacterAtOffsetFar(0x909, state->window, 0, 8);
    }

    work->transfer_pending = 0;
    Scheduler_AddOrUpdateCallback(ReelGame_RunFrame, 1152);
    Scheduler_AddOrUpdateCallback(BattlePres_ProcessPendingTileTransfer, 1152);

    frame = 0;
    while (state->state != 10) {
        if (frame <= 16) {
            Graphics_ScaleRgb555(shade, (u16 *)0x05000200,
                frame << 12, 256);
            Graphics_ScaleRgb555((u16 *)work, (u16 *)0x05000000, frame << 12, 256);
        }

        if (state->state == 3) {
            phase = __modsi3(frame, 80);
            if (phase <= 15) {
                Palette_StepTowardResource((s32)&ResourceId_MercuryDjinnSheet);
            } else if (phase <= 31) {
                Palette_StepTowardResource((s32)&ResourceId_VenusDjinnSheet);
            } else if (phase <= 47) {
                Palette_StepTowardResource((s32)&ResourceId_EmberStreakSheet);
            } else if (phase <= 63) {
                Palette_StepTowardResource((s32)&ResourceId_LimePalette);
            } else {
                Palette_StepTowardResource((s32)&ResourceId_JupiterDjinnSheet);
            }

            if (state->timer <= 15) {
                Palette_DarkenSceneStep();
            }
            if (state->timer > 16 && (frame & 7) == 0) {
                x = (s32)(((Random16() & 0x7F) + 56) << 16);
                y = (s32)(((Random16() & 0x1F) + 48) << 16);
                particle = (Particle *)tiles + ((frame / 8) & 3) * 256;
                for (i = 0; i != 256; i++) {
                    speed = (s32)(Random16() & 0xFF) + 64;
                    angle = (s32)(Random16() & 0xFFFF);
                    particle->x = x;
                    particle->y = y;
                    particle->vx = (speed * Trig_Sin(angle)) >> 6;
                    particle->vy = (-(speed * Trig_Cos(angle))) >> 6;
                    particle->life = (s32)(Random16() & 0xF) + 16;
                    particle++;
                }
            }

            particle = (Particle *)tiles;
            for (i = 0; i != 1024; i++) {
                if (particle->life > 0) {
                    px = particle->x;
                    particle->life--;
                    if ((u32)px <= 0x00FFFFFF) {
                        py = particle->y;
                        if (py <= 0x007FFFFF && py >= 0) {
                            half = __divsi3(particle->life, 12) + 1;
                            routine[i & 1](canvas,
                                sprites + Data_080f86f8[half - 1],
                                (px >> 16) - half,
                                (py >> 16) - half,
                                half * 2, half * 2);
                        }
                    }
                    particle->x += particle->vx;
                    particle->y += particle->vy;
                    particle->vx = particle->vx * 60 / 64;
                    particle->vy = particle->vy * 60 / 64;
                }
                particle++;
            }
        }

        if (state->state == 0
            || state->state == 2) {
            for (i = 0; i != 7; i++) {
                flags[i] = 0;
            }
            if (state->state == 0) {
                flags[3] = 1;
                if (state->bet > 1) {
                    flags[4] = 1;
                    flags[2] = 1;
                }
                if (state->bet > 2) {
                    flags[5] = 1;
                    flags[1] = 1;
                }
                if (state->bet > 3) {
                    flags[6] = 1;
                    flags[0] = 1;
                }
            } else if ((frame & 7) <= 3) {
                record = state->line;
                for (i = 0; i != 7; i++) {
                    flags[i] = *record;
                    record++;
                }
            }

            for (k = 0; k != 3; k++) {
                rank = 65 - (k != 1);
                if (flags[1] != 0) {
                    BattleFx_DrawCanvasLine(20, k + 19, 200, k + 19, rank);
                }
                if (flags[2] != 0) {
                    BattleFx_DrawCanvasLine(28, k + 35, 200, k + 35, rank);
                }
                if (flags[3] != 0) {
                    BattleFx_DrawCanvasLine(20, k + 51, 200, k + 51, rank);
                }
                if (flags[4] != 0) {
                    BattleFx_DrawCanvasLine(28, k + 67, 200, k + 67, rank);
                }
                if (flags[5] != 0) {
                    BattleFx_DrawCanvasLine(20, k + 83, 200, k + 83, rank);
                }
                if (flags[0] != 0) {
                    BattleFx_DrawCanvasLine(28, k + 5, 200, k + 91, rank);
                }
                if (flags[6] != 0) {
                    BattleFx_DrawCanvasLine(28, k + 97, 200, k + 11, rank);
                }
            }
        }

        work->transfer_pending = 1;
        WaitFrames(1);
        frame++;
    }

    for (i = 0; i != 17; i++) {
        scale = 0x10000 - (i << 12);
        Graphics_ScaleRgb555(shade, (u16 *)0x05000200, scale, 256);
        Graphics_ScaleRgb555((u16 *)work, (u16 *)0x05000000, scale, 256);
        WaitFrames(1);
    }

    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback(BattlePres_ProcessPendingTileTransfer);
    Scheduler_RemoveCallback(ReelGame_RunFrame);
    Runtime_ReleaseHeapBlock(45);
    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
    Runtime_ReleaseHeapBlock(41);
}
