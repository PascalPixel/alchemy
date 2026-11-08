#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);

struct Half {
  u16 v;
};

void Func_02000884(s32 id, s32 *pos, s32 dir, s32 palette, s32 value) {
  struct Actor *object = Actor_GetById(id);
  u8 *sprite;
  u8 **list;
  s32 i;

  if (object != NULL) {
    struct Half zero;

    object->x = *pos++;
    object->y = *pos++;
    object->z = *pos;
    object->angle = dir;
    zero.v = 0;
    object->motionFlags = zero.v;
    ((u8 *)object->animation)[26] = zero.v;
    Actor_SetAnimationSpeed(object, value);
  }
  sprite = object->animation;
  i = 0;
  if (i >= sprite[27])
    return;
  list = (u8 **)(sprite + 40);
next:
  {
    u8 *cell = *list++;

    if (cell[5] != palette) {
      cell[5] = palette;
      cell[22] = 255;
    }
  }
  i++;
  if (i < sprite[27])
    goto next;
}
