.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Resource_FarCall00B
Resource_FarCall00B:
	.global Func_081a6000
	.thumb_func
Func_081a6000:
	overlay_veneer Func_081a729c
	.global Func_081a6008
	.thumb_func
Func_081a6008:
	overlay_veneer Data_081a786c
	.global Func_081a6010
	.thumb_func
Func_081a6010:
	overlay_veneer Title_NoOpPalette
	.global Func_081a6018
	.thumb_func
Func_081a6018:
	overlay_veneer Title_ShowSplashScreen
	.global Func_081a6020
	.thumb_func
Func_081a6020:
	overlay_veneer Func_081a76f8
	.global Func_081a6028
	.thumb_func
Func_081a6028:
	overlay_veneer Func_081a71c8
