/* NONMATCHING: 2026-10-01 brief Wave2 FillWords plain-source attempt.
 * Removing this one source device changes Menu_OpenBackdropScreen.
 * First remaining difference: Menu_OpenBackdropScreen: mov	r1, #128 => ldr	r2, .L0+28 (109/109 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * This reduced draft preserves the affected function and its declarations.
 * Production retains the measured device with its FAKEMATCH reason.
 */
#include "TYPES.H"
#include "SCENE.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "IO_REG.H"
#include "OWNER_STATE.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

/* The tile in character block 1 that holds the window frame. */
#define FRAME_TILE 150
extern u8 gMenuWork[];

/* The frame tile the backdrop screen loads into BG character block 1. */
extern const u8 Menu_BackdropFrameTile[];
typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);
typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

struct BackdropSave {
    u8 unknown_000[0xa8];
    u8 tiles[0x2000];
    u16 palette[64];
};

struct MenuWork {
    u8 unknown_000[0x30];
    s32 window;
    u8 unknown_034[0x150];
    struct BackdropSave *backdrop;
};

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
    /* FAKEMATCH: direct calls add r9, sl and fp saves and change the backdrop stack layout. */
    return copy(dst, src, size);
}


s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void Func_080153d8(void *);
void *Runtime_GetLowTableAddress(void);
void Graphics_AdjustPaletteBank(s32);
s32 Func_080aafb8(struct BackdropSave *);
#define ACTION_MASK 0x3fff
#define FLAG_FIRST 0x8000
#define FLAG_SECOND 0x4000
extern u8 Data_03001f2c[];
s16 Djinn_ListOwnerEntries(void *, s32, s32);

/* menu/core/compute_entry_values.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Graphics_AdjustPaletteBank(s32 arg0)
;

/* Opens the full-width menu window: saves BG character block 1 and
   palettes 4-7 into the backdrop buffer, blanks them with fill patterns,
   loads the frame tile and palettes, then runs the backdrop screen. */
s32 Menu_OpenBackdropScreen(void)
{
    struct MenuWork *work = *(struct MenuWork **)gMenuWork;
    struct BackdropSave *backdrop = work->backdrop;

    UiWindow_UpdateOrCreate(&work->window, 0, 5, 30, 15, 2);
    WaitFrames(1);
    CopyWords(Iwram_CopyWords, backdrop->tiles, BG_CHAR_BLOCK(1), 0x2000);
    CopyWords(Iwram_CopyWords, backdrop->palette, (void *)&BG_PLTT_COLOR(4, 0), 128);
    Iwram_FillWords(BG_CHAR_BLOCK(1), 0x2000, 0x33333333);
    Iwram_FillWords((void *)&BG_PLTT_COLOR(4, 0), 128, 0x55555555);
    Func_080153d8(BG_CHAR_BLOCK(1) + 0x1000);
    CopyWords(Iwram_CopyWords, BG_CHAR_BLOCK(1) + FRAME_TILE * TILE_SIZE_4BPP,
              Menu_BackdropFrameTile, TILE_SIZE_4BPP);
    Dma_Set(Runtime_GetLowTableAddress(), (void *)&BG_PLTT_COLOR(5, 0), 0x80000010, REG_DMA3);
    BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
    Dma_Set((void *)&BG_PLTT_COLOR(15, 0), (void *)&BG_PLTT_COLOR(7, 0), 0x80000010, REG_DMA3);
    Graphics_AdjustPaletteBank(8);
    BG_PLTT_COLOR(7, 4) = BG_PLTT_COLOR(15, 4);
    BG_PLTT_COLOR(6, 4) = BG_PLTT_COLOR(15, 4);
    return Func_080aafb8(backdrop);
}

/* Lists all slots in b, marks those absent from a, then appends slots only in a. */
s32 OwnerAction_DiffSlots(struct OwnerActionSlot *a, struct OwnerActionSlot *b,
                          u16 *out, s32 *first_count, s32 *second_count)
;

s32 Menu_ComputeEntryValues(void *tbl)
;
