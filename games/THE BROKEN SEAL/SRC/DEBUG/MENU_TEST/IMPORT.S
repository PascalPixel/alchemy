@ Import veneers after the MENU_TEST overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — MENU_TEST overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_DebugRemainder
	.thumb_func
Engine_DebugRemainder:
	overlay_veneer IwramSignedRemainder
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer WaitFramesFar
	.global Engine_DebugCreateWindow
	.thumb_func
Engine_DebugCreateWindow:
	overlay_veneer UiWindow_CreateFar
	.global Engine_DebugFinalizeWindow
	.thumb_func
Engine_DebugFinalizeWindow:
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
	.global UiText_DrawResource
	.thumb_func
UiText_DrawResource:
	overlay_veneer UiText_DrawResourceFar
	.global UiText_DrawCharacterAtOffset
	.thumb_func
UiText_DrawCharacterAtOffset:
	overlay_veneer UiText_DrawCharacterAtOffsetFar
	.global UiText_DrawStringInWindow
	.thumb_func
UiText_DrawStringInWindow:
	overlay_veneer UiText_DrawStringInWindowFar
	.global UiText_DrawNumberAtOffset
	.thumb_func
UiText_DrawNumberAtOffset:
	overlay_veneer UiText_DrawNumberAtOffsetFar
	.global UiWindow_CreateWithSideObject
	.thumb_func
UiWindow_CreateWithSideObject:
	overlay_veneer UiWindow_CreateWithSideObjectFar
	.global UiWork_PushValueSlot
	.thumb_func
UiWork_PushValueSlot:
	overlay_veneer UiWork_PushValueSlotFar
	.global UiWork_FinalizePendingCore
	.thumb_func
UiWork_FinalizePendingCore:
	overlay_veneer UiWork_FinalizePendingCoreFar
	.global Engine_DebugRedrawWindow
	.thumb_func
Engine_DebugRedrawWindow:
	overlay_veneer RenderOutput_RedrawSavedRectFar
	.global Engine_DebugClearWindow
	.thumb_func
Engine_DebugClearWindow:
	overlay_veneer RenderOutput_ClearListFar
	.global NameEntry_EditOwnerName
	.thumb_func
NameEntry_EditOwnerName:
	overlay_veneer NameEntry_EditOwnerNameFar
	.global DebugMenu_BrowseIcons
	.thumb_func
DebugMenu_BrowseIcons:
	overlay_veneer DebugMenu_BrowseIconsFar
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	.global Owner_RecalculateStats
	.thumb_func
Owner_RecalculateStats:
	overlay_veneer Owner_RecalculateStatsFar
	.global Engine_DebugGetItem
	.thumb_func
Engine_DebugGetItem:
	overlay_veneer Func_08077018
	.global Inventory_AddItem
	.thumb_func
Inventory_AddItem:
	overlay_veneer Inventory_AddItemFar
	.global PartyInventory_Add
	.thumb_func
PartyInventory_Add:
	overlay_veneer PartyInventory_AddFar
	.global Owner_AdjustFirstValue
	.thumb_func
Owner_AdjustFirstValue:
	overlay_veneer Owner_AdjustFirstValueFar
	.global Owner_AdjustSecondValue
	.thumb_func
Owner_AdjustSecondValue:
	overlay_veneer Owner_AdjustSecondValueFar
	.global Party_AddActiveOwner
	.thumb_func
Party_AddActiveOwner:
	overlay_veneer Party_AddActiveOwnerFar
	.global Party_RemoveActiveOwner
	.thumb_func
Party_RemoveActiveOwner:
	overlay_veneer Party_RemoveActiveOwnerFar
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
	.global PartyInventory_HasSpace
	.thumb_func
PartyInventory_HasSpace:
	overlay_veneer PartyInventory_HasSpaceFar
	.global Engine_DebugDrawItemDetails
	.thumb_func
Engine_DebugDrawItemDetails:
	overlay_veneer ItemMenu_DrawItemDetailsFar
	.global Battle_ApplyPresetItemsAndFlags
	.thumb_func
Battle_ApplyPresetItemsAndFlags:
	overlay_veneer Battle_ApplyPresetItemsAndFlagsFar
	.global Shop_ConfirmAct
	.thumb_func
Shop_ConfirmAct:
	overlay_veneer Shop_ConfirmActFar
	.global Shop_PickUnitItem
	.thumb_func
Shop_PickUnitItem:
	overlay_veneer Shop_PickUnitItemFar
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer AudioCommand_PlayFar
