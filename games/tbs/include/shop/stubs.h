#ifndef ALCHEMY_SHOP_STUBS_H
#define ALCHEMY_SHOP_STUBS_H

/* @file */
/* Shops, inns and the cursors used by their screens. */

s32 mnShopOpenDebugShopStub(s32 unused);
s32 mnShopRunStub(s32 stockRow, s32 keeperId);
s32 scSanctumRunStub(s32 unitId);
s32 mnInnCheckInStub(s32 innId, s32 actorId);
void mnCursorAdvanceStub(struct MENU_CURSOR *cursor);
void mnCursorMoveTowardTargetStub(struct MENU_CURSOR *cursor);
void mnCursorSetStub(struct MENU_CURSOR *cursor, s32 targetX, s32 targetY,
					 s32 frames);
void mnCursorSetPosImmediateStub(struct MENU_CURSOR *cursor, s32 x, s32 y);
void mnQuantityBarFillStub(s32 itemCount, s32 color, u8 *tileBuffer);
void mnShopSaveAndTintFieldPaletteStub(s32 tint);
void mnShopRestoreFieldPaletteStub(void);
s32 mnItemPickerRunStub(s32 *selectedUnit, s32 *selectedSlot);
void sySndWaitForJingleStub(void);
void mnInnPlaySleepStub(s32 roomPrice);

#endif
