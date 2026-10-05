.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global WaitFramesFar
	.thumb_func
WaitFramesFar:
	overlay_veneer WaitFrames
	.global Scheduler_ResetTaskTableFar
	.thumb_func
Scheduler_ResetTaskTableFar:
	overlay_veneer Scheduler_ResetTaskTable
	.global Scheduler_AddOrUpdateCallbackFar
	.thumb_func
Scheduler_AddOrUpdateCallbackFar:
	overlay_veneer Scheduler_AddOrUpdateCallback
	.global Func_080000d8
	.thumb_func
Func_080000d8:
	overlay_veneer Scheduler_RemoveCallback
	.global Scheduler_EnableCallbacksFar
	.thumb_func
Scheduler_EnableCallbacksFar:
	overlay_veneer Scheduler_EnableCallbacks
	.global Scheduler_DisableCallbacksFar
	.thumb_func
Scheduler_DisableCallbacksFar:
	overlay_veneer Scheduler_DisableCallbacks
	.global Scheduler_RunCallbacksByKeyFar
	.thumb_func
Scheduler_RunCallbacksByKeyFar:
	overlay_veneer Scheduler_RunCallbacksByKey
	.global Func_080000f8
	.thumb_func
Func_080000f8:
	overlay_veneer Random16
	.global ArcTan2Far
	.thumb_func
ArcTan2Far:
	overlay_veneer ArcTan2
	.global Math_IntegerSqrtFar
	.thumb_func
Math_IntegerSqrtFar:
	overlay_veneer Math_IntegerSqrt
	.global FixedSqrtFar
	.thumb_func
FixedSqrtFar:
	overlay_veneer FixedSqrt
	.global Math_SineFar
	.thumb_func
Math_SineFar:
	overlay_veneer Trig_Sin
	.global Math_CosineFar
	.thumb_func
Math_CosineFar:
	overlay_veneer Trig_Cos
	.global Func_08000128
	.thumb_func
Func_08000128:
	overlay_veneer Vector_AddPolarOffset
	.global Runtime_SetIrqHandlerFar
	.thumb_func
Runtime_SetIrqHandlerFar:
	overlay_veneer Runtime_SetIrqHandler
	.global Runtime_InitializeHeapFar
	.thumb_func
Runtime_InitializeHeapFar:
	overlay_veneer Runtime_InitializeHeap
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
	.global Runtime_GetRemainingIwramFar
	.thumb_func
Runtime_GetRemainingIwramFar:
	overlay_veneer Runtime_GetRemainingIwram
	.global Runtime_GetRemainingEwramFar
	.thumb_func
Runtime_GetRemainingEwramFar:
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
	.global Runtime_WriteDebugTextTilesFar
	.thumb_func
Runtime_WriteDebugTextTilesFar:
	overlay_veneer Runtime_WriteDebugTextTiles
	.global Text_DrawHexRightAlignedFar
	.thumb_func
Text_DrawHexRightAlignedFar:
	overlay_veneer Text_DrawHexRightAligned
	.global Text_DrawSignedDecimalRightAlignedFar
	.thumb_func
Text_DrawSignedDecimalRightAlignedFar:
	overlay_veneer Text_DrawSignedDecimalRightAligned
	.global Func_08000198
	.thumb_func
Func_08000198:
	overlay_veneer Func_08014aa8
	.global Resource_DecodeByteLzInRamFar
	.thumb_func
Resource_DecodeByteLzInRamFar:
	overlay_veneer Resource_DecodeByteLzInRam
	.global Resource_DecodeType01Far
	.thumb_func
Resource_DecodeType01Far:
	overlay_veneer Resource_DecodeType01
	.global Func_080001b0
	.thumb_func
Func_080001b0:
	overlay_veneer Func_08014368
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
	overlay_veneer Resource_FindFreeEntry
	.global Func_080001d8
	.thumb_func
Func_080001d8:
	overlay_veneer Func_08014b70
	.global AffineMatrix_BuildForEffectFar
	.thumb_func
AffineMatrix_BuildForEffectFar:
	overlay_veneer AffineMatrix_BuildForEffect
	.global Func_080001e8
	.thumb_func
Func_080001e8:
	overlay_veneer Func_080140d8
	.global Ui_LoadWindowGraphicsFar
	.thumb_func
Ui_LoadWindowGraphicsFar:
	overlay_veneer Ui_LoadWindowGraphics
	.global PaletteDma_LoadBlockFar
	.thumb_func
PaletteDma_LoadBlockFar:
	overlay_veneer PaletteDma_LoadBlock
	.global Render_ResetTransformStateFar
	.thumb_func
Render_ResetTransformStateFar:
	overlay_veneer Render_ResetTransformState
	.global Func_08000208
	.thumb_func
Func_08000208:
	overlay_veneer Func_08014ee0
	.global Graphics_SaveTransferWorkOnceFar
	.thumb_func
Graphics_SaveTransferWorkOnceFar:
	overlay_veneer Graphics_SaveTransferWorkOnce
	.global Graphics_RestoreTransferWorkFar
	.thumb_func
Graphics_RestoreTransferWorkFar:
	overlay_veneer Graphics_RestoreTransferWork
	.global Func_08000220
	.thumb_func
Func_08000220:
	overlay_veneer Func_08014efc
	.global SceneTransform_ApplyPitchFar
	.thumb_func
SceneTransform_ApplyPitchFar:
	overlay_veneer SceneTransform_ApplyPitch
	.global SceneTransform_ApplyYawFar
	.thumb_func
SceneTransform_ApplyYawFar:
	overlay_veneer SceneTransform_ApplyYaw
	.global Func_08000238
	.thumb_func
Func_08000238:
	overlay_veneer Func_080150e4
	.global SceneTransform_ApplyPositionFar
	.thumb_func
SceneTransform_ApplyPositionFar:
	overlay_veneer SceneTransform_ApplyPosition
	.global Func_08000248
	.thumb_func
Func_08000248:
	overlay_veneer Func_080151ac
	.global Func_08000250
	.thumb_func
Func_08000250:
	overlay_veneer Func_0801524c
	.global Func_08000258
	.thumb_func
Func_08000258:
	overlay_veneer Func_08015384
	.global Graphics_PrepareTransferInIwramWorkFar
	.thumb_func
Graphics_PrepareTransferInIwramWorkFar:
	overlay_veneer Graphics_PrepareTransferInIwramWork
	.global Render_ProjectPointFar
	.thumb_func
Render_ProjectPointFar:
	overlay_veneer Render_ProjectPoint
	.global Func_08000270
	.thumb_func
Func_08000270:
	overlay_veneer Func_08015714
	.global Camera_StoreSceneParametersFar
	.thumb_func
Camera_StoreSceneParametersFar:
	overlay_veneer Camera_StoreSceneParameters
	.global Func_08000280
	.thumb_func
Func_08000280:
	overlay_veneer Func_080132d0
	.global Runtime_ConstantZeroResultFar
	.thumb_func
Runtime_ConstantZeroResultFar:
	overlay_veneer Runtime_ConstantZeroResult
	.global Resource_GetTableEntryFar
	.thumb_func
Resource_GetTableEntryFar:
	overlay_veneer Resource_GetTableEntry
	.global Resource_ReservedNoOpFar
	.thumb_func
Resource_ReservedNoOpFar:
	overlay_veneer Resource_ReservedNoOp
	.global Resource_LoadCodeFar
	.thumb_func
Resource_LoadCodeFar:
	overlay_veneer Resource_LoadCode
	.global Blend_SetDarkenTarget16Far
	.thumb_func
Blend_SetDarkenTarget16Far:
	overlay_veneer Blend_SetDarkenTarget16
	.global Func_080002b0
	.thumb_func
Func_080002b0:
	overlay_veneer Blend_SetDarkenTarget0
	.global Func_080002b8
	.thumb_func
Func_080002b8:
	overlay_veneer Blend_SetBrightenTarget16
	.global Func_080002c0
	.thumb_func
Func_080002c0:
	overlay_veneer Blend_SetBrightenTarget0
	.global Func_080002c8
	.thumb_func
Func_080002c8:
	overlay_veneer Blend_ConfigureTransition
	.global Blend_WaitForTransitionFar
	.thumb_func
Blend_WaitForTransitionFar:
	overlay_veneer Blend_WaitForTransition
	.global Func_080002d8
	.thumb_func
Func_080002d8:
	overlay_veneer Func_080132b8
	.global Func_080002e0
	.thumb_func
Func_080002e0:
	overlay_veneer Func_080132bc
	.global Func_080002e8
	.thumb_func
Func_080002e8:
	overlay_veneer Func_080132c0
	.global Func_080002f0
	.thumb_func
Func_080002f0:
	overlay_veneer Func_080132c4
	.global Func_080002f8
	.thumb_func
Func_080002f8:
	overlay_veneer Func_080132c8
	.global Func_08000300
	.thumb_func
Func_08000300:
	overlay_veneer Func_08016180
	.global Func_08000308
	.thumb_func
Func_08000308:
	overlay_veneer Func_08016348
	.global QueueIoWriteDelay1Far
	.thumb_func
QueueIoWriteDelay1Far:
	overlay_veneer QueueIoWriteDelay1
	.global QueueIoWriteDelay2Far
	.thumb_func
QueueIoWriteDelay2Far:
	overlay_veneer QueueIoWriteDelay2
	.global QueueIoWriteDelay3Far
	.thumb_func
QueueIoWriteDelay3Far:
	overlay_veneer QueueIoWriteDelay3
	.global QueueIoWriteDelay5Far
	.thumb_func
QueueIoWriteDelay5Far:
	overlay_veneer QueueIoWriteDelay5
	.global QueueIoWriteDelay6Far
	.thumb_func
QueueIoWriteDelay6Far:
	overlay_veneer QueueIoWriteDelay6
	.global QueueIoWriteDelay7Far
	.thumb_func
QueueIoWriteDelay7Far:
	overlay_veneer QueueIoWriteDelay7
	.global QueueIoWriteDelay9Far
	.thumb_func
QueueIoWriteDelay9Far:
	overlay_veneer QueueIoWriteDelay9
	.global QueueIoWriteDelay10Far
	.thumb_func
QueueIoWriteDelay10Far:
	overlay_veneer QueueIoWriteDelay10
	.global QueueIoWriteDelay11Far
	.thumb_func
QueueIoWriteDelay11Far:
	overlay_veneer QueueIoWriteDelay11
	.global Runtime_SetMainState19Far
	.thumb_func
Runtime_SetMainState19Far:
	overlay_veneer Runtime_SetMainState19
	.global Graphics_PrepareTransferAndRunFar
	.thumb_func
Graphics_PrepareTransferAndRunFar:
	overlay_veneer Graphics_PrepareTransferAndRun
	.global SerialRuntime_WaitForStatusMaskFar
	.thumb_func
SerialRuntime_WaitForStatusMaskFar:
	overlay_veneer SerialRuntime_WaitForStatusMask
	.global SerialRuntime_RemoveIrqHandlersFar
	.thumb_func
SerialRuntime_RemoveIrqHandlersFar:
	overlay_veneer SerialRuntime_RemoveIrqHandlers
	.global Sound_LoadPresetParametersFar
	.thumb_func
Sound_LoadPresetParametersFar:
	overlay_veneer Sound_LoadPresetParameters
	.global Func_08000380
	.thumb_func
Func_08000380:
	overlay_veneer Func_0801680c
	.global Party_CheckFar
	.thumb_func
Party_CheckFar:
	overlay_veneer SerialRuntime_BeginTransferB
	.global Func_08000390
	.thumb_func
Func_08000390:
	overlay_veneer Func_080168a0
	.global SerialRuntime_WaitForTransferBFar
	.thumb_func
SerialRuntime_WaitForTransferBFar:
	overlay_veneer SerialRuntime_WaitForTransferB
	.global Func_080003a0
	.thumb_func
Func_080003a0:
	overlay_veneer Func_080168f8
	.global Func_080003a8
	.thumb_func
Func_080003a8:
	overlay_veneer Func_0801692c
	.global Func_080003b0
	.thumb_func
Func_080003b0:
	overlay_veneer Func_08016990
	.global Scheduler_SetCallbackMaskFar
	.thumb_func
Scheduler_SetCallbackMaskFar:
	overlay_veneer Scheduler_SetCallbackMask
	.global Owner_GetStateFar
	.thumb_func
Owner_GetStateFar:
	overlay_veneer Owner_GetState
	.global GameFlag_TestFar
	.thumb_func
GameFlag_TestFar:
	overlay_veneer GameFlag_Test
	.global GameFlag_SetBitFar
	.thumb_func
GameFlag_SetBitFar:
	overlay_veneer GameFlag_SetBit
	.global GameFlag_ClearBitFar
	.thumb_func
GameFlag_ClearBitFar:
	overlay_veneer GameFlag_ClearBit
	.global Func_080003e0
	.thumb_func
Func_080003e0:
	overlay_veneer Func_08016d34
	.global GameFlag_GetByteFar
	.thumb_func
GameFlag_GetByteFar:
	overlay_veneer GameFlag_GetByte
	.global GameFlag_SetByteFar
	.thumb_func
GameFlag_SetByteFar:
	overlay_veneer GameFlag_SetByte
	.global Func_080003f8
	.thumb_func
Func_080003f8:
	overlay_veneer Func_08016d7c
	.global Func_08000400
	.thumb_func
Func_08000400:
	overlay_veneer Func_08016d98
	.global GameFlag_GetNibbleFar
	.thumb_func
GameFlag_GetNibbleFar:
	overlay_veneer GameFlag_GetNibble
	.global Func_08000410
	.thumb_func
Func_08000410:
	overlay_veneer Func_08016dd0
	.global Scheduler_FindCallbackFar
	.thumb_func
Scheduler_FindCallbackFar:
	overlay_veneer Scheduler_FindCallback
	.global Func_08000420
	.thumb_func
Func_08000420:
	overlay_veneer Func_08014220
