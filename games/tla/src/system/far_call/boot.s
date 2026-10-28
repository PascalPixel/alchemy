.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global WaitFramesFar
	.thumb_func
WaitFramesFar:
	overlay_veneer WaitFrames
	.global Task_ResetAllFar
	.thumb_func
Task_ResetAllFar:
	overlay_veneer Task_ResetAll
	.global Task_AddFar
	.thumb_func
Task_AddFar:
	overlay_veneer Task_Add
	.global Func_080000d8
	.thumb_func
Func_080000d8:
	overlay_veneer Task_Remove
	.global Task_SuspendAllFar
	.thumb_func
Task_SuspendAllFar:
	overlay_veneer Task_SuspendAll
	.global Task_ResumeAllFar
	.thumb_func
Task_ResumeAllFar:
	overlay_veneer Task_ResumeAll
	.global Task_RunPassFar
	.thumb_func
Task_RunPassFar:
	overlay_veneer Task_RunPass
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
	.global Irq_SetHandlerFar
	.thumb_func
Irq_SetHandlerFar:
	overlay_veneer Irq_SetHandler
	.global Heap_InitializeFar
	.thumb_func
Heap_InitializeFar:
	overlay_veneer Heap_Initialize
	.global Func_08000140
	.thumb_func
Func_08000140:
	overlay_veneer HeapSlot_Alloc
	.global HeapSlot_AllocEwramFar
	.thumb_func
HeapSlot_AllocEwramFar:
	overlay_veneer HeapSlot_AllocEwram
	.global HeapSlot_ReleaseFar
	.thumb_func
HeapSlot_ReleaseFar:
	overlay_veneer HeapSlot_Release
	.global Heap_GetRemainingIwramFar
	.thumb_func
Heap_GetRemainingIwramFar:
	overlay_veneer Heap_GetRemainingIwram
	.global Heap_GetRemainingEwramFar
	.thumb_func
Heap_GetRemainingEwramFar:
	overlay_veneer Heap_GetRemainingEwram
	.global Heap_AllocFar
	.thumb_func
Heap_AllocFar:
	overlay_veneer Heap_Alloc
	.global Heap_AllocEwramFar
	.thumb_func
Heap_AllocEwramFar:
	overlay_veneer Heap_AllocEwram
	.global Sys_FreeFar
	.thumb_func
Sys_FreeFar:
	overlay_veneer Heap_Release
	.global DebugText_PrintFar
	.thumb_func
DebugText_PrintFar:
	overlay_veneer DebugText_Print
	.global DebugText_PrintHexFar
	.thumb_func
DebugText_PrintHexFar:
	overlay_veneer DebugText_PrintHex
	.global DebugText_PrintDecimalFar
	.thumb_func
DebugText_PrintDecimalFar:
	overlay_veneer DebugText_PrintDecimal
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
	.global VramCache_ResetFar
	.thumb_func
VramCache_ResetFar:
	overlay_veneer VramCache_Reset
	.global Resource_ResetEntryFar
	.thumb_func
Resource_ResetEntryFar:
	overlay_veneer Resource_ResetEntry
	.global VramCache_ReleaseVramFar
	.thumb_func
VramCache_ReleaseVramFar:
	overlay_veneer VramCache_ReleaseVram
	.global VramCache_LoadFar
	.thumb_func
VramCache_LoadFar:
	overlay_veneer VramCache_Load
	.global Resource_FindFreeEntryFar
	.thumb_func
Resource_FindFreeEntryFar:
	overlay_veneer Resource_FindFreeEntry
	.global DebugText_ClearFar
	.thumb_func
DebugText_ClearFar:
	overlay_veneer Func_08014b70
	.global AffineMatrix_BuildForEffectFar
	.thumb_func
AffineMatrix_BuildForEffectFar:
	overlay_veneer AffineMatrix_BuildForEffect
	.global Oam_AddSpriteAtDepthFar
	.thumb_func
Oam_AddSpriteAtDepthFar:
	overlay_veneer Func_080140d8
	.global Window_LoadGraphicsFar
	.thumb_func
Window_LoadGraphicsFar:
	overlay_veneer Window_LoadGraphics
	.global Palette_LoadBasicObjColorsFar
	.thumb_func
Palette_LoadBasicObjColorsFar:
	overlay_veneer Palette_LoadBasicObjColors
	.global Render_ResetTransformStateFar
	.thumb_func
Render_ResetTransformStateFar:
	overlay_veneer Render_ResetTransformState
	.global SceneTransform_ResetMatrixFar
	.thumb_func
SceneTransform_ResetMatrixFar:
	overlay_veneer SceneTransform_ResetMatrix
	.global Transform_PushFar
	.thumb_func
Transform_PushFar:
	overlay_veneer Transform_Push
	.global Transform_PopFar
	.thumb_func
Transform_PopFar:
	overlay_veneer Transform_Pop
	.global SceneTransform_ApplyRotationFar
	.thumb_func
SceneTransform_ApplyRotationFar:
	overlay_veneer SceneTransform_ApplyRotation
	.global SceneTransform_ApplyPitchFar
	.thumb_func
SceneTransform_ApplyPitchFar:
	overlay_veneer SceneTransform_ApplyPitch
	.global SceneTransform_ApplyYawFar
	.thumb_func
SceneTransform_ApplyYawFar:
	overlay_veneer SceneTransform_ApplyYaw
	.global SceneTransform_ApplyRollFar
	.thumb_func
SceneTransform_ApplyRollFar:
	overlay_veneer Func_080150e4
	.global SceneTransform_ApplyPositionFar
	.thumb_func
SceneTransform_ApplyPositionFar:
	overlay_veneer SceneTransform_ApplyPosition
	.global SceneTransform_ApplyScaleFar
	.thumb_func
SceneTransform_ApplyScaleFar:
	overlay_veneer Func_080151ac
	.global SceneTransform_ApplyRotationTranslationFar
	.thumb_func
SceneTransform_ApplyRotationTranslationFar:
	overlay_veneer SceneTransform_ApplyRotationTranslation
	.global SceneTransform_ApplyScaledRotationFar
	.thumb_func
SceneTransform_ApplyScaledRotationFar:
	overlay_veneer SceneTransform_ApplyScaledRotation
	.global Transform_SetLookAtFar
	.thumb_func
Transform_SetLookAtFar:
	overlay_veneer Transform_SetLookAt
	.global Render_ProjectPointFar
	.thumb_func
Render_ProjectPointFar:
	overlay_veneer Render_ProjectPoint
	.global Projection_SetFieldOfViewFar
	.thumb_func
Projection_SetFieldOfViewFar:
	overlay_veneer Projection_SetFieldOfView
	.global Projection_SetFar
	.thumb_func
Projection_SetFar:
	overlay_veneer Projection_Set
	.global Func_08000280
	.thumb_func
Func_08000280:
	overlay_veneer Func_080132d0
	.global System_ReturnZeroFar
	.thumb_func
System_ReturnZeroFar:
	overlay_veneer System_ReturnZero
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
	.global Fade_ToBlackFar
	.thumb_func
Fade_ToBlackFar:
	overlay_veneer Fade_ToBlack
	.global Fade_FromBlackFar
	.thumb_func
Fade_FromBlackFar:
	overlay_veneer Fade_FromBlack
	.global Fade_ToWhiteFar
	.thumb_func
Fade_ToWhiteFar:
	overlay_veneer Fade_ToWhite
	.global Fade_FromWhiteFar
	.thumb_func
Fade_FromWhiteFar:
	overlay_veneer Fade_FromWhite
	.global Fade_StartFar
	.thumb_func
Fade_StartFar:
	overlay_veneer Fade_Start
	.global Fade_WaitFar
	.thumb_func
Fade_WaitFar:
	overlay_veneer Fade_Wait
	.global RuntimeDispatch_ReservedNoOpAFar
	.thumb_func
RuntimeDispatch_ReservedNoOpAFar:
	overlay_veneer Func_080132b8
	.global RuntimeDispatch_ReservedNoOpBFar
	.thumb_func
RuntimeDispatch_ReservedNoOpBFar:
	overlay_veneer Func_080132bc
	.global RuntimeDispatch_ReservedNoOpCFar
	.thumb_func
RuntimeDispatch_ReservedNoOpCFar:
	overlay_veneer Func_080132c0
	.global RuntimeDispatch_ReservedNoOpDFar
	.thumb_func
RuntimeDispatch_ReservedNoOpDFar:
	overlay_veneer Func_080132c4
	.global RuntimeDispatch_ReservedNoOpEFar
	.thumb_func
RuntimeDispatch_ReservedNoOpEFar:
	overlay_veneer Func_080132c8
	.global Serial_InitializeFar
	.thumb_func
Serial_InitializeFar:
	overlay_veneer Func_08016180
	.global Serial_PollAndTransferFar
	.thumb_func
Serial_PollAndTransferFar:
	overlay_veneer Serial_PollAndTransfer
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
	.global Input_ResetKeyRepeatFar
	.thumb_func
Input_ResetKeyRepeatFar:
	overlay_veneer Input_ResetKeyRepeat
	.global Transform_ApplyLookAtFar
	.thumb_func
Transform_ApplyLookAtFar:
	overlay_veneer Transform_ApplyLookAt
	.global Serial_WaitForStatusMaskFar
	.thumb_func
Serial_WaitForStatusMaskFar:
	overlay_veneer Serial_WaitForStatusMask
	.global Serial_RemoveIrqHandlersFar
	.thumb_func
Serial_RemoveIrqHandlersFar:
	overlay_veneer Serial_RemoveIrqHandlers
	.global Sound_LoadPresetParametersFar
	.thumb_func
Sound_LoadPresetParametersFar:
	overlay_veneer Sound_LoadPresetParameters
	.global Func_08000380
	.thumb_func
Func_08000380:
	overlay_veneer Serial_StartSend
	.global Party_CheckFar
	.thumb_func
Party_CheckFar:
	overlay_veneer Serial_StartReceive
	.global Func_08000390
	.thumb_func
Func_08000390:
	overlay_veneer Serial_WaitForSend
	.global Serial_WaitForReceiveFar
	.thumb_func
Serial_WaitForReceiveFar:
	overlay_veneer Serial_WaitForReceive
	.global Serial_WaitForTransfersFar
	.thumb_func
Serial_WaitForTransfersFar:
	overlay_veneer Serial_WaitForTransfers
	.global Serial_GetActiveTransfersFar
	.thumb_func
Serial_GetActiveTransfersFar:
	overlay_veneer Serial_GetActiveTransfers
	.global Serial_StepBlockTransferFar
	.thumb_func
Serial_StepBlockTransferFar:
	overlay_veneer Func_08016990
	.global Task_SetFlagsFar
	.thumb_func
Task_SetFlagsFar:
	overlay_veneer Task_SetFlags
	.global Unit_GetFar
	.thumb_func
Unit_GetFar:
	overlay_veneer Unit_Get
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
	.global GameFlag_ToggleFar
	.thumb_func
GameFlag_ToggleFar:
	overlay_veneer GameFlag_Toggle
	.global GameFlag_GetByteFar
	.thumb_func
GameFlag_GetByteFar:
	overlay_veneer GameFlag_GetByte
	.global GameFlag_SetByteFar
	.thumb_func
GameFlag_SetByteFar:
	overlay_veneer GameFlag_SetByte
	.global GameFlag_IncrementByteFar
	.thumb_func
GameFlag_IncrementByteFar:
	overlay_veneer Func_08016d7c
	.global GameFlag_DecrementByteFar
	.thumb_func
GameFlag_DecrementByteFar:
	overlay_veneer Func_08016d98
	.global GameFlag_GetNibbleFar
	.thumb_func
GameFlag_GetNibbleFar:
	overlay_veneer GameFlag_GetNibble
	.global Func_08000410
	.thumb_func
Func_08000410:
	overlay_veneer Func_08016dd0
	.global Task_FindFar
	.thumb_func
Task_FindFar:
	overlay_veneer Task_Find
	.global Func_08000420
	.thumb_func
Func_08000420:
	overlay_veneer Func_08014220
