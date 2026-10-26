@ The title, the introduction and the Nintendo and Camelot logos.
@ C2: aligned 8-byte far-call stubs: export and overlay_veneer emit ldr r4, bx r4 and a target word.
#include "stub.inc"
	.global FarCall_TitleTable
FarCall_TitleTable:
	export Title_ShowIntroFar, Title_ShowIntro
	overlay_veneer Title_NoOpColor
	overlay_veneer Title_NoOpPalette
	export Title_ShowNintendoLogoFar, Title_ShowNintendoLogo
	export Title_ShowCamelotLogoFar, Title_ShowCamelotLogo
