/// @file
/// Shops, inns and the cursors used by their screens.

/// Opens the debug shop with 200000 coins, broken equipment to repair and
/// ailing members for the sanctum.
s32 Shop_OpenDebugShopFar(s32 unused);
/// Runs a shop counter until the player leaves: row picks the stock and
/// keeper_id the object whose portrait the lines show.
s32 Shop_RunFar(s32 row, s32 keeper_id);
/// Runs a sanctum's welcome, services and farewell; unitId is the keeper, whose
/// portrait the lines show. Returns 0.
s32 Sanctum_RunFar(s32 unitId);
/// Runs an inn's counter, the party sleeping if the player pays: mode is the
/// inn's price row and objectId the innkeeper.
s32 Inn_CheckInFar(s32 mode, s32 objectId);
/// Moves the cursor one frame along its straight glide to its target; the
/// menus' cursor tasks call it every frame.
void MenuCursor_AdvanceFar(struct MenuCursor *cursor);
/// Closes a quarter of the cursor's distance to its target on each axis, at
/// least a pixel.
void MenuCursor_MoveTowardTargetFar(struct MenuCursor *cursor);
/// Starts a glide of the cursor to (targetX, targetY) over kind frames, as name
/// entry and Colosso's choice use it.
void MenuCursor_SetFar(struct MenuCursor *cursor, s32 targetX, s32 targetY,
                       s32 kind);
/// Puts the cursor and its sprite at (x, y) at once, as a menu opens.
void MenuCursor_SetPositionImmediateFar(struct MenuCursor *cursor, s32 x,
                                        s32 y);
/// Colours the first count segments of a quantity bar's tiles in base, the
/// right pixel of each in colour selector.
void QuantityBar_FillFar(s32 count, s32 selector, u8 *base);
/// Saves the field's colours and fades to layer over 16 frames, before a
/// sanctum or Djinni capture effect.
void Shop_SaveAndTintFieldPaletteFar(s32 layer);
/// Fades back to the field colours saved before the effect, over 16 frames.
void Shop_RestoreFieldPaletteFar(void);
/// Lets the player pick a member and one of their droppable items, as when
/// every bag is full. Returns -1 when cancelled.
s32 ItemPicker_RunFar(s32 *selected_unit, s32 *selectedItem);
/// Waits a frame at a time until the sound driver has finished its last
/// command, such as a fanfare.
void AudioCommand_WaitForStateByteClearFar(void);
/// Charges room_price, heals every member still standing and plays the night at
/// the inn.
void Inn_PlaySleepFar(s32 room_price);
