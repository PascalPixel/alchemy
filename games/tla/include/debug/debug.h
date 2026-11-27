#ifndef ALCHEMY_DEBUG_H
#define ALCHEMY_DEBUG_H

// What the debug menus and the debug rooms share: the warp menu's presets, the shortcut grid's
// abilities, the palette editor's swatches and the steps the debug choosers take.

#include "types.h"
#include "edition.h"

// The palette editor's swatch tiles, compressed; it decompresses them to tile 208 of character
// block 0, one tile a colour.
extern const Uint8 DebugPaletteSwatchTiles[];

// One row of the debug warp menu: its name and what choosing it does.
typedef struct DEBUG_WARP_PRESET
{
    const Uint8 *name;
    // UB: the handlers return nothing; mnDebugWarpRun calls them as returning Sint32.
#if FIX_FUNCTION_SIGNATURES
    void (*warp)(void);
#else
    Sint32 (*warp)(void);
#endif
} DEBUG_WARP_PRESET;

// The debug warp menu's nine rows (debug_tables.c).
extern const DEBUG_WARP_PRESET DebugWarpPresets[];
// The abilities the debug shortcut grid offers, by column and row.
extern const Sint16            DebugShortcutAbilities[4][9];

// The debug rooms' item choosers (dbIncreaseItemAndLevelSelectItem, dbShopTestSelectItem): Right
// and Left move ten numbers.
#define DEBUG_BROWSER_ROW_STEP  10
// The same choosers' R and L move thirty.
#define DEBUG_BROWSER_PAGE_STEP 30
// The step the debug menus take on L and R: ten scenes, entrances, flag pages, portraits or sound
// numbers (window.c's choosers); the battle test chooser (battle_debug.c) takes it on Up and Down,
// ten encounters.
#define DEBUG_LARGE_STEP        10

// The warp menu's handlers, one a row of DebugWarpPresets, each named for its row's label
// ("0:Jupiter" to "8:Kaoru Special1").
void   mnDebugWarpJupiter(void);
void   mnDebugWarpArafura2(void);
void   mnDebugWarpEnergySample(void);
void   mnDebugWarpArafura3(void);
void   mnDebugWarpChampa2(void);
void   mnDebugWarpKaoruSpecial4(void);
void   mnDebugWarpKaoruSpecial3(void);
void   mnDebugWarpKaoruSpecial2(void);
void   mnDebugWarpKaoruSpecial1(void);

void   mnDebugFlagEditorDrawTable(Sint32 window, Sint32 startFlag);
Sint32 mnDebugFlagEditorHandleInput(Sint32 window, Sint32 *page, Sint32 *cursor);

#endif
