#ifndef ALCHEMY_UNIT_RUNTIME_RUNTIME_H
#define ALCHEMY_UNIT_RUNTIME_RUNTIME_H

#include "types.h"
#include "task.h"
#include "irq.h"

struct AffineTransform;

void System_Initialize(void);
void System_IdleForever(void);
/// The cartridge's language, from its header (System_GetLanguage).
enum CartridgeLanguage {
  CARTRIDGE_LANGUAGE_ENGLISH,
  CARTRIDGE_LANGUAGE_GERMAN,
  CARTRIDGE_LANGUAGE_FRENCH,
  CARTRIDGE_LANGUAGE_SPANISH,
  CARTRIDGE_LANGUAGE_ITALIAN
};
s32 System_GetLanguage(void);
void Stub_ReservedNoOpA(void);
void Stub_ReservedNoOpB(void);
void Stub_ReservedNoOpC(void);
void Stub_ReservedNoOpD(void);
void Stub_ReservedNoOpE(void);
s32 Stub_ReturnZero(s32 unused0, const u8 *unused1);
void System_CopyBuildStamp(void);
void Stub_NoOpHook(s32 unused);
void *Resource_GetTableEntry(s32 index);
void Code_ResolveLongBranches(u16 *entries, s32 byteLength);
void Overlay_Load(u32 index, void *destination);
void Irq_Ignore(void);
void System_InstallIwramAndIrqs(void);
void Irq_SetHandler(u32 irq, s32 vcount, InterruptHandler handler);
void WaitTicks(s32 ticks);
void Input_ResetKeyRepeat(void);
void Input_UpdateKeyRepeatAndDirection(void);
void System_VBlankHandler(void);
s32 Sound_LoadReverbCode(s32 code);
void Sound_ReservedNoOpA(void);
void Sound_ReservedNoOpB(void);
void Input_HandleKeyInterrupt(void);
void ScreenFade_ToBlack(s32 frames);
void ScreenFade_FromBlack(s32 frames);
void ScreenFade_ToWhite(s32 frames);
void ScreenFade_FromWhite(s32 frames);
s32 Decompress_TaggedLz(const void *source, void *destination);
s32 Decompress_MoveToFrontLz(const void *source, void *destination);
s32 Decompress_ByteLz(const void *source, void *destination);
void Serial_WaitForSend(void);
void Serial_WaitForReceive(void);

void IoWriteQueue_FlushPending(void);

void ScreenFade_Update(void);
void ScreenFade_Start(s8 bright, s32 layer, u32 start, s32 target, s32 frames);
void ScreenFade_Wait(void);

void Oam_ResetFrame(void);
s32 OamMatrix_AllocFromTransform(struct AffineTransform *source);
void Oam_AddSpriteAtDepth(void *slotEntry, s32 slot);
void Oam_BuildBuffer(void *work);

s32 VramCache_AllocateBlocks(u32 id, u32 size);
s32 VramCache_GetLongestFreeRun(void);
s32 VramCache_FreeBlocks(s32 id);
s32 VramCache_FreeEntry(u32 no);
s32 VramCache_ReleaseVram(u32 no);
s32 VramCache_Load(u32 no, u32 size, const void *src);
void VramCache_Reset(void);
s32 VramCache_FindFreeEntry(void);
s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 no, s32 src);

void Task_ResetAll(void);
void Task_CopyWords(u32 *destination, u32 *source, u32 byteCount);
void Task_Sort(void);
s32 Task_Find(TaskFunc callback);
s32 Task_Add(TaskFunc callback, s32 order);
void Task_Idle(void);
void Task_Empty(void);
s32 Task_Remove(TaskFunc callback);
s32 Task_SuspendAll(TaskFunc callback);
s32 Task_SuspendOverlayTasks(void);
s32 Task_SetFlags(TaskFunc callback, u32 mask);
s32 Task_ResumeAll(TaskFunc callback);
s32 Task_ResumeOverlayTasks(void);
void Task_RunPass(s32 key);

u32 Random16(void);
void Vector_AddPolarOffset(s32 radius, s32 angle, void *position);
s32 ArcTan2(s32 x, s32 y);
s32 Math_IntegerSqrt(s32 value);
s32 FixedSqrt(s32 value);

s32 Palette_GetBasicSpriteColors(void);
void String_FormatHex(u32 value);
void String_FormatDecimal(s32 number);
void DebugText_PrintBlanks(u32 count);
void *memcpy(void *destination, const void *source, unsigned long size);
void DebugText_Print(const u8 *src);
void DebugText_PrintHex(u32 value, s32 digits);
void DebugText_PrintDecimal(s32 value, s32 digits);
void DebugText_Clear(void);
void Window_LoadGraphics(void);
void Palette_LoadBasicSpriteColors(void);

void InitHeap(void);
s32 Heap_GetRemainingIwram(void);
s32 Heap_GetRemainingEwram(void);
void *AllocHeapSlot(s32 kind, s32 size);
void *AllocHeapSlotEwram(s32 kind, s32 size);
void *Alloc(s32 size);
void *AllocEwram(s32 size);
void Heap_ReservedNoOp(void);

void Transform_Reset(void);
void Transform_Push(void);
void Transform_Store(void *dst);
void Transform_Load(const void *src);
void Transform_Pop(void);
void Transform_ResetMatrix(void);
void Transform_ApplyRotation(s32 *angles);
void Transform_ApplyPitch(s32 angle);
void Transform_ApplyYaw(s32 angle);
void Transform_ApplyRoll(s32 angle);
void Transform_ApplyPosition(const s32 *position);
void Transform_ApplyScale(const s32 *scale);
void Transform_ApplyRotationTranslation(s32 *angles, s32 *position);
void Transform_ApplyScaledRotation(s32 *angles, s32 *position, s32 *scale);
void Transform_BuildLookAt(s32 *eye, s32 *target, s32 *out);
void Transform_SetLookAt(s32 *eye, s32 *target);
void Transform_ApplyLookAt(s32 *eye, s32 *target);
void Projection_SetFieldOfView(u32 fieldOfView, s32 nearDepth, s32 farDepth);
void Projection_Set(u32 focal, u32 nearDepth, u32 farDepth);
s32 Transform_ProjectPoint(const void *point, s32 *screen);

void Decompress_Halfwords(const void *source, void *destination);
void PaletteFade_BackupAndBrighten(u8 *buffer, u32 amount, u8 *backup,
                                   u32 bytes);
void PaletteFade_BackupAndDarken(u8 *buffer, u32 amount, u8 *backup, u32 bytes);
void PaletteFade_BackupAndScaleThreeQuarters(u8 *buffer, u8 *backup, u32 bytes);
void PaletteFade_BackupAndHalve(u8 *buffer, u8 *backup, u32 bytes);
void PaletteFade_BackupAndBrightenPartial(u8 *buffer, u32 amount, u8 *backup,
                                          u32 bytes);
void PaletteFade_BackupAndDarkenPartial(u8 *buffer, u32 amount, u8 *backup,
                                        u32 bytes);
void PaletteFade_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup,
                                                    u32 bytes);
void PaletteFade_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes);
s32 Save_InitializeWork(void);
u32 Save_SelectWriteSector(s32 mode);
u32 Save_WriteSector(s32 sector);
s32 Save_ReadSector(s32 index);
u16 Save_EraseSector(u16 sector);
s32 Save_WriteRecord(s32 recordId, void *source);
u32 Save_ReadRecord(s32 recordId, void *destination);
u32 Save_DeleteRecord(s32 recordId);
s32 Save_ChecksumSector(void);
u32 Save_FindLatestSector(s32 recordId);
s32 Save_InvalidateSector(s32 index);
s32 Save_CompareBytes(u8 *left, u8 *right, s32 count);
u32 Save_GetLatestCounter(s32 recordId);
s32 Save_LoadFileSummaries(void);
u32 Save_ReleaseWork(void);

void Serial_Initialize(void);
void Serial_EnableTransferTimer(void);
void Serial_DisableTransfer(void);
s32 Serial_PollAndTransfer(void *send, void *receive);
s32 Serial_PollStatus(void);
s32 Serial_ExchangePayloads(void *send, void *receive);
void Serial_PrepareSendPacket(void *payload);
u8 Serial_CollectReceivedPayloads(void *payload);
void Serial_HandleTransferInterrupt(void);
void Serial_RemoveIrqHandlers(void);
u32 Serial_WaitForStatusMask(s32 mask);
s32 Serial_StartSend(void *source, s32 size);
s32 Serial_StartReceive(void *destination);
void Serial_WaitForTransfers(void);
s32 Serial_GetActiveTransfers(void);
void LinkBattle_ResetTransferState(void);
void Serial_StepBlockTransfer(void);
void SerialTest_Delay(void);
void SerialTest_Run(void);

#endif
