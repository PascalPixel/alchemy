#include "TYPES.H"
#include "ANIMSPR.H"
#include "GLOBAL_CELLS.H"
#include "METADATA_LOOKUP.H"
#include "DMA.H"

extern u8 gMenuCtrlWork[];

/* animation/lookup_value_by_key.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

struct LookupEntry {
    s32 key;
    s32 value;
};

/* animation/initialize_objects.c */
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

struct AnimationMetadata *Resource_GetMetadataRecordFar(s32);
s32 Animation_LookupValueByKey(s32);

void ResourceMetadata_ClearRecord(void *);

extern u8 Func_0800a418[];

/* The strided tile copy runs from a heap copy of itself. */
extern u8 Tile_CopyStridedCodeSize[];

/* The 16 by 8 oval, in colour 1, that objects draw beneath them. */
extern const u8 Object_ShadowTiles[];
void *Runtime_AllocateBlock(s32 slot, s32 size);
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void PaletteDma_LoadBlock(void);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);

extern struct AnimationEntry *gAnimationObjects[];
s32 Animation_LookupValueByKey(s32 key);

struct AnimationEntry *AnimationObject_Allocate(s32 id);

/* One loadable number and the resource that holds its script. */
struct ResourceSlotNumber {
    u16 resource;
    s16 number;
};

struct ResourceSlot {
    s32 tag;
    void *buffer;
};

struct ResourceSlotWork {
    u8 unknown_00[0x1c];
    struct ResourceSlot slots[8];
};

/* Up to 256 numbers, ended by number zero. */
extern struct ResourceSlotNumber ResourceSlot_NumberTable[];
/* Five tables that map a script's bytes below 0xe0 for a text variant. */
extern u8 ResourceSlot_ConversionTables[][256];

void *Resource_GetTableEntry(s32 resource_id);
u32 Resource_DecodeType01(const void *source, void *destination);

/*
 * Load a number's script into a slot's buffer: find its resource, decode
 * it, turn the leading offset table (ended by a zero word) into pointers
 * and, for a text variant, convert the bytes that follow the table. Returns
 * the area of the number's metadata record, or zero when the slot or the
 * number does not exist.
 */
s32 ResourceSlot_Load(u32 slot, u32 *buffer, s32 number, u32 variant)
{
    struct AnimationMetadata *metadata;
    struct ResourceSlotWork *work;
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

    if (slot > 7)
        return 0;
    work = *(struct ResourceSlotWork **)gMenuCtrlWork;
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
    struct LookupEntry *p;

    p = (struct LookupEntry *)(*(u32 *)((u32)&gMenuCtrlWork) + 0x1c);
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

/* Pointer assignments reordered the 168/68/134-byte initializers.
   Keep scalar address words at the metadata boundary. */
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

        object->kind = metadata->draw_kind;
        animation = metadata->animation;
        *(s32 *)&object->frames = frames;
        *(s32 *)&object->field_0c = animation;
        object->mode = metadata->frame_codec;
        object->frame = 0xff;
        object->script = 0;
        object->pos = 0;
    }

    return 0;
}

/* animation/init_work_from_metadata.c */
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
            work->kind = info->draw_kind;
            *(s32 *)&work->field_0c = info->animation;
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
    struct AnimationEntry **remaining_slot;
    struct AnimationEntry **slot_cursor;
    s32 slot_value;
    s32 later_slot_count;
    u32 slot_index;
    u32 later_index;

    if (state != NULL && handle != 0) {
        ResourceMetadata_ClearRecord((void *)handle);
        slot_index = 0;
        if (handle != (s32)state->entries[0]) {
            slot_cursor = state->entries;
        next_slot:
            slot_index++;
            if (slot_index <= 3U) {
                slot_cursor++;
                if (handle != (s32)*slot_cursor)
                    goto next_slot;
            }
        }
        if (slot_index != 4) {
            state->entries[slot_index] = 0;
            later_index = slot_index + 1;
            later_slot_count = 0;
            if (later_index <= 3U) {
                remaining_slot = (struct AnimationEntry **)(later_index * sizeof *remaining_slot + (u32)state + 0x28);
                do {
                    slot_value = (s32)*remaining_slot++;
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

void ResourceMetadata_ReleaseSlot(struct AnimationObject *group, u32 no)
{
    struct AnimationEntry **p;
    struct AnimationEntry *t;
    struct AnimationEntry *v;
    s32 cnt;
    u32 i;

    if (group != NULL && no <= 3) {
        v = group->entries[no];
        if (v != NULL) {
            ResourceMetadata_ClearRecord(v);
            group->entries[no] = NULL;
            i = no + 1;
            cnt = 0;
            if (i <= 3) {
                p = (struct AnimationEntry **)(i * sizeof *p + (u32)group + 0x28);
                do {
                    t = *p++;
                    if (t != NULL)
                        cnt++;
                    i++;
                } while (i <= 3);
            }
            if (cnt == 0)
                group->count = (s8)no;
        }
    }
}

void Animation_SetWorkEntry(struct AnimationEntry *work, s32 no)
{
    s32 hi;
    struct AnimationMetadata *info;
    s32 value;

    hi = 0x80 & no;
    if (work->field_0c != 0) {
        info = Resource_GetMetadataRecordFar(work->anim_id);
        if (no < (s32)info->animation_count) {
            value = ((s32 *)work->field_0c)[no];
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

s32 AnimationObjects_SelectAnimation(struct AnimationObject *state, s32 flags)
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
            if (entry->field_0c == 0)
                goto next_entry;

            metadata = Resource_GetMetadataRecordFar(entry->anim_id);
            if (flags >= metadata->animation_count)
                goto next_entry;

            selected_animation = ((s32 *)entry->field_0c)[flags];
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

s32 AnimationObjects_SetHalfword02OnActive(struct AnimationObject *group, s32 val)
{
    u8 n = group->count;
    struct AnimationEntry **p;
    s32 cnt;

    if (n != 0) {
        val = (s32)((u32)val << 4);
        p = group->entries;
        cnt = n;
        do {
            struct AnimationEntry *entry = *p++;
            if (entry != 0 && entry->field_0c != 0)
                entry->timer = val;
            cnt--;
        } while (cnt != 0);
    }
    return 0;
}

void AnimationObjects_SetField15OnActive(struct AnimationObject *group, s32 val)
{
    u8 n = group->count;
    if (n != 0) {
        struct AnimationEntry **p = group->entries;
        s32 cnt = n;
        do {
            struct AnimationEntry *entry = *p++;
            if (entry != 0 && entry->field_0c != 0)
                entry->step = val;
            cnt--;
        } while (cnt != 0);
    }
}

/* Allocates and clears the object and state blocks (from the heap in mode
   3), loads the object graphics and copies the strided tile copy routine
   into heap block 53. */
void ObjectSystem_Configure(s32 mode)
{
    void *objects;
    void *states;
    void *table;
    u32 size;
    volatile u32 zero;

    if (mode == 3) {
        objects = Runtime_AllocateBlock(4, 0xe00);
        states = Runtime_AllocateBlock(3, 0x600);
    } else {
        objects = Runtime_AllocateHeapBlock(4, 0xe00);
        states = Runtime_AllocateHeapBlock(3, 0x600);
    }
    PaletteDma_LoadBlock();
    zero = 0;
    Dma_Set((const void *)&zero, objects, 0x85000380, (volatile u32 *)0x040000d4);
    zero = 0;
    Dma_Set((const void *)&zero, states, 0x85000180, (volatile u32 *)0x040000d4);
    VramBlock_LoadCached(93, 128, Object_ShadowTiles);
    size = (u32)Tile_CopyStridedCodeSize;
    table = Runtime_AllocateHeapBlock(53, size);
    Dma_Set((const void *)Func_0800a418, table, 0x84000000 | (size >> 2),
            (volatile u32 *)0x040000d4);
}

/* Takes the first free object of the 64 animation objects, one whose draw
 * kind is zero, and sets it up from the metadata of id: its frames, looked up
 * by id when the metadata gives none, its animation table and first entry,
 * draw kind and frame codec. Returns the object, or NULL when the metadata
 * has zero width or no object is free. */
struct AnimationEntry *AnimationObject_Allocate(s32 id)
{
    struct AnimationMetadata *metadata;
    struct AnimationEntry *entry;
    struct AnimationEntry *found;
    struct AnimationEntry *object;
    s32 i;
    s32 frames;
    s32 animation;
    /* FAKEMATCH: the zero stored into pos and param is a halfword field of
       a struct, so GCC takes it from the literal pool ahead of the frames
       load and keeps it in r8, as the ROM does; a plain zero local loads
       it after the frames. */
    struct { u16 v; } zero;

    found = NULL;
    metadata = Resource_GetMetadataRecordFar(id);
    entry = gAnimationObjects[0];
    object = NULL;

    if (metadata->width != 0) {
        for (i = 0; i <= 63; i++, entry++) {
            if (entry->kind == 0) {
                found = entry;
                break;
            }
        }
        if (found != NULL) {
            zero.v = 0;
            frames = metadata->frames;
            object = found;
            object->anim_id = (s16)id;
            if (frames == 0)
                frames = Animation_LookupValueByKey(id);
            animation = metadata->animation;
            *(s32 *)&object->field_0c = animation;
            *(s32 *)&object->frames = frames;
            object->mode = metadata->frame_codec;
            object->frame = 0xff;
            *(u32 *)&object->script = *(u32 *)animation;
            object->pos = zero.v;
            object->kind = metadata->draw_kind;
            object->param = zero.v;
        }
    }
    return object;
}
