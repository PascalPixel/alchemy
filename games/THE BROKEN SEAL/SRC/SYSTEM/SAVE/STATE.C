#include "RUNTIME_MEM.H"
#include "SAVE_STATE.H"
#include "TYPES.H"
#include "SCENE.H"
#include "FIXED_MATH.H"
#include "GLOBAL_CELLS.H"
#include "RUNTIME_INTERFACES.H"
extern s32 Flash_Handler0;

/* save/state/select_write_slot.c */
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

/* save/state/write_workspace_slot.c */
s32 _call_via_r3(s32, s32, s32, s32);
s32 Flash_VerifySector(u16, s32);

/* The old-style u16 formal receives an int-promoted incoming word. */
u32 SaveState_WriteWorkspaceSlot(code)
u16 code;
{
    s32 *param = &Flash_Handler0;
    s32 result;
    struct SaveWorkspace *work;
    s32 value;

    work = gSaveWorkspace;
    value = code & 0xFFFF;
    if ((_call_via_r3(value, (s32)&work->slot,
                       (s32)param, *param) << 0x10) != 0) {
        return 1U;
    }
    result = Flash_VerifySector(value, (s32)&work->slot);
    return (u32)((0 - result) | result) >> 0x1F;
}

#include "DMA.H"
#include "FLASH.H"

s32 SaveState_ReadSlotAndCheckChecksum(s32 index)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    u32 checksum;

    work = gSaveWorkspace;
    ReadFlash((u16)index, 0, work->slot.bytes, sizeof(work->slot));
    Dma_Set(&work->slot, &header, 0x84000004, (volatile u32 *)0x040000d4);
    WAIT_DMA();
    checksum = SaveState_ChecksumWorkspace();
    return (u16)checksum - header.checksum;
}

typedef u16 (*EraseSectorProc)(u16);
extern EraseSectorProc gEraseFlashSector;

u16 SaveState_EraseSlotSector(u16 value)
{
    return gEraseFlashSector(value);
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
    /* FAKEMATCH: retain the existing byte and halfword cursors over the real
       workspace arrays. Direct indexed fields change saved registers and
       instruction order at the same complete 64/60-byte extents. */
    u16 *sequence_cursor;
    u16 sequence;
    u32 latest_sequence;
    u32 slot_index;
    u32 latest_slot;
    struct SaveWorkspace *save_state;
    u8 *slot_cursor;

    save_state = gSaveWorkspace;
    latest_slot = 0x10;
    latest_sequence = 0;
    slot_index = 0;
    sequence_cursor = save_state->sequence;
    slot_cursor = (u8 *)save_state;
    do {
        if ((slot_cursor[0] != 0) && (record_id == slot_cursor[sizeof(save_state->occupied)])) {
            sequence = *sequence_cursor;
            if (latest_sequence < (u32)sequence) {
                latest_sequence = (u32)sequence;
                latest_slot = slot_index;
            }
        }
        slot_index += 1;
        sequence_cursor += 1;
        slot_cursor += 1;
    } while (slot_index <= 0xFU);
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
    /* FAKEMATCH: retain the existing byte and halfword cursors over the real
       workspace arrays. Direct indexed fields change saved registers and
       instruction order at the same complete 64/60-byte extents. */
    u16 *sequence_cursor;
    u16 sequence;
    u32 latest_sequence;
    u32 slot_index;
    struct SaveWorkspace *save_state;
    u8 *slot_cursor;

    save_state = gSaveWorkspace;
    slot_index = 0;
    latest_sequence = 0;
    sequence_cursor = save_state->sequence;
    slot_cursor = (u8 *)save_state;
    do {
        if ((slot_cursor[0] != 0) && (record_id == slot_cursor[sizeof(save_state->occupied)])) {
            sequence = *sequence_cursor;
            if (latest_sequence < (u32)sequence) {
                latest_sequence = (u32)sequence;
            }
        }
        slot_index += 1;
        sequence_cursor += 1;
        slot_cursor += 1;
    } while (slot_index <= 0xFU);
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
            ReadFlash((u16)index, 0, (u8 *)summary, sizeof(*summary));
            count++;
        }
        index = SaveState_FindLatestSlot(group + 3);
        if (index <= 15)
            ReadFlash((u16)index, 0x110, summary->unknown_38, sizeof(summary->unknown_38));
        else
            *(u32 *)summary->unknown_38 = 0;
        group++;
        summary++;
    } while (group <= 2);
    return count;
}

typedef void (*InterruptHandler)(void);

void Runtime_SetIrqHandler(u32 irq, s32 vcount, InterruptHandler handler);

u32 SaveState_ReleaseWorkspace(void)
{
    /* FAKEMATCH: retain only the existing ignored-result definition. No
       caller consumes a result; true void changes the return-address pop
       from r1 to r0. The actual heap helper is void; no word is returned. */
    Runtime_SetIrqHandler(5, 0, NULL);
    Runtime_ReleaseHeapBlock(51);
}
