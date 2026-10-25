// Draft of overlay 6aa's Func_02002b4c, written from its listing; its loop is a
// goto loop, as the native loads IwramMulQ16's address inside it: differ 18 of
// 98. Left: the native spills the four arguments in order before it sets the
// counter, gives 0x4000 sl and the angle r9 where this swaps them, and stores
// the gravity after it halves the size.
#include "../../../../games/tla/src/scene/mars_lighthouse_entrance/room.h"
#include "iwram_call.h"

extern const u8 gData_02005714[];

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
u32 Random16Far(void);

/// Throws two sparks (object kind 325) from (x, y, z), each upward at about
/// speed plus 0x14000 with a random spread, at a random angle and size.
void Func_02002b4c(s32 x, s32 y, s32 z, s32 speed) {
  s32 i;

  i = 1;
next:
  {
    struct ObjectRuntime *object = Func_02004bd8(325, x, y, z);

    if (object != NULL) {
      s32 angle;
      s32 rise;
      s32 radius;
      u32 size;

      rise = speed + (Random16Far() >> 4) - (Random16Far() >> 4);
      angle = 0x4000 + rise;
      radius = Random16Far() * 8;
      size = Random16Far();
      Func_02004bd0(object, gData_02005714);
      ObjectDispatch_SetSingleChildFlags(object, 0);
      object->velocityY = rise + 0x14000;
      object->velocityZ = Iwram_MulQ16(radius, Math_Cosine(angle));
      object->velocityX = Iwram_MulQ16(radius, Math_Sine(angle));
      object->acceleration = 0;
      object->gravity = 0x547a;
      object->scaleX = (size >> 1) + 0x4000;
      object->scaleY = object->scaleX;
      object->bounce = 0x8000;
      Animation_ApplyChildValues(object, 1);
    }
  }
  if (--i >= 0)
    goto next;
}
