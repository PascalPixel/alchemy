	.include "games/tbs/src/cartridge/overlay.inc"
	overlay_veneer FileScreen_Run
	overlay_veneer FileScreen_GetEntrances
	overlay_veneer FileScreen_GetExits
	overlay_veneer FileScreen_GetActorPlacements
	overlay_veneer FileScreen_GetTriggers
	overlay_veneer Scene_GetExitRegions
