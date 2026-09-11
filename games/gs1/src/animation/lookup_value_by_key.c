#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "metadata_lookup.h"

/* animation/lookup_value_by_key.c */
/* animation/lookup_value_by_key.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

struct LookupEntry {
    s32 key;
    s32 value;
};

s32 Animation_LookupValueByKey(s32 key)
{
    u32 no;
    struct LookupEntry *p;

    p = (struct LookupEntry *)(*(u32 *)ADDR_03001E68 + 0x1c);
    no = 0;
loop_1:
    if (p->key == key) {
        return p->value;
    }
    no++;
    p++;
    /* 表は8要素で終わる。 */
    if (no > 7) {
        return 0;
    }
    goto loop_1;
}

/* animation/initialize_objects.c */
struct AnimationObject {
    s16 id;
    u8 padding02[2];
    u8 draw_kind;
    u8 padding05[2];
    u8 frame_codec;
    s32 frames;
    s32 animation;
    u32 current;
    u8 state;
    u8 padding15;
    u8 marker;
};

struct AnimationMetadata {
    u8 width;
    u8 height;
    u16 scale;
    u8 draw_kind;
    u8 animation_count;
    s8 adjust_x;
    s8 adjust_y;
    u8 padding08[2];
    u8 frame_codec;
    u8 padding0b;
    s32 frames;
    s32 animation;
};

struct AnimationSetupState {
    u8 padding00[24];
    u32 scale;
    u8 padding1c[4];
    u8 width;
    u8 height;
    s8 adjust_x;
    s8 adjust_y;
    u8 padding24[3];
    u8 count;
    struct AnimationObject *objects[4];
};

s32 Animation_InitializeObjects(struct AnimationSetupState *state)
{
    s32 index;

    for (index = 0; index < state->count; index++) {
        struct AnimationObject *object = state->objects[index];
        struct AnimationMetadata *metadata = Sys_Run(object->id);
        s32 frames;
        s32 animation;

        if (metadata->width == 0)
            continue;

        if (index == 0) {
            state->width = metadata->width;
            state->height = metadata->height;
            state->scale = metadata->scale << 8;
            state->adjust_y = metadata->adjust_y;
            state->adjust_x = metadata->adjust_x;
        }

        frames = metadata->frames;
        if (frames == 0)
            frames = Sys_Check(object->id);

        object->draw_kind = metadata->draw_kind;
        animation = metadata->animation;
        object->frames = frames;
        object->animation = animation;
        object->frame_codec = metadata->frame_codec;
        object->marker = 0xff;
        object->current = 0;
        object->state = 0;
    }

    return 0;
}

/* animation/init_work_from_metadata.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Animation_InitWorkFromMetadata(void *work)
{
    s32 value;
    s32 z;
    void *info;

    if (work != NULL) {
        info = Sys_Run(FIELD_AT_OFFSET(work, s16, 0));
        if (FIELD_AT_OFFSET(info, u8, 0) != 0) {
            value = FIELD_AT_OFFSET(info, s32, 0x0c);
            if (value == 0) {
                value = Sys_Check(FIELD_AT_OFFSET(work, s16, 0));
            }
            FIELD_AT_OFFSET(work, u8, 4) = FIELD_AT_OFFSET(info, u8, 4);
            FIELD_AT_OFFSET(work, s32, 0x0c) = FIELD_AT_OFFSET(info, s32, 0x10);
            FIELD_AT_OFFSET(work, s32, 8) = value;
            FIELD_AT_OFFSET(work, u8, 7) = FIELD_AT_OFFSET(info, u8, 0x0a);
            z = 0;
            FIELD_AT_OFFSET(work, u8, 0x16) = 0xff;
            FIELD_AT_OFFSET(work, s32, 0x10) = z;
            FIELD_AT_OFFSET(work, u8, 0x14) = z;
        }
    }
}

/* resource/metadata/register.c */
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
    value = Sys_CheckRegister(id);
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

/* animation/set_work_entry.c */
/* animation/set_work_entry.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Animation_SetWorkEntry(void *work, s32 no)
{
    s32 hi;
    void *info;
    s32 value;

    hi = 0x80 & no;
    if (FIELD_AT_OFFSET(work, s32, 0x0c) != 0) {
        info = Sys_Run((s32)FIELD_AT_OFFSET(work, s16, 0));
        if (no < (s32)FIELD_AT_OFFSET(info, u8, 5)) {
            value = *(s32 *)((u8 *)FIELD_AT_OFFSET(work, s32, 0x0c) + (no * 4));
            FIELD_AT_OFFSET(work, u8, 4) = (u8)FIELD_AT_OFFSET(info, u8, 4);
            FIELD_AT_OFFSET(work, s32, 0x10) = value;
            FIELD_AT_OFFSET(work, s8, 0x15) = 0x10;
            if (hi == 0) {
                FIELD_AT_OFFSET(work, s8, 0x14) = hi;
                FIELD_AT_OFFSET(work, s16, 2) = (s16)hi;
            }
        }
    }
}

/* animation/select_object_animation.c */
struct AnimationMetadata {
    u8 width;
    u8 height;
    u16 scale;
    u8 draw_kind;
    u8 animation_count;
    s8 adjust_x;
    s8 adjust_y;
    u8 frame_codec;
    u8 padding0b;
    s32 frames;
    s32 animation;
};

struct AnimationObject {
    s16 id;
    s16 frame_index;
    u8 draw_kind;
    u8 padding05[3];
    s32 padding08;
    s32 animation_table;
    s32 current_animation;
    u8 playback_state;
    u8 reset_timer;
};

struct AnimationSetupState {
    u8 padding00[34];
    u8 adjust_x;
    u8 adjust_y;
    u8 selected_animation;
    u8 padding37[2];
    u8 count;
    struct AnimationObject *entries[4];
};

extern struct AnimationMetadata *Sys_Run(s32 id);

s32 AnimationObjects_SelectAnimation(struct AnimationSetupState *state, s32 flags)
{
    s32 high_bit;
    s32 index;

    high_bit = flags & 0x80;
    flags &= 0x7f;

    if (state->selected_animation != flags) {
        index = 0;
        goto test_entry;
entry_loop:
        {
            struct AnimationObject *entry = state->entries[index];
            struct AnimationMetadata *metadata;
            s32 selected_animation;

            if (entry == 0)
                goto next_entry;
            if (entry->animation_table == 0)
                goto next_entry;

            metadata = Sys_Run(entry->id);
            if (flags >= metadata->animation_count)
                goto next_entry;

            selected_animation = ((s32 *)entry->animation_table)[flags];
            entry->draw_kind = metadata->draw_kind;
            entry->current_animation = selected_animation;
            entry->reset_timer = 0x10;
            if (high_bit == 0) {
                entry->playback_state = 0;
                entry->frame_index = 0;
            }
            if (index == 0) {
                state->adjust_y = metadata->adjust_y;
                state->adjust_x = metadata->adjust_x;
            }
            goto next_entry;
        }
next_entry:
        index++;
test_entry:
        if (index < state->count)
            goto entry_loop;
        state->selected_animation = (u8)flags;
    }
    return 0;
}

/* animation/set_halfword02_on_active_objects.c */
s32 AnimationObjects_SetHalfword02OnActive(u8 *grp, s32 val)
{
    u8 n = grp[0x27];
    void **p;
    s32 cnt;

    if (n != 0) {
        /* 有効な登録項目だけへ下位16bitを4bit左へずらして設定する。 */
        val = (s32)((u32)val << 4);
        p = (void **)(grp + 0x28);
        cnt = n;
        do {
            u8 *obj = *p++;
            if (obj != 0 && *(s32 *)(obj + 0xc) != 0)
                *(s16 *)(obj + 2) = val;
            cnt--;
        } while (cnt != 0);
    }
    return 0;
}

/* animation/set_field15_on_active_objects.c */
void AnimationObjects_SetField15OnActive(u8 *grp, s32 val)
{
    u8 n = grp[0x27];
    if (n != 0) {
        /* 登録順を保ったまま有効な項目へ値を配る。 */
        void **p = (void **)(grp + 0x28);
        s32 cnt = n;
        do {
            u8 *obj = *p++;
            if (obj != 0 && *(s32 *)(obj + 0xc) != 0)
                obj[0x15] = val;
            cnt--;
        } while (cnt != 0);
    }
}
