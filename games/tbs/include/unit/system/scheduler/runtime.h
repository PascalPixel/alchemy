#ifndef ALCHEMY_UNIT_SYSTEM_SCHEDULER_RUNTIME_H
#define ALCHEMY_UNIT_SYSTEM_SCHEDULER_RUNTIME_H

#include "types.h"
#include "callback_scheduler.h"
#include "irq.h"

struct AffineTransform;

void System_Initialize(void);
void Runtime_IdleForever(void);
s32 System_GetLanguage(void);
void RuntimeDispatch_ReservedNoOpA(void);
void RuntimeDispatch_ReservedNoOpB(void);
void RuntimeDispatch_ReservedNoOpC(void);
void RuntimeDispatch_ReservedNoOpD(void);
void RuntimeDispatch_ReservedNoOpE(void);
s32 RuntimeDispatch_ReturnZero(s32 unused0, const u8 *unused1);
void Resource_LoadWorkHeader(void);
void RuntimeDispatch_NoOpHook(s32 unused);
void *Resource_GetTableEntry(s32 index);
void PackedTable_AdjustMarkedOffsets(u16 *entries, s32 byte_length);
void Resource_LoadCode(u32 index, void *destination);
void Runtime_IgnoreInterrupt(void);
void Runtime_InstallIwramAndIrqs(void);
void Runtime_SetIrqHandler(u32 irq, s32 vcount, InterruptHandler handler);
void WaitFrames(s32 frames);
void Runtime_SetMainState19(void);
void Input_UpdateKeyRepeatAndDirection(void);
void System_VBlankHandler(void);
s32 Sound_LoadPresetParameters(s32 preset);
void Sound_ReservedNoOpA(void);
void Sound_ReservedNoOpB(void);
void Input_HandleKeyInterrupt(void);
void IoWriteQueue_FlushPending(void);
void BlendTransition_Update(void);
void Blend_SetDarkenTarget16(s32 frames);
void Blend_SetDarkenTarget0(s32 frames);
void Blend_SetBrightenTarget16(s32 frames);
void Blend_SetBrightenTarget0(s32 frames);
void Blend_ConfigureTransition(s8 bright, s32 layer, u32 start, s32 target, s32 frames);
void Blend_WaitForTransition(void);
void Graphics_ResetFrameState(void);
s32 AffineMatrix_BuildForEffect(struct AffineTransform *source);
void Runtime_PushSlotEntry(void *slot_entry, s32 slot);
void Render_BuildOamList(void *work);
s32 ResourceTable_AllocateBlocks(u32 id, u32 size);
s32 ResourceTable_GetLongestFreeBlockRun(void);
s32 Resource_ClearSlotReferences(s32 id);
s32 Resource_ResetEntry(u32 no);
s32 Resource_ActivateEntry(u32 no);
s32 VramBlock_LoadCached(u32 no, u32 size, const void *src);
void Resource_InitializeTable(void);
s32 Resource_FindFreeEntry(void);
s32 Resource_LoadIntoFreeSlot(s32 size);
s32 Resource_GetBuffer(s32 no, s32 src);
void Scheduler_ResetTaskTable(void);
void Scheduler_CopyWords(u32 *destination, u32 *source, u32 byte_count);
void Scheduler_SortTasks(void);
s32 Scheduler_FindCallback(SchedulerCallback callback);
s32 Scheduler_AddOrUpdateCallback(SchedulerCallback callback, s32 order);
void Scheduler_Idle(void);
void Scheduler_EmptyCallback(void);
s32 Scheduler_RemoveCallback(SchedulerCallback callback);
s32 Scheduler_EnableCallbacks(SchedulerCallback callback);
s32 Scheduler_EnableUnmaskedOverlayCallbacks(void);
s32 Scheduler_SetCallbackMask(SchedulerCallback callback, u32 mask);
s32 Scheduler_DisableCallbacks(SchedulerCallback callback);
s32 Scheduler_DisableOverlayCallbacks(void);
void Runtime_InvokeCallbacksByKey(s32 key);
u32 Random16(void);
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
s32 ArcTan2(s32 x, s32 y);
s32 Math_IntegerSqrt(s32 value);
s32 FixedSqrt(s32 value);
s32 Runtime_GetLowTableAddress(void);
void Text_FormatHexToWork(u32 value);
void Text_FormatSignedDecimalToWork(s32 arg0);
void Graphics_FillWordStreamWithF000(u32 count);
void *memcpy(void *destination, const void *source, unsigned long size);
void Runtime_WriteDebugTextTiles(const u8 *src);
void Text_DrawHexRightAligned(u32 value, s32 keta);
void Text_DrawSignedDecimalRightAligned(s32 value, s32 keta);
void Bg0_ClearTilemap(void);
void Ui_LoadWindowGraphics(void);
void PaletteDma_LoadBlock(void);
void Runtime_InitializeHeap(void);
s32 Runtime_GetRemainingIwram(void);
s32 Runtime_GetRemainingEwram(void);
void *Runtime_AllocateHeapBlock(s32 kind, s32 size);
void *Runtime_AllocateBlock(s32 kind, s32 size);
void *MemAlloc(s32 size);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void RuntimeMemory_ReservedNoOp(void);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_DecodeType2(const void *source, void *destination);
s32 Resource_DecodeByteLz(const void *source, void *destination);
void SerialRuntime_WaitForTransferA(void);
void SerialRuntime_WaitForTransferB(void);

#endif
