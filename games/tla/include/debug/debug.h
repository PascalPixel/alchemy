#ifndef ALCHEMY_DEBUG_H
#define ALCHEMY_DEBUG_H

/// @file
/// What the debug menus and the debug rooms share: the warp menu's presets,
/// the shortcut grid's abilities, the palette editor's swatches and the steps
/// the debug choosers take.

#include "types.h"
#include "edition.h"

/// The palette editor's swatch tiles, compressed; it decompresses them to
/// tile 208 of character block 0, one tile a colour.
extern const u8 gDebug_PaletteSwatchTiles[];

/// One row of the debug warp menu: its name and what choosing it does.
struct DebugWarpPreset {
  const u8 *name;
  // UB: the handlers return nothing; DebugWarp_Run calls them as returning
  // s32.
#ifdef UBFIX
  void (*warp)(void);
#else
  s32 (*warp)(void);
#endif
};

/// The debug warp menu's nine rows (debug_tables.c).
extern const struct DebugWarpPreset gDebugWarp_Presets[];
/// The abilities the debug shortcut grid offers, by column and row.
extern const s16 gDebugShortcut_Abilities[4][9];

/// The debug rooms' item choosers (IncreaseItemAndLevel_SelectItem,
/// ShopTest_SelectItem): Right and Left move ten numbers.
#define DEBUG_BROWSER_ROW_STEP 10
/// The same choosers' R and L move thirty.
#define DEBUG_BROWSER_PAGE_STEP 30
/// The step the debug menus take on L and R: ten scenes, entrances, flag
/// pages, portraits or sound numbers (window.c's choosers); the battle test
/// chooser (battle_debug.c) takes it on Up and Down, ten encounters.
#define DEBUG_LARGE_STEP 10

/// The warp menu's handlers, one a row of gDebugWarp_Presets, each named for
/// its row's label ("0:Jupiter" to "8:Kaoru Special1").
void DebugWarp_Jupiter(void);
void DebugWarp_Arafura2(void);
void DebugWarp_EnergySample(void);
void DebugWarp_Arafura3(void);
void DebugWarp_Champa2(void);
void DebugWarp_KaoruSpecial4(void);
void DebugWarp_KaoruSpecial3(void);
void DebugWarp_KaoruSpecial2(void);
void DebugWarp_KaoruSpecial1(void);

void DebugFlagEditor_DrawTable(s32 window, s32 startFlag);
s32 DebugFlagEditor_HandleInput(s32 window, s32 *page, s32 *cursor);

#endif
