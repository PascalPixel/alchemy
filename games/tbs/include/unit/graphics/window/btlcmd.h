#ifndef ALCHEMY_UNIT_GRAPHICS_WINDOW_BTLCMD_H
#define ALCHEMY_UNIT_GRAPHICS_WINDOW_BTLCMD_H

#include "types.h"
#include "edition.h"
#include "glyph.h"

struct BattleActionRecord;
struct BtlAct;
struct GlyphPalette;
struct OwnerActionSlot;
struct RenderInput;
struct SelectionWork;
struct UiWindow;

s32 Menu_BuildLocalizedPatternTiles(void);
int UiWindow_DrawThreeTileColumn(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 unused);
void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output, s32 shift);
void Link_DrawShiftedTilePair(s32 offset);
void Graphics_ExpandVramTilesByColorTable(u16 *dst);
s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index);
s32 Resource_LoadKind26EntryToBuffer(s32 resource, s32 index);
s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index);
s32 Ui_LoadEntryForKind(u32 kind, s32 value);
s32 Ui_GetTableWordZero(s32 no);
void Ui_RunCopiedRoutine(GlyphTransfer *work);
s32 Link_CreateCountdownLabelWindow(void);
s32 Resource_LoadIndexedIntoBuffer(s32 arg0, s32 arg1);
void UiText_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph, s32 tile);
void UiText_DrawCharacter(struct SelectionWork *work, s32 index, u32 kind);
void Graphics_SetBg1Priority3(void);
void Graphics_ClearBg1ControlBit2(void);
void Graphics_InstallBg1VCountHandler(void);
struct UiWindow *UiText_ShowMessageAndWaitComplete(s32 message, s32 x, s32 y);
s32 Ui_RunSelectionScreen(s32 mode);
void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 alt);
s32 BattleLayout_HighlightPartyPanels(u16 *ids);
s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value);
s32 DjinnMenu_ListChangedDjinn(const struct OwnerActionSlot *oldGrid, const struct OwnerActionSlot *newGrid, u16 *out, s32 *gained, s32 *lost);
s32 Ui_CreateOutputFromResourceSlot(struct RenderInput *input, s32 x, s32 y, s32 abil);
struct RenderInput *DjinnMenu_ShowChangePreview(struct RenderInput *win, s32 owner, s32 code, s32 page, s32 *pageCount);
s32 Ui_RunOwnerStatusScreen(u16 *list, s32 listCount, s32 owner);
s32 Battle_SelectAbility(s32 owner, s32 unused);
s32 SummonMenu_SelectSummon(s32 unused0, s32 unused1, const u8 *standby);
s32 Item_ClassifyUseAbility(s32 unit, s32 item);
void Vram_CopyTile(u32 source_tile, u32 destination_tile);
s32 ItemList_SelectEntry(s32 owner, u16 *entries, s32 count);
s32 BattleMenu_RunActionSelection(s32 unit_id, u16 *actions, s32 count);
s32 BattleTarget_RunSelection(s32 sel, s32 mode, u32 spread, u32 kind);
void UpdateLinkSessionCountdown(void);
s32 UiText_ShowLocalizedMessageAndWait(void);
s32 Battle_ClassifyEntryKind(const struct BtlAct *act);
s32 Battle_CollectPartyCommands(struct BattleActionRecord *out, u16 *in, s32 count);

#endif
