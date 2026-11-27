#ifndef ALCHEMY_WINDOW_STUBS_H
#define ALCHEMY_WINDOW_STUBS_H

/* @file */
/* The stub calls into the window module, as the other modules' code calls */
/* them. */

#include "types.h"

struct BATTLE_ACTION_RECORD;
struct WINDOW;
struct WINDOW_SPR;
/* Sets up the window system for battle. */
void mnWindowSystemInitBattleStub(s32 kind);
/* Opens the party status window, sized by mnPartyStatBuildLayoutBounds, and */
/* draws it, holding the tilemap upload meanwhile. */
void mnPartyStatCreateWindowStub(s32 flags);
/* Returns 1 once no message window is still typing its text, else 0. */
s32 mnMessageAllTypedStub(void);
/* Returns the pointing hand's tiles, whatever index is asked for. */
s32 mnWindowSprGetPointerTilesStub(s32 index);
/* Sets up the window system for the field and the other screens outside */
/* battle. */
void mnWindowSystemInitFieldStub(void);
/* Opens a window at tile (x, y) of width by height tiles in the first free */
/* window slot, and returns it, or NULL when all slots are in use. attrs takes
 */
/* the WINDOW_FLAG_ flags. */
struct WINDOW *mnWindowCreateStub(s32 x, s32 y, s32 width, s32 height,
								  s32 attrs);
/* Closes a window; with release it also erases the window's border from the */
/* screen and forgets its place. */
void mnWindowCloseStub(struct WINDOW *window, s32 release);
/* Decodes a message and draws it as tiles in a window at pixel (offsetX, */
/* offsetY) of its interior. */
void mnTextDrawMessageAsTilesStub(s32 message, struct WINDOW *window,
								  s32 offsetX, s32 offsetY);
/* Closes each message's window whose text has ended, then waits ten frames. */
void mnMessageCloseFinishedWindowsStub(void);
/* Selects Yes or No, starting on `selection`; cancelling answers No. The icons
 */
/* stand on the bottom row when `bottom` is set; `message` is the first label */
/* message, or 0 for the command names, and `labelWidth` the label window's */
/* width. */
s32 mnYesNoMenuRunStub(s32 bottom, s32 message, s32 labelWidth, s32 selection);
/* Loads the pointing hand cursor's tiles into a free VRAM slot and returns the
 */
/* slot. */
s32 mnPointingHandLoadIntoFreeSlotStub(void);
/* Clears a window's contents: redraws it empty and releases its sprites. */
void mnWindowSprPrepareForRedrawStub(void *window);
/* Shows a message in a window centred on the screen, waits until it is typed */
/* out and closes it, as its flags say. */
void fdMessageShowStub(s32 message, s32 flags);
/* The debug flag editor: shows 16 rows of 16 flags a page and takes input */
/* until closed, starting on page 8. */
void mnDebugFlagEditorRunStub(void);
/* Runs the field menu until the player picks something that leaves it. */
void mnFieldMenuRunStub(void);
/* The debug scene selector: picks a scene and entrance and travels there. */
void mnDebugSceneSelectRunStub(void);
/* Closes the party status window and frees its heap block. */
void mnPartyStatCloseStub(void);
/* Sets the decode flags the next message is decoded with (mnMessageDecode). */
void mnMessageSetDecodeFlagsStub(s32 flags);
/* Fills `spans`, a page of the screen transition's window edges, with the */
/* windows' horizontal extent on each of the 160 scan lines. */
void mnScanlineWindowFillSpansStub(u16 *spans);
/* As mnTextGetMessageSizeStub, but measuring every page of the message. */
void mnTextGetMessageSizeAllPagesStub(s32 message, s32 *left, s32 *top,
									  u32 *width, u32 *height);
/* Decodes a message and fits a window for its first page on the screen near */
/* tile (*left, *top), returning its place and size in tiles. */
void mnTextGetMessageSizeStub(s32 message, s32 *left, s32 *top, u32 *width,
							  u32 *height);
/* Returns the portrait of a speaker, a character or a sprite, or -1 when it */
/* has none. */
s32 mnPortraitGetForSpeakerStub(s32 speaker);
/* Decodes message into text as a zero-terminated string of 16-bit codes, with
 */
/* at most capacity entries, keeping style codes with their operands. */
void mnMessageDecodeRawStub(s32 message, u16 *text, s32 capacity);
/* Copies icon `icon` into a free VRAM slot and shows it as a 16x16 sprite at */
/* (x, y) in `window`. */
struct WINDOW_SPR *mnWindowSprCreateLoadedStub(s32 icon, s32 layers,
											   struct WINDOW *window, s32 x,
											   s32 y);
/* Selects a shop command (Buy, Sell, Artifacts, Repair) on the bottom row, */
/* starting on `selection`, and returns the one chosen. */
s32 mnSelectShopCommandStub(s32 selection);
/* Selects a Sanctum service (Revive, Cure Poison, Repel Evil, Remove Curse) on
 */
/* the bottom row, starting on `selection`. */
s32 mnSelectSanctumServiceStub(s32 selection);
/* Decodes message and returns its text's width and height in pixels. */
void mnTextMeasureMessageStub(s32 message, s32 *width, s32 *height);
/* Sets the window colours for a window colour (0 to 23) and brightness (0 to */
/* 14, 7 neutral). */
void mnWindowColApplyStub(s32 color, s32 brightness);
/* Collects the party's commands for a turn into `commands`, one for each of */
/* the `commandCount` units in `actors`. */
s32 btCollectPartyCommandsStub(struct BATTLE_ACTION_RECORD *commands,
							   void *actors, s32 commandCount);
/* Opens the area-name window, which closes after ninety frames. */
void mnAreaNameWindowShowStub(s32 scene);
/* Handles Save your quest, Sleep and Change settings from the Pause menu. The
 */
/* field passes it a 0 it does not read. */
void mnPauseMenuRunStub(s32 unused);
/* Announces a summon the party can now use. */
void gmSummonShowLearnedMessageStub(s32 summon);

/* Resets the pen and text style of the first idle message. */
void mnMessageResetIdleStub(void);
/* Selects Yes or No at (x, y). */
s32 mnYesNoMenuRunAtStub(s32 x, s32 y, s32 selection);

/* Draws message centred in a 192-pixel canvas of one byte a pixel, as the */
/* credits draw their scrolling lines; they pass an alignment it does not read.
 */
s32 mnTextDrawMessageToCanvasStub(s32 message, u8 *canvas, s32 unused);

/* Opens a portrait window at tile (x, y) showing the speaker's portrait. */
struct WINDOW *mnPortraitOpenWindowStub(s32 speaker, s32 portraitOption, s32 x,
										s32 y);

/* Draws a byte string as tiles in a window at pixel (offsetX, offsetY) of its
 */
/* interior. */
void mnTextDrawStringAtOffsetStub(const u8 *text, s32 window, s32 offsetX,
								  s32 offsetY);
/* Draws a byte string into a window's tilemap at pixel (x, y) of its */
/* interior, rounded down to tiles. */
void mnTextDrawStringInWindowStub(u8 *text, s32 window, u32 x, u32 y);
/* Draws a number, width digits as mnStringFormatNum takes it, as tiles in a */
/* window at pixel (x, y) of its interior. */
void mnTextDrawNumAtOffsetStub(s32 value, s32 width, s32 window, s32 x, s32 y);
/* As mnTextDrawNumAtOffsetStub, into the window's tilemap. */
void mnTextDrawNumInWindowStub(s32 value, s32 width, s32 window, s32 x, s32 y);
/* Sets the text colour (0 to 15) of drawn text. */
s32 mnTextSetColStub(s32 color);
/* mnAbilityBuildIconTiles, for an overlay. */
void btAbilityBuildIconTilesStub(s32 action, s32 withBase, s32 *slot, s32 *tile,
								 s32 reuse);
/* Draws a divider from (x1, y1) to (x2, y2), one of which must agree, joining
 */
/* it to the frame and divider tiles already on the tilemap. */
void mnWindowDrawDividerLineStub(s32 window, s32 x1, s32 y1, s32 x2, s32 y2);
/* Reloads the portrait picture for the key into a portrait slot with that */
/* slot's palette (PORTRAIT_PALETTE + slot), reusing the slot's VRAM cache */
/* entry. */
void mnPortraitReloadStub(s32 slot, s32 key, s32 unused);

#endif
