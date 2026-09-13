#include "types.h"
#include "global_cells.h"
#include "metadata_lookup.h"

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

struct AnimationMetadata *Sys_Run(s32);
s32 Sys_Check(s32);

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
