#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "animspr.h"
#include "metadata_lookup.h"


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
            work->frames = (void **)value;
            work->scripts = (u8 **)animation;
            work->pos = z;
            /* FAKEMATCH: then the codec and the frame. */
            asm("");
            work->mode = info->frame_codec;
            work->frame = 0xff;
        }
    }
}

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
    s32 count;
    u32 slot;
    u32 i;

    if (state != NULL && handle != 0) {
        ResourceMetadata_ClearRecord((void *)handle);
        for (slot = 0; slot <= 3; slot++) {
            if (handle == state->slots[slot])
                break;
        }
        if (slot != 4) {
            state->slots[slot] = 0;
            count = 0;
            for (i = slot + 1; i <= 3; i++) {
                if (state->slots[i] != 0)
                    count++;
            }
            if (count == 0)
                state->count = slot;
        }
    }
}

void ResourceMetadata_ReleaseSlot(struct MetadataSlotState *state, u32 no)
{
    s32 slot;
    s32 count;
    u32 i;

    if (state != NULL && no <= 3) {
        slot = state->slots[no];
        if (slot != 0) {
            ResourceMetadata_ClearRecord((void *)slot);
            state->slots[no] = 0;
            count = 0;
            for (i = no + 1; i <= 3; i++) {
                if (state->slots[i] != 0)
                    count++;
            }
            if (count == 0)
                state->count = no;
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
        if (no < info->animation_count) {
            value = (s32)work->scripts[no];
            work->kind = info->draw_kind;
            work->script = (u8 *)value;
            work->step = 0x10;
            if (hi == 0) {
                work->pos = hi;
                work->timer = hi;
            }
        }
    }
}

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
        state->last_no = flags;
    }
    return 0;
}
