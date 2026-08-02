#include "metadata_lookup.h"
#include "types.h"

struct AnimationMetadata_0800bbc0 {
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
    const s32 *animation;
};

struct AnimationObject_0800bbc0 {
    s16 resource_id;
    u8 padding02[2];
    u8 draw_kind;
    u8 state;
    u8 padding06;
    u8 frame_codec;
    s32 frames;
    const s32 *animation;
    s32 current;
    u8 finished;
    u8 padding15;
    u8 marker;
    u8 padding17;
};

extern struct AnimationObject_0800bbc0 *Data_03001e5c;

s32 Func_0800b798(s32);

/*
 * Allocate one of 64 animation-object records and seed it from the resource
 * metadata.  A zero-width resource is not drawable.  Metadata may omit its
 * frame count, in which case Func_0800b798 derives it from the animation.
 */
struct AnimationObject_0800bbc0 *Func_0800bbc0(s32 resource_id)
{
    struct AnimationMetadata_0800bbc0 *metadata =
        (struct AnimationMetadata_0800bbc0 *)Func_08185000(resource_id);
    struct AnimationObject_0800bbc0 *object = 0;
    s32 index;

    if (metadata->width == 0)
        return 0;

    for (index = 0; index < 64; index++) {
        if (Data_03001e5c[index].draw_kind == 0) {
            object = &Data_03001e5c[index];
            break;
        }
    }
    if (object == 0)
        return 0;

    object->resource_id = (s16)resource_id;
    object->frames = metadata->frames;
    if (object->frames == 0)
        object->frames = Func_0800b798(resource_id);
    object->animation = metadata->animation;
    object->frame_codec = metadata->frame_codec;
    object->marker = 0xff;
    object->current = metadata->animation[0];
    object->finished = 0;
    object->draw_kind = metadata->draw_kind;
    object->state = 0;
    return object;
}
