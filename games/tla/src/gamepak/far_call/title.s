@ The title, the introduction and the Nintendo and Camelot logos.
	.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global FarCall_TitleTable
FarCall_TitleTable:
	export Title_ShowTitleScreenFar, Title_ShowTitleScreen
	export Title_NoOpColorFar, Title_NoOpColor
	export Title_NoOpPaletteFar, Title_NoOpPalette
	export Title_ShowNintendoLogoFar, Title_ShowNintendoLogo
	export Title_ShowCamelotLogoFar, Title_ShowCamelotLogo
	export TitleIntro_RunFar, TitleIntro_Run
