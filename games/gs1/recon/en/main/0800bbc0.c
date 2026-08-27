#include "types.h"

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

struct AnimationObject {
    s16 id;
    u8 padding02[2];
    u8 draw_kind;
    u8 unknown05;
    u8 padding06;
    u8 frame_codec;
    s32 frames;
    s32 animation;
    u32 current;
    u8 state;
    u8 padding15;
    u8 marker;
};

extern struct AnimationObject *Data_03001e5c[];
extern struct AnimationMetadata *Func_08185000(s32 id);
extern s32 Func_0800b798(s32 id);
extern u8 Data_00000000[];

struct AnimationObject *Func_0800bbc0(s32 id)
{
    struct AnimationMetadata *metadata;
    struct AnimationObject *entry;
    struct AnimationObject *found;
    struct AnimationObject *object;
    s32 i;
    s32 frames;
    s32 animation;
    u8 zero;

    found = 0;
    metadata = Func_08185000(id);
    entry = Data_03001e5c[0];
    object = 0;

    if (metadata->width != 0) {
        for (i = 0; i <= 63; i++, entry++) {
            if (entry->draw_kind == 0) {
                found = entry;
                break;
            }
        }
        if (found != 0) {
            zero = (u8)(u32)Data_00000000;
            frames = metadata->frames;
            object = found;
            object->id = (s16)id;
            if (frames == 0)
                frames = Func_0800b798(id);
            animation = metadata->animation;
            object->animation = animation;
            object->frames = frames;
            object->frame_codec = metadata->frame_codec;
            object->marker = 0xff;
            object->current = *(u32 *)animation;
            object->state = zero;
            object->draw_kind = metadata->draw_kind;
            object->unknown05 = zero;
        }
    }
    return object;
}
