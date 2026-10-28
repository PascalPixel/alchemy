// Unless flag 0x109 is set, places the object at the watched object first +
// side, gives it Func_02000dc0 and faces it along the pair first and first + 1.
#include "types.h"
#include "actor.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Func_02000dc0(void);

void Func_02000ee4(u16 id, s16 first, s16 side) {
  struct Actor *object = Actor_GetById(id);
  struct Actor *a;
  struct Actor *b;
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
  object->x = Actor_GetById(linked)->x;
  object->y = Actor_GetById(linked)->y;
  object->z = Actor_GetById(linked)->z;
  Scene_SetActorSpritePriority(object, 2);
  HALF_AT(object, 102) = first;
  /* FAKEMATCH(temporary): the byte is computed before its address; as
   * BYTE_AT(object, 98) = side ^ 1, 6 of 104 instructions differ in tla-en. */
  facing = side ^ 1;
  BYTE_AT(object, 98) = facing;
  object->update = (void *)Func_02000dc0;
  next = left + 1;
  flags = &object->motionFlags;
  if (right != 0) {
    a = Actor_GetById(left);
    b = Actor_GetById(next);
    if (a->x < b->x)
      object->angle = 0x8000;
    else
      object->angle = 0;
  } else {
    a = Actor_GetById(left);
    b = Actor_GetById(next);
    if (a->x > b->x)
      object->angle = 0x8000;
    else
      object->angle = 0;
  }
  *flags = 0;
}
