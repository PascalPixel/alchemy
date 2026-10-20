/// @file
/// Shops, inns and the cursors used by their screens.
/// Far-call interfaces; their ordered jump stubs live in
/// games/tbs/src/system/far_call/SHOP.S.

s32 Battle_ApplyPresetItemsAndFlagsFar(s32 unused);
s32 Shop_RunFar(s32 row, s32 keeper_id);
s32 Shop_ConfirmActFar(s32 unitId);
s32 Inn_CheckInFar(s32 mode, s32 objectId);
void ShopCursor_AdvanceFar(struct ShopCursor *cursor);
void ShopCursor_MoveTowardTargetFar(struct ShopCursor *cursor);
void Shop_SetCursorFar(struct ShopCursor *cursor, s32 targetX, s32 targetY,
                       s32 kind);
void ShopCursor_SetPositionImmediateFar(struct ShopCursor *cursor, s32 x,
                                        s32 y);
void Shop_FillSelectorFar(s32 count, s32 selector, u8 *base);
void Shop_SaveAndTintFieldPaletteFar(s32 layer);
void Shop_RestoreFieldPaletteFar(void);
s32 Shop_PickUnitItemFar(s32 *selected_unit, s32 *selectedItem);
void AudioCommand_WaitForStateByteClearFar(void);
void Inn_PlaySleepFar(s32 room_price);
