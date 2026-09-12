#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "global_cells.h"
#include "runtime_interfaces.h"

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
            index = UnsignedModulo(Random16(), count);
            index = empty[index];
        }
    }
    return index;
}

/* save/state/write_workspace_slot.c */
/*
 * Write one workspace slot of the save state, returning nonzero when
 * either step reports a nonzero result.
 */
#include "types.h"
#include "scene.h"
#include "global_cells.h"

/*
 * State_SetMode names a `bx rN` slot: the call is indirect through the
 * register that slot selects, and the trailing argument is the callee
 * address rather than a parameter of the callee.
 */

struct Work_08005868 {
    u8 unknown_00[64];
    s32 data;
};

/*
 * The old-style definition is deliberate: with a prototype the u16
 * parameter would arrive unpromoted and the mask below would change. The
 * word at 0x02004C04 is the address called while its pointer is passed as
 * an argument; the shift by 16 tests the low halfword of the result.
 */
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
    result = State_ApplyWriteWorkspaceSlot(value, (s32)&work->data);
    return (u32)((0 - result) | result) >> 0x1F;
}

/* save/state/erase_slot_sector.c */
typedef u16 (*Callback_08005904)(u16);
extern Callback_08005904 gOv;

u16 SaveState_EraseSlotSector(u16 value)
{
    return gOv(value);
}

/* save/state/delete_record.c */
/* save/state/delete_record.c */
u32 SaveState_DeleteRecord(s32 record_id)
{
    s32 index;
    s32 deletion_result;

    index = State_Do(record_id);
    if (index > 0xFU) {
        return 1U;
    }
    deletion_result = State_Check(index);
    return (u32)((0 - deletion_result) | deletion_result) >> 0x1F;
}

/* save/state/checksum_workspace.c */
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

/* save/state/find_latest_slot.c */
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

/* save/state/compare_bytes.c */
/* save/state/compare_bytes.c */
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

/* save/state/get_latest_sequence.c */
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

/* save/state/release_workspace.c */
typedef void (*InterruptHandler)(void);

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
  State_Place(5, arg, (InterruptHandler)fn);
 return State_DoReleaseWorkspace(id);
}
