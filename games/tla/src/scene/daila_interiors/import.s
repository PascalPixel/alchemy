.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global Unit_Get
	.thumb_func
Unit_Get:
	overlay_veneer Unit_GetFar
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
	overlay_veneer Map_CopyCellsRectFar
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_02000cbc
	.thumb_func
Func_02000cbc:
	overlay_veneer Func_08020300
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Func_02000ccc
	.thumb_func
Func_02000ccc:
	overlay_veneer MessageArgument_PushFar
	.global Message_CloseFinishedWindows
	.thumb_func
Message_CloseFinishedWindows:
	overlay_veneer Message_CloseFinishedWindowsFar
	.global Unit_Calculate
	.thumb_func
Unit_Calculate:
	overlay_veneer Unit_CalculateFar
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
	.global Unit_RefreshPsynergy
	.thumb_func
Unit_RefreshPsynergy:
	overlay_veneer Unit_RefreshPsynergyFar
	.global Inventory_CountItem
	.thumb_func
Inventory_CountItem:
	overlay_veneer Inventory_CountItemFar
	.global Func_02000d14
	.thumb_func
Func_02000d14:
	overlay_veneer PartyInventory_FindRoomFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Import_EventStart
	.thumb_func
Import_EventStart:
	overlay_veneer Event_StartFar
	.global Import_EventEnd
	.thumb_func
Import_EventEnd:
	overlay_veneer Event_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
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
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
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
	.global Import_EventSetMessageId
	.thumb_func
Import_EventSetMessageId:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Import_EventShowMessageAndWait
	.thumb_func
Import_EventShowMessageAndWait:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Import_ActorSetSpritePriority
	.thumb_func
Import_ActorSetSpritePriority:
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
	.global Import_EventResolvePendingActions
	.thumb_func
Import_EventResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
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
	overlay_veneer Party_GetLeaderActorFar
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
	overlay_veneer Sound_PlayFar
