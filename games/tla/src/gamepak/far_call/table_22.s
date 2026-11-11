.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_080ad0b0
	.thumb_func
Func_080ad0b0:
	overlay_veneer Unit_TryLevelUp
	.global Func_080ad0b8
	.thumb_func
Func_080ad0b8:
	overlay_veneer Game_RestartQuest
