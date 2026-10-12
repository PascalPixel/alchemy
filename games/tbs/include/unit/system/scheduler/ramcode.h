#ifndef ALCHEMY_UNIT_SYSTEM_SCHEDULER_RAMCODE_H
#define ALCHEMY_UNIT_SYSTEM_SCHEDULER_RAMCODE_H

#include "types.h"
#include "callback_scheduler.h"
#include "irq.h"

void Resource_DecompressHalfwords(const void *source, void *destination);
void ColorBuffer_BackupAndBrighten(u8 *buffer, u32 amount, u8 *backup, u32 bytes);
void ColorBuffer_BackupAndDarken(u8 *buffer, u32 amount, u8 *backup, u32 bytes);
void ColorBuffer_BackupAndScaleThreeQuarters(u8 *buffer, u8 *backup, u32 bytes);
void ColorBuffer_BackupAndHalve(u8 *buffer, u8 *backup, u32 bytes);
void ColorBuffer_BackupAndBrightenPartial(u8 *buffer, u32 amount, u8 *backup, u32 bytes);
void ColorBuffer_BackupAndDarkenPartial(u8 *buffer, u32 amount, u8 *backup, u32 bytes);
void ColorBuffer_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup, u32 bytes);
void ColorBuffer_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes);
s32 SaveState_InitializeWorkspace(void);
u32 SaveState_SelectWriteSlot(s32 mode);
u32 SaveState_WriteWorkspaceSlot(s32 sector);
s32 SaveState_ReadSlotAndCheckChecksum(s32 index);
u16 SaveState_EraseSlotSector(u16 value);
s32 SaveState_WriteRecord(s32 record_id, void *source);
u32 SaveState_ReadRecordPayload(s32 record_id, void *destination);
u32 SaveState_DeleteRecord(s32 record_id);
s32 SaveState_ChecksumWorkspace(void);
u32 SaveState_FindLatestSlot(s32 record_id);
s32 SaveState_InvalidateSlot(s32 index);
s32 SaveState_CompareBytes(u8 *left, u8 *right, s32 count);
u32 SaveState_GetLatestSequence(s32 record_id);
s32 SaveState_LoadSummaryRecords(void);
u32 SaveState_ReleaseWorkspace(void);

#endif
