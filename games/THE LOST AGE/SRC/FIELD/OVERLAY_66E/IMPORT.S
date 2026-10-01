@ Import veneers after the OVERLAY_66E overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_66E overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Runtime_AllocateBlock
	.thumb_func
Runtime_AllocateBlock:
	overlay_veneer Runtime_AllocateBlockFar
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_020010a0
	.thumb_func
Func_020010a0:
	overlay_veneer Func_08020300
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_020010b0
	.thumb_func
Func_020010b0:
	overlay_veneer UiText_DrawQuantity
	.global Func_020010b8
	.thumb_func
Func_020010b8:
	overlay_veneer Item_Get
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global Party_AdjustSixDigitCounterA
	.thumb_func
Party_AdjustSixDigitCounterA:
	overlay_veneer Party_AdjustSixDigitCounterAFar
	.global Func_020010d0
	.thumb_func
Func_020010d0:
	overlay_veneer Func_080ad2e8
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_020010e0
	.thumb_func
Func_020010e0:
	overlay_veneer Func_080c8018
	.global Func_020010e8
	.thumb_func
Func_020010e8:
	overlay_veneer Func_080c8020
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global Func_02001108
	.thumb_func
Func_02001108:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02001138
	.thumb_func
Func_02001138:
	overlay_veneer Func_080c8180
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02001148
	.thumb_func
Func_02001148:
	overlay_veneer Func_080c81a0
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02001158
	.thumb_func
Func_02001158:
	overlay_veneer Func_080c8208
	.global Func_02001160
	.thumb_func
Func_02001160:
	overlay_veneer Func_080c8210
	.global Func_02001168
	.thumb_func
Func_02001168:
	overlay_veneer Func_080c8378
	.global Func_02001170
	.thumb_func
Func_02001170:
	overlay_veneer Func_080c8380
	.global Func_02001178
	.thumb_func
Func_02001178:
	overlay_veneer Func_080c8390
	.global Func_02001180
	.thumb_func
Func_02001180:
	overlay_veneer Func_080c84e0
	.global Func_02001188
	.thumb_func
Func_02001188:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer Func_080c8778
	.endif
	.endif
	.global Func_02001190
	.thumb_func
Func_02001190:
	overlay_veneer Func_08108008
	.global Func_02001198
	.thumb_func
Func_02001198:
	overlay_veneer Func_08108018
	.global Func_020011a0
	.thumb_func
Func_020011a0:
	overlay_veneer Func_08108070
	.global Func_020011a8
	.thumb_func
Func_020011a8:
	overlay_veneer Audio_PlayCue
