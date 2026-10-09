@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.global FarCall_PaletteTable
FarCall_PaletteTable:
	.global Title_ShowIntroFar
	.thumb_func
Title_ShowIntroFar:
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
