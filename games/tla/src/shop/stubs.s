@ Shops, inns and the cursors used by their screens.
	.syntax unified
	.thumb
	.include "games/tla/src/system/stub.s"
	.global Stubs_Shop
Stubs_Shop:
	stub mnShopOpenDebugShopStub, mnShopOpenDebugShop
	stub mnShopRunStub, mnShopRun
	stub mnSanctumRunStub, mnSanctumRun
	stub mnInnCheckInStub, mnInnCheckIn
	stub mnForgeRunStub, mnForgeRun
	stub mnInnQuoteRoomPriceStub, mnInnQuoteRoomPrice
	stub mnCursorAdvanceStub, mnCursorAdvance
	stub mnCursorMoveTowardTargetStub, mnCursorMoveTowardTarget
	stub mnCursorSetStub, mnCursorSet
	stub mnCursorSetPosImmediateStub, mnCursorSetPosImmediate
	stub mnQuantityBarFillStub, mnQuantityBarFill
	stub mnShopSaveAndTintFieldPaletteStub, mnShopSaveAndTintFieldPalette
	stub mnShopRestoreFieldPaletteStub, mnShopRestoreFieldPalette
	stub mnItemPickerRunToDropStub, mnItemPickerRunToDrop
	stub mnItemPickerRunToShowStub, mnItemPickerRunToShow
	stub mnSndWaitForJingleStub, mnSndWaitForJingle
	stub mnInnPlaySleepStub, mnInnPlaySleep
	stub mnVRAMBlockLoadResourceStub, mnVRAMBlockLoadResource
	stub mnForgePartyHasMaterialStub, mnForgePartyHasMaterial
	stub mnForgeSetItemStub, mnForgeSetItem
	stub mnForgeGetItemStub, mnForgeGetItem
