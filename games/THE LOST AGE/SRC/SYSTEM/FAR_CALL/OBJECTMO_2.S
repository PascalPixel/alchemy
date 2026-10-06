.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global BattleFx_GetResourceIdFar
	.thumb_func
BattleFx_GetResourceIdFar:
	overlay_veneer Func_080d1eac
	.global Event_ClearInvalidPackedValuesFar
	.thumb_func
Event_ClearInvalidPackedValuesFar:
	overlay_veneer Event_ClearValidPackedIds
	.global BattleFx_InitializeSlotsFar
	.thumb_func
BattleFx_InitializeSlotsFar:
	overlay_veneer BattleFx_InitializeSlots
	.global BattleFx_ClearActiveSlotsAndScheduleUpdatesFar
	.thumb_func
BattleFx_ClearActiveSlotsAndScheduleUpdatesFar:
	overlay_veneer BattleFx_ClearActiveSlotsAndScheduleUpdates
	.global Motion_LaunchFromFocusedObjectFar
	.thumb_func
Motion_LaunchFromFocusedObjectFar:
	overlay_veneer Func_080d3070
	.global ObjectMotion_OffsetPositionAndResetFar
	.thumb_func
ObjectMotion_OffsetPositionAndResetFar:
	overlay_veneer ObjectMotion_OffsetPositionAndReset
	.global ObjectMotion_CommitPositionAndActivateFar
	.thumb_func
ObjectMotion_CommitPositionAndActivateFar:
	overlay_veneer ObjectMotion_CommitPositionAndActivate
	.global Object_LinkObjectAndSetCallbackFar
	.thumb_func
Object_LinkObjectAndSetCallbackFar:
	overlay_veneer Object_LinkObjectAndSetCallback
	.global BattleEffect_PauseObjectFar
	.thumb_func
BattleEffect_PauseObjectFar:
	overlay_veneer BattleEffect_PauseObject
	.global Func_080c8610
	.thumb_func
Func_080c8610:
	overlay_veneer Func_080e035c
	.global Func_080c8618
	.thumb_func
Func_080c8618:
	overlay_veneer Func_080d3c2c
	.global ObjectTable_ReadActiveValueFar
	.thumb_func
ObjectTable_ReadActiveValueFar:
	overlay_veneer ObjectTable_ReadActiveValue
