.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
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
	.global Import_ActorResetMotion
	.thumb_func
Import_ActorResetMotion:
	overlay_veneer Actor_StartIdleFar
	.global Map_SetLayerEntryFlag
	.thumb_func
Map_SetLayerEntryFlag:
	overlay_veneer Map_SetLayerEntryFlagFar
	.global Map_CopyCellsRect
	.thumb_func
Map_CopyCellsRect:
	overlay_veneer Map_CopyCellsRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSpriteFlagsFar
	.global Func_02000cbc
	.thumb_func
Func_02000cbc:
	overlay_veneer Actor_SetItemIconFar
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global MessageArgument_Push
	.thumb_func
MessageArgument_Push:
	overlay_veneer MessageArgument_PushFar
	.global Message_CloseFinishedWindows
	.thumb_func
Message_CloseFinishedWindows:
	overlay_veneer Message_CloseFinishedWindowsFar
	.global Unit_Calculate
	.thumb_func
Unit_Calculate:
	overlay_veneer Unit_CalculateFar
	.global Import_ItemGet
	.thumb_func
Import_ItemGet:
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
	.global PartyInventory_FindRoom
	.thumb_func
PartyInventory_FindRoom:
	overlay_veneer PartyInventory_FindRoomFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer Event_WaitFar
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
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_WalkToAndWaitById
	.thumb_func
Actor_WalkToAndWaitById:
	overlay_veneer Actor_WalkToAndWaitByIdFar
	.global Actor_SetHorizontalPositionWithTerrain
	.thumb_func
Actor_SetHorizontalPositionWithTerrain:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_FaceActorById
	.thumb_func
Actor_FaceActorById:
	overlay_veneer Actor_FaceActorByIdFar
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
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Import_ActorSetSpritePriority
	.thumb_func
Import_ActorSetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02000d94
	.thumb_func
Func_02000d94:
	overlay_veneer Actor_ShowEmoteByIdFar
	.global Func_02000d9c
	.thumb_func
Func_02000d9c:
	overlay_veneer Field_TryJumpForwardFar
	.global ItemDisplay_ShowPicture
	.thumb_func
ItemDisplay_ShowPicture:
	overlay_veneer ItemDisplay_ShowPictureFar
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
	overlay_veneer FieldYesNo_AskFar
	.global Func_02000dc4
	.thumb_func
Func_02000dc4:
	overlay_veneer Func_080c86f8
	.global Func_02000dcc
	.thumb_func
Func_02000dcc:
	overlay_veneer Func_080c8708
	.global Party_GetLeaderActor
	.thumb_func
Party_GetLeaderActor:
	overlay_veneer Party_GetLeaderActorFar
	.global Shop_Run
	.thumb_func
Shop_Run:
	overlay_veneer Shop_RunFar
	.global Sanctum_Run
	.thumb_func
Sanctum_Run:
	overlay_veneer Sanctum_RunFar
	.global Inn_CheckIn
	.thumb_func
Inn_CheckIn:
	overlay_veneer Inn_CheckInFar
	.global ItemPicker_RunToDrop
	.thumb_func
ItemPicker_RunToDrop:
	overlay_veneer ItemPicker_RunToDropFar
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
