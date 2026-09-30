@ Import veneers after the OVERLAY_658 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_658 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global Func_02002ad8
	.thumb_func
Func_02002ad8:
	overlay_veneer Func_080003c8
	.global Func_02002ae0
	.thumb_func
Func_02002ae0:
	overlay_veneer Func_080003d0
	.global Func_02002ae8
	.thumb_func
Func_02002ae8:
	overlay_veneer Func_08020170
	.global Func_02002af0
	.thumb_func
Func_02002af0:
	overlay_veneer Func_080201e8
	.global Func_02002af8
	.thumb_func
Func_02002af8:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02002b10
	.thumb_func
Func_02002b10:
	overlay_veneer UiText_DrawQuantity
	.global Func_02002b18
	.thumb_func
Func_02002b18:
	overlay_veneer Func_080ad2f0
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02002b28
	.thumb_func
Func_02002b28:
	overlay_veneer Func_080c8018
	.global Func_02002b30
	.thumb_func
Func_02002b30:
	overlay_veneer Func_080c8020
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_SnapHeadingAndOffset
	.thumb_func
ObjectMotion_SnapHeadingAndOffset:
	overlay_veneer ObjectMotion_SnapHeadingAndOffsetFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02002b88
	.thumb_func
Func_02002b88:
	overlay_veneer Func_080c80f8
	.global Func_02002b90
	.thumb_func
Func_02002b90:
	overlay_veneer Func_080c8100
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Func_02002bb8
	.thumb_func
Func_02002bb8:
	overlay_veneer Func_080c8158
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Func_02002bc8
	.thumb_func
Func_02002bc8:
	overlay_veneer Func_080c8170
	.global Func_02002bd0
	.thumb_func
Func_02002bd0:
	overlay_veneer Func_080c8180
	.global Func_02002bd8
	.thumb_func
Func_02002bd8:
	overlay_veneer Func_080c8188
	.global Func_02002be0
	.thumb_func
Func_02002be0:
	overlay_veneer Func_080c8198
	.global Func_02002be8
	.thumb_func
Func_02002be8:
	overlay_veneer Func_080c81a0
	.global Func_02002bf0
	.thumb_func
Func_02002bf0:
	overlay_veneer Func_080c81d0
	.global Func_02002bf8
	.thumb_func
Func_02002bf8:
	overlay_veneer Func_080c81d8
	.global Func_02002c00
	.thumb_func
Func_02002c00:
	overlay_veneer Func_080c8200
	.global Func_02002c08
	.thumb_func
Func_02002c08:
	overlay_veneer Func_080c8210
	.global Func_02002c10
	.thumb_func
Func_02002c10:
	overlay_veneer Func_080c8218
	.global Func_02002c18
	.thumb_func
Func_02002c18:
	overlay_veneer Func_080c8228
	.global Func_02002c20
	.thumb_func
Func_02002c20:
	overlay_veneer Func_080c8238
	.global Func_02002c28
	.thumb_func
Func_02002c28:
	overlay_veneer Func_080c8240
	.global Func_02002c30
	.thumb_func
Func_02002c30:
	overlay_veneer Func_080c8278
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	overlay_veneer Event_ClearStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_02002c50
	.thumb_func
Func_02002c50:
	overlay_veneer Func_080c83e0
	.global Func_02002c58
	.thumb_func
Func_02002c58:
	overlay_veneer Func_080c84e0
	.global Func_02002c60
	.thumb_func
Func_02002c60:
	overlay_veneer Func_080c8570
	.global Func_02002c68
	.thumb_func
Func_02002c68:
	overlay_veneer Func_080c85e8
	.global ObjectMotion_OffsetPositionAndReset
	.thumb_func
ObjectMotion_OffsetPositionAndReset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Func_02002c80
	.thumb_func
Func_02002c80:
	overlay_veneer Func_080c8778
	.global Func_02002c88
	.thumb_func
Func_02002c88:
	overlay_veneer Func_08108008
	.global Func_02002c90
	.thumb_func
Func_02002c90:
	overlay_veneer Audio_PlayCue
