@ The title, the introduction and the Nintendo and Camelot logos.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_TitleTable
FarCall_TitleTable:
	export Title_ShowIntroFar, Title_ShowIntro
	overlay_veneer Title_NoOpColor
	overlay_veneer Title_NoOpPalette
	export Title_ShowNintendoLogoFar, Title_ShowNintendoLogo
	export Title_ShowCamelotLogoFar, Title_ShowCamelotLogo
