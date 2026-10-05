.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_PaletteTable
FarCall_PaletteTable:
	.global Func_080f2000
	.thumb_func
Func_080f2000:
	overlay_veneer Title_ShowIntro
	overlay_veneer Graphics_NoOpColor
	overlay_veneer Title_NoOpPalette
	.global Title_ShowSplashScreenFar
	.thumb_func
Title_ShowSplashScreenFar:
	overlay_veneer Title_ShowSplashScreen
	.global Title_ShowAnimatedSplashFar
	.thumb_func
Title_ShowAnimatedSplashFar:
	overlay_veneer Title_ShowAnimatedSplash
