@ Import veneers after the DERI_HEYA overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — DERI_HEYA overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
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
	.global Func_02000c9c
	.thumb_func
Func_02000c9c:
	overlay_veneer Object_ResetMotion
	.global Func_02000ca4
	.thumb_func
Func_02000ca4:
	overlay_veneer Func_080201a0
	.global Func_02000cac
	.thumb_func
Func_02000cac:
	overlay_veneer Func_080201e0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_02000cbc
	.thumb_func
Func_02000cbc:
	overlay_veneer Func_08020300
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02000ccc
	.thumb_func
Func_02000ccc:
	overlay_veneer UiText_DrawQuantity
	.global UiWork_FinalizePendingCore
	.thumb_func
UiWork_FinalizePendingCore:
	overlay_veneer UiWork_FinalizePendingCoreFar
	.global Owner_RecalculateStats
	.thumb_func
Owner_RecalculateStats:
	overlay_veneer Owner_RecalculateStatsFar
	.global Func_02000ce4
	.thumb_func
Func_02000ce4:
	overlay_veneer Item_Get
	.global Inventory_GetQuantity
	.thumb_func
Inventory_GetQuantity:
	overlay_veneer Inventory_GetQuantityFar
	.global PartyInventory_Add
	.thumb_func
PartyInventory_Add:
	overlay_veneer PartyInventory_AddFar
	.global Inventory_Discard
	.thumb_func
Inventory_Discard:
	overlay_veneer Inventory_DiscardFar
	.global Owner_RefreshClassActions
	.thumb_func
Owner_RefreshClassActions:
	overlay_veneer Owner_RefreshClassActionsFar
	.global Inventory_CountItem
	.thumb_func
Inventory_CountItem:
	overlay_veneer Inventory_CountItemFar
	.global Func_02000d14
	.thumb_func
Func_02000d14:
	overlay_veneer Func_080ad2f0
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Engine_EventBegin
	.thumb_func
Engine_EventBegin:
	overlay_veneer Func_080c8018
	.global Engine_EventEnd
	.thumb_func
Engine_EventEnd:
	overlay_veneer Func_080c8020
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global Func_02000d4c
	.thumb_func
Func_02000d4c:
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
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer Func_080c8180
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Engine_EventShowMessage
	.thumb_func
Engine_EventShowMessage:
	overlay_veneer Func_080c81a0
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Engine_ActorSetSpritePriority
	.thumb_func
Engine_ActorSetSpritePriority:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02000d94
	.thumb_func
Func_02000d94:
	overlay_veneer Func_080c8210
	.global Func_02000d9c
	.thumb_func
Func_02000d9c:
	overlay_veneer Func_080c82e0
	.global Func_02000da4
	.thumb_func
Func_02000da4:
	overlay_veneer Func_080c83d8
	.global Func_02000dac
	.thumb_func
Func_02000dac:
	overlay_veneer Func_080c8480
	.global Engine_EventPrepareSpeakers
	.thumb_func
Engine_EventPrepareSpeakers:
	overlay_veneer Func_080c84e0
	.global Func_02000dbc
	.thumb_func
Func_02000dbc:
	overlay_veneer Func_080c8558
	.global Func_02000dc4
	.thumb_func
Func_02000dc4:
	overlay_veneer Func_080c86f8
	.global Func_02000dcc
	.thumb_func
Func_02000dcc:
	overlay_veneer Func_080c8708
	.global Func_02000dd4
	.thumb_func
Func_02000dd4:
	overlay_veneer Func_080c8778
	.global Func_02000ddc
	.thumb_func
Func_02000ddc:
	overlay_veneer Func_08108008
	.global Func_02000de4
	.thumb_func
Func_02000de4:
	overlay_veneer Func_08108010
	.global Func_02000dec
	.thumb_func
Func_02000dec:
	overlay_veneer Func_08108018
	.global Func_02000df4
	.thumb_func
Func_02000df4:
	overlay_veneer Func_08108068
	.global Func_02000dfc
	.thumb_func
Func_02000dfc:
	overlay_veneer Audio_PlayCue
