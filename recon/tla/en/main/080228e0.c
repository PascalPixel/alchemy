#include "types.h"
#include "animspr.h"
#include "metadata_lookup.h"

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
    struct AnimationEntry *entries[4];
};
s32 Animation_LookupValueByKey(s32 key);
s32 InitializeAnimationObjects(struct MetadataSlotState *state)
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
        *(s32 *)&object->scripts = animation;
        object->mode = metadata->frame_codec;
        object->frame = 0xff;
        object->script = 0;
        object->pos = 0;
    }

    return 0;
}
