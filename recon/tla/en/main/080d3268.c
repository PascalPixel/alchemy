#include "types.h"
#include "object_runtime.h"

struct AnimationFrameSet {
    u8 unknown_00[16];
    u8 *codes;
    u8 index;
};

struct AnimationPlayer {
    u8 unknown_00[24];
    u8 animation;
    u8 unknown_19[15];
    struct AnimationFrameSet *frames;
};

void WaitFrames(s32 frames);

void ObjectMotion_WaitForAnimationChange(u32 id)
{
    struct ObjectRuntime *object = ObjectTable_Get(id);
    struct AnimationPlayer *player;
    u32 saved;
    volatile u32 *animation;
    s32 frames;

    if (object != NULL && object->animation_kind == 1) {
        player = object->animation;
        frames = 0;
        animation = &saved;
        *animation = player->animation;
        for (; frames <= 89; frames++) {
            WaitFrames(1);
            if (*animation != player->animation)
                break;
            if (player->frames != NULL && player->frames->codes != NULL
                && player->frames->codes[player->frames->index] == 241)
                break;
        }
    }
}
