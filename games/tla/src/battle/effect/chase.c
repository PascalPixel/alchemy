#include "types.h"
#include "iwram_call.h"
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

// Steps the sprite through a four-frame cycle on the actor's timer, as
// overlay 68c's Func_02001398 does, but leaves the next frame alone on the
// fourth step.
void Func_080e9e8c(struct FieldActor *object)
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
        break;
    }
    object->timer++;
}

struct Chase {
    s32 x;
    s32 z;
    s32 unknown_08;
    s32 target_x;
    s32 target_z;
};

s32 ArcTan2(s32 y, s32 x);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

// 目標へ speed だけ近づけ、通り過ぎたら目標に合わせる
void Func_080e9ee4(struct Chase *chase, s32 speed)
{
    s32 dx = chase->target_x - chase->x;
    s32 dz = chase->target_z - chase->z;
    u16 angle = ArcTan2(dz, dx);

    chase->x += Iwram_MulQ16(Trig_Cos(angle), speed);
    chase->z += Iwram_MulQ16(Trig_Sin(angle), speed);
    {
        s32 next_dx = chase->target_x - chase->x;
        s32 next_dz = chase->target_z - chase->z;

        if ((next_dx ^ dx) & 0x80000000)
            chase->x = chase->target_x;
        if ((dz ^ next_dz) & 0x80000000)
            chase->z = chase->target_z;
    }
}
