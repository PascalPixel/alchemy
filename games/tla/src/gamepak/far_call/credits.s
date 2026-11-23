@ The credits' entry point.
	.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global FarCall_CreditsTable
FarCall_CreditsTable:
	export Credits_RunFar, Credits_Run
	export EndingImage_StartFar, EndingImage_Start
	export EndingImage_ShowFar, EndingImage_Show
	export SceneState_SetScrollTargetFar, SceneState_SetScrollTarget
	export EndingImage_WaitForScrollFar, EndingImage_WaitForScroll
	export EndingImage_LowerTopStripFar, EndingImage_LowerTopStrip
