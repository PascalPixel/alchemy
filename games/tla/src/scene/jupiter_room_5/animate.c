#include "types.h"
#include "field_event.h"

extern u32 gFrameCount;

u32  __umodsi3();
void Animation_ApplyChildValues();

struct SpriteFrames
{
    u8 unk00[5];
    u8 frame;
};

struct AnimatedSprite
{
    u8                   unk00[0x19];
    u8                   frame;
    u8                   nextFrame;
    u8                   unk1b[0xd];
    struct SpriteFrames *frames;
};

void Func_020000c8(struct FieldActor *object)
{
    struct AnimatedSprite *sprite = (struct AnimatedSprite *)object->sprite;
    struct SpriteFrames   *frames = sprite->frames;

    switch (object->timer & 3)
    {
    case 0:
        frames->frame = 7;
        sprite->frame = 1;
        sprite->nextFrame = 2;
        break;
    case 1:
        sprite->frame = 1;
        sprite->nextFrame = 0;
        break;
    case 2:
        frames->frame = 7;
        sprite->frame = 1;
        sprite->nextFrame = 0;
        break;
    case 3:
        frames->frame = 0;
        sprite->frame = 1;
        sprite->nextFrame = 0;
        break;
    }
    object->timer++;
}

void Func_02000120(struct FieldActor *object)
{
    u32 tick = gFrameCount;

    if (tick & 1)
        Animation_ApplyChildValues(object, (tick >> 1) % 6);
}
