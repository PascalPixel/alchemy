#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "project.h"
#include "types.h"
#include "iwram_call.h"
#include "scene.h"
#include "global_cells.h"
#include "transform.h"
#include "dma.h"
#include "ramcode.h"
#include "mem.h"
#include "io_reg.h"
#include "fixed_math.h"
#include "save_state.h"
#include "irq.h"
#include "runtime_interfaces.h"
#include "flash.h"

void Graphics_PrepareTransferInIwramWork(s32 *eye, s32 *target)
{
    Graphics_PrepareTransfer(eye, target, gTransform);
}

void Graphics_PrepareTransferAndRun(s32 *eye, s32 *target)
{
    s32 work[12];

    Graphics_PrepareTransfer(eye, target, work);
    Iwram_TransformMatrix(work);
}

void Camera_SetAngleParameters(u32 value, s32 param1, s32 param2)
{
    s32 half;
    s32 first;
    s32 result;

    half = (s32)(value + (value >> 31)) >> 1;
    first = Trig_Sin(half);
    result = Iwram_RatioMulQ14(
        first,
        Trig_Cos(half)* 0x50
    );
    gProjection.near = param1;
    gProjection.focal = result;
    gProjection.far = param2;
}

void Camera_StoreSceneParameters(u32 value0, u32 value1, u32 value2)
{
    struct Projection *work = &gProjection;

    work->focal = value0;
    work->near = value1;
    work->far = value2;
}

s32 Render_ProjectPoint(s32 *point, s32 *screen)
{
    struct Projection *projection;
    s32 depth;
    s32 scale;
    s32 result;

    Iwram_TransformVector(point, screen);
    projection = &gProjection;
    depth = -screen[2];
    result = 0;
    if (depth >= projection->near && depth <= projection->far) {
        screen[2] = depth >> 16;
        if (projection->focal != 0) {
            u32 shifted = (u32)depth >> 11;

            scale = Iwram_UnsignedDivide(projection->focal << 5, shifted);
        } else {
            scale = 0x151eb;
        }
        screen[0] = projection->center_x + Iwram_MulQ16(screen[0], scale) / 0x10000;
        screen[1] = projection->center_y - Iwram_MulQ16(screen[1], scale) / 0x10000;
        result = scale;
    }
    return result;
}

void Resource_DecompressHalfwords(const void *source, void *destination)
{
    u32 words = (u32)Resource_DecompressHalfwordsCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)Resource_DecodeHalfwordLz, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(const void *, void *))routine)(source, destination);
}

s32 Resource_DecodeType01(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;

    RamCode_Load(routine, Resource_DecodeType01Arm, Resource_DecodeType01CodeSize);
    result = routine(source, destination);
    MemFree(routine);
    return result;
}

s32 Resource_DecodeType2(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;

    RamCode_Load(routine, Resource_Decode2, Resource_Decode2CodeSize);
    result = routine(source, destination);
    MemFree(routine);
    return result;
}

s32 Resource_DecodeByteLz(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;

    RamCode_Load(routine, Resource_DecodeByteLzArm, Resource_DecodeByteLzCodeSize);
    result = routine(source, destination);
    MemFree(routine);
    return result;
}

void ColorBuffer_BackupAndBrighten(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndBrightenCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_BrightenKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndDarken(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndDarkenCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_DarkenKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndScaleThreeQuarters(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndScaleThreeQuartersCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_ScaleThreeQuartersKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

void ColorBuffer_BackupAndHalve(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndHalveCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_HalveKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

void ColorBuffer_BackupAndBrightenPartial(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndBrightenPartialCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_BrightenPartialKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndDarkenPartial(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndDarkenPartialCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_DarkenPartialKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndScaleNonzeroThreeQuartersCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_ScaleNonzeroThreeQuartersKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

void ColorBuffer_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndHalveNonzeroCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_HalveNonzeroKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

s32 SaveState_InitializeWorkspace(void)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    struct SaveSlotHeader *slot;
    volatile u32 zero;
    u32 index;

    work = Runtime_AllocateBlock(0x33, sizeof(*work));
    zero = 0;
    START_DMA(&zero, work, 0x85000440);
    SetFlashTimerIntr(2, &gIrqHandlers[5]);

    for (index = 0; index < 8; index++) {
        if ((u16)IdentifyFlash() == 0)
            break;
        WaitFrames(1);
    }
    if (index > 7)
        return 1;

    for (index = 0; index <= 15; index++) {
        u32 status;
        u32 record;
        u8 id;

        work->occupied[index] = 0;
        work->record_id[index] = 0x10;
        work->sequence[index] = 0;
        status = SaveState_ReadSlotAndCheckChecksum(index);
        START_DMA(&work->slot, &header, 0x84000004);
        WAIT_DMA();
        slot = &header;

        if (SaveState_CompareBytes(slot->signature, (u8 *)Save_Signature, 7) != 0)
            continue;
        work->sequence[index] = slot->sequence;
        record = slot->record_id;
        id = record;
        if (id > 15)
            continue;
        if (status != 0)
            continue;

        work->occupied[index] = 1;
        work->record_id[index] = record;
        if (status < index) {
            do {
                if (work->record_id[status] == (u8)record) {
                    if (work->sequence[status] < slot->sequence)
                        work->occupied[status] = 0;
                    else
                        work->occupied[index] = 0;
                }
                status++;
            } while (status < index);
        }
    }
    return 0;
}

u32 Random16(void);

u32 SaveState_SelectWriteSlot(s32 mode)
{
    u32 empty[16];
    s32 count;
    u32 index;
    u8 *slot;
    u8 value;

    slot = gSaveWorkspace->occupied;
    count = 0;
    index = 0;
    do {
        value = *slot++;
        if (value == 0) {
            empty[count++] = index;
        }
        index += 1;
    } while (index <= 0xFU);
    index = 0x10;
    if (count != 0) {
        if (count == 1) {
            index = empty[0];
            if (SaveState_FindLatestSlot(mode) == 0x10) {
                index = 0x10;
            }
        } else {
            index = Random16() % count;
            index = empty[index];
        }
    }
    return index;
}

s32 _call_via_r3(s32, s32, s32, s32);

u32 SaveState_WriteWorkspaceSlot(s32 sector)
{
    s32 *param = &Flash_Handler0;
    s32 result;
    struct SaveWorkspace *work;
    s32 value;

    work = gSaveWorkspace;
    value = (u16)sector;
    if ((_call_via_r3(value, (s32)&work->slot,
                       (s32)param, *param) << 0x10) != 0) {
        return 1U;
    }
    result = Flash_VerifySector(value, (s32)&work->slot);
    return (u32)((0 - result) | result) >> 0x1F;
}

#define Flash_Handler0 (*(s32 *)&Flash_Handler0)

s32 SaveState_ReadSlotAndCheckChecksum(s32 index)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    u32 checksum;

    work = gSaveWorkspace;
    ReadFlash(index, 0, work->slot.bytes, sizeof(work->slot));
    Dma_Set(&work->slot, &header, 0x84000004, REG_DMA3);
    WAIT_DMA();
    checksum = SaveState_ChecksumWorkspace();
    return (u16)checksum - header.checksum;
}

typedef u16 (*EraseSectorProc)(u16);

u16 SaveState_EraseSlotSector(u16 value)
{
    return ((EraseSectorProc)gEraseFlashSector)(value);
}

s32 SaveState_WriteRecord(s32 record_id, void *source)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    volatile u32 zero;
    u32 current;
    u32 slot;

    work = gSaveWorkspace;
    zero = 0;
    START_DMA(&zero, &work->slot, 0x85000400);
    WAIT_DMA();
    current = SaveState_FindLatestSlot(record_id);
    slot = SaveState_SelectWriteSlot(record_id);
    if (slot > 15)
        return 1;

    START_DMA(source, work->slot.record.payload, 0x840003fc);
    WAIT_DMA();
    START_DMA(Save_HeaderTemplate, &header, 0x84000002);
    WAIT_DMA();
    header.record_id = record_id;
    header.checksum = SaveState_ChecksumWorkspace();
    header.sequence = SaveState_GetLatestSequence(record_id) + 1;
    START_DMA(&header, &work->slot.record.header, 0x84000004);
    WAIT_DMA();

    if (SaveState_WriteWorkspaceSlot(slot) != 0)
        return 1;
    if (current <= 15 && SaveState_InvalidateSlot(current) != 0)
        return 1;

    if (header.sequence > 0xfde8) {
        header.sequence = 1;
        START_DMA(&header, &work->slot.record.header, 0x84000004);
        WAIT_DMA();
        if (SaveState_WriteWorkspaceSlot(current) != 0)
            return 1;
        if (SaveState_InvalidateSlot(slot) != 0)
            return 1;
        slot = current;
    }

    work->occupied[slot] = 1;
    work->record_id[slot] = record_id;
    work->sequence[slot] = header.sequence;
    return 0;
}

u32 SaveState_ReadRecordPayload(s32 record_id, void *destination)
{
    struct SaveWorkspace *work;
    u32 index;

    work = gSaveWorkspace;
    index = SaveState_FindLatestSlot(record_id);
    if (index > 15)
        return 1;
    SaveState_ReadSlotAndCheckChecksum(index);
    START_DMA(work->slot.record.payload, destination, 0x840003fc);
    WAIT_DMA();
    return 0;
}

u32 SaveState_DeleteRecord(s32 record_id)
{
    s32 index;
    s32 deletion_result;

    index = SaveState_FindLatestSlot(record_id);
    if (index > 0xFU) {
        return 1U;
    }
    deletion_result = SaveState_InvalidateSlot(index);
    return (u32)((0 - deletion_result) | deletion_result) >> 0x1F;
}

s32 SaveState_ChecksumWorkspace(void)
{
    struct SaveWorkspace *work;
    u32 limit;
    u32 offset;
    s32 sum;

    work = gSaveWorkspace;
    limit = 0xFE7;
    sum = 0;
    offset = 0;
    do {
        sum += work->slot.record.payload[offset + 0];
        sum += work->slot.record.payload[offset + 1];
        sum += work->slot.record.payload[offset + 2];
        sum += work->slot.record.payload[offset + 3];
        sum += work->slot.record.payload[offset + 4];
        sum += work->slot.record.payload[offset + 5];
        sum += work->slot.record.payload[offset + 6];
        sum += work->slot.record.payload[offset + 7];
        offset += 8;
    } while (offset <= limit);
    return sum;
}

u32 SaveState_FindLatestSlot(s32 record_id)
{
    struct SaveWorkspace *work;
    u32 latest_sequence;
    u32 latest_slot;
    u32 i;

    work = gSaveWorkspace;
    latest_slot = 0x10;
    latest_sequence = 0;
    for (i = 0; i <= 15; i++) {
        if (work->occupied[i] != 0 && record_id == work->record_id[i]) {
            if (latest_sequence < work->sequence[i]) {
                latest_sequence = work->sequence[i];
                latest_slot = i;
            }
        }
    }
    return latest_slot;
}

s32 SaveState_InvalidateSlot(s32 index)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    volatile u32 zero;

    work = gSaveWorkspace;
    zero = 0;
    START_DMA(&zero, &header, 0x85000004);
    WAIT_DMA();
    START_DMA(Save_HeaderTemplate, &header, 0x84000002);
    WAIT_DMA();
    header.record_id = 0x10;
    header.sequence = 0;
    START_DMA(&header, &work->slot.record.header, 0x84000004);
    WAIT_DMA();
    if (SaveState_WriteWorkspaceSlot(index) != 0)
        return 1;
    work->occupied[index] = 0;
    work->record_id[index] = 0x10;
    work->sequence[index] = 0;
    return 0;
}

s32 SaveState_CompareBytes(u8 *left, u8 *right, s32 count)
{
    s32 difference = 0;

    while (count != 0) {
        difference = *left - *right;
        if (difference != 0)
            break;
        count--;
        left++;
        right++;
    }
    return difference;
}

u32 SaveState_GetLatestSequence(s32 record_id)
{
    struct SaveWorkspace *work;
    u32 latest_sequence;
    u32 i;

    work = gSaveWorkspace;
    latest_sequence = 0;
    for (i = 0; i <= 15; i++) {
        if (work->occupied[i] != 0 && record_id == work->record_id[i]) {
            if (latest_sequence < work->sequence[i])
                latest_sequence = work->sequence[i];
        }
    }
    return latest_sequence;
}

s32 SaveState_LoadSummaryRecords(void)
{
    struct SaveWorkspace *work;
    struct SaveSummary *summary;
    volatile u32 zero;
    u32 group;
    s32 count;

    work = gSaveWorkspace;
    summary = work->summary;
    count = 0;
    group = 0;
    do {
        u32 index;

        zero = 0;
        START_DMA(&zero, summary, 0x85000010);
        index = SaveState_FindLatestSlot(group);
        if (index <= 15) {
            ReadFlash(index, 0, (u8 *)summary, sizeof(*summary));
            count++;
        }
        index = SaveState_FindLatestSlot(group + 3);
        if (index <= 15)
            ReadFlash(index, 0x110, summary->unknown_38, sizeof(summary->unknown_38));
        else
            *(u32 *)summary->unknown_38 = 0;
        group++;
        summary++;
    } while (group <= 2);
    return count;
}

u32 SaveState_ReleaseWorkspace(void)
{
    // 値を返さない int 関数 (pop {r1}; bx r1)
    Runtime_SetIrqHandler(5, 0, NULL);
    Runtime_ReleaseHeapBlock(51);
}
