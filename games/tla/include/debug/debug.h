#ifndef ALCHEMY_DEBUG_H
#define ALCHEMY_DEBUG_H

/// @file
/// DEBUG

#include "types.h"
#include "edition.h"

s32 Scene_ApplyEntrance(s32);
s32 Field_PlaceAtEntrance(void);

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
#define BROWSER_ROW_STEP 10
/// The same choosers' R and L move thirty.
#define BROWSER_PAGE_STEP 30

void DebugWarp_Jupiter(void);
void DebugWarp_Arafura2(void);
void DebugWarp_EnergySample(void);
void DebugWarp_Arafura3(void);
void DebugWarp_Champa2(void);
void DebugWarp_KaoruSpecial4(void);
void DebugWarp_KaoruSpecial3(void);
void DebugWarp_KaoruSpecial2(void);
void DebugWarp_KaoruSpecial1(void);

#endif
