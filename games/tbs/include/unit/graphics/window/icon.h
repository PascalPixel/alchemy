#ifndef ALCHEMY_UNIT_GRAPHICS_WINDOW_ICON_H
#define ALCHEMY_UNIT_GRAPHICS_WINDOW_ICON_H

#include "types.h"
#include "edition.h"
#include "glyph.h"

s32 Ui_CountIconTableEntries(void);
s32 Ui_CountSecondTableEntries(void);
void Ui_BuildPairedPatternsToSlot(s32 no0, s32 no1, s32 *slot, s32 *ret, s32 flag);
void Ability_RequestGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse);
void UiIcon_BuildItemIconTiles(u32 glyph, s32 with_base, s32 *src, s32 *dst, s32 reuse);
s32 ItemIcon_Compose(u32 code, u32 layers);
s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2);
s32 Ui_BuildPatternToSlot(s32 icon, s32 unused, s32 slot);
s32 RenderResource_LoadTableEntry(u32 value, s32 unused, u32 slot);
void Ui_PrepareTransferForItem(s32 code);
void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse);
void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 with_base, s32 *src, s32 *dst, s32 reuse);
void Ui_PrepareTransferFromTableEntry(u32 index);
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile, s32 palette, s32 reuse);
void UiGlyph_ReservedNoOp(void);
void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *glyph, s32 outlined);

#endif
