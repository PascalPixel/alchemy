#include "../../../../games/tla/src/scene/mars_lighthouse_star_room/mars_lighthouse_star_room.h"
#include "frame.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Func_020013e4(void);

void Func_02001424(void) {
  u32 frame = gFrameCount;
  struct Actor *object;
  struct FieldSprite *sprite;
  u32 x, r;
  s32 z;
  s32 column;

  if (frame & 1)
    return;
  if ((frame >> 1) & 1) {
    r = Random16Far();
    x = r << 3;
    x -= r;
    z = Random16Far() * 15;
    x <<= 2;
    x >>= 16;
    column = 155;
  } else {
    r = Random16Far();
    x = r << 3;
    x -= r;
    z = Random16Far() * 15;
    x <<= 2;
    x >>= 16;
    column = 178;
  }
  column <<= 18;
  z <<= 3;
  x <<= 16;
  x += column;
  z = (u32)z >> 16;
  z <<= 16;
  z += 0x1cc0000;
  object = ObjectCreate_(0x11d, x, 0x200000, z);
  if (object != NULL) {
    /* FAKEMATCH(barrier): an empty asm clobbering r0, so the child flags call
     * copies object in again; as no asm, 7 of 91 instructions differ in tla-en.
     */
    asm("" : : : "r0");
    object->unk5c[0] = 2;
    object->motionFlags = 0;
    object->action = 0;
    object->priorityFlags &= 0xfe;
    sprite = object->animation;
    sprite->priority = 2;
    Actor_SetSingleChildFlags(object, 0);
    object->scaleX = 0x8000;
    object->scaleY = 0x8000;
    *(s32 *)&object->watchedObject = 60;
    object->update = Func_020013e4;
    Func_020041cc(object, 5);
    Animation_SetStateFlags(object, 1);
  }
}
