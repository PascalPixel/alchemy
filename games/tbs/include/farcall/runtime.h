/// @file
/// Frame scheduling, memory, resources and core runtime services.
/// Far-call interfaces; their ordered jump stubs live in
/// games/tbs/src/system/far_call/RUNTIME.S.

void WaitFramesFar(s32 frames);
s32 Scheduler_AddOrUpdateCallbackFar(SchedulerCallback callback, s32 order);
s32 Scheduler_RemoveCallbackFar(SchedulerCallback callback);
u32 Random16Far(void);
s32 ArcTan2Far(s32 x, s32 y);
s32 FixedSqrtFar(s32 value);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, void *position);
void Runtime_SetIrqHandlerFar(u32 irq, s32 vcount, InterruptHandler handler);
void *Runtime_AllocateHeapBlockFar(s32 kind, s32 size);
void *Runtime_AllocateBlockFar(s32 kind, s32 size);
/// Far call to MemAlloc.
void *Runtime_BumpAllocateFar(s32 size);
void *Runtime_BumpAllocateAlternatePoolFar(s32 size);
s32 Resource_DecodeByteLzFar(const void *source, void *destination);
s32 Resource_DecodeType01Far(const void *source, void *destination);
s32 Resource_ResetEntryFar(u32 no);
s32 VramCache_ReleaseVramFar(u32 no);
s32 VramCache_LoadFar(u32 no, u32 size, const void *src);
s32 Resource_FindFreeEntryFar(void);
s32 AffineMatrix_BuildForEffectFar(struct AffineTransform *source);
void Runtime_PushSlotEntryFar(void *slot_entry, s32 slot);
void Ui_LoadWindowGraphicsFar(void);
void *Resource_GetTableEntryFar(s32 index);
void RuntimeDispatch_NoOpHookFar(s32 unused);
void Fade_ToBlackFar(s32 frames);
void Fade_WaitFar(void);
void SerialRuntime_InitializeFar(void);
s32 SerialRuntime_PollAndTransferFar(void *send, void *receive);
void SerialRuntime_RemoveIrqHandlersFar(void);
s32 Sound_LoadPresetParametersFar(s32 preset);
s32 SerialRuntime_BeginSendFar(void *source, s32 transferValue);
s32 SerialRuntime_BeginReceiveFar(void *destination);
s32 SerialRuntime_GetActiveTransfersFar(void);
s32 Scheduler_SetTaskFlagsFar(SchedulerCallback callback, u32 mask);
