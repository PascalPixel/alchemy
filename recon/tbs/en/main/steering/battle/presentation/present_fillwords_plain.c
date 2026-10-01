/* NONMATCHING: 2026-10-01 brief Wave2 FillWords plain-source attempt.
 * Removing this one source device changes BattlePresentation_BuildTilemap.
 * Remaining difference: a direct call changes BattlePresentation_BuildTilemap from push {r5, r6, lr} to push {r5, r6, r7, lr} (45/44 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * Production retains the measured helper with a body-local FAKEMATCH reason.
 */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "SCENE.H"
#include "RAM_BUFFER.H"


/* Heap-allocation cache: Data_03001e50[kind] holds kind's block. Kind 44
   is the presentation state, kind 10 its scroll records. */
extern void *Data_03001e50[];

extern u8 gDisp[];

/* Eight 4bpp tiles of a bar that narrows by a column from one to the next. */
extern const u8 BattlePres_TileVariants[];

s32 BattlePres_SetupTransitionScene(s32, s32, s32, s32);

/* battle/presentation/trans/timer.c */
struct Display080c01bc {
  u8 padding_00[0x36];
  s16 field_36;
};

struct Position080c01bc {
  s16 field_00;
  s16 field_02;
};

/* Builds the battle presentation tilemap: 256 blank words, 128 border words,
   240 words of sequential tile pairs from 0x0201, then 640 border words. */
void BattlePresentation_BuildTilemap(s32 *destination)
{
    s32 entry;
    u32 index;

    Iwram_FillWords(destination, 0x100, -1);
    destination += 0x40;
    Iwram_FillWords(destination, 0x80, 0x03ff03ff);
    entry = (0x0201 << 16) | 0x0200;
    destination += 0x20;
    index = 0;
    do {
        index++;
        *destination++ = entry;
        entry += 0x00020002;
    } while (index <= 239);
    Iwram_FillWords(destination, 0x280, 0x03ff03ff);
}

/* While the presentation mode is 2, restart the H-blank DMA that feeds
   the BG2 control register from the current scroll record, and copy
   that record's affine parameters. */
void BattlePres_UpdateHBlankScroll(void)
;

void BattlePresentation_UploadTileVariant(void)
;

/* Main-image symbols: every pool word inside the ROM or the work RAM. */
void BattlePres_AdvanceTransitionTimer(void)
;

/* battle/presentation/trans/draw_rows.c */
void BattlePres_DrawTransitionRows(void)
;

void Graphics_ClearBg0Vofs(void)
;
