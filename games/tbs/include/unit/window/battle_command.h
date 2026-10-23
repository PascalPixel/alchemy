#ifndef ALCHEMY_UNIT_WINDOW_BATTLE_COMMAND_H
#define ALCHEMY_UNIT_WINDOW_BATTLE_COMMAND_H

#include "types.h"
#include "edition.h"
#include "glyph.h"

struct UiWindow;
struct RenderOutput;

struct RenderOutput *RenderOutput_Create(s32 slot, s32 oamFlags,
                                         struct UiWindow *window, s32 offsetX,
                                         s32 offsetY);
struct RenderOutput *RenderOutput_CreateFromResource(s32 mode, s32 resource,
                                                     struct UiWindow *input,
                                                     s32 x, s32 y);
struct RenderOutput *RenderOutput_CreateLoaded(s32 code, s32 layers,
                                               struct UiWindow *input, s32 x,
                                               s32 y);
struct RenderOutput *RenderOutput_CreateWithTransform(s32 icon,
                                                      struct UiWindow *input,
                                                      s32 x, s32 y);
struct RenderOutput *RenderOutput_CreateFromTable(s32 table_entry,
                                                  struct UiWindow *input, s32 x,
                                                  s32 y);
struct RenderOutput *CreateSideObject(s32 character, s32 unused, s32 side,
                                      struct UiWindow *window, s32 x, s32 y);
void Ui_LoadCharacterEntryForSlot(u32 side, s32 character, s32 unused);
void RenderOutput_SetAnimation(struct RenderOutput *out, s32 mode);
void RenderOutput_SetInvertedSentinel(void *output, s32 priority);
void UiWindow_FillFromScene(void *spans);
void UiWindow_FillScreenBlockRect(s32 unused0, s32 unused1, u32 width,
                                  u32 height, s32 value);

void UiWindow_BuildLayoutBounds(s32 flags);
void UiWindow_CreateWithLayoutBounds(s32 flags);
void UiWindow_DrawColumnBorders(struct UiWindow *window, u32 flags);
#ifdef UBFIX
void UiWindow_DrawStatusBarTiles(struct UiWindow *window, s32 x, s32 y,
                                 s32 length);
#else
s32 UiWindow_DrawStatusBarTiles(struct UiWindow *window, s32 x, s32 y,
                                s32 length);
#endif
void PartyStatus_Draw(s32 flags);
void PartyStatus_Close(void);
void UiWindow_SetTileAttributeBitRect(const struct UiWindow *window, s32 x,
                                      s32 y, s32 width, s32 height, u32 set);

struct SaveSummary;

void *Text_FormatPlayTime(s32 value, u8 *out);
u32 SaveState_FindFreeSummarySlot(void);
s32 SaveState_CountRecordsExcludingFlagged(s32 flag);
s32 SaveState_ScanRecordFlags(void);
u32 SaveState_BuildSummaryHeader(void);
s16 SaveState_WriteCurrentSlotPair(void);
s32 SaveState_WriteSlotPair(s32 arg0);
s32 SaveState_ProcessSelectedSlot(void);
s32 SaveState_ConfirmAndProcessSelectedSlot(void);
s32 SaveState_CopySlotPair(void);
s32 SaveState_DeleteSelectedSlot(void);
void PaletteGlow_UpdateSine(void);
void PaletteGlow_Start(void);
void PaletteGlow_Stop(void);
void UiWindow_FillTilemapRect(struct UiWindow *window, s32 x, s32 y, s32 width,
                              s32 height);
void SaveMenu_ShowPartySprites(struct UiWindow *origin, s32 x, s32 y,
                               struct SaveSummary *descriptor);
void SaveMenu_HidePartySprites(void);
void SaveMenu_PlacePartySprites(void);
void SaveMenu_ShowDjinnSprites(struct UiWindow *origin, s32 x, s32 y);
void SaveMenu_HideDjinnSprites(void);
void SaveMenu_PlaceDjinnSprites(void);
void SaveMenu_DrawDjinnCounts(struct UiWindow *win, struct SaveSummary *row);
void SaveMenu_DrawFileSummary(struct UiWindow *win, struct SaveSummary *rec);
s32 SaveMenu_SelectSlot(s32 file, s32 mode);
s32 SaveMenu_WriteSelectedSlot(void);
s32 SaveState_LoadRecordIntoWork(s32 mode);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
s32 NameEntry_AcceptsMark(s32 mark, s32 letter);
s32 NameEntry_CountLetters(const u8 *name);
void UiWindow_CopyTilemapRegion(const struct UiWindow *window,
                                const void *source);
void UiWindow_SetTileAttributeRect(const struct UiWindow *window, s32 x, s32 y,
                                   s32 width, s32 height, u32 palette);
void Resource_CopyFixedBlockB(s32 slot);
void Resource_CopyFixedBlockA(s32 slot);
s32 UiText_SetRenderString(const u8 *str);
void UiText_DrawPaddedLabel(struct UiWindow *window, u8 *name);
s32 NameEntry_EditOwnerName(s32 member);
s32 PartyTalkMenu_Choose(s32 unused);
void Djinn_ShowJoinedMessage(s32 member, s32 element, s32 djinn);
s32 Party_LookupCharacterValueByFlag32(u32 index);
void Party_ShowJoinedMessage(s32 member);
void Party_ShowPairJoinedMessage(s32 first, s32 second);
void RenderResource_LoadPair(s32 picture, s32 slot);
struct RenderOutput *
RenderResource_CreatePair(s32 picture, struct UiWindow *window, s32 x, s32 y);
void RenderOutput_BobPair(struct RenderOutput *obj);
void RenderResource_LoadFrame(s32 icon, s32 slot, s32 remap);
struct RenderOutput *RenderResource_CreateFrame(s32 icon, s32 remap,
                                                struct UiWindow *window, s32 x,
                                                s32 y);
void Ui_ApplyTableScaleToObject(struct RenderOutput *obj);

struct BattleActionRecord;
struct BattleAct;
struct GlyphPalette;
struct OwnerActionSlot;
struct SelectionWork;

#ifdef UBFIX
void Menu_BuildLocalizedPatternTiles(void);
void UiWindow_DrawThreeTileColumn(struct UiWindow *window, s32 arg1, s32 arg2,
                                  s32 arg3, s32 unused);
#else
s32 Menu_BuildLocalizedPatternTiles(void);
int UiWindow_DrawThreeTileColumn(struct UiWindow *window, s32 arg1, s32 arg2,
                                 s32 arg3, s32 unused);
#endif
void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output,
                                   s32 shift);
void Link_DrawShiftedTilePair(s32 offset);
void Graphics_ExpandVramTilesByColorTable(u16 *dst);
s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index);
s32 ItemIcon_LoadIntoSlot(s32 resource, s32 index);
s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index);
s32 UiGlyph_LoadPortrait(u32 member, s32 cacheSlot);
s32 Ui_GetTableWordZero(s32 no);
void UiIcon_Decompress(GlyphTransfer *work);
struct UiWindow *Link_CreateCountdownLabelWindow(void);
s32 Resource_LoadIndexedIntoBuffer(s32 arg0, s32 arg1);
void UiText_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph,
                              s32 tile);
void UiText_DrawCharacter(struct SelectionWork *work, s32 index, u32 kind);
void Graphics_SetBg1Priority3(void);
void Graphics_SetBg1Priority0(void);
void Graphics_InstallBg1VCountHandler(void);
struct UiWindow *UiText_ShowMessageAndWaitComplete(s32 message, s32 x, s32 y);
s32 Menu_RunCommandRow(s32 mode);
void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 alt);
s32 BattleLayout_HighlightPartyPanels(u16 *ids);
#ifdef UBFIX
void UiText_FormatNumberToHalfwords(s16 *out, s32 value);
#else
s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value);
#endif
s32 DjinnMenu_ListChangedDjinn(const struct OwnerActionSlot *oldGrid,
                               const struct OwnerActionSlot *newGrid, u16 *out,
                               s32 *gained, s32 *lost);
#ifdef UBFIX
void Ui_CreateOutputFromResourceSlot(struct UiWindow *input, s32 x, s32 y,
                                     s32 ability);
#else
s32 Ui_CreateOutputFromResourceSlot(struct UiWindow *input, s32 x, s32 y,
                                    s32 ability);
#endif
struct UiWindow *DjinnMenu_ShowChangePreview(struct UiWindow *win, s32 owner,
                                             s32 code, s32 page,
                                             s32 *pageCount);
s32 Ui_RunOwnerStatusScreen(u16 *list, s32 listCount, s32 owner);
s32 Battle_SelectAbility(s32 owner, s32 unused);
s32 SummonMenu_SelectSummon(s32 unused0, s32 unused1, const u8 *standby);
s32 Item_ClassifyUseAbility(s32 unit, s32 item);
void Vram_CopyTile(u32 source_tile, u32 destination_tile);
s32 ItemList_SelectEntry(s32 owner, u16 *entries, s32 count);
s32 BattleMenu_RunActionSelection(s32 unitId, u16 *actions, s32 count);
s32 BattleTarget_RunSelection(s32 selection, s32 mode, u32 spread, u32 kind);
void UpdateLinkSessionCountdown(void);
s32 UiText_ShowLocalizedMessageAndWait(void);
s32 Battle_ClassifyEntryKind(const struct BattleAct *act);
s32 Battle_CollectPartyCommands(struct BattleActionRecord *out, u16 *in,
                                s32 count);

void AffineEffect_UpdateFrame(void);
void *AffineEffect_InitializeWork(void);
void Menu_EndResourceSelection(void);
s32 Menu_RunResourceSelectionLoop(s32 initial);
s32 Menu_SelectResource(s32 start, s32 goal);
void Menu_LoadResourceSlot(s32 slot, s32 index);
void Menu_AppendResourceEntry(s32 no);
void Menu_CenterResourceEntries(s32 row, s32 width, s32 resourceBase);
void Menu_LayoutResourceEntries(s32 x, s32 y, s32 width, s32 resourceBase);
/// The field menu's commands, as Menu_SelectTopEntry takes and returns them
/// (gMenu_TopEntryCommandByPosition).
enum FieldMenuCommand {
  FIELD_MENU_PSYNERGY = 1,
  FIELD_MENU_DJINN = 2,
  FIELD_MENU_ITEM = 3,
  FIELD_MENU_STATUS = 4
};

/// The title menu's choices, as Menu_SelectSaveSlotAction returns them
/// (gMenu_SaveSlotActionByPosition).
enum SaveSlotAction {
  SAVE_ACTION_NEW_GAME = 0,
  SAVE_ACTION_CONTINUE = 1,
  SAVE_ACTION_COPY = 2,
  SAVE_ACTION_ERASE = 3,
  SAVE_ACTION_BATTLE = 4,
  SAVE_ACTION_SEND = 5
};

/// What Menu_SelectResourceLayout offers.
enum ResourceLayoutMode {
  RESOURCE_LAYOUT_TRANSFER_METHOD = 0, ///< Password or Cable
  RESOURCE_LAYOUT_PASSWORD_LEVEL = 1   ///< Gold, Silver or Bronze
};

/// The places of the transfer method menu (RESOURCE_LAYOUT_TRANSFER_METHOD).
enum TransferMethod { TRANSFER_METHOD_PASSWORD = 0, TRANSFER_METHOD_CABLE = 1 };

/// The places of the password level menu (RESOURCE_LAYOUT_PASSWORD_LEVEL).
enum PasswordLevel {
  PASSWORD_LEVEL_GOLD = 0,
  PASSWORD_LEVEL_SILVER = 1,
  PASSWORD_LEVEL_BRONZE = 2
};

/// What Menu_RunConfirmSelection and Menu_RunConfirmSelectionAt return.
enum ConfirmChoice { CONFIRM_YES = 0, CONFIRM_NO = 1 };

s32 Menu_SelectTopEntry(s32 selection);
s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1);
s32 Menu_SelectSaveSlotAction(void);
void Menu_DrawModeLabel(void);
void Menu_DrawModeIndicator(void);
s32 Menu_SelectResourceLayout(s32 mode);
s32 Menu_SelectShopCommand(s32 selection);
s32 Menu_SelectSanctumService(s32 selection);
s32 Menu_RunConfirmSelection(s32 low, s32 message, s32 w, s32 selection);
s32 Menu_RunConfirmSelectionAt(s32 x, s32 y, s32 selection);
s32 Menu_SelectCoinsOrMedals(s32 selection);

void DebugMenu_ScheduleSceneSelector(void);
void Menu_DrawSelectionRow(struct UiWindow *work, s32 first, const s16 *second);
#ifdef UBFIX
void Menu_RunSelection(void);
#else
s16 Menu_RunSelection(void);
#endif
s32 Menu_HandleSelectionRowInput(struct UiWindow *arg, s16 value, s16 *sub,
                                 s16 *mode);
s32 Menu_RunSelectionWithCursorObject(void);
void Text_FormatHex(u32 value, u32 count, u8 *out);
void Menu_DrawFlagBitTable(struct UiWindow *window, s32 start_flag);
s32 Menu_HandleFlagGridInput(struct UiWindow *window, s32 *page, s32 *cursor);
void Menu_ReservedNoOp(void);
void Djinn_SetFoundFlagAndAdd(s32 owner, s32 element, s32 index);
s32 Menu_RunWorkspaceResultLoop(void);
s32 DebugMenu_BrowseIcons(void);
s32 DebugMenu_BrowsePortraits(void);

#endif
