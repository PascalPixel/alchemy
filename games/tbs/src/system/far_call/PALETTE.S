@ Title, introduction and splash-screen presentation.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_PaletteTable
FarCall_PaletteTable:
	export Title_ShowIntroFar, Title_ShowIntro
	overlay_veneer Graphics_NoOpColor
	overlay_veneer Title_NoOpPalette
	export Title_ShowSplashScreenFar, Title_ShowSplashScreen
	export Title_ShowAnimatedSplashFar, Title_ShowAnimatedSplash
