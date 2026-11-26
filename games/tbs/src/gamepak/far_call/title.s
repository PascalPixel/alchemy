@ The title, the introduction and the publisher's and Camelot's logos.
	.include "games/tbs/src/gamepak/far_call/table.inc"
	.global FarCall_TitleTable
FarCall_TitleTable:
	export Title_ShowTitleScreenFar, Title_ShowTitleScreen
	overlay_veneer Title_AfterIntroNoOp
	overlay_veneer Title_BeforeLogosNoOp
	export Title_ShowPublisherLogoFar, Title_ShowPublisherLogo
	export Title_ShowCamelotLogoFar, Title_ShowCamelotLogo
