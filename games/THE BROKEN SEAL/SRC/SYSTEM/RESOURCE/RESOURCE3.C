#include "TYPES.H"
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

struct AnimationMetadata *Resource_GetMetadataRecordFar(s32);
s32 Animation_LookupValueByKey(s32);

struct MetadataSlotState {
    u8 unknown_00[24];
    s32 shifted;
    u8 unknown_1c[4];
    u8 first;
    u8 second;
    u8 third;
    u8 fourth;
    u8 unknown_24[3];
    u8 count;
    s32 slots[4];
};

struct MetadataRecord {
    u8 first;
    u8 second;
    u16 value;
    u8 unknown_04[2];
    u8 third;
    u8 fourth;
};

void ResourceMetadata_ClearRecord(void *);

struct AnimationMetadata2 {
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

struct AnimationObject2 {
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

struct AnimationSetupState2 {
    u8 padding00[34];
    u8 adjust_x;
    u8 adjust_y;
    u8 selected_animation;
    u8 padding37[2];
    u8 count;
    struct AnimationObject2 *entries[4];
};

extern u8 Func_0800a418[];

/* The strided tile copy runs from a heap copy of itself. */
extern u8 Tile_CopyStridedCodeSize[];

/* The 16 by 8 oval, in colour 1, that objects draw beneath them. */
extern const u8 Object_ShadowTiles[];
void *Runtime_AllocateBlock(s32 slot, s32 size);
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void PaletteDma_LoadBlock(void);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);

struct AnimationMetadata3 {
    u8 width;
    u8 height;
    u16 scale;
    u8 draw_kind;
    u8 animation_count;
    s8 adjust_x;
    s8 adjust_y;
    u8 reserved_08[2];
    u8 frame_codec;
    u8 reserved_0b;
    s32 frames;
    s32 animation;
};

struct AnimationObject3 {
    s16 id;
    u8 reserved_02[2];
    u8 draw_kind;
    u8 field_05;
    u8 reserved_06;
    u8 frame_codec;
    s32 frames;
    s32 animation;
    u32 current;
    u8 state;
    u8 reserved_15;
    u8 marker;
};

extern struct AnimationObject3 *gAnimationObjects[];
s32 Animation_LookupValueByKey(s32 key);

struct AnimationObject3 *AnimationObject_Allocate(s32 id);

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

s32 Animation_InitializeObjects(struct AnimationSetupState *state)
{
    s32 index;

    for (index = 0; index < state->count; index++) {
        struct AnimationObject *object = state->objects[index];
        struct AnimationMetadata *metadata = Resource_GetMetadataRecordFar(object->id);
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
            frames = Animation_LookupValueByKey(object->id);

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
void Animation_InitWorkFromMetadata(void *work)
{
    s32 value;
    s32 z;
    void *info;

    if (work != NULL) {
        info = Resource_GetMetadataRecordFar(FIELD_AT_OFFSET(work, s16, 0));
        if (FIELD_AT_OFFSET(info, u8, 0) != 0) {
            value = FIELD_AT_OFFSET(info, s32, 0x0c);
            if (value == 0) {
                value = Animation_LookupValueByKey(FIELD_AT_OFFSET(work, s16, 0));
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
    value = AnimationObject_Allocate(id);
    if (value == 0)
        return 0;
    state->slots[index] = value;
    metadata = Resource_GetMetadataRecordFar(id);
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
                *(s8 *)(rec + 0x27) = (s8)no;
        }
    }
}

void Animation_SetWorkEntry(void *work, s32 no)
{
    s32 hi;
    void *info;
    s32 value;

    hi = 0x80 & no;
    if (FIELD_AT_OFFSET(work, s32, 0x0c) != 0) {
        info = Resource_GetMetadataRecordFar((s32)FIELD_AT_OFFSET(work, s16, 0));
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

s32 AnimationObjects_SelectAnimation(struct AnimationSetupState2 *state, s32 flags)
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
            struct AnimationObject2 *entry = state->entries[index];
            struct AnimationMetadata2 *metadata;
            s32 selected_animation;

            if (entry == 0)
                goto next_entry;
            if (entry->animation_table == 0)
                goto next_entry;

            metadata = Resource_GetMetadataRecordFar(entry->id);
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
struct AnimationObject3 *AnimationObject_Allocate(s32 id)
{
    struct AnimationMetadata3 *metadata;
    struct AnimationObject3 *entry;
    struct AnimationObject3 *found;
    struct AnimationObject3 *object;
    s32 i;
    s32 frames;
    s32 animation;
    /* FAKEMATCH: the zero stored into state and field_05 is a halfword field of
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
            if (entry->draw_kind == 0) {
                found = entry;
                break;
            }
        }
        if (found != NULL) {
            zero.v = 0;
            frames = metadata->frames;
            object = found;
            object->id = (s16)id;
            if (frames == 0)
                frames = Animation_LookupValueByKey(id);
            animation = metadata->animation;
            object->animation = animation;
            object->frames = frames;
            object->frame_codec = metadata->frame_codec;
            object->marker = 0xff;
            object->current = *(u32 *)animation;
            object->state = zero.v;
            object->draw_kind = metadata->draw_kind;
            object->field_05 = zero.v;
        }
    }
    return object;
}
