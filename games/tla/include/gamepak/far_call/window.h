#ifndef ALCHEMY_FAR_CALL_WINDOW_H
#define ALCHEMY_FAR_CALL_WINDOW_H

/// @file
/// The far calls into the window bank, as the other banks' code calls them.

#include "types.h"

struct BattleActionRecord;
struct Window;
struct WindowSprite;
/// Sets up the window system for battle.
void WindowSystem_InitBattleFar(s32 kind);
/// Opens the party status window, sized by PartyStatus_BuildLayoutBounds, and
/// draws it, holding the tilemap upload meanwhile.
void PartyStatus_CreateWindowFar(s32 flags);
/// Returns 1 once no message window is still typing its text, else 0.
s32 Message_AllTypedFar(void);
/// Returns the pointing hand's tiles, whatever index is asked for.
s32 WindowSprite_GetPointerTilesFar(s32 index);
/// Sets up the window system for the field and the other screens outside
/// battle.
void WindowSystem_InitFieldFar(void);
/// Opens a window at tile (x, y) of width by height tiles in the first free
/// window slot, and returns it, or NULL when all slots are in use. attrs takes
/// the WINDOW_FLAG_ flags.
struct Window *Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 attrs);
/// Closes a window; with release it also erases the window's border from the
/// screen and forgets its place.
void Window_CloseFar(struct Window *window, s32 release);
/// Decodes a message and draws it as tiles in a window at pixel (offsetX,
/// offsetY) of its interior.
void Text_DrawMessageAsTilesFar(s32 message, struct Window *window, s32 offsetX,
                                s32 offsetY);
/// Closes each message's window whose text has ended, then waits ten frames.
void Message_CloseFinishedWindowsFar(void);
/// Selects Yes or No, starting on `selection`; cancelling answers No. The icons
/// stand on the bottom row when `bottom` is set; `message` is the first label
/// message, or 0 for the command names, and `labelWidth` the label window's
/// width.
s32 YesNoMenu_RunFar(s32 bottom, s32 message, s32 labelWidth, s32 selection);
/// Loads the pointing hand cursor's tiles into a free VRAM slot and returns the
/// slot.
s32 PointingHand_LoadIntoFreeSlotFar(void);
/// Clears a window's contents: redraws it empty and releases its sprites.
void WindowSprite_PrepareForRedrawFar(void *window);
/// Shows a message in a window centred on the screen, waits until it is typed
/// out and closes it, as its flags say.
void FieldMessage_ShowFar(s32 message, s32 flags);
/// The debug flag editor: shows 16 rows of 16 flags a page and takes input
/// until closed, starting on page 8.
void DebugFlagEditor_RunFar(void);
/// Runs the field menu until the player picks something that leaves it.
void FieldMenu_RunFar(void);
/// The debug scene selector: picks a scene and entrance and travels there.
void DebugSceneSelect_RunFar(void);
/// Closes the party status window and frees its heap block.
void PartyStatus_CloseFar(void);
/// Sets the decode flags the next message is decoded with (Message_Decode).
void Message_SetDecodeFlagsFar(s32 flags);
/// Fills `spans`, a page of the screen transition's window edges, with the
/// windows' horizontal extent on each of the 160 scan lines.
void ScanlineWindow_FillSpansFar(u16 *spans);
/// As Text_GetMessageSizeFar, but measuring every page of the message.
void Text_GetMessageSizeAllPagesFar(s32 message, s32 *left, s32 *top,
                                    u32 *width, u32 *height);
/// Decodes a message and fits a window for its first page on the screen near
/// tile (*left, *top), returning its place and size in tiles.
void Text_GetMessageSizeFar(s32 message, s32 *left, s32 *top, u32 *width,
                            u32 *height);
/// Returns the portrait of a speaker, a character or a sprite, or -1 when it
/// has none.
s32 Portrait_GetForSpeakerFar(s32 speaker);
/// Decodes message into text as a zero-terminated string of 16-bit codes, with
/// at most capacity entries, keeping style codes with their operands.
void Message_DecodeRawFar(s32 message, u16 *text, s32 capacity);
/// Copies icon `icon` into a free VRAM slot and shows it as a 16x16 sprite at
/// (x, y) in `window`.
struct WindowSprite *WindowSprite_CreateLoadedFar(s32 icon, s32 layers,
                                                  struct Window *window, s32 x,
                                                  s32 y);
/// Selects a shop command (Buy, Sell, Artifacts, Repair) on the bottom row,
/// starting on `selection`, and returns the one chosen.
s32 Menu_SelectShopCommandFar(s32 selection);
/// Selects a Sanctum service (Revive, Cure Poison, Repel Evil, Remove Curse) on
/// the bottom row, starting on `selection`.
s32 Menu_SelectSanctumServiceFar(s32 selection);
/// Decodes message and returns its text's width and height in pixels.
void Text_MeasureMessageFar(s32 message, s32 *width, s32 *height);
/// Sets the window colours for a window colour (0 to 23) and brightness (0 to
/// 14, 7 neutral).
void WindowColor_ApplyFar(s32 color, s32 brightness);
/// Collects the party's commands for a turn into `commands`, one for each of
/// the `commandCount` units in `actors`.
s32 Battle_CollectPartyCommandsFar(struct BattleActionRecord *commands,
                                   void *actors, s32 commandCount);
/// Opens the area-name window, which closes after ninety frames.
void AreaNameWindow_ShowFar(s32 scene);
/// Handles Save your quest, Sleep and Change settings from the Pause menu. The
/// field passes it a 0 it does not read.
void PauseMenu_RunFar(s32 unused);
/// Announces a summon the party can now use.
void Summon_ShowLearnedMessageFar(s32 summon);

/// Resets the pen and text style of the first idle message.
void Message_ResetIdleFar(void);
/// Selects Yes or No at (x, y).
s32 YesNoMenu_RunAtFar(s32 x, s32 y, s32 selection);

/// Draws message centred in a 192-pixel canvas of one byte a pixel, as the
/// credits draw their scrolling lines; they pass an alignment it does not read.
s32 Text_DrawMessageToCanvasFar(s32 message, u8 *canvas, s32 unused);

/// Opens a portrait window at tile (x, y) showing the speaker's portrait.
struct Window *Portrait_OpenWindowFar(s32 speaker, s32 portraitOption, s32 x,
                                      s32 y);

/// Draws a byte string as tiles in a window at pixel (offsetX, offsetY) of its
/// interior.
void Text_DrawStringAtOffsetFar(const u8 *text, s32 window, s32 offsetX,
                                s32 offsetY);
/// Draws a byte string into a window's tilemap at pixel (x, y) of its
/// interior, rounded down to tiles.
void Text_DrawStringInWindowFar(u8 *text, s32 window, u32 x, u32 y);
/// Draws a number, width digits as String_FormatNumber takes it, as tiles in a
/// window at pixel (x, y) of its interior.
void Text_DrawNumberAtOffsetFar(s32 value, s32 width, s32 window, s32 x, s32 y);
/// As Text_DrawNumberAtOffsetFar, into the window's tilemap.
void Text_DrawNumberInWindowFar(s32 value, s32 width, s32 window, s32 x, s32 y);
/// Sets the text colour (0 to 15) of drawn text.
s32 Text_SetColorFar(s32 color);
/// Ability_BuildIconTiles, for an overlay.
void Ability_BuildIconTilesFar(s32 action, s32 withBase, s32 *slot, s32 *tile,
                               s32 reuse);
/// Draws a divider from (x1, y1) to (x2, y2), one of which must agree, joining
/// it to the frame and divider tiles already on the tilemap.
void Window_DrawDividerLineFar(s32 window, s32 x1, s32 y1, s32 x2, s32 y2);
/// Reloads the portrait picture for the key into a portrait slot with that
/// slot's palette (PORTRAIT_PALETTE + slot), reusing the slot's VRAM cache
/// entry.
void Portrait_ReloadFar(s32 slot, s32 key, s32 unused);

#endif
