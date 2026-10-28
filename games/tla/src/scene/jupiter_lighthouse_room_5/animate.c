#include "types.h"
#include "field_event.h"

extern u32 gFrameCount;

u32 __umodsi3();
void Animation_ApplyChildValues();

struct SpritePictures {
  u8 unk00[5];
  u8 picture;
};

struct AnimatedSprite {
  u8 unk00[0x19];
  u8 picture;
  u8 nextPicture;
  u8 unk1b[0xd];
  struct SpritePictures *pictures;
};

void Func_020000c8(struct FieldActor *object) {
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
    sprite->nextPicture = 0;
    break;
  }
  object->timer++;
}

void Func_02000120(struct FieldActor *object) {
  u32 frameCount = gFrameCount;

  if (frameCount & 1)
    Animation_ApplyChildValues(object, (frameCount >> 1) % 6);
}
