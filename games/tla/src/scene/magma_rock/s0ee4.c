// Unless flag 0x109 is set, places the object at the watched object first +
// side, gives it Func_02000dc0 and faces it along the pair first and first + 1.
#include "types.h"
#include "object_runtime.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void Func_02000dc0(void);

void Func_02000ee4(u16 id, s16 first, s16 side) {
  struct ObjectRuntime *object = Object_GetById(id);
  struct ObjectRuntime *a;
  struct ObjectRuntime *b;
  u16 left;
  u16 right;
  s32 linked;
  s32 next;
  u8 *flags;
  u8 facing;

  if (GameFlag_Test(0x109))
    return;
  left = first;
  right = side;
  linked = left + right;
  object->x = Object_GetById(linked)->x;
  object->y = Object_GetById(linked)->y;
  object->z = Object_GetById(linked)->z;
  Object_SetSpritePriority(object, 2);
  HALF_AT(object, 102) = first;
  /* FAKEMATCH(temporary): the byte is computed before its address; as
   * BYTE_AT(object, 98) = side ^ 1, 6 of 104 instructions differ in tla-en. */
  facing = side ^ 1;
  BYTE_AT(object, 98) = facing;
  object->update = (void *)Func_02000dc0;
  next = left + 1;
  flags = &object->motionFlags;
  if (right != 0) {
    a = Object_GetById(left);
    b = Object_GetById(next);
    if (a->x < b->x)
      object->angle = 0x8000;
    else
      object->angle = 0;
  } else {
    a = Object_GetById(left);
    b = Object_GetById(next);
    if (a->x > b->x)
      object->angle = 0x8000;
    else
      object->angle = 0;
  }
  *flags = 0;
}
