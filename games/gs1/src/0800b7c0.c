#include "metadata_lookup.h"
#include "types.h"

struct Object_0800b7c0 {
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

struct Metadata_0800b7c0 {
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

struct State_0800b7c0 {
    u8 padding00[24];
    u32 scale;
    u8 padding1c[4];
    u8 width;
    u8 height;
    s8 adjust_x;
    s8 adjust_y;
    u8 padding24[3];
    u8 count;
    struct Object_0800b7c0 *objects[4];
};

extern s32 Func_0800b798(s32);

s32 Func_0800b7c0(struct State_0800b7c0 *state)
{
    s32 index;

    for (index = 0; index < state->count; index++) {
        struct Object_0800b7c0 *object = state->objects[index];
        struct Metadata_0800b7c0 *metadata = Func_08185000(object->id);
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
            frames = Func_0800b798(object->id);

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
