#include "TYPES.H"
#include "FIXED_MATH.H"
#include "UI.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "GAME_STATE.H"
#include "IWRAM_CALL.H"
#include "MAP_SCROLL.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "SYSTEM.H"
#include "TBS_EDITION.H"

/* One of the five reels: where it stands, its twenty-one symbols, whether
   it is held and where it is to stop. */
struct Reel {
    s32 position;
    u8 symbols[21];
    u8 held;
    u8 stop;
};

/* The reel game's state block, the seventh heap-cache cell. */
struct ReelWork {
    struct Reel reels[5];
    s32 state;
    s32 cursor;
    s32 spins;
    s32 bet;
    u16 keys;
    u16 direction;
    u16 pressed;
    u16 repeat;
    u8 unknown_0a4[4];
    s32 timer;
    s32 winning_lines[7];
    u8 unknown_0c8[0x400];
    s32 window;
    s32 sub_window;
    u8 unknown_4d0[8];
    u16 scanline_offsets[160];
#if EDITION_INTERNATIONAL
    /* The localized editions' block is one word longer. */
    s32 unknown_618;
#endif
};

extern u8 gBattleFxWork[];
extern char MsgSlotsBet;
extern const u8 ReelGame_TitleLetterWidths[];

/* Reel game: clear every particle's variant, line the title letters
   up above the screen (each 0x80000 higher than the last, spaced by their
   widths), build the scanline offset curve (zero outside a cosine bump
   mirrored about line 86), reset the state, cursor and spin count, and open
   the two-line coin window. */
void ReelGame_InitTitle(void)
{
    struct ReelWork *state;
    struct BattleEffectWork *work;
    struct EffectStep *letter;
    const u8 *width;
    s32 left;
    s32 step;
    s32 y;
    s32 i;
    s32 angle;
    s32 window;
    s32 message;

    state = ((struct ReelWork **)gBattleFxWork)[6];
    work = ((struct BattleEffectWork **)gBattleFxWork)[0];
    left = 0;
    for (i = 0; i != 0x800; i++)
        ((struct EffectStep *)Ram_MapCellBuffer)[i].variant = 0;

    letter = work->particles;
    y = -0x200000;
    width = ReelGame_TitleLetterWidths;
    for (i = 0; i != REEL_TITLE_LETTERS; i++) {
        letter[i].x = (left + REEL_TITLE_X) << 16;
        step = *width;
        width++;
        left += step;
        letter[i].y = y;
        letter[i].velocity_y = 0;
        letter[i].variant = 0;
        y += -0x80000;
    }

    for (i = 0; i != 160; i++)
        state->scanline_offsets[i] = 0;

    for (i = 0; i != 40; i++) {
        angle = i * 0x199;
        state->scanline_offsets[23 + i] = (u32)(Trig_Cos(angle) * 3) >> 15;
        state->scanline_offsets[110 - i] = (u32)(Trig_Cos(angle) * 3) >> 15;
    }

    state->spins = 0;
    state->state = 0;
    state->cursor = 0;
    work->transfer_mode = 1;
    work->transfer_value = 0;
    *(volatile u16 *)0x04000050 = 0;
    window = UiWindow_CreateFar(18, 0, 12, 4, 6);
    state->sub_window = window;
    message = (s32)&MsgSlotsBet;
    UiText_DrawCharacterAtOffsetFar(message, window, 0, 8);
    UiText_DrawCharacterAtOffsetFar(message - 1, state->sub_window, 0, 0);
}

/* The saved game from its fourth word on, as the reel game reads it. */
struct ReelSave {
    u8 unknown_000[0x120];
    s8 won_prizes[16];
};

extern u8 gOamCopyEnabled;
extern DrawRectangle gWorkSlot[];
extern u16 ReelGame_SparkCellOffsets[];
extern char MsgSlotsChangeBet;
extern char MsgSlotsControls;

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void RuntimeDispatch_NoOpHook(s32 resource);
u32 Resource_DecodeType01(const void *source, void *destination);
void FarCall_WindowTable(void);
void ReelGame_InitTitle(void);
s32 FarCall_EffectTable(s32 slot, s32 width_shift, s32 height_shift, s32 flags, u32 mode);
s32 Graphics_ScaleRgb555(u16 *source, u16 *destination, s32 scale, s32 count);
s32 PartyInventory_CountItemFar(s32 item);
void Palette_DarkenSceneStep(void);
void Palette_StepTowardResource(s32 resource);
void BattleFx_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);
void Unnamed_080f6440(void);
void BattlePres_ProcessPendingTileTransfer(void);

#define REG_DMA3 ((volatile u32 *)0x040000d4)

/* The sparks live at the start of the map cell buffer. */
#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)

/* The reel game. Builds the machine's two backgrounds and its sprites,
   deals each reel twenty-one symbols with eight distinct places for the
   ranked ones, fades in, and then draws the bet lines or the winning lines
   and the win's sparks every frame until the game's state says it is over;
   then fades out and frees everything. */
void ReelGame_Run(void)
{
    s32 picks[8];
    s32 lit[7];
    DrawRectangle draw[2];
    u8 *sheet;
    void *canvas;
    struct BattleEffectWork *work;
    struct ReelWork *state;
    struct ReelSave *save;
    u8 *entry;
    u8 *tiles;
    s32 frame;
    u32 count;
    s32 i;
    s32 j;
    s32 k;

    sheet = Runtime_AllocateHeapBlock(41, 0x60e);
    canvas = Runtime_AllocateHeapBlock(40, 0x8000);
    work = Runtime_AllocateBlock(39, sizeof(struct BattleEffectWork));
    state = Runtime_AllocateBlock(45, sizeof(struct ReelWork));
    tiles = Ram_MapCellBuffer;
    save = (struct ReelSave *)((u8 *)&gGameState + 12);
    RuntimeDispatch_NoOpHook((s32)&ResourceId_EffectFarCalls);
    save->won_prizes[0] = -1;
    state->repeat = 0;
    state->bet = 1;
    Scheduler_ResetTaskTable();
    gOamCopyEnabled = 0;

    count = 0;
    j = 0;
    for (k = 0; k != 20; k++) {
        for (i = 0; i != 32; i++, j += 2) {
            if (i >= 5 && i <= 24 && k > 2 && k <= 13) {
                *(volatile u16 *)(0x06002800 + j) = 0xa1a6;
            } else if (i > 29) {
                *(volatile u16 *)(0x06002800 + j) = 0;
            } else {
                *(volatile u16 *)(0x06002800 + j) = 0xa1a8 + count;
                count++;
            }
        }
    }
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD), sheet);
    entry = Resource_GetTableEntry((s32)&ResourceId_SlotsFramePicture);
    Dma_Set(entry, (void *)0x05000140, 0x84000008, REG_DMA3);
    entry += 32;
    Resource_DecodeType01(entry, tiles);
    j = 0;
    for (k = 0; k != 20; k++) {
        for (i = 0; i != 30; i++) {
            if (!(i >= 5 && i <= 24 && k > 2 && k <= 13)) {
                Dma_Set(tiles + (k * 30 + i) * 32,
                    (void *)(0x0600b500 + j), 0x84000008, REG_DMA3);
                j += 32;
            }
        }
    }
    Iwram_ClearWords((void *)0x06002d00, 0x300);
    j = 0;
    for (k = 0; k != 20; k++) {
        for (i = 0; i != 32; i++, j += 2) {
            if (k < 2 || k > 16)
                *(volatile u16 *)(0x06003000 + j) = 0xbf;
            else
                *(volatile u16 *)(0x06003000 + j) = k * 32 + i + 148;
        }
    }
    *(volatile u16 *)0x0400000a = 0x509;
    *(volatile u16 *)0x0400000c = 0x680;
    gBgScroll[0].x = 0;
    gBgScroll[0].y = 0;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 0;
    gBgScroll[2].x = 0;
    gBgScroll[2].y = 0;
    *(volatile u16 *)0x04000048 = 0x3737;
    *(volatile u16 *)0x0400004a = 0x2727;
    *(volatile u16 *)0x04000050 = 0x3f44;
    *(volatile u16 *)0x04000052 = 0x1010;
    *(volatile u16 *)0x04000014 = 0;
    *(volatile u16 *)0x04000018 = 0;
    *(volatile u16 *)0x04000016 = 0xff60;
    *(volatile u16 *)0x0400001a = 0xff60;
    *(volatile u16 *)0x04000040 = 0x28c8;
    *(volatile u16 *)0x04000044 = 0x1878;
    *(volatile u16 *)0x04000040 = 0xf0;
    *(volatile u16 *)0x04000044 = 0xa0;
    *(volatile u16 *)0x04000042 = 0xf0;
    *(volatile u16 *)0x04000046 = 0xa0;
    /* The saved sprite palette and the canvas size are declared here, once
       the display is set: the stack shows them made after the bet's
       address and before the state's. */
    {
        u16 *shade = (u16 *)((u8 *)work + 0x200);
        s32 size = 0x8000;

        state->state = 0;
        state->cursor = 0;
        state->spins = 0;
        work->frame = 0;
        state->timer = 0;
        Dma_Set(Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet),
            (void *)0x05000000, 0x84000020, REG_DMA3);
        *(u16 *)0x05000080 = 0x2f8b;
        *(u16 *)0x05000082 = 0x5bf6;
        entry = Resource_GetTableEntry((s32)&ResourceId_SlotsIcons);
        Dma_Set(entry, (void *)0x05000200, 0x84000078, REG_DMA3);
        entry += 480;
        Resource_DecodeType01(entry, tiles);
        Dma_Set(tiles, (void *)0x06010000, 0x84001b30, REG_DMA3);
        entry = Resource_GetTableEntry((s32)&ResourceId_SlotsJackpot);
        Dma_Set(entry, (void *)0x050003e0, 0x84000008, REG_DMA3);
        entry += 32;
        Resource_DecodeType01(entry, tiles);
        Dma_Set(tiles, (void *)REEL_JACKPOT_TILES, REEL_JACKPOT_COPY, REG_DMA3);
        FarCall_WindowTable();
        ReelGame_InitTitle();

        for (i = 0; i != 5; i++) {
            struct Reel *reel = &state->reels[i];

            reel->position = 8;
            reel->held = 0;
            reel->stop = 255;
            for (k = 0; k != 21; k++)
                reel->symbols[k] = Random16() % 5;
        }
        for (i = 0; i != 5; i++) {
            struct Reel *reel = &state->reels[i];

            for (j = 0; j != 8; j++) {
                picks[j] = Random16() % 21;
                for (k = 0; k != j; k++) {
                    if (picks[j] == picks[k]) {
                        j--;
                        break;
                    }
                }
            }
            for (k = 0; k != 8; k++) {
                s32 rank = k;

                if (rank > 5)
                    rank = 5;
                reel->symbols[picks[k]] = rank;
            }
        }

        FarCall_EffectTable(46, 8, 7, 3, 2);
        draw[0] = gWorkSlot[46];
        FarCall_EffectTable(47, 8, 7, 3, 3);
        draw[1] = gWorkSlot[47];
        Iwram_FillWords(canvas, size, 0);
        Dma_Set(canvas, (void *)0x06003500, 0x84002000, REG_DMA3);
        Dma_Set((void *)0x05000000, work, 0x84000080, REG_DMA3);
        Dma_Set((void *)0x05000200, shade, 0x84000080, REG_DMA3);
        Graphics_ScaleRgb555(shade, (u16 *)0x05000200, 0, 0x100);
        Graphics_ScaleRgb555((u16 *)work, (u16 *)0x05000000, 0, 0x100);
        *(volatile u16 *)0x04000000 = 0x3740;
        if (PartyInventory_CountItemFar(228) == 1) {
            s32 window = UiWindow_CreateFar(6, 16, 18, 3, 6);

            state->window = window;
            UiText_DrawCharacterAtOffsetFar((s32)&MsgSlotsControls, window, 0, 0);
        } else {
            s32 window = UiWindow_CreateFar(REEL_HELP_WINDOW_X, 16, REEL_HELP_WINDOW_WIDTH, 4, 6);
            s32 message;

            state->window = window;
            message = (s32)&MsgSlotsChangeBet;
            UiText_DrawCharacterAtOffsetFar(message, window, 0, 0);
            UiText_DrawCharacterAtOffsetFar(message + 1, state->window, 0, 8);
        }
        work->transfer_pending = 0;
        Scheduler_AddOrUpdateCallback((s32)Unnamed_080f6440, 0x480);
        Scheduler_AddOrUpdateCallback((s32)BattlePres_ProcessPendingTileTransfer, 0x480);

        for (frame = 0; state->state != 10; frame++) {
            if (frame <= 16) {
                Graphics_ScaleRgb555(shade, (u16 *)0x05000200, frame << 12, 0x100);
                Graphics_ScaleRgb555((u16 *)work, (u16 *)0x05000000, frame << 12, 0x100);
            }
            if (state->state == 3) {
                s32 phase = frame % 80;

                if (phase <= 15)
                    Palette_StepTowardResource((s32)&ResourceId_MercuryDjinnSheet);
                else if (phase <= 31)
                    Palette_StepTowardResource((s32)&ResourceId_VenusDjinnSheet);
                else if (phase <= 47)
                    Palette_StepTowardResource((s32)&ResourceId_EmberStreakSheet);
                else if (phase <= 63)
                    Palette_StepTowardResource((s32)&ResourceId_LimePalette);
                else
                    Palette_StepTowardResource((s32)&ResourceId_JupiterDjinnSheet);
                if (state->timer <= 15)
                    Palette_DarkenSceneStep();
                if (state->timer > 16 && (frame & 7) == 0) {
                    s32 x = (Random16() & 127) + 56;
                    s32 y = (Random16() & 31) + 48;
                    struct EffectStep *spark = &SPARKS[(frame / 8 & 3) * 256];

                    for (j = 0; j != 256; j++) {
                        s32 speed;
                        s32 angle;

                        speed = 255;
                        speed &= Random16();
                        angle = Random16() & 0xffff;
                        spark->x = x << 16;
                        spark->y = y << 16;
                        spark->velocity_x = Trig_Sin(angle) * (speed + 64) >> 6;
                        spark->velocity_y = -(Trig_Cos(angle) * (speed + 64)) >> 6;
                        spark->variant = (Random16() & 15) + 16;
                        spark++;
                    }
                }
                for (j = 0; j != 1024; j++) {
                    struct EffectStep *spark = &SPARKS[j];

                    if (spark->variant > 0) {
                        s32 x;

                        spark->variant--;
                        x = spark->x;
                        if ((u32)x <= 0xffffff) {
                            s32 y = spark->y;

                            if (y <= 0x7fffff) {
                                if (y >= 0) {
                                    s32 px = x >> 16;
                                    s32 py = y >> 16;
                                    s32 size = spark->variant / 12 + 1;

                                    draw[j & 1](canvas, sheet + ReelGame_SparkCellOffsets[size - 1],
                                        px - size, py - size, size * 2, size * 2);
                                }
                            }
                        }
                        spark->x += spark->velocity_x;
                        spark->y += spark->velocity_y;
                        spark->velocity_x = spark->velocity_x * 60 / 64;
                        spark->velocity_y = spark->velocity_y * 60 / 64;
                    }
                }
            }
            if (state->state == 0 || state->state == 2) {
                for (j = 0; j != 7; j++)
                    lit[j] = 0;
                if (state->state == 0) {
                    lit[3] = 1;
                    if (state->bet > 1) {
                        lit[4] = 1;
                        lit[2] = 1;
                    }
                    if (state->bet > 2) {
                        lit[5] = 1;
                        lit[1] = 1;
                    }
                    if (state->bet > 3) {
                        lit[6] = 1;
                        lit[0] = 1;
                    }
                } else if ((frame & 7) <= 3) {
                    for (j = 0; j != 7; j++)
                        lit[j] = state->winning_lines[j];
                }
                for (j = 0; j != 3; j++) {
                    s32 color = j != 1;

                    color = 65 - color;
                    if (lit[1])
                        BattleFx_DrawCanvasLine(20, j + 19, 200, j + 19, color);
                    if (lit[2])
                        BattleFx_DrawCanvasLine(28, j + 35, 200, j + 35, color);
                    if (lit[3])
                        BattleFx_DrawCanvasLine(20, j + 51, 200, j + 51, color);
                    if (lit[4])
                        BattleFx_DrawCanvasLine(28, j + 67, 200, j + 67, color);
                    if (lit[5])
                        BattleFx_DrawCanvasLine(20, j + 83, 200, j + 83, color);
                    if (lit[0])
                        BattleFx_DrawCanvasLine(28, j + 5, 200, j + 91, color);
                    if (lit[6])
                        BattleFx_DrawCanvasLine(28, j + 97, 200, j + 11, color);
                }
            }
            work->transfer_pending = 1;
            WaitFrames(1);
        }

        for (j = 0; j != 17; j++) {
            Graphics_ScaleRgb555(shade, (u16 *)0x05000200, 0x10000 - (j << 12), 0x100);
            Graphics_ScaleRgb555((u16 *)work, (u16 *)0x05000000, 0x10000 - (j << 12), 0x100);
            WaitFrames(1);
        }
        Runtime_ReleaseHeapBlock(47);
        Runtime_ReleaseHeapBlock(46);
        Scheduler_RemoveCallback((u32)BattlePres_ProcessPendingTileTransfer);
        Scheduler_RemoveCallback((u32)Unnamed_080f6440);
        Runtime_ReleaseHeapBlock(45);
        Runtime_ReleaseHeapBlock(40);
        Runtime_ReleaseHeapBlock(39);
        Runtime_ReleaseHeapBlock(41);
    }
}
