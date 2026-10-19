#include "scene.h"
#include "map.h"

// For each six-byte record (actor, unused, flag) up to an actor of 0xffff:
// when the record's flag is set, the cell under the actor in map layer 2 takes
// collision code 255.
void Func_02000918(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct FieldActor *object;
  /* FAKEMATCH(pin): the limit in r2, so the next id takes r3; as a plain
   * local, 10 of 31 instructions differ in tla-en. */
  register s32 limit asm("r2");
  s32 id;

  goto next;
  do {
    /* FAKEMATCH(barrier): the record pointer passed through an empty asm, so
     * the id is read again and the pointer outranks the actor for r5; without
     * it, 19 of 31 instructions differ in tla-en. */
    asm("" : "+l"(list));
    object = Object_GetById(list[0]);
    if (GameFlag_Test(list[2]))
      *(u8 *)((u32)map->layers[2].cells + (Func_020041e0(object) << 2) + 2) =
          255;
    list += 3;
  next:
    limit = 0xff;
    id = list[0];
    /* FAKEMATCH(barrier): the limit's shift waits for the id's load; as
     * while (list[0] != 0xffff), 2 of 31 instructions differ in tla-en. */
    asm("" : "+l"(limit) : "l"(id));
  } while (id != (limit << 8) + 0xff);
}

// For each six-byte record up to an actor of 0xffff, the cell under the actor
// in map layer 2 takes collision code 232.
void Func_0200095c(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 index;
  /* FAKEMATCH(pin): the limit in r2, so the next id takes r3; as a plain
   * local, 10 of 25 instructions differ in tla-en. */
  register s32 limit asm("r2");
  s32 id;

  goto next;
  do {
    /* FAKEMATCH(barrier): the id is read again for the call rather than shared
     * with the loop test; without it, 9 of 25 instructions differ in tla-en. */
    asm("" : : : "memory");
    index = Func_020041e0(Object_GetById(*list));
    list += 3;
    *(u8 *)((u32)map->layers[2].cells + (index << 2) + 2) = 232;
  next:
    limit = 0xff;
    id = *list;
    /* FAKEMATCH(barrier): the limit's shift waits for the id's load; as
     * while (*list != 0xffff), 2 of 25 instructions differ in tla-en. */
    asm("" : "+l"(limit) : "l"(id));
  } while (id != (limit << 8) + 0xff);
}
