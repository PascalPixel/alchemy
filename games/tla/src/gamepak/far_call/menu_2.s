.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Menu_SelectShopCommandFar
	.thumb_func
Menu_SelectShopCommandFar:
	overlay_veneer Menu_SelectShopCommand
	.global Menu_SelectSanctumServiceFar
	.thumb_func
Menu_SelectSanctumServiceFar:
	overlay_veneer Menu_SelectSanctumService
	.global Menu_RunConfirmSelectionFar
	.thumb_func
Menu_RunConfirmSelectionFar:
	overlay_veneer Menu_RunConfirmSelection
	.global Menu_SelectCoinsOrMedalsFar
	.thumb_func
Menu_SelectCoinsOrMedalsFar:
	overlay_veneer Menu_SelectCoinsOrMedals
	.global Func_08038388
	.thumb_func
Func_08038388:
	overlay_veneer Func_08044a54
