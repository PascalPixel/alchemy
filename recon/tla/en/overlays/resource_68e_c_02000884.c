// Draft of overlay 68e's Func_02000884 (0x02008884), the twin of ☀️'s
// TolbiSpring_PlaceActor: differ 14 of 48. Register choice in the palette
// loop: the native counts in r1, loads the cell count into r3 and walks the
// list in r4; here the count takes r1 and the index r5. The palette needs a
// pin to r8 (the native keeps the facing in r7; as a plain argument the
// palette ranks higher in global.c, 5 refs over 36 insns, and takes r7:
// differ 32). Tried: a for loop (28 pinned, 44 plain; loop.c hoists the 255),
// the index pinned to r1 (16), a u8 palette (33), a (u8) compare (37), five
// minutes of permute (22).
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetAnimationSpeed(struct ObjectRuntime *object,
                                         s32 value);

struct Half {
  u16 v;
};

void Func_02000884(s32 id, s32 *pos, s32 dir, s32 palette, s32 value) {
  /* FAKEMATCH: the native keeps the palette in r8 and the facing in r7 (differ
   * 32 unpinned). */
  register s32 color asm("r8") = palette;
  struct ObjectRuntime *object = Object_GetById(id);
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
    ObjectDispatch_SetAnimationSpeed(object, value);
  }
  sprite = object->animation;
  i = 0;
  if (i >= sprite[27])
    return;
  list = (u8 **)(sprite + 40);
next:
  {
    u8 *cell = *list++;

    if (cell[5] != color) {
      cell[5] = color;
      cell[22] = 255;
    }
  }
  i++;
  if (i < sprite[27])
    goto next;
}
