.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global BattleMessage_CloseFar
	.thumb_func
BattleMessage_CloseFar:
	overlay_veneer BattleMessage_Close
	.global Message_ResetIdleFar
	.thumb_func
Message_ResetIdleFar:
	overlay_veneer Message_ResetIdle
	.global MenuArrow_CreateFar
	.thumb_func
MenuArrow_CreateFar:
	overlay_veneer MenuArrow_Create
	.global MenuArrow_SetPositionFar
	.thumb_func
MenuArrow_SetPositionFar:
	overlay_veneer MenuArrow_SetPosition
	.global MenuArrow_ReleaseFar
	.thumb_func
MenuArrow_ReleaseFar:
	overlay_veneer MenuArrow_Release
	.global Func_08038240
	.thumb_func
Func_08038240:
	overlay_veneer Item_RemoveFromInventory
