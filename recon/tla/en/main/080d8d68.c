// Near miss, differ 6 of 75: links two objects into workspace slot `index`
// of heap slot 0x9c: records their ids and the first one's sprite priority
// (2 for a many-part object), builds the link between their positions raised
// 8 pixels (Func_080d8e08), and sets both to mode 2. No ☀️ twin found. Left:
// instruction order only: the native copies id to r7 after loading the
// work, loads the second end's y after building 0x80000 and adds the raise to
// the first y straight after, as in Func_080da1b8.
#include "types.h"
#include "field_sprite.h"
#include "object_runtime.h"
#include "work9c.h"

void Func_080d8e08(s32 index, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2);

void Func_080d8d68(s32 index, s32 id, s32 other) {
  struct Work9c *work = WORK_9C;
  s32 priority = 2;
  struct ObjectRuntime *first = ObjectTable_Get(id);
  struct ObjectRuntime *second = ObjectTable_Get(other);

  if (first != NULL && second != NULL) {
    struct Work9cSlot *slot;
    s32 lift;

    if (first->animationKind == 1)
      priority = ((struct FieldSprite *)first->animation)->priority;
    slot = &work->slots[index];
    slot->id = id;
    slot->value = other;
    *(s16 *)slot->unk06 = priority;
    lift = 128;
    /* FAKEMATCH: starts 0x80000 before the stack arguments load, as the
     * native does (differ 12 without it). */
    asm volatile("" : "+l"(lift));
    lift <<= 12;
    Func_080d8e08(index, first->x, first->y + lift, first->z, second->x,
                  second->y + lift, second->z);
  }
  Object_SetMode(first, 2);
  Object_SetMode(second, 2);
}
