.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Resource_FarCall00E
Resource_FarCall00E:
	.global Func_081b2000
	.thumb_func
Func_081b2000:
	overlay_veneer LuckyWheels_Start
