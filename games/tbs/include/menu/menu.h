#ifndef ALCHEMY_MENU_H
#define ALCHEMY_MENU_H

/* @file */
/* The menus' list orders, sign strings and functions. */

#include "fixed_math.h"
#include "types.h"
#include "edition.h"
#include "stubs.h"
#include "system/globals.h"

/* The Status screen highlight's width in tiles on each status row: Down, */
/* Poison, Venom, Curse and Haunt. */
extern u8 StatusScreen_CursorWidths[];
extern u8 Menu_ListOrderItemsFirst[];
extern u8 Menu_ListOrderEquippedThenItems[];
extern u8 Menu_ListOrderEquippedThenEquipment[];
extern u8 Menu_ListOrderEquipmentFirst[];
/* `-`, before a stat change that is not a gain (mnItemMenuDrawStat). */
extern u8 Menu_MinusSignString;
/* `+`, before a stat gain. */
extern u8 Menu_PlusSignString;
/* The field menu's command by icon position plus one, a row of 6 with the */
/* Djinn icon and one without: 1 Psynergy, 2 Djinn, 3 Item, 4 Status */
/* (mnFieldMenuSelectCommand). */
extern s8 FieldMenu_CommandByPosition[];
/* The inverse: each command's icon position plus one, in the same two rows. */
extern s8 FieldMenu_PositionByCommand[];
/* The price digits 0 to 9, 32 bytes each (digits.4bpp), copied into a price */
/* sprite's tiles (mnShopDrawPriceDigit). */
extern u8 Shop_GlyphBytes[];
/* The level label of a save summary: `L`; `Nv.` in Spanish, `Niv` in French. */
extern u8 StatusScreen_LevelLetterString[];

struct SPR;
struct ACTOR_SYSTEM_STATE;
struct SELECTION_MENU_STATE;
struct MENU_CURSOR;

/* A background's horizontal and vertical scroll offsets. */
typedef struct SCROLL_PAIR {
	u16 x; /* pixels */
	u16 y; /* pixels */
} SCROLL_PAIR;
/* The small down arrow, 16 by 16 (sdown): the item menu's stat drop. */
extern u8 Menu_SmallDownArrowTiles[];
/* The small up arrow, then the up and down arrows, 16 by 16 each; the item */
/* menu's stat rise loads the first (mnLoadStatArrows). */
extern u8 Menu_ArrowTiles[];
/* The Djinn menu's up arrow, 16 by 16 (graphics/ui/djinn_arrow_up). */
extern u8 Menu_DjinnArrowUpTiles[];
/* The Djinn menu's down arrow, 16 by 16 (graphics/ui/djinn_arrow_down). */
extern u8 Menu_DjinnArrowDownTiles[];
/* The item menu's quantity picture (quantity.4bpp). */
extern const u8 Menu_QuantityTiles[];
/* The Djinn menu's level label: `Lv`; `Nv.` in Spanish, `Niv` in French. */
extern const char DjinnMenu_TextLevel[];
/* `/`, between the Djinn menu's numbers. */
extern const char DjinnMenu_TextSlash[];
/* Heap slot 6: the actor system's state (ACTOR_SYSTEM_STATE). */
#define ActorSystemStateWork                                                   \
	(*(struct ACTOR_SYSTEM_STATE **)&HeapSlots[HEAP_SLOT_ACTOR_SYSTEM])
/* Heap slot 18: the selection screen while it runs. */
#define SelectionMenuStateWork                                                 \
	(*(struct SELECTION_MENU_STATE **)&HeapSlots[HEAP_SLOT_SELECTION_MENU])
/* Nonzero offers Battle on the File Screen, when a file */
/* has any of game flags 48 to 127 set. */
extern s16 TitleBattleOptionEnabled;
/* The Japanese item menu's help: `L+A:`, before the arrange help. */
extern u8 ItemMenu_ArrangeKeysString[];
extern s32 ItemMenu_CommandColumnXTable[];
/* The Japanese item menu's help: `R:`. */
extern u8 ItemMenu_EquipmentKeyString[];
/* One 4-bit tile (backdrop.4bpp), a down arrow: the Djinn screens draw it */
/* between a thing and what a change makes of it (a Djinni set and on */
/* standby, a class and the next), and the Djinn preview blinks it, drawing it
 */
/* and a fill of the window background (TEXT_CANVAS_BACKGROUND_WORD) in turn */
/* every four frames. */
extern const u8 Menu_ChangeArrowTile[];
/* The colon the debug menus' scene and flag rows draw */
/* (window/screens_debug.c): `:`. */
extern const u8 Menu_ColonString[];
/* The selection screen's left arrow, 8 by 16 at 256 colours */
/* (graphics/ui/sprite_cursors/left). */
extern u8 Menu_CursorLeftActorTiles[];
/* `0123456789ABCDEF`, drawn by a debug window. */
extern const u8 Menu_HexDigitsString[];
/* The HP label: `HP`; `KP` in German, `PV` in Spanish and French, `PS` in */
/* Italian. */
extern const u8 Menu_HpString[];
/* The Status screen's level label: `Lv`; `Nv.` in Spanish, `Niv` in French. */
extern const u8 Menu_LvString[];
/* The four Djinn sprites, one per element from SPRITE_VENUS_DJINNI to */
/* SPRITE_JUPITER_DJINNI, that mnFileListShowDjinnSprs shows in a row. */
extern const s32 FileList_DjinnSpriteIds[4];
/* `.`, the decimal point of an item's stat multiplier (`x1.2`). */
extern u8 Menu_PointString[];
/* The PP label: `PP`; `EP` in Japanese. */
extern const u8 Menu_PpString[];
/* The File Screen's action by icon position, a row of 6 for each layout: */
/* 0 New Game, 1 Continue, 2 Copy, 3 Erase, 4 Battle, 5 Send. */
extern s8 FileList_ActionByPosition[];
/* Frames mnIconMenuAnimateTo waits at each icon on its way, by icons still to
 */
/* go. */
extern u8 Menu_SelectionStepDelays[];
/* `L:`, the Japanese Psynergy menu's first shortcut label. */
extern const u8 Menu_ShortcutLColonString[];
/* `L`, the first shortcut's label beside an ability name wider than 11 */
/* (Japanese only). */
extern const u8 Menu_ShortcutLString[];
/* `R:`, the second shortcut's label (Japanese only). */
extern const u8 Menu_ShortcutRColonString[];
/* `R`, the second shortcut's label beside an ability name wider than 11 */
/* (Japanese only). */
extern const u8 Menu_ShortcutRString[];
/* `/`, between a current and a maximum. */
extern const u8 Menu_SlashString[];
/* `x`, before an item's stat multiplier (not in Japanese). */
extern u8 Menu_TimesString[];
extern s8 SettingsMenu_SleepIcons[];
extern s8 SettingsMenu_SpeechIcons[];
extern s8 SettingsMenu_SpeedIcons[];
/* The status panel's level label: `Lv`; `Nv.` in Spanish, `Niv` in French. */
extern u8 StatusPanel_LevelString[];
extern u8 MenuCursor_BobX[];
extern u8 MenuCursor_BobY[];
/* A debug window's title: `ENERGY`. */
extern u8 DebugMenu_EnergyTitle[];
/* A debug window's title: `ITEM`. */
extern u8 DebugMenu_ItemTitle[];
/* A debug window's label: `PAGE:`. */
extern u8 DebugMenu_PageLabel[];
/* `-`, between the first and last entry numbers of a debug window's page */
/* (32 a page). */
extern u8 DebugMenu_RangeDash[];
/* A debug window's title: `STATUS`. */
extern u8 DebugMenu_StatusTitle[];
/* The settings menu's slider colours (graphics/ui/slider_palette). */
extern const u16 SettingsMenu_SliderPalette[];
/* The settings menu's slider, 32 by 8 (slider.4bpp32x8). */
extern const u8 SettingsMenu_SliderTiles[];
/* The title's imports of mnTitleShowCamelotLogoStub and mnCreditsRunStub, */
/* called with an argument the functions do not take. */
void syImportTitleShowCamelotLogo(s32 mode);
void syImportCreditsRun(s32 mode);

struct UNIT;
struct DJINN_LIST_TABLE;
struct ITEM_MENU_STATE;
struct PAGED_LIST;
struct PSYNERGY_ENTRY;
struct WINDOW;
struct WINDOW_SPR;

/* The menu module's functions in the order of its stub table */
/* (src/menu/stubs.s). */
s32 mnItemMenuRun(void);
s32 mnPsynergyMenuRun(void);
s32 mnStatScrRun(void);
s32 mnPartyMemberChooserOpen(void);
s32 mnIconCreateHandCursor(struct WINDOW *window, s32 x, s32 y);
void mnCreatePartyFigures(void *source, s32 originX, s32 originY, s32 spacing,
						  s32 unused);
void mnReleasePartyFigures(void);
void mnIconShow(struct WINDOW_SPR *object);
s32 mnDjinnMenuRun(void);
void mnItemMenuDrawItemDetails(struct WINDOW *window, s32 inventoryEntry);

/* The item menu's sort orders (Menu_ListOrder*): one byte per group, an */
/* item type, with LIST_ORDER_EQUIPPED for only the equipped items of that */
/* type, ending with LIST_ORDER_END. */
#define LIST_ORDER_TYPE_MASK 0x7f
#define LIST_ORDER_EQUIPPED	 0x80
#define LIST_ORDER_END		 255

/* The item menu's four sort orders (mnItemMenuSort), which */
/* L + A cycles through in this order. */
typedef enum LIST_SORT {
	/* The equipped items, the items that are not equipment, then the other */
	/* equipment (Menu_ListOrderEquippedThenItems). */
	LIST_SORT_EQUIPPED_THEN_ITEMS = 0,
	/* The equipped items, the other equipment, then the rest */
	/* (Menu_ListOrderEquippedThenEquipment). */
	LIST_SORT_EQUIPPED_THEN_EQUIPMENT = 1,
	LIST_SORT_EQUIPMENT_FIRST = 2, /* Menu_ListOrderEquipmentFirst */
	/* The items that are not equipment first (Menu_ListOrderItemsFirst). */
	LIST_SORT_ITEMS_FIRST = 3,
	LIST_SORT_COUNT = 4
} LIST_SORT;

/* What the menus share (menu_shared.c): their state, windows, status panel, */
/* hand cursor, party figures and entry icons, the paged lists and their */
/* highlights, and the watch for Start. */
s32 mnResumeFieldTasks(void);
s32 mnSuspendFieldTasks(void);
void mnInitState(s32 unused);
s32 mnWindowUpdateOrCreate(struct WINDOW **handle, s32 x, s32 y, s32 w, s32 h,
						   s32 flags);
void mnWindowCloseIfOpen(struct WINDOW **handle, s32 instant);
void mnStatPanelDraw(struct WINDOW *unused, s32 member, s32 slot, s32 mode);
void mnTextDrawNumRightAligned(s32 number, struct WINDOW *window, s32 x, s32 y);
void mnStatPanelDrawHpPp(struct UNIT *unit, struct WINDOW *window);
void mnTextDrawStatComparison(const struct UNIT *after,
							  const struct UNIT *before, struct WINDOW *window);

s32 mnIconCreateHandCursorUnusedArgument(struct WINDOW *window, s32 unused,
										 s32 x, s32 y);
void mnReloadMemberPortrait(const void *unused, s32 unit);

struct WINDOW *mnCursorCreate(void *source);
void mnPlacePartyFigures(void);
void mnCursorPos(s32 xOffset, s32 yOffset);
void mnCursorSlide(s32 x, s32 y);
void mnCursorNoOp(void);

void mnPsynergyMenuPlaceEntryIconsDefault(void);
void mnPsynergyMenuPlaceEntryIcons(s32 originX, s32 originY, s32 columns);
void mnPsynergyMenuPlaceEntryIcon(struct WINDOW_SPR **slot, s32 index,
								  s32 originX, s32 originY, s32 columns);
void mnItemMenuPlaceIcon(struct WINDOW_SPR **slot, s32 slotIndex, s32 originX,
						 s32 originY, s32 columns);
void mnItemMenuPlaceIcons(s32 layout);
void mnItemMenuShowMessage(s32 message, s32 x, s32 y);
s32 mnItemMenuSort(u16 *inventory, s32 mode);
void mnCopyListOrder(s32 mode, u8 *order);

s32 mnPagedListHandleInput(s32 horizontal, s32 count, s32 pageSize, s32 *cursor,
						   s32 *page);
void mnPaletteLightenHighlight(s32 paletteNumber);
void mnPagedListDrawPageNumbers(struct WINDOW *window, s32 itemCount,
								s32 pageSize, s32 selectedPage, s32 rightEdge);
void mnWindowSetHighlightRect(const struct WINDOW *window, s32 x, s32 y,
							  s32 width, s32 height, u32 palette);
void mnPaletteLoadIconsIntoHighlight(void);
void mnSetPageIcons(s32 pageSize, s32 first, struct WINDOW *window, s32 x,
					s32 y);
void mnDrawCoins(struct WINDOW *window);

void mnWindowSetBounds(struct WINDOW *window, s32 x, s32 y, s32 width,
					   s32 height);
void mnWindowHoldTilemapUpload(void);
void mnWindowReleaseTilemapUpload(void);
s32 mnSndPlayReturnOne(s32 sound);
void mnWatchStart(void);
void mnStartWatchingStart(void);
void mnStopWatchingStart(void);
void mnWindowPaletteSetNormalTextCol(void);
void mnWindowPaletteSetNoHpTextCol(void);
void mnWindowPaletteSetLowHpTextCol(void);

/* The rows a page of the Psynergy and item lists holds. */
#define LIST_PAGE_SIZE 5

/* The y of the menus' row of party portraits, and of the chosen member's, */
/* raised above the rest. */
#define MENU_MEMBER_Y		 30
#define MENU_MEMBER_RAISED_Y 26
/* The cursor's x beside the party row's portrait `member`: the portraits stand
 */
/* 24 pixels apart. */
#define MENU_MEMBER_CURSOR_X(member) ((member) * 24 - 10)

/* The Item menu (menu_item.c): its member and command choices, the item list */
/* and its pages, the details, giving, dropping and equipping. */
s32 mnItemMenuRunCommands(s32 *memberOut, s32 *targetOut, s32 *itemOut);
s32 mnItemMenuUse(void);
void mnItemMenuInit(s32 x, s32 y, s32 mode, s32 columns);
void mnSpawnIconEntries(struct ITEM_MENU_STATE *menu, struct WINDOW *window);
void mnItemMenuHideAllIcons(void);
void mnItemMenuHidePageIcons(void);
void mnItemMenuClose(void);
s32 mnItemMenuSelectMember(s32 side);
s32 mnItemMenuRunMemberSelection(u16 *memberIds, u16 *items);
void mnItemMenuShowEquipment(s32 memberId);
s32 mnItemMenuSelectTarget(s32 mode);
void mnEquipmentMenuUpdateCompatibilityIndicators(void);
void mnEquipmentMenuStopCompatibilityIndicators(void);
s32 mnItemMenuIsStatRaisingItem(s32 itemId);
void mnItemMenuDrawMessage(s32 unused, s32 message);
void mnHideEmptyEntryIcons(const u16 *items);
s32 mnItemMenuCountItems(s32 memberId);
s32 mnItemMenuGetItemQuantity(s32 memberId, s32 itemId);
s32 mnItemMenuCollectItems(struct UNIT *unit, u16 *items, s32 mode);
void mnItemMenuDrawIcons(u16 *inventoryEntries, s32 iconStyle);
void mnItemMenuRefreshMember(s32 memberId, s32 mode);
void mnItemMenuNoOp(void);
void mnItemMenuDrawEquipPreview(s32 member, s32 slot, s32 mode, s32 target);
s32 mnInventoryRemoveFirstUnflagged(s32 memberId);
s32 mnItemMenuCommandCursorX(s32 column, s32 row);
s32 mnItemMenuCommandCursorY(s32 column, s32 row);
s32 mnItemMenuChooseCommand(void);
void mnItemMenuBuildCommands(s8 *commandStates);
void mnItemMenuDrawCommands(const s8 *commandStates, struct WINDOW *window);
s32 mnItemClassifyUseMode(s32 memberId, s32 inventoryEntry);
void mnItemMenuTryBreak(void);
s32 mnItemMenuOpenDetail(s32 itemIndex);
s32 mnItemMenuShowDetails(s32 value);
void mnItemMenuDrawStat(s32 statChange, s32 unused, struct WINDOW *window,
						s32 x, s32 y);
void mnItemMenuPlaceItemWindowBelowItemQuestion(void);
void mnItemMenuPlaceItemWindowBelowQuestion(void);
void mnItemMenuFitMessageWindowToItemQuestion(void);
void mnItemMenuFitMessageWindowToLongItemQuestion(void);
void mnItemMenuFitMessageWindowToItemChoices(void);
void mnItemMenuFitMessageWindowToQuestion(void);
s32 mnItemMenuSelectGiveQuantity(s32 baseQuantity, s32 quantityRange,
								 s32 dropMode);
void mnItemMenuDrawItemHead(void);
s32 mnItemMenuConfirmDrop(s32 item);
s32 mnItemMenuConfirmEquip(s32 unused);
void mnLoadStatArrows(void);
s32 mnItemMenuBuildPagedList(struct PAGED_LIST *result, s32 index);
s32 mnItemMenuShowSelectedRow(struct WINDOW *window, void *memberState,
							  struct PAGED_LIST *pageState);
s32 mnItemMenuDrawListPage(struct WINDOW *window, s32 unused,
						   const struct PAGED_LIST *pageState);
s32 mnItemMenuItemNamePageReturnTrue(void);
void mnItemMenuItemNamePageNoOp(void);
s32 mnItemMenuRunList(s32 sideIndex);

/* The Psynergy menu (menu_psynergy.c): its member and target choices, the */
/* shortcuts, and the Psynergy list and its pages. */
s32 mnPsynergyMenuRunSteps(s32 *outMember, s32 *unused, s32 *outAction);
s32 mnPsynergyMenuClassifySelectedPsynergy(void);
s32 mnPsynergyMenuSelectPartySlot(s32 partySlot);
s32 mnPsynergyMenuSelectMember(u16 *memberIds, u16 *unused);
void mnPsynergyMenuRefreshMemberPsynergy(s32 memberId);
s32 mnPsynergyMenuReturnTrue(void);
void mnPsynergyMenuNoOp(void);
s32 mnPsynergyMenuSelectTarget(s32 targetMode);
s32 mnPsynergyMenuSetShortcut(s32 memberId, s32 abilityId, s32 shortcutIndex);
s32 mnPsynergyMenuDrawShortcuts(struct WINDOW *window, s32 member);
void mnPsynergyMenuCreateEntryGrid(void);
void mnPsynergyMenuCloseWindows(void);
void mnPsynergyMenuDrawPsynergyIcons(u16 *psynergies);
s32 mnPsynergyMenuCollectActions(struct UNIT *unit, u16 *actions, s32 mode);
s32 mnPsynergyMenuBuildPagedList(struct PAGED_LIST *result, s32 index);
s32 mnPsynergyMenuShowSelectedRow(struct WINDOW *window, s32 *memberState,
								  struct PAGED_LIST *state);
s32 mnPsynergyMenuDrawPage(struct WINDOW *window, s32 unused,
						   const struct PAGED_LIST *pageState);
s32 mnPsynergyMenuRunList(s32 sideIndex);
s32 mnPsynergyMenuIsActionRestricted(s32 actionEntry);

/* The party member chooser and the Status menu (menu_status.c): the party */
/* row, the status page, the Psynergy page and the equipment page. */
s32 mnPartyMemberChooserRun(void);
s32 mnStatScrRunPages(void);
s32 mnStatScrSelectMember(s32 slot);
s32 mnStatScrShowDjinnList(void);
s32 mnStatScrRunPartyRow(void);
s32 mnPartyMemberChooserChooseMember(void);
s32 mnStatScrMovePartyMember(s32 selectedIndex, s32 direction);
void mnStatScrInit(s32 x, s32 y, s32 mode, s32 columns);
void mnStatScrShowMemberStat(s32 member, s32 frame);
s32 mnStatScrRunStatPage(void);
void mnStatScrDrawSelectionCursor(s32 side, s32 selected, u8 *entries,
								  s32 invert);
void mnStatScrDrawSelectionLabels(struct WINDOW *target, s32 selected,
								  const u8 *entries);
void mnStatScrShowMemberProgressMessage(struct WINDOW *destination, s32 row,
										s32 hasDjinn);
void mnStatScrDrawStatPage(struct WINDOW *window, s32 memberId, s32 mode);
void mnBusyWait(void);
void mnStatScrDrawTopRows(struct WINDOW *window, s32 memberId, s32 flags);
s32 mnStatScrListStatuses(u8 *ailments, s32 requested, s32 memberId);
s32 mnPagedListBuild(struct PAGED_LIST *result, s32 index);

void mnBuildPatternTiles(void);
void mnPsynergyMenuDrawRange(struct WINDOW *window, s32 x, s32 y, s32 range,
							 s32 unused);
s32 mnStatScrShowPsynergyRow(struct WINDOW *window, s32 unused,
							 struct PAGED_LIST *state);
s32 mnStatScrDrawPsynergyPage(struct WINDOW *window, s32 unused,
							  const struct PAGED_LIST *res);
s32 mnStatScrRunPsynergyPage(void);
s32 mnStatScrPsynergyPageReturnTrue(void);
void mnPsynergyMenuDrawPreparedPsynergyIcons(const void *unused, s32 memberId);
void mnPsynergyMenuPreparedIconsNoOp(void);
void mnPsynergyMenuPreparedIconsSecondNoOp(void);
s32 mnStatScrShowEquipmentRow(struct WINDOW *window, s32 unused,
							  struct PAGED_LIST *state);
s32 mnStatScrDrawEquipmentPage(struct WINDOW *window, s32 unused,
							   struct PAGED_LIST *state);
s32 mnStatScrRunEquipmentPage(void);
s32 mnMoveGridCursor(s32 *columnOut, s32 *rowOut, s32 direction);
s32 mnGridCursorReturnTrue(void);

void mnItemMenuDrawEquipment(struct WINDOW *window, s32 memberId, s32 mode);
void mnItemMenuDrawEquippedItemNames(struct WINDOW *window, u16 *items);
void mnPlaceEntryIconsInGrid(s32 originX, s32 originY, s32 columns);
void mnPlaceEntryIconInGrid(struct WINDOW_SPR *icon, s32 index, s32 originX,
							s32 originY, s32 columns);
void mnItemMenuPlaceEquippedIcons(u16 *items);
void mnHideEntryIcons(void);
s32 mnStatScrCreateStatIcons(struct WINDOW *window);
void mnStatScrShowStatIcons(const u8 *flags);
void mnStatScrHideStatIcons(void);
s32 mnStatScrLoadStatIcons(const u8 *enabled);

/* Using an item or Psynergy from the menus (menu_use.c). */
void mnItemPrepareUsePalette(void);
void mnItemUseNoOp(void);
s32 mnItemUse(s32 slot, s32 memberId, s32 targetId);
s32 mnItemUseReturnTrue(void);
s32 mnAbilityApplyToTargets(s32 effectId, s32 sourceId, s32 targetId,
							s32 fixedScale);
void mnItemPlayUseSnd(u32 item);
void mnAbilityPlayUseSnd(s32 action);
s32 mnAbilityReturnFalse(void);

/* The Djinn menu (menu_djinn.c): its screen, the Djinn lists and their help. */
s32 mnWrapIndex(s32 value, s32 modulus);
s32 mnDjinnMenuPlacePartyFigures(s32 yOffset);
s32 mnDjinnMenuRunScr(void);
void mnDjinnMenuAdjustPalettes(s32 brightnessDelta);
#ifdef UBFIX
typedef void MENU_BACKDROP_RESULT;
#else
typedef s32 MENU_BACKDROP_RESULT;
#endif
/* UB: mnOpenBackdropScr returns the result of mnDjinnMenuDrawElementList, */
/* called with no prototype though it returns nothing; nothing reads it. */
MENU_BACKDROP_RESULT mnOpenBackdropScr(void);
s32 mnPsynergyListDiff(struct PSYNERGY_ENTRY *before,
					   struct PSYNERGY_ENTRY *after, u16 *actionsOut,
					   s32 *gainedCountOut, s32 *lostCountOut);
s32 mnDjinnMenuBuildLists(void *table);
#ifdef UBFIX
void mnDjinnMenuDrawElementList(struct DJINN_LIST_TABLE *table);
#endif
s32 mnWindowSetInteriorPaletteBitRect(const struct WINDOW *window, s32 x, s32 y,
									  s32 width, s32 height, s32 palette);
void mnWindowSetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette);
void mnWindowSetInteriorRectPalette(void *window, s32 x, s32 y, s32 width,
									s32 height, s32 palette);
s32 mnDjinnMenuShowHelp(void);
s32 mnDjinnMenuSelectDjinn(s32 mode);
s32 mnDjinnListUnitEntries(u16 *entriesOut, s32 memberId, s32 element);
s32 mnDjinnMenuDrawPairPreview(s32 mode, s32 pageIndex);
s32 mnDjinnMenuDrawStatPreview(struct WINDOW *window, s32 x, s32 y,
							   s32 memberId, s32 removePairSlot,
							   s32 addPairSlot, s32 previewMode, s32 page,
							   s32 side);

/* The rows of Djinn figures over the Djinn and Status menus */
/* (menu_djinn_figures.c). */
void mnDjinnFiguresInitTopRow(void *window, s32 unused);
void mnDjinnFiguresUpdateAllPoss(void);
void mnDjinnFiguresUpdateBottomRow(void);
void mnDjinnFiguresInitBottomRow(s32 window, s32 unused);
s32 mnDjinnFiguresSetSlotPos(s32 index, s32 x, s32 y, s32 negative);
s32 mnDjinnFiguresSetSlotPhase(s32 index, FIXED phase);
s32 mnDjinnFiguresReplaceSlot(s32 index, s32 element, s32 animation);
void mnStartPartyFiguresStanding(void);
void mnDjinnFiguresReleaseTopRow(void);
void mnDjinnFiguresReleaseBottomRow(void);

/* The Djinn menu's preview of a change before it is made, and the checks that
 */
/* keep the party members' Djinn counts even (menu_djinn_preview.c). */
s32 mnDjinnMenuRunPreview(u32 mode);
s32 mnDjinnMenuShowMemberStat(void);
s32 mnDjinnMarkTradeOnlyRecipients(s8 *requiresTrade, s32 sourceMemberColumn);
s32 mnDjinnCheckGiveBalance(s32 sourceMemberColumn, s32 targetMemberColumn);
void mnDjinnCountOwned(u8 *ownedCounts);

/* The menus' small icons: the arrows and the Psynergy icons (menu_icons.c). */
void mnLoadDjinnArrows(void);
void mnIconMenuReleaseArrows(void);
s32 mnIconLoadResourceIntoSlot(s32 abilityId, s32 slot);
s32 mnIconCreatePsynergyIcon(struct WINDOW *window, s32 x, s32 y,
							 s32 abilityId);
s32 mnIconCreateStatChangeArrow(struct WINDOW *window, s32 x, s32 y,
								s32 variant);
s32 mnIconCreateDjinnArrow(struct WINDOW *window, s32 x, s32 y, s32 variant);

/* The Japanese status panel's PP row, "EP    /": label and slash in one */
/* string, drawn by the unit summary in menu/menu_shared.c. */
extern const u8 StatusPanel_PpRowString[];
/* The Japanese status panel's HP row, "HP    /", the slash already in its */
/* cell at 48. */
extern const u8 StatusPanel_HpRowString[];
/* The HP label outside Japan, in each language's form ("KP" in German, "PV" */
/* in Spanish and French, "PS" in Italian). */
extern const u8 StatusPanel_HpString[];
/* The PP label outside Japan, which every language writes "PP". */
extern const u8 StatusPanel_PpString[];
/* The slash between a current and maximum figure on the status panel, */
/* outside Japan. */
extern const u8 StatusPanel_SlashString[];

#endif
