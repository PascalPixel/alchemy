@ Far-call stub table heading the system runtime at 0x080000c0: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080000c0
	.thumb_func
Func_080000c0:
	.global WaitFramesFar
	.thumb_func
WaitFramesFar:
	overlay_veneer WaitFrames
	overlay_veneer Scheduler_ResetTaskTable
	.global Scheduler_AddOrUpdateCallbackFar
	.thumb_func
Scheduler_AddOrUpdateCallbackFar:
	overlay_veneer Scheduler_AddOrUpdateCallback
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	.global Func_080000d8
	.thumb_func
Func_080000d8:
	overlay_veneer Scheduler_RemoveCallback
	overlay_veneer Scheduler_EnableCallbacks
	overlay_veneer Scheduler_DisableCallbacks
	overlay_veneer Runtime_InvokeCallbacksByKey
	.global Random16Far
	.thumb_func
Random16Far:
	.global Func_080000f8
	.thumb_func
Func_080000f8:
	overlay_veneer Random16
	.global ArcTan2Far
	.thumb_func
ArcTan2Far:
	.global Func_08000100
	.thumb_func
Func_08000100:
	overlay_veneer ArcTan2
	overlay_veneer Math_IntegerSqrt
	.global FixedSqrtFar
	.thumb_func
FixedSqrtFar:
	overlay_veneer FixedSqrt
	.global Trig_SinFar
	.thumb_func
Trig_SinFar:
	overlay_veneer Trig_Sin
	.global Trig_CosFar
	.thumb_func
Trig_CosFar:
	overlay_veneer Trig_Cos
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	.global Func_08000128
	.thumb_func
Func_08000128:
	overlay_veneer Vector_AddPolarOffset
	.global Runtime_SetIrqHandlerFar
	.thumb_func
Runtime_SetIrqHandlerFar:
	overlay_veneer Runtime_SetIrqHandler
	overlay_veneer Runtime_InitializeHeap
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	.global Func_08000140
	.thumb_func
Func_08000140:
	overlay_veneer Runtime_AllocateHeapBlock
	.global Runtime_AllocateBlockFar
	.thumb_func
Runtime_AllocateBlockFar:
	overlay_veneer Runtime_AllocateBlock
	.global Runtime_ReleaseHeapBlockFar
	.thumb_func
Runtime_ReleaseHeapBlockFar:
	overlay_veneer Runtime_ReleaseHeapBlock
	overlay_veneer Runtime_GetRemainingIwram
	overlay_veneer Runtime_GetRemainingEwram
	.global Runtime_BumpAllocateFar
	.thumb_func
Runtime_BumpAllocateFar:
	overlay_veneer Runtime_BumpAllocate
	.global Runtime_BumpAllocateAlternatePoolFar
	.thumb_func
Runtime_BumpAllocateAlternatePoolFar:
	overlay_veneer Runtime_BumpAllocateAlternatePool
	.global Sys_FreeFar
	.thumb_func
Sys_FreeFar:
	overlay_veneer Sys_Free
	overlay_veneer Runtime_WriteDebugTextTiles
	overlay_veneer Text_DrawHexRightAligned
	overlay_veneer Text_DrawSignedDecimalRightAligned
	overlay_veneer Graphics_FillWordStreamWithF000
	.global Resource_DecodeByteLzFar
	.thumb_func
Resource_DecodeByteLzFar:
	overlay_veneer Resource_DecodeByteLz
	.global Resource_DecodeType01Far
	.thumb_func
Resource_DecodeType01Far:
	overlay_veneer Resource_DecodeType01
	overlay_veneer Resource_InitializeTable
	.global Resource_ResetEntryFar
	.thumb_func
Resource_ResetEntryFar:
	overlay_veneer Resource_ResetEntry
	.global Resource_ActivateEntryFar
	.thumb_func
Resource_ActivateEntryFar:
	overlay_veneer Resource_ActivateEntry
	.global VramBlock_LoadCachedFar
	.thumb_func
VramBlock_LoadCachedFar:
	overlay_veneer VramBlock_LoadCached
	.global Resource_FindFreeEntryFar
	.thumb_func
Resource_FindFreeEntryFar:
	.global Func_080001d0
	.thumb_func
Func_080001d0:
	overlay_veneer Resource_FindFreeEntry
	overlay_veneer Bg0_ClearTilemap
	.global AffineMatrix_BuildForEffectFar
	.thumb_func
AffineMatrix_BuildForEffectFar:
	overlay_veneer AffineMatrix_BuildForEffect
	.global Runtime_PushSlotEntryFar
	.thumb_func
Runtime_PushSlotEntryFar:
	overlay_veneer Runtime_PushSlotEntry
	.global Ui_LoadWindowGraphicsFar
	.thumb_func
Ui_LoadWindowGraphicsFar:
	overlay_veneer Ui_LoadWindowGraphics
	overlay_veneer PaletteDma_LoadBlock
	overlay_veneer Render_ResetTransformState
	overlay_veneer SceneTransform_ResetMatrix
	overlay_veneer Graphics_SaveTransferWorkOnce
	overlay_veneer Graphics_RestoreTransferWork
	overlay_veneer SceneTransform_ApplyRotation
	overlay_veneer SceneTransform_ApplyPitch
	overlay_veneer SceneTransform_ApplyYaw
	overlay_veneer SceneTransform_ApplyRoll
	overlay_veneer SceneTransform_ApplyPosition
	overlay_veneer SceneTransform_ApplyScale
	overlay_veneer SceneTransform_ApplyRotationTranslation
	overlay_veneer SceneTransform_ApplyScaledRotation
	overlay_veneer Graphics_PrepareTransferInIwramWork
	overlay_veneer Render_ProjectPoint
	overlay_veneer Camera_SetAngleParameters
	overlay_veneer Camera_StoreSceneParameters
	overlay_veneer Resource_LoadWorkHeader
	overlay_veneer RuntimeDispatch_ReturnZero
	.global Resource_GetTableEntryFar
	.thumb_func
Resource_GetTableEntryFar:
	overlay_veneer Resource_GetTableEntry
	.global RuntimeDispatch_NoOpHookFar
	.thumb_func
RuntimeDispatch_NoOpHookFar:
	overlay_veneer RuntimeDispatch_NoOpHook
	overlay_veneer Resource_LoadCode
	.global Blend_SetDarkenTarget16Far
	.thumb_func
Blend_SetDarkenTarget16Far:
	overlay_veneer Blend_SetDarkenTarget16
	overlay_veneer Blend_SetDarkenTarget0
	overlay_veneer Blend_SetBrightenTarget16
	overlay_veneer Blend_SetBrightenTarget0
	overlay_veneer Blend_ConfigureTransition
	.global Blend_WaitForTransitionFar
	.thumb_func
Blend_WaitForTransitionFar:
	overlay_veneer Blend_WaitForTransition
	overlay_veneer RuntimeDispatch_ReservedNoOpA
	overlay_veneer RuntimeDispatch_ReservedNoOpB
	overlay_veneer RuntimeDispatch_ReservedNoOpC
	overlay_veneer RuntimeDispatch_ReservedNoOpD
	overlay_veneer RuntimeDispatch_ReservedNoOpE
	.global SerialRuntime_InitializeFar
	.thumb_func
SerialRuntime_InitializeFar:
	overlay_veneer SerialRuntime_Initialize
	.global SerialRuntime_PollAndTransferFar
	.thumb_func
SerialRuntime_PollAndTransferFar:
	overlay_veneer SerialRuntime_PollAndTransfer
	overlay_veneer QueueIoWriteDelay1
	.global QueueIoWriteDelay2Far
	.thumb_func
QueueIoWriteDelay2Far:
	overlay_veneer QueueIoWriteDelay2
	.global QueueIoWriteDelay3Far
	.thumb_func
QueueIoWriteDelay3Far:
	overlay_veneer QueueIoWriteDelay3
	overlay_veneer QueueIoWriteDelay5
	overlay_veneer QueueIoWriteDelay6
	overlay_veneer QueueIoWriteDelay7
	overlay_veneer QueueIoWriteDelay9
	overlay_veneer QueueIoWriteDelay10
	overlay_veneer QueueIoWriteDelay11
	overlay_veneer Runtime_SetMainState19
	overlay_veneer Graphics_PrepareTransferAndRun
	overlay_veneer SerialRuntime_WaitForStatusMask
	.global SerialRuntime_RemoveIrqHandlersFar
	.thumb_func
SerialRuntime_RemoveIrqHandlersFar:
	overlay_veneer SerialRuntime_RemoveIrqHandlers
	.global Sound_LoadPresetParametersFar
	.thumb_func
Sound_LoadPresetParametersFar:
	overlay_veneer Sound_LoadPresetParameters
	.global SerialRuntime_BeginTransferAFar
	.thumb_func
SerialRuntime_BeginTransferAFar:
	overlay_veneer SerialRuntime_BeginTransferA
	.global SerialRuntime_BeginTransferBFar
	.thumb_func
SerialRuntime_BeginTransferBFar:
	overlay_veneer SerialRuntime_BeginTransferB
	overlay_veneer SerialRuntime_WaitForTransferA
	overlay_veneer SerialRuntime_WaitForTransferB
	overlay_veneer SerialRuntime_WaitForTransfers
	.global SerialRuntime_GetActiveTransfersFar
	.thumb_func
SerialRuntime_GetActiveTransfersFar:
	overlay_veneer SerialRuntime_GetActiveTransfers
	overlay_veneer SerialRuntime_StepBlockTransfer
	.global Scheduler_SetCallbackMaskFar
	.thumb_func
Scheduler_SetCallbackMaskFar:
	overlay_veneer Scheduler_SetCallbackMask