@ The title, the introduction and the Nintendo and Camelot logos.
	.include "games/tbs/src/gamepak/far_call/table.inc"
	.global FarCall_TitleTable
FarCall_TitleTable:
	export Title_ShowTitleScreenFar, Title_ShowTitleScreen
	overlay_veneer Title_AfterIntroNoOp
	overlay_veneer Title_BeforeLogosNoOp
	export Title_ShowNintendoLogoFar, Title_ShowNintendoLogo
	export Title_ShowCamelotLogoFar, Title_ShowCamelotLogo
