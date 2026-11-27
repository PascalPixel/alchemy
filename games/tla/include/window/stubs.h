#ifndef ALCHEMY_WINDOW_STUBS_H
#define ALCHEMY_WINDOW_STUBS_H

// The stub calls into the window module, as the other modules' code calls them.

#include "types.h"

struct BATTLE_ACTION_RECORD;
struct WINDOW;
struct WINDOW_SPR;
// Sets up the window system for battle.
void   mnWindowSystemInitBattleStub(Sint32 kind);
// Opens the party status window, sized by mnPartyStatusBuildLayoutBounds, and draws it, holding the
// tilemap upload meanwhile.
void   mnPartyStatusCreateWindowStub(Sint32 flags);
// Returns 1 once no message window is still typing its text, else 0.
Sint32 mnMessageAllTypedStub(void);
// Returns the pointing hand's tiles, whatever index is asked for.
Sint32 mnWindowSpriteGetPointerTilesStub(Sint32 index);
// Sets up the window system for the field and the other screens outside battle.
void   mnWindowSystemInitFieldStub(void);
// Opens a window at tile (x, y) of width by height tiles in the first free window slot, and returns
// it, or NULL when all slots are in use. attributes takes the WINDOW_FLAG_ flags.
struct WINDOW        *
mnWindowCreateStub(Sint32 x, Sint32 y, Sint32 width, Sint32 height, Sint32 attributes);
// Closes a window; with release it also erases the window's border from the screen and forgets its
// place.
void   mnWindowCloseStub(struct WINDOW *window, Sint32 release);
// Decodes a message and draws it as tiles in a window at pixel (offsetX, offsetY) of its interior.
void   mnTextDrawMessageAsTilesStub(Sint32         msg,
                                    struct WINDOW *window,
                                    Sint32         offsetX,
                                    Sint32         offsetY);
// Closes each message's window whose text has ended, then waits ten ticks.
void   mnMessageCloseFinishedWindowsStub(void);
// Selects Yes or No, starting on `selection`; cancelling answers No. The icons stand on the bottom
// row when `bottom` is set; `msg` is the first label message, or 0 for the command names, and
// `labelWidth` the label window's width.
Sint32 mnYesNoMenuRunStub(Sint32 bottom, Sint32 msg, Sint32 labelWidth, Sint32 selection);
// Loads the pointing hand cursor's tiles into a free VRAM slot and returns the slot.
Sint32 mnPointingHandLoadIntoFreeSlotStub(void);
// Clears a window's contents: redraws it empty and releases its sprites.
void   mnWindowSpritePrepareForRedrawStub(void *window);
// Shows a message in a window centred on the screen, waits until it is typed out and closes it, as
// its flags say.
void   mnFieldMessageShowStub(Sint32 msg, Sint32 flags);
// The debug flag editor: shows 16 rows of 16 flags a page and takes input until closed, starting on
// page 8.
void   mnDebugFlagEditorRunStub(void);
// Runs the field menu until the player picks something that leaves it.
void   mnFieldMenuRunStub(void);
// The debug scene selector: picks a scene and entrance and travels there.
void   mnDebugSceneSelectRunStub(void);
// Closes the party status window and frees its heap block.
void   mnPartyStatusCloseStub(void);
// Sets the decode flags the next message is decoded with (mnMessageDecode).
void   mnMessageSetDecodeFlagsStub(Sint32 flags);
// Fills `spans`, a page of the screen transition's window edges, with the windows' horizontal
// extent on each of the 160 scan lines.
void   mnScanlineWindowFillSpansStub(Uint16 *spans);
// As mnTextGetMessageSizeStub, but measuring every page of the message.
void   mnTextGetMessageSizeAllPagesStub(
      Sint32 msg, Sint32 *left, Sint32 *top, Uint32 *width, Uint32 *height);
// Decodes a message and fits a window for its first page on the screen near tile (*left, *top),
// returning its place and size in tiles.
void mnTextGetMessageSizeStub(Sint32 msg, Sint32 *left, Sint32 *top, Uint32 *width, Uint32 *height);
// Returns the portrait of a speaker, a character or a sprite, or -1 when it has none.
Sint32             mnPortraitGetForSpeakerStub(Sint32 speaker);
// Decodes message into text as a zero-terminated string of 16-bit codes, with at most capacity
// entries, keeping style codes with their operands.
void               mnMessageDecodeRawStub(Sint32 msg, Uint16 *text, Sint32 capacity);
// Copies icon `icon` into a free VRAM slot and shows it as a 16x16 sprite at (x, y) in `window`.
struct WINDOW_SPR *mnWindowSpriteCreateLoadedStub(
    Sint32 icon, Sint32 layers, struct WINDOW *window, Sint32 x, Sint32 y);
// Selects a shop command (Buy, Sell, Artifacts, Repair) on the bottom row, starting on `selection`,
// and returns the one chosen.
Sint32         mnSelectShopCommandStub(Sint32 selection);
// Selects a Sanctum service (Revive, Cure Poison, Repel Evil, Remove Curse) on the bottom row,
// starting on `selection`.
Sint32         mnSelectSanctumServiceStub(Sint32 selection);
// Decodes message and returns its text's width and height in pixels.
void           mnTextMeasureMessageStub(Sint32 msg, Sint32 *width, Sint32 *height);
// Sets the window colours for a window colour (0 to 23) and brightness (0 to 14, 7 neutral).
void           mnWindowColorApplyStub(Sint32 color, Sint32 brightness);
// Collects the party's commands for a turn into `commands`, one for each of the `commandCount`
// units in `actors`.
Sint32         mnBattleCollectPartyCommandsStub(struct BATTLE_ACTION_RECORD *commands,
                                                void                        *actors,
                                                Sint32                       commandCount);
// Opens the area-name window, which closes after ninety ticks.
void           mnAreaNameWindowShowStub(Sint32 scene);
// Handles Save your quest, Sleep and Change settings from the Pause menu. The field passes it a 0
// it does not read.
void           mnPauseMenuRunStub(Sint32 unused);
// Announces a summon the party can now use.
void           mnSummonShowLearnedMessageStub(Sint32 summon);

// Resets the pen and text style of the first idle message.
void           mnMessageResetIdleStub(void);
// Selects Yes or No at (x, y).
Sint32         mnYesNoMenuRunAtStub(Sint32 x, Sint32 y, Sint32 selection);

// Draws message centred in a 192-pixel canvas of one byte a pixel, as the credits draw their
// scrolling lines; they pass an alignment it does not read.
Sint32         mnTextDrawMessageToCanvasStub(Sint32 msg, Uint8 *canvas, Sint32 unused);

// Opens a portrait window at tile (x, y) showing the speaker's portrait.
struct WINDOW *mnPortraitOpenWindowStub(Sint32 speaker, Sint32 portraitOption, Sint32 x, Sint32 y);

// Draws a byte string as tiles in a window at pixel (offsetX, offsetY) of its interior.
void mnTextDrawStringAtOffsetStub(const Uint8 *text, Sint32 window, Sint32 offsetX, Sint32 offsetY);
// Draws a byte string into a window's tilemap at pixel (x, y) of its interior, rounded down to
// tiles.
void mnTextDrawStringInWindowStub(Uint8 *text, Sint32 window, Uint32 x, Uint32 y);
// Draws a number, width digits as mnStringFormatNumber takes it, as tiles in a window at pixel (x,
// y) of its interior.
void mnTextDrawNumberAtOffsetStub(Sint32 value, Sint32 width, Sint32 window, Sint32 x, Sint32 y);
// As mnTextDrawNumberAtOffsetStub, into the window's tilemap.
void mnTextDrawNumberInWindowStub(Sint32 value, Sint32 width, Sint32 window, Sint32 x, Sint32 y);
// Sets the text colour (0 to 15) of drawn text.
Sint32 mnTextSetColorStub(Sint32 color);
// mnAbilityBuildIconTiles, for an overlay.
void   mnAbilityBuildIconTilesStub(
      Sint32 action, Sint32 withBase, Sint32 *slot, Sint32 *tile, Sint32 reuse);
// Draws a divider from (x1, y1) to (x2, y2), one of which must agree, joining it to the tick and
// divider tiles already on the tilemap.
void mnWindowDrawDividerLineStub(Sint32 window, Sint32 x1, Sint32 y1, Sint32 x2, Sint32 y2);
// Reloads the portrait picture for the key into a portrait slot with that slot's palette
// (PORTRAIT_PALETTE + slot), reusing the slot's VRAM cache entry.
void mnPortraitReloadStub(Sint32 slot, Sint32 key, Sint32 unused);

#endif
