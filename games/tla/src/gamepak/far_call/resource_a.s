.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Resource_FarCall00A
Resource_FarCall00A:
	.global Func_081a0000
	.thumb_func
Func_081a0000:
	overlay_veneer Func_081a06e0
	.global Func_081a0008
	.thumb_func
Func_081a0008:
	overlay_veneer Func_081a150c
	.global Func_081a0010
	.thumb_func
Func_081a0010:
	overlay_veneer Func_081a15dc
	.global Func_081a0018
	.thumb_func
Func_081a0018:
	overlay_veneer SceneState_SetScrollTarget
	.global Func_081a0020
	.thumb_func
Func_081a0020:
	overlay_veneer Func_081a17d0
	.global Func_081a0028
	.thumb_func
Func_081a0028:
	overlay_veneer Func_081a18f8
