/* FarCall_ShopTable: one entry per 8-byte stub, in table order. */
FAR_CALL(Battle_ApplyPresetItemsAndFlagsFar, Battle_ApplyPresetItemsAndFlags, s32, (s32 unused))
FAR_CALL(Shop_RunFar, Shop_Run, s32, (s32 row, s32 keeper_id))
FAR_CALL(Shop_ConfirmActFar, Shop_ConfirmAct, s32, (s32 unit_id))
FAR_CALL(Inn_CheckInFar, Inn_CheckIn, s32, (s32 mode, s32 object_id))
FAR_CALL(ShopCursor_AdvanceFar, ShopCursor_Advance, void, (struct ShopCursor * cursor))
FAR_CALL(
	ShopCursor_MoveTowardTargetFar, ShopCursor_MoveTowardTarget, void, (struct ShopCursor * cursor))
FAR_CALL(Shop_SetCursorFar, Shop_SetCursor, void,
	(struct ShopCursor * cursor, s32 target_x, s32 target_y, s32 kind))
FAR_CALL(ShopCursor_SetPositionImmediateFar, ShopCursor_SetPositionImmediate, void,
	(struct ShopCursor * cursor, s32 x, s32 y))
FAR_CALL(Shop_FillSelectorFar, Shop_FillSelector, void, (s32 count, s32 selector, u8 *base))
FAR_CALL(Shop_RestoreSceneTilesFar, Shop_RestoreSceneTiles, void, (s32 layer))
FAR_CALL(Shop_InitEffectFar, Shop_InitEffect, void, (void))
FAR_CALL(Shop_PickUnitItemFar, Shop_PickUnitItem, s32, (s32 * selected_unit, s32 *selected_item))
FAR_CALL(AudioCommand_WaitForStateByteClearFar, AudioCommand_WaitForStateByteClear, void, (void))
FAR_CALL(Inn_PlaySleepFar, Inn_PlaySleep, void, (s32 room_price))
