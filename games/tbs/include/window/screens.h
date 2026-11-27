#ifndef ALCHEMY_SCREENS_H
#define ALCHEMY_SCREENS_H

// The menu object's functions (window/screens.c): window sprites and portraits, the party status
// window, the file list and saving, name entry, the joined-party messages, the battle menus, the
// icon menus and the debug menus.

#include "edition.h"
#include "types.h"
#include "graphics/glyph.h"

struct WINDOW;
struct WINDOW_SPR;

struct WINDOW_SPR      *
mnWindowSpriteCreateOverlaidIcon(Sint32 icon, struct WINDOW *window, Sint32 x, Sint32 y);

void mnPartyStatusBuildLayoutBounds(Sint32 flags);
void mnWindowDrawColumnBorders(struct WINDOW *window, Uint32 flags);
UNREAD_RESULT mnPartyStatusDrawBars(struct WINDOW *window, Sint32 x, Sint32 y, Sint32 length);
void          mnWindowSetInteriorPaletteBit(
             const struct WINDOW *window, Sint32 x, Sint32 y, Sint32 width, Sint32 height, Uint32 set);

struct BUP_FILE_SUMMARY;

void  *mnStringFormatPlayTime(Sint32 ticks, Uint8 *out);
Uint32 mnSaveFindFreeFile(void);
Uint32 mnSaveBuildFileSummary(void);
Sint32 mnSaveConfirmAndProcessSelectedFile(void);
void   mnFileListUpdatePulse(void);
void   mnFileListStartPulse(void);
void   mnFileListStopPulse(void);
void   mnWindowFillTilemapRect(
      struct WINDOW *window, Sint32 x, Sint32 y, Sint32 width, Sint32 height);
void mnFileListShowPartySprites(struct WINDOW           *window,
                                Sint32                   x,
                                Sint32                   y,
                                struct BUP_FILE_SUMMARY *summary);
void mnFileListHidePartySprites(void);
void mnFileListPlacePartySprites(void);
void mnFileListShowDjinnSprites(struct WINDOW *window, Sint32 x, Sint32 y);
void mnFileListHideDjinnSprites(void);
void mnFileListPlaceDjinnSprites(void);

// What the file list is choosing a file for, which decides the files the cursor may stop on.
typedef enum FILE_MENU_MODE
{
    FILE_LIST_SAVE,     // saving: any file, empty ones too
    FILE_LIST_CONTINUE, // continuing: files not yet cleared
    FILE_LIST_COPY,     // copying: any file with data
    FILE_LIST_ERASE,    // erasing: any file with data
    // The title's Battle: files whose party has found a Djinni (BUP_FILE_SUMMARY.djinnCount).
    FILE_LIST_BATTLE,
    FILE_LIST_CLEARED // the title's Send: cleared files only
} FILE_MENU_MODE;

void   mnFileListDrawDjinnCounts(struct WINDOW *window, struct BUP_FILE_SUMMARY *summary);
void   mnFileListDrawSummary(struct WINDOW *window, struct BUP_FILE_SUMMARY *summary);
Sint32 mnFileListSelectFile(Sint32 selectedFile, Sint32 menuMode);
Bool   mnNameEntryAcceptsMark(Sint32 mark, Sint32 letter);
Sint32 mnNameEntryCountLetters(const Uint8 *name);
void   mnWindowCopyTilemapRegion(const struct WINDOW *window, const void *src);
void   mnWindowSetInteriorPalette(
      const struct WINDOW *window, Sint32 x, Sint32 y, Sint32 width, Sint32 height, Uint32 palette);
Sint32             mnTextSetString(const Uint8 *str);
void               mnNameEntryDrawName(struct WINDOW *window, Uint8 *name);
Sint32             mnPartyGetMemberPortraitSprite(Uint32 member);
void               mnTitleLoad(Sint32 picture, Sint32 slot);
struct WINDOW_SPR *mnTitleCreate(Sint32 picture, struct WINDOW *window, Sint32 x, Sint32 y);
void               mnTitleBob(struct WINDOW_SPR *sprite);
void               mnCommandIconLoad(Sint32 icon, Sint32 slot, Sint32 remap);
struct WINDOW_SPR      *
mnCommandIconCreate(Sint32 icon, Sint32 remap, struct WINDOW *window, Sint32 x, Sint32 y);
void mnWindowSpritePulse(struct WINDOW_SPR *sprite);

struct BATTLE_ACTION;
struct ABILITY_DEFINITION;
struct ICON_PALETTE;
struct PSYNERGY_ENTRY;
struct BATTLE_COMMAND_ROW_STATE;

UNREAD_RESULT mnRangeIconBuildTiles(void);
UNREAD_RESULT
mnRangeIconDraw(struct WINDOW *window, Sint32 x, Sint32 y, Sint32 pattern, Sint32 unused);
void   mnGraphicsMergeShiftedTileRows(Uint32 *first, Uint32 *second, Uint32 *output, Sint32 shift);
Sint32 mnStatusIconLoadIntoSlot(Sint32 resource, Sint32 index);
Sint32 mnItemIconLoadIntoSlot(Sint32 resource, Sint32 index);
Sint32 mnAbilityIconLoadIntoSlot(Sint32 resource, Sint32 index);
Sint32 mnPortraitLoad(Uint32 member, Sint32 cacheSlot);
void   mnIconDecompress(ICON_TRANSFER *state);
struct WINDOW *mnLinkCreateCountdownLabelWindow(void);
Sint32         mnCommandIconReloadSlot(Sint32 slotIndex, Sint32 icon);
void           mnBattleCommandRowLoadIconTiles(ICON_PALETTE *palette, Sint32 glyph, Sint32 tile);
void mnBattleCommandRowLoadIcon(struct BATTLE_COMMAND_ROW_STATE *state, Sint32 index, Uint32 icon);
void mnCommandRowPutBg1Behind(void);
void mnCommandRowPutBg1InFront(void);
void mnGraphicsInstallBg1VCountHandler(void);
struct WINDOW *mnMessageShowAndWait(Sint32 msg, Sint32 x, Sint32 y);
Sint32         mnBattleCommandRowRun(Sint32 mode);
UNREAD_RESULT  mnStringFormatNumberToHalfwords(Sint16 *out, Sint32 value);
Sint32         mnBattleDjinnMenuListChangedAbilities(const struct PSYNERGY_ENTRY *oldGrid,
                                                     const struct PSYNERGY_ENTRY *newGrid,
                                                     Uint16                      *out,
                                                     Sint32                      *gained,
                                                     Sint32                      *lost);
UNREAD_RESULT
mnWindowSpriteCreateAbilityIcon(struct WINDOW *window, Sint32 x, Sint32 y, Sint32 abilityId);
struct WINDOW *mnBattleDjinnMenuShowChangePreview(
    struct WINDOW *window, Sint32 member, Sint32 encodedDjinn, Sint32 page, Sint32 *pageCount);
Sint32 mnBattleStatusScreenRun(Uint16 *list, Sint32 listCount, Sint32 member);
Sint32 mnBattleDjinnMenuRun(Sint32 member, Sint32 unused);
Sint32 mnBattleSummonMenuRun(Sint32 unusedA, Sint32 unusedB, const Uint8 *standby);
Sint32 mnItemCheckBattleUse(Sint32 unit, Sint32 itemId);
void   mnVRAMCopyTile(Uint32 srcTile, Uint32 destTile);
Sint32 mnBattleItemMenuRun(Sint32 member, Uint16 *entries, Sint32 count);
Sint32 mnBattlePsynergyMenuRun(Sint32 unitId, Uint16 *actions, Sint32 count);
Sint32 mnBattleTargetRunSelection(Sint32 selection, Sint32 mode, Uint32 spread, Uint32 kind);
void   mnBattleMenuUpdate(void);
Sint32 mnBattleRefuseFlee(void);
Sint32 mnBattleClassifyEntryKind(const struct ABILITY_DEFINITION *act);

void   mnIconMenuDrawIcons(void);
void  *mnIconMenuOpen(void);
void   mnIconMenuClose(void);
Sint32 mnIconMenuRun(Sint32 init);
Sint32 mnIconMenuAnimateTo(Sint32 start, Sint32 goal);
void   mnCommandIconLoadIntoSlot(Sint32 slot, Sint32 index);
void   mnIconMenuAddIcon(Sint32 icon);
void   mnIconMenuCenter(Sint32 row, Sint32 width, Sint32 resourceBase);
void   mnIconMenuLayoutIcons(Sint32 x, Sint32 y, Sint32 width, Sint32 resourceBase);

// The field menu's commands, as mnFieldMenuSelectCommand takes and returns them
// (FieldMenuCommandByPosition).
typedef enum FIELD_MENU_COMMAND
{
    FIELD_MENU_PSYNERGY = 1,
    FIELD_MENU_DJINN    = 2,
    FIELD_MENU_ITEM     = 3,
    FIELD_MENU_STATUS   = 4
} FIELD_MENU_COMMAND;

// The title menu's choices, as mnFileListSelectAction returns them (FileListActionByPosition).
typedef enum FILE_ACTION
{
    SAVE_ACTION_NEW_GAME = 0,
    SAVE_ACTION_CONTINUE = 1,
    SAVE_ACTION_COPY     = 2,
    SAVE_ACTION_ERASE    = 3,
    SAVE_ACTION_BATTLE   = 4,
    SAVE_ACTION_SEND     = 5
} FILE_ACTION;

// What mnIconMenuChooseSendOption offers.
typedef enum RESOURCE_LAYOUT_MODE
{
    SEND_OPTION_TRANSFER_METHOD = 0, // Password or Cable
    SEND_OPTION_PASSWORD_LEVEL  = 1  // Gold, Silver or Bronze
} RESOURCE_LAYOUT_MODE;

// The places of the transfer method menu (SEND_OPTION_TRANSFER_METHOD).
typedef enum TRANSFER_METHOD
{
    TRANSFER_METHOD_PASSWORD = 0,
    TRANSFER_METHOD_CABLE    = 1
} TRANSFER_METHOD;

// The places of the password level menu (SEND_OPTION_PASSWORD_LEVEL).
typedef enum PASSWORD_LEVEL
{
    PASSWORD_LEVEL_GOLD   = 0,
    PASSWORD_LEVEL_SILVER = 1,
    PASSWORD_LEVEL_BRONZE = 2
} PASSWORD_LEVEL;

// What mnYesNoMenuRun and mnYesNoMenuRunAt return.
typedef enum YES_NO
{
    YES = 0,
    NO  = 1
} YES_NO;

Sint32 mnFieldMenuSelectCommand(Sint32 selection);
void   mnPasswordLevelDrawHelp(void);
void   mnTransferMethodDrawHelp(void);

void   mnDebugSceneSelectSchedule(void);
void   mnDebugSceneSelectDrawRow(struct WINDOW *window, Sint32 sceneId, const Sint16 *entrance);
Sint32 mnDebugSceneSelectHandleRowInput(struct WINDOW *window,
                                        Sint16         sceneId,
                                        Sint16        *entrance,
                                        Sint16        *selectedField);
void   mnStringFormatHexDigits(Uint32 value, Uint32 count, Uint8 *out);
void   mnDebugFlagEditorDrawTable(struct WINDOW *window, Sint32 page);
Sint32 mnDebugFlagEditorHandleInput(struct WINDOW *window, Sint32 *page, Sint32 *cursor);
void   mnReservedNoOperation(void);
void   mnDjinnSetFoundFlagAndAdd(Sint32 member, Sint32 element, Sint32 index);

// The Japanese name entry screen's second keyboard, which Select switches to.
extern const Uint8 NameEntryAlternateKeyboardTilemap[];
// Eight offsets, 0 to 3 and back, that make the name entry screen's caret circle: the x offset
// leads the y offset by five steps.
extern Sint8       NameEntryCaretWave[];
// The kana that take the voiced mark, as first and last pairs ended by 0.
extern const Uint8 NameEntryVoicedMarkRanges[];
// The Delete key's label on the German, Spanish and French name entry screen keyboards ("ENT",
// "SUP", "EFF").
extern Uint8       NameEntryDeleteKeyLabel[];
// The End key's label on the German, Spanish and French keyboards ("ENDE", "FIN", "FIN").
extern Uint8       NameEntryEndKeyLabel[];
// The kana that take the semi-voiced mark, in the same pairs.
extern const Uint8 NameEntrySemivoicedMarkRanges[];
// The name entry screen keyboard's compressed tilemap, the Spanish edition's with its own letters.
extern const Uint8 NameEntryKeyboardTilemap[];
// The 16x16 sprite the name entry screen shows as its name caret.
extern const Uint8 NameEntryCaretTiles[];

#endif
