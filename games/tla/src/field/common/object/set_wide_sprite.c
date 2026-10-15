#include "types.h"
#include "object_runtime.h"

struct ObjectSprite {
  u8 unk00[5];
  u8 attr0High;
  u8 unk06;
  u8 attr1High;
  u8 unk08[0xf];
  u8 yOffset;
};

void Object_SetWideSprite(u32 objectId, s32 mode) {
  struct ObjectRuntime *object;
  struct ObjectSprite *sprite;

  object = ObjectTable_Get(objectId);
  if (object == 0)
    return;
  if (mode == 0) {
    object->x = 0;
    object->z = 0;
    object->unk4c = 0x31415927;
    return;
  }
  sprite = object->animation;
  if (sprite == 0)
    return;
  sprite->yOffset += 4;
  sprite->attr0High = (sprite->attr0High & 0x3f) | 0x40;
  sprite->attr1High &= 0x3f;
}

void Object_SetSquareSprite(u32 objectId) {
  struct ObjectRuntime *object;
  struct ObjectSprite *sprite;

  object = ObjectTable_Get(objectId);
  if (object == 0)
    return;
  sprite = object->animation;
  if (sprite == 0)
    return;
  /* FAKEMATCH: the one-pass block reads the shape byte before the offset's 8 is
   * built; written plainly the 8 is built first (differ 2). */
  do {
    sprite->yOffset = 8;
    sprite->attr0High &= 0x3f;
  } while (0);
  sprite->attr1High = (sprite->attr1High & 0x3f) | 0x40;
}
