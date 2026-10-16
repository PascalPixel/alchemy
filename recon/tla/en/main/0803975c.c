// Near miss, differ 2 of 60 (one reordered pair): The Broken Seal's
// UiWork_ShiftPanelRowsLeft with this game's 30 rows at 0x06002500. The native
// loads the first row's address between the two instructions of step * 3 (lsls
// r3, r0, #1; ldr r6; adds r3, r3, r0); this adds first. The other declaration
// order measured 2, a one-pass block around the shift 10, and three minutes of
// alchemy permute found nothing better.
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "window.h"
#include "ram_buffer.h"
#include "resource.h"
#include "iwram_call.h"

#include "dma.h"
#include "io_reg.h"

#define PANEL_TILE_ROWS ((u32 *)0x06002500)
#define PANEL_ROW_COUNT 30
#define PANEL_ROW_WORDS 32
#define PANEL_ROW_LEAD 8

// Shifts each panel row left by step characters.
void UiWork_ShiftPanelRowsLeft(s32 step) {
  s32 shift = step * 6;
  u32 *base = PANEL_TILE_ROWS;
  s32 n;

  for (n = 0; n < PANEL_ROW_COUNT; n++) {
    u32 *row = base + n * PANEL_ROW_WORDS;

    Dma_Set(row + PANEL_ROW_LEAD + shift, row + PANEL_ROW_LEAD,
            0x84000000 | (PANEL_ROW_WORDS - PANEL_ROW_LEAD - shift), REG_DMA3);
    Iwram_FillWords(row + (PANEL_ROW_WORDS - shift), shift * 4, 0);
  }
}
