#ifndef ALCHEMY_SHOP_STUBS_H
#define ALCHEMY_SHOP_STUBS_H

/* Shops, inns and the cursors used by their screens. */

Sint32 mnShopOpenDebugShopStub(Sint32 unused);
Sint32 mnShopRunStub(Sint32 stockRow, Sint32 keeperId);
Sint32 mnSanctumRunStub(Sint32 unitId);
Sint32 mnInnCheckInStub(Sint32 innId, Sint32 actorId);
void   mnCursorAdvanceStub(struct MENU_CURSOR *cursor);
void   mnCursorMoveTowardTargetStub(struct MENU_CURSOR *cursor);
void   mnCursorSetStub(
	  struct MENU_CURSOR *cursor, Sint32 tgtX, Sint32 tgtY, Sint32 ticks);
void mnCursorSetPosImmediateStub(
	struct MENU_CURSOR *cursor, Sint32 x, Sint32 y);
void   mnQuantityBarFillStub(Sint32 itemCnt, Sint32 col, Uint8 *tileBuf);
void   mnShopSaveAndTintFieldPaletteStub(Sint32 tint);
void   mnShopRestoreFieldPaletteStub(void);
Sint32 mnItemPickerRunStub(Sint32 *selectedUnit, Sint32 *selectedSlot);
void   mnSndWaitForJingleStub(void);
void   mnInnPlaySleepStub(Sint32 roomPrice);

#endif
