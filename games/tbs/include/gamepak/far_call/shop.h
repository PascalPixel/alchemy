#ifndef ALCHEMY_FAR_CALL_SHOP_H
#define ALCHEMY_FAR_CALL_SHOP_H

/// @file
/// Shops, inns and the cursors used by their screens.

s32 Shop_OpenDebugShopFar(s32 unused);
s32 Shop_RunFar(s32 row, s32 keeper_id);
s32 Sanctum_RunFar(s32 unitId);
s32 Inn_CheckInFar(s32 mode, s32 actorId);
void MenuCursor_AdvanceFar(struct MenuCursor *cursor);
void MenuCursor_MoveTowardTargetFar(struct MenuCursor *cursor);
void MenuCursor_SetFar(struct MenuCursor *cursor, s32 targetX, s32 targetY,
                       s32 kind);
void MenuCursor_SetPositionImmediateFar(struct MenuCursor *cursor, s32 x,
                                        s32 y);
void QuantityBar_FillFar(s32 count, s32 selector, u8 *base);
void Shop_SaveAndTintFieldPaletteFar(s32 layer);
void Shop_RestoreFieldPaletteFar(void);
s32 ItemPicker_RunFar(s32 *selected_unit, s32 *selectedItem);
void Sound_WaitForJingleFar(void);
void Inn_PlaySleepFar(s32 room_price);

#endif
