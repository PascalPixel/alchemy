#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "resource.h"
#include "types.h"
#include "animspr.h"
#include "global_cells.h"
#include "metadata_lookup.h"
#include "dma.h"
#include "io_reg.h"

struct ResourceSlotNumber {
    u16 resource;
    s16 number;
};

enum { RESOURCE_SLOT_COUNT = 8, ANIMATION_OBJECT_COUNT = 64 };

extern struct ResourceSlotNumber ResourceSlot_NumberTable[];

s32 ResourceSlot_Load(u32 slot, u32 *buffer, s32 number, u32 variant)
{
    struct AnimationMetadata *metadata;
    struct ObjectSystemWork *work;
    struct ResourceSlot *record;
    struct ResourceSlotNumber *entry;
    u32 *offset;
    u8 *text;
    u8 *end;
    u8 *table;
    u32 size;
    u32 count;
    u32 index;
    u16 entry_number;
    s32 resource;

    if (slot >= RESOURCE_SLOT_COUNT)
        return 0;
    work = gMenuCtrlWork;
    record = &work->slots[slot];
    metadata = Resource_GetMetadataRecordFar(number);
    record->tag = (slot << 12) | number;
    record->buffer = buffer;

    entry = ResourceSlot_NumberTable;
    for (count = 0; count < 256; count++) {
        s16 *numbers = &entry->number;

        resource = entry->resource;
        entry_number = *numbers;
        entry++;
        if (entry_number == 0)
            return 0;
        if (entry_number == number)
            break;
    }
    size = Resource_DecodeType01(Resource_GetTableEntry(resource), buffer);

    offset = buffer;
    for (count = 0; count < 256; count++) {
        if (*offset == 0)
            break;
        *offset += (u32)buffer;
        offset++;
    }
    if (variant != 0) {
        index = variant - 1;
        text = (u8 *)(offset + 1);
        end = (u8 *)buffer + size;
        if (index > 4)
            index = 0;
        table = ResourceSlot_ConversionTables[index];
        for (; text < end; text++) {
            u32 c = *text;

            if (c <= 0xdf) {
                c = table[c];
                *text = c;
            }
        }
    }
    return metadata->width * metadata->height;
}

s32 Animation_LookupValueByKey(s32 key)
{
    u32 no;
    struct ResourceSlot *p = gMenuCtrlWork->slots;

    for (no = 0; no < RESOURCE_SLOT_COUNT; no++, p++) {
        if (p->tag == key)
            return (s32)p->buffer;
    }
    return 0;
}

s32 Animation_InitializeObjects(struct AnimationObject *state)
{
    s32 index;

    for (index = 0; index < state->count; index++) {
        struct AnimationEntry *object = state->entries[index];
        struct AnimationMetadata *metadata = Resource_GetMetadataRecordFar(object->anim_id);
        s32 frames;
        s32 animation;

        if (metadata->width == 0)
            continue;

        if (index == 0) {
            state->width = metadata->width;
            state->height = metadata->height;
            state->scale = metadata->scale << 8;
            state->offset_y = metadata->adjust_y;
            state->offset_x = metadata->adjust_x;
        }

        frames = metadata->frames;
        if (frames == 0)
            frames = Animation_LookupValueByKey(object->anim_id);

        /* FAKEMATCH: metadata address-word stores keep frames before script/codec stores; typed pointers move frames later. */
        object->kind = metadata->draw_kind;
        animation = metadata->animation;
        *(s32 *)&object->frames = frames;
        *(s32 *)&object->scripts = animation;
        object->mode = metadata->frame_codec;
        object->frame = 0xff;
        object->script = 0;
        object->pos = 0;
    }

    return 0;
}

void Animation_InitWorkFromMetadata(struct AnimationEntry *work)
{
    s32 value;
    s32 z;
    struct AnimationMetadata *info;

    if (work != NULL) {
        info = Resource_GetMetadataRecordFar(work->anim_id);
        if (info->width != 0) {
            value = info->frames;
            if (value == 0)
                value = Animation_LookupValueByKey(work->anim_id);
            /* FAKEMATCH: address-word assignments keep the frames store before codec/zero setup; typed pointers move it after. */
            work->kind = info->draw_kind;
            *(s32 *)&work->scripts = info->animation;
            *(s32 *)&work->frames = value;
            work->mode = info->frame_codec;
            z = 0;
            work->frame = 0xff;
            work->script = (u8 *)z;
            work->pos = z;
        }
    }
}

s32 ResourceMetadata_Register(struct AnimationObject *state, s32 id)
{
    s32 value = (s32)state->entries[0];
    s32 index = 0;
    struct AnimationEntry **slot;
    struct AnimationMetadata *metadata;

    if (value != 0) {
        slot = &state->entries[0];
        do {
            index++;
            if (index > 3)
                break;
            slot++;
            value = (s32)*slot;
        } while (value != 0);
    }
    if (index == 4)
        return -1;
    value = (s32)AnimationObject_Allocate(id);
    if (value == 0)
        return 0;
    state->entries[index] = (struct AnimationEntry *)value;
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

void ResourceMetadata_Unregister(struct AnimationObject *state, s32 handle)
{
    s32 count;
    u32 slot;
    u32 i;

    if (state != NULL && handle != 0) {
        ResourceMetadata_ClearRecord((void *)handle);
        for (slot = 0; slot <= 3; slot++) {
            if (handle == (s32)state->entries[slot])
                break;
        }
        if (slot != 4) {
            state->entries[slot] = 0;
            count = 0;
            for (i = slot + 1; i <= 3; i++) {
                if (state->entries[i] != 0)
                    count++;
            }
            if (count == 0)
                state->count = slot;
        }
    }
}

void ResourceMetadata_ReleaseSlot(struct AnimationObject *group, u32 no)
{
    struct AnimationEntry *v;
    s32 cnt;
    u32 i;

    if (group != NULL && no <= 3) {
        v = group->entries[no];
        if (v != NULL) {
            ResourceMetadata_ClearRecord(v);
            group->entries[no] = NULL;
            cnt = 0;
            for (i = no + 1; i <= 3; i++) {
                if (group->entries[i] != NULL)
                    cnt++;
            }
            if (cnt == 0)
                group->count = no;
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

s32 AnimationObjects_SelectAnimation(struct AnimationObject *state, s32 flags)
{
    s32 high_bit;
    s32 index;

    high_bit = flags & 0x80;
    flags &= 0x7f;

    /* FAKEMATCH: the index/test loop keeps an 8-byte frame; a for loop spills an entry cursor and uses 12 bytes. */
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

s32 AnimationObjects_SetActiveFrameTimers(struct AnimationObject *group, s32 val)
{
    u8 n = group->count;
    struct AnimationEntry **p;
    s32 cnt;

    if (n != 0) {
        val = val << 4;
        p = group->entries;
        cnt = n;
        do {
            struct AnimationEntry *entry = *p++;
            if (entry != 0 && entry->scripts != 0)
                entry->timer = val;
            cnt--;
        } while (cnt != 0);
    }
    return 0;
}

void AnimationObjects_SetActiveTimerSteps(struct AnimationObject *group, s32 val)
{
    u8 n = group->count;
    if (n != 0) {
        struct AnimationEntry **p = group->entries;
        s32 cnt = n;
        do {
            struct AnimationEntry *entry = *p++;
            if (entry != 0 && entry->scripts != 0)
                entry->step = val;
            cnt--;
        } while (cnt != 0);
    }
}

void ObjectSystem_Configure(s32 mode)
{
    void *objects;
    void *states;
    void *table;
    u32 size;
    volatile u32 zero;

    if (mode == 3) {
        objects = Runtime_AllocateBlock(4, ANIMATION_OBJECT_COUNT * sizeof(struct AnimationObject));
        states = Runtime_AllocateBlock(3, ANIMATION_OBJECT_COUNT * sizeof(struct AnimationEntry));
    } else {
        objects = Runtime_AllocateHeapBlock(4, ANIMATION_OBJECT_COUNT * sizeof(struct AnimationObject));
        states = Runtime_AllocateHeapBlock(3, ANIMATION_OBJECT_COUNT * sizeof(struct AnimationEntry));
    }
    PaletteDma_LoadBlock();
    zero = 0;
    Dma_Set((const void *)&zero, objects, 0x85000000 | (ANIMATION_OBJECT_COUNT * sizeof(struct AnimationObject) / 4), REG_DMA3);
    zero = 0;
    Dma_Set((const void *)&zero, states, 0x85000000 | (ANIMATION_OBJECT_COUNT * sizeof(struct AnimationEntry) / 4), REG_DMA3);
    VramBlock_LoadCached(93, 128, Object_ShadowTiles);
    size = (u32)Tile_CopyStridedCodeSize;
    table = Runtime_AllocateHeapBlock(53, size);
    Dma_Set((const void *)Tile_CopyStrided, table, 0x84000000 | (size >> 2),
            REG_DMA3);
}

struct AnimationEntry *AnimationObject_Allocate(s32 id)
{
    struct AnimationMetadata *metadata;
    struct AnimationEntry *entry;
    struct AnimationEntry *found;
    struct AnimationEntry *object;
    s32 i;
    s32 frames;
    s32 animation;

    found = NULL;
    metadata = Resource_GetMetadataRecordFar(id);
    entry = gAnimationObjects[0];
    object = NULL;

    if (metadata->width != 0) {
        for (i = 0; i < ANIMATION_OBJECT_COUNT; i++, entry++) {
            if (entry->kind == 0) {
                found = entry;
                break;
            }
        }
        if (found != NULL) {
            frames = metadata->frames;
            object = found;
            object->anim_id = id;
            if (frames == 0)
                frames = Animation_LookupValueByKey(id);
            /* FAKEMATCH: the original address-word boundary keeps the two list stores before codec/frame setup; pointer assignments reorder them in the complete 134-byte allocation path. */
            animation = metadata->animation;
            *(s32 *)&object->scripts = animation;
            *(s32 *)&object->frames = frames;
            object->mode = metadata->frame_codec;
            object->frame = 0xff;
            *(u32 *)&object->script = *(u32 *)animation;
            object->pos = 0;
            object->kind = metadata->draw_kind;
            object->param = 0;
        }
    }
    return object;
}
