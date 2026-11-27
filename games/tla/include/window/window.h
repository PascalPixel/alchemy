#ifndef ALCHEMY_WINDOW_H
#define ALCHEMY_WINDOW_H

/* @file */
/* Windows, the messages that type into them, and the window system's state, */
/* as window.c declares them for the files that use them. */

#include "types.h"
#include "system.h"
#include "window/window_tile.h"
#include "window/message_argument.h"
#include "window/message_speed.h"
#include "edition.h"
#include "tla_edition.h"
#include "graphics/oam.h"
#include "graphics/render_input.h"
#include "graphics/window_close.h"
#include "static_assert.h"

struct BATTLE_ACTION;
struct MENU_SPR;
struct WINDOW;
struct WINDOW_SPR;

/* The window module's functions in the order of its stub table. */
/* Sets up the window system for the field (and the other screens outside */
/* battle): allocates and clears its state, blanks the tilemap, frees */
/* every window sprite and starts the two per-frame tasks, the tilemap upload */
/* and the window update. */
void mnWindowSystemInitField(void);
/* Sets up the window system for battle, as mnWindowSystemInitField does for */
/* the field, starting the field's window update when loadGlyphTiles is set */
/* (mnWindowSystemStartBattleUpdate). */
void mnWindowSystemInitBattle(s32 loadGlyphTiles);
/* Opens a window at tile (x, y), width by height tiles, in the first free */
/* slot, with the WINDOW_FLAG_ flags in style; NULL when every slot is taken. */
WINDOW *mnWindowCreate(s32 x, s32 y, s32 width, s32 height, s32 attrs);
/* Closes a window: with instant 0 it zooms shut over WINDOW_CLOSE_FRAMES; */
/* otherwise it is erased at once and freed. */
void mnWindowClose(WINDOW *window, s32 instant);
/* Decodes message into text (a u16 string) with no length limit: text must */
/* have room for the whole message and its terminator. */
void mnMessageDecodeRawUnbounded(s32 message, u16 *text);
/* Decodes message into text, at most capacity codes and a terminator, */
/* keeping style codes with their operands. */
void mnMessageDecodeRaw(s32 message, u16 *text, s32 capacity);
/* Decodes key into destination, at most capacity halfwords with the */
/* terminator, and returns its length. */
s32 mnMessageCopyExpanded(s32 message, u16 *destination, u32 capacity);
/* Opens a window sized to a message near tile (x, y) and starts typing the */
/* message into it. packed holds the window options in its low bits */
/* (MESSAGE_WINDOW_OPTION) and the speaker's voice sound in bits 16 to 27 */
/* (MESSAGE_WINDOW_VOICE). */
WINDOW *mnMessageOpenWindow(s32 message, s32 x, s32 y, u32 packed);
/* Shows a message in a window centred on the screen, waits until it is typed */
/* out and closes it, as its flags say. */
void mnFieldMessageShow(s32 message, s32 flags);
/* Returns 1 once every message's window has its text typed out, or 0 while */
/* one is still typing (its state 0). */
s32 mnMessageAllTyped(void);
/* Returns 1 when a window is fully closed (its slot free), and 0 while it is */
/* open or closing. */
s32 mnWindowIsClosed(WINDOW *window);
/* Sets the decode flags the next message is decoded with */
/* (mnMessageDecode): MESSAGE_DECODE_HALTING_SPEECH makes it halting */
/* speech, a short pause before each character, broken off with an ellipsis */
/* after the first word; MESSAGE_DECODE_SPACING_AFTER puts a wide gap */
/* (ENTRY_WIDE_GAP) after certain characters in English and Japanese. Flags */
/* already set stay set: only mnMessageOpenWindow clears both, once it has */
/* started a message typing, so they also apply to every message decoded before
 */
/* that. */
void mnMessageSetDecodeFlags(u32 flags);
/* Clears a window's contents: redraws it empty and releases the */
/* window sprites it carries. A text canvas window is left alone. */
void mnWindowSprPrepareForRedraw(WINDOW *window);
/* Clears an area of a window's interior to the window fill. left, top, right */
/* and bottom are pixels inside the window, and the area covers every tile they
 */
/* touch. */
void mnWindowClearInteriorTiles(const WINDOW *window, u32 left, u32 top,
								u32 right, u32 bottom);
/* Draws a divider line from tile (x1, y1) to (x2, y2), joining the frame and */
/* the dividers already drawn. */
void mnWindowDrawDividerLine(WINDOW *win, u32 x1, u32 y1, u32 x2, u32 y2);
/* Decodes a message and draws it into a text canvas window at pixel (x, y), */
/* all at once. */
void mnTextDrawMessage(s32 message, s32 window, s32 x, s32 y);
/* Decodes a message and draws it as tiles in a window at pixel (offsetX, */
/* offsetY) of its interior. offsetY is rounded down to a tile row; offsetX */
/* picks the tile, and its remainder is where the text starts in it. */
void mnTextDrawMessageAsTiles(s32 message, WINDOW *window, u32 offsetX,
							  u32 offsetY);
/* Draws a byte string into a text canvas window at pixel (x, y). */
void mnTextDrawString(u8 *text, s32 window, s32 x, s32 y);
/* Draws a byte string as tiles in window at pixel (offsetX, offsetY) of its */
/* interior. */
void mnTextDrawStringAtOffset(u8 *text, WINDOW *window, s32 offsetX,
							  s32 offsetY);
/* Draws a byte string into a window's tilemap at pixel (x, y) of its */
/* interior, rounded down to tiles. */
void mnTextDrawStringInWindow(u8 *text, s32 window, u32 x, u32 y);
/* Draws a number (width as mnStringFormatNum) into a text canvas window at */
/* pixel (x, y). */
void mnTextDrawNum(s32 value, s32 width, s32 window, s32 x, s32 y);
/* Draws a number (width as mnStringFormatNum) as tiles in a window at pixel */
/* (x, y) of its interior. */
void mnTextDrawNumAtOffset(s32 value, s32 width, WINDOW *window, s32 x, s32 y);
/* Draws a number (width as mnStringFormatNum takes it) into a window's */
/* tilemap at pixel (x, y) of its interior. */
void mnTextDrawNumInWindow(s32 value, s32 width, s32 window, s32 x, s32 y);
/* Sets the colour (TEXT_COL) the next text is drawn in. */
void mnTextSetCol(s32 color);
/* Sets the text style (TXT_STYLE) of drawn text: outlined, or a motion */
/* of the glyph sprites. Reached only through a bare entry of the window */
/* stub table. */
void mnTextSetStyle(s32 style);
/* Creates a window sprite for the VRAM cache slot `slot` and attaches it to */
/* `window`, placed at (offsetX, offsetY) pixels from the window's inner corner
 */
/* (one tile in from its top-left border). `oamFlags` holds the OAM shape and */
/* size bits. Releases the slot and returns NULL when no sprite is free. */
WINDOW_SPR *mnWindowSprCreate(s32 slot, s32 oamFlags, WINDOW *window,
							  s32 offsetX, s32 offsetY);
/* Loads resource `resource` in loading mode `mode` into a VRAM slot and shows
 */
/* it as a 16x16 sprite at (x, y) in `window`. */
WINDOW_SPR *mnWindowSprCreateFromResource(s32 mode, s32 resource,
										  WINDOW *window, s32 x, s32 y);
/* Shows the 32x32 portrait of `character` at (x, y) in `window`, in portrait */
/* slot `side` (0 or 1, or any other value for the first free one; the second */
/* is tried first), in PORTRAIT_PALETTE + side. */
WINDOW_SPR *mnPortraitCreate(s32 character, s32 position, s32 side,
							 WINDOW *window, s32 x, s32 y);
/* Reloads the portrait picture for the key into a portrait slot with that */
/* slot's palette (PORTRAIT_PALETTE + slot), reusing the slot's VRAM cache */
/* entry. A slot of 2 or more means whichever slot already shows the picture, */
/* the second first; when neither does, nothing is loaded. */
void mnPortraitReload(u32 slot, s32 key, s32 unused);
/* Sets a window sprite's motion and starts it from its first step; does */
/* nothing for NULL. */
void mnWindowSprSetMotion(WINDOW_SPR *out, s32 mode);
/* Sets the depth of the window sprite `sprite` to ~priority, which is */
/* OAM_DEPTH_FRONT - `priority`: a higher priority draws further back. Does */
/* nothing when `sprite` is NULL. */
void mnWindowSprSetPriority(void *sprite, s32 priority);
/* Opens a portrait window at tile (x, y) showing the speaker's portrait. */
/* portraitOption is passed on through mnPortraitCreate to */
/* mnIconLoadWithPalette, which ignores it. */
WINDOW *mnPortraitOpenWindow(s32 speaker, s32 portraitOption, s32 x, s32 y);
/* Closes at once the portrait window showing speaker `id`'s portrait. */
void mnPortraitCloseWindow(s32 speaker);
/* Sizes a message's window by its first page. */
s32 mnTextGetMessageSize(s32 message, s32 *x, s32 *y, u32 *width, u32 *height);
/* Sizes a message's window to hold every page. */
s32 mnTextGetMessageSizeAllPages(s32 message, s32 *x, s32 *y, u32 *width,
								 u32 *height);
/* Empties the table of message arguments. */
void mnMessageArgumentClearAll(void);
/* Adds a message argument of kind (MESSAGE_ARGUMENT_) with value, dropped when
 */
/* all eight places are in use. */
void mnMessageArgumentPush(u32 value, u32 kind);
/* Opens the party status window, sized by mnPartyStatBuildLayoutBounds, and */
/* draws it, holding the tilemap upload meanwhile. */
void mnPartyStatCreateWindow(s32 flags);
/* Draws the party status window with `flags` (PARTY_STATUS_): each member's */
/* name, HP and PP, and in battle the standby Djinn. */
void mnPartyStatDraw(s32 flags);
/* Closes the party status window and frees its heap block. */
void mnPartyStatClose(void);
/* Closes each message's window whose text has ended, then waits ten frames. */
void mnMessageCloseFinishedWindows(void);
/* Closes every window: first the messages' windows, waiting until they have */
/* closed, then the windows opened directly, which it does not wait for. */
void mnWindowCloseAll(void);
/* Allocates and resets the selection screen: the row of icons the player picks
 */
/* from, with arrows at each end and a pointing hand over the chosen target, */
/* whose picture takes a VRAM cache entry. */
void mnSelectionMenuCreate(void);
/* Empties the selection screen's list of entries. */
void mnSelectionMenuClear(void);
/* Adds an entry to the selection screen's list, up to */
/* SELECTION_MENU_ENTRY_LIMIT. The kinds mnSelectionMenuSetupIcon draws (enum */
/* ICON_KIND) are ICON_KIND_COMMAND, ICON_KIND_YES_NO, ICON_KIND_ITEM and */
/* ICON_KIND_ABILITY; base is the command's, item's or ability's number. */
void mnSelectionMenuAddEntry(u32 kind, u32 base);
/* Starts drawing the selection screen each frame, in the vertical blank pass */
/* (TASK_PASS_FRAME). */
void mnSelectionMenuStartDrawing(void);
/* Stops drawing the selection screen. */
void mnSelectionMenuStopDrawing(void);
/* Opens or refits the name window above the selection row and writes the name
 */
/* of the icon under the cursor, or that nothing is known (ICON_KIND_ABILITY) */
/* or held (ICON_KIND_ITEM). ICON_KIND_YES_NO is a yes or no question, placed */
/* at the bottom or top of the screen. count is the name's width in tiles */
/* inside the frame; 0 keeps the open window as it is. The window stays open */
/* until mnSelectionMenuClose closes it. */
void mnSelectionMenuOpenNameWindow(s32 mode, u32 count);
/* Closes the selection screen and frees its icons, tiles and state. The */
/* chosen icons go too, so the choices are only in the indexes the loops */
/* returned. */
void mnSelectionMenuClose(void);
/* Sets up both end arrows of the selection row. */
void mnSetupSelectionBothSides(void);
/* Runs the selection row until the player chooses (the chosen entry's index) */
/* or, when mode allows it, cancels with B (-1). SELECTION_FIXED allows no */
/* moving or choosing; any mode but 0 allows cancelling. Reached only through */
/* a bare entry of the window stub table. */
s32 mnSelectionMenuRun(s32 mode);
/* As mnSelectionMenuRun, with the menu sounds. */
u32 mnSelectionMenuRunWithSounds(u32 mode);
/* Does nothing. */
void mnArrowNoOp2(void);
/* Does nothing. */
void mnArrowNoOp3(void);
/* Makes the icons of the listed entries from top, as many as the row's places,
 */
/* in a row along the bottom of the screen, and loads the large icon of the */
/* one under the cursor. */
void mnSelectionMenuBuildEntries(u32 unused);
/* Does nothing; reached only through the window stub table. */
void mnSelectionMenuReservedNoOp(void);
/* Collects the party's commands for a turn into `commands`, one for each of */
/* the `commandCount` units in `actors`, with two nested functions: the party */
/* panels' clear and the link wait's message (mnLinkDrawWaitingForOpponent). */
/* Unlike The Broken Seal's, each round first appends the forced actions of */
/* statused members (mnBattlePartySplitChoosers), whose count is added to the */
/* result; B on the party's row undoes a reserve swap; Switch swaps a reserve */
/* member in (mnBattlePartyChooseReserve after choosing the slot); Status lists
 */
/* the reserve as well; and the Djinn tutorial's hint follows an unleash. */
s32 mnBattleCollectPartyCommands(struct BATTLE_ACTION *commands, u16 *actors,
								 s32 commandCount);
/* Shows a message in the battle message window and waits until it has been */
/* typed out. */
void mnBattleMessageShowAndWait(s32 message);
/* Decodes message and starts it in the message window along the bottom of */
/* the screen (tiles 0, 15, 30 by 6), creating the window first if it is not */
/* open; a message that decodes to nothing shows nothing. */
void mnBattleMessageOpen(s32 message);
/* Does nothing. */
void mnArrowNoOp6(void);
/* Returns the portrait of a character (ids below */
/* PORTRAIT_FIRST_SPRITE_SPEAKER) or another speaker's sprite (from */
/* PORTRAIT_FIRST_SPRITE_PORTRAIT for those), or -1 when it has none. */
s32 mnPortraitGetForSpeaker(u32 value);
/* Runs the field menu until the player picks something that leaves it, and */
/* returns the chosen screen's result. Cancelling the menu returns an */
/* undefined value, the missing result of mnPartyStatCloseWithMenu (0 under */
/* UBFIX); its one caller, fdSequenceRun, ignores the result. Its case for */
/* command 0, which stores the trigger tile ahead in the field state's */
/* examineCell, is unreachable. */
s32 mnFieldMenuRun(void);
/* The File Screen's row of icons, from the save summaries: New Game, */
/* Continue, Copy and Erase for one or two files (starting on Continue); no New
 */
/* Game or Copy with three; with every file cleared no Continue, and only Erase
 */
/* with three such files; then Battle and Update when they are enabled. */
/* Returns the action chosen (FILE_ACTION) by its position; with Battle */
/* off, the action after Erase is Update's. With no files it returns */
/* SAVE_ACTION_NEW_GAME at once, and -1 when cancelled or the backup memory */
/* cannot be read. */
s32 mnFileListSelectAction(void);
/* Selects the transfer method or the password level (`mode`, enum */
/* SEND_OPTION_MENU), with a heading window above and an explanation of the */
/* selected icon below, redrawn by a task while the menu runs. Returns the */
/* place chosen, or -1 when cancelled. */
s32 mnIconMenuChooseSendOption(s32 mode);
/* Shows the area's name in a window near the top of the screen for */
/* AREA_NAME_WINDOW_FRAMES, kept in the field state until */
/* mnAreaNameWindowUpdate closes it. */
void mnAreaNameWindowShow(void);
/* Closes the area name at once, if it is shown. */
void mnAreaNameWindowHide(void);
/* Shows a message centred across the screen and in its top 15 rows (yOffset */
/* tiles lower) and waits until it is typed out, then closes it, in the window
 */
/* mode (CENTERED_MESSAGE_) gives. */
void mnFieldMessageShowCentered(s32 message, s32 mode, s32 yOffset);
/* Closes the battle message window at once, if one is open, and forgets it, */
/* so the next battle message (mnBattleMessageOpen) opens a new one. */
void mnBattleMessageClose(void);
/* Resets the pen and text style of the first idle message, clearing the canvas
 */
/* when the message still has a window. */
void mnMessageResetIdle(void);
/* Sets up a sprite as the right-pointing arrow, in a free VRAM cache entry */
/* returned in *slot, which mnArrowRelease frees. */
void mnArrowCreate(struct MENU_SPR *sprite, s32 *slot);
/* Places an arrow sprite at screen pixel (x, y) and queues it for drawing */
/* this frame, at depth OAM_DEPTH_FRONT - 3; the caller calls it every frame it
 */
/* shows. */
void mnArrowSetPos(struct MENU_SPR *sprite, s32 x, s32 y);
/* Frees an arrow sprite's tile slot. */
void mnArrowRelease(s32 slot);
/* Removes itemId from the party's inventory and returns 0. */
s32 mnItemRemoveFromInventory(s32 unused, s32 itemId);
/* Decodes item code's icon (the first icon for code 0) into the glyph buffer.
 */
void mnIconDrawItem(s32 code);
/* Draws a zero-terminated string of 16-bit character codes into a text canvas
 */
/* window at pixel (x, y), running its colour, outline, line spacing and new */
/* line codes; other control codes are skipped with their arguments. */
void mnTextDrawWideStringAtOffset(u16 *text, WINDOW *window, s32 x, s32 y);
/* Fills `spans`, a page of the screen transition's window edges past its WININ
 */
/* and WINOUT, with the windows' horizontal extent on each of the 160 scan */
/* lines: (left << 8) | right in the second halfword (WIN1H) of each line's */
/* pair, read from the UI tilemap by the ARM function */
/* mnScanlineWindowBuildSpans (window_spans.c), copied into RAM for the call. */
/* With no window state, every line gets an empty span. */
void mnScanlineWindowFillSpans(void *spans);
/* Redraws a window empty, its border and fill, at its current rectangle. */
void mnWindowSprRedrawSavedRect(WINDOW *window);
/* Releases every window sprite in a list, or in a window's list, and empties */
/* it. */
void mnWindowSprClearList(void *sprites);
/* Writes value into a window's tilemap at tile (x, y) of its interior, in the
 */
/* palette mode given (TILEMAP_ENTRY_). */
void mnWindowSetTilemapEntry(WINDOW *window, s32 value, s32 x, s32 y, u32 mode);
/* The debug destination menu: picks a scene and an entrance, starting from */
/* the scene the party came from, and sends the party there with A; returns */
/* -1 after setting the destination and -2 when B cancels. */
s16 mnDebugSceneSelectRun(void);
/* The flag browser: shows a page of flags and takes input until closed, */
/* starting on page 8. Returns 0. */
s32 mnDebugFlagEditorRun(void);
/* Loads the icon of an entry of kind mode into sprite tile slot result (a free
 */
/* slot when -1) and returns the slot, or -1 when none was free. */
s32 mnSelectionIconLoadIntoSlot(s32 mode, s32 value, s32 result, s32 option);
/* Marks the text tiles the tilemap shows, and frees those it no longer shows.
 */
void mnWindowMarkVisibleTextTiles(void);
/* Draws an item's icon into the glyph buffer from its inventory code with the
 */
/* layers asked for (ICON_ flags). */
s32 mnItemIconCompose(u32 code, u32 layers);
/* Highlights the party status columns of the units in `ids`, ended by */
/* BATTLE_UNIT_LIST_END. */
s32 mnPartyStatHighlightMembers(u16 *ids);
/* Builds item icon glyph, over the icon base when withBase is set, and loads */
/* it into *cacheSlot (a free slot unless reuse is set). Returns the tile in */
/* *tileOut. */
void mnIconBuildItemIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
							  s32 *tileOut, s32 reuse);
/* Builds a Psynergy icon, over the icon base when withBase is set, and loads */
/* it into *slot, or a free slot unless reuse is set. */
void mnIconBuildAbilityIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
								 s32 *tileOut, s32 reuse);
/* Copies icon `icon` (with `layers` passed on to the copier) into a free VRAM
 */
/* slot and shows it as a 16x16 sprite at (x, y) in `window`. */
WINDOW_SPR *mnWindowSprCreateLoaded(s32 icon, s32 layers, WINDOW *window, s32 x,
									s32 y);
/* Lets the player choose a file and saves the game to it, asking before it */
/* writes over a file in use. */
s32 mnSaveToChosenFile(void);
/* Selects a file and loads it. */
s32 mnSaveChooseAndLoadFile(s32 mode);
/* Selects a file and copies it to a free file. Its failures say "Failed to */
/* erase." (MsgEraseFailed), as erasing's do. */
s32 mnSaveCopyFile(void);
/* Selects a file and erases it. */
s32 mnSaveEraseSelectedFile(void);
/* Writes the link lobby's tallies (MainRoutineStateWork.linkTallies) into the
 */
/* current file's first part (SaveSlot), under the saving screen, leaving the */
/* rest of the part as it was saved. Returns -1 when no file is current. */
s32 mnSaveWriteLinkTallies(void);
/* Writes both halves of the save buffer, as they are, to save file `file`. */
/* Returns 0, or a SAVE_ERROR_* after showing its message. */
s32 mnSaveWriteFile(s32 file);
/* Loads the save files' summaries for the File Screen, sets the title's Battle
 */
/* and Update options from them, and returns how many files there are. */
s32 mnSaveScanFileSummaries(void);
/* The functions of other modules the tests open that no header declares yet, */
/* as their modules define them. */
s32 mnNameEntryEditMemberName(s32 member);
/* Loads the pointing hand's tiles into VRAM cache entry slot. */
/* A unit whose ROM code is compiled against another form defines */
/* OWN_PointingHand_LoadIntoSlot and declares that form itself. */
#ifndef OWN_PointingHand_LoadIntoSlot
void mnPointingHandLoadIntoSlot(s32 slot);
#endif
/* Loads the name entry caret's tiles into VRAM cache entry slot. */
/* A unit whose ROM code is compiled against another form defines */
/* OWN_NameCaret_LoadIntoSlot and declares that form itself. */
#ifndef OWN_NameCaret_LoadIntoSlot
void mnNameCaretLoadIntoSlot(s32 slot);
#endif
/* Moves the row of icons to start at pixel (x, y), 16 pixels apart. */
void mnSelectionMenuSetPos(u32 x, u32 y);
/* Browses pages of item, Psynergy and status-pattern icons, suspending the */
/* field's actor system while the browser owns its window and cache entries. */
/* L and R switch the kind, Left and Right turn its pages (item and Psynergy */
/* icons from page * ICON_BROWSER_PAGE_ICONS on; the status patterns are the */
/* first page's on every page, whatever the heading says), A or B closes it. */
/* Returns 0. The debug rooms open it (debug/level.c, debug/psynergy_test.c, */
/* debug/shop_test.c). */
s32 mnDebugBrowserIcons(void);
/* A debug browser of the portraits: left and right step through */
/* Portrait_CharacterMap and then Portrait_SpriteMap, L and R by */
/* DEBUG_LARGE_STEP, each drawn with its number and its name from */
/* MsgNamesMasterPoi; A or B leaves. */
s32 mnDebugBrowserPortraits(void);
/* Shows that a Djinni joined a party member while the join jingle */
/* (MUS_A_NEW_ALLY) plays, until it ends or A, B, L or R is pressed. */
void mnDjinnShowJoinedMessage(s32 member, s32 element, s32 djinn);
/* Shows that a party member joined the party while the join jingle */
/* (MUS_A_NEW_ALLY) plays, until it ends or A, B, L or R is pressed. */
void mnPartyShowJoinedMessage(s32 member);
/* Shows that two party members joined the party while the join jingle */
/* (MUS_A_NEW_ALLY) plays, until it ends or A, B, L or R is pressed. */
void mnPartyShowPairJoinedMessage(s32 first, s32 second);
/* Announces a summon the party can now use: "... can now summon ..." */
/* (MsgCanNowSummon) until the jingle ends or a button is pressed, then */
/* "To summon" (MsgToSummon) with its two largest Djinn counts, until a button
 */
/* is pressed. */
void mnSummonShowLearnedMessage(s32 summon);
/* Sets the window colours (WINDOW_SHADE) for a window colour (0 to */
/* WINDOW_COLOR_COUNT - 1, a hue around the colour wheel) and brightness (0 to
 */
/* WINDOW_BRIGHTNESS_COUNT - 1, WINDOW_BRIGHTNESS_NEUTRAL the plain one). */
void mnWindowColApply(s32 color, s32 brightness);
/* Runs the settings menu: 0 when A or START keeps the settings, -1 when B */
/* drops them. */
s32 mnSettingsMenuRun(void);
/* Handles Save your quest, Sleep and Change settings from the Pause menu. */
/* Saving is refused, with its message, while FLAG_CANNOT_SAVE_HERE or */
/* FLAG_NO_PLACE_TO_SAVE is set. A refused or cancelled save or a cancelled */
/* settings menu reopens the Pause menu on the same entry; cancelling the Pause
 */
/* menu returns -1. Sleep shows its message and sets SleepRequested. Otherwise
 */
/* returns 0. */
s32 mnPauseMenuRun(void);
/* Fills the top-left `width` x `height` tiles of WINDOW_TILEMAP_VRAM, a */
/* BG_SCREEN_WIDTH-tile map, with the tilemap entry `value`. */
void mnBgScrFillRect(s32 unusedA, s32 unusedB, u32 width, u32 height,
					 s32 value);
/* Selects a shop command (Buy, Sell, Artifacts, Repair), centred on the bottom
 */
/* row, starting on position `selection`, and returns the position chosen. */
s32 mnSelectShopCommand(s32 selection);
/* Selects a Sanctum service (Revive, Cure Poison, Repel Evil, Remove Curse), */
/* centred on the bottom row, starting on position `selection`. */
s32 mnSelectSanctumService(s32 selection);
/* Selects Yes or No, starting on `selection`; cancelling answers No. The icons
 */
/* stand on the bottom row when `bottom` is set, else at the top; `message` is
 */
/* the first label message, or 0 for the command names, and `labelWidth` the */
/* label window's width, 3 when 0. */
s32 mnYesNoMenuRun(s32 bottom, s32 message, s32 labelWidth, s32 selection);
/* Selects Coins or Lucky Medals, centred on the bottom row, starting on */
/* `selection`, and returns the position chosen. */
s32 mnSelectCoinsOrMedals(s32 selection);
/* Returns 0; only the stub table refers to it. */
s32 mnScreensReturnZero(void);
/* Selects Yes or No with the icons from tile (x, y), starting on */
/* `selection`; cancelling answers No. */
s32 mnYesNoMenuRunAt(s32 x, s32 y, s32 selection);
/* Loads the pointing hand cursor's tiles into a free VRAM slot and returns the
 */
/* slot, which the caller frees with syVramCacheFreeEntry. */
s32 mnPointingHandLoadIntoFreeSlot(void);
/* Decodes message and returns its text's width and height in pixels. */
void mnTextMeasureMessage(s32 message, s32 *x, s32 *y);
/* Switches the UI tilemap entries of the `width` x `height` tile rectangle at
 */
/* screen tile (x, y) to WINDOW_PALETTE or HIGHLIGHT_PALETTE by bit 12, keeping
 */
/* the rest of each entry: the menus' row highlight. `paletteBit` is the */
/* palette, of which only the low bit counts. */
void mnWindowHighlightRect(s32 x, s32 y, s32 width, s32 height, s32 paletteBit);
/* Builds the Psynergy icon of a battle action, as mnIconBuildAbilityIconTiles.
 */
void mnAbilityBuildIconTiles(s32 action, s32 withBase, s32 *slot, s32 *tile,
							 s32 reuse);
/* Copies the UI's ten digit tiles ('0' to '9', tiles 48 to 57) twice into */
/* `dst`, each pixel mapped through Graphics_ExpandNibbleTable: first with */
/* colour 1 drawn as colour 10, then unchanged. */
void mnGraphicsExpandVramTilesByColTable(u16 *dst);
/* Frees the second range of text tiles, and with allowAlternateTiles set lets
 */
/* text take them too. */
void mnTextUseExtraTiles(s32 allowAlternateTiles);
/* Opens the field command menu and animates from start to the command's icon.
 */
/* Returns the final icon position after closing the menu. */
s32 mnAnimateSelectionToEntry(s32 start, s32 command);
/* Returns Window_FixedTileBlocks[0], whatever no is. */
s32 mnWindowSprGetPointerTiles(s32 no);
/* Decodes message and starts it typing in window, which is already open; */
/* returns 0 when the message is empty or there is no window. */
s32 mnMessageOpenDecoded(s32 window, s32 message);
/* Returns how many save files there are, leaving out those whose summary has */
/* its send flag set (the files saved at the end of the game) when */
/* `excludeSent` is nonzero, or SAVE_ERROR_NO_MEMORY when the backup memory */
/* cannot be read. It opens and releases the save workspace itself. */
s32 mnSaveCountFilesExcludingSent(s32 excludeSent);
/* Draws an empty window: a border around a filled interior, at tile (x, y) of
 */
/* width by height tiles, and frees the text tiles the rectangle showed. */
void mnWindowDrawBorder(s32 x, s32 y, u32 width, u32 height);
/* Erases a rectangle of the window tilemap, in tiles, and frees the text */
/* tiles it showed. */
void mnWindowEraseBorderRect(s32 x, s32 y, u32 width, u32 height);
/* Draws the two page arrows (PageArrowTiles) into the two tiles at */
/* VRAM address `offset`, sliding outward: the left arrow 2 or 3 pixels to */
/* the left, the right one as far to the right, eight frames each. They are */
/* drawn over the border's top edge tile (WINDOW_TILE_BORDER_TOP) and top-right
 */
/* corner tile (WINDOW_TILE_BORDER_TOP_RIGHT). The battle menus call it each */
/* frame for LIST_PAGE_ARROW_VRAM; the field menus (by stub call) and the */
/* password pages of the title's Send call it for PAGE_ARROW_VRAM. */
void mnPageArrowsDraw(s32 offset);
/* Unused: writes frames as "HHH:MM:SS:CC", hours to hundredths, into out and */
/* returns out. Its stub mnStringFormatStopwatchStub is exported, but no */
/* overlay imports it and nothing calls it. */
u8 *mnStringFormatStopwatch(u32 frames, u8 *out);
/* Saves the game to slot (at most 2). */
s32 mnSaveToFile(u32 slot);
/* Draws an item's icon as mnItemIconCompose does and loads it into VRAM cache
 */
/* entry cacheSlot. */
s32 mnItemIconLoadLayersIntoSlot(s32 inventoryCode, s32 layers, s32 cacheSlot);
/* Draws message centred in a 192-pixel canvas of one byte a pixel, as the */
/* story crawl draws its lines. A line of dashes draws nothing and returns how
 */
/* many dashes past the first it has (the crawl waits on it); any other line */
/* returns 0. */
s32 mnTextDrawMessageToCanvas(s32 message, u8 *canvas);
/* Draws character as a glyph sprite on the window at x, y and returns its */
/* width, or 0 when no window sprite is free. */
s32 mnGlyphDrawAsSpr(WINDOW *window, s32 character, s32 x, s32 y);
/* Draws the character source names into a 16x16 tile pair and copies it to */
/* destination. */
void mnGlyphDrawToTiles(const void *source, void *destination);
/* Places a glyph sprite on the window at x, y as mnGlyphDrawAsSpr does, but */
/* draws nothing, and returns the VRAM address of its tiles for the caller to */
/* fill, or NULL when no window sprite is free. */
void *mnGlyphAddBlankSpr(WINDOW *window, s32 character, s32 x, s32 y);

/* The window sprites WIN_STATE keeps, free or in a window. */
enum { WINDOW_SPRITE_COUNT = 64 };

/* The window tiles text is drawn into: TEXT_TILES from TEXT_TILE_FIRST, and */
/* with WIN_STATE.extraTextTiles as many more from TEXT_EXTRA_TILE_FIRST. */
#define TEXT_TILE_FIRST		  128
#define TEXT_TILES			  128
#define TEXT_EXTRA_TILE_FIRST 512

/* The window renderer's windows. */
#define WINDOW_COUNT 12

/* The window system's state: its text tile and upload state, the windows' */
/* tilemap, the windows, the three text messages, the 64 window sprites, and */
/* the decoded message and its arguments. */
typedef struct WIN_STATE {
	u16 nextTextTile;  /* the text tile the next search for a free one starts at
						*/
	u8 extraTextTiles; /* 1 lets text use the second range of tiles */
	/* The tilemap's 4-row bands to upload: bit 0 for all, bit n + 1 for band n.
	 */
	u8 dirty;
	u8 mindRead;		  /* nonzero widens a message window by 2 */
	u8 battleMode;		  /* where the windows are drawn (WINDOW_BATTLE_MODE) */
	u8 holdTilemapUpload; /* while set, the tilemap is not uploaded */
	u8 textPal;			  /* text tiles' palette; 15 by default */
	u16 tilemap[20][32];  /* the windows' BG tilemap */
	WINDOW wins[WINDOW_COUNT];
	MESSAGE msgs[3];
	WINDOW_SPR sprs[WINDOW_SPRITE_COUNT];
	WINDOW_SPR_LIST freeSprs; /* the sprites not in any window */
	/* Per text tile (tiles 128 to 255, then 512 to 639): bit 0 once allocated,
	 */
	/* bit 1 while the tilemap shows it, 0 when free. */
	u8 textTileState[2 * TEXT_TILES];
	u16 lineSpacing;			/* the current message's, copied in to draw */
	u16 unusedAfterLineSpacing; /* set to 1 with the text style; not read */
	u16 outline;				/* the current message's, copied in to draw */
	u16 colour;					/* the current message's, copied in to draw */
	u16 textBuf[TEXT_BUFFER_MASK + 1]; /* decoded text, a ring of codes */
	u16 letterSpacing;				   /* 9 with the text style */
	u16 textBufEnd;					   /* where the next code goes in textBuf */
	u16 lastMsgStart;	   /* where the last decoded message began in textBuf
							*/
	u16 glyphResource;	   /* VRAM cache slot of the page arrow; 99 when none */
	u16 glyphTiles;		   /* first of the glyph tiles (cache entry 95) */
	u32 argumentValues[8]; /* message argument values (mnMessageArgumentPush) */
	u16 argumentKinds[8];  /* the MESSAGE_ARGUMENT_ kind of each; 0 when free */
	u16 portraitPictures[2]; /* picture shown in each portrait slot; 999 when */
							 /* free */
	u16 portraitSprs[2];	 /* VRAM cache slot of each portrait slot's picture
							  */
	u16 speakerVoiceSnd;	 /* first of four voice blip sounds; 0 for none */
	u16 speakerVoiceCountdown; /* characters until the next voice blip */
	u8 pageArrowShown;		   /* 1 while the next-page arrow is shown */
	u8 waitingForSnd;		   /* set while a message waits on its sound */
	/* The decode flags set for the next messages (mnMessageSetDecodeFlags): */
	/* MESSAGE_DECODE_HALTING_SPEECH and MESSAGE_DECODE_SPACING_AFTER. */
	u8 haltingSpeech;
	u8 spacingAfter;
	/* Nonzero lets a tall message fit in 32 pixels, placing its glyphs by */
	/* y / 320 (MESSAGE.compressLines). */
	s8 textMode;
} WIN_STATE;

STATIC_ASSERT(offsetof(WIN_STATE, tilemap) == 8);
STATIC_ASSERT(offsetof(WIN_STATE, wins) == 1288);
STATIC_ASSERT(offsetof(WIN_STATE, msgs) == 1720);
STATIC_ASSERT(offsetof(WIN_STATE, sprs) == 1840);
STATIC_ASSERT(offsetof(WIN_STATE, freeSprs.head) == 3632);
STATIC_ASSERT(offsetof(WIN_STATE, freeSprs.tailLink) == 3636);

STATIC_ASSERT(offsetof(WIN_STATE, lineSpacing) == 3896);
STATIC_ASSERT(offsetof(WIN_STATE, colour) == 3902);
STATIC_ASSERT(offsetof(WIN_STATE, textBuf) == 3904);
STATIC_ASSERT(offsetof(WIN_STATE, battleMode) == 5);
STATIC_ASSERT(offsetof(WIN_STATE, textPal) == 7);
STATIC_ASSERT(offsetof(WIN_STATE, textTileState) == 3640);

#if (LANGUAGE != LANGUAGE_JAPANESE)
STATIC_ASSERT(offsetof(WIN_STATE, textBufEnd) == 4930);
STATIC_ASSERT(offsetof(WIN_STATE, glyphResource) == 4934);
STATIC_ASSERT(offsetof(WIN_STATE, argumentValues) == 4940);
STATIC_ASSERT(offsetof(WIN_STATE, argumentKinds) == 4972);
STATIC_ASSERT(sizeof(WIN_STATE) == 5008);
#else
STATIC_ASSERT(offsetof(WIN_STATE, textBufEnd) == 4418);
STATIC_ASSERT(offsetof(WIN_STATE, glyphResource) == 4422);
STATIC_ASSERT(offsetof(WIN_STATE, argumentValues) == 4428);
STATIC_ASSERT(offsetof(WIN_STATE, argumentKinds) == 4460);
STATIC_ASSERT(sizeof(WIN_STATE) == 4496);
#endif

/* mnMessageSetDecodeFlags' flags, which set WIN_STATE.haltingSpeech and */
/* spacingAfter for the messages decoded next. */
typedef enum MESSAGE_DECODE_FLAG {
	MESSAGE_DECODE_HALTING_SPEECH = 1, /* a pause before each character */
	MESSAGE_DECODE_SPACING_AFTER = 2   /* a wide gap after some characters */
} MESSAGE_DECODE_FLAG;

/* WIN_STATE.glyphResource while the page arrow has no VRAM cache slot. */
#define NO_GLYPH_RESOURCE 99

/* The last character code the window's text tiles cover. */
#define TEXT_LAST_FONT_CODE 255
/* In Japanese, the codes after the font's are kanji (Font_KanjiGlyphs). */
#define KANJI_FIRST_CODE (TEXT_LAST_FONT_CODE + 1)
/* A main-font glyph row's pixels fill its halfword from the high bit, a bit */
/* each: shifted up by this, the leftmost pixel is the word's sign bit. */
#define GLYPH_ROW_BITS 16

/* WINDOW_SPR.kind: what a window sprite owns besides itself, which */
/* mnWindowSprRelease frees. */
typedef enum WINDOW_SPR_KIND {
	/* Nothing else to free: a glyph in the window's glyph tiles, or a shop's */
	/* cursor or arrow, whose tiles the shop frees. */
	WINDOW_SPRITE_PLAIN = 0,
	WINDOW_SPRITE_ICON = 1,	   /* its own VRAM cache entry */
	WINDOW_SPRITE_PORTRAIT = 2 /* a VRAM cache entry and a palette slot */
} WINDOW_SPR_KIND;

/* What mnWindowPutGlyph draws. */
typedef enum GLYPH_DRAW_MODE {
	GLYPH_DRAW_CHARACTER = 0, /* the character given, as a tile */
	GLYPH_DRAW_PAGE_ARROW = 1 /* the next-page arrow, in the window's corner */
} GLYPH_DRAW_MODE;

void mnWindowSprInitFreeList(void);

struct MENU_CURSOR;

/* The text canvas tiles in VRAM: 120 4bpp tiles from tile 296, enough for the
 */
/* interior of a full-width window six tiles tall. */
#define TEXT_CANVAS_VRAM	   ((void *)0x06002500)
#define TEXT_CANVAS_SIZE	   (120 * TILE_SIZE_4BPP)
#define TEXT_CANVAS_FIRST_TILE 296

/* The UI tilemap in VRAM (BG screen block 4), which */
/* mnWindowSystemUploadTilemap copies the state's tilemap to. */
#define WINDOW_TILEMAP_VRAM ((u8 *)0x06002000)

/* The sprite palette of portrait slot 0; slot 1 uses the next. */
#define PORTRAIT_PALETTE 14

/* The BG palette the menus keep for their selected row or page: a copy of */
/* WINDOW_PALETTE with its background lightened (mnPaletteLightenHighlight), */
/* which the item icons borrow. */
#define HIGHLIGHT_PALETTE 14
/* The window palette's background colour, behind its text; in */
/* HIGHLIGHT_PALETTE, the selected row's. */
#define WINDOW_BACKGROUND_COLOR 4
/* Eight 4bpp pixels of WINDOW_BACKGROUND_COLOR, for filling the text canvas */
/* or a tile a word at a time. */
#define TEXT_CANVAS_BACKGROUND_WORD 0x44444444
/* The colour that text drawn in a palette other than WINDOW_PALETTE stands */
/* on; the menus copy WINDOW_BACKGROUND_COLOR into it. */
#define TEXT_BACKGROUND_COLOR 14
/* WINDOW_PALETTE's white, black and two greys, which Mind Read turns grey */
/* while it shows thoughts. */
#define WINDOW_COLOR_WHITE		1
#define WINDOW_COLOR_BLACK		3
#define WINDOW_COLOR_DARK_GREY	11
#define WINDOW_COLOR_LIGHT_GREY 12
/* The BG palette that battle and the Djinn menu fill with the first basic */
/* sprite palette, its TEXT_BACKGROUND_COLOR the window background: the element
 */
/* icons and the coloured numbers are drawn in it. */
#define BASIC_COLORS_PALETTE 5

/* The two page arrows beside a paged list's page numbers, in the menus and */
/* the File Screen: tiles 296 and 297 of character block 0, which */
/* mnPageArrowsDraw slides back and forth each frame, and their */
/* tilemap entries. */
#define PAGE_ARROW_TILE 296
#define PAGE_ARROW_VRAM                                                        \
	((s32)BG_CHAR_BLOCK(0) + PAGE_ARROW_TILE * TILE_SIZE_4BPP)
#define PAGE_ARROW_LEFT	 WINDOW_TILE(PAGE_ARROW_TILE)
#define PAGE_ARROW_RIGHT WINDOW_TILE(PAGE_ARROW_TILE + 1)

/* The page numbers of a paged list, the battle lists' and the File Screen */
/* password grid's: 20 tiles from this one of character block 0, which */
/* mnGraphicsExpandVramTilesByColTable builds from the UI's digits '0' to '9',
 */
/* first with colour 1 drawn as 10, then in their own colours; and their */
/* tilemap entries, + n giving page n + 1's number, the page shown lit. */
#define PAGE_DIGITS_TILE	 768
#define PAGE_NUMBER_TILE	 WINDOW_TILE(PAGE_DIGITS_TILE + 1)
#define PAGE_NUMBER_LIT_TILE WINDOW_TILE(PAGE_DIGITS_TILE + 10 + 1)

/* How mnWindowSetTilemapEntry writes its value: as it is, not at all, or as a
 */
/* tile in one of three palettes. */
typedef enum TILEMAP_ENTRY_MODE {
	TILEMAP_ENTRY_PLAIN = 0,
	TILEMAP_ENTRY_NONE = 1,
	TILEMAP_ENTRY_HIGHLIGHT = 2, /* in HIGHLIGHT_PALETTE */
	TILEMAP_ENTRY_WINDOW = 3,	 /* in WINDOW_PALETTE */
	/* In palette 1, which the Status screens fill with the basic sprite */
	/* colours for their element icons. */
	TILEMAP_ENTRY_ICON = 4
} TILEMAP_ENTRY_MODE;

/* The window map entry of the first element icon, in BASIC_COLORS_PALETTE; */
/* the four elements follow it in order (ELEMENT). */
#define ELEMENT_ICON_TILE (BG_TILE_PALETTE(BASIC_COLORS_PALETTE) | 1)

/* The command icons (ResourceId_CommandIcons), each drawn with the command */
/* name of the same number (MsgCommandName + icon). */
typedef enum COMMAND_ICON {
	COMMAND_ICON_ATTACK = 0,
	COMMAND_ICON_PSYNERGY = 1,
	COMMAND_ICON_ITEM = 2,
	COMMAND_ICON_DEFEND = 3,
	COMMAND_ICON_FLEE = 4,
	COMMAND_ICON_YES = 5,
	COMMAND_ICON_NO = 6,
	COMMAND_ICON_STATUS = 7,
	COMMAND_ICON_CHECK = 8,
	COMMAND_ICON_USE = 9,
	COMMAND_ICON_GIVE = 10,
	COMMAND_ICON_DROP = 11,
	COMMAND_ICON_SPEAK = 12,
	COMMAND_ICON_EQUIP = 13,
	COMMAND_ICON_FIGHT = 14,
	COMMAND_ICON_DJINN = 15,
	COMMAND_ICON_SUMMON = 16,
	COMMAND_ICON_BUY = 17,
	COMMAND_ICON_SELL = 18,
	COMMAND_ICON_ARTIFACTS = 19,
	COMMAND_ICON_REPAIR = 20,
	COMMAND_ICON_NEW_GAME = 21,
	COMMAND_ICON_CONTINUE = 22,
	COMMAND_ICON_COPY = 23,
	COMMAND_ICON_ERASE = 24,
	COMMAND_ICON_REVIVE = 25,
	COMMAND_ICON_CURE_POISON = 26,
	COMMAND_ICON_REPEL_EVIL = 27,
	COMMAND_ICON_REMOVE_CURSE = 28,
	COMMAND_ICON_BATTLE = 29,
	COMMAND_ICON_SEND = 30,
	COMMAND_ICON_UPDATE = 31,
	COMMAND_ICON_COINS = 32,
	COMMAND_ICON_LUCKY_MEDALS = 33,
	/* The settings menu's choices and the Pause menu's entries */
	/* (SettingsMenu_SpeedIcons to SettingsMenu_SleepIcons), whose command names
	 */
	/* are placeholders. */
	COMMAND_ICON_SPEED_SLOW = 34,
	COMMAND_ICON_SPEED_NORMAL = 35,
	COMMAND_ICON_SPEED_FAST = 36,
	COMMAND_ICON_SPEECH_OFF = 37,
	COMMAND_ICON_SPEECH_ON = 38,
	COMMAND_ICON_AUTO_SLEEP_OFF = 39,
	COMMAND_ICON_AUTO_SLEEP_ON = 40,
	COMMAND_ICON_SAVE_QUEST = 41,
	COMMAND_ICON_SLEEP = 42,
	COMMAND_ICON_CHANGE_SETTINGS = 43,
	COMMAND_ICON_PASSWORD = 44,
	COMMAND_ICON_LINK = 45,
	COMMAND_ICON_GOLD = 46,
	COMMAND_ICON_SILVER = 47,
	COMMAND_ICON_BRONZE = 48,
	COMMAND_ICON_SWITCH = 49
} COMMAND_ICON;

/* Two copies of a tilemap entry, for filling a run of entries a word at a */
/* time. */
#define WINDOW_TILE_PAIR(entry) ((u32)(entry) << 16 | (entry))

/* A tilemap entry's tile number, the bits below its flip bits and palette */
/* number in the hardware's screen entry. */
#define TILE_NUMBER_MASK 0x3ff
/* The tilemap entry flip bits. */
#define TILE_HFLIP 0x400
#define TILE_VFLIP 0x800
/* A screen entry of tile n turned half a turn, flipped both ways. */
#define FLIP_XY(n) ((n) | TILE_HFLIP | TILE_VFLIP)
/* A screen entry of tile n flipped top to bottom. */
#define FLIP_Y(n) ((n) | TILE_VFLIP)

/* The window border's tiles. */
enum {
	WINDOW_TILE_BLANK = WINDOW_TILE(0),
	/* The frame a window flashes with as it swells (mnWindowDrawFlashBorder),
	 */
	/* which mnWindowShowFlashFrame loads over tiles 8 to 15. */
	WINDOW_TILE_FLASH_TOP_LEFT = WINDOW_TILE(8),
	WINDOW_TILE_FLASH_TOP = WINDOW_TILE(9),
	WINDOW_TILE_FLASH_TOP_RIGHT = WINDOW_TILE(10),
	WINDOW_TILE_FLASH_BOTTOM_LEFT = WINDOW_TILE(11),
	WINDOW_TILE_FLASH_BOTTOM = WINDOW_TILE(12),
	WINDOW_TILE_FLASH_BOTTOM_RIGHT = WINDOW_TILE(13),
	WINDOW_TILE_FLASH_LEFT = WINDOW_TILE(14),
	WINDOW_TILE_FLASH_RIGHT = WINDOW_TILE(15),
	WINDOW_TILE_BORDER_TOP_LEFT = WINDOW_TILE(16),
	WINDOW_TILE_BORDER_TOP = WINDOW_TILE(17),
	WINDOW_TILE_BORDER_TOP_RIGHT = WINDOW_TILE(18),
	WINDOW_TILE_BORDER_BOTTOM_LEFT = WINDOW_TILE(19),
	WINDOW_TILE_BORDER_BOTTOM = WINDOW_TILE(20),
	WINDOW_TILE_BORDER_BOTTOM_RIGHT = WINDOW_TILE(21),
	WINDOW_TILE_BORDER_LEFT = WINDOW_TILE(22),
	WINDOW_TILE_BORDER_RIGHT = WINDOW_TILE(23),
	WINDOW_TILE_BORDER_ROUND_CORNER = WINDOW_TILE(28),
	WINDOW_TILE_WINDOW_FILL = WINDOW_TILE(32),
	/* Divider lines (mnWindowDrawDividerLine) and their joins. */
	WINDOW_TILE_DIVIDER_TEE_DOWN =
		WINDOW_TILE(9), /* a vertical line leaving below */
	WINDOW_TILE_DIVIDER_TEE_UP =
		WINDOW_TILE(10), /* a vertical line leaving above */
	WINDOW_TILE_DIVIDER_TEE_RIGHT =
		WINDOW_TILE(11), /* a horizontal line leaving right */
	WINDOW_TILE_DIVIDER_TEE_LEFT =
		WINDOW_TILE(12), /* a horizontal line leaving left */
	WINDOW_TILE_DIVIDER_CROSS = WINDOW_TILE(13),
	WINDOW_TILE_DIVIDER_HORIZONTAL = WINDOW_TILE(14),
	WINDOW_TILE_DIVIDER_VERTICAL = WINDOW_TILE(15),
	WINDOW_TILE_BORDER_TOP_TEE =
		WINDOW_TILE(24), /* the top edge meeting a line */
	WINDOW_TILE_BORDER_BOTTOM_TEE =
		WINDOW_TILE(25), /* the bottom edge meeting a line */
	WINDOW_TILE_BORDER_LEFT_TEE =
		WINDOW_TILE(26), /* the left edge meeting a line */
	WINDOW_TILE_BORDER_RIGHT_TEE =
		WINDOW_TILE(27), /* the right edge meeting a line */
	/* In battle, the bottom border tiles' lower rows, which */
	/* mnWindowSystemInitBattle copies to tiles 128 to 130. */
	WINDOW_TILE_BATTLE_BORDER_BOTTOM_LEFT = WINDOW_TILE(128),
	WINDOW_TILE_BATTLE_BORDER_BOTTOM = WINDOW_TILE(129),
	WINDOW_TILE_BATTLE_BORDER_BOTTOM_RIGHT = WINDOW_TILE(130),
	/* In battle, the tile the command row fills its empty panels with, and */
	/* mnWindowEraseBorderRect below row 16. */
	WINDOW_TILE_COMMAND_PANEL = WINDOW_TILE(127),
};

/* How a window sprite moves (WINDOW_SPR.motion), run each frame by */
/* mnWindowSprUpdateAll; 0 is a free window sprite. */
enum {
	WINDOW_SPRITE_MOTION_STILL = 1,
	WINDOW_SPRITE_MOTION_PAGE_ARROW = 2, /* the next-page arrow, bobbing */
	WINDOW_SPRITE_MOTION_WOBBLE = 4,	 /* swings along a small arc */
	WINDOW_SPRITE_MOTION_SHAKE = 5, /* jitters by a pixel every other frame */
	WINDOW_SPRITE_MOTION_POP = 6,	/* shown at double size, then normal size */
	WINDOW_SPRITE_MOTION_SPIN = 7,	/* turns while circling its place */
	WINDOW_SPRITE_MOTION_SWELL =
		9, /* pulses between 1 and 1.5 times its size */
	WINDOW_SPRITE_MOTION_SWELL_HALF = 10, /* the same at half the scale */
	WINDOW_SPRITE_MOTION_SHRINK =
		11, /* shrinks from 1.5 times to its size, once */
	WINDOW_SPRITE_MOTION_SHRINK_HALF = 12, /* the same at half the scale */
	WINDOW_SPRITE_MOTION_HIDDEN = 13,	   /* not drawn */
	/* A hop and back, each in a direction of its own. */
	WINDOW_SPRITE_MOTION_HOP_RIGHT = 14,
	WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE = 15,
	WINDOW_SPRITE_MOTION_HOP_LEFT = 16,
	WINDOW_SPRITE_MOTION_HOP_UP = 17,
	WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT = 18,
	/* Still, and drawn only while within 64 pixels of the screen. */
	WINDOW_SPRITE_MOTION_ON_SCREEN = 19,
};

/* Text colours (mnTextSetCol), named as the basic sprite palette */
/* (BASIC_COLORS_PALETTE) shows them. */
typedef enum TEXT_COL {
	TEXT_COLOR_RED = 2,
	TEXT_COLOR_YELLOW = 4,
	TEXT_COLOR_PINK = 9, /* a sealed party member's Psynergy */
	TEXT_COLOR_GREY = 14,
	TEXT_COLOR_WHITE = 15, /* a message's colour at start */
} TEXT_COL;

/* The flags of a UI window (WINDOW.flg), which mnWindowCreate takes */
/* as its attributes. */
enum {
	WINDOW_FLAG_OPEN = 0x1,	   /* the window slot is in use */
	WINDOW_FLAG_INSTANT = 0x2, /* opens and closes at once, with no zoom */
	/* Passed beside WINDOW_FLAG_INSTANT by the battle menus and the party */
	/* status window; mnWindowCreate never reads it. */
	WINDOW_FLAG_UNREAD = 0x4,
	/* text is drawn into a canvas of bitmap tiles (mnWindowMapTextCanvasTiles)
	 */
	WINDOW_FLAG_TEXT_CANVAS = 0x8,
	/* the canvas has a border and a filled background; without it the canvas is
	 */
	/* transparent and borderless */
	WINDOW_FLAG_BORDERED_CANVAS = 0x20,
	WINDOW_FLAG_OUTLINED_TEXT = 0x40, /* the canvas text gets an outline */
	/* when a new page resizes the window, its bottom edge stays put */
	WINDOW_FLAG_ANCHOR_BOTTOM = 0x80,
	/* when a new page resizes the window, its left edge stays put */
	WINDOW_FLAG_FIXED_X = 0x100,
};

/* The spinning hand of the selection screen and the battle target cursor, */
/* 16 by 16 pixels in 256 colours, 256 bytes a frame (sprite_cursors.s). */
extern const u8 Menu_AnimatedCursorTiles[];

/* The labels of the battle Status screen and target window: "HP" ("PS" in */
/* Italian), the slash between a value and its maximum, and "PP". */
extern const u8 OwnerStatus_HpString[];
extern const u8 OwnerStatus_SlashString[];
extern const u8 OwnerStatus_PpString[];

/* The width in pixels of a string of 16-bit character codes. */
s32 mnTextGetWideStringWidth(u16 *text);

/* Runs the Pause menu from entry initial and returns the chosen entry (enum */
/* PAUSE_MENU_ENTRY), or -1 when B or START cancels. */
s32 mnPauseMenuChoose(s32 initial);

/* Loads a 32x32 icon into sprite tile slot *slot (a free one unless reuse is */
/* set) and its colours into sprite palette `palette`; its tile goes to *tile.
 */
void mnIconLoadWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
						   s32 palette, s32 reuse);

/* Lets the player choose the reserve member who takes party slot `slot` in */
/* battle. */
s32 mnBattlePartyChooseReserve(s32 slot);

/* Counts the standby Djinn on unitId's side, and each element's into counts */
/* when it is not NULL (btDjinnCountStandby). */
s32 btDjinnCountStandbyStub(u32 unitId, u8 *counts);

/* The Djinn tutorial's message on giving a Djinni. */
s32 mnDjinnGetGiveTutorialMessage(void);

/* Decodes message and empties the argument table; returns where its text */
/* starts. */
s32 mnMessageDecodeAndClearArguments(s32 message);
/* Decodes message after the text decoded last. */
void mnMessageDecodeAppend(s32 message);

/* Moves a menu cursor a frame along its straight line toward its target */
/* (mnCursorAdvance). */
void mnCursorAdvanceStub(struct MENU_CURSOR *cursor);
/* Moves a menu cursor a quarter of the way to its target, at least a pixel */
/* (mnCursorMoveTowardTarget). */
void mnCursorMoveTowardTargetStub(struct MENU_CURSOR *cursor);

/* Writes input as decimal text into buffer and returns where it starts: with */
/* width 0 unpadded and signed, else right-aligned in width characters. */
u8 *mnStringFormatNum(u8 *buffer, s32 input, s32 width);

/* A task: draws each party sprite of the file list at its place and scale. */
void mnFileListPlacePartySprs(void);

/* The pointing hand cursor, a 16x16 sprite, shared by the battle menus, the */
/* name entry screen and the talk choices. */
extern const u8 PointingHandTiles[];

/* Fills the text canvas with the window background colour. */
s32 mnTextFillCanvasBackground(void);

/* Opens the party status window of the field menu and waits a frame. */
void mnPartyStatOpenAndWait(void);

/* The layers mnItemIconCompose draws over an item's icon. */
#define ICON_BASE			 1 /* the base under the icon */
#define ICON_COUNT_ABOVE_ONE 2 /* the count, unless it is 1 */
#define ICON_COUNT			 4 /* the count, always */
/* Mark 1, the cross, on an item with INVENTORY_BROKEN. */
#define ICON_BROKEN 8
/* Mark 0, the equip mark, on an item with INVENTORY_EQUIPPED. */
#define ICON_EQUIPPED 16
/* Mark 2 on a cursed item that cannot be unequipped (ITEM_CURSED and */
/* ITEM_CANNOT_UNEQUIP) when its code has INVENTORY_EQUIPPED. */
#define ICON_CURSED 32

/* An action's id in its encoded list entry, below its flags. */
#define ACTION_ID_MASK 0x3fff

/* The party status window's flags (mnPartyStatDraw, */
/* mnPartyStatCreateWindow): one column of six tiles per party member */
/* (name, HP and, when shown, PP with a bar under each), and in battle an */
/* optional five-tile column of standby Djinn counts on its left. */
typedef enum PARTY_STAT_FLAG {
	PARTY_STATUS_HP_ONLY = 0,	  /* names and HP */
	PARTY_STATUS_SHOW_PP = 1,	  /* add the PP row */
	PARTY_STATUS_SHOW_DJINN = 2,  /* add the Djinn column (needs the PP row) */
	PARTY_STATUS_CLOSED = 9,	  /* erase the window */
	PARTY_STATUS_KEEP_FLAGS = -1, /* redraw with the flags it has */
	/* BATTLE_STATE.partyStat while the party cannot summon. */
	PARTY_STATUS_NO_SUMMONS = PARTY_STATUS_SHOW_PP,
	/* BATTLE_STATE.partyStat while it can, drawn only while a Djinni is on */
	/* standby. */
	PARTY_STATUS_SUMMONS = PARTY_STATUS_SHOW_PP | PARTY_STATUS_SHOW_DJINN
} PARTY_STAT_FLAG;

/* Where the windows are drawn (WIN_STATE.battleMode). */
typedef enum WINDOW_BATTLE_MODE {
	WINDOW_FIELD = 0,  /* on the field; the field setup clears the block */
	WINDOW_BATTLE = 1, /* in battle, from mnWindowSystemInitBattle */
	/* In battle, while mnBattleMessageOpen decodes a battle message: the */
	/* Japanese decoder then sets names off with spaces and style codes. */
	WINDOW_BATTLE_MESSAGE = 2,
	/* A flag with WINDOW_BATTLE: glyphs are drawn blended (semi-transparent),
	 */
	/* while the Djinn tutorial's line shows over the command row. */
	WINDOW_GLYPHS_BLENDED = 4,
} WINDOW_BATTLE_MODE;

/* WIN_STATE.portraitPictures of a free portrait slot, as */
/* mnPortraitClearSlots writes it. */
#define PORTRAIT_SLOT_EMPTY 999

/* The window mnFieldMessageShowCentered shows its message in. */
typedef enum CENTERED_MESSAGE_MODE {
	/* No frame or size: it opens and closes at once. */
	CENTERED_MESSAGE_BARE = 0,
	/* A zooming window sized to the message, waited for as it closes. */
	CENTERED_MESSAGE_WINDOW = 1
} CENTERED_MESSAGE_MODE;
/* The colour a bare centred message is written in, which a scene sets white */
/* again before each one while the screen is warm-tinted (Mars Lighthouse's */
/* inscriptions, the Gabomba Statue's chamber). */
#define CENTERED_TEXT_COLOR OBJ_PLTT_COLOR(0, 15)

/* The flags mnFieldMessageShow takes. */
typedef enum FIELD_MESSAGE_FLAG {
	FIELD_MESSAGE_ZOOM = 0x01, /* the window zooms open and shut */
	/* The window state is busy while it shows. */
	FIELD_MESSAGE_WAITING_FOR_SOUND = 0x02,
	FIELD_MESSAGE_KEEP_OPEN = 0x04,	  /* the window is left open */
	FIELD_MESSAGE_LOWER = 0x08,		  /* 4 tiles below the top rows' centre */
	FIELD_MESSAGE_HOLD_UPLOAD = 0x20, /* the tilemap upload is held off */
	FIELD_MESSAGE_LOWEST = 0x40		  /* 12 tiles below the top rows' centre */
} FIELD_MESSAGE_FLAG;

/* The flags a yes-or-no question's message takes: it zooms open, stays open */
/* for the Yes/No menu below it and sits lower. */
#define QUESTION_MESSAGE                                                       \
	(FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN | FIELD_MESSAGE_LOWER)

/* The Pause menu's entries, as mnPauseMenuChoose returns them */
/* (counted from Save your quest even when only Change settings shows). */
typedef enum PAUSE_MENU_ENTRY {
	PAUSE_MENU_SAVE_QUEST = 0,
	PAUSE_MENU_SLEEP = 1,
	PAUSE_MENU_CHANGE_SETTINGS = 2,
	/* In debug mode, R and L also leave the menu with these; mnPauseMenuRun
	   acts */
	/* on neither. */
	PAUSE_MENU_DEBUG_R = 20,
	PAUSE_MENU_DEBUG_L = 21
} PAUSE_MENU_ENTRY;

#define WINDOW_COLOR_COUNT 24 /* "Window color" takes 0 to 23 */
/* "Window brightness" takes 0 to 14. */
#define WINDOW_BRIGHTNESS_COUNT 15

/* The brightness that adds nothing to the hue's colour. */
#define WINDOW_BRIGHTNESS_NEUTRAL 7

/* Steps round WindowColor_WaveTable's colour wheel; each hue is four. */
#define WINDOW_COLOR_WHEEL_STEPS 96

/* How many levels HIGHLIGHT_PALETTE lightens each channel of */
/* WINDOW_BACKGROUND_COLOR by (mnPaletteLightenHighlight). */
#define HIGHLIGHT_LIGHTEN_STEPS 9

/* The bob of a prompt or cursor: up and down by up to 2 pixels, a sine every */
/* 16 frames. */
#define CURSOR_BOB_Y() (sySin(FrameCount * (TRIG_TURN / 16)) / (FIXED_ONE / 2))

/* mnMessageOpenWindow's packed argument: the speaker's voice in bits 16 to 27,
 */
/* above the window options. */
#define MESSAGE_WINDOW_VOICE(voice) ((voice) << 16)
/* The speaker's voice in a packed argument, as mnMessageOpenWindow takes it */
/* out. */
#define MESSAGE_WINDOW_VOICE_OF(packed) (((u32)(packed) << 4) >> 20)
/* The window options of a packed argument: the bits below the speaker's */
/* voice. */
#define MESSAGE_WINDOW_OPTIONS_MASK (MESSAGE_WINDOW_VOICE(1) - 1)

/* mnMessageOpenWindow's window options, in packed's low bits. */
typedef enum MESSAGE_WINDOW_OPTION {
	MESSAGE_WINDOW_ZOOM = 1, /* zoom open rather than appear at once */
	/* Set by the shopkeepers' windows and some others; mnMessageOpenWindow
	   never */
	/* reads it. */
	MESSAGE_WINDOW_UNREAD = 2,
	MESSAGE_WINDOW_TEXT_CANVAS = 8,	   /* WINDOW_FLAG_TEXT_CANVAS */
	MESSAGE_WINDOW_ANCHOR_BOTTOM = 16, /* WINDOW_FLAG_ANCHOR_BOTTOM */
	MESSAGE_WINDOW_FIXED_X = 32		   /* WINDOW_FLAG_FIXED_X */
} MESSAGE_WINDOW_OPTION;

/* mnPortraitOpenWindow's window is PORTRAIT_WINDOW_TILES tiles square, a tile
 */
/* wider while minds are read. */
#define PORTRAIT_WINDOW_TILES 5

/* The party members' portraits, the icon bank's first rows, as */
/* Portrait_CharacterMap pairs them with the party. */
typedef enum CHARACTER_PORTRAIT {
	PORTRAIT_ISAAC,
	PORTRAIT_GARET,
	PORTRAIT_IVAN,
	PORTRAIT_MIA,
	PORTRAIT_FELIX,
	PORTRAIT_JENNA,
	PORTRAIT_SHEBA,
	PORTRAIT_PIERS,
	PORTRAIT_PIERS_SPEAKING /* the one Piers's messages show */
} CHARACTER_PORTRAIT;

/* The other speakers' portraits, from the icon bank's row */
/* PORTRAIT_FIRST_SPRITE_ICON on, as Portrait_SpriteMap pairs them with */
/* sprites; each is named for its picture. */
typedef enum SPR_PORTRAIT {
	SPRITE_PORTRAIT_JUPITER_DJINNI = 0,
	SPRITE_PORTRAIT_VENUS_DJINNI,
	SPRITE_PORTRAIT_MARS_DJINNI,
	SPRITE_PORTRAIT_MERCURY_DJINNI,
	SPRITE_PORTRAIT_KRADEN,
	SPRITE_PORTRAIT_SATUROS,
	SPRITE_PORTRAIT_MENARDI,
	SPRITE_PORTRAIT_ALEX,
	SPRITE_PORTRAIT_DORA,
	SPRITE_PORTRAIT_KYLE,
	SPRITE_PORTRAIT_WOMAN_IN_GREEN_HEADSCARF,
	SPRITE_PORTRAIT_WOMAN_WITH_ORANGE_PONYTAIL,
	SPRITE_PORTRAIT_BOY_WITH_ORANGE_HAIR,
	SPRITE_PORTRAIT_MAN_IN_TEAL_SHIRT,
	SPRITE_PORTRAIT_MAN_IN_TALL_HAT,
	SPRITE_PORTRAIT_BEARDED_MAN_IN_LONG_ROBE,
	SPRITE_PORTRAIT_BALD_MAN_IN_RED,
	SPRITE_PORTRAIT_YOUNG_MAN_IN_GREEN,
	SPRITE_PORTRAIT_WOMAN_WITH_PONYTAIL,
	SPRITE_PORTRAIT_WOMAN_IN_PINK_APRON,
	SPRITE_PORTRAIT_MAN_WITH_DARK_HAIR,
	SPRITE_PORTRAIT_ISAAC_CHILD,
	SPRITE_PORTRAIT_GARET_CHILD,
	SPRITE_PORTRAIT_FELIX_CHILD,
	SPRITE_PORTRAIT_JENNA_CHILD,
	SPRITE_PORTRAIT_FELIX_MASKED,
	SPRITE_PORTRAIT_FELIX_ADULT = 27,
	SPRITE_PORTRAIT_INVISIBLE_BABI,
	SPRITE_PORTRAIT_FARAN,
	SPRITE_PORTRAIT_POI,
	SPRITE_PORTRAIT_RIKI,
	SPRITE_PORTRAIT_TAVI,
	SPRITE_PORTRAIT_MADRAS_MAYOR,
	SPRITE_PORTRAIT_MADRAS_ELDER,
	SPRITE_PORTRAIT_ALHAFRAS_MAYOR,
	SPRITE_PORTRAIT_FORTUNE_TELLER,
	SPRITE_PORTRAIT_AKAFUBU,
	SPRITE_PORTRAIT_MOAPA,
	SPRITE_PORTRAIT_MAHA,
	SPRITE_PORTRAIT_JENNAS_FATHER,
	SPRITE_PORTRAIT_JENNAS_MOTHER,
	SPRITE_PORTRAIT_BRIGGS,
	SPRITE_PORTRAIT_CHAUCHA,
	SPRITE_PORTRAIT_EOLEO,
	SPRITE_PORTRAIT_OBABA,
	SPRITE_PORTRAIT_SUSA,
	SPRITE_PORTRAIT_LADY_UZUME,
	SPRITE_PORTRAIT_KUSHINADA,
	SPRITE_PORTRAIT_HYDROS,
	SPRITE_PORTRAIT_CONSERVATO,
	SPRITE_PORTRAIT_KARST,
	SPRITE_PORTRAIT_AGATIO,
	SPRITE_PORTRAIT_WEAPON_MERCHANT = 54,
	SPRITE_PORTRAIT_ARMOR_MERCHANT,
	SPRITE_PORTRAIT_WOMAN_WITH_TEAL_HEADBAND,
	SPRITE_PORTRAIT_INNKEEPER,
	SPRITE_PORTRAIT_INN_WORKER,
	SPRITE_PORTRAIT_WOMAN_WITH_ORANGE_HAIR,
	SPRITE_PORTRAIT_ARTIFACT_MERCHANT,
	SPRITE_PORTRAIT_LUNPA,
	SPRITE_PORTRAIT_PUELLE,
	SPRITE_PORTRAIT_MASKED_SHAMAN,
	SPRITE_PORTRAIT_PROX_ELDER,
	SPRITE_PORTRAIT_SUNSHINE,
	SPRITE_PORTRAIT_MAN_WITH_ORANGE_BEARD,
	SPRITE_PORTRAIT_GIRL_WITH_LONG_BLONDE_HAIR,
	SPRITE_PORTRAIT_IZUMO_WOMAN_IN_CREAM,
	SPRITE_PORTRAIT_IZUMO_MAN_IN_RED,
	SPRITE_PORTRAIT_IZUMO_WOMAN_IN_ORANGE,
	SPRITE_PORTRAIT_OLD_MAN_WITH_GREY_BEARD,
	SPRITE_PORTRAIT_OLD_WOMAN_WITH_GREY_HAIR,
	SPRITE_PORTRAIT_FEIZHI,
	SPRITE_PORTRAIT_HAMMA,
	/* A row of the icon bank with no picture. */
	SPRITE_PORTRAIT_MISSING = 76
} SPR_PORTRAIT;

void mnWindowHighlightRectStub(s32 x, s32 y, s32 width, s32 height, s32 style);
void mnWindowSetTilemapEntryStub(s32 window, s32 value, s32 x, s32 y, u32 mode);
WINDOW_SPR *mnWindowSprCreateFromResourceStub(s32 mode, s32 resource,
											  WINDOW *window, s32 x, s32 y);
void mnTextDrawMessageStub(s32 message, s32 window, s32 x, s32 y);

/* Each main font glyph's width in pixels, from the space (32) on. */
extern const u8 Font_GlyphWidths[];
/* Clears the text canvas to transparent. */
s32 mnTextClearCanvas(void);

/* The window sprites' fixed tile blocks (window/fixed_tile_blocks.c). */
extern s32 Window_FixedTileBlocks[];
/* Starts the field's per-frame window update, after reserving the glyph */
/* sprites' tiles and resetting the text style. */
void mnWindowSystemStartFieldUpdate(void);
/* Starts the per-frame window update: the field's when loadGlyphTiles is set,
 */
/* else the battle's. */
void mnWindowSystemStartBattleUpdate(s32 loadGlyphTiles);
/* Steps each window's open or close animation. */
void mnWindowUpdateZooms(void);
/* Types more of each message whose window is open, and closes or marks its */
/* window at the text's end. */
void mnMessageUpdateAll(void);
/* Copies each band of the window tilemap marked dirty to VRAM, a VBlank task.
 */
void mnWindowSystemUploadTilemap(void);
/* Frees the text tiles under a rectangle of the window tilemap, in tiles. */
void mnWindowClearTextTilesInRect(s32 x, s32 y, u32 width, u32 height);
/* Moves each sprite an open window carries and queues it for drawing. */
void mnWindowSprUpdateAll(void);
/* Frees a window sprite and the VRAM cache entry it showed. */
void mnWindowSprRelease(WINDOW_SPR *node);
/* Marks both portrait slots free. */
void mnPortraitClearSlots(void);

/* The party status window's column lines, ended by -1: the Djinn column's */
/* first, then one between members. */
extern const s8 Window_PartyColumnOffsets[];

#endif
