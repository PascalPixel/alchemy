@ The title, the introduction and the publisher's and Camelot's logos.
	.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global FarCall_TitleTable
FarCall_TitleTable:
	export Title_ShowTitleScreenFar, Title_ShowTitleScreen
	export Title_AfterIntroNoOpFar, Title_AfterIntroNoOp
	export Title_BeforeLogosNoOpFar, Title_BeforeLogosNoOp
	export Title_ShowPublisherLogoFar, Title_ShowPublisherLogo
	export Title_ShowCamelotLogoFar, Title_ShowCamelotLogo
	export TitleIntro_RunFar, TitleIntro_Run
