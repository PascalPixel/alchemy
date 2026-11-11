#include "scene.h"
#include "map.h"

/// For each six-byte record (actor, unused, flag) up to an actor of 0xffff:
/// when the record's flag is set, the cell under the actor in map layer 2 takes
/// collision code 255.
void Func_02000918(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct FieldActor *object;
  /* FAKEMATCH(pin): the limit in r2, so the next id takes r3; as a plain
   * local, 10 of 31 instructions differ in tla-en. */
  register s32 limit asm("r2");

  goto next;
  do {
    /* FAKEMATCH(barrier): the record pointer passed through an empty asm, so
     * the id is read again and the pointer outranks the actor for r5; without
     * it, 19 of 31 instructions differ in tla-en. */
    asm("" : "+l"(list));
    object = Actor_GetById(list[0]);
    if (GameFlag_Test(list[2]))
      *(u8 *)((u32)map->layers[2].cells + (Func_020041e0(object) << 2) + 2) =
          255;
    list += 3;
  next:
    limit = 0xff;
  } while (list[0] != (limit << 8) + 0xff);
}

/// For each six-byte record up to an actor of 0xffff, the cell under the actor
/// in map layer 2 takes collision code 232.
void Func_0200095c(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cell;
  s32 collisionCode;

  if (*list != 0xffff) {
  loop:
    cell = &map->layers[2].cells[Func_020041e0(Actor_GetById(*list))];
    collisionCode = 232;
    /* FAKEMATCH(dead): the advance in a once-only block, so the store waits for
     * it; as list += 3, 2 of 25 instructions differ in tla-en. */
    do {
      list += 3;
    } while (0);
    cell->collisionCode = collisionCode;
    if (*list != 0xffff)
      goto loop;
  }
}
