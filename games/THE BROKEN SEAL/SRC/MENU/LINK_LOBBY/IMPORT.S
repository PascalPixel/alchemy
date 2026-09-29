@ Import veneers after the LINK_LOBBY overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — LINK_LOBBY overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global Engine_MathRemainder
	.thumb_func
Engine_MathRemainder:
	overlay_veneer IwramSignedRemainder
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer WaitFramesFar
	.global Engine_TaskAddCallback
	.thumb_func
Engine_TaskAddCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Engine_TaskRemoveCallback
	.thumb_func
Engine_TaskRemoveCallback:
	overlay_veneer Scheduler_RemoveCallbackFar
	.global Runtime_AllocateBlock
	.thumb_func
Runtime_AllocateBlock:
	overlay_veneer Runtime_AllocateBlockFar
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
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
	.global Sound_LoadPresetParameters
	.thumb_func
Sound_LoadPresetParameters:
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
	.global Scheduler_SetCallbackMask
	.thumb_func
Scheduler_SetCallbackMask:
	overlay_veneer Scheduler_SetCallbackMaskFar
	.global Engine_MapRedraw
	.thumb_func
Engine_MapRedraw:
	overlay_veneer Map_ApplyWorkOriginAndSpanFar
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Map_ClearLayerEntryFlag
	.thumb_func
Map_ClearLayerEntryFlag:
	overlay_veneer Map_ClearLayerEntryFlagFar
	.global Map_SetLayerEntryFlag
	.thumb_func
Map_SetLayerEntryFlag:
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
	.global UiText_OpenMessageWindow
	.thumb_func
UiText_OpenMessageWindow:
	overlay_veneer UiText_OpenMessageWindowFar
	.global UiWork_IsComplete
	.thumb_func
UiWork_IsComplete:
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
	.global SaveState_ProcessSelectedSlot
	.thumb_func
SaveState_ProcessSelectedSlot:
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
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
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
	.global Engine_EventChooseYesNo
	.thumb_func
Engine_EventChooseYesNo:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer Object_GetByIdFar
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global Engine_ActorWalkTo
	.thumb_func
Engine_ActorWalkTo:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global Engine_ActorWaitForMove
	.thumb_func
Engine_ActorWaitForMove:
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
	.global Event_SetPairWork1c0
	.thumb_func
Event_SetPairWork1c0:
	overlay_veneer Event_SetPairWork1c0Far
	.global BattleFx_SetWeightedResult
	.thumb_func
BattleFx_SetWeightedResult:
	overlay_veneer BattleFx_SetWeightedResultFar
	.global Party_SetFields1ceAnd1d0
	.thumb_func
Party_SetFields1ceAnd1d0:
	overlay_veneer Party_SetFields1ceAnd1d0Far
	.global Event_SetPair1d4
	.thumb_func
Event_SetPair1d4:
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
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer AudioCommand_PlayFar
