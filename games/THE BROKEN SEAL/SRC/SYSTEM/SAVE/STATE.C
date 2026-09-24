#include "TYPES.H"
#include "SCENE.H"
#include "FIXED_MATH.H"
#include "GLOBAL_CELLS.H"
#include "RUNTIME_INTERFACES.H"

/* save/state/select_write_slot.c */
u32 Random16(void);

u32 SaveState_SelectWriteSlot(s32 mode)
{
    u32 empty[16];
    s32 count;
    u32 index;
    u8 *slot;
    u8 value;

    slot = *(u8 **)ADDR_03001F1C;
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
            if (State_Do(mode) == 0x10) {
                index = 0x10;
            }
        } else {
            index = Math_ModU(Random16(), count);
            index = empty[index];
        }
    }
    return index;
}

/* save/state/write_workspace_slot.c */
s32 State_SetMode(s32, s32, s32, s32);
s32 State_Apply(u16, s32);

struct Work_08005868 {
    u8 unknown_00[64];
    s32 data;
};

u32 SaveState_WriteWorkspaceSlot(code)
u16 code;
{
    s32 *param = (s32 *)0x02004C04;
    s32 result;
    struct Work_08005868 *work;
    s32 value;

    work = *(struct Work_08005868 **)ADDR_03001F1C;
    value = code & 0xFFFF;
    if ((State_SetMode(value, (s32)&work->data,
                       (s32)param, *param) << 0x10) != 0) {
        return 1U;
    }
    result = State_Apply(value, (s32)&work->data);
    return (u32)((0 - result) | result) >> 0x1F;
}

#include "TYPES.H"
#include "DMA.H"
#include "FLASH.H"
#include "GLOBAL_CELLS.H"

#include "SAVE_STATE.H"

struct DmaChannel {
    u32 source;
    u32 destination;
    u32 control;
};

static __inline__ void Dma_WaitForCompletion(volatile struct DmaChannel *channel)
{
    while ((channel->control & 0x80000000) != 0) {}
}

s32 SaveState_ReadSlotAndCheckChecksum(s32 index)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    u32 checksum;

    work = *(struct SaveWorkspace **)ADDR_03001F1C;
    ReadFlash((u16)index, 0, work->slot.bytes, sizeof(work->slot));
    Dma_Set(&work->slot, &header, 0x84000004, (volatile u32 *)0x040000d4);
    Dma_WaitForCompletion((volatile struct DmaChannel *)0x040000d4);
    checksum = SaveState_ChecksumWorkspace();
    return (u16)checksum - header.checksum;
}

#include "TYPES.H"

typedef u16 (*Callback_08005904)(u16);
extern Callback_08005904 gEraseFlashSector;

u16 SaveState_EraseSlotSector(u16 value)
{
    return gEraseFlashSector(value);
}

#include "SAVE_STATE.H"

s32 SaveState_WriteRecord(s32 record_id, void *source)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    volatile u32 zero;
    u32 current;
    u32 slot;

    work = SAVE_WORKSPACE;
    zero = 0;
    START_DMA(&zero, &work->slot, 0x85000400);
    WAIT_DMA();
    current = SaveState_FindLatestSlot(record_id);
    slot = SaveState_SelectWriteSlot(record_id);
    if (slot > 15)
        return 1;

    START_DMA(source, work->slot.record.payload, 0x840003fc);
    WAIT_DMA();
    START_DMA(SAVE_HEADER_TEMPLATE, &header, 0x84000002);
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

#include "SAVE_STATE.H"

u32 SaveState_ReadRecordPayload(s32 record_id, void *destination)
{
    struct SaveWorkspace *work;
    u32 index;

    work = SAVE_WORKSPACE;
    index = SaveState_FindLatestSlot(record_id);
    if (index > 15)
        return 1;
    SaveState_ReadSlotAndCheckChecksum(index);
    START_DMA(work->slot.record.payload, destination, 0x840003fc);
    WAIT_DMA();
    return 0;
}

#include "TYPES.H"

u32 SaveState_FindLatestSlot(s32);
s32 SaveState_InvalidateSlot(s32);

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

#include "TYPES.H"
#include "GLOBAL_CELLS.H"

struct Runtime08005ae0 {
    u8 bytes[0x103F];
};

s32 SaveState_ChecksumWorkspace(void)
{
    struct Runtime08005ae0 *runtime;
    u32 limit;
    u32 offset;
    s32 sum;

    runtime = *(struct Runtime08005ae0 **)ADDR_03001F1C;
    limit = 0xFE7;
    sum = 0;
    offset = 0;
    do {
        sum += runtime->bytes[offset + 0x50];
        sum += runtime->bytes[offset + 0x51];
        sum += runtime->bytes[offset + 0x52];
        sum += runtime->bytes[offset + 0x53];
        sum += runtime->bytes[offset + 0x54];
        sum += runtime->bytes[offset + 0x55];
        sum += runtime->bytes[offset + 0x56];
        sum += runtime->bytes[offset + 0x57];
        offset += 8;
    } while (offset <= limit);
    return sum;
}

#include "TYPES.H"
#include "GLOBAL_CELLS.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u32 SaveState_FindLatestSlot(s32 record_id)
{
    u16 *sequence_cursor;
    u16 sequence;
    u32 latest_sequence;
    u32 slot_index;
    u32 latest_slot;
    void *save_state;
    void *slot_cursor;

    save_state = *(void **)ADDR_03001F1C;
    latest_slot = 0x10;
    latest_sequence = 0;
    slot_index = 0;
    sequence_cursor = save_state + 0x20;
    slot_cursor = save_state;
    do {
        if ((FIELD_AT_OFFSET(slot_cursor, u8 *, 0) != 0) && (record_id == FIELD_AT_OFFSET(slot_cursor, u8 *, 0x10))) {
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

#include "SAVE_STATE.H"

s32 SaveState_InvalidateSlot(s32 index)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    volatile u32 zero;

    work = SAVE_WORKSPACE;
    zero = 0;
    START_DMA(&zero, &header, 0x85000004);
    WAIT_DMA();
    START_DMA(SAVE_HEADER_TEMPLATE, &header, 0x84000002);
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

#include "TYPES.H"

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

#include "TYPES.H"
#include "GLOBAL_CELLS.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u32 SaveState_GetLatestSequence(s32 record_id)
{
    u16 *sequence_cursor;
    u16 sequence;
    u32 latest_sequence;
    u32 slot_index;
    void *save_state;
    void *slot_cursor;

    save_state = *(void **)ADDR_03001F1C;
    slot_index = 0;
    latest_sequence = 0;
    sequence_cursor = save_state + 0x20;
    slot_cursor = save_state;
    do {
        if ((FIELD_AT_OFFSET(slot_cursor, u8 *, 0) != 0) && (record_id == FIELD_AT_OFFSET(slot_cursor, u8 *, 0x10))) {
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

#include "SAVE_STATE.H"

s32 SaveState_LoadSummaryRecords(void)
{
    struct SaveWorkspace *work;
    u8 *summary;
    volatile u32 zero;
    u32 group;
    s32 count;

    work = SAVE_WORKSPACE;
    summary = work->summary[0];
    count = 0;
    group = 0;
    do {
        u32 index;

        zero = 0;
        START_DMA(&zero, summary, 0x85000010);
        index = SaveState_FindLatestSlot(group);
        if (index <= 15) {
            ReadFlash((u16)index, 0, summary, 64);
            count++;
        }
        index = SaveState_FindLatestSlot(group + 3);
        if (index <= 15)
            ReadFlash((u16)index, 0x110, summary + 56, 4);
        else
            *(u32 *)(summary + 56) = 0;
        group++;
        summary += 64;
    } while (group <= 2);
    return count;
}

#include "TYPES.H"
#include "RUNTIME_INTERFACES.H"

typedef void (*InterruptHandler)(void);

u32 Runtime_ReleaseHeapBlock(s32);
void Runtime_SetIrqHandler(s32, s32, InterruptHandler);
u32 SaveState_ReleaseWorkspace(void)
{
  int fn;
  long long id;
  long long tmp;
  int arg;
  unsigned int no;
  fn = 0;
  id = (tmp = (no = 0x33));
  arg = 0;
  Runtime_SetIrqHandler(5, arg, (InterruptHandler)fn);
 return Runtime_ReleaseHeapBlock(id);
}
