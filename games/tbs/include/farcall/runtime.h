/// @file
/// Frame scheduling, memory, resources and core runtime services.
/// Far-call interfaces; their ordered jump stubs live in
/// games/tbs/src/system/far_call/RUNTIME.S.

/// Far call to WaitFrames.
void WaitFramesFar(s32 frames);
/// Far call to Scheduler_AddOrUpdateCallback.
s32 Scheduler_AddOrUpdateCallbackFar(SchedulerCallback callback, s32 order);
/// Far call to Scheduler_RemoveCallback.
s32 Scheduler_RemoveCallbackFar(SchedulerCallback callback);
/// Far call to Random16.
u32 Random16Far(void);
/// Far call to ArcTan2.
s32 ArcTan2Far(s32 x, s32 y);
/// Far call to FixedSqrt.
s32 FixedSqrtFar(s32 value);
/// Far call to Vector_AddPolarOffset.
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, void *position);
/// Far call to Runtime_SetIrqHandler.
void Runtime_SetIrqHandlerFar(u32 irq, s32 vcount, InterruptHandler handler);
/// Far call to Runtime_AllocateHeapBlock.
void *Runtime_AllocateHeapBlockFar(s32 kind, s32 size);
/// Far call to Runtime_AllocateBlock.
void *Runtime_AllocateBlockFar(s32 kind, s32 size);
/// Far call to MemAlloc.
void *Runtime_BumpAllocateFar(s32 size);
/// Far call to Runtime_BumpAllocateAlternatePool.
void *Runtime_BumpAllocateAlternatePoolFar(s32 size);
/// Far call to Resource_DecodeByteLz.
s32 Resource_DecodeByteLzFar(const void *source, void *destination);
/// Far call to Resource_DecodeType01.
s32 Resource_DecodeType01Far(const void *source, void *destination);
/// Far call to Resource_ResetEntry.
s32 Resource_ResetEntryFar(u32 no);
/// Far call to VramCache_ReleaseVram.
s32 VramCache_ReleaseVramFar(u32 no);
/// Far call to VramCache_Load.
s32 VramCache_LoadFar(u32 no, u32 size, const void *src);
/// Far call to Resource_FindFreeEntry.
s32 Resource_FindFreeEntryFar(void);
/// Far call to AffineMatrix_BuildForEffect.
s32 AffineMatrix_BuildForEffectFar(struct AffineTransform *source);
/// Far call to Runtime_PushSlotEntry.
void Runtime_PushSlotEntryFar(void *slot_entry, s32 slot);
/// Far call to Ui_LoadWindowGraphics.
void Ui_LoadWindowGraphicsFar(void);
/// Far call to Resource_GetTableEntry.
void *Resource_GetTableEntryFar(s32 index);
/// Far call to RuntimeDispatch_NoOpHook.
void RuntimeDispatch_NoOpHookFar(s32 unused);
/// Far call to Fade_ToBlack.
void Fade_ToBlackFar(s32 frames);
/// Far call to Fade_Wait.
void Fade_WaitFar(void);
/// Far call to SerialRuntime_Initialize.
void SerialRuntime_InitializeFar(void);
/// Far call to SerialRuntime_PollAndTransfer.
s32 SerialRuntime_PollAndTransferFar(void *send, void *receive);
/// Far call to SerialRuntime_RemoveIrqHandlers.
void SerialRuntime_RemoveIrqHandlersFar(void);
/// Far call to Sound_LoadPresetParameters.
s32 Sound_LoadPresetParametersFar(s32 preset);
/// Far call to SerialRuntime_BeginSend.
s32 SerialRuntime_BeginSendFar(void *source, s32 transferValue);
/// Far call to SerialRuntime_BeginReceive.
s32 SerialRuntime_BeginReceiveFar(void *destination);
/// Far call to SerialRuntime_GetActiveTransfers.
s32 SerialRuntime_GetActiveTransfersFar(void);
/// Far call to Scheduler_SetTaskFlags.
s32 Scheduler_SetTaskFlagsFar(SchedulerCallback callback, u32 mask);
