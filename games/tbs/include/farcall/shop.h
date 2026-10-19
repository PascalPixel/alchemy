/// @file
/// Shops, inns and the cursors used by their screens.
/// Far-call interfaces; their ordered jump stubs live in
/// games/tbs/src/system/far_call/SHOP.S.

/// Far call to Battle_ApplyPresetItemsAndFlags.
s32 Battle_ApplyPresetItemsAndFlagsFar(s32 unused);
/// Far call to Shop_Run.
s32 Shop_RunFar(s32 row, s32 keeper_id);
/// Far call to Shop_ConfirmAct.
s32 Shop_ConfirmActFar(s32 unitId);
/// Far call to Inn_CheckIn.
s32 Inn_CheckInFar(s32 mode, s32 objectId);
/// Far call to ShopCursor_Advance.
void ShopCursor_AdvanceFar(struct ShopCursor *cursor);
/// Far call to ShopCursor_MoveTowardTarget.
void ShopCursor_MoveTowardTargetFar(struct ShopCursor *cursor);
/// Far call to Shop_SetCursor.
void Shop_SetCursorFar(struct ShopCursor *cursor, s32 targetX, s32 targetY,
                       s32 kind);
/// Far call to ShopCursor_SetPositionImmediate.
void ShopCursor_SetPositionImmediateFar(struct ShopCursor *cursor, s32 x,
                                        s32 y);
/// Far call to Shop_FillSelector.
void Shop_FillSelectorFar(s32 count, s32 selector, u8 *base);
/// Far call to Shop_RestoreSceneTiles.
void Shop_RestoreSceneTilesFar(s32 layer);
/// Far call to Shop_InitEffect.
void Shop_InitEffectFar(void);
/// Far call to Shop_PickUnitItem.
s32 Shop_PickUnitItemFar(s32 *selected_unit, s32 *selectedItem);
/// Far call to AudioCommand_WaitForStateByteClear.
void AudioCommand_WaitForStateByteClearFar(void);
/// Far call to Inn_PlaySleep.
void Inn_PlaySleepFar(s32 room_price);
