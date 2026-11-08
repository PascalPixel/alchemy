#include "types.h"
#include "iwram_call.h"
#include "field_event.h"

struct SpritePictures {
  u8 unk00[5];
  u8 picture;
};

struct AnimatedSprite {
  u8 unk00[0x12];
  s16 rotation;
  u8 unk14[5];
  u8 picture;
  u8 nextPicture;
  u8 unk1b[0xd];
  struct SpritePictures *pictures;
};

/// Steps the sprite through a four-picture cycle on the actor's timer, as
/// overlay 68c's Func_02001398 does, but leaves the next picture alone on the
/// fourth step.
void Func_080e9e8c(struct FieldActor *object) {
  struct AnimatedSprite *sprite = (struct AnimatedSprite *)object->sprite;
  struct SpritePictures *pictures = sprite->pictures;

  switch (object->timer & 3) {
  case 0:
    pictures->picture = 7;
    sprite->picture = 1;
    sprite->nextPicture = 2;
    break;
  case 1:
    sprite->picture = 1;
    sprite->nextPicture = 0;
    break;
  case 2:
    pictures->picture = 7;
    sprite->picture = 1;
    sprite->nextPicture = 0;
    break;
  case 3:
    pictures->picture = 0;
    sprite->picture = 1;
    break;
  }
  object->timer++;
}

struct Chase {
  s32 x;
  s32 z;
  s32 unk08;
  s32 targetX;
  s32 targetZ;
};

s32 ArcTan2(s32 y, s32 x);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

/// Moves toward the target by speed, stopping on it on overshoot.
void Motion_StepTowardTarget(struct Chase *chase, s32 speed) {
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
