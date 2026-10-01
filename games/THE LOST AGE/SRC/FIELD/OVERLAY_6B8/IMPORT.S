@ Import veneers after the OVERLAY_6B8 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B8 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathModulo
	.thumb_func
Engine_MathModulo:
	overlay_veneer IwramUnsignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Func_02000da4
	.thumb_func
Func_02000da4:
	overlay_veneer Func_080200c0
	.global Func_02000dac
	.thumb_func
Func_02000dac:
	overlay_veneer Func_080201e0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02000dc4
	.thumb_func
Func_02000dc4:
	overlay_veneer Func_08020300
	.global UiWork_Finalize
	.thumb_func
UiWork_Finalize:
	overlay_veneer UiWork_FinalizeFar
	.global UiText_OpenMessageWindow
	.thumb_func
UiText_OpenMessageWindow:
	overlay_veneer UiText_OpenMessageWindowFar
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global UiWork_IsComplete
	.thumb_func
UiWork_IsComplete:
	overlay_veneer UiWork_IsCompleteFar
	.global Func_02000dec
	.thumb_func
Func_02000dec:
	overlay_veneer Func_080380f8
	.global Func_02000df4
	.thumb_func
Func_02000df4:
	overlay_veneer UiText_DrawQuantity
	.global UiWork_FinalizePendingCore
	.thumb_func
UiWork_FinalizePendingCore:
	overlay_veneer UiWork_FinalizePendingCoreFar
	.global Func_02000e04
	.thumb_func
Func_02000e04:
	overlay_veneer Func_080382f8
	.global Func_02000e0c
	.thumb_func
Func_02000e0c:
	overlay_veneer Func_08038318
	.global Owner_RecalculateStats
	.thumb_func
Owner_RecalculateStats:
	overlay_veneer Owner_RecalculateStatsFar
	.global Inventory_AddItem
	.thumb_func
Inventory_AddItem:
	overlay_veneer Inventory_AddItemFar
	.global Owner_AdjustFirstValue
	.thumb_func
Owner_AdjustFirstValue:
	overlay_veneer Owner_AdjustFirstValueFar
	.global Owner_AdjustSecondValue
	.thumb_func
Owner_AdjustSecondValue:
	overlay_veneer Owner_AdjustSecondValueFar
	.global Djinn_AddToOwner
	.thumb_func
Djinn_AddToOwner:
	overlay_veneer Djinn_AddToOwnerFar
	.global Djinn_Activate
	.thumb_func
Djinn_Activate:
	overlay_veneer Djinn_ActivateFar
	.global Party_AdvanceOwnerCountToTarget
	.thumb_func
Party_AdvanceOwnerCountToTarget:
	overlay_veneer Party_AdvanceOwnerCountToTargetFar
	.global Item_AdjustCounter
	.thumb_func
Item_AdjustCounter:
	overlay_veneer Item_AdjustCounterFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02000e5c
	.thumb_func
Func_02000e5c:
	overlay_veneer Func_080c8078
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global Func_02000e74
	.thumb_func
Func_02000e74:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Func_02000e94
	.thumb_func
Func_02000e94:
	overlay_veneer Func_080c8180
	.global Func_02000e9c
	.thumb_func
Func_02000e9c:
	overlay_veneer Func_080c81a0
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02000eac
	.thumb_func
Func_02000eac:
	overlay_veneer Func_080c8210
	.global Func_02000eb4
	.thumb_func
Func_02000eb4:
	overlay_veneer Event_SetPairWork1c0Far
	.global Func_02000ebc
	.thumb_func
Func_02000ebc:
	overlay_veneer Func_080c8280
	.global Func_02000ec4
	.thumb_func
Func_02000ec4:
	overlay_veneer Func_080c85c0
	.global Func_02000ecc
	.thumb_func
Func_02000ecc:
	overlay_veneer Func_080c8760
	.global Func_02000ed4
	.thumb_func
Func_02000ed4:
	overlay_veneer Func_080c87b0
	.global Func_02000edc
	.thumb_func
Func_02000edc:
	overlay_veneer Func_080c87b8
	.global Func_02000ee4
	.thumb_func
Func_02000ee4:
	overlay_veneer Func_080c87c0
	.global Func_02000eec
	.thumb_func
Func_02000eec:
	overlay_veneer Func_080c87c8
	.global Func_02000ef4
	.thumb_func
Func_02000ef4:
	overlay_veneer Func_080c87e8
	.global Func_02000efc
	.thumb_func
Func_02000efc:
	overlay_veneer Func_080c87f0
	.global Func_02000f04
	.thumb_func
Func_02000f04:
	overlay_veneer Func_080c8880
	.global Func_02000f0c
	.thumb_func
Func_02000f0c:
	overlay_veneer Func_080c8888
	.global Func_02000f14
	.thumb_func
Func_02000f14:
	overlay_veneer Func_080c8948
	.global Func_02000f1c
	.thumb_func
Func_02000f1c:
	overlay_veneer Func_08108008
	.global Func_02000f24
	.thumb_func
Func_02000f24:
	overlay_veneer Func_08108010
	.global Func_02000f2c
	.thumb_func
Func_02000f2c:
	overlay_veneer Func_08108018
	.global Func_02000f34
	.thumb_func
Func_02000f34:
	overlay_veneer Func_08108068
	.global Func_02000f3c
	.thumb_func
Func_02000f3c:
	overlay_veneer Audio_PlayCue
