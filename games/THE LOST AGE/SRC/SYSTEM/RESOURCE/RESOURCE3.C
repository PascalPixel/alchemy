#include "TYPES.H"
#include "ANIMSPR.H"
#include "METADATA_LOOKUP.H"

void ResourceMetadata_ClearRecord(void *record);
s32 AnimationObject_Allocate(s32 id);
s32 Animation_LookupValueByKey(s32 key);

/* ☀️'s: set a layer up from its metadata, looking its frames up among the
   loaded slots when the metadata has none. */
void Animation_InitWorkFromMetadata(struct AnimationEntry *work)
{
    s32 value;
    s32 z;
    s32 animation;
    struct AnimationMetadata *info;

    if (work != NULL) {
        info = Resource_GetMetadataRecordFar(work->anim_id);
        if (info->width != 0) {
            value = info->frames;
            if (value == 0)
                value = Animation_LookupValueByKey(work->anim_id);
            z = 0;
            work->kind = info->draw_kind;
            work->script = (u8 *)z;
            /* FAKEMATCH: ⚓️ clears the script before storing the lists. */
            asm("");
            animation = info->animation;
            *(s32 *)&work->frames = value;
            *(s32 *)&work->scripts = animation;
            /* FAKEMATCH: and the cursor straight after them. */
            asm("");
            work->pos = z;
            /* FAKEMATCH: then the codec and the frame. */
            asm("");
            work->mode = info->frame_codec;
            work->frame = 0xff;
        }
    }
}

/* ⚓️ keeps the slot count at 0x1b, where ☀️ keeps it at 0x27, and the
   size and offsets from 0x0c. */
struct MetadataSlotState {
    u8 unknown_00[0x0c];
    s32 scale;
    u8 unknown_10[4];
    u8 width;
    u8 height;
    s8 offset_x;
    s8 offset_y;
    u8 unknown_18[3];
    u8 count;
    u8 unknown_1c[0x0c];
    s32 slots[4];
};

/* ☀️'s: give the object its next free animation slot, taking the first
   one's size and offsets from its metadata. */
s32 ResourceMetadata_Register(struct MetadataSlotState *state, s32 id)
{
    s32 value = state->slots[0];
    s32 index = 0;
    s32 *slot;
    struct AnimationMetadata *metadata;

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
    value = AnimationObject_Allocate(id);
    if (value == 0)
        return 0;
    state->slots[index] = value;
    metadata = Resource_GetMetadataRecordFar(id);
    if (state->count == 0) {
        state->width = metadata->width;
        state->height = metadata->height;
        state->scale = metadata->scale << 8;
        state->offset_y = metadata->adjust_y;
        state->offset_x = metadata->adjust_x;
    }
    if (index == state->count)
        state->count = index + 1;
    return value;
}

void ResourceMetadata_Unregister(struct MetadataSlotState *state, s32 handle)
{
    s32 *remaining_slot;
    s32 *slot_cursor;
    s32 slot_value;
    s32 later_slot_count;
    u32 slot_index;
    u32 later_index;
    u32 slot_offset;

    if (state != NULL && handle != 0) {
        ResourceMetadata_ClearRecord((void *)handle);
        slot_index = 0;
        if (handle != state->slots[0]) {
            slot_cursor = state->slots;
        next_slot:
            slot_index++;
            if (slot_index <= 3U) {
                slot_cursor++;
                if (handle != *slot_cursor)
                    goto next_slot;
            }
        }
        if (slot_index != 4) {
            slot_offset = slot_index * 4 + 0x28;
            *(s32 *)((u8 *)state + slot_offset) = 0;
            later_index = slot_index + 1;
            later_slot_count = 0;
            if (later_index <= 3U) {
                remaining_slot = (s32 *)(later_index * 4 + (u32)state + 0x28);
                do {
                    slot_value = *remaining_slot++;
                    if (slot_value != 0)
                        later_slot_count++;
                    later_index++;
                } while (later_index <= 3U);
            }
            if (later_slot_count == 0)
                state->count = (s8)slot_index;
        }
    }
}

void ResourceMetadata_ReleaseSlot(u8 *rec, u32 no)
{
    void **p;
    void *t;
    s32 off;
    void *v;
    s32 cnt;
    u32 i;

    if (rec != NULL && no <= 3) {
        off = no * 4 + 0x28;
        v = *(void **)(rec + off);
        if (v != NULL) {
            ResourceMetadata_ClearRecord(v);
            *(void **)(rec + off) = NULL;
            i = no + 1;
            cnt = 0;
            if (i <= 3) {
                p = (void **)(i * 4 + (u32)rec + 0x28);
                do {
                    t = *p++;
                    if (t != NULL)
                        cnt++;
                    i++;
                } while (i <= 3);
            }
            if (cnt == 0)
                *(s8 *)(rec + 0x1b) = (s8)no;
        }
    }
}

void Animation_SetWorkEntry(struct AnimationEntry *work, s32 no)
{
    s32 hi;
    struct AnimationMetadata *info;
    s32 value;

    hi = 0x80 & no;
    if (work->scripts != 0) {
        info = Resource_GetMetadataRecordFar(work->anim_id);
        if (no < (s32)info->animation_count) {
            value = (s32)work->scripts[no];
            work->kind = info->draw_kind;
            work->script = (u8 *)value;
            work->step = 0x10;
            if (hi == 0) {
                work->pos = hi;
                work->timer = (s16)hi;
            }
        }
    }
}

/* ⚓️'s animation object keeps its offsets, last animation and entry count
   0x0c earlier than ☀️'s, and its entries at the same 0x28. */
struct AnimationGroup {
    u8 unknown_00[0x16];
    s8 offset_x;
    s8 offset_y;
    u8 last_no;
    u8 unknown_19[2];
    u8 count;
    u8 unknown_1c[0x0c];
    struct AnimationEntry *entries[4];
};

s32 Animation_ApplyChildArgument(struct AnimationGroup *state, s32 flags)
{
    s32 high_bit;
    s32 index;

    high_bit = flags & 0x80;
    flags &= 0x7f;

    if (state->last_no != flags) {
        index = 0;
        goto test_entry;
entry_loop:
        {
            struct AnimationEntry *entry = state->entries[index];
            struct AnimationMetadata *metadata;
            s32 selected_animation;

            if (entry == 0)
                goto next_entry;
            if (entry->scripts == 0)
                goto next_entry;

            metadata = Resource_GetMetadataRecordFar(entry->anim_id);
            if (flags >= metadata->animation_count)
                goto next_entry;

            selected_animation = (s32)entry->scripts[flags];
            entry->kind = metadata->draw_kind;
            entry->script = (u8 *)selected_animation;
            entry->step = 0x10;
            if (high_bit == 0) {
                entry->pos = 0;
                entry->timer = 0;
            }
            if (index == 0) {
                state->offset_y = metadata->adjust_y;
                state->offset_x = metadata->adjust_x;
            }
            goto next_entry;
        }
next_entry:
        index++;
test_entry:
        if (index < state->count)
            goto entry_loop;
        state->last_no = (u8)flags;
    }
    return 0;
}
