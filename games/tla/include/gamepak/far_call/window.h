#ifndef ALCHEMY_FAR_CALL_WINDOW_H
#define ALCHEMY_FAR_CALL_WINDOW_H

/// @file
/// The far calls into the window bank, as the other banks' code calls them.

#include "types.h"

struct BattleActionRecord;
struct Window;
struct WindowSprite;
void WindowSystem_InitBattleFar(s32 mode);
/// Opens the party status window, sized by PartyStatus_BuildLayoutBounds, and
/// draws it, holding the tilemap upload meanwhile.
void PartyStatus_CreateWindowFar(s32 layout);
/// Returns 0 if any of the three messages has a window still writing (state is
/// zero), else 1.
s32 Message_AllTypedFar(void);
/// Returns gWindow_FixedTileBlocks[0], whatever no is.
s32 WindowSprite_GetPointerTilesFar(s32 index);
void WindowSystem_InitFieldFar(void);
/// Opens a window at tile (x, y) of width by height tiles in the first free
/// window slot, and returns it, or NULL when all slots are in use.
struct Window *Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 mode);
void Window_CloseFar(struct Window *window, s32 mode);
/// Decodes a message and draws it as tiles in a window at pixel (offsetX,
/// offsetY) of its interior.
void Text_DrawMessageAsTilesFar(s32 message, struct Window *window, s32 x,
                                s32 y);
void Message_CloseFinishedWindowsFar(void);
/// Selects Yes or No (icons 5 and 6).
s32 YesNoMenu_RunFar(s32 useBottom, s32 name, s32 width, s32 third);
s32 Resource_LoadFixedBlockBIntoFreeSlotFar(void);
void WindowSprite_PrepareForRedrawFar(void *window);
void FieldMessage_ShowFar(s32 message, s32 flags);
/// Shows 16x16 flags per page and takes input until closed, starting on page 8.
void DebugFlagEditor_RunFar(void);
/// Runs the field menu until the player picks something that leaves it, and
/// returns the chosen screen's result.
void FieldMenu_RunFar(void);
/// Debug: the scene selector.
void DebugSceneSelect_RunFar(void);
void PartyStatus_CloseFar(void);
/// Sets the decode flags the next message is decoded with (Message_Decode).
void Message_SetDecodeFlagsFar(s32 flags);
/// Fills the 0x140-byte row buffer with blank tiles, or builds it from the
/// window tiles' map through the cell picture routine copied to RAM.
void ScanlineWindow_FillSpansFar(u16 *pairs);
/// As Text_GetMessageSizeFar, but measuring every page of the message.
void Text_GetMessageSizeAllPagesFar(s32 message, s32 *left, s32 *top,
                                    u32 *width, u32 *height);
/// Decodes a message and fits a window for its first page on the screen near
/// tile (*left, *top), returning its place and size in tiles.
void Text_GetMessageSizeFar(s32 message, s32 *left, s32 *top, u32 *width,
                            u32 *height);
s32 Portrait_GetForSpeakerFar(s32 sprite);
/// Decodes message into text as a zero-terminated string of 16-bit codes, with
/// at most capacity entries, keeping style codes with their operands.
void Message_DecodeRawFar(s32 message, u16 *buffer, s32 length);
struct WindowSprite *WindowSprite_CreateLoadedFar(s32 item, s32 mode,
                                                  struct Window *window, s32 x,
                                                  s32 y);
/// Selects a shop command from icons 0x11-0x14, centred on row 17, showing the
/// selected command's name in a window.
s32 Menu_SelectShopCommandFar(s32 action);
/// Selects a sanctum service from icons 0x19-0x1c, centred on row 17.
s32 Menu_SelectSanctumServiceFar(s32 prev);
void Text_MeasureMessageFar(s32 message, s32 *width, s32 *height);
void WindowColor_ApplyFar(s32 first, s32 second);
s32 Battle_CollectPartyCommandsFar(struct BattleActionRecord *entries,
                                   void *buffer, s32 mode);
/// Opens the area-name window, which closes after ninety frames.
void AreaNameWindow_ShowFar(s32 scene);
/// Runs the save screen's selection until a choice completes.
void PauseMenu_RunFar(s32 mode);
/// Announces a summon the party can now use.
void Summon_ShowLearnedMessageFar(s32 value);

void Message_ResetIdleFar(void);
/// Selects Yes or No at (x, y).
s32 YesNoMenu_RunAtFar(s32 x, s32 y, s32 selection);

/// Draws message centred in a 192-pixel canvas of one byte a pixel, as the
/// story crawl draws its lines.
s32 Text_DrawMessageToCanvasFar(s32 message, u8 *canvas, s32 align);

struct Window *Portrait_OpenWindowFar(s32 resource, s32 x, s32 y, s32 mode);

#endif
