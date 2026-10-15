#include "types.h"
#include "iwram_call.h"
#include "field_event.h"

struct SpriteFrames
{
    u8 unk00[5];
    u8 frame;
};

struct AnimatedSprite
{
    u8                   unk00[0x12];
    s16                  rotation;
    u8                   unk14[5];
    u8                   frame;
    u8                   nextFrame;
    u8                   unk1b[0xd];
    struct SpriteFrames *frames;
};

// Steps the sprite through a four-frame cycle on the actor's timer, as
// overlay 68c's Func_02001398 does, but leaves the next frame alone on the
// fourth step.
void Func_080e9e8c(struct FieldActor *object)
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
        break;
    }
    object->timer++;
}

struct Chase
{
    s32 x;
    s32 z;
    s32 unk08;
    s32 targetX;
    s32 targetZ;
};

s32 ArcTan2(s32 y, s32 x);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

// 目標へ speed だけ近づけ、通り過ぎたら目標に合わせる
void Func_080e9ee4(struct Chase *chase, s32 speed)
{
    s32 dx = chase->targetX - chase->x;
    s32 dz = chase->targetZ - chase->z;
    u16 angle = ArcTan2(dz, dx);

    chase->x += Iwram_MulQ16(Trig_Cos(angle), speed);
    chase->z += Iwram_MulQ16(Trig_Sin(angle), speed);
    {
        s32 nextDx = chase->targetX - chase->x;
        s32 nextDz = chase->targetZ - chase->z;

        if ((nextDx ^ dx) & 0x80000000)
            chase->x = chase->targetX;
        if ((dz ^ nextDz) & 0x80000000)
            chase->z = chase->targetZ;
    }
}
