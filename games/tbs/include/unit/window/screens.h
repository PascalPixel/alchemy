#ifndef ALCHEMY_UNIT_WINDOW_SCREENS_H
#define ALCHEMY_UNIT_WINDOW_SCREENS_H

#include "types.h"
#include "edition.h"
#include "glyph.h"

struct Window;
struct WindowSprite;

struct WindowSprite *WindowSprite_Create(s32 slot, s32 oamFlags,
                                         struct Window *window, s32 offsetX,
                                         s32 offsetY);
struct WindowSprite *WindowSprite_CreateFromResource(s32 mode, s32 resource,
                                                     struct Window *input,
                                                     s32 x, s32 y);
struct WindowSprite *WindowSprite_CreateLoaded(s32 code, s32 layers,
                                               struct Window *input, s32 x,
                                               s32 y);
struct WindowSprite *
WindowSprite_CreateWithTransform(s32 icon, struct Window *input, s32 x, s32 y);
struct WindowSprite *WindowSprite_CreateFromTable(s32 table_entry,
                                                  struct Window *input, s32 x,
                                                  s32 y);
struct WindowSprite *Portrait_Create(s32 character, s32 unused, s32 side,
                                     struct Window *window, s32 x, s32 y);
void Portrait_Reload(u32 side, s32 character, s32 unused);
void WindowSprite_SetMotion(struct WindowSprite *out, s32 mode);
void WindowSprite_SetInvertedSentinel(void *sprite, s32 priority);
void ScanlineWindow_FillSpans(void *spans);
void BgScreen_FillRect(s32 unused0, s32 unused1, u32 width, u32 height,
                       s32 value);

void PartyStatus_BuildLayoutBounds(s32 flags);
void PartyStatus_CreateWindow(s32 flags);
void Window_DrawColumnBorders(struct Window *window, u32 flags);
#ifdef UBFIX
void PartyStatus_DrawBars(struct Window *window, s32 x, s32 y, s32 length);
#else
s32 PartyStatus_DrawBars(struct Window *window, s32 x, s32 y, s32 length);
#endif
void PartyStatus_Draw(s32 flags);
void PartyStatus_Close(void);
void Window_SetTextTileBitRect(const struct Window *window, s32 x, s32 y,
                               s32 width, s32 height, u32 set);

struct SaveFileSummary;

void *String_FormatPlayTime(s32 value, u8 *out);
u32 Save_FindFreeFile(void);
s32 Save_CountRecordsExcludingFlagged(s32 flag);
s32 Save_ScanRecordFlags(void);
u32 Save_BuildFileSummary(void);
s16 Save_WriteCurrentFile(void);
s32 Save_WriteFile(s32 file);
s32 Save_ProcessSelectedFile(void);
s32 Save_ConfirmAndProcessSelectedFile(void);
s32 Save_CopyFile(void);
s32 Save_EraseSelectedFile(void);
void FileMenu_UpdatePulse(void);
void FileMenu_StartPulse(void);
void FileMenu_StopPulse(void);
void Window_FillTilemapRect(struct Window *window, s32 x, s32 y, s32 width,
                            s32 height);
void FileMenu_ShowPartySprites(struct Window *origin, s32 x, s32 y,
                               struct SaveFileSummary *descriptor);
void FileMenu_HidePartySprites(void);
void FileMenu_PlacePartySprites(void);
void FileMenu_ShowDjinnSprites(struct Window *origin, s32 x, s32 y);
void FileMenu_HideDjinnSprites(void);
void FileMenu_PlaceDjinnSprites(void);
void FileMenu_DrawDjinnCounts(struct Window *win, struct SaveFileSummary *row);
void FileMenu_DrawSummary(struct Window *win, struct SaveFileSummary *rec);
s32 FileMenu_SelectFile(s32 file, s32 mode);
s32 Save_ToChosenFile(void);
s32 Save_LoadRecordIntoWork(s32 mode);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
s32 NamingScreen_AcceptsMark(s32 mark, s32 letter);
s32 NamingScreen_CountLetters(const u8 *name);
void Window_CopyTilemapRegion(const struct Window *window, const void *source);
void Window_SetTextTileRect(const struct Window *window, s32 x, s32 y,
                            s32 width, s32 height, u32 palette);
void NamingScreen_LoadPointingHandTiles(s32 slot);
void NamingScreen_LoadCaretTiles(s32 slot);
s32 Text_SetString(const u8 *str);
void Text_DrawPaddedLabel(struct Window *window, u8 *name);
s32 NamingScreen_EditMemberName(s32 member);
s32 ColossoMenu_Choose(s32 unused);
void Djinn_ShowJoinedMessage(s32 member, s32 element, s32 djinn);
s32 Party_LookupCharacterValueByFlag32(u32 index);
void Party_ShowJoinedMessage(s32 member);
void Party_ShowPairJoinedMessage(s32 first, s32 second);
void MenuTitle_Load(s32 picture, s32 slot);
struct WindowSprite *MenuTitle_Create(s32 picture, struct Window *window, s32 x,
                                      s32 y);
void MenuTitle_Bob(struct WindowSprite *obj);
void CommandIcon_Load(s32 icon, s32 slot, s32 remap);
struct WindowSprite *CommandIcon_Create(s32 icon, s32 remap,
                                        struct Window *window, s32 x, s32 y);
void WindowSprite_Pulse(struct WindowSprite *obj);

struct BattleAction;
struct AbilityDefinition;
struct GlyphPalette;
struct PsynergyEntry;
struct BattleCommandRowWork;

#ifdef UBFIX
void Menu_BuildLocalizedPatternTiles(void);
void Window_DrawThreeTileColumn(struct Window *window, s32 x, s32 y,
                                s32 pattern, s32 unused);
#else
s32 Menu_BuildLocalizedPatternTiles(void);
int Window_DrawThreeTileColumn(struct Window *window, s32 x, s32 y, s32 pattern,
                               s32 unused);
#endif
void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output,
                                   s32 shift);
void Link_DrawPageArrows(s32 offset);
void Graphics_ExpandVramTilesByColorTable(u16 *dst);
s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index);
s32 ItemIcon_LoadIntoSlot(s32 resource, s32 index);
s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index);
s32 Portrait_Load(u32 member, s32 cacheSlot);
s32 WindowSprite_GetPointerTiles(s32 no);
void Icon_Decompress(GlyphTransfer *work);
struct Window *Link_CreateCountdownLabelWindow(void);
s32 Resource_LoadIndexedIntoBuffer(s32 slotIndex, s32 icon);
void Text_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph, s32 tile);
void Text_DrawCharacter(struct BattleCommandRowWork *work, s32 index, u32 kind);
void Graphics_SetBg1Priority3(void);
void Graphics_SetBg1Priority0(void);
void Graphics_InstallBg1VCountHandler(void);
struct Window *Message_ShowAndWait(s32 message, s32 x, s32 y);
s32 BattleCommandRow_Run(s32 mode);
void Window_HighlightRect(s32 x, s32 y, s32 width, s32 height, s32 alt);
s32 PartyStatus_HighlightMembers(u16 *ids);
#ifdef UBFIX
void String_FormatNumberToHalfwords(s16 *out, s32 value);
#else
s32 String_FormatNumberToHalfwords(s16 *out, s32 value);
#endif
s32 BattleDjinnMenu_ListChangedDjinn(const struct PsynergyEntry *oldGrid,
                                     const struct PsynergyEntry *newGrid,
                                     u16 *out, s32 *gained, s32 *lost);
#ifdef UBFIX
void WindowSprite_CreateAbilityIcon(struct Window *input, s32 x, s32 y,
                                    s32 ability);
#else
s32 WindowSprite_CreateAbilityIcon(struct Window *input, s32 x, s32 y,
                                   s32 ability);
#endif
struct Window *BattleDjinnMenu_ShowChangePreview(struct Window *win, s32 member,
                                                 s32 code, s32 page,
                                                 s32 *pageCount);
s32 BattleSummaryScreen_Run(u16 *list, s32 listCount, s32 member);
s32 BattleDjinnMenu_Run(s32 member, s32 unused);
s32 BattleSummonMenu_Run(s32 unused0, s32 unused1, const u8 *standby);
s32 Item_ClassifyUseAbility(s32 unit, s32 itemId);
void Vram_CopyTile(u32 source_tile, u32 destination_tile);
s32 BattleItemMenu_Run(s32 member, u16 *entries, s32 count);
s32 BattlePsynergyMenu_Run(s32 unitId, u16 *actions, s32 count);
s32 BattleTarget_RunSelection(s32 selection, s32 mode, u32 spread, u32 kind);
void UpdateLinkSessionCountdown(void);
s32 Message_ShowLocalizedAndWait(void);
s32 Battle_ClassifyEntryKind(const struct AbilityDefinition *act);
s32 Battle_CollectPartyCommands(struct BattleAction *out, u16 *in, s32 count);

void IconMenu_DrawIcons(void);
void *IconMenu_Open(void);
void IconMenu_Close(void);
s32 IconMenu_Run(s32 initial);
s32 IconMenu_AnimateTo(s32 start, s32 goal);
void CommandIcon_LoadIntoSlot(s32 slot, s32 index);
void IconMenu_AddIcon(s32 no);
void IconMenu_Center(s32 row, s32 width, s32 resourceBase);
void IconMenu_LayoutIcons(s32 x, s32 y, s32 width, s32 resourceBase);
/// The field menu's commands, as FieldMenu_SelectCommand takes and returns them
/// (gFieldMenu_CommandByPosition).
enum FieldMenuCommand {
  FIELD_MENU_PSYNERGY = 1,
  FIELD_MENU_DJINN = 2,
  FIELD_MENU_ITEM = 3,
  FIELD_MENU_STATUS = 4
};

/// The title menu's choices, as FileMenu_SelectAction returns them
/// (gFileMenu_ActionByPosition).
enum FileAction {
  SAVE_ACTION_NEW_GAME = 0,
  SAVE_ACTION_CONTINUE = 1,
  SAVE_ACTION_COPY = 2,
  SAVE_ACTION_ERASE = 3,
  SAVE_ACTION_BATTLE = 4,
  SAVE_ACTION_SEND = 5
};

/// What IconMenu_ChooseSendOption offers.
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

/// What YesNoMenu_Run and YesNoMenu_RunAt return.
enum YesNo { YES = 0, NO = 1 };

s32 FieldMenu_SelectCommand(s32 selection);
s32 Menu_AnimateSelectionToEntry(s32 start, s32 command);
s32 FileMenu_SelectAction(void);
void Menu_DrawModeLabel(void);
void Menu_DrawModeIndicator(void);
s32 IconMenu_ChooseSendOption(s32 mode);
s32 Menu_SelectShopCommand(s32 selection);
s32 Menu_SelectSanctumService(s32 selection);
s32 YesNoMenu_Run(s32 low, s32 message, s32 w, s32 selection);
s32 YesNoMenu_RunAt(s32 x, s32 y, s32 selection);
s32 Menu_SelectCoinsOrMedals(s32 selection);

void DebugMapSelect_Schedule(void);
void DebugMapSelect_DrawRow(struct Window *window, s32 first,
                            const s16 *second);
#ifdef UBFIX
void DebugMapSelect_Run(void);
#else
s16 DebugMapSelect_Run(void);
#endif
s32 DebugMapSelect_HandleRowInput(struct Window *arg, s16 value, s16 *sub,
                                  s16 *mode);
s32 DebugFlagEditor_Run(void);
void String_FormatHexDigits(u32 value, u32 count, u8 *out);
void DebugFlagEditor_DrawTable(struct Window *window, s32 start_flag);
s32 DebugFlagEditor_HandleInput(struct Window *window, s32 *page, s32 *cursor);
void Menu_ReservedNoOp(void);
void Djinn_SetFoundFlagAndAdd(s32 member, s32 element, s32 index);
s32 PauseMenu_Run(void);
s32 DebugBrowser_Icons(void);
s32 DebugBrowser_Portraits(void);

#endif
