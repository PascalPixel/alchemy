@ The credits' entry point.
	.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global FarCall_CreditsTable
FarCall_CreditsTable:
	export Credits_RunFar, Credits_Run
	export EndingPicture_StartFar, EndingPicture_Start
	export EndingPicture_ShowFar, EndingPicture_Show
	export SceneState_SetScrollTargetFar, SceneState_SetScrollTarget
	export EndingPicture_WaitForScrollFar, EndingPicture_WaitForScroll
	export EndingPicture_LowerTopStripFar, EndingPicture_LowerTopStrip
