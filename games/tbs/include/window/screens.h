#ifndef ALCHEMY_SCREENS_H
#define ALCHEMY_SCREENS_H

/// @file
/// The menu object's routines (window/screens.c): window sprites and
/// portraits, the party status window, the file list and saving, name entry,
/// the joined-party messages, the battle menus, the icon menus and the debug
/// menus.

#include "edition.h"
#include "types.h"
#include "graphics/glyph.h"

struct Window;
struct WindowSprite;

struct WindowSprite *WindowSprite_Create(s32 slot, s32 oamFlags,
                                         struct Window *window, s32 offsetX,
                                         s32 offsetY);
struct WindowSprite *WindowSprite_CreateFromResource(s32 mode, s32 resource,
                                                     struct Window *window,
                                                     s32 x, s32 y);
struct WindowSprite *WindowSprite_CreateLoaded(s32 icon, s32 layers,
                                               struct Window *window, s32 x,
                                               s32 y);
struct WindowSprite *
WindowSprite_CreateOverlaidIcon(s32 icon, struct Window *window, s32 x, s32 y);
struct WindowSprite *WindowSprite_CreateFromTable(s32 tableEntry,
                                                  struct Window *window, s32 x,
                                                  s32 y);
struct WindowSprite *Portrait_Create(s32 character, s32 unused, s32 side,
                                     struct Window *window, s32 x, s32 y);
void Portrait_Reload(u32 side, s32 character, s32 unused);
void WindowSprite_SetMotion(struct WindowSprite *sprite, s32 mode);
void WindowSprite_SetPriority(void *sprite, s32 priority);
void ScanlineWindow_FillSpans(void *spans);
void BgScreen_FillRect(s32 x, s32 y, u32 width, u32 height, s32 value);

void PartyStatus_BuildLayoutBounds(s32 flags);
void PartyStatus_CreateWindow(s32 flags);
void Window_DrawColumnBorders(struct Window *window, u32 flags);
UNREAD_RESULT PartyStatus_DrawBars(struct Window *window, s32 x, s32 y,
                                   s32 length);
void PartyStatus_Draw(s32 flags);
void PartyStatus_Close(void);
void Window_SetInteriorPaletteBit(const struct Window *window, s32 x, s32 y,
                                  s32 width, s32 height, u32 set);

struct SaveFileSummary;

void *String_FormatPlayTime(s32 frames, u8 *out);
u32 Save_FindFreeFile(void);
s32 Save_CountFilesExcludingSent(s32 excludeSent);
s32 Save_ScanFileSummaries(void);
u32 Save_BuildFileSummary(void);
s16 Save_WriteCurrentFile(void);
s32 Save_WriteFile(s32 file);
s32 Save_WriteLinkTallies(void);
s32 Save_ConfirmAndProcessSelectedFile(void);
s32 Save_CopyFile(void);
s32 Save_EraseSelectedFile(void);
void FileList_UpdatePulse(void);
void FileList_StartPulse(void);
void FileList_StopPulse(void);
void Window_FillTilemapRect(struct Window *window, s32 x, s32 y, s32 width,
                            s32 height);
void FileList_ShowPartySprites(struct Window *window, s32 x, s32 y,
                               struct SaveFileSummary *summary);
void FileList_HidePartySprites(void);
void FileList_PlacePartySprites(void);
void FileList_ShowDjinnSprites(struct Window *window, s32 x, s32 y);
void FileList_HideDjinnSprites(void);
void FileList_PlaceDjinnSprites(void);

/// What the file list is choosing a file for, which decides the files the
/// cursor may stop on.
enum FileMenuMode {
  FILE_LIST_SAVE,     ///< saving: any file, empty ones too
  FILE_LIST_CONTINUE, ///< continuing: files not yet cleared
  FILE_LIST_COPY,     ///< copying: any file with data
  FILE_LIST_ERASE,    ///< erasing: any file with data
  /// The title's Battle: files whose party has found a Djinni
  /// (SaveFileSummary.djinnCount).
  FILE_LIST_BATTLE,
  FILE_LIST_CLEARED ///< the title's Send: cleared files only
};

void FileList_DrawDjinnCounts(struct Window *window,
                              struct SaveFileSummary *summary);
void FileList_DrawSummary(struct Window *window,
                          struct SaveFileSummary *summary);
s32 FileList_SelectFile(s32 selectedFile, s32 menuMode);
s32 Save_ToChosenFile(void);
s32 Save_ChooseAndLoadFile(s32 mode);
s32 PointingHand_LoadIntoFreeSlot(void);
s32 NameEntry_AcceptsMark(s32 mark, s32 letter);
s32 NameEntry_CountLetters(const u8 *name);
void Window_CopyTilemapRegion(const struct Window *window, const void *source);
void Window_SetInteriorPalette(const struct Window *window, s32 x, s32 y,
                               s32 width, s32 height, u32 palette);
void NameEntry_LoadPointingHandTiles(s32 slot);
void NameEntry_LoadCaretTiles(s32 slot);
s32 Text_SetString(const u8 *str);
void NameEntry_DrawName(struct Window *window, u8 *name);
s32 NameEntry_EditMemberName(s32 member);
s32 ColossoMenu_Choose(s32 unused);
void Djinn_ShowJoinedMessage(s32 member, s32 element, s32 djinn);
s32 Party_GetMemberPortraitSprite(u32 member);
void Party_ShowJoinedMessage(s32 member);
void Party_ShowPairJoinedMessage(s32 first, s32 second);
void MenuTitle_Load(s32 picture, s32 slot);
struct WindowSprite *MenuTitle_Create(s32 picture, struct Window *window, s32 x,
                                      s32 y);
void MenuTitle_Bob(struct WindowSprite *sprite);
void CommandIcon_Load(s32 icon, s32 slot, s32 remap);
struct WindowSprite *CommandIcon_Create(s32 icon, s32 remap,
                                        struct Window *window, s32 x, s32 y);
void WindowSprite_Pulse(struct WindowSprite *sprite);

struct BattleAction;
struct AbilityDefinition;
struct IconPalette;
struct PsynergyEntry;
struct BattleCommandRowState;

UNREAD_RESULT RangeIcon_BuildTiles(void);
UNREAD_RESULT RangeIcon_Draw(struct Window *window, s32 x, s32 y, s32 pattern,
                             s32 unused);
void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output,
                                   s32 shift);
void PageArrows_Draw(s32 offset);
void Graphics_ExpandVramTilesByColorTable(u16 *dst);
s32 StatusIcon_LoadIntoSlot(s32 resource, s32 index);
s32 ItemIcon_LoadIntoSlot(s32 resource, s32 index);
s32 AbilityIcon_LoadIntoSlot(s32 resource, s32 index);
s32 Portrait_Load(u32 member, s32 cacheSlot);
s32 WindowSprite_GetPointerTiles(s32 index);
void Icon_Decompress(IconTransfer *state);
struct Window *Link_CreateCountdownLabelWindow(void);
s32 CommandIcon_ReloadSlot(s32 slotIndex, s32 icon);
void BattleCommandRow_LoadIconTiles(struct IconPalette *palette, s32 glyph,
                                    s32 tile);
void BattleCommandRow_LoadIcon(struct BattleCommandRowState *state, s32 index,
                               u32 icon);
void CommandRow_PutBg1Behind(void);
void CommandRow_PutBg1InFront(void);
void Graphics_InstallBg1VCountHandler(void);
struct Window *Message_ShowAndWait(s32 message, s32 x, s32 y);
s32 BattleCommandRow_Run(s32 mode);
void Window_HighlightRect(s32 x, s32 y, s32 width, s32 height, s32 paletteBit);
s32 PartyStatus_HighlightMembers(u16 *ids);
UNREAD_RESULT String_FormatNumberToHalfwords(s16 *out, s32 value);
s32 BattleDjinnMenu_ListChangedAbilities(const struct PsynergyEntry *oldGrid,
                                         const struct PsynergyEntry *newGrid,
                                         u16 *out, s32 *gained, s32 *lost);
UNREAD_RESULT WindowSprite_CreateAbilityIcon(struct Window *window, s32 x,
                                             s32 y, s32 abilityId);
struct Window *BattleDjinnMenu_ShowChangePreview(struct Window *window,
                                                 s32 member, s32 encodedDjinn,
                                                 s32 page, s32 *pageCount);
s32 BattleStatusScreen_Run(u16 *list, s32 listCount, s32 member);
s32 BattleDjinnMenu_Run(s32 member, s32 unused);
s32 BattleSummonMenu_Run(s32 unusedA, s32 unusedB, const u8 *standby);
s32 Item_CheckBattleUse(s32 unit, s32 itemId);
void Vram_CopyTile(u32 sourceTile, u32 destinationTile);
s32 BattleItemMenu_Run(s32 member, u16 *entries, s32 count);
s32 BattlePsynergyMenu_Run(s32 unitId, u16 *actions, s32 count);
s32 BattleTarget_RunSelection(s32 selection, s32 mode, u32 spread, u32 kind);
void BattleMenu_Update(void);
s32 Battle_RefuseFlee(void);
s32 Battle_ClassifyEntryKind(const struct AbilityDefinition *act);
s32 Battle_CollectPartyCommands(struct BattleAction *commands, u16 *actors,
                                s32 commandCount);

void IconMenu_DrawIcons(void);
void *IconMenu_Open(void);
void IconMenu_Close(void);
s32 IconMenu_Run(s32 initial);
s32 IconMenu_AnimateTo(s32 start, s32 goal);
void CommandIcon_LoadIntoSlot(s32 slot, s32 index);
void IconMenu_AddIcon(s32 icon);
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

/// The title menu's choices, as FileList_SelectAction returns them
/// (gFileList_ActionByPosition).
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
  SEND_OPTION_TRANSFER_METHOD = 0, ///< Password or Cable
  SEND_OPTION_PASSWORD_LEVEL = 1   ///< Gold, Silver or Bronze
};

/// The places of the transfer method menu (SEND_OPTION_TRANSFER_METHOD).
enum TransferMethod { TRANSFER_METHOD_PASSWORD = 0, TRANSFER_METHOD_CABLE = 1 };

/// The places of the password level menu (SEND_OPTION_PASSWORD_LEVEL).
enum PasswordLevel {
  PASSWORD_LEVEL_GOLD = 0,
  PASSWORD_LEVEL_SILVER = 1,
  PASSWORD_LEVEL_BRONZE = 2
};

/// What YesNoMenu_Run and YesNoMenu_RunAt return.
enum YesNo { YES = 0, NO = 1 };

s32 FieldMenu_SelectCommand(s32 selection);
s32 Menu_AnimateSelectionToEntry(s32 start, s32 command);
s32 FileList_SelectAction(void);
void PasswordLevel_DrawHelp(void);
void TransferMethod_DrawHelp(void);
s32 IconMenu_ChooseSendOption(s32 mode);
s32 Menu_SelectShopCommand(s32 selection);
s32 Menu_SelectSanctumService(s32 selection);
s32 YesNoMenu_Run(s32 bottom, s32 message, s32 labelWidth, s32 selection);
s32 YesNoMenu_RunAt(s32 x, s32 y, s32 selection);
s32 Menu_SelectCoinsOrMedals(s32 selection);

void DebugSceneSelect_Schedule(void);
void DebugSceneSelect_DrawRow(struct Window *window, s32 sceneId,
                              const s16 *entrance);
#ifdef UBFIX
void DebugSceneSelect_Run(void);
#else
s16 DebugSceneSelect_Run(void);
#endif
s32 DebugSceneSelect_HandleRowInput(struct Window *window, s16 sceneId,
                                    s16 *entrance, s16 *selectedField);
s32 DebugFlagEditor_Run(void);
void String_FormatHexDigits(u32 value, u32 count, u8 *out);
void DebugFlagEditor_DrawTable(struct Window *window, s32 page);
s32 DebugFlagEditor_HandleInput(struct Window *window, s32 *page, s32 *cursor);
void Menu_ReservedNoOp(void);
void Djinn_SetFoundFlagAndAdd(s32 member, s32 element, s32 index);
s32 PauseMenu_Run(void);
s32 DebugBrowser_Icons(void);
s32 DebugBrowser_Portraits(void);

/// The Japanese name entry screen's second keyboard, which Select switches to.
extern const u8 gNameEntry_AlternateKeyboardTilemap[];
/// Eight offsets, 0 to 3 and back, that make the name entry screen's caret
/// circle: the x offset leads the y offset by five steps.
extern s8 gNameEntry_CaretWave[];
/// The kana that take the voiced mark, as first and last pairs ended by 0.
extern const u8 gNameEntry_VoicedMarkRanges[];
/// The Delete key's label on the German, Spanish and French name entry screen
/// keyboards ("ENT", "SUP", "EFF").
extern u8 gNameEntry_DeleteKeyLabel[];
/// The End key's label on the German, Spanish and French keyboards ("ENDE",
/// "FIN", "FIN").
extern u8 gNameEntry_EndKeyLabel[];
/// The kana that take the semi-voiced mark, in the same pairs.
extern const u8 gNameEntry_SemivoicedMarkRanges[];
/// The name entry screen keyboard's compressed tilemap, the Spanish edition's
/// with its own letters.
extern const u8 gNameEntry_KeyboardTilemap[];
/// The 16x16 sprite the name entry screen shows as its name caret.
extern const u8 gNameEntry_CaretTiles[];

#endif
