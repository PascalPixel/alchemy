#ifndef ALCHEMY_MENU_H
#define ALCHEMY_MENU_H

/* The menus' list orders, sign strings and functions. */

#include "fixed_math.h"
#include "types.h"
#include "edition.h"
#include "stubs.h"
#include "system/globals.h"

/* The Status screen highlight's width in tiles on each status row: Down,
   Poison, Venom, Curse and Haunt. */
extern Uint8 StatusScreen_CursorWidths[];
extern Uint8 Menu_ListOrderItemsFirst[];
extern Uint8 Menu_ListOrderEquippedThenItems[];
extern Uint8 Menu_ListOrderEquippedThenEquipment[];
extern Uint8 Menu_ListOrderEquipmentFirst[];
/* `-`, before a stat change that is not a gain (mnItemMenuDrawStat). */
extern Uint8 Menu_MinusSignString;
/* `+`, before a stat gain. */
extern Uint8 Menu_PlusSignString;
/* The field menu's command by icon position plus one, a row of 6 with the Djinn
   icon and one without: 1 Psynergy, 2 Djinn, 3 Item, 4 Status
   (mnFieldMenuSelectCommand). */
extern Sint8 FieldMenu_CommandByPosition[];
/* The inverse: each command's icon position plus one, in the same two rows. */
extern Sint8 FieldMenu_PositionByCommand[];
/* The price digits 0 to 9, 32 bytes each (digits.4bpp), copied into a price
   sprite's tiles (mnShopDrawPriceDigit). */
extern Uint8 Shop_GlyphBytes[];
/* The level label of a save summary: `L`; `Nv.` in Spanish, `Niv` in French. */
extern Uint8 StatusScreen_LevelLetterString[];

struct SPR;
struct ACTOR_SYSTEM_STATE;
struct SELECTION_MENU_STATE;
struct MENU_CURSOR;

/* A background's horizontal and vertical scroll offsets. */
typedef struct SCROLL_PAIR {
	Uint16 x; /* pixels */
	Uint16 y; /* pixels */
} SCROLL_PAIR;

/* The small down arrow, 16 by 16 (sdown): the item menu's stat drop. */
extern Uint8 Menu_SmallDownArrowTiles[];
/* The small up arrow, then the up and down arrows, 16 by 16 each; the item
   menu's stat rise loads the first (mnLoadStatArrows). */
extern Uint8 Menu_ArrowTiles[];
/* The Djinn menu's up arrow, 16 by 16 (graphics/ui/djinn_arrow_up). */
extern Uint8 Menu_DjinnArrowUpTiles[];
/* The Djinn menu's down arrow, 16 by 16 (graphics/ui/djinn_arrow_down). */
extern Uint8 Menu_DjinnArrowDownTiles[];
/* The item menu's quantity picture (quantity.4bpp). */
extern const Uint8 Menu_QuantityTiles[];
/* The Djinn menu's level label: `Lv`; `Nv.` in Spanish, `Niv` in French. */
extern const char DjinnMenu_TextLevel[];
/* `/`, between the Djinn menu's numbers. */
extern const char DjinnMenu_TextSlash[];
/* Heap slot 6: the actor system's state (ACTOR_SYSTEM_STATE). */
#define ActorSystemStateWork \
	(*(struct ACTOR_SYSTEM_STATE **)&HeapSlots[HEAP_SLOT_ACTOR_SYSTEM])
/* Heap slot 18: the selection screen while it runs. */
#define SelectionMenuStateWork \
	(*(struct SELECTION_MENU_STATE **)&HeapSlots[HEAP_SLOT_SELECTION_MENU])
/* Nonzero offers Battle on the File Screen, when a file has any of game flags
   48 to 127 set. */
extern Sint16 TitleBattleOptionEnabled;
/* The Japanese item menu's help: `L+A:`, before the arrange help. */
extern Uint8  ItemMenu_ArrangeKeysString[];
extern Sint32 ItemMenu_CommandColumnXTable[];
/* The Japanese item menu's help: `R:`. */
extern Uint8 ItemMenu_EquipmentKeyString[];
/* One 4-bit tile (backdrop.4bpp), a down arrow: the Djinn screens draw it
   between a thing and what a change makes of it (a Djinni set and on standby, a
   class and the next), and the Djinn preview blinks it, drawing it and a fill
   of the window background (TEXT_CANVAS_BACKGROUND_WORD) in turn every four
   frames. */
extern const Uint8 Menu_ChangeArrowTile[];
/* The colon the debug menus' scene and flag rows draw (window/screens_debug.c):
   `:`. */
extern const Uint8 Menu_ColonString[];
/* The selection screen's left arrow, 8 by 16 at 256 colours
   (graphics/ui/sprite_cursors/left). */
extern Uint8 Menu_CursorLeftActorTiles[];
/* `0123456789ABCDEF`, drawn by a debug window. */
extern const Uint8 Menu_HexDigitsString[];
/* The HP label: `HP`; `KP` in German, `PV` in Spanish and French, `PS` in
   Italian. */
extern const Uint8 Menu_HpString[];
/* The Status screen's level label: `Lv`; `Nv.` in Spanish, `Niv` in French. */
extern const Uint8 Menu_LvString[];
/* The four Djinn sprites, one per element from SPRITE_VENUS_DJINNI to
   SPRITE_JUPITER_DJINNI, that mnFileListShowDjinnSprs shows in a row. */
extern const Sint32 FileList_DjinnSpriteIds[4];
/* `.`, the decimal point of an item's stat multiplier (`x1.2`). */
extern Uint8 Menu_PointString[];
/* The PP label: `PP`; `EP` in Japanese. */
extern const Uint8 Menu_PpString[];
/* The File Screen's action by icon position, a row of 6 for each layout:
   0 New Game, 1 Continue, 2 Copy, 3 Erase, 4 Battle, 5 Send. */
extern Sint8 FileList_ActionByPosition[];
/* Frames mnIconMenuAnimateTo waits at each icon on its way, by icons still to
   go. */
extern Uint8 Menu_SelectionStepDelays[];
/* `L:`, the Japanese Psynergy menu's first shortcut label. */
extern const Uint8 Menu_ShortcutLColonString[];
/* `L`, the first shortcut's label beside an ability name wider than 11
   (Japanese only). */
extern const Uint8 Menu_ShortcutLString[];
/* `R:`, the second shortcut's label (Japanese only). */
extern const Uint8 Menu_ShortcutRColonString[];
/* `R`, the second shortcut's label beside an ability name wider than 11
   (Japanese only). */
extern const Uint8 Menu_ShortcutRString[];
/* `/`, between a current and a maximum. */
extern const Uint8 Menu_SlashString[];
/* `x`, before an item's stat multiplier (not in Japanese). */
extern Uint8 Menu_TimesString[];
extern Sint8 SettingsMenu_SleepIcons[];
extern Sint8 SettingsMenu_SpeechIcons[];
extern Sint8 SettingsMenu_SpeedIcons[];
/* The status panel's level label: `Lv`; `Nv.` in Spanish, `Niv` in French. */
extern Uint8 StatusPanel_LevelString[];
extern Uint8 MenuCursor_BobX[];
extern Uint8 MenuCursor_BobY[];
/* A debug window's title: `ENERGY`. */
extern Uint8 DebugMenu_EnergyTitle[];
/* A debug window's title: `ITEM`. */
extern Uint8 DebugMenu_ItemTitle[];
/* A debug window's label: `PAGE:`. */
extern Uint8 DebugMenu_PageLabel[];
/* `-`, between the first and last entry numbers of a debug window's page (32 a
   page). */
extern Uint8 DebugMenu_RangeDash[];
/* A debug window's title: `STATUS`. */
extern Uint8 DebugMenu_StatusTitle[];
/* The settings menu's slider colours (graphics/ui/slider_palette). */
extern const Uint16 SettingsMenu_SliderPalette[];
/* The settings menu's slider, 32 by 8 (slider.4bpp32x8). */
extern const Uint8 SettingsMenu_SliderTiles[];
/* The title's imports of mnTitleShowCamelotLogoStub and mnCreditsRunStub,
   called with an argument the functions do not take. */
void syImportTitleShowCamelotLogo(Sint32 mode);
void syImportCreditsRun(Sint32 mode);

struct UNIT;
struct DJINN_LIST_TABLE;
struct ITEM_MENU_STATE;
struct PAGED_LIST;
struct PSYNERGY_ENTRY;
struct WINDOW;
struct WINDOW_SPR;

/* The menu module's functions in the order of its stub table
   (src/menu/stubs.s). */
Sint32 mnItemMenuRun(void);
Sint32 mnPsynergyMenuRun(void);
Sint32 mnStatScreenRun(void);
Sint32 mnPartyMemberChooserOpen(void);
Sint32 mnIconCreateHandCursor(struct WINDOW *window, Sint32 x, Sint32 y);
void   mnCreatePartyFigures(void *source, Sint32 originX, Sint32 originY,
	  Sint32 spacing, Sint32 unused);
void   mnReleasePartyFigures(void);
void   mnIconShow(struct WINDOW_SPR *object);
Sint32 mnDjinnMenuRun(void);
void   mnItemMenuDrawItemDetails(struct WINDOW *window, Sint32 inventoryEntry);

/* The item menu's sort orders (Menu_ListOrder*): one byte per group, an item
   type, with LIST_ORDER_EQUIPPED for only the equipped items of that type,
   ending with LIST_ORDER_END. */
#define LIST_ORDER_TYPE_MASK 0x7f
#define LIST_ORDER_EQUIPPED	 0x80
#define LIST_ORDER_END		 255

/* The item menu's four sort orders (mnItemMenuSort), which L + A cycles through
   in this order. */
typedef enum LIST_SORT {
	/* The equipped items, the items that are not equipment, then the other
	   equipment (Menu_ListOrderEquippedThenItems). */
	LIST_SORT_EQUIPPED_THEN_ITEMS = 0,
	/* The equipped items, the other equipment, then the rest
	   (Menu_ListOrderEquippedThenEquipment). */
	LIST_SORT_EQUIPPED_THEN_EQUIPMENT = 1,
	LIST_SORT_EQUIPMENT_FIRST = 2, /* Menu_ListOrderEquipmentFirst */
	/* The items that are not equipment first (Menu_ListOrderItemsFirst). */
	LIST_SORT_ITEMS_FIRST = 3,
	LIST_SORT_COUNT = 4
} LIST_SORT;

/* What the menus share (menu_shared.c): their state, windows, status panel,
   hand cursor, party figures and entry icons, the paged lists and their
   highlights, and the watch for Start. */
Sint32 mnResumeFieldEvents(void);
Sint32 mnSuspendFieldEvents(void);
void   mnInitState(Sint32 unused);
Sint32 mnWindowUpdateOrCreate(struct WINDOW **handle, Sint32 x, Sint32 y,
	Sint32 w, Sint32 h, Sint32 flags);
void   mnWindowCloseIfOpen(struct WINDOW **handle, Sint32 instant);
void   mnStatPanelDraw(
	  struct WINDOW *unused, Sint32 member, Sint32 slot, Sint32 mode);
void mnTextDrawNumRightAligned(
	Sint32 number, struct WINDOW *window, Sint32 x, Sint32 y);
void mnStatPanelDrawHpPp(struct UNIT *unit, struct WINDOW *window);
void mnTextDrawStatComparison(
	const struct UNIT *after, const struct UNIT *before, struct WINDOW *window);

Sint32 mnIconCreateHandCursorUnusedArgument(
	struct WINDOW *window, Sint32 unused, Sint32 x, Sint32 y);
void mnReloadMemberPortrait(const void *unused, Sint32 unit);

struct WINDOW *mnCursorCreate(void *source);
void		   mnPlacePartyFigures(void);
void		   mnCursorPos(Sint32 xOffset, Sint32 yOffset);
void		   mnCursorSlide(Sint32 x, Sint32 y);
void		   mnCursorNoOperation(void);

void mnPsynergyMenuPlaceEntryIconsDefault(void);
void mnPsynergyMenuPlaceEntryIcons(
	Sint32 originX, Sint32 originY, Sint32 columns);
void   mnPsynergyMenuPlaceEntryIcon(struct WINDOW_SPR **slot, Sint32 index,
	  Sint32 originX, Sint32 originY, Sint32 columns);
void   mnItemMenuPlaceIcon(struct WINDOW_SPR **slot, Sint32 slotIndex,
	  Sint32 originX, Sint32 originY, Sint32 columns);
void   mnItemMenuPlaceIcons(Sint32 layout);
void   mnItemMenuShowMessage(Sint32 message, Sint32 x, Sint32 y);
Sint32 mnItemMenuSort(Uint16 *inventory, Sint32 mode);
void   mnCopyListOrder(Sint32 mode, Uint8 *order);

Sint32 mnPagedListHandleInput(Sint32 horizontal, Sint32 count, Sint32 pageSize,
	Sint32 *cursor, Sint32 *page);
void   mnPaletteLightenHighlight(Sint32 paletteNumber);
void   mnPagedListDrawPageNumbers(struct WINDOW *window, Sint32 itemCount,
	  Sint32 pageSize, Sint32 selectedPage, Sint32 rightEdge);
void   mnWindowSetHighlightRect(const struct WINDOW *window, Sint32 x, Sint32 y,
	  Sint32 width, Sint32 height, Uint32 palette);
void   mnPaletteLoadIconsIntoHighlight(void);
void   mnSetPageIcons(
	  Sint32 pageSize, Sint32 first, struct WINDOW *window, Sint32 x, Sint32 y);
void mnDrawCoins(struct WINDOW *window);

void mnWindowSetBounds(
	struct WINDOW *window, Sint32 x, Sint32 y, Sint32 width, Sint32 height);
void   mnWindowHoldTilemapUpload(void);
void   mnWindowReleaseTilemapUpload(void);
Sint32 mnSndPlayReturnOne(Sint32 sound);
void   mnWatchStart(void);
void   mnStartWatchingStart(void);
void   mnStopWatchingStart(void);
void   mnWindowPaletteSetNormalTextCol(void);
void   mnWindowPaletteSetNoHpTextCol(void);
void   mnWindowPaletteSetLowHpTextCol(void);

/* The rows a page of the Psynergy and item lists holds. */
#define LIST_PAGE_SIZE 5

/* The y of the menus' row of party portraits, and of the chosen member's,
   raised above the rest. */
#define MENU_MEMBER_Y				 30
#define MENU_MEMBER_RAISED_Y		 26
/* The cursor's x beside the party row's portrait `member`: the portraits stand
   24 pixels apart. */
#define MENU_MEMBER_CURSOR_X(member) ((member) * 24 - 10)

/* The Item menu (menu_item.c): its member and command choices, the item list
   and its pages, the details, giving, dropping and equipping. */
Sint32 mnItemMenuRunCommands(
	Sint32 *memberOut, Sint32 *targetOut, Sint32 *itemOut);
Sint32 mnItemMenuUse(void);
void   mnItemMenuInit(Sint32 x, Sint32 y, Sint32 mode, Sint32 columns);
void   mnSpawnIconEntries(struct ITEM_MENU_STATE *menu, struct WINDOW *window);
void   mnItemMenuHideAllIcons(void);
void   mnItemMenuHidePageIcons(void);
void   mnItemMenuClose(void);
Sint32 mnItemMenuSelectMember(Sint32 side);
Sint32 mnItemMenuRunMemberSelection(Uint16 *memberIds, Uint16 *items);
void   mnItemMenuShowEquipment(Sint32 memberId);
Sint32 mnItemMenuSelectTarget(Sint32 mode);
void   mnEquipmentMenuUpdateCompatibilityIndicators(void);
void   mnEquipmentMenuStopCompatibilityIndicators(void);
Bool   mnItemMenuIsStatRaisingItem(Sint32 itemId);
void   mnItemMenuDrawMessage(Sint32 unused, Sint32 message);
void   mnHideEmptyEntryIcons(const Uint16 *items);
Sint32 mnItemMenuCountItems(Sint32 memberId);
Sint32 mnItemMenuGetItemQuantity(Sint32 memberId, Sint32 itemId);
Sint32 mnItemMenuCollectItems(struct UNIT *unit, Uint16 *items, Sint32 mode);
void   mnItemMenuDrawIcons(Uint16 *inventoryEntries, Sint32 iconStyle);
void   mnItemMenuRefreshMember(Sint32 memberId, Sint32 mode);
void   mnItemMenuNoOperation(void);
void   mnItemMenuDrawEquipPreview(
	  Sint32 member, Sint32 slot, Sint32 mode, Sint32 target);
Sint32 mnInventoryRemoveFirstUnflagged(Sint32 memberId);
Sint32 mnItemMenuCommandCursorX(Sint32 column, Sint32 row);
Sint32 mnItemMenuCommandCursorY(Sint32 column, Sint32 row);
Sint32 mnItemMenuChooseCommand(void);
void   mnItemMenuBuildCommands(Sint8 *commandStates);
void mnItemMenuDrawCommands(const Sint8 *commandStates, struct WINDOW *window);
Sint32 mnItemClassifyUseMode(Sint32 memberId, Sint32 inventoryEntry);
void   mnItemMenuTryBreak(void);
Sint32 mnItemMenuOpenDetail(Sint32 itemIndex);
Sint32 mnItemMenuShowDetails(Sint32 value);
void mnItemMenuDrawStat(Sint32 statChange, Sint32 unused, struct WINDOW *window,
	Sint32 x, Sint32 y);
void mnItemMenuPlaceItemWindowBelowItemQuestion(void);
void mnItemMenuPlaceItemWindowBelowQuestion(void);
void mnItemMenuFitMessageWindowToItemQuestion(void);
void mnItemMenuFitMessageWindowToLongItemQuestion(void);
void mnItemMenuFitMessageWindowToItemChoices(void);
void mnItemMenuFitMessageWindowToQuestion(void);
Sint32 mnItemMenuSelectGiveQuantity(
	Sint32 baseQuantity, Sint32 quantityRange, Sint32 dropMode);
void   mnItemMenuDrawItemHead(void);
Sint32 mnItemMenuConfirmDrop(Sint32 item);
Sint32 mnItemMenuConfirmEquip(Sint32 unused);
void   mnLoadStatArrows(void);
Sint32 mnItemMenuBuildPagedList(struct PAGED_LIST *result, Sint32 index);
Sint32 mnItemMenuShowSelectedRow(
	struct WINDOW *window, void *memberState, struct PAGED_LIST *pageState);
Sint32 mnItemMenuDrawListPage(
	struct WINDOW *window, Sint32 unused, const struct PAGED_LIST *pageState);
Sint32 mnItemMenuItemNamePageReturnTrue(void);
void   mnItemMenuItemNamePageNoOperation(void);
Sint32 mnItemMenuRunList(Sint32 sideIndex);

/* The Psynergy menu (menu_psynergy.c): its member and target choices, the
   shortcuts, and the Psynergy list and its pages. */
Sint32 mnPsynergyMenuRunSteps(
	Sint32 *outMember, Sint32 *unused, Sint32 *outAction);
Sint32 mnPsynergyMenuClassifySelectedPsynergy(void);
Sint32 mnPsynergyMenuSelectPartySlot(Sint32 partySlot);
Sint32 mnPsynergyMenuSelectMember(Uint16 *memberIds, Uint16 *unused);
void   mnPsynergyMenuRefreshMemberPsynergy(Sint32 memberId);
Sint32 mnPsynergyMenuReturnTrue(void);
void   mnPsynergyMenuNoOperation(void);
Sint32 mnPsynergyMenuSelectTarget(Sint32 targetMode);
Sint32 mnPsynergyMenuSetShortcut(
	Sint32 memberId, Sint32 abilityId, Sint32 shortcutIndex);
Sint32 mnPsynergyMenuDrawShortcuts(struct WINDOW *window, Sint32 member);
void   mnPsynergyMenuCreateEntryGrid(void);
void   mnPsynergyMenuCloseWindows(void);
void   mnPsynergyMenuDrawPsynergyIcons(Uint16 *psynergies);
Sint32 mnPsynergyMenuCollectActions(
	struct UNIT *unit, Uint16 *actions, Sint32 mode);
Sint32 mnPsynergyMenuBuildPagedList(struct PAGED_LIST *result, Sint32 index);
Sint32 mnPsynergyMenuShowSelectedRow(
	struct WINDOW *window, Sint32 *memberState, struct PAGED_LIST *state);
Sint32 mnPsynergyMenuDrawPage(
	struct WINDOW *window, Sint32 unused, const struct PAGED_LIST *pageState);
Sint32 mnPsynergyMenuRunList(Sint32 sideIndex);
Bool   mnPsynergyMenuIsActionRestricted(Sint32 actionEntry);

/* The party member chooser and the Status menu (menu_status.c): the party row,
   the status page, the Psynergy page and the equipment page. */
Sint32 mnPartyMemberChooserRun(void);
Sint32 mnStatScreenRunPages(void);
Sint32 mnStatScreenSelectMember(Sint32 slot);
Sint32 mnStatScreenShowDjinnList(void);
Sint32 mnStatScreenRunPartyRow(void);
Sint32 mnPartyMemberChooserChooseMember(void);
Bool   mnStatScreenMovePartyMember(Sint32 selectedIndex, Sint32 direction);
void   mnStatScreenInit(Sint32 x, Sint32 y, Sint32 mode, Sint32 columns);
void   mnStatScreenShowMemberStat(Sint32 member, Sint32 frame);
Sint32 mnStatScreenRunStatPage(void);
void   mnStatScreenDrawSelectionCursor(
	  Sint32 side, Sint32 selected, Uint8 *entries, Sint32 invert);
void mnStatScreenDrawSelectionLabels(
	struct WINDOW *target, Sint32 selected, const Uint8 *entries);
void mnStatScreenShowMemberProgressMessage(
	struct WINDOW *destination, Sint32 row, Sint32 hasDjinn);
void mnStatScreenDrawStatPage(
	struct WINDOW *window, Sint32 memberId, Sint32 mode);
void mnBusyWait(void);
void mnStatScreenDrawTopRows(
	struct WINDOW *window, Sint32 memberId, Sint32 flags);
Sint32 mnStatScreenListStatuses(
	Uint8 *ailments, Sint32 requested, Sint32 memberId);
Sint32 mnPagedListBuild(struct PAGED_LIST *result, Sint32 index);

void mnBuildPatternTiles(void);
void mnPsynergyMenuDrawRange(
	struct WINDOW *window, Sint32 x, Sint32 y, Sint32 range, Sint32 unused);
Sint32 mnStatScreenShowPsynergyRow(
	struct WINDOW *window, Sint32 unused, struct PAGED_LIST *state);
Sint32 mnStatScreenDrawPsynergyPage(
	struct WINDOW *window, Sint32 unused, const struct PAGED_LIST *res);
Sint32 mnStatScreenRunPsynergyPage(void);
Sint32 mnStatScreenPsynergyPageReturnTrue(void);
void   mnPsynergyMenuDrawPreparedPsynergyIcons(
	  const void *unused, Sint32 memberId);
void   mnPsynergyMenuPreparedIconsNoOperation(void);
void   mnPsynergyMenuPreparedIconsSecondNoOperation(void);
Sint32 mnStatScreenShowEquipmentRow(
	struct WINDOW *window, Sint32 unused, struct PAGED_LIST *state);
Sint32 mnStatScreenDrawEquipmentPage(
	struct WINDOW *window, Sint32 unused, struct PAGED_LIST *state);
Sint32 mnStatScreenRunEquipmentPage(void);
Sint32 mnMoveGridCursor(Sint32 *columnOut, Sint32 *rowOut, Sint32 direction);
Sint32 mnGridCursorReturnTrue(void);

void mnItemMenuDrawEquipment(
	struct WINDOW *window, Sint32 memberId, Sint32 mode);
void   mnItemMenuDrawEquippedItemNames(struct WINDOW *window, Uint16 *items);
void   mnPlaceEntryIconsInGrid(Sint32 originX, Sint32 originY, Sint32 columns);
void   mnPlaceEntryIconInGrid(struct WINDOW_SPR *icon, Sint32 index,
	  Sint32 originX, Sint32 originY, Sint32 columns);
void   mnItemMenuPlaceEquippedIcons(Uint16 *items);
void   mnHideEntryIcons(void);
Sint32 mnStatScreenCreateStatIcons(struct WINDOW *window);
void   mnStatScreenShowStatIcons(const Uint8 *flags);
void   mnStatScreenHideStatIcons(void);
Sint32 mnStatScreenLoadStatIcons(const Uint8 *enabled);

/* Using an item or Psynergy from the menus (menu_use.c). */
void   mnItemPrepareUsePalette(void);
void   mnItemUseNoOperation(void);
Sint32 mnItemUse(Sint32 slot, Sint32 memberId, Sint32 targetId);
Sint32 mnItemUseReturnTrue(void);
Sint32 mnAbilityApplyToTargets(
	Sint32 effectId, Sint32 sourceId, Sint32 targetId, Sint32 fixedScale);
void   mnItemPlayUseSnd(Uint32 item);
void   mnAbilityPlayUseSnd(Sint32 action);
Sint32 mnAbilityReturnFalse(void);

/* The Djinn menu (menu_djinn.c): its screen, the Djinn lists and their help. */
Sint32 mnWrapIndex(Sint32 value, Sint32 modulus);
Sint32 mnDjinnMenuPlacePartyFigures(Sint32 yOffset);
Sint32 mnDjinnMenuRunScreen(void);
void   mnDjinnMenuAdjustPalettes(Sint32 brightnessDelta);
#if FIX_FUNCTION_SIGNATURES
typedef void MENU_BACKDROP_RESULT;
#else
typedef Sint32 MENU_BACKDROP_RESULT;
#endif
/* UB: mnOpenBackdropScreen returns the result of mnDjinnMenuDrawElementList,
   called with no prototype though it returns nothing; nothing reads it. */
MENU_BACKDROP_RESULT mnOpenBackdropScreen(void);
Sint32				 mnPsynergyListDiff(struct PSYNERGY_ENTRY *before,
				  struct PSYNERGY_ENTRY *after, Uint16 *actionsOut, Sint32 *gainedCountOut,
				  Sint32 *lostCountOut);
Sint32				 mnDjinnMenuBuildLists(void *table);
#if FIX_FUNCTION_SIGNATURES
void mnDjinnMenuDrawElementList(struct DJINN_LIST_TABLE *table);
#endif
Sint32 mnWindowSetInteriorPaletteBitRect(const struct WINDOW *window, Sint32 x,
	Sint32 y, Sint32 width, Sint32 height, Sint32 palette);
void   mnWindowSetRectPalette(
	  Sint32 x, Sint32 y, Sint32 width, Sint32 height, Sint32 palette);
void   mnWindowSetInteriorRectPalette(void *window, Sint32 x, Sint32 y,
	  Sint32 width, Sint32 height, Sint32 palette);
Sint32 mnDjinnMenuShowHelp(void);
Sint32 mnDjinnMenuSelectDjinn(Sint32 mode);
Sint32 mnDjinnListUnitEntries(
	Uint16 *entriesOut, Sint32 memberId, Sint32 element);
Sint32 mnDjinnMenuDrawPairPreview(Sint32 mode, Sint32 pageIndex);
Sint32 mnDjinnMenuDrawStatPreview(struct WINDOW *window, Sint32 x, Sint32 y,
	Sint32 memberId, Sint32 removePairSlot, Sint32 addPairSlot,
	Sint32 previewMode, Sint32 page, Sint32 side);

/* The rows of Djinn figures over the Djinn and Status menus
   (menu_djinn_figures.c). */
void   mnDjinnFiguresInitTopRow(void *window, Sint32 unused);
void   mnDjinnFiguresUpdateAllPoss(void);
void   mnDjinnFiguresUpdateBottomRow(void);
void   mnDjinnFiguresInitBottomRow(Sint32 window, Sint32 unused);
Sint32 mnDjinnFiguresSetSlotPos(
	Sint32 index, Sint32 x, Sint32 y, Sint32 negative);
Sint32 mnDjinnFiguresSetSlotPhase(Sint32 index, FIXED phase);
Sint32 mnDjinnFiguresReplaceSlot(
	Sint32 index, Sint32 element, Sint32 animation);
void mnStartPartyFiguresStanding(void);
void mnDjinnFiguresReleaseTopRow(void);
void mnDjinnFiguresReleaseBottomRow(void);

/* The Djinn menu's preview of a change before it is made, and the checks that
   keep the party members' Djinn counts even (menu_djinn_preview.c). */
Sint32 mnDjinnMenuRunPreview(Uint32 mode);
Sint32 mnDjinnMenuShowMemberStat(void);
Sint32 mnDjinnMarkTradeOnlyRecipients(
	Sint8 *requiresTrade, Sint32 sourceMemberColumn);
Sint32 mnDjinnCheckGiveBalance(
	Sint32 sourceMemberColumn, Sint32 targetMemberColumn);
void mnDjinnCountOwned(Uint8 *ownedCounts);

/* The menus' small icons: the arrows and the Psynergy icons (menu_icons.c). */
void   mnLoadDjinnArrows(void);
void   mnIconMenuReleaseArrows(void);
Sint32 mnIconLoadResourceIntoSlot(Sint32 abilityId, Sint32 slot);
Sint32 mnIconCreatePsynergyIcon(
	struct WINDOW *window, Sint32 x, Sint32 y, Sint32 abilityId);
Sint32 mnIconCreateStatChangeArrow(
	struct WINDOW *window, Sint32 x, Sint32 y, Sint32 variant);
Sint32 mnIconCreateDjinnArrow(
	struct WINDOW *window, Sint32 x, Sint32 y, Sint32 variant);

/* The Japanese status panel's PP row, "EP    /": label and slash in one
   string, drawn by the unit summary in menu/menu_shared.c. */
extern const Uint8 StatusPanel_PpRowString[];
/* The Japanese status panel's HP row, "HP    /", the slash already in its
   cell at 48. */
extern const Uint8 StatusPanel_HpRowString[];
/* The HP label outside Japan, in each language's form ("KP" in German, "PV" in
   Spanish and French, "PS" in Italian). */
extern const Uint8 StatusPanel_HpString[];
/* The PP label outside Japan, which every language writes "PP". */
extern const Uint8 StatusPanel_PpString[];
/* The slash between a current and maximum figure on the status panel, outside
   Japan. */
extern const Uint8 StatusPanel_SlashString[];

#endif
