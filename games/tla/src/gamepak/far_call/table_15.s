.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_080381c0
	.thumb_func
Func_080381c0:
	overlay_veneer Func_0804bc08
	.global BattleMessage_ShowAndWaitFar
	.thumb_func
BattleMessage_ShowAndWaitFar:
	overlay_veneer BattleMessage_ShowAndWait
	.global Func_080381d0
	.thumb_func
Func_080381d0:
	overlay_veneer BattleMessage_Open
	.global Func_080381d8
	.thumb_func
Func_080381d8:
	overlay_veneer MenuArrow_NoOpCallback4
