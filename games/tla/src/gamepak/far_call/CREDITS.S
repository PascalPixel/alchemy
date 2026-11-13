	.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Resource_FarCall00A
Resource_FarCall00A:
	export Credits_RunFar, Credits_Run
	export Func_081a0008, EndingPicture_Start
	export Func_081a0010, EndingPicture_Show
	export SceneState_SetScrollTargetFar, SceneState_SetScrollTarget
	export Func_081a0020, EndingPicture_WaitForScroll
	export Func_081a0028, EndingPicture_LowerTopStrip
