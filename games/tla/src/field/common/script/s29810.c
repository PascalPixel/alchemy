#include "types.h"
#include "field_event.h"

#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))

u32 Random16(void);

void Func_08029810(struct FieldActor *object) {
  u32 first;
  u32 second;
  s32 x;

  HALF_AT(object->sprite, 18) += 0x400;
  first = Random16();
  second = Random16();
  x = object->x;
  /* FAKEMATCH: a one-pass block keeps the position load ahead of the two
   * halvings; as one statement the first halving comes before the load (2
   * differ). */
  do {
  } while (0);
  second >>= 1;
  first >>= 1;
  object->x = x + (first - second);
}
