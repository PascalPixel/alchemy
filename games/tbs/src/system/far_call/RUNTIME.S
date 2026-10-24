@ Frame tasks, memory, resources and core runtime services.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_RuntimeTable
FarCall_RuntimeTable:
	export WaitFramesFar, WaitFrames
	overlay_veneer Task_ResetAll
	export Task_AddFar, Task_Add
	export Task_RemoveFar, Task_Remove
	overlay_veneer Task_SuspendAll
	overlay_veneer Task_ResumeAll
	overlay_veneer Task_RunPass
	export Random16Far, Random16
	export ArcTan2Far, ArcTan2
	overlay_veneer Math_IntegerSqrt
	export FixedSqrtFar, FixedSqrt
	export Trig_SinFar, Trig_Sin
	export Trig_CosFar, Trig_Cos
	export Vector_AddPolarOffsetFar, Vector_AddPolarOffset
	export Irq_SetHandlerFar, Irq_SetHandler
	overlay_veneer InitHeap
	export AllocHeapSlotFar, AllocHeapSlot
	export AllocHeapSlotEwramFar, AllocHeapSlotEwram
	export FreeHeapSlotFar, FreeHeapSlot
	overlay_veneer Heap_GetRemainingIwram
	overlay_veneer Heap_GetRemainingEwram
	export AllocFar, Alloc
	export AllocEwramFar, AllocEwram
	export Sys_FreeFar, Free
	overlay_veneer DebugText_Print
	overlay_veneer DebugText_PrintHex
	overlay_veneer DebugText_PrintDecimal
	overlay_veneer DebugText_PrintBlanks
	export Decompress_ByteLzFar, Decompress_ByteLz
	export Decompress_TaggedLzFar, Decompress_TaggedLz
	overlay_veneer VramCache_Reset
	export VramCache_FreeEntryFar, VramCache_FreeEntry
	export VramCache_ReleaseVramFar, VramCache_ReleaseVram
	export VramCache_LoadFar, VramCache_Load
	export VramCache_FindFreeEntryFar, VramCache_FindFreeEntry
	overlay_veneer DebugText_Clear
	export OamMatrix_AllocFromTransformFar, OamMatrix_AllocFromTransform
	export Oam_AddSpriteAtDepthFar, Oam_AddSpriteAtDepth
	export Window_LoadGraphicsFar, Window_LoadGraphics
	overlay_veneer Palette_LoadBasicSpriteColors
	overlay_veneer Transform_Reset
	overlay_veneer Transform_ResetMatrix
	overlay_veneer Transform_Push
	overlay_veneer Transform_Pop
	overlay_veneer Transform_ApplyRotation
	overlay_veneer Transform_ApplyPitch
	overlay_veneer Transform_ApplyYaw
	overlay_veneer Transform_ApplyRoll
	overlay_veneer Transform_ApplyPosition
	overlay_veneer Transform_ApplyScale
	overlay_veneer Transform_ApplyRotationTranslation
	overlay_veneer Transform_ApplyScaledRotation
	overlay_veneer Transform_SetLookAt
	overlay_veneer Transform_ProjectPoint
	overlay_veneer Projection_SetFieldOfView
	overlay_veneer Projection_Set
	overlay_veneer System_CopyBuildStamp
	overlay_veneer Stub_ReturnZero
	export Resource_GetTableEntryFar, Resource_GetTableEntry
	export Stub_NoOpHookFar, Stub_NoOpHook
	overlay_veneer Overlay_Load
	export ScreenFade_ToBlackFar, ScreenFade_ToBlack
	overlay_veneer ScreenFade_FromBlack
	overlay_veneer ScreenFade_ToWhite
	overlay_veneer ScreenFade_FromWhite
	overlay_veneer ScreenFade_Start
	export ScreenFade_WaitFar, ScreenFade_Wait
	overlay_veneer Stub_ReservedNoOpA
	overlay_veneer Stub_ReservedNoOpB
	overlay_veneer Stub_ReservedNoOpC
	overlay_veneer Stub_ReservedNoOpD
	overlay_veneer Stub_ReservedNoOpE
	export Serial_InitializeFar, Serial_Initialize
	export Serial_PollAndTransferFar, Serial_PollAndTransfer
	overlay_veneer IoWriteQueue_Store8
	export IoWriteQueue_Store16Far, IoWriteQueue_Store16
	export IoWriteQueue_Store32Far, IoWriteQueue_Store32
	overlay_veneer IoWriteQueue_SetBits8
	overlay_veneer IoWriteQueue_SetBits16
	overlay_veneer IoWriteQueue_SetBits32
	overlay_veneer IoWriteQueue_ClearBits8
	overlay_veneer IoWriteQueue_ClearBits16
	overlay_veneer IoWriteQueue_ClearBits32
	overlay_veneer Input_ResetKeyRepeat
	overlay_veneer Transform_ApplyLookAt
	overlay_veneer Serial_WaitForStatusMask
	export Serial_RemoveIrqHandlersFar, Serial_RemoveIrqHandlers
	export Sound_LoadReverbCodeFar, Sound_LoadReverbCode
	export Serial_StartSendFar, Serial_StartSend
	export Serial_StartReceiveFar, Serial_StartReceive
	overlay_veneer Serial_WaitForSend
	overlay_veneer Serial_WaitForReceive
	overlay_veneer Serial_WaitForTransfers
	export Serial_GetActiveTransfersFar, Serial_GetActiveTransfers
	overlay_veneer Serial_StepBlockTransfer
	export Task_SetFlagsFar, Task_SetFlags
