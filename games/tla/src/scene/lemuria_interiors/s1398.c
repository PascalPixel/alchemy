#include "types.h"
#include "field_event.h"

struct SpriteFrames {
    u8 unknown_00[5];
    u8 frame;
};

struct AnimatedSprite {
    u8 unknown_00[0x12];
    s16 rotation;
    u8 unknown_14[5];
    u8 frame;
    u8 next_frame;
    u8 unknown_1b[0xd];
    struct SpriteFrames *frames;
};

void Func_02001398(struct FieldActor *object)
{
    struct AnimatedSprite *sprite = (struct AnimatedSprite *)object->sprite;
    struct SpriteFrames *frames = sprite->frames;

    switch (object->timer & 3) {
    case 0:
        frames->frame = 7;
        sprite->frame = 1;
        sprite->next_frame = 2;
        break;
    case 1:
        sprite->frame = 1;
        sprite->next_frame = 0;
        break;
    case 2:
        frames->frame = 7;
        sprite->frame = 1;
        sprite->next_frame = 0;
        break;
    case 3:
        frames->frame = 0;
        sprite->frame = 1;
        sprite->next_frame = 0;
        break;
    }
    object->timer++;
}
