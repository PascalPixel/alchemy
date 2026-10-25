#include "types.h"
#include "object_runtime.h"
#include "call.h"

void Func_020041cc(struct ObjectRuntime *object, s32 mode);
struct ObjectRuntime *Func_020041e4();
void Func_020015ec(void);

void Func_02001928(struct ObjectRuntime *object, s32 priority);

struct SpriteParts {
  u8 unk00[9];
  u8 unk09Low : 2;
  u8 part : 2;
};
extern const u8 gData_02004744[];

void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_020041dc(struct ObjectRuntime *object, const void *script);

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_02004744; Func_02001e1c of the Mars Lighthouse entrance is the same
/// function.
void Func_02001928(struct ObjectRuntime *object, s32 part) {
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
  Func_020041dc(object, gData_02004744);
  object->action = zero;
  object->speedLimit = zero;
  object->acceleration = zero;
}

void Func_0200196c(s32 *position, s32 speed, s32 flag) {
  struct ObjectRuntime *object = (struct ObjectRuntime *)Value4(
      (s32 (*)())Func_020041e4, 366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -speed;
    Func_02001928(object, 3);
    object->update = Func_020015ec;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_020041cc(object, 2);
  }
}
