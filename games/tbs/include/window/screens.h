#ifndef ALCHEMY_SCREENS_H
#define ALCHEMY_SCREENS_H

/* @file */
/* The menu object's functions (window/screens.c): window sprites and */
/* portraits, the party status window, the file list and saving, name entry, */
/* the joined-party messages, the battle menus, the icon menus and the debug */
/* menus. */

#include "edition.h"
#include "types.h"
#include "graphics/glyph.h"

struct WINDOW;
struct WINDOW_SPR;

struct WINDOW_SPR *
mnWindowSprCreateOverlaidIcon(s32 icon, struct WINDOW *window, s32 x, s32 y);

void mnPartyStatBuildLayoutBounds(s32 flags);
void mnWindowDrawColumnBorders(struct WINDOW *window, u32 flags);
UNREAD_RESULT mnPartyStatDrawBars(struct WINDOW *window, s32 x, s32 y,
								  s32 length);
void mnWindowSetInteriorPaletteBit(const struct WINDOW *window, s32 x, s32 y,
								   s32 width, s32 height, u32 set);

struct SAVE_FILE_SUMMARY;

void *mnStringFormatPlayTime(s32 frames, u8 *out);
u32 mnSaveFindFreeFile(void);
u32 mnSaveBuildFileSummary(void);
s32 mnSaveConfirmAndProcessSelectedFile(void);
void mnFileListUpdatePulse(void);
void mnFileListStartPulse(void);
void mnFileListStopPulse(void);
void mnWindowFillTilemapRect(struct WINDOW *window, s32 x, s32 y, s32 width,
							 s32 height);
void mnFileListShowPartySprs(struct WINDOW *window, s32 x, s32 y,
							 struct SAVE_FILE_SUMMARY *summary);
void mnFileListHidePartySprs(void);
void mnFileListPlacePartySprs(void);
void mnFileListShowDjinnSprs(struct WINDOW *window, s32 x, s32 y);
void mnFileListHideDjinnSprs(void);
void mnFileListPlaceDjinnSprs(void);

/* What the file list is choosing a file for, which decides the files the */
/* cursor may stop on. */
typedef enum FILE_MENU_MODE {
	FILE_LIST_SAVE,		/* saving: any file, empty ones too */
	FILE_LIST_CONTINUE, /* continuing: files not yet cleared */
	FILE_LIST_COPY,		/* copying: any file with data */
	FILE_LIST_ERASE,	/* erasing: any file with data */
	/* The title's Battle: files whose party has found a Djinni */
	/* (SAVE_FILE_SUMMARY.djinnCnt). */
	FILE_LIST_BATTLE,
	FILE_LIST_CLEARED /* the title's Send: cleared files only */
} FILE_MENU_MODE;

void mnFileListDrawDjinnCounts(struct WINDOW *window,
							   struct SAVE_FILE_SUMMARY *summary);
void mnFileListDrawSummary(struct WINDOW *window,
						   struct SAVE_FILE_SUMMARY *summary);
s32 mnFileListSelectFile(s32 selectedFile, s32 menuMode);
s32 mnNameEntryAcceptsMark(s32 mark, s32 letter);
s32 mnNameEntryCountLetters(const u8 *name);
void mnWindowCopyTilemapRegion(const struct WINDOW *window, const void *source);
void mnWindowSetInteriorPalette(const struct WINDOW *window, s32 x, s32 y,
								s32 width, s32 height, u32 palette);
s32 mnTextSetString(const u8 *str);
void mnNameEntryDrawName(struct WINDOW *window, u8 *name);
s32 mnPartyGetMemberPortraitSpr(u32 member);
void mnTitleLoad(s32 picture, s32 slot);
struct WINDOW_SPR *mnTitleCreate(s32 picture, struct WINDOW *window, s32 x,
								 s32 y);
void mnTitleBob(struct WINDOW_SPR *sprite);
void mnCommandIconLoad(s32 icon, s32 slot, s32 remap);
struct WINDOW_SPR *mnCommandIconCreate(s32 icon, s32 remap,
									   struct WINDOW *window, s32 x, s32 y);
void mnWindowSprPulse(struct WINDOW_SPR *sprite);

struct BATTLE_ACTION;
struct ABILITY_DEFINITION;
struct ICON_PALETTE;
struct PSYNERGY_ENTRY;
struct BATTLE_COMMAND_ROW_STATE;

UNREAD_RESULT mnRangeIconBuildTiles(void);
UNREAD_RESULT mnRangeIconDraw(struct WINDOW *window, s32 x, s32 y, s32 pattern,
							  s32 unused);
void mnGraphicsMergeShiftedTileRows(u32 *first, u32 *second, u32 *output,
									s32 shift);
s32 mnStatIconLoadIntoSlot(s32 resource, s32 index);
s32 mnItemIconLoadIntoSlot(s32 resource, s32 index);
s32 mnAbilityIconLoadIntoSlot(s32 resource, s32 index);
s32 mnPortraitLoad(u32 member, s32 cacheSlot);
void mnIconDecompress(ICON_TRANSFER *state);
struct WINDOW *mnLinkCreateCountdownLabelWindow(void);
s32 mnCommandIconReloadSlot(s32 slotIndex, s32 icon);
void mnBattleCommandRowLoadIconTiles(ICON_PALETTE *palette, s32 glyph,
									 s32 tile);
void mnBattleCommandRowLoadIcon(struct BATTLE_COMMAND_ROW_STATE *state,
								s32 index, u32 icon);
void mnCommandRowPutBg1Behind(void);
void mnCommandRowPutBg1InFront(void);
void mnGraphicsInstallBg1VCountHandler(void);
struct WINDOW *mnMessageShowAndWait(s32 message, s32 x, s32 y);
s32 mnBattleCommandRowRun(s32 mode);
UNREAD_RESULT mnStringFormatNumToHalfwords(s16 *out, s32 value);
s32 mnBattleDjinnMenuListChangedAbilities(const struct PSYNERGY_ENTRY *oldGrid,
										  const struct PSYNERGY_ENTRY *newGrid,
										  u16 *out, s32 *gained, s32 *lost);
UNREAD_RESULT mnWindowSprCreateAbilityIcon(struct WINDOW *window, s32 x, s32 y,
										   s32 abilityId);
struct WINDOW *mnBattleDjinnMenuShowChangePreview(struct WINDOW *window,
												  s32 member, s32 encodedDjinn,
												  s32 page, s32 *pageCount);
s32 mnBattleStatScrRun(u16 *list, s32 listCount, s32 member);
s32 mnBattleDjinnMenuRun(s32 member, s32 unused);
s32 mnBattleSummonMenuRun(s32 unusedA, s32 unusedB, const u8 *standby);
s32 mnItemCheckBattleUse(s32 unit, s32 itemId);
void mnVramCopyTile(u32 sourceTile, u32 destinationTile);
s32 mnBattleItemMenuRun(s32 member, u16 *entries, s32 count);
s32 mnBattlePsynergyMenuRun(s32 unitId, u16 *actions, s32 count);
s32 mnBattleTargetRunSelection(s32 selection, s32 mode, u32 spread, u32 kind);
void mnBattleMenuUpdate(void);
s32 mnBattleRefuseFlee(void);
s32 mnBattleClassifyEntryKind(const struct ABILITY_DEFINITION *act);

void mnIconMenuDrawIcons(void);
void *mnIconMenuOpen(void);
void mnIconMenuClose(void);
s32 mnIconMenuRun(s32 initial);
s32 mnIconMenuAnimateTo(s32 start, s32 goal);
void mnCommandIconLoadIntoSlot(s32 slot, s32 index);
void mnIconMenuAddIcon(s32 icon);
void mnIconMenuCenter(s32 row, s32 width, s32 resourceBase);
void mnIconMenuLayoutIcons(s32 x, s32 y, s32 width, s32 resourceBase);
/* The field menu's commands, as mnFieldMenuSelectCommand takes and returns */
/* them (FieldMenu_CommandByPosition). */
typedef enum FIELD_MENU_COMMAND {
	FIELD_MENU_PSYNERGY = 1,
	FIELD_MENU_DJINN = 2,
	FIELD_MENU_ITEM = 3,
	FIELD_MENU_STATUS = 4
} FIELD_MENU_COMMAND;

/* The title menu's choices, as mnFileListSelectAction returns them */
/* (FileList_ActionByPosition). */
typedef enum FILE_ACTION {
	SAVE_ACTION_NEW_GAME = 0,
	SAVE_ACTION_CONTINUE = 1,
	SAVE_ACTION_COPY = 2,
	SAVE_ACTION_ERASE = 3,
	SAVE_ACTION_BATTLE = 4,
	SAVE_ACTION_SEND = 5
} FILE_ACTION;

/* What mnIconMenuChooseSendOption offers. */
typedef enum RESOURCE_LAYOUT_MODE {
	SEND_OPTION_TRANSFER_METHOD = 0, /* Password or Cable */
	SEND_OPTION_PASSWORD_LEVEL = 1	 /* Gold, Silver or Bronze */
} RESOURCE_LAYOUT_MODE;

/* The places of the transfer method menu (SEND_OPTION_TRANSFER_METHOD). */
typedef enum TRANSFER_METHOD {
	TRANSFER_METHOD_PASSWORD = 0,
	TRANSFER_METHOD_CABLE = 1
} TRANSFER_METHOD;

/* The places of the password level menu (SEND_OPTION_PASSWORD_LEVEL). */
typedef enum PASSWORD_LEVEL {
	PASSWORD_LEVEL_GOLD = 0,
	PASSWORD_LEVEL_SILVER = 1,
	PASSWORD_LEVEL_BRONZE = 2
} PASSWORD_LEVEL;

/* What mnYesNoMenuRun and mnYesNoMenuRunAt return. */
typedef enum YES_NO { YES = 0, NO = 1 } YES_NO;

s32 mnFieldMenuSelectCommand(s32 selection);
void mnPasswordLevelDrawHelp(void);
void mnTransferMethodDrawHelp(void);

void mnDebugSceneSelectSchedule(void);
void mnDebugSceneSelectDrawRow(struct WINDOW *window, s32 sceneId,
							   const s16 *entrance);
s32 mnDebugSceneSelectHandleRowInput(struct WINDOW *window, s16 sceneId,
									 s16 *entrance, s16 *selectedField);
void mnStringFormatHexDigits(u32 value, u32 count, u8 *out);
void mnDebugFlagEditorDrawTable(struct WINDOW *window, s32 page);
s32 mnDebugFlagEditorHandleInput(struct WINDOW *window, s32 *page, s32 *cursor);
void mnReservedNoOp(void);
void mnDjinnSetFoundFlagAndAdd(s32 member, s32 element, s32 index);

/* The Japanese name entry screen's second keyboard, which Select switches to.
 */
extern const u8 NameEntry_AlternateKeyboardTilemap[];
/* Eight offsets, 0 to 3 and back, that make the name entry screen's caret */
/* circle: the x offset leads the y offset by five steps. */
extern s8 NameEntry_CaretWave[];
/* The kana that take the voiced mark, as first and last pairs ended by 0. */
extern const u8 NameEntry_VoicedMarkRanges[];
/* The Delete key's label on the German, Spanish and French name entry screen */
/* keyboards ("ENT", "SUP", "EFF"). */
extern u8 NameEntry_DeleteKeyLabel[];
/* The End key's label on the German, Spanish and French keyboards ("ENDE", */
/* "FIN", "FIN"). */
extern u8 NameEntry_EndKeyLabel[];
/* The kana that take the semi-voiced mark, in the same pairs. */
extern const u8 NameEntry_SemivoicedMarkRanges[];
/* The name entry screen keyboard's compressed tilemap, the Spanish edition's */
/* with its own letters. */
extern const u8 NameEntry_KeyboardTilemap[];
/* The 16x16 sprite the name entry screen shows as its name caret. */
extern const u8 NameEntry_CaretTiles[];

#endif
