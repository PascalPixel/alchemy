/// @file
/// Window construction, text drawing and selection interfaces.

/// Sets up the window system for the field and the other screens outside
/// battle: a fresh work block, a blank tilemap and the per-frame upload and
/// update callbacks.
void UiWork_InitializeWithResourceCountersFar(void);
/// Sets up the window system for battle; kind set reserves the glyph sprites'
/// tiles, which Battle_RunEncounter asks for only during the Djinn lesson.
void UiWork_InitializeFar(s32 kind);
#ifndef OWN_UiWindow_CreateFar
/// Opens a window at tile (x, y), width by height tiles, and waits for it to
/// zoom open unless attrs has UI_WINDOW_INSTANT. Returns NULL when every window
/// slot is taken.
struct UiWindow *UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height,
                                    s32 attrs);
#endif
/// Clears a window and closes it, at once with release set, else over four
/// frames without waiting; UiWork_IsIdle says when its slot is free. NULL is
/// ignored.
void UiWork_FinalizeFar(struct UiWindow *work, s32 release);
/// Decodes message into buf with no length limit, so buf must hold the whole
/// message and its terminator.
void Ui_AdjustValueWithoutLimitFar(s32 message, u16 *buf);
/// Decodes message into text as 16-bit codes, at most capacity entries with the
/// terminator, leaving names and numbers as their codes.
void UiText_DecodeMessageFar(s32 message, u16 *text, s32 capacity);
/// Decodes message into destination with names and numbers written out, and
/// returns its length; it uses up the message arguments and overwrites the ring
/// of decoded text.
s32 UiText_CopyMessageStringFar(s32 key, u16 *destination, u32 capacity);
/// Opens a window sized to a message near tile (x, y) and starts typing into
/// it, returning once the window is open. packed holds the window options in
/// its low bits and the speaker's voice sound in bits 16 to 27.
struct UiWindow *UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y,
                                             u32 packed);
/// Shows a message in a window, waits until it's typed out and closes it.
/// Unless flags move it lower, the window goes above or below the player's
/// character, away from where it stands.
void UiText_ShowPositionedMessageAndWaitFar(s32 no, s32 flags);
/// Returns 1 once every message channel has finished typing its text, even if
/// its window is still open.
s32 UiWork_IsCompleteFar(void);
/// Returns 1 once window has closed and freed its slot; a new window in that
/// slot reads as still open.
s32 UiWork_IsIdleFar(struct UiWindow *window);
/// Sets the effects the next messages are decoded with until a message window
/// starts typing: bit 0 makes halting speech, broken off with an ellipsis after
/// the first word.
void UiWork_SetBusyFlagsFar(s32 flags);
/// Redraws a window's empty frame and releases the sprites it carries, leaving
/// a text canvas alone.
void RenderOutput_PrepareForRedrawFar(struct UiWindow *window);
/// Clears every tile a pixel rectangle inside a window touches to the window's
/// fill.
void UiWindow_ClearInteriorTilesFar(const struct UiWindow *window, u32 left,
                                    u32 top, u32 right, u32 bottom);
/// Draws a horizontal or vertical divider inside a window between two tile
/// corners, joining the frame and crossing the lines it meets.
void UiWindow_DrawDividerLineFar(struct UiWindow *win, u32 x1, u32 y1, u32 x2,
                                 u32 y2);
/// Decodes message arg0 and draws it all at once into the text canvas arg1 at
/// pixel (arg2, arg3), overwriting the ring of decoded text.
void UiText_DrawResourceFar(s32 arg0, struct UiWindow *arg1, s32 arg2,
                            s32 arg3);
#ifndef OWN_UiText_DrawCharacterAtOffsetFar
/// Decodes the message numbered character and draws it as tiles in a window's
/// interior at pixel (offsetX, offsetY), offsetY rounded down to a tile row;
/// nothing is drawn off the tilemap.
void UiText_DrawCharacterAtOffsetFar(s32 character,
                                     const struct UiWindow *position,
                                     u32 offsetX, u32 offsetY);
#endif
/// Draws a byte string of at most 255 characters into the text canvas arg1 at
/// pixel (arg2, arg3).
void UiText_DrawStringFar(const u8 *text, struct UiWindow *arg1, s32 arg2,
                          s32 arg3);
/// Draws a byte string of at most 255 characters as tiles in a window's
/// interior at pixel (offsetX, offsetY).
void UiText_DrawStringAtOffsetFar(const u8 *text, const struct UiWindow *work,
                                  s32 offsetX, s32 offsetY);
/// Draws a byte string straight into a window's tilemap, (x, y) being pixels of
/// its interior rounded down to tiles.
void UiText_DrawStringInWindowFar(const u8 *text, struct UiWindow *arg1, u32 x,
                                  u32 y);
/// Draws the number arg0, arg1 digits wide as UiText_FormatNumber pads it, into
/// the text canvas arg2 at pixel (arg3, arg4).
void UiText_DrawNumberFar(s32 arg0, s32 arg1, struct UiWindow *arg2, s32 arg3,
                          s32 arg4);
/// Draws a number, padded as UiText_FormatNumber pads it, as tiles in a
/// window's interior.
void UiText_DrawNumberAtOffsetFar(s32 arg0, s32 arg1,
                                  const struct UiWindow *arg2, s32 arg3,
                                  s32 arg4);
/// Draws a number, padded as UiText_FormatNumber pads it, into a window's
/// tilemap.
void UiText_DrawNumberInWindowFar(s32 arg0, s32 arg1, struct UiWindow *arg2,
                                  s32 arg3, s32 arg4);
/// Sets the colour (0 to 15) text is drawn in from now on; the menus colour HP
/// figures with it and put colour 15 back after.
void UiWork_SetParameterNibbleFar(s32 param);
/// Shows the picture in VRAM cache slot as a sprite in a window, (offsetX,
/// offsetY) pixels in from its inner corner. Returns NULL, releasing the slot,
/// when every output is in use.
struct RenderOutput *RenderOutput_CreateFar(s32 slot, s32 flags,
                                            struct UiWindow *input, s32 offsetX,
                                            s32 offsetY);
/// Loads a resource into a VRAM slot and shows it as a 16 by 16 sprite at (x,
/// y) in a window, as the field menus show their item and Psynergy icons.
/// Returns NULL when it can't be loaded.
struct RenderOutput *RenderOutput_CreateFromResourceFar(s32 mode, s32 resource,
                                                        struct UiWindow *input,
                                                        s32 x, s32 y);
/// Shows a character's 32 by 32 portrait in a window, in portrait slot side or
/// the first free one (CreateSideObject). Returns NULL when the character has
/// no portrait or both slots are taken.
struct RenderOutput *SideObject_CreateFar(s32 object_kind, s32 position,
                                          s32 side, struct UiWindow *arg3,
                                          s32 arg4, s32 arg5);
/// Reloads the portrait in slot so it shows character, keeping its sprite, as
/// the field menus change a portrait already on screen.
void Ui_LoadCharacterEntryForSlotFar(u32 slot, s32 character, s32 value);
/// Opens a portrait window at tile (x, y) for the speaker arg0, at once.
/// Returns 0 when the speaker has no portrait.
struct UiWindow *UiWindow_CreateWithSideObjectFar(s32 arg0, s32 arg1, s32 x,
                                                  s32 y);
/// Closes at once the portrait window showing speaker entry, if one is open.
void UiWork_FinalizeEntityMatchingLocalizedIdFar(u32 entry);
/// Decodes message no and fits a window for its first page on the screen near
/// tile (*x, *y), returning its place and size in tiles. Returns 0 for an empty
/// message.
s32 UiText_GetResourceDimensionsFar(s32 no, s32 *x, s32 *y, u32 *width,
                                    u32 *height);
/// As UiText_GetResourceDimensions, but measuring every page of the message;
/// the field's speaker windows use it.
s32 UiText_GetResourceDimensionsAltFar(s32 no, s32 *x, s32 *y, u32 *width,
                                       u32 *height);
/// Empties the table of message arguments.
void UiWork_ClearValueNameTablesFar(void);
/// Pushes an argument of kind (MESSAGE_ARGUMENT_) for the next message decoded,
/// dropped when all eight places are taken; arguments of one kind are read in
/// the order they were pushed.
void UiWork_PushValueSlotFar(u32 value, u32 kind);
/// Opens the party status window with flags in its own heap block and draws it;
/// battle opens it erased, for the battle to draw.
void UiWindow_CreateWithLayoutBoundsFar(s32 flags);
/// Draws the party status window: names, HP and optionally PP with their bars,
/// and in battle the standby Djinn of each element.
void PartyStatus_DrawFar(s32 flags);
/// Closes the party status window and frees its heap block.
void PartyStatus_CloseFar(void);
/// Closes each message window whose text has ended, then waits ten frames;
/// windows still typing stay open.
void UiWork_FinalizePendingCoreFar(void);
/// Closes every window, waiting only for the message windows to finish closing.
void UiWork_DrainPendingFar(s32 unused);
/// Runs the party's battle commands for a round into out, one per unit in in,
/// and returns how many it wrote: 1 when the party flees, -1 when the link
/// fails.
s32 Battle_CollectPartyCommandsFar(struct BattleActionRecord *out, u16 *in,
                                   s32 count);
/// Shows a message in the battle message window and waits until it's typed out.
void UiText_ShowMessageAndWaitCoreFar(s32 argument);
/// Starts typing a message into the battle message window without waiting,
/// opening the window along the bottom of the screen if it's closed.
void UiText_PrepareMessageWorkFar(s32 argument);
/// Returns the portrait of a character, or the sprite of another speaker, or -1
/// when the speaker has none.
s32 Localization_LookupEntryIdFar(u32 value);
/// Runs the field menu until the player picks a screen that leaves it, and
/// returns that screen's result; Map_Run ignores it.
s32 Menu_RunTopSelectionFar(void);
/// Runs the title menu built from the save files, New Game to Send, and returns
/// the chosen SaveSlotAction, or -1 when cancelled.
s32 Menu_SelectSaveSlotActionFar(void);
/// Lets the player choose the Send screens' transfer method or password level,
/// and returns the choice, or -1 when cancelled.
s32 Menu_SelectResourceLayoutFar(s32 mode);
/// Shows the area's name near the top of the screen for 90 frames, counted down
/// by UiTimedNotice_Tick.
void UiTimedNotice_CreateFar();
/// Closes the area name at once, if it's shown.
void UiTimedNotice_CloseIfActiveFar(void);
/// Shows a message centred across the screen, yOffset tiles lower, waits until
/// it's typed out and closes it.
void UiText_ShowCenteredMessageFar(s32 message, s32 mode, s32 yOffset);
/// Closes the battle message window at once, so the next battle message opens a
/// new one.
void UiWork_FinalizeSharedSlotFar(void);
/// Clears the first idle message channel's pen, style and canvas, as auto
/// battle does after each action.
void UiWork_ResetFreeChannelFar(void);
/// Sets up a sprite as the right-pointing arrow and returns its tile slot in
/// *slot, for UiTextResource_Release to free.
void UiTextResource_InitializeFar(struct MenuSprite *object, s32 *slot);
/// Puts an arrow sprite at screen pixel (x, y) for this frame only, so callers
/// set it every frame it shows.
void UiTextResource_SetPositionFar(struct MenuSprite *obj, s32 x, s32 y);
/// Frees the tile slot of an arrow sprite.
void UiTextResource_ReleaseFar(s32 value);
/// Draws an item's icon, from its inventory code, into the glyph work buffer
/// with no frame or marks.
void Ui_PrepareTransferForItemFar(s32 code);
/// Draws a string of 16-bit character codes into a text canvas at pixel (x, y)
/// at once, running its colour, outline and new line codes. NULL draws the ring
/// of decoded text.
void UiText_RenderWideStringAtOffsetFar(u16 *text, struct UiWindow *window,
                                        s32 x, s32 y);
/// Writes each scan line's window frame extent into a page of the display
/// transition's window table, as a window transition runs.
void UiWindow_FillScanlineSpansFar(void *dst);
/// Redraws a window's empty frame at its current rectangle.
void RenderOutput_RedrawSavedRectFar(struct UiWindow *window);
/// Releases every sprite in a list of render outputs, or a window's, and
/// empties it; NULL is ignored.
void RenderOutput_ClearListFar(void *work);
/// Writes value into a window's tilemap at tile (x, y) of its interior, with
/// the palette mode names (TILEMAP_ENTRY_); y -1 is the frame's top row.
void UiWindow_SetTilemapEntryFar(struct UiWindow *window, s32 value, s32 x,
                                 s32 y, u32 mode);
#ifdef UBFIX
/// Debug: the map selector, returning nothing under UBFIX.
void Menu_RunSelectionFar(void);
#else
/// Debug: the map selector, where A warps to the chosen map and warp
/// destination and B closes it. The field opens it from the Start menu with B
/// held. Its result is undefined, and nothing reads it.
s16 Menu_RunSelectionFar(void);
#endif
/// Debug: shows the game flags 256 at a time as a grid the player can toggle,
/// starting on page 8. Returns 0.
s32 Menu_RunSelectionWithCursorObjectFar(void);
#ifndef OWN_Resource_LoadByModeIntoSlotFar
/// Loads the icon of an entry of kind mode into sprite tile slot result, or a
/// free one when it's -1, and returns the slot, or -1 when none is free.
s32 Resource_LoadByModeIntoSlotFar(s32 mode, s32 value, s32 result, s32 option);
#endif
/// Marks the text tiles the tilemap shows and frees those it no longer shows;
/// the field and battle menus call it as they redraw.
void UiWindow_MarkVisibleTileAttributesFar(void);
/// Highlights the party status columns of the units in ids, ended by 0xff,
/// while their commands are chosen. Returns 0.
s32 BattleLayout_HighlightPartyPanelsFar(u16 *ids);
/// Copies icon code into a free VRAM slot and shows it as a 16 by 16 sprite at
/// (x, y) in a window, as the shops show their items. Returns NULL when no slot
/// is free.
struct RenderOutput *RenderOutput_CreateLoadedFar(s32 code, s32 layers,
                                                  struct UiWindow *input, s32 x,
                                                  s32 y);
/// Runs the Save command: the player picks a file, confirms saving over it and
/// the game is saved there. Returns 0, -1 when cancelled, or a SAVE_ERROR_
/// after its message.
s32 Save_WriteSelectedSlotFar(void);
/// Lets the player pick a file in save menu mode arg and loads it as the
/// current file, as the title's Continue does; it returns as the Save command
/// does.
s32 Save_LoadRecordIntoWorkFar(s32 arg);
/// Runs the file menu's Copy, writing a chosen file to the first empty one. Its
/// errors are shown before it returns them.
s32 Save_CopyFileFar(void);
/// Runs the file menu's Erase, deleting a chosen file once the player confirms
/// it. Returns 0 also when the player answers No.
s32 Save_EraseSelectedFileFar(void);
/// Writes the save stamp into the current file's first record, as the link
/// lobby saves its battle results. Returns 0, -1 with no file chosen, or a
/// SAVE_ERROR_.
s32 Save_ProcessSelectedFileFar(void);
/// Reads the save summaries for the title menu and returns how many files there
/// are, plus 100 when every file has its send flag; it also decides whether
/// Battle and Send are offered.
s32 Save_ScanRecordFlagsFar(void);
/// Runs the name entry screen for member entry. Returns 0, or -1 when
/// cancelled; the name changes only when the player ends on End.
s32 NameEntry_EditOwnerNameFar(s32 entry);
/// Debug: pages through the item, Psynergy and status icons. Returns 0.
s32 DebugMenu_BrowseIconsFar(void);
/// Debug: steps through the character and sprite portrait maps, showing each
/// entry's portrait and label. Returns 0.
s32 DebugMenu_BrowsePortraitsFar(void);
/// Shows `The <element> Djinni <name> joined <member>.` while the join fanfare
/// plays, until it ends or a button is pressed; djinn is the Djinni's number in
/// its element.
void Djinn_ShowJoinedMessageFar(s32 pc, s32 element, s32 djinn);
/// Sets the window colours for a hue (0 to 23) and a brightness (0 to 14, 7
/// neutral) without touching the saved settings.
void PaletteGlow_UpdateFar(s32 phase, s32 brightness);
/// Runs Save, Sleep and Options from the Start menu; a cancelled screen reopens
/// the menu. Returns -1 when the menu itself is cancelled, otherwise 0.
s32 Menu_RunWorkspaceResultLoopFar();
/// Lets the player choose Buy, Sell, Artifacts or Repair, starting on
/// selection. Returns the position chosen, or -1 when cancelled.
s32 Menu_SelectShopCommandFar(s32 selection);
/// Lets the player choose a sanctum's Revive, Cure Poison, Repel Evil or Remove
/// Curse, starting on selection. Returns the position chosen, or -1 when
/// cancelled.
s32 Menu_SelectSanctumServiceFar(s32 selection);
/// Asks Yes or No, at the bottom of the screen when low is nonzero, returning
/// CONFIRM_YES or CONFIRM_NO; cancelling answers CONFIRM_NO.
s32 Menu_RunConfirmSelectionFar(s32 low, s32 message, s32 w, s32 selection);
/// Lets the player choose Coins or Lucky Medals, as Tolbi's spring asks.
/// Returns the position chosen, or -1 when cancelled.
s32 Menu_SelectCoinsOrMedalsFar(s32 selection);
/// Asks what to do in Colosso, "Descriptions", "Cheer" or "Nothing", with a
/// pointing hand. Returns 0 to 2, or -1 when cancelled.
s32 PartyTalkMenu_ChooseFar(s32 unused);
/// Asks Yes or No from tile (x, y), returning CONFIRM_YES or CONFIRM_NO;
/// cancelling answers CONFIRM_NO.
s32 Menu_RunConfirmSelectionAtFar(s32 x, s32 y, s32 selection);
/// Loads the pointing hand cursor's tiles into a free VRAM slot, unchecked, and
/// returns the slot for Resource_ResetEntry to free.
s32 Resource_LoadFixedBlockBIntoFreeSlotFar(void);
/// Decodes message no and returns its text's width and height in pixels,
/// leaving its arguments for the message to be shown.
void UiText_MeasureResourceEntriesFar(s32 no, s32 *x, s32 *y);
/// Switches a rectangle of screen tiles to palette 15, or 14 when alt's low bit
/// is clear: the menus' row highlight (Ui_SetRectHighlight).
void UiWindow_SetPaletteBitRectFar(s32 x, s32 y, s32 width, s32 height,
                                   s32 alt);
/// Builds the Psynergy icon of a battle action, as UiIcon_BuildAbilityIconTiles
/// does.
void Ability_LoadGlyphFar(s32 action, s32 with_base, s32 *slot, s32 *tile,
                          s32 reuse);
/// Copies the UI's ten digit tiles into dst twice, first recoloured and then as
/// they are, for the menus' page numbers.
void Graphics_ExpandVramTilesByColorTableFar(u16 *dst);
/// Frees the second range of text tiles and, with flag set, lets text take them
/// too, doubling the room for text to 256 tiles while a menu is open.
void UiWork_SetAltFlagAndClearTableFar(s32 flag);
/// Opens the field command menu and moves its cursor to a command's icon, as
/// the world map's scenes show it picking Djinn. Returns the icon's position.
s32 Menu_AnimateSelectionToEntryFar(s32 arg0, s32 arg1);
/// Returns the first entry of gUi_FixedTileBlocks, whatever no is.
s32 Ui_GetTableWordZeroFar(s32 no);
/// Types a message into an open window from its top left, without voice blips.
/// Returns the typing channel as an integer, or 0 when nothing could start.
s32 UiText_OpenEntryMessageFar(struct UiWindow *window, s32 argument);
/// Returns how many save files there are, leaving out the files saved at the
/// end of the game when flag is nonzero, or SAVE_ERROR_NO_MEMORY.
s32 Save_CountRecordsExcludingFlaggedFar(s32 flag);
/// Draws an empty window frame at tile (x, y), width by height tiles, and frees
/// the text tiles it covers.
void UiWindow_DrawFrameFar(s32 x, s32 y, u32 width, u32 height);
/// Erases a window's rectangle from the tilemap and frees the text tiles it
/// showed.
void UiWindow_EraseBorderRectFar(s32 x, s32 y, u32 width, u32 height);
/// Draws the menus' two page arrows into the tiles at VRAM offset, sliding
/// outward and back over eight frames.
void Link_DrawShiftedTilePairFar(s32 offset);
/// Shows `<member> joined your party.` with the member's portrait while the
/// join fanfare plays, until it ends or a button is pressed.
void Party_ShowJoinedMessageFar();
/// Shows `<first> and <second> joined your party.` with both portraits while
/// the join fanfare plays.
void Party_ShowPairJoinedMessageFar(s32 first, s32 second);
