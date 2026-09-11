#include "metadata_lookup.h"
#include "types.h"
#include "scene.h"

/* resource/metadata/register.c */
struct MetadataSlotState {
    u8 padding0[24];
    s32 shifted;
    u8 padding1[4];
    u8 first;
    u8 second;
    u8 third;
    u8 fourth;
    u8 padding2[3];
    u8 count;
    s32 slots[4];
};

struct MetadataRecord {
    u8 first;
    u8 second;
    u16 value;
    u8 padding[2];
    u8 third;
    u8 fourth;
};

s32 ResourceMetadata_Register(struct MetadataSlotState *state, s32 id)
{
    s32 value = state->slots[0];
    s32 index = 0;
    s32 *slot;
    struct MetadataRecord *metadata;

    if (value != 0) {
        slot = &state->slots[0];
        do {
            index++;
            if (index > 3)
                break;
            slot++;
            value = *slot;
        } while (value != 0);
    }
    if (index == 4)
        return -1;
    value = Sys_Check(id);
    if (value == 0)
        return 0;
    state->slots[index] = value;
    metadata = Sys_Run(id);
    if (state->count == 0) {
        state->first = metadata->first;
        state->second = metadata->second;
        state->shifted = metadata->value << 8;
        state->fourth = metadata->fourth;
        state->third = metadata->third;
    }
    if (index == state->count)
        state->count = index + 1;
    return value;
}

/* resource/metadata/unregister.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void ResourceMetadata_Unregister(void *state, s32 handle)
{
    s32 *remaining_slot;
    s32 *slot_cursor;
    s32 slot_value;
    s32 later_slot_count;
    u32 slot_index;
    u32 later_index;
    u32 slot_offset;

    if ((state != 0) && (handle != 0)) {
        Sys_Do(handle);
        slot_index = 0;
        if (handle != FIELD_AT_OFFSET(state, s32, 0x28)) {
            slot_cursor = (s32 *)((u8 *)state + 0x28);
loop_4:
            slot_index += 1;
            if (slot_index <= 3U) {
                slot_cursor += 1;
                if (handle != *slot_cursor) {
                    goto loop_4;
                }
            }
        }
        if (slot_index != 4) {
            slot_offset = (slot_index * 4) + 0x28;
            FIELD_AT_OFFSET(state, s32, slot_offset) = 0;
            later_index = slot_index + 1;
            later_slot_count = 0;
            if (later_index <= 3U) {
                remaining_slot = (s32 *)((later_index * 4) + (u32)state + 0x28);
                do {
                    slot_value = *remaining_slot++;
                    if (slot_value != 0) {
                        later_slot_count += 1;
                    }
                    later_index += 1;
                } while (later_index <= 3U);
            }
            if (later_slot_count == 0) {
                FIELD_AT_OFFSET(state, s8, 0x27) = (s8)slot_index;
            }
        }
    }
}

/* resource/metadata/release_slot.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void ResourceMetadata_ReleaseSlot(u8 *rec, u32 no)
{
    void **p;
    void *t;
    s32 off;
    void *v;
    s32 cnt;
    u32 i;

    if (rec != 0 && no <= 3) {
        off = (no * 4) + 0x28;
        v = FIELD_AT_OFFSET(rec, void *, off);
        if (v != 0) {
            Sys_Do(v);
            FIELD_AT_OFFSET(rec, void *, off) = NULL;
            i = no + 1;
            cnt = 0;
            if (i <= 3) {
                p = (void **)((i * 4) + (u32)rec + 0x28);
                do {
                    t = *p++;
                    if (t != 0) {
                        cnt++;
                    }
                    i++;
                } while (i <= 3);
            }
            if (cnt == 0) {
                FIELD_AT_OFFSET(rec, s8, 0x27) = (s8)no;
            }
        }
    }
}
