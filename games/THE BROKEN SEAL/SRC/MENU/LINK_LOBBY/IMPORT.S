@ Import veneers after the LINK_LOBBY overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — LINK_LOBBY overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramSignedDivide
	overlay_veneer IwramSignedRemainder
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer WaitFramesFar
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	overlay_veneer Scheduler_RemoveCallbackFar
	overlay_veneer Runtime_AllocateBlockFar
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global Runtime_BumpFree
	.thumb_func
Runtime_BumpFree:
	overlay_veneer Sys_FreeFar
	.global SerialRuntime_Initialize
	.thumb_func
SerialRuntime_Initialize:
	overlay_veneer SerialRuntime_InitializeFar
	.global SerialRuntime_RemoveIrqHandlers
	.thumb_func
SerialRuntime_RemoveIrqHandlers:
	overlay_veneer SerialRuntime_RemoveIrqHandlersFar
	overlay_veneer Sound_LoadPresetParametersFar
	overlay_veneer SerialRuntime_BeginTransferAFar
	.global SerialRuntime_BeginTransferB
	.thumb_func
SerialRuntime_BeginTransferB:
	overlay_veneer SerialRuntime_BeginTransferBFar
	.global SerialRuntime_GetActiveTransfers
	.thumb_func
SerialRuntime_GetActiveTransfers:
	overlay_veneer SerialRuntime_GetActiveTransfersFar
	overlay_veneer Scheduler_SetCallbackMaskFar
	overlay_veneer Map_ApplyWorkOriginAndSpanFar
	overlay_veneer Map_CopyMetatileIndicesRectFar
	overlay_veneer Map_ClearLayerEntryFlagFar
	overlay_veneer Map_SetLayerEntryFlagFar
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Engine_DebugCreateWindow
	.thumb_func
Engine_DebugCreateWindow:
	overlay_veneer UiWindow_CreateFar
	.global UiWork_Finalize
	.thumb_func
UiWork_Finalize:
	overlay_veneer UiWork_FinalizeFar
	.global Ui_AdjustValueWithoutLimit
	.thumb_func
Ui_AdjustValueWithoutLimit:
	overlay_veneer Ui_AdjustValueWithoutLimitFar
	overlay_veneer UiText_OpenMessageWindowFar
	overlay_veneer UiWork_IsCompleteFar
	.global RenderOutput_PrepareForRedraw
	.thumb_func
RenderOutput_PrepareForRedraw:
	overlay_veneer RenderOutput_PrepareForRedrawFar
	.global UiText_DrawNumber
	.thumb_func
UiText_DrawNumber:
	overlay_veneer UiText_DrawNumberFar
	.global UiWork_ClearValueNameTables
	.thumb_func
UiWork_ClearValueNameTables:
	overlay_veneer UiWork_ClearValueNameTablesFar
	.global UiText_DrawQuantity
	.thumb_func
UiText_DrawQuantity:
	overlay_veneer UiWork_PushValueSlotFar
	overlay_veneer SaveState_ProcessSelectedSlotFar
	.global Trade_GetOfferState
	.thumb_func
Trade_GetOfferState:
	overlay_veneer Trade_GetOfferStateFar
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer GameFlag_TestFar
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer GameFlag_SetBitFar
	.global Engine_GameFlagClear
	.thumb_func
Engine_GameFlagClear:
	overlay_veneer GameFlag_ClearBitFar
	overlay_veneer GameFlag_GetByteFar
	overlay_veneer GameFlag_SetByteFar
	.global Party_CountActiveOwners
	.thumb_func
Party_CountActiveOwners:
	overlay_veneer Party_CountActiveOwnersFar
	overlay_veneer Owner_RefreshActiveRatiosFar
	.global Engine_EventBegin
	.thumb_func
Engine_EventBegin:
	overlay_veneer Battle_ResetFar
	.global Engine_EventEnd
	.thumb_func
Engine_EventEnd:
	overlay_veneer BattleFx_FinishActionFar
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer Object_GetByIdFar
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Engine_ActorFaceActor
	.thumb_func
Engine_ActorFaceActor:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer Event_SetValue1d8Far
	.global Engine_EventOpenMessage
	.thumb_func
Engine_EventOpenMessage:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Engine_EventShowMessage
	.thumb_func
Engine_EventShowMessage:
	overlay_veneer BattleEv_RunWaitFar
	overlay_veneer Event_SetPairWork1c0Far
	overlay_veneer BattleFx_SetWeightedResultFar
	overlay_veneer Party_SetFields1ceAnd1d0Far
	overlay_veneer Event_SetPair1d4Far
	overlay_veneer Event_SetStatus1c6Far
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Engine_SanctumOpen
	.thumb_func
Engine_SanctumOpen:
	overlay_veneer Shop_ConfirmActFar
	overlay_veneer BattlePlacement_UpdateTimedEntriesTwentyTimesFar
	.global DebugParty_LoadPreset
	.thumb_func
DebugParty_LoadPreset:
	overlay_veneer DebugParty_LoadPresetFar
	overlay_veneer AudioCommand_PlayFar
