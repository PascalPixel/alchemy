#include "mars_lighthouse_room_m.h"

struct SpriteParts {
  u8 unk00[9];
  u8 unk09Low : 2;
  u8 part : 2;
};
extern const u8 gData_020032d4[];

void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_02003120(struct ObjectRuntime *object, const void *script);

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_020032d4; Func_02001e1c of the Mars Lighthouse entrance is the same
/// function.
void Func_02001cc4(struct ObjectRuntime *object, s32 part) {
  /* FAKEMATCH(temporary): one zero for every cleared field; as six stores of
   * 0, 29 of 30 instructions differ in tla-en. */
  s32 zero = 0;

  object->motionFlags = zero;
  object->collisionFlags = zero;
  {
    s32 three = 3;

    /* FAKEMATCH(barrier): an asm holding a 3 builds the part mask before the
     * sprite loads; without it, 4 of 30 instructions differ in tla-en. */
    asm volatile("" : : "r"(three));
    ((struct SpriteParts *)object->animation)->part = part;
  }
  ObjectDispatch_SetSingleChildFlags(object, 0);
  Func_02003120(object, gData_020032d4);
  object->action = zero;
  object->speedLimit = zero;
  object->acceleration = zero;
}

void Func_02001d08(struct MarsLighthouseRoomMVector *position, s32 value,
                   s32 flag) {
  struct ObjectRuntime *object = (struct ObjectRuntime *)Value4(
      (s32 (*)())Func_02003128, 366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -value;
    Func_02001cc4(object, 3);
    object->update = Func_02001988;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_02003118(object, 2);
  }
}

void Func_02001d50(struct MarsLighthouseRoomMVector *position, s32 value,
                   s32 flag) {
  struct ObjectRuntime *object = (struct ObjectRuntime *)Value4(
      (s32 (*)())Func_02003128, 366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = value;
    Func_02001cc4(object, 3);
    object->update = Func_02001b64;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_02003118(object, 2);
  }
}

void Func_02001d98(struct MarsLighthouseRoomMVector *position, s32 value,
                   s32 flag) {
  struct ObjectRuntime *object = (struct ObjectRuntime *)Value4(
      (s32 (*)())Func_02003128, 366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -value;
    Func_02001cc4(object, 3);
    object->update = Func_02001b64;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_02003118(object, 2);
  }
}

s32 Func_02001de0(struct ObjectRuntime *a, struct ObjectRuntime *b) {
  s32 height;

  if ((a->x - b->x >= 0 ? a->x - b->x : b->x - a->x) < 0xc0000) {
    height = a->y - b->y;
    if (height > 0 && height < 0x100000) {
      if ((a->z - b->z >= 0 ? a->z - b->z : b->z - a->z) < 0x80000)
        return 1;
    }
  }
  return 0;
}
