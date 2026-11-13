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
	.global YesNoMenu_RunFar
	.thumb_func
YesNoMenu_RunFar:
	overlay_veneer YesNoMenu_Run
	.global Menu_SelectCoinsOrMedalsFar
	.thumb_func
Menu_SelectCoinsOrMedalsFar:
	overlay_veneer Menu_SelectCoinsOrMedals
	.global Func_08038388
	.thumb_func
Func_08038388:
	overlay_veneer Func_08044a54
