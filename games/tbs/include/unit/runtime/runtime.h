#ifndef ALCHEMY_UNIT_RUNTIME_RUNTIME_H
#define ALCHEMY_UNIT_RUNTIME_RUNTIME_H

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
void System_CopyBuildStamp(void);
void RuntimeDispatch_NoOpHook(s32 unused);
void *Resource_GetTableEntry(s32 index);
void Code_ResolveLongBranches(u16 *entries, s32 byte_length);
void Resource_LoadCode(u32 index, void *destination);
void Runtime_IgnoreInterrupt(void);
void Runtime_InstallIwramAndIrqs(void);
void Runtime_SetIrqHandler(u32 irq, s32 vcount, InterruptHandler handler);
void WaitFrames(s32 frames);
void Input_ResetKeyRepeat(void);
void Input_UpdateKeyRepeatAndDirection(void);
void System_VBlankHandler(void);
s32 Sound_LoadPresetParameters(s32 preset);
void Sound_ReservedNoOpA(void);
void Sound_ReservedNoOpB(void);
void Input_HandleKeyInterrupt(void);
void Fade_ToBlack(s32 frames);
void Fade_FromBlack(s32 frames);
void Fade_ToWhite(s32 frames);
void Fade_FromWhite(s32 frames);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_DecodeType2(const void *source, void *destination);
s32 Resource_DecodeByteLz(const void *source, void *destination);
void SerialRuntime_WaitForSend(void);
void SerialRuntime_WaitForReceive(void);

void IoWriteQueue_FlushPending(void);

void Fade_Update(void);
void Fade_Start(s8 bright, s32 layer, u32 start, s32 target, s32 frames);
void Fade_Wait(void);

void Render_ResetFrame(void);
s32 AffineMatrix_BuildForEffect(struct AffineTransform *source);
void Runtime_PushSlotEntry(void *slot_entry, s32 slot);
void Render_BuildOamList(void *work);

s32 VramCache_AllocateBlocks(u32 id, u32 size);
s32 VramCache_GetLongestFreeRun(void);
s32 VramCache_FreeBlocks(s32 id);
s32 Resource_ResetEntry(u32 no);
s32 VramCache_ReleaseVram(u32 no);
s32 VramCache_Load(u32 no, u32 size, const void *src);
void VramCache_Reset(void);
s32 Resource_FindFreeEntry(void);
s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 no, s32 src);

void Scheduler_ResetTaskTable(void);
void Scheduler_CopyWords(u32 *destination, u32 *source, u32 byte_count);
void Scheduler_SortTasks(void);
s32 Scheduler_FindCallback(SchedulerCallback callback);
s32 Scheduler_AddOrUpdateCallback(SchedulerCallback callback, s32 order);
void Scheduler_Idle(void);
void Scheduler_EmptyCallback(void);
s32 Scheduler_RemoveCallback(SchedulerCallback callback);
s32 Scheduler_SuspendTasks(SchedulerCallback callback);
s32 Scheduler_SuspendOverlayTasks(void);
s32 Scheduler_SetTaskFlags(SchedulerCallback callback, u32 mask);
s32 Scheduler_ResumeTasks(SchedulerCallback callback);
s32 Scheduler_ResumeOverlayTasks(void);
void Scheduler_RunPass(s32 key);

u32 Random16(void);
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
s32 ArcTan2(s32 x, s32 y);
s32 Math_IntegerSqrt(s32 value);
s32 FixedSqrt(s32 value);

s32 Palette_GetBasicObjColors(void);
void NumberText_FormatHex(u32 value);
void NumberText_FormatDecimal(s32 arg0);
void DebugText_PrintBlanks(u32 count);
void *memcpy(void *destination, const void *source, unsigned long size);
void DebugText_Print(const u8 *src);
void DebugText_PrintHex(u32 value, s32 digits);
void DebugText_PrintDecimal(s32 value, s32 digits);
void DebugText_Clear(void);
void Ui_LoadWindowGraphics(void);
void Palette_LoadBasicObjColors(void);

void Runtime_InitializeHeap(void);
s32 Runtime_GetRemainingIwram(void);
s32 Runtime_GetRemainingEwram(void);
void *Runtime_AllocateHeapBlock(s32 kind, s32 size);
void *Runtime_AllocateBlock(s32 kind, s32 size);
void *MemAlloc(s32 size);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void RuntimeMemory_ReservedNoOp(void);

void Render_ResetTransformState(void);
void Transform_Push(void);
void Transform_Store(void *dst);
void Transform_Load(const void *src);
void Transform_Pop(void);
void SceneTransform_ResetMatrix(void);
void SceneTransform_ApplyRotation(s32 *ang);
void SceneTransform_ApplyPitch(s32 ang);
void SceneTransform_ApplyYaw(s32 ang);
void SceneTransform_ApplyRoll(s32 ang);
void SceneTransform_ApplyPosition(const s32 *pos);
void SceneTransform_ApplyScale(const s32 *scale);
void SceneTransform_ApplyRotationTranslation(s32 *ang, s32 *pos);
void SceneTransform_ApplyScaledRotation(s32 *ang, s32 *pos, s32 *scale);
void Transform_BuildLookAt(s32 *eye, s32 *target, s32 *out);
void Transform_SetLookAt(s32 *eye, s32 *target);
void Transform_ApplyLookAt(s32 *eye, s32 *target);
void Projection_SetFieldOfView(u32 value, s32 param1, s32 param2);
void Projection_Set(u32 value0, u32 value1, u32 value2);
s32 Render_ProjectPoint(s32 *point, s32 *screen);

void Resource_DecompressHalfwords(const void *source, void *destination);
void ColorBuffer_BackupAndBrighten(u8 *buffer, u32 amount, u8 *backup,
                                   u32 bytes);
void ColorBuffer_BackupAndDarken(u8 *buffer, u32 amount, u8 *backup, u32 bytes);
void ColorBuffer_BackupAndScaleThreeQuarters(u8 *buffer, u8 *backup, u32 bytes);
void ColorBuffer_BackupAndHalve(u8 *buffer, u8 *backup, u32 bytes);
void ColorBuffer_BackupAndBrightenPartial(u8 *buffer, u32 amount, u8 *backup,
                                          u32 bytes);
void ColorBuffer_BackupAndDarkenPartial(u8 *buffer, u32 amount, u8 *backup,
                                        u32 bytes);
void ColorBuffer_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup,
                                                    u32 bytes);
void ColorBuffer_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes);
s32 SaveState_InitializeWorkspace(void);
u32 SaveState_SelectWriteSlot(s32 mode);
u32 SaveState_WriteWorkspaceSlot(s32 sector);
s32 SaveState_ReadSlotAndCheckChecksum(s32 index);
u16 SaveState_EraseSlotSector(u16 sector);
s32 SaveState_WriteRecord(s32 recordId, void *source);
u32 SaveState_ReadRecordPayload(s32 recordId, void *destination);
u32 SaveState_DeleteRecord(s32 recordId);
s32 SaveState_ChecksumWorkspace(void);
u32 SaveState_FindLatestSlot(s32 recordId);
s32 SaveState_InvalidateSlot(s32 index);
s32 SaveState_CompareBytes(u8 *left, u8 *right, s32 count);
u32 SaveState_GetLatestSequence(s32 recordId);
s32 SaveState_LoadSummaryRecords(void);
u32 SaveState_ReleaseWorkspace(void);

void SerialRuntime_Initialize(void);
void SerialRuntime_EnableTransferTimer(void);
void SerialRuntime_DisableTransfer(void);
s32 SerialRuntime_PollAndTransfer(void *send, void *receive);
s32 SerialRuntime_PollStatus(void);
s32 SerialRuntime_ExchangePayloads(void *send, void *receive);
void SerialRuntime_PrepareSendPacket(void *payload);
u8 SerialRuntime_CollectReceivedPayloads(void *payload);
void SerialRuntime_HandleTransferInterrupt(void);
void SerialRuntime_RemoveIrqHandlers(void);
u32 SerialRuntime_WaitForStatusMask(s32 mask);
s32 SerialRuntime_BeginSend(void *source, s32 transferValue);
s32 SerialRuntime_BeginReceive(void *destination);
void SerialRuntime_WaitForTransfers(void);
s32 SerialRuntime_GetActiveTransfers(void);
void BattleLink_ResetTransferState(void);
void SerialRuntime_StepBlockTransfer(void);
void RuntimeWait_BusyLoopTick(void);
void SerialTest_Run(void);

#endif
