#include "types.h"

typedef struct {
    u8 reserved_00[0x1e];
    u16 animation_frame;
    u8 reserved_20[6];
    u8 animation_state;
} MapObject;

typedef struct {
    u8 reserved_00[0x50];
    MapObject * volatile object;
} MapSprite;

extern u8 * volatile Data_03001e70;

#define initialize_story_actor_animation_frame Func_020002cc

s32 initialize_story_actor_animation_frame(MapSprite *sprite)
{
    volatile u16 *animation_frame = (u16 *)(Data_03001e70 + 282);
    MapObject *object = sprite->object;

    object->animation_frame = *animation_frame;
    object->animation_state = 0;
    return 1;
}
