// Draft, differ 32 of 91 (was 79), rewritten from the listing: on even frames
// creates object kind 0x11d at a random point of one of two columns (cells 155
// and 178, chosen by frame bit 1), with update Func_020013e4. Left: the native
// computes each branch's x as r*7 before the second Random16Far call and scales
// it after, in r5, and builds the create call's x in place in r5; this form
// computes x after both calls. Tried: x computed in steps (38), x built in
// place before the call (43), permute 6 minutes.
#include "../../../../games/tla/src/scene/mars_lighthouse_star_room/mars_lighthouse_star_room.h"
#include "frame.h"

void Func_020013e4(void);

void Func_02001424(void) {
  u32 frame = gFrameCount;
  struct ObjectRuntime *object;
  struct FieldSprite *sprite;
  u32 x;
  s32 z;
  s32 column;

  if (frame & 1)
    return;
  if ((frame >> 1) & 1) {
    x = (u32)(Random16Far() * 7 * 4) >> 16;
    z = Random16Far() * 15;
    column = 155;
  } else {
    x = (u32)(Random16Far() * 7 * 4) >> 16;
    z = Random16Far() * 15;
    column = 178;
  }
  object = ObjectCreate_(0x11d, (x << 16) + (column << 18), 0x200000,
                         (((u32)(z << 3) >> 16) << 16) + 0x1cc0000);
  if (object == NULL)
    return;
  object->unk5c[0] = 2;
  object->motionFlags = 0;
  object->action = 0;
  object->priorityFlags &= 0xfe;
  sprite = object->animation;
  sprite->priority = 2;
  ObjectDispatch_SetSingleChildFlags(object, 0);
  object->scaleX = 0x8000;
  object->scaleY = 0x8000;
  *(s32 *)&object->linkedObject = 60;
  object->update = Func_020013e4;
  Func_020041cc(object, 5);
  Animation_SetStateFlags(object, 1);
}
