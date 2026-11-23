@ The System bank: frames and tasks, memory, resources and the other services
@ every division calls.
	.include "games/tbs/src/gamepak/far_call/table.inc"
	.global FarCall_RuntimeTable
FarCall_RuntimeTable:
	export waitFar, wait
	overlay_veneer Task_ResetAll
	export Task_AddFar, Task_Add
	export Task_RemoveFar, Task_Remove
	overlay_veneer Task_SuspendAll
	overlay_veneer Task_ResumeAll
	overlay_veneer Task_RunPass
	export Random16Far, Random16
	export Trig_AtanFar, Trig_Atan
	overlay_veneer Math_IntegerSqrt
	export Math_SqrtFXFar, Math_SqrtFX
	export Trig_SinFar, Trig_Sin
	export Trig_CosFar, Trig_Cos
	export Vector_AddPolarOffsetFar, Vector_AddPolarOffset
	export Irq_SetHandlerFar, Irq_SetHandler
	overlay_veneer Heap_Initialize
	export HeapSlot_AllocFar, HeapSlot_Alloc
	export HeapSlot_AllocEwramFar, HeapSlot_AllocEwram
	export HeapSlot_ReleaseFar, HeapSlot_Release
	overlay_veneer Heap_GetRemainingIwram
	overlay_veneer Heap_GetRemainingEwram
	export Heap_AllocFar, Heap_Alloc
	export Heap_AllocEwramFar, Heap_AllocEwram
	export Heap_ReleaseFar, Heap_Release
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
	overlay_veneer Matrix_Initialize
	overlay_veneer Matrix_ResetUnit
	overlay_veneer Matrix_Push
	overlay_veneer Matrix_Pop
	overlay_veneer Matrix_Rotate
	overlay_veneer Matrix_RotateX
	overlay_veneer Matrix_RotateY
	overlay_veneer Matrix_RotateZ
	overlay_veneer Matrix_TranslateVector
	overlay_veneer Matrix_ScaleVector
	overlay_veneer Matrix_RotateTranslate
	overlay_veneer Matrix_RotateTranslateScale
	overlay_veneer Matrix_SetLookAt
	overlay_veneer Matrix_ProjectPoint
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
	overlay_veneer Matrix_LookAt
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
