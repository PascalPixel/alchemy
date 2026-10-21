.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Game_RunFar
	.thumb_func
Game_RunFar:
	overlay_veneer Game_Run
	.global FieldEvent_WaitFramesFar
	.thumb_func
FieldEvent_WaitFramesFar:
	overlay_veneer EventRuntime_Wait
	.global EventRuntime_BeginFar
	.thumb_func
EventRuntime_BeginFar:
	overlay_veneer EventRuntime_Begin
	.global EventRuntime_EndFar
	.thumb_func
EventRuntime_EndFar:
	overlay_veneer EventRuntime_End
	.global Event_RunObjectHookAndWaitFar
	.thumb_func
Event_RunObjectHookAndWaitFar:
	overlay_veneer Event_RunObjectHookAndWait
	.global Event_CallWithLastActiveObjectIdFar
	.thumb_func
Event_CallWithLastActiveObjectIdFar:
	overlay_veneer Event_CallWithLastActiveObjectId
	.global Scene_AssignViewFlagsFar
	.thumb_func
Scene_AssignViewFlagsFar:
	overlay_veneer Scene_AssignViewFlags
	.global Event_SetSceneEventsFar
	.thumb_func
Event_SetSceneEventsFar:
	overlay_veneer Event_SetSceneEvents
	.global Event_PrepareObjectAndApplyValueFar
	.thumb_func
Event_PrepareObjectAndApplyValueFar:
	overlay_veneer Event_PrepareObjectAndApplyValue
	.global Event_PrepareTwoObjectsAndApplyFar
	.thumb_func
Event_PrepareTwoObjectsAndApplyFar:
	overlay_veneer Event_PrepareTwoObjectsAndApply
	.global Party_RemoveOwnerRestoredFar
	.thumb_func
Party_RemoveOwnerRestoredFar:
	overlay_veneer Party_RemoveOwnerRestored
	.global PartyInventory_GiveItemFar
	.thumb_func
PartyInventory_GiveItemFar:
	overlay_veneer PartyInventory_GiveItem
	.global Inventory_TryAddAndReturnOwnerFar
	.thumb_func
Inventory_TryAddAndReturnOwnerFar:
	overlay_veneer Inventory_TryAddAndReturnOwner
	.global Event_AskYesNoWithActorAnimationFar
	.thumb_func
Event_AskYesNoWithActorAnimationFar:
	overlay_veneer Event_AskYesNoWithActorAnimation
	.global Func_080c8078
	.thumb_func
Func_080c8078:
	overlay_veneer Func_080d296c
	.global UiText_DrawQuantityPairWithCueFar
	.thumb_func
UiText_DrawQuantityPairWithCueFar:
	overlay_veneer UiText_DrawQuantityPairWithCue
